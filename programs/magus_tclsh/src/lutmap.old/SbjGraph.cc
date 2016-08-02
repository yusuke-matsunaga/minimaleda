
/// @file magus/lutmap/SbjGraph.cc 
/// @brief SbjGraph の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: SbjGraph.cc 2274 2009-06-10 07:45:29Z matsunaga $
///
/// Copyright (C) 2005-2006 Yusuke Matsunaga
/// All rights reserved.


#if HAVE_CONFIG_H
#include "magus_config.h"
#endif

#include "SbjGraph.h"


BEGIN_NAMESPACE_MAGUS_LUTMAP

///////////////////////////////////////////////////////////////////////
// クラス SbjNode
///////////////////////////////////////////////////////////////////////

// コンストラクタ
SbjNode::SbjNode() :
  mImmDom(NULL),
  mMark(0),
  mDepth(0),
  mLevel(0)
{
  mFanins[0].set_to(this, 0);
  mFanins[1].set_to(this, 1);
}
      
// デストラクタ
SbjNode::~SbjNode()
{
}

// @brief 実効的なファンアウト数を計算する．
void
SbjNode::scan_efo(hash_set<ymuint>& mark) const
{
  if ( !is_unselected() ) {
    mark.insert(id());
    return;
  }
  for (SbjEdgeList::const_iterator p = mFanoutList.begin();
       p != mFanoutList.end(); ++ p) {
    SbjEdge* e = *p;
    SbjNode* fonode = e->to();
    fonode->scan_efo(mark);
  }
}

// @brief ファンアウトに出力が含まれているか調べ pomark をセットする．
void
SbjNode::scan_po()
{
  mMark &= ~kPoMask;
  for (SbjEdgeList::iterator p = mFanoutList.begin();
       p != mFanoutList.end(); ++ p) {
    SbjEdge* e = *p;
    if ( e->to()->is_output() ) {
      mMark |= kPoMask;
      break;
    }
  }
}

// @brief ID を表す文字列の取得
string
SbjNode::id_str() const
{
  ostringstream buf;
  if ( is_input() ) {
    buf << "I" << id();
  }
  else if ( is_logic() ) {
    buf << "L" << id();
  }
  else if ( is_output() ) {
    buf << "O" << id();
  }
  else {
    buf << "X" << id();
  }
  return buf.str();
}


///////////////////////////////////////////////////////////////////////
// クラス SbjGraph
///////////////////////////////////////////////////////////////////////

// コンストラクタ
SbjGraph::SbjGraph() :
  mAlloc(4096),
  mLevel(0),
  mLevelValid(false)
{
}

// コピーコンストラクタ
SbjGraph::SbjGraph(const SbjGraph& src) :
  mAlloc(4096)
{
  vector<SbjNode*> nodemap;
  copy(src, nodemap);
}

// 代入演算子
const SbjGraph&
SbjGraph::operator=(const SbjGraph& src)
{
  if ( &src != this ) {
    clear();
    vector<SbjNode*> nodemap;
    copy(src, nodemap);
  }
  return *this;
}

// デストラクタ
SbjGraph::~SbjGraph()
{
  clear();
}

// 複製する．
void
SbjGraph::copy(const SbjGraph& src,
	       vector<SbjNode*>& nodemap)
{
  ymuint n = src.max_node_id();
  nodemap.clear();
  nodemap.resize(n);
  
  // 外部入力の生成
  const SbjNodeList& input_list = src.input_list();
  for (SbjNodeList::const_iterator p = input_list.begin();
       p != input_list.end(); ++ p) {
    SbjNode* src_node = *p;
    SbjNode* dst_node = new_input(src_node->name());
    nodemap[src_node->id()] = dst_node;
  }

  // 論理ノードの生成
  vector<SbjNode*> node_list;
  src.sort(node_list);
  ymuint nl = node_list.size();
  for (ymuint i = 0; i < nl; ++ i) {
    SbjNode* src_node = node_list[i];

    SbjNode* src_inode0 = src_node->fanin(0);
    SbjNode* input0 = nodemap[src_inode0->id()];
    assert_cond(input0, __FILE__, __LINE__);

    SbjNode* src_inode1 = src_node->fanin(1);
    SbjNode* input1 = nodemap[src_inode1->id()];
    assert_cond(input1, __FILE__, __LINE__);

    SbjNode* dst_node = new_logic(src_node->name(),
				  src_node->fcode(),
				  input0, input1);
    if ( src_node->mImmDom ) {
      dst_node->mImmDom = nodemap[src_node->mImmDom->id()];
    }
    dst_node->mMark = src_node->mMark;
    nodemap[src_node->id()] = dst_node;
  }

  // 外部出力の生成
  const SbjNodeList& output_list = src.output_list();
  for (SbjNodeList::const_iterator p = output_list.begin();
       p != output_list.end(); ++ p) {
    SbjNode* src_onode = *p;
    SbjNode* src_inode = src_onode->fanin(0);
    const string& name = src_onode->name();
    SbjNode* dst_inode = NULL;
    if ( src_inode ) {
      dst_inode = nodemap[src_inode->id()];
    }
    (void) new_output(name, dst_inode, src_onode->output_inv());
  }
}

