
/// @file libym_bdn/BdNetwork.cc
/// @brief BdNetwork の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_bdn/BdNetwork.h"
#include "ym_bdn/BdnNode.h"
#include "ym_bdn/BdnNodeHandle.h"


BEGIN_NAMESPACE_YM_BDN

///////////////////////////////////////////////////////////////////////
// クラス BdnNode
///////////////////////////////////////////////////////////////////////

// コンストラクタ
BdnNode::BdnNode() :
  mFlags(0),
  mLevel(0)
{
  mFanins[0].set_to(this, 0);
  mFanins[1].set_to(this, 1);
}

// デストラクタ
BdnNode::~BdnNode()
{
}

// @brief ファンアウトに出力が含まれているか調べ pomark をセットする．
void
BdnNode::scan_po()
{
  mFlags &= ~kPoMask;
  const BdnFanoutList& folist = fanout_list();
  for (BdnFanoutList::const_iterator p = folist.begin();
       p != folist.end(); ++ p) {
    BdnEdge* e = *p;
    if ( e->to()->is_output() ) {
      mFlags |= kPoMask;
      break;
    }
  }
}

// @brief ID を表す文字列の取得
string
BdnNode::id_str() const
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
  else if ( is_latch() ) {
    buf << "Q" << id();
  }
  else {
    buf << "X" << id();
  }
  return buf.str();
}


///////////////////////////////////////////////////////////////////////
// クラス BdNetwork
///////////////////////////////////////////////////////////////////////

// コンストラクタ
BdNetwork::BdNetwork() :
  mAlloc(4096),
  mHashTable(NULL),
  mHashSize(0),
  mLevel(0U)

{
  alloc_table(1024);
}

// コピーコンストラクタ
BdNetwork::BdNetwork(const BdNetwork& src) :
  mAlloc(4096),
  mHashTable(NULL),
  mHashSize(0),
  mLevel(0U)
{
  copy(src);
}

// 代入演算子
const BdNetwork&
BdNetwork::operator=(const BdNetwork& src)
{
  if ( &src != this ) {
    clear();
    copy(src);
  }
  return *this;
}

// デストラクタ
BdNetwork::~BdNetwork()
{
  clear();
  delete [] mHashTable;
}

// 複製する．
void
BdNetwork::copy(const BdNetwork& src)
{
  ymuint n = src.max_node_id();
  vector<BdnNode*> nodemap(n);

  const BdnNodeList& src_latch_list = src.latch_list();

  // 外部入力ノードの生成
  ymuint ni = src.input_num();
  for (ymuint i = 0; i < ni; ++ i) {
    BdnNode* src_node = src.input(i);
    BdnNode* dst_node = new_input(src.input_name(i));
    nodemap[src_node->id()] = dst_node;
  }
  // ラッチノードの生成
  for (BdnNodeList::const_iterator p = src_latch_list.begin();
       p != src_latch_list.end(); ++ p) {
    BdnNode* src_node = *p;
    BdnNode* dst_node = new_latch(src_node->reset_val());
    nodemap[src_node->id()] = dst_node;
  }

  // 論理ノードの生成
  vector<BdnNode*> node_list;
  src.sort(node_list);
  ymuint nl = node_list.size();
  for (ymuint i = 0; i < nl; ++ i) {
    BdnNode* src_node = node_list[i];

    BdnNode* src_inode0 = src_node->fanin(0);
    BdnNode* input0 = nodemap[src_inode0->id()];
    assert_cond(input0, __FILE__, __LINE__);

    BdnNode* src_inode1 = src_node->fanin(1);
    BdnNode* input1 = nodemap[src_inode1->id()];
    assert_cond(input1, __FILE__, __LINE__);

    BdnNodeHandle tmp_h = new_logic(src_node->fcode(),
				    BdnNodeHandle(input0, false),
				    BdnNodeHandle(input1, false));
    assert_cond(tmp_h.inv() == false, __FILE__, __LINE__);
    BdnNode* dst_node = tmp_h.node();
    nodemap[src_node->id()] = dst_node;
  }

  // 外部出力ノードの生成
  ymuint no = src.output_num();
  for (ymuint i = 0; i < no; ++ i) {
    BdnNode* src_onode = src.output(i);
    BdnNode* src_inode = src_onode->fanin(0);
    BdnNode* dst_inode = NULL;
    if ( src_inode ) {
      dst_inode = nodemap[src_inode->id()];
    }
    BdnNodeHandle dst_handle(dst_inode, src_onode->output_inv());
    BdnNode* dst_onode = new_output(src.output_name(i), dst_handle);
    nodemap[src_onode->id()] = dst_onode;
  }

  // ラッチノードの入力の設定
  for (BdnNodeList::const_iterator p = src_latch_list.begin();
       p != src_latch_list.end(); ++ p) {
    const BdnNode* src_input = *p;
    const BdnNode* src_output = src_input->fanin(0);
    BdnNode* dst_input = nodemap[src_input->id()];
    assert_cond(dst_input->is_latch(), __FILE__, __LINE__);
    BdnNode* dst_output = NULL;
    if ( src_output ) {
      dst_output = nodemap[src_output->id()];
    }
    BdnNodeHandle dst_handle(dst_output, src_input->output_inv());
    change_latch(dst_input, dst_handle);
  }
}

