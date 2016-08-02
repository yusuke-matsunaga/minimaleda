
/// @file libym_bnet/BNetSopDecomp.cc
/// @brief BNetSopDecomp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: BNetSopDecomp.cc 2274 2009-06-10 07:45:29Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_bnet/BNetSopDecomp.h"
#include "ym_bnet/BNetManip.h"


BEGIN_NAMESPACE_YM_BNET

// @brief コンストラクタ．
BNetSopDecomp::BNetSopDecomp()
{
}

// @brief デストラクタ
BNetSopDecomp::~BNetSopDecomp()
{
}
  
// @brief ネットワーク全体を積和形論理式に分解する．
// @param[in] network 対象のネットワーク
void
BNetSopDecomp::operator()(BNetwork& network)
{
  mManip = new BNetManip(&network);
  
  BNodeVector node_list;
  network.tsort(node_list);
  size_t n = network.logic_node_num();
  for (size_t i = 0; i < n; ++ i) {
    BNode* node = node_list[i];
    const LogExpr& expr = node->func();
    if ( expr.is_sop() ) {
      // すでに SOP 形式なら分解の必要はない
      continue;
    }

    sop_decomp_sub(node, expr, node);
  }

  delete mManip;
  mManip = NULL;
}

// sop_decomp_node で用いられるサブルーティン
// expr を根とする論理式を分解して root_node を根のノードとする木を作る．
// この関数を呼び出す時には根のタイプは二項演算子であることを保証すること．
void
BNetSopDecomp::sop_decomp_sub(BNode* orig_node,
			      const LogExpr& expr,
			      BNode* root_node)
{
  assert_cond(!expr.is_zero() && !expr.is_one() && !expr.is_literal(),
	      __FILE__, __LINE__);

  if ( expr.is_and() ) {
    BNodeVector fanin_list;
    LogExpr new_expr = LogExpr::make_one();

    size_t ni = expr.child_num();
    for (size_t i = 0; i < ni; i ++) {
      LogExpr opr1 = expr.child(i);
      assert_cond(!opr1.is_zero() && !opr1.is_one(), __FILE__, __LINE__);
      if ( opr1.is_literal() ) {
	tVarId pos = opr1.varid();
	tPol pol = opr1.is_posiliteral() ? kPolPosi : kPolNega;
	fanin_list.push_back(orig_node->fanin(pos));
	new_expr &= LogExpr::make_literal(i, pol);
      }
      else {
	BNode* node1 = mManip->new_logic();
	sop_decomp_sub(orig_node, opr1, node1);
	fanin_list.push_back(node1);
	new_expr &= LogExpr::make_posiliteral(i);
      }
    }
    // 根のノードにセットする．
    bool stat = mManip->change_logic(root_node, new_expr, fanin_list);
    assert_cond(stat, __FILE__, __LINE__);
  }
  else if ( expr.is_or() ) {
    BNodeVector fanin_list;
    LogExpr new_expr = LogExpr::make_zero();

    size_t ni = expr.child_num();
    for (size_t i = 0; i < ni; i ++) {
      LogExpr opr1 = expr.child(i);
      assert_cond(!opr1.is_zero() && !opr1.is_one(), __FILE__, __LINE__);
      if ( opr1.is_literal() ) {
	tVarId pos = opr1.varid();
	tPol pol = opr1.is_posiliteral() ? kPolPosi : kPolNega;
	fanin_list.push_back(orig_node->fanin(pos));
	new_expr |= LogExpr::make_literal(fanin_list.size() - 1, pol);
      }
      else if ( opr1.is_and() ) {
	LogExpr prd = LogExpr::make_one();
	size_t ni1 = opr1.child_num();
	for (size_t j = 0; j < ni1; j ++) {
	  LogExpr opr2 = opr1.child(j);
	  assert_cond(!opr2.is_zero() && !opr2.is_one(), __FILE__, __LINE__);
	  if ( opr2.is_literal() ) {
	    tVarId pos= opr2.varid();
	    tPol pol = opr2.is_posiliteral() ? kPolPosi : kPolNega;
	    fanin_list.push_back(orig_node->fanin(pos));
	    prd &= LogExpr::make_literal(fanin_list.size() - 1, pol);
	  }
	  else {
	    BNode* node1 = mManip->new_logic();
	    sop_decomp_sub(orig_node, opr2, node1);
	    fanin_list.push_back(node1);
	    prd &= LogExpr::make_posiliteral(fanin_list.size() - 1);
	  }
	}
	new_expr |= prd;
      }
      else {
	BNode* node1 = mManip->new_logic();
	sop_decomp_sub(orig_node, opr1, node1);
	fanin_list.push_back(node1);
	new_expr |= LogExpr::make_posiliteral(fanin_list.size() - 1);
      }
    }
    // 根のノードにセットする．
    bool stat = mManip->change_logic(root_node, new_expr, fanin_list);
    assert_cond(stat, __FILE__, __LINE__);
  }
  else if ( expr.is_xor() ) {
    // まず2入力XORに分解してそれを SOP 形式で表す．
    BNodeVector fanin_list;
    tPol pol = kPolPosi;

    size_t ni = expr.child_num();
    for (size_t i = 0; i < ni; i ++) {
      LogExpr opr1 = expr.child(i);
      assert_cond(!opr1.is_zero() && !opr1.is_one(), __FILE__, __LINE__);
      if ( opr1.is_literal() ) {
	tVarId pos = opr1.varid();
	if ( opr1.is_negaliteral() ) {
	  pol = ~pol;
	}
	fanin_list.push_back(orig_node->fanin(pos));
      }
      else {
	BNode* node1 = mManip->new_logic();
	sop_decomp_sub(orig_node, opr1, node1);
	fanin_list.push_back(node1);
      }
    }
    build_xorsop_tree(0, ni, fanin_list, pol, root_node);
  }
}