// 空にする．
void
SbjGraph::clear()
{
  // まず最初に接続を切る．
  for (SbjNodeList::iterator p = mOutputList.begin();
       p != mOutputList.end(); ++ p) {
    SbjNode* node = *p;
    connect(NULL, node, 0);
  }
  for (SbjNodeList::iterator p = mLnodeList.begin();
       p != mLnodeList.end(); ++ p) {
    SbjNode* node = *p;
    connect(NULL, node, 0);
    connect(NULL, node, 1);
  }

  for (SbjNodeList::iterator p = mInputList.begin();
       p != mInputList.end(); ) {
    // p は実際にはノード内のメンバをアクセスするので delete する前に
    // 進めておく必要がある．
    SbjNode* node = *p;
    ++ p;
    delete_input(node);
  }
  assert_cond(mInputList.empty(), __FILE__, __LINE__);

  for (SbjNodeList::iterator p = mOutputList.begin();
       p != mOutputList.end(); ) {
    // p は実際にはノード内のメンバをアクセスするので delete する前に
    // 進めておく必要がある．
    SbjNode* node = *p;
    ++ p;
    delete_output(node);
  }
  assert_cond(mOutputList.empty(), __FILE__, __LINE__);

  for (SbjNodeList::iterator p = mLnodeList.begin();
       p != mLnodeList.end(); ) {
    // p は実際にはノード内のメンバをアクセスするので delete する前に
    // 進めておく必要がある．
    SbjNode* node = *p;
    ++ p;
    delete_logic(node);
  }
  assert_cond(mLnodeList.empty(), __FILE__, __LINE__);

  mInputArray.clear();
  mOutputArray.clear();
}

// @brief ソートされたノードのリストを得る．
void
SbjGraph::sort(vector<SbjNode*>& node_list) const
{
  node_list.clear();
  node_list.reserve(n_lnodes());

  ymuint rpos = 0;
  
  // 外部入力のみをファンインにするノードを node_list に追加する．
  for (SbjNodeList::const_iterator p = mInputList.begin();
       p != mInputList.end(); ++ p) {
    SbjNode* node = *p;
    node->set_umark();
    const SbjEdgeList& fo_list = node->fanout_list();
    for (SbjEdgeList::const_iterator p = fo_list.begin();
	 p != fo_list.end(); ++ p) {
      SbjEdge* e = *p;
      SbjNode* onode = e->to();
      if ( onode->umark() || onode->is_output() ) continue;
      bool ready = true;
      for (ymuint i = 0; i < 2; ++ i) {
	SbjNode* inode0 = onode->fanin(i);
	if ( !inode0->umark() ) {
	  ready = false;
	  break;
	}
      }
      if ( ready ) {
	onode->set_umark();
	node_list.push_back(onode);
      }
    }
  }

  while ( rpos < node_list.size() ) {
    SbjNode* node = node_list[rpos];
    ++ rpos;
    const SbjEdgeList& fo_list = node->fanout_list();
    for (SbjEdgeList::const_iterator p = fo_list.begin();
	 p != fo_list.end(); ++ p) {
      SbjEdge* e = *p;
      SbjNode* onode = e->to();
      if ( onode->umark() || onode->is_output() ) continue;
      bool ready = true;
      for (ymuint i = 0; i < 2; ++ i) {
	SbjNode* inode0 = onode->fanin(i);
	if ( !inode0->umark() ) {
	  ready = false;
	  break;
	}
      }
      if ( ready ) {
	onode->set_umark();
	node_list.push_back(onode);
      }
    }
  }
  assert_cond(node_list.size() == n_lnodes(), __FILE__, __LINE__);

  for (SbjNodeList::const_iterator p = mInputList.begin();
       p != mInputList.end(); ++ p) {
    SbjNode* node = *p;
    node->clear_umark();
  }
  for (vector<SbjNode*>::iterator p = node_list.begin();
       p != node_list.end(); ++ p) {
    SbjNode* node = *p;
    node->clear_umark();
  }
}

