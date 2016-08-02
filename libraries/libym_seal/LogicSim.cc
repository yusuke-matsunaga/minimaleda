
/// @file libym_seal/LogicSim.cc
/// @brief LogicSim の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: LogicSim.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "LogicSim.h"


BEGIN_NAMESPACE_YM_SEAL

// @brief コンストラクタ
// @param[in] bnetwork ネットワーク
LogicSim::LogicSim(const BNetwork* bnetwork) :
  mNetwork(bnetwork),
  mValArray(bnetwork->max_node_id())
{
  mNetwork->tsort(mNodeArray);
}

// @brief デストラクタ
LogicSim::~LogicSim()
{
}
  
// @brief シミュレーションを行う．
// @param[in] input_vector 入力ベクタを整数にしたもの
// @param[in] cur_state 現状態
// @param[out] output_vector 出力ベクタを整数にしたもの
// @param[out] next_state 次状態
void
LogicSim::operator()(ymuint input_vector,
		     State cur_state,
		     ymuint& output_vector,
		     State& next_state)
{
  ymuint var_num = 0;
  for (BNodeList::const_iterator p = mNetwork->inputs_begin();
       p != mNetwork->inputs_end(); ++ p, ++ var_num) {
    BNode* node = *p;
    if ( input_vector & (1U << var_num) ) {
      mValArray[node->id()] = 1;
    }
    else {
      mValArray[node->id()] = 0;
    }
  }

  var_num = 0;
  for (BNodeList::const_iterator p = mNetwork->latch_nodes_begin();
       p != mNetwork->latch_nodes_end(); ++ p, ++ var_num) {
    BNode* node = *p;
    if ( cur_state[var_num] == '1' ) {
      mValArray[node->id()] = 1;
    }
    else {
      mValArray[node->id()] = 0;
    }
  }

  for (BNodeVector::iterator p = mNodeArray.begin(); p != mNodeArray.end(); ++ p) {
    BNode* node = *p;
    LogExpr node_func = node->func();
    ymuint ni = node->ni();
    vector<ymulong> fanin_vals(ni);
    for (ymuint i = 0; i < ni; ++ i) {
      BNode* inode = node->fanin(i);
      fanin_vals[i] = mValArray[inode->id()];
    }
    mValArray[node->id()] = node_func.eval(fanin_vals, 1UL);
  }
  
  output_vector = 0U;
  var_num = 0;
  for (BNodeList::const_iterator p = mNetwork->outputs_begin();
       p != mNetwork->outputs_end(); ++ p, ++ var_num) {
    BNode* node = *p;
    BNode* inode = node->fanin(0);
    if ( mValArray[inode->id()] ) {
      output_vector |= (1U << var_num);
    }
  }

  next_state = "";
  for (BNodeList::const_iterator p = mNetwork->latch_nodes_begin();
       p != mNetwork->latch_nodes_end(); ++ p) {
    BNode* node = *p;
    BNode* inode = node->fanin(0);
    if ( mValArray[inode->id()] ) {
      next_state += "1";
    }
    else {
      next_state += "0";
    }
  }
}

END_NAMESPACE_YM_SEAL
