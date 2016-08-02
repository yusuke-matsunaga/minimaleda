
/// @file libym_bnet/BlifBNetConv.cc
/// @brief BlifBNetConv の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: BlifBNetConv.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_blifbnetconv/BlifBNetConv.h"
#include "ym_bnet/BNetwork.h"
#include "ym_bnet/BNetManip.h"
#include "ym_blif/BlifNetwork.h"
#include "ym_lexp/LogExpr.h"


BEGIN_NAMESPACE_YM

// @brief コンストラクタ
BlifBNetConv::BlifBNetConv() :
  mManip(NULL)
{
}

// @brief デストラクタ
BlifBNetConv::~BlifBNetConv()
{
}

// @brief 変換する．
// @param[in] blif_network 変換元のネットワーク
// @param[in] network 変換先のネットワーク
bool
BlifBNetConv::operator()(const BlifNetwork& blif_network,
			 BNetwork& network)
{
  assert_cond(mManip == NULL, __FILE__, __LINE__);
  
  network.clear();
  
  mManip = new BNetManip(&network);
  mNodeMap.resize(blif_network.max_node_id(), NULL);

  // モデル名の設定
  network.set_model_name(blif_network.name());

  // 外部入力ノードの生成
  ymuint32 npi = blif_network.npi();
  for (ymuint32 i = 0; i < npi; ++ i) {
    const BlifNode* blif_node = blif_network.pi(i);
    BNode* node = mManip->new_input(blif_node->name());
    mNodeMap[blif_node->id()] = node;
  }
  // ラッチノードの生成
  ymuint32 nff = blif_network.nff();
  for (ymuint32 i = 0; i < nff; ++ i) {
    const BlifNode* blif_node = blif_network.ff(i);
    BNode* node = mManip->new_latch(blif_node->name());
    mNodeMap[blif_node->id()] = node;
  }
  // 外部出力に用いられているノードを再帰的に生成
  ymuint32 npo = blif_network.npo();
  for (ymuint32 i = 0; i < npo; ++ i) {
    const BlifNode* blif_node = blif_network.po(i);
    BNode* node = make_node(blif_node);
    BNode* onode = mManip->new_output(blif_node->name());
    mManip->change_output(onode, node);
  }
  // ラッチに用いられているノードを再帰的に生成
  for (ymuint32 i = 0; i < nff; ++ i) {
    const BlifNode* blif_node = blif_network.ff(i);
    BNode* node = mNodeMap[blif_node->id()];
    BNode* inode = make_node(blif_node->fanin(0));
    int rval = -1;
    if ( blif_node->opat() == '0' ) {
      rval = 0;
    }
    else if ( blif_node->opat() == '1' ) {
      rval = 1;
    }
    mManip->change_latch(node, inode, rval);
  }

  delete mManip;
  mManip = NULL;

  return true;
}

// blif_node に対応するノードを作る．
BNode*
BlifBNetConv::make_node(const BlifNode* blif_node)
{
  BNode* node = mNodeMap[blif_node->id()];
  if ( node == NULL ) {
    assert_cond( blif_node->type() == BlifNode::kLogic, __FILE__, __LINE__);
    ymuint32 ni = blif_node->ni();
    vector<BNode*> fanins(ni);
    for (ymuint32 i = 0; i < ni; ++ i) {
      fanins[i] = make_node(blif_node->fanin(i));
    }
    node = mManip->new_logic(blif_node->name());
    mNodeMap[blif_node->id()] = node;

    ymuint32 nc = blif_node->nc();
    LogExpr expr;
    if ( blif_node->opat() == '1' ) {
      if ( ni == 0 ) {
	expr = LogExpr::make_one();
      }
      else {
	vector<LogExpr> or_expr;
	for (ymuint32 c = 0; c < nc; ++ c) {
	  vector<LogExpr> and_expr;
	  for (ymuint32 i = 0; i < ni; ++ i) {
	    char v = blif_node->cube_pat(c, i);
	    if ( v == '0' ) {
	      and_expr.push_back(LogExpr::make_negaliteral(i));
	    }
	    else if ( v == '1' ) {
	      and_expr.push_back(LogExpr::make_posiliteral(i));
	    }
	  }
	  or_expr.push_back(LogExpr::make_and(and_expr));
	}
	expr = LogExpr::make_or(or_expr);
      }
    }
    else {
      if ( ni == 0 ) {
	expr = LogExpr::make_zero();
      }
      else {
	vector<LogExpr> or_expr;
	for (ymuint32 c = 0; c < nc; ++ c) {
	  vector<LogExpr> and_expr;
	  for (ymuint32 i = 0; i < ni; ++ i) {
	    char v = blif_node->cube_pat(c, i);
	    if ( v == '0' ) {
	      and_expr.push_back(LogExpr::make_posiliteral(i));
	    }
	    else if ( v == '1' ) {
	      and_expr.push_back(LogExpr::make_negaliteral(i));
	    }
	  }
	  or_expr.push_back(LogExpr::make_or(and_expr));
	}
	expr = LogExpr::make_and(or_expr);
      }
    }
    
    bool stat = mManip->change_logic(node, expr, fanins, false);
    assert_cond( stat, __FILE__, __LINE__);
  }
  return node;
}

END_NAMESPACE_YM