// @brief 逆順でソートされたノードのリストを得る．
void
SbjGraph::rsort(vector<SbjNode*>& node_list) const
{
  node_list.clear();
  node_list.reserve(n_lnodes());

  ymuint rpos = 0;

  // 外部出力のみをファンアウトにするノードを node_list に追加する．
  for (SbjNodeList::const_iterator p = mOutputList.begin();
       p != mOutputList.end(); ++ p) {
    SbjNode* node = *p;
    node->set_umark();
    SbjNode* inode = node->fanin(0);
    if ( inode == NULL || inode->umark() || inode->is_input() ) continue;
    const SbjEdgeList& fo_list = inode->fanout_list();
    bool ready = true;
    for (SbjEdgeList::const_iterator p = fo_list.begin();
	 p != fo_list.end(); ++ p) {
      SbjEdge* e = *p;
      SbjNode* onode = e->to();
      if ( !onode->umark() ) {
	ready = false;
	break;
      }
    }
    if ( ready ) {
      inode->set_umark();
      node_list.push_back(inode);
    }
  }

  // node_list からノードを取り出し，同様の処理を行う．
  while ( rpos < node_list.size() ) {
    SbjNode* node = node_list[rpos];
    ++ rpos;
    for (ymuint i = 0; i < 2; ++ i) {
      SbjNode* inode = node->fanin(i);
      if ( inode->umark() || inode->is_input() ) continue;
      const SbjEdgeList& fo_list = inode->fanout_list();
      bool ready = true;
      for (SbjEdgeList::const_iterator p = fo_list.begin();
	   p != fo_list.end(); ++ p) {
	SbjEdge* e = *p;
	SbjNode* onode = e->to();
	if ( !onode->umark() ) {
	  ready = false;
	  break;
	}
      }
      if ( ready ) {
	inode->set_umark();
	node_list.push_back(inode);
      }
    }
  }
  assert_cond(node_list.size() == n_lnodes(), __FILE__, __LINE__);

  for (SbjNodeList::const_iterator p = mOutputList.begin();
       p != mOutputList.end(); ++ p) {
    SbjNode* node = *p;
    node->clear_umark();
  }
  for (vector<SbjNode*>::iterator p = node_list.begin();
       p != node_list.end(); ++ p) {
    SbjNode* node = *p;
    node->clear_umark();
  }
}

// 入力ノードを作る．
SbjNode*
SbjGraph::new_input(const string& name)
{
  SbjNode* node = new_node(name);
  
  // 入力ノード配列に登録
  ymuint subid = mInputArray.size();
  mInputArray.push_back(node);

  // 入力リストに登録
  mInputList.push_back(node);

  node->set_input(subid);

  node->mLevel = 0;
  
  return node;
}

// 出力ノードを作る．
SbjNode*
SbjGraph::new_output(const string& name,
		     SbjNode* inode,
		     bool inv)
{
  SbjNode* node = new_node(name);
  
  // 出力ノード配列に登録
  ymuint subid = mOutputArray.size();
  mOutputArray.push_back(node);

  // 出力リストに登録
  mOutputList.push_back(node);

  node->set_output(subid, inv);
  connect(inode, node, 0);

  return node;
}

