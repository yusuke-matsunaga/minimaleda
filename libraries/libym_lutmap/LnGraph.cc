
/// @file libym_lutmap/LnGraph.cc
/// @brief LutNetwork の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: LutNetwork.cc 2274 2009-06-10 07:45:29Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_lutmap/LnGraph.h"


BEGIN_NAMESPACE_YM_LUTMAP

///////////////////////////////////////////////////////////////////////
// クラス LnNode
///////////////////////////////////////////////////////////////////////

// コンストラクタ
LnNode::LnNode() :
  mFlags(0),
  mNi(0),
  mFanins(NULL),
  mLevel(0)
{
}

// デストラクタ
LnNode::~LnNode()
{
}

// @brief ファンアウトに出力が含まれているか調べ pomark をセットする．
void
LnNode::scan_po()
{
  mFlags &= ~kPoMask;
  for (LnEdgeList::iterator p = mFanoutList.begin();
       p != mFanoutList.end(); ++ p) {
    LnEdge* e = *p;
    LnNode* to = e->to();
    if ( to->is_ppo() ) {
      mFlags |= kPoMask;
      break;
    }
  }
}

// @brief ID を表す文字列の取得
string
LnNode::id_str() const
{
  ostringstream buf;
  buf << id();
  return buf.str();
}


//////////////////////////////////////////////////////////////////////
// クラス LnPort
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] name 名前
// @param[in] io_node_vec 対応する入出力ノードのベクタ
LnPort::LnPort(const string& name,
	       const vector<LnNode*>& io_node_vec) :
  mName(name),
  mBody(io_node_vec)
{
}

// @brief デストラクタ
LnPort::~LnPort()
{
}


///////////////////////////////////////////////////////////////////////
// クラス LnGraph
///////////////////////////////////////////////////////////////////////

// コンストラクタ
LnGraph::LnGraph() :
  mAlloc(4096),
  mAlloc2(4096),
  mLevel(0),
  mLevelValid(false)
{
}

// コピーコンストラクタ
LnGraph::LnGraph(const LnGraph& src) :
  mAlloc(4096),
  mAlloc2(4096)
{
  vector<LnNode*> nodemap;
  copy(src, nodemap);
}

// 代入演算子
const LnGraph&
LnGraph::operator=(const LnGraph& src)
{
  if ( &src != this ) {
    clear();
    vector<LnNode*> nodemap;
    copy(src, nodemap);
  }
  return *this;
}

// デストラクタ
LnGraph::~LnGraph()
{
  clear();
}

// 複製する．
void
LnGraph::copy(const LnGraph& src,
	      vector<LnNode*>& nodemap)
{
  mName = src.mName;

  ymuint n = src.max_node_id();
  nodemap.clear();
  nodemap.resize(n);

  // 外部入力の生成
  const LnNodeList& input_list = src.input_list();
  for (LnNodeList::const_iterator p = input_list.begin();
       p != input_list.end(); ++ p) {
    LnNode* src_node = *p;
    LnNode* dst_node = new_input();
    nodemap[src_node->id()] = dst_node;
  }

  // DFFノードの生成
  const LnNodeList& dff_list = src.dff_list();
  for (LnNodeList::const_iterator p = dff_list.begin();
       p != dff_list.end(); ++ p) {
    LnNode* src_node = *p;
    LnNode* dst_node = new_dff();
    nodemap[src_node->id()] = dst_node;
  }

  // LUTノードの生成
  vector<LnNode*> node_list;
  src.sort(node_list);
  ymuint nl = node_list.size();
  for (ymuint i = 0; i < nl; ++ i) {
    LnNode* src_node = node_list[i];
    ymuint ni = src_node->fanin_num();
    vector<LnNode*> dst_inputs(ni);
    vector<int> tv;
    for (ymuint j = 0; j < ni; ++ j) {
      LnNode* src_inode = src_node->fanin(j);
      LnNode* input = nodemap[src_inode->id()];
      assert_cond(input, __FILE__, __LINE__);
      dst_inputs[j] = input;
    }
    src_node->tv(tv);

    LnNode* dst_node = new_lut(dst_inputs, tv);
    if ( src_node->pomark() ) {
      dst_node->mFlags |= LnNode::kPoMask;
    }
    nodemap[src_node->id()] = dst_node;
  }

  // DFFノードの入力の接続
  for (LnNodeList::const_iterator p = dff_list.begin();
       p != dff_list.end(); ++ p) {
    LnNode* src_node = *p;
    LnNode* dst_node = nodemap[src_node->id()];
    LnNode* src_inode = src_node->fanin(0);
    LnNode* dst_inode = nodemap[src_inode->id()];
    set_dff_input(dst_node, dst_inode);
  }

  // 外部出力の生成
  const LnNodeList& output_list = src.output_list();
  for (LnNodeList::const_iterator p = output_list.begin();
       p != output_list.end(); ++ p) {
    LnNode* src_onode = *p;
    LnNode* src_inode = src_onode->fanin(0);
    LnNode* dst_inode = NULL;
    if ( src_inode ) {
      dst_inode = nodemap[src_inode->id()];
    }
    (void) new_output(dst_inode);
  }

  // ポートの複製
  ymuint np = src.port_num();
  for (ymuint i = 0; i < np; ++ i) {
    const LnPort* src_port = src.port(i);
    ymuint nb = src_port->bit_width();
    vector<LnNode*> tmp(nb);
    for (ymuint j = 0; j < nb; ++ j) {
      const LnNode* src_node = src_port->bit(j);
      LnNode* dst_node = nodemap[src_node->id()];
      tmp[j] = dst_node;
    }
    add_port(src_port->name(), tmp);
  }
}