// 空にする．
void
BdNetwork::clear()
{
  mInputArray.clear();
  mInputNameArray.clear();
  mOutputArray.clear();
  mOutputNameArray.clear();
  mLatchList.clear();
  mLnodeList.clear();
  mItvlMgr.clear();
  mLevel = 0U;
}

BEGIN_NONAMESPACE

// sort() の下請け関数
// node のファンアウトのうち，すべてのファンインがマークされている
// ノードを node_list に追加する．
void
sort_sub(BdnNode* node,
	 vector<bool>& marks,
	 vector<BdnNode*>& node_list)
{
  const BdnFanoutList& fo_list = node->fanout_list();
  for (BdnFanoutList::const_iterator p = fo_list.begin();
       p != fo_list.end(); ++ p) {
    BdnEdge* e = *p;
    BdnNode* onode = e->to();
    if ( !marks[onode->id()] && onode->is_logic() &&
	 marks[onode->fanin0()->id()] &&
	 marks[onode->fanin1()->id()] ) {
      marks[onode->id()] = true;
      node_list.push_back(onode);
    }
  }
}

END_NONAMESPACE

// @brief ソートされたノードのリストを得る．
void
BdNetwork::sort(vector<BdnNode*>& node_list) const
{
  node_list.clear();
  node_list.reserve(lnode_num());

  // 処理済みの印を表す配列
  ymuint n = max_node_id();
  vector<bool> marks(n, false);

  // 外部入力とラッチのみをファンインにするノードを node_list に追加する．
  ymuint ni = input_num();
  for (ymuint i = 0; i < ni; ++ i) {
    BdnNode* node = input(i);
    marks[node->id()] = true;
    sort_sub(node, marks, node_list);
  }
  for (BdnNodeList::const_iterator p = latch_list().begin();
       p != latch_list().end(); ++ p) {
    BdnNode* node = *p;
    marks[node->id()] = true;
    sort_sub(node, marks, node_list);
  }
  // node_list のノードを一つずつとりだし，ファンアウトが node_list
  // に積めるかチェックする．
  for (ymuint rpos = 0; rpos < node_list.size(); ++ rpos) {
    BdnNode* node = node_list[rpos];
    sort_sub(node, marks, node_list);
  }
  // うまくいっていれば全ての論理ノードが node_list に入っているはず．
  assert_cond(node_list.size() == lnode_num(), __FILE__, __LINE__);
}


BEGIN_NONAMESPACE

// rsort() の下請け関数
// node のすべてのファンアウトがマークされていたら node_list に積む．
void
rsort_sub(BdnNode* node,
	  vector<bool>& marks,
	  vector<BdnNode*>& node_list)
{
  if ( node == NULL ||
       !node->is_logic() ||
       marks[node->id()] ) return;

  const BdnFanoutList& fo_list = node->fanout_list();
  for (BdnFanoutList::const_iterator p = fo_list.begin();
       p != fo_list.end(); ++ p) {
    BdnEdge* e = *p;
    BdnNode* onode = e->to();
    if ( !marks[onode->id()] ) {
      return;
    }
  }
  marks[node->id()] = true;
  node_list.push_back(node);
}

END_NONAMESPACE