// 論理ノードを作る．
SbjNode*
SbjGraph::new_logic(const string& name,
		    ymuint fcode,
		    SbjNode* inode0,
		    SbjNode* inode1)
{
  SbjNode* node = new_node(name);

  // 論理ノードリストに登録
  mLnodeList.push_back(node);

  node->set_logic(fcode);
  connect(inode0, node, 0);
  connect(inode1, node, 1);
  
  return node;
}

// 入力ノードの削除
void
SbjGraph::delete_input(SbjNode* node)
{
  assert_cond(node->is_input(), __FILE__, __LINE__);
  mInputList.erase(node);
  delete_node(node);
}

// 出力ノードの削除
void
SbjGraph::delete_output(SbjNode* node)
{
  assert_cond(node->is_output(), __FILE__, __LINE__);
  mOutputList.erase(node);
  delete_node(node);

  mLevel = 0;
  mLevelValid = false;
}

// 論理ノードの削除
void
SbjGraph::delete_logic(SbjNode* node)
{
  assert_cond(node->is_logic(), __FILE__, __LINE__);
  assert_cond(node->n_fanout() == 0, __FILE__, __LINE__);
  connect(NULL, node, 0);
  connect(NULL, node, 1);

  mLnodeList.erase(node);
  delete_node(node);
}

// 新しいノードを作成する．
// 作成されたノードを返す．
SbjNode*
SbjGraph::new_node(const string& name)
{
  SbjNode* node = NULL;

  // 空いているIDを探してノード配列へ登録
  int id = mItvlMgr.avail_num();
  mItvlMgr.erase(id);

  ymuint uid = static_cast<ymuint>(id);
  while( mNodeArray.size() <= uid ) {
    // mNodeArray の大きさが小さいときのための埋め草
    void* p = mAlloc.get_memory(sizeof(SbjNode));
    SbjNode* node = new (p) SbjNode;
    mNodeArray.push_back(node);
  }
  node = mNodeArray[uid];
  node->mId = uid;
  node->mName = name;
  node->mImmDom = NULL;
  node->mMark = 0;

  return node;
}

// node を削除する．
void
SbjGraph::delete_node(SbjNode* node)
{
  // new_node の逆の処理を行なう．
  mItvlMgr.add(static_cast<int>(node->mId));
}
    
// @brief 出力ノードの内容を変更する
// @param[in] 変更対象の出力ノード
// @param[in] inode 入力のノード
void
SbjGraph::change_output(SbjNode* node,
			SbjNode* inode,
			bool inv)
{
  node->set_output(node->subid(), inv);
  connect(inode, node, 0);
}

// @brief 論理ノードの内容を再設定する．
// @param[in] node 変更対象の論理ノード
// @param[in] fcode 機能コード
// @param[in] inode1 1番めの入力ノード
// @param[in] inode2 2番めの入力ノード
void
SbjGraph::change_logic(SbjNode* node,
		       ymuint fcode,
		       SbjNode* inode1,
		       SbjNode* inode2)
{
  node->set_logic(fcode);
  connect(inode1, node, 0);
  connect(inode2, node, 1);
}

