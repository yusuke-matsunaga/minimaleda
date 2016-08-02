
/// @file libym_aig/AigMgrImpl.cc
/// @brief AigMgrImpl の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
/// 
/// $Id: AigMgrImpl.cc 2203 2009-04-16 05:04:40Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "AigMgrImpl.h"
#include "ym_aig/AigNode.h"


BEGIN_NAMESPACE_YM_AIG

//////////////////////////////////////////////////////////////////////
// クラス AigMgrImpl
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
AigMgrImpl::AigMgrImpl() :
  mAlloc(sizeof(AigNode) * 1024),
  mHashTable(NULL),
  mHashSize(0)
{
  alloc_table(1024);
}
  
// @brief デストラクタ
AigMgrImpl::~AigMgrImpl()
{
  // ノードは mAlloc がすべて片付けてくれる．
  delete [] mHashTable;
}

// @brief 外部入力を作る．
AigHandle
AigMgrImpl::make_input()
{
  AigNode* node = new_node(true);
  ymuint input_id = mInputNodes.size();
  node->mFanins[0].mPackedData = input_id;
  mInputNodes.push_back(node);
  AigHandle ans(node, false);
  return ans;
}

// @brief AND ノードを作る．
AigHandle
AigMgrImpl::make_and(AigHandle handle1,
		     AigHandle handle2)
{
  // trivial な場合の処理
  if ( handle1.is_zero() || handle2.is_zero() ) {
    return make_zero();
  }
  if ( handle1.is_one() ) {
    return handle2;
  }
  if ( handle2.is_one() ) {
    return handle1;
  }
  if ( handle1 == handle2 ) {
    return handle1;
  }
  if ( handle1.node() == handle2.node() ) {
    // handle1.inv != handle2.inv() のはず
    return make_zero();
  }

  // 順番の正規化
  if ( handle1.node_id() < handle2.node_id() ) {
    AigHandle tmp = handle1;
    handle1 = handle2;
    handle2 = tmp;
  }

  // 同じ構造を持つノードが既にないか調べる．
  ymuint pos = hash_func(handle1, handle2);
  ymuint idx = pos % mHashSize;
  for (AigNode* node = mHashTable[idx]; node; node = node->mLink) {
    if ( node->mFanins[0] == handle1 && node->mFanins[1] == handle2 ) {
      // 同じノードがあった．
      return AigHandle(node, false);
    }
  }
  
  // 新しいノードを作る．
  AigNode* node = new_node(false);
  node->mFanins[0] = handle1;
  node->mFanins[1] = handle2;

  // ハッシュ表に登録する．
  if ( node_num() >= mNextLimit ) {
    alloc_table(mHashSize * 2);
    // サイズが変わったのでインデックスを再計算する．
    idx = pos % mHashSize;
  }
  node->mLink = mHashTable[idx];
  mHashTable[idx] = node;
  
  AigHandle ans(node, false);
  return ans;
}

// 新しいノードを作成する．
// 作成されたノードを返す．
AigNode*
AigMgrImpl::new_node(bool input)
{
  ymuint id = mAllNodes.size();
  void* p = mAlloc.get_memory(sizeof(AigNode));
  AigNode* node = new (p) AigNode(input, id);
  mAllNodes.push_back(node);
  return node;
}
  
// @brief ハッシュ表を確保する．
void
AigMgrImpl::alloc_table(ymuint req_size)
{
  AigNode** old_table = mHashTable;
  ymuint old_size = mHashSize;
  
  if ( mHashSize == 0 ) {
    mHashSize = 1024;
  }
  while ( mHashSize < req_size ) {
    mHashSize <<= 1;
  }
  mHashTable = new AigNode*[mHashSize];
  for (ymuint i = 0; i < mHashSize; ++ i) {
    mHashTable[i] = NULL;
  }
  if ( old_size > 0 ) {
    for (ymuint i = 0; i < old_size; ++ i) {
      AigNode* next = NULL;
      for (AigNode* node = old_table[i]; node; node = next) {
	next = node->mLink;
	ymuint pos = hash_func(node->mFanins[0], node->mFanins[1]);
	ymuint idx = pos % mHashSize;
	node->mLink = mHashTable[idx];
	mHashTable[idx] = node;
      }
    }
    delete [] old_table;
  }
  mNextLimit = static_cast<ymuint32>(mHashSize * 1.8);
}

END_NAMESPACE_YM_AIG