// @brief 逆順でソートされたノードのリストを得る．
void
BdNetwork::rsort(vector<BdnNode*>& node_list) const
{
  node_list.clear();
  node_list.reserve(lnode_num());

  // 処理済みの印をあらわす配列
  ymuint n = max_node_id();
  vector<bool> marks(n, false);

  // 外部出力とラッチのみをファンアウトにするノードを node_list に追加する．
  ymuint no = output_num();
  for (ymuint i = 0; i < no; ++ i) {
    BdnNode* node = output(i);
    marks[node->id()] = true;
    BdnNode* inode = node->fanin(0);
    rsort_sub(inode, marks, node_list);
  }
  for (BdnNodeList::const_iterator p = latch_list().begin();
       p != latch_list().end(); ++ p) {
    BdnNode* node = *p;
    marks[node->id()] = true;
    BdnNode* inode = node->fanin(0);
    rsort_sub(inode, marks, node_list);
  }

  // node_list からノードを取り出し，同様の処理を行う．
  for (ymuint rpos = 0; rpos < node_list.size(); ++ rpos) {
    BdnNode* node = node_list[rpos];
    rsort_sub(node->fanin0(), marks, node_list);
    rsort_sub(node->fanin1(), marks, node_list);
  }
  // うまくいっていればすべての論理ノードが node_list に入っているはず．
  assert_cond(node_list.size() == lnode_num(), __FILE__, __LINE__);
}

// 入力ノードを作る．
BdnNode*
BdNetwork::new_input(const string& name)
{
  assert_cond(mInputArray.size() == mInputNameArray.size(), __FILE__, __LINE__);

  BdnNode* node = new_node();

  // 入力ノード配列に登録
  ymuint id = mInputArray.size();
  mInputArray.push_back(node);

  // 入力名の配列に名前を登録．
  mInputNameArray.push_back(name);

  node->set_input(id);
  node->mLevel = 0;

  return node;
}

// 出力ノードを作る．
BdnNode*
BdNetwork::new_output(const string& name,
		      BdnNodeHandle inode_handle)
{
  assert_cond(mOutputArray.size() == mOutputNameArray.size(), __FILE__, __LINE__);

  BdnNode* node = new_node();

  // 出力ノード配列に登録
  ymuint id = mOutputArray.size();
  mOutputArray.push_back(node);

  // 出力名の配列に名前を登録
  mOutputNameArray.push_back(name);

  node->set_output(id);

  change_output(node, inode_handle);

  mLevel = 0U;

  return node;
}

// @brief 出力ノードの内容を変更する
// @param[in] 変更対象の出力ノード
// @param[in] inode 入力のノード
void
BdNetwork::change_output(BdnNode* node,
			 BdnNodeHandle inode_handle)
{
  bool inv = inode_handle.inv();
  node->set_inv(inv);
  BdnNode* inode = inode_handle.node();
  connect(inode, node, 0);

  mLevel = 0U;
}

// @brief ラッチを作る．
// @param[in] reset_val リセット値 ( 0 / 1 / 2 )
// @note reset_val = 2 の時は不定
BdnNode*
BdNetwork::new_latch(int reset_val)
{
  BdnNode* node = new_node();

  node->set_latch(reset_val);

  // ラッチリストに登録
  mLatchList.push_back(node);

  mLevel = 0U;

  return node;
}

// @brief ラッチのファンインを変更する．
// @param[in] latch 変更対象のラッチ
// @param[in] input_handle ラッチの入力に接続しているノード+極性
void
BdNetwork::change_latch(BdnNode* latch,
			BdnNodeHandle input_handle)
{
  // 実はこれと同じ
  change_output(latch, input_handle);
}

// @brief ラッチのリセット値を変更する．
// @param[in] latch 変更対象のラッチ
// @param[in] reset_val リセット値 ( 0 / 1 / 2 )
// @note reset_val = 2 の時は不定
void
BdNetwork::change_reset_value(BdnNode* latch,
			      int reset_val)
{
  latch->set_latch(reset_val);
}

// @brief 論理ノードを作る．
// @param[in] fcode 機能コード
// @param[in] inode1_handle 1番めの入力ノード+極性
// @param[in] inode2_handle 2番めの入力ノード+極性
// @return 作成したノードを返す．
// @note fcode の出力極性を正規化する．
// @note すでに同じ機能コード，同じファンインを持つノードがあればそれを返す．
BdnNodeHandle
BdNetwork::new_logic(ymuint fcode,
		     BdnNodeHandle inode1_handle,
		     BdnNodeHandle inode2_handle)
{
  mLevel = 0U;
  return set_logic(NULL, fcode, inode1_handle, inode2_handle);
}

