/// @file libym_aig/tests/GpNode.cc
/// @brief GpNode の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: GpNode.cc 2274 2009-06-10 07:45:29Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "GpNode.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
// クラス GpMgr
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
GpMgr::GpMgr() :
  mAlloc(1024),
  mHashTable(NULL),
  mHashSize(0)
{
  alloc_table(1024);
}

// @brief デストラクタ
GpMgr::~GpMgr()
{
  // ノードのメモリは mAlloc のデストラクタで無事開放される．
}
  
// @brief 定数0ノードを生成する．
GpHandle
GpMgr::make_const0()
{
  return GpHandle(NULL, false);
}

// @brief 定数1ノードを生成する．
GpHandle
GpMgr::make_const1()
{
  return GpHandle(NULL, true);
}
  
// @brief 入力ノードを生成する．
// @param[in] input_id 入力番号
// @note すでに同じ入力番号のノードが存在していたらそれを返す．
GpHandle
GpMgr::make_input(ymuint32 input_id,
		  ymuint32 fv)
{
  while ( mInputList.size() <= input_id ) {
    GpNode* node = new_node();
    ymuint32 new_id = mInputList.size();
    node->mInputId = (new_id << 4) | 2U;
    mInputList.push_back(node);
  }
  GpNode* node = mInputList[input_id];
  node->mLevel = 0;
  node->mSize = 0;
  node->mPat = fv;
  return GpHandle(node, false);
}

// @brief AND ノードを生成する．
// @param[in] fanin0 ファンイン0
// @param[in] fanin1 ファンイン1
// @note もしも既に同じ構造のノードがあればそれを返す．
// @note 場合によってはファンインその物や定数ノードを返すこともある．
GpHandle
GpMgr::make_and(GpHandle fanin0,
		GpHandle fanin1,
		ymuint32 fv)
{
  // トリビアルな場合の処理
  if ( fanin0.is_const0() || fanin1.is_const0() ) {
    return make_const0();
  }
  if ( fanin0.is_const1() ) {
    return fanin1;
  }
  if ( fanin1.is_const1() ) {
    return fanin0;
  }
  if ( fanin0 == fanin1 ) {
    return fanin0;
  }
  if ( fanin0.node() == fanin1.node() ) {
    // 上でハンドルの一致は調べているのでここに来たら逆極性
    return make_const0();
  }
  
  // 順番の正規化
  if ( fanin0.node()->id() < fanin1.node()->id() ) {
    GpHandle tmp = fanin0;
    fanin0 = fanin1;
    fanin1 = tmp;
  }

  // 同じ構造を持つノードがないか調べる．
  ymuint pos = hash_func(fanin0, fanin1);
  ymuint idx = pos % mHashSize;
  for (GpNode* node = mHashTable[idx]; node; node = node->mSlink) {
    if ( node->mFanin[0] == fanin0 && node->mFanin[1] == fanin1 ) {
      return GpHandle(node, false);
    }
  }

  // 新しいノードを作る．
  GpNode* node = new_node();
  node->mInputId = 3U;
  node->mFanin[0] = fanin0;
  node->mFanin[1] = fanin1;
  node->mLevel = fanin0.node()->level() + fanin1.node()->level() + 1;
  node->mSize = count_size(node);
  node->mPat = fv;
  
  // ハッシュ表に登録する．
  if ( mAndList.size() >= mNextLimit ) {
    alloc_table(mHashSize * 2);
    // サイズが変わったのでインデックスを再計算する．
    idx = pos % mHashSize;
  }
  node->mSlink = mHashTable[idx];
  mHashTable[idx] = node;
  mAndList.push_back(node);

  return GpHandle(node, false);
}

// @brief ノードを生成する下請け関数
GpNode*
GpMgr::new_node()
{
  void* p = mAlloc.get_memory(sizeof(GpNode));
  ymuint id = mNodeList.size();
  GpNode* node = new (p) GpNode(id);
  mNodeList.push_back(node);
  return node;
}

BEGIN_NONAMESPACE

// count_size() の下請け関数
ymuint32
cs_sub(GpNode* node,
       vector<bool>& marks)
{
  if ( node->is_input() ) {
    return 0;
  }
  if ( marks[node->id()] ) {
    // すでにカウント済み
    return 0;
  }
  marks[node->id()] = true;
  return cs_sub(node->fanin0(), marks) + cs_sub(node->fanin1(), marks) + 1;
}

END_NONAMESPACE

// @brief node を根とするパタンのノード数を数える．
ymuint32
GpMgr::count_size(GpNode* node)
{
  vector<bool> marks(mNodeList.size(), false);
  return cs_sub(node, marks);
}
  
// @brief ハッシュ関数
ymuint32
GpMgr::hash_func(GpHandle fanin0,
		 GpHandle fanin1)
{
  return fanin0.mNodePol + fanin1.mNodePol;
}

// @brief ハッシュテーブルを拡大する．
void
GpMgr::alloc_table(ymuint req_size)
{
  GpNode** old_table = mHashTable;
  ymuint old_size = mHashSize;

  if ( mHashSize == 0 ) {
    mHashSize = 1024;
  }
  while ( mHashSize < req_size ) {
    mHashSize <<= 1;
  }
  
  mHashTable = new GpNode*[mHashSize];
  for (ymuint i = 0; i < mHashSize; ++ i) {
    mHashTable[i] = NULL;
  }
  if ( old_size > 0 ) {
    for (ymuint i = 0; i < old_size; ++ i) {
      GpNode* next = NULL;
      for (GpNode* node = old_table[i]; node; node = next) {
	next = node->mSlink;
	ymuint pos = hash_func(node->mFanin[0], node->mFanin[1]);
	ymuint idx = pos % mHashSize;
	node->mSlink = mHashTable[idx];
	mHashTable[idx] = node;
      }
    }
    delete [] old_table;
  }
  mNextLimit = static_cast<ymuint32>(mHashSize * 1.8);
}


//////////////////////////////////////////////////////////////////////
// クラス GpNode
//////////////////////////////////////////////////////////////////////

// コンストラクタ
// 入力ノードを表す
GpNode::GpNode(ymuint id) :
  mId(id),
  mSlink(NULL)
{
}

// デストラクタ
GpNode::~GpNode()
{
}

END_NAMESPACE_YM
