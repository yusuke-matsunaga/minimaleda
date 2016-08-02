
/// @file libym_aig/tests/CutSweep.cc
/// @brief CutSweep の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: CutSweep.cc 1594 2008-07-18 10:26:12Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "CutSweep.h"


BEGIN_NAMESPACE_YM_AIG

// @brief 処理の最初に呼ばれる関数
// @param[in] network 対象のサブジェクトグラフ
// @param[in] limit カットサイズ
// @note デフォルトの実装ではなにもしない．
void
CutSweep::all_init(const AigNetwork& network,
		   size_t limit)
{
  size_t n = network.max_node_id();
  mTmpVect.clear();
  mTmpVect.resize(n);
  mValid.clear();
  mValid.resize(n);
  mCnodeListArray.clear();
  mCnodeListArray.resize(n);
}

ymulong
CutSweep::calc_vect(AigNode* node)
{
  size_t id = node->id();
  if ( !mValid[id] ) {
    ymulong pat0 = calc_vect(node->fanin0());
    if ( node->fanin0_inv() ) {
      pat0 = ~pat0;
    }
    ymulong pat1 = calc_vect(node->fanin1());
    if ( node->fanin1_inv() ) {
      pat1 = ~pat1;
    }
    mTmpVect[id] = pat0 & pat1;
    mValid[id] = true;
  }
  return mTmpVect[id];
}

void
CutSweep::clear_vect(AigNode* node)
{
  size_t id = node->id();
  if ( mValid[id] ) {
    mValid[id] = false;
    if ( node->is_and() ) {
      clear_vect(node->fanin0());
      clear_vect(node->fanin1());
    }
  }
}

void
CutSweep::found(AigNode* root,
		size_t ni,
		AigNode* inputs[])
{
  cout << "root:   " << root->id() << endl
       << "leaves:";
  for (size_t i = 0; i < ni; ++ i) {
    cout << " " << inputs[i]->id();
  }
  cout << endl << endl;
  size_t np = 1 << ni;
  for (size_t i = 0; i < ni; ++ i) {
    ymulong pat = 0UL;
    for (size_t p = 0; p < np; ++ p) {
      if ( p & (1UL << i) ) {
	pat |= (1UL << p);
      }
    }
    AigNode* input = inputs[i];
    mTmpVect[input->id()] = pat;
    mValid[input->id()] = true;
  }
  ymulong ofunc = calc_vect(root);
  clear_vect(root);
  for (size_t p = 0; p < np; ++ p) {
    if ( ofunc & (1UL << p) ) {
      cout << "1";
    }
    else {
      cout << "0";
    }
  }
  cout << endl;
}
  
// @brief node のカット集合のフットプリントを得る．
vector<AigNode*>&
CutSweep::cnode_list(AigNode* node)
{
  return mCnodeListArray[node->id()];
}

END_NAMESPACE_YM_AIG