// tmp_fanins[b: b + ni - 1] を入力とする XOR をあらわす
// SOP 2分木を network 上のノードとして作る．
// root_node が NULL でない場合には root_node を根のノードとする．
// root_node が NULL の場合には新しいノードを作る．
// いずれの場合でも根のノードを返す．
BNode*
BNetSopDecomp::build_xorsop_tree(size_t b,
				 size_t ni,
				 const BNodeVector& tmp_fanins,
				 tPol pol,
				 BNode* root_node)
{
  assert_cond(ni > 1, __FILE__, __LINE__);

  vector<BNode*> fanins(2);

  size_t b1 = b;
  size_t ni1 = ni / 2;
  if ( ni1 == 1 ) {
    fanins[0] = tmp_fanins[b1];
  }
  else {
    fanins[0] = build_xorsop_tree(b1, ni1, tmp_fanins, kPolPosi, NULL);
  }

  size_t b2 = b1 + ni1;
  size_t ni2 = ni - ni1;
  if ( ni2 == 1 ) {
    fanins[1] = tmp_fanins[b2];
  }
  else {
    fanins[1] = build_xorsop_tree(b2, ni2, tmp_fanins, kPolPosi, NULL);
  }

  LogExpr plit0 = LogExpr::make_posiliteral(0);
  LogExpr nlit0 = LogExpr::make_negaliteral(0);
  LogExpr plit1 = LogExpr::make_posiliteral(1);
  LogExpr nlit1 = LogExpr::make_negaliteral(1);
  LogExpr expr;
  if ( pol == kPolPosi ) {
    // XOR を表す積和形論理式
    expr = plit0 & nlit1 | plit1 & nlit0;
  }
  else {
    // XNOR を表す積和形論理式
    expr = plit0 & plit1 | nlit0 & nlit1;
  }
  if ( root_node == NULL ) {
    root_node = mManip->new_logic();
  }
  bool stat = mManip->change_logic(root_node, expr, fanins);
  assert_cond(stat, __FILE__, __LINE__);

  return root_node;
}

END_NAMESPACE_YM_BNET