// @brief 入出力ノードに関連づけられたポートを得る．
// @param[in] node 入出力ノード
const LnPort*
LnGraph::port(const LnNode* node) const
{
  if ( node->is_input() ) {
    return mInputPortArray[node->subid()].mPort;
  }
  else if ( node->is_output() ) {
    return mOutputPortArray[node->subid()].mPort;
  }
  else {
    return NULL;
  }
}

// @brief 入出力ノードのポートにおけるビット位置を得る．
// @param[in] node 入出力ノード
ymuint
LnGraph::port_pos(const LnNode* node) const
{
  if ( node->is_input() ) {
    return mInputPortArray[node->subid()].mPos;
  }
  else if ( node->is_output() ) {
    return mOutputPortArray[node->subid()].mPos;
  }
  else {
    return NULL;
  }
}

// @brief 入力ノードと DFF ノードのリストを得る．
// @param[out] node_list ノードを格納するリスト
// @return 要素数を返す．
ymuint
LnGraph::ppi_list(list<LnNode*>& node_list) const
{
  node_list.clear();
  for (LnNodeList::const_iterator p = input_list().begin();
       p != input_list().end(); ++ p) {
    node_list.push_back(*p);
  }
  for (LnNodeList::const_iterator p = dff_list().begin();
       p != dff_list().end(); ++ p) {
    node_list.push_back(*p);
  }
  return node_list.size();
}

// @brief 出力ノードと DFF ノードのリストを得る．
// @param[out] node_list ノードを格納するリスト
// @return 要素数を返す．
ymuint
LnGraph::ppo_list(list<LnNode*>& node_list) const
{
  node_list.clear();
  for (LnNodeList::const_iterator p = output_list().begin();
       p != output_list().end(); ++ p) {
    node_list.push_back(*p);
  }
  for (LnNodeList::const_iterator p = dff_list().begin();
       p != dff_list().end(); ++ p) {
    node_list.push_back(*p);
  }
  return node_list.size();
}


BEGIN_NONAMESPACE

// node のファンアウトのうちファンインがすべてマークされているノード
// をリストに加える．
void
sort_sub(LnNode* node,
	 vector<bool>& mark,
	 vector<LnNode*>& node_list)
{
  const LnEdgeList& fo_list = node->fanout_list();
  for (LnEdgeList::const_iterator p = fo_list.begin();
       p != fo_list.end(); ++ p) {
    LnEdge* e = *p;
    LnNode* onode = e->to();
    if ( mark[onode->id()] || !onode->is_lut() ) continue;
    bool ready = true;
    ymuint ni = onode->fanin_num();
    for (ymuint i = 0; i < ni; ++ i) {
      LnNode* inode0 = onode->fanin(i);
      if ( !mark[inode0->id()] ) {
	ready = false;
	break;
      }
    }
    if ( ready ) {
      mark[onode->id()] = true;
      node_list.push_back(onode);
    }
  }
}

END_NONAMESPACE