// from を to の pos 番目のファンインとする．
// to の pos 番目にすでに接続があった場合には自動的に削除される．
void
SbjGraph::connect(SbjNode* from,
		  SbjNode* to,
		  ymuint pos)
{
  // SbjNode::mFaoutList を変更するのはここだけ
  
  SbjEdge* edge = to->fanin_edge(pos);
  SbjNode* old_from = edge->from();
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
SbjGraph::level() const
{
  if ( !mLevelValid ) {
    for (SbjNodeListConstIter p = input_list().begin();
	 p != input_list().end(); ++ p) {
      SbjNode* node = *p;
      node->mLevel = 0;
    }
    
    vector<SbjNode*> node_list;
    sort(node_list);
    for (vector<SbjNode*>::const_iterator p = node_list.begin();
	 p != node_list.end(); ++ p) {
      SbjNode* node = *p;
      ymuint l = 0;
      for (ymuint i = 0; i < 2; ++ i) {
	SbjNode* inode = node->fanin(i);
	ymuint l1 = inode->mLevel;
	if ( l < l1 ) {
	  l = l1;
	}
      }
      node->mLevel = l + 1;
    }
    
    ymuint max_l = 0;
    for (SbjNodeListConstIter p = output_list().begin();
	 p != output_list().end(); ++ p) {
      SbjNode* node = *p;
      SbjNode* inode = node->fanin(0);
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


//////////////////////////////////////////////////////////////////////
// immediate dominator 関係のコード
//////////////////////////////////////////////////////////////////////

// @brief 各ノードの immediate dominator を求める．
void
SbjGraph::find_imm_dom()
{
  vector<SbjNode*> node_list;
  rsort(node_list);

  ymuint n = max_node_id();
  vector<ymuint> order_array(n);

  ymuint ord = 0;
  for (vector<SbjNode*>::iterator p = node_list.begin();
       p != node_list.end(); ++ p, ++ ord) {
    SbjNode* node = *p;
    node->find_imm_dom(ord, order_array);
  }
}

// @brief immedate dominator を求める．
void
SbjNode::find_imm_dom(ymuint order,
		      vector<ymuint>& order_array)
{
  order_array[id()] = order;
  ymuint nfo = mFanoutList.size();
  if ( nfo == 0 || pomark() ) {
    mImmDom = NULL;
  }
  else if ( nfo == 1 ) {
    const SbjEdge* e = mFanoutList.front();
    mImmDom = e->to();
  }
  else {
    SbjEdgeList::iterator p = mFanoutList.begin();
    const SbjEdge* e = *p;
    const SbjNode* fo = e->to();
    mImmDom = fo->imm_dom();
    for ( ; mImmDom != NULL && p != mFanoutList.end(); ++ p) {
      const SbjEdge* e = *p;
      const SbjNode* fo = e->to();
      const SbjNode* imm1 = fo->imm_dom();
      // mImmDom と imm1 をマージ
      while ( mImmDom != imm1 && mImmDom != NULL && imm1 != NULL ) {
	if ( order_array[mImmDom->id()] < order_array[imm1->id()] ) {
	  imm1 = imm1->imm_dom();
	}
	else {
	  mImmDom = mImmDom->imm_dom();
	}
      }
      if ( mImmDom != imm1 ) {
	mImmDom = NULL;
      }
    }
  }
}


// @brief efo の計算をする．
void
SbjGraph::set_efo() const
{
  for (SbjNodeList::const_iterator p = mInputList.begin();
       p != mInputList.end(); ++ p) {
    SbjNode* node = *p;
    hash_set<ymuint> mark;
    for (SbjEdgeList::iterator p = node->mFanoutList.begin();
	 p != node->mFanoutList.end(); ++ p) {
      SbjEdge* e = *p;
      SbjNode* fonode = e->to();
      fonode->scan_efo(mark);
    }
    node->mNefo = mark.size();
  }
  for (SbjNodeList::const_iterator p = mLnodeList.begin();
       p != mLnodeList.end(); ++ p) {
    SbjNode* node = *p;
    ymuint n = 0;
    if ( node->is_unselected() ) {
      n = 1;
    }
    else {
      hash_set<ymuint> mark;
      for (SbjEdgeList::iterator p = node->mFanoutList.begin();
	   p != node->mFanoutList.end(); ++ p) {
	SbjEdge* e = *p;
	SbjNode* fonode = e->to();
	fonode->scan_efo(mark);
      }
      n =mark.size();
    }
    node->mNefo = n;
  }
}

// @brief SbjGraph の内容をダンプする関数
void
dump(ostream& s,
     const SbjGraph& sbjgraph)
{
  const SbjNodeList& input_list = sbjgraph.input_list();
  for (SbjNodeList::const_iterator p = input_list.begin();
       p != input_list.end(); ++ p) {
    const SbjNode* node = *p;
    s << "I" << node->subid() << "(" << node->id_str() << "): "
      << node->name() << endl;
  }

  const SbjNodeList& lnode_list = sbjgraph.lnode_list();
  for (SbjNodeList::const_iterator p = lnode_list.begin();
       p != lnode_list.end(); ++ p) {
    const SbjNode* node = *p;
    s << node->id_str() << ": " << node->name() << " = LOGIC[";
    ymuint fcode = node->fcode();
    s << ((fcode >> 3) & 1) << ((fcode >> 2) & 1)
      << ((fcode >> 1) & 1) << (fcode & 1) << "] ("
      << " " << node->fanin(0)->id_str()
      << ", " << node->fanin(1)->id_str()
      << ")" << endl;
    s << "\t => " << node->mffc_root()->id_str() << " (";
    if ( node->imm_dom() == NULL ) {
      s << "*****";
    }
    else {
      s << node->imm_dom()->id_str();
    }
    s << ")";
    s << endl;
  }

  const SbjNodeList& output_list = sbjgraph.output_list();
  for (SbjNodeList::const_iterator p = output_list.begin();
       p != output_list.end(); ++ p) {
    const SbjNode* node = *p;
    const SbjEdge* e = node->fanin_edge(0);
    s << "O" << node->subid() << "(" << node->id_str() << "): "
      << node->name() << " = ";
    const SbjNode* inode = e->from();
    if ( inode ) {
      // 普通のノードの場合
      if ( node->output_inv() ) {
	s << "~";
      }
      s << inode->id_str();
    }
    else {
      // 定数ノードの場合
      if ( node->output_inv() ) {
	s << "1";
      }
      else {
	s << "0";
      }
    }
    s << endl;
  }
}

// @brief SbjGraph の内容を blif 形式で出力する関数
void
write_blif(ostream& s,
	   const SbjGraph& sbjgraph,
	   const string& module_name)
{
  s << ".model " << module_name << endl;
  const SbjNodeList& input_list = sbjgraph.input_list();
  for (SbjNodeList::const_iterator p = input_list.begin();
       p != input_list.end(); ++ p) {
    const SbjNode* node = *p;
    s << ".inputs " << node->name() << endl;
  }

  const SbjNodeList& output_list = sbjgraph.output_list();
  for (SbjNodeList::const_iterator p = output_list.begin();
       p != output_list.end(); ++ p) {
    const SbjNode* node = *p;
    s << ".outputs " << node->name() << endl;
  }
  for (SbjNodeList::const_iterator p = output_list.begin();
       p != output_list.end(); ++ p) {
    const SbjNode* node = *p;
    const SbjNode* inode = node->fanin(0);
    if ( inode == 0 ) {
      s << ".names " << node->name() << endl;
      if ( node->output_inv() ) {
	s << "1" << endl;
      }
      else {
	s << "0" << endl;
      }
    }
    else if ( node->name() != inode->name() ) {
      s << ".names " << inode->name() << " " << node->name() << endl;
      if ( node->output_inv() ) {
	s << "0 1" << endl;
      }
      else {
	s << "1 1" << endl;
      }
    }
    s << endl;
  }
  
  const SbjNodeList& lnode_list = sbjgraph.lnode_list();
  for (SbjNodeList::const_iterator p = lnode_list.begin();
       p != lnode_list.end(); ++ p) {
    const SbjNode* node = *p;
    s << ".names " << node->fanin(0)->name()
      << " " << node->fanin(1)->name()
      << " " << node->name() << endl;
    switch ( node->fcode() ) {
    case 0x0: // 0000
    case 0x3: // 0011
    case 0x5: // 0101
    case 0xa: // 1010
    case 0xc: // 1100
    case 0xf: // 1111
      assert_not_reached(__FILE__, __LINE__);
      break;
    case 0x1: // 0001
      s << "00 1" << endl;
      break;
    case 0x2: // 0010
      s << "10 1" << endl;
      break;
    case 0x4: // 0100
      s << "01 1" << endl;
      break;
    case 0x6: // 0110
      s << "10 1" << endl
	<< "01 1" << endl;
      break;
    case 0x7: // 0111
      s << "0- 1" << endl
	<< "-0 1" << endl;
      break;
    case 0x8: // 1000
      s << "11 1" << endl;
      break;
    case 0x9: // 1001
      s << "00 1" << endl
	<< "11 1" << endl;
      break;
    case 0xb: // 1011
      s << "1- 1" << endl
	<< "-0 1" << endl;
      break;
    case 0xd: // 1101
      s << "0- 1" << endl
	<< "-1 1" << endl;
      break;
    case 0xe: // 1110
      s << "1- 1" << endl
	<< "-1 1" << endl;
      break;
    }
    s << endl;
  }
  s << ".end" << endl;
}

END_NAMESPACE_MAGUS_LUTMAP