// @brief 論理ノードの内容を変更する．
// @param[in] node 変更対象の論理ノード
// @param[in] fcode 機能コード
// @param[in] inode1_handle 1番めの入力ノード+極性
// @param[in] inode2_handle 2番めの入力ノード+極性
// @note fcode の出力極性を正規化する．
// @note 実際には新しいノードを作ってそこへのリンクを内部で持つ．
void
BdNetwork::change_logic(BdnNode* node ,
			ymuint fcode,
			BdnNodeHandle inode1_handle,
			BdnNodeHandle inode2_handle)
{
  BdnNodeHandle new_handle = set_logic(node, fcode, inode1_handle, inode2_handle);
  if ( new_handle == BdnNodeHandle(node, false) ) {
    // 変化なし
    return;
  }

  mLevel = 0U;

  // node のファンアウト先の内容を変える．
  const BdnFanoutList& fo_list = node->fanout_list();
  for (BdnFanoutList::const_iterator p = fo_list.begin();
       p != fo_list.end(); ++ p) {
    BdnEdge* edge = *p;
    BdnNode* onode = edge->to();
    if ( onode->is_logic() ) {
      BdnNodeHandle inode1_handle(onode->fanin0());
      BdnNodeHandle inode2_handle(onode->fanin1());
      if ( edge->pos() == 0 ) {
	inode1_handle = new_handle;
      }
      else {
	inode2_handle = new_handle;
      }
      change_logic(onode, onode->fcode(), inode1_handle, inode2_handle);
    }
    else if ( onode->is_output() ) {
      change_output(onode, new_handle);
    }
    else if ( onode->is_latch() ) {
      change_latch(onode, new_handle);
    }
  }
}


BEGIN_NONAMESPACE

// BdnNode のハッシュ関数
inline
ymuint
hash_func(ymuint fcode,
	  const BdnNode* node1,
	  const BdnNode* node2)
{
  return (node1->id() * 3 + node2->id() << 3) | (fcode >> 1);
}

END_NONAMESPACE