// @brief ソートされたノードのリストを得る．
void
LnGraph::sort(vector<LnNode*>& node_list) const
{
  node_list.clear();
  node_list.reserve(lnode_num());

  ymuint n = max_node_id();
  vector<bool> mark(n, false);

  // 外部入力とDFFのみをファンインにするノードを node_list に追加する．
  list<LnNode*> tmp_list;
  ppi_list(tmp_list);
  for (list<LnNode*>::const_iterator p = tmp_list.begin();
       p != tmp_list.end(); ++ p) {
    LnNode* node = *p;
    mark[node->id()] = true;
    sort_sub(node, mark, node_list);
  }

  // 入力を持たないノード(定数ノード)を node_list に追加する．
  for (LnNodeList::const_iterator p = mLutList.begin();
       p != mLutList.end(); ++ p) {
    LnNode* node = *p;
    if ( node->fanin_num() == 0 && !mark[node->id()] ) {
      mark[node->id()] = true;
      node_list.push_back(node);
    }
  }

  ymuint rpos = 0;
  while ( rpos < node_list.size() ) {
    LnNode* node = node_list[rpos];
    ++ rpos;
    sort_sub(node, mark, node_list);
  }
  assert_cond(node_list.size() == lnode_num(), __FILE__, __LINE__);
}

// @brief モジュール名を設定する．
void
LnGraph::set_name(const string& name)
{
  mName = name;
}

// @brief ポートを追加する(ベクタ版)．
// @param[in] name ポート名
// @param[in] io_node_vec 対応する入出力ノードのベクタ
void
LnGraph::add_port(const string& name,
		  const vector<LnNode*>& io_node_vec)
{
  LnPort* port = new LnPort(name, io_node_vec);
  mPortArray.push_back(port);
  ymuint n = io_node_vec.size();
  for (ymuint i = 0; i < n; ++ i) {
    LnNode* node = io_node_vec[i];
    if ( node->is_input() ) {
      PortInfo& port_info = mInputPortArray[node->subid()];
      port_info.mPort = port;
      port_info.mPos = i;
    }
    else if ( node->is_output() ) {
      PortInfo& port_info = mOutputPortArray[node->subid()];
      port_info.mPort = port;
      port_info.mPos = i;
    }
    else {
      assert_not_reached(__FILE__, __LINE__);
    }
  }
}

// @brief 入力ノードを作る．
// @return 作成したノードを返す．
LnNode*
LnGraph::new_input()
{
  LnNode* node = new_node(0);

  // 入力ノード配列に登録
  ymuint subid = mInputArray.size();
  mInputArray.push_back(node);
  // ダミーの place-holder を追加する．
  mInputPortArray.push_back(PortInfo());

  // 入力リストに登録
  mInputList.push_back(node);

  node->set_input(subid);

  node->mLevel = 0;

  return node;
}

// @brief 出力ノードを作る．
// @param[in] inode 入力のノード
// @return 作成したノードを返す．
LnNode*
LnGraph::new_output(LnNode* inode)
{
  LnNode* node = new_node(1);

  // 出力ノード配列に登録
  ymuint subid = mOutputArray.size();
  mOutputArray.push_back(node);
  // ダミーの place-holder を追加する．
  mOutputPortArray.push_back(PortInfo());

  // 出力リストに登録
  mOutputList.push_back(node);

  node->set_output(subid);
  connect(inode, node, 0);

  return node;
}

// @brief LUTノードを作る．
// @param[in] inodes 入力ノードのベクタ
// @param[in] tv 真理値ベクタ
LnNode*
LnGraph::new_lut(const vector<LnNode*>& inodes,
		 const vector<int>& tv)
{
  ymuint ni = inodes.size();
  LnNode* node = new_node(ni);

  // LUTノードリストに登録
  mLutList.push_back(node);

  // LUTノードタイプに設定
  node->set_lut();

  // 真理値ベクタを設定
  assert_cond(tv.size() == (1U << ni), __FILE__, __LINE__);
  node->mTv = tv;

  // ファンインの設定
  for (ymuint i = 0; i < ni; ++ i) {
    connect(inodes[i], node, i);
  }

  return node;
}

// @brief DFFノードを作る．
// @return 作成したノードを返す．
LnNode*
LnGraph::new_dff()
{
  LnNode* node = new_node(4);

  // DFFノードリストに登録
  mDffList.push_back(node);

  // DFFノードタイプに設定
  node->set_dff();

  return node;
}

