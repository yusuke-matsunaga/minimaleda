
/// @file atpg/src/dtpg_sat/tgnet2aig.cc
/// @brief TgNetwork から AIG を作る関数の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: tgnet2aig.cc 2203 2009-04-16 05:04:40Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "dtpgcl_nsdef.h"
#include "ym_tgnet/TgNetwork.h"
#include "ym_tgnet/TgNode.h"
#include "ym_aig/AigMgr.h"


BEGIN_NAMESPACE_YM_ATPG_DTPGCL

// TgNode から AIG を作る
AigHandle
tgnode2aig(tTgGateType type,
	   const vector<AigHandle>& fanins,
	   AigMgr& aig_mgr)
{
  ymuint ni = fanins.size();
  bool inv = false;
  AigHandle aig;
  switch ( type ) {
  case kTgNot:
    inv = true;
    // わざと次に続く
  case kTgOutput:
  case kTgBuff:
    assert_cond(fanins.size() == 1, __FILE__, __LINE__);
    aig = fanins[0];
    break;

  case kTgNand:
    inv = true;
    // わざと次に続く
  case kTgAnd:
    assert_cond(ni >= 2, __FILE__, __LINE__);
    aig = aig_mgr.make_and(fanins[0], fanins[1]);
    for (ymuint i = 2; i < ni; ++ i) {
      aig = aig_mgr.make_and(aig, fanins[i]);
    }
    break;

  case kTgNor:
    inv = true;
    // わざと次に続く
  case kTgOr:
    assert_cond(ni >= 2, __FILE__, __LINE__);
    aig = aig_mgr.make_or(fanins[0], fanins[1]);
    for (ymuint i = 2; i < ni; ++ i) {
      aig = aig_mgr.make_or(aig, fanins[i]);
    }
    break;

  case kTgXnor:
    inv = true;
    // わざと次に続く
  case kTgXor:
    assert_cond(ni >= 2, __FILE__, __LINE__);
    aig = aig_mgr.make_xor(fanins[0], fanins[1]);
    for (ymuint i = 2; i < ni; ++ i) {
      aig = aig_mgr.make_xor(aig, fanins[i]);
    }
    break;

  case kTgUndef:
  case kTgInput:
  default:
    assert_not_reached(__FILE__, __LINE__);
    break;
  }
  if ( inv ) {
    return ~aig;
  }
  else {
    return aig;
  }
}

// TgNetwork から AIG を作る関数
void
tgnet2aig(const TgNetwork& network,
	  AigMgr& aig_mgr,
	  vector<AigHandle>& node_map)
{
  ymuint npi = network.input_num2();
  ymuint npo = network.output_num2();
  ymuint nl = network.logic_num();
  ymuint ngate = nl + npi;

  ymuint max_id = network.node_num();
  node_map.clear();
  node_map.resize(max_id);

  // 入力の生成
  for (ymuint i = 0; i < npi; ++ i) {
    const TgNode* node = network.input(i);
    AigHandle aig = aig_mgr.make_input();
    node_map[node->gid()] = aig;
  }

  // 内部ノードの生成
  for (ymuint i = 0; i < nl; ++ i) {
    const TgNode* node = network.sorted_logic(i);
    ymuint nfi = node->ni();
    vector<AigHandle> fanins(nfi);
    for (ymuint j = 0; j < nfi; ++ j) {
      const TgNode* inode = node->fanin(j);
      fanins[j] = node_map[inode->gid()];
    }
    if ( node->is_cplx_logic() ) {
      LogExpr expr = network.get_lexp(node);
      node_map[node->gid()] = aig_mgr.make_logic(expr, fanins);
    }
    else {
      node_map[node->gid()] = tgnode2aig(node->type(), fanins, aig_mgr);
    }
  }
}

END_NAMESPACE_YM_ATPG_DTPGCL