// 論理ノードの内容を設定する．
BdnNodeHandle
BdNetwork::set_logic(BdnNode* node,
		     ymuint fcode,
		     BdnNodeHandle inode0_handle,
		     BdnNodeHandle inode1_handle)
{
  // 入力の極性を fcode に反映させる．
  if ( inode0_handle.inv() ) {
    ymuint tmp0 = fcode & 0x5; // 0101
    ymuint tmp1 = fcode & 0xa; // 1010
    fcode = (tmp0 << 1) | (tmp1 >> 1);
  }
  if ( inode1_handle.inv() ) {
    ymuint tmp0 = fcode & 0x3; // 0011
    ymuint tmp1 = fcode & 0xc; // 1100
    fcode = (tmp0 << 2) | (tmp1 >> 2);
  }

  BdnNode* inode0 = inode0_handle.node();
  BdnNode* inode1 = inode1_handle.node();

  // 境界条件の検査
  if ( inode0 == NULL ) {
    if ( inode1 == NULL ) {
      if ( (fcode & 1U) == 1U ) {
	return BdnNodeHandle::make_one();
      }
      else {
	return BdnNodeHandle::make_zero();
      }
    }
    if ( (fcode & 1U) == 1U ) {
      if ( (fcode & 4U) == 4U ) {
	// 11 なので定数1
	return BdnNodeHandle::make_one();
      }
      else {
	// 10 なので ~(inode1)
	return BdnNodeHandle(inode1, true);
      }
    }
    else {
      if ( (fcode & 4U) == 4U ) {
	// 01 なので inode1
	return BdnNodeHandle(inode1, false);
      }
      else {
	// 00 なので定数0
	return BdnNodeHandle::make_zero();
      }
    }
  }
  if ( inode1 == NULL ) {
    if ( (fcode & 1U) == 1U ) {
      if ( (fcode & 2U) == 2U ) {
	// 11 なので定数1
	return BdnNodeHandle::make_one();
      }
      else {
	// 10 なので ~(inode0)
	return BdnNodeHandle(inode0, true);
      }
    }
    else {
      if ( (fcode & 2U) == 2U ) {
	// 01 なので inode0
	return BdnNodeHandle(inode0, false);
      }
      else {
	// 00 なので定数0
	return BdnNodeHandle::make_zero();
      }
    }
  }
  if ( inode0 == inode1 ) {
    if ( (fcode & 1U) == 1U ) {
      if ( (fcode & 8U) == 8U ) {
	return BdnNodeHandle::make_one();
      }
      else {
	return BdnNodeHandle(inode0, true);
      }
    }
    else {
      if ( (fcode & 8U) == 8U ) {
	return BdnNodeHandle(inode0, false);
      }
      else {
	return BdnNodeHandle::make_zero();
      }
    }
  }

  // 出力極性の正規化
  bool inv = (fcode & 1U) == 1U;
  if ( inv ) {
    fcode ^= 0xf;
  }

  // ファンインの順番の正規化
  if ( inode0->id() > inode1->id() ) {
    BdnNode* tmp = inode0;
    inode0 = inode1;
    inode1 = tmp;
    if ( fcode & 2U ) {
      if ( (fcode & 4U) == 0U ) {
	fcode &= 8U;
	fcode |= 4U;
      }
    }
    else if ( fcode & 4U ) {
      fcode &= 8U;
      fcode |= 2U;
    }
  }

  // 同じ構造を持つノードが既にないか調べる．
  ymuint pos = hash_func(fcode, inode0, inode1);
  ymuint idx = pos % mHashSize;
  for (BdnNode* node1 = mHashTable[idx]; node1; node1 = node1->mLink) {
    if ( node1->fcode() == fcode &&
	 node1->fanin0() == inode0 &&
	 node1->fanin1() == inode1 ) {
      // 同じノードがあった．
      return BdnNodeHandle(node1, inv);
    }
  }

  if ( node == NULL ) {
    // 新しいノードを作る．
    node = new_node();

    // 論理ノードリストに登録
    mLnodeList.push_back(node);

    if ( lnode_num() >= mNextLimit ) {
      alloc_table(mHashSize * 2);
      // サイズが変わったのでインデックスを再計算する．
      idx = pos % mHashSize;
    }
  }
  else {
    // ハッシュ表から取り除く
    ymuint pos0 = hash_func(node->fcode(), node->fanin0(), node->fanin1());
    ymuint idx0 = pos0 % mHashSize;
    BdnNode* prev = mHashTable[idx0];
    if ( prev == node ) {
      mHashTable[idx0] = node->mLink;
    }
    for (BdnNode* node0 = 0; (node0 = prev->mLink); prev = node0) {
      if ( node0 == node ) {
	prev->mLink = node->mLink;
	break;
      }
    }
    // エラーチェック(node0 == NULL) はしていない．
  }

  node->set_logic(fcode);
  connect(inode0, node, 0);
  connect(inode1, node, 1);

  // ハッシュ表に登録する．
  node->mLink = mHashTable[idx];
  mHashTable[idx] = node;

  return BdnNodeHandle(node, inv);
}

// from を to の pos 番目のファンインとする．
// to の pos 番目にすでに接続があった場合には自動的に削除される．
void
BdNetwork::connect(BdnNode* from,
		   BdnNode* to,
		   ymuint pos)
{
  // BdnNode::mFaoutList を変更するのはここだけ

  BdnEdge* edge = &to->mFanins[pos];
  BdnNode* old_from = edge->from();
  if ( old_from ) {
    old_from->mFanoutList.erase(edge);
    old_from->scan_po();
  }
  edge->set_from(from);
  if ( from ) {
    from->mFanoutList.push_back(edge);
    from->scan_po();
  }

  // 構造が変わったのでレベルの情報は無効化される．
  mLevel = 0;
}

// @brief ハッシュ表を確保する．
void
BdNetwork::alloc_table(ymuint req_size)
{
  BdnNode** old_table = mHashTable;
  ymuint old_size = mHashSize;

  if ( mHashSize == 0 ) {
    mHashSize = 1024;
  }
  while ( mHashSize < req_size ) {
    mHashSize <<= 1;
  }
  mHashTable = new BdnNode*[mHashSize];
  for (ymuint i = 0; i < mHashSize; ++ i) {
    mHashTable[i] = NULL;
  }
  if ( old_size > 0 ) {
    for (ymuint i = 0; i < old_size; ++ i) {
      BdnNode* next = NULL;
      for (BdnNode* node = old_table[i]; node; node = next) {
	next = node->mLink;
	ymuint pos = hash_func(node->fcode(), node->fanin0(), node->fanin1());
	ymuint idx = pos % mHashSize;
	node->mLink = mHashTable[idx];
	mHashTable[idx] = node;
      }
    }
    delete [] old_table;
  }
  mNextLimit = static_cast<ymuint32>(mHashSize * 1.8);
}