// @brief DFFノードの入力を設定する．
// @param[in] node 対象の DFF ノード
// @param[in] inode 入力のノード
void
LnGraph::set_dff_input(LnNode* node,
		       LnNode* inode)
{
  assert_cond(node->is_dff(), __FILE__, __LINE__);

  connect(inode, node, 0);
}

// @brief DFFノードのクロック入力を設定する．
// @param[in] node 対象の DFF ノード
// @param[in] inode 入力のノード
void
LnGraph::set_dff_clock(LnNode* node,
		       LnNode* inode)
{
  assert_cond(node->is_dff(), __FILE__, __LINE__);

  connect(inode, node, 1);
}

// @brief DFFノードのセット入力を設定する．
// @param[in] node 対象の DFF ノード
// @param[in] inode 入力のノード
void
LnGraph::set_dff_set(LnNode* node,
		     LnNode* inode)
{
  assert_cond(node->is_dff(), __FILE__, __LINE__);

  connect(inode, node, 2);
}

// @brief DFFノードのリセット入力を設定する．
// @param[in] node 対象の DFF ノード
// @param[in] inode 入力のノード
void
LnGraph::set_dff_rst(LnNode* node,
		     LnNode* inode)
{
  assert_cond(node->is_dff(), __FILE__, __LINE__);

  connect(inode, node, 3);
}

// 新しいノードを作成する．
// 作成されたノードを返す．
LnNode*
LnGraph::new_node(ymuint ni)
{
  LnNode* node = NULL;

  // 空いているIDを探してノード配列へ登録
  int id = mItvlMgr.avail_num();
  mItvlMgr.erase(id);

  ymuint uid = static_cast<ymuint>(id);
  while( mNodeArray.size() <= uid ) {
    // mNodeArray の大きさが小さいときのための埋め草
    void* p = mAlloc.get_memory(sizeof(LnNode));
    LnNode* node = new (p) LnNode;
    mNodeArray.push_back(node);
  }
  node = mNodeArray[uid];
  node->mNi = ni;
  if ( ni ) {
    void* q = mAlloc2.get_memory(sizeof(LnEdge) * ni);
    node->mFanins = new (q) LnEdge[ni];
    for (ymuint i = 0; i < ni; ++ i) {
      node->mFanins[i].set_to(node, i);
    }
  }
  else {
    node->mFanins = NULL;
  }
  node->mId = uid;

  return node;
}

// 空にする．
void
LnGraph::clear()
{
  mName.clear();

  // ポートを削除する．
  for (vector<LnPort*>::iterator p = mPortArray.begin();
       p != mPortArray.end(); ++ p) {
    LnPort* port = *p;
    delete port;
  }
  mPortArray.clear();

  // まず最初に接続を切る．
  for (LnNodeList::iterator p = mOutputList.begin();
       p != mOutputList.end(); ++ p) {
    LnNode* node = *p;
    connect(NULL, node, 0);
  }
  for (LnNodeList::iterator p = mLutList.begin();
       p != mLutList.end(); ++ p) {
    LnNode* node = *p;
    ymuint ni = node->fanin_num();
    for (ymuint i = 0; i < ni; ++ i) {
      connect(NULL, node, i);
    }
  }
  for (LnNodeList::iterator p = mDffList.begin();
       p != mDffList.end(); ++ p) {
    LnNode* node = *p;
    for (ymuint i = 0; i < 4; ++ i) {
      connect(NULL, node, i);
    }
  }

  for (LnNodeList::iterator p = mInputList.begin();
       p != mInputList.end(); ) {
    // p は実際にはノード内のメンバをアクセスするので delete する前に
    // 進めておく必要がある．
    LnNode* node = *p;
    ++ p;
    delete_input(node);
  }
  assert_cond(mInputList.empty(), __FILE__, __LINE__);

  for (LnNodeList::iterator p = mOutputList.begin();
       p != mOutputList.end(); ) {
    // p は実際にはノード内のメンバをアクセスするので delete する前に
    // 進めておく必要がある．
    LnNode* node = *p;
    ++ p;
    delete_output(node);
  }
  assert_cond(mOutputList.empty(), __FILE__, __LINE__);

  for (LnNodeList::iterator p = mLutList.begin();
       p != mLutList.end(); ) {
    // p は実際にはノード内のメンバをアクセスするので delete する前に
    // 進めておく必要がある．
    LnNode* node = *p;
    ++ p;
    delete_lut(node);
  }
  assert_cond(mLutList.empty(), __FILE__, __LINE__);

  for (LnNodeList::iterator p = mDffList.begin();
       p != mDffList.end(); ) {
    // p は実際にはノード内のメンバをアクセスするので delete する前に
    // 進めておく必要がある．
    LnNode* node = *p;
    ++ p;
    delete_dff(node);
  }
  assert_cond(mDffList.empty(), __FILE__, __LINE__);

  mInputArray.clear();
  mOutputArray.clear();
}