// 新しいノードを作成する．
// 作成されたノードを返す．
BdnNode*
BdNetwork::new_node()
{
  BdnNode* node = NULL;

  // 空いているIDを探してノード配列へ登録
  int id = mItvlMgr.avail_num();
  mItvlMgr.erase(id);

  ymuint uid = static_cast<ymuint>(id);
  while( mNodeArray.size() <= uid ) {
    // mNodeArray の大きさが小さいときのための埋め草
    void* p = mAlloc.get_memory(sizeof(BdnNode));
    BdnNode* node = new (p) BdnNode;
    mNodeArray.push_back(node);
  }
  node = mNodeArray[uid];
  node->mId = uid;
  node->mFlags = 0U;

  return node;
}

// node を削除する．
void
BdNetwork::delete_node(BdnNode* node)
{
  if ( node->is_logic() ) {
    mLnodeList.erase(node);
  }

  // new_node の逆の処理を行なう．
  mItvlMgr.add(static_cast<int>(node->mId));

  // mNodeArray 内のエントリはクリアしない．
  // id の再利用と同様に BdnNode も再利用する．
}

// @brief 段数を求める．
ymuint
BdNetwork::level() const
{
  if ( (mLevel & 1U) == 0U ) {
    ymuint ni = input_num();
    for (ymuint i = 0; i < ni; ++ i) {
      BdnNode* node = input(i);
      node->mLevel = 0;
    }

    vector<BdnNode*> node_list;
    sort(node_list);
    for (vector<BdnNode*>::const_iterator p = node_list.begin();
	 p != node_list.end(); ++ p) {
      BdnNode* node = *p;
      BdnNode* inode0 = node->fanin0();
      ymuint l = inode0->mLevel;
      BdnNode* inode1 = node->fanin1();
      ymuint l1 = inode1->mLevel;
      if ( l < l1 ) {
	l = l1;
      }
      node->mLevel = l + 1;
    }

    ymuint max_l = 0;
    ymuint no = output_num();
    for (ymuint i = 0; i < no; ++ i) {
      BdnNode* node = output(i);
      BdnNode* inode = node->fanin0();
      if ( inode ) {
	ymuint l1 = inode->mLevel;
	if ( max_l < l1 ) {
	  max_l = l1;
	}
      }
    }
    mLevel = (max_l << 1) | 1U;
  }
  return (mLevel >> 1);
}

// @brief BdNetwork の内容をダンプする関数
void
dump(ostream& s,
     const BdNetwork& network)
{
  ymuint ni = network.input_num();
  for (ymuint i = 0; i < ni; ++ i) {
    const BdnNode* node = network.input(i);
    s << node->id_str();
    string name = network.input_name(i);
    if ( name != string() ) {
      s << "(" << name << ") ";
    }
    s << " : = INPUT" << endl;
  }
  const BdnNodeList& latch_list = network.latch_list();
  for (BdnNodeList::const_iterator p = latch_list.begin();
       p != latch_list.end(); ++ p) {
    const BdnNode* node = *p;
    s << node->id_str();
    s << " : = LATCH(";
    const BdnNode* inode = node->fanin0();
    if ( inode ) {
      if ( node->output_inv() ) {
	s << "~";
      }
      s << inode->id_str();
    }
    else {
      if ( node->output_inv() ) {
	s << "1";
      }
      else {
	s << "0";
      }
    }
    s << ", reset_value = " << node->reset_val()
      << ")" << endl;
  }

  const BdnNodeList& lnode_list = network.lnode_list();
  for (BdnNodeList::const_iterator p = lnode_list.begin();
       p != lnode_list.end(); ++ p) {
    const BdnNode* node = *p;
    s << node->id_str();
    s << " :  = LOGIC[";
    ymuint fcode = node->fcode();
    s << ((fcode >> 3) & 1) << ((fcode >> 2) & 1)
      << ((fcode >> 1) & 1) << (fcode & 1) << "] ("
      << " " << node->fanin0()->id_str()
      << ", " << node->fanin1()->id_str()
      << ")" << endl;
  }

  ymuint no = network.output_num();
  for (ymuint i = 0; i < no; ++ i) {
    const BdnNode* node = network.output(i);
    s << node->id_str();
    string name = network.output_name(i);
    if ( name != string() ) {
      s << "(" << name << ") ";
    }
    s << " : = OUTPUT(";
    const BdnNode* inode = node->fanin0();
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
    s << ")" << endl;
  }
}

END_NAMESPACE_YM_BDN