// 入力ノードの削除
void
LnGraph::delete_input(LnNode* node)
{
  assert_cond(node->is_input(), __FILE__, __LINE__);
  mInputList.erase(node);
  delete_node(node);
}

// 出力ノードの削除
void
LnGraph::delete_output(LnNode* node)
{
  assert_cond(node->is_output(), __FILE__, __LINE__);
  mOutputList.erase(node);
  delete_node(node);

  mLevel = 0;
  mLevelValid = false;
}

// LUTノードの削除
void
LnGraph::delete_lut(LnNode* node)
{
  assert_cond(node->is_lut(), __FILE__, __LINE__);
  assert_cond(node->fanout_num() == 0, __FILE__, __LINE__);
  ymuint ni = node->fanin_num();
  for (ymuint i = 0; i < ni; ++ i) {
    connect(NULL, node, i);
  }

  mLutList.erase(node);
  delete_node(node);
}

// @brief DFFノードを削除する．
// @param[in] node 対象のノード
// @note node のファンアウトは空でなければならない．
void
LnGraph::delete_dff(LnNode* node)
{
  assert_cond(node->is_dff(), __FILE__, __LINE__);
  assert_cond(node->fanout_num() == 0, __FILE__, __LINE__);
  connect(NULL, node, 0);

  mDffList.erase(node);
  delete_node(node);
}

// node を削除する．
void
LnGraph::delete_node(LnNode* node)
{
  // new_node の逆の処理を行なう．
  mItvlMgr.add(static_cast<int>(node->mId));
  // 本当は LnEdge のデストラクタを起動する必要があるが中身がないので
  // 省略する．
  if ( node->fanin_num() ) {
    mAlloc2.put_memory(sizeof(LnEdge)* node->fanin_num(), node->mFanins);
  }
}

// from を to の pos 番目のファンインとする．
// to の pos 番目にすでに接続があった場合には自動的に削除される．
void
LnGraph::connect(LnNode* from,
		 LnNode* to,
		 ymuint pos)
{
  // LnNode::mFaoutList を変更するのはここだけ

  LnEdge* edge = to->fanin_edge(pos);
  LnNode* old_from = edge->from();
  if ( old_from ) {
    old_from->mFanoutList.erase(edge);
    old_from->scan_po();
  }
  edge->set_from(from);
  if ( from ) {
    from->mFanoutList.push_back(edge);
    from->scan_po();
  }

  mLevel = 0;
  mLevelValid = false;
}

// @brief 段数を求める．
ymuint
LnGraph::level() const
{
  if ( !mLevelValid ) {
    list<LnNode*> tmp_list;
    ppi_list(tmp_list);
    for (list<LnNode*>::const_iterator p = tmp_list.begin();
	 p != tmp_list.end(); ++ p) {
      LnNode* node = *p;
      node->mLevel = 0;
    }

    vector<LnNode*> node_list;
    sort(node_list);
    for (vector<LnNode*>::const_iterator p = node_list.begin();
	 p != node_list.end(); ++ p) {
      LnNode* node = *p;
      ymuint l = 0;
      ymuint ni = node->fanin_num();
      for (ymuint i = 0; i < ni; ++ i) {
	LnNode* inode = node->fanin(i);
	ymuint l1 = inode->mLevel;
	if ( l < l1 ) {
	  l = l1;
	}
      }
      node->mLevel = l + 1;
    }

    ymuint max_l = 0;
    ppo_list(tmp_list);
    for (list<LnNode*>::const_iterator p = tmp_list.begin();
	 p != tmp_list.end(); ++ p) {
      LnNode* node = *p;
      LnNode* inode = node->fanin(0);
      if ( inode ) {
	ymuint l1 = inode->mLevel;
	if ( max_l < l1 ) {
	  max_l = l1;
	}
      }
    }
    mLevel = max_l;
    mLevelValid = true;
  }
  return mLevel;
}

END_NAMESPACE_YM_LUTMAP
