
/// @file libym_bdd/bmc/bmc_and.cc
/// @brief AND 演算を行う関数の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: bmc_and.cc 700 2007-05-31 00:41:30Z matsunaga $
///
/// Copyright (C) 2005-2006 Yusuke Matsunaga
/// All rights reserved.


#if HAVE_CONFIG_H
#include <ymconfig.h>
#endif

#include "BddMgrClassic.h"
#include "BmcCompTbl.h"


BEGIN_NAMESPACE_YM_BDD

// 2つのBDDのANDを計算するapply演算
// これで，AND,NAND,OR,NORをすべて計算する．
tBddEdge
BddMgrClassic::and_op(tBddEdge f,
		      tBddEdge g)
{
  // エラー状態のチェック
  if ( check_error(f) || check_error(g) ) {
    // どちらかがエラー
    return kEdgeError;
  }
  if ( check_overflow(f) || check_overflow(g) ) {
    // どちらかがオーバーフロー
    return kEdgeOverflow;
  }

  // 特別な場合の処理
  // 1：片方のBDDが0なら答えは0，互いに否定の関係でも答えは0．
  // 2：両方とも1なら答えは1．
  // 3：片方が1ならもう片方を返す．
  // 4：同じBDDどうしのANDは自分自身
  // 2 は 3 でカバーされている．
  if ( check_zero(f) || check_zero(g) || check_reverse(f, g) ) {
    return kEdge0;
  }
  if ( check_one(f) ) {
    return g;
  }
  if ( check_one(g) || f == g ) {
    return f;
  }
  // この時点で f,g は終端ではない．

  // 演算結果テーブルが当たりやすくなるように順序を正規化する
  if (f > g) {
    tBddEdge tmp = f;
    f = g;
    g = tmp;
  }

  tBddEdge result = mAndTable->get(f, g);
  if ( result == kEdgeInvalid ) {
    // 演算結果テーブルには登録されていない
    tBddEdge f_0, f_1;
    tBddEdge g_0, g_1;
    Var* var = split(f, g, f_0, f_1, g_0, g_1);
    tBddEdge r_0 = and_op(f_0, g_0);
    tBddEdge r_1 = and_op(f_1, g_1);
    result = new_node(var, r_0, r_1);
    mAndTable->put(f, g, result);
  }

  return result;
}

// 2つのBDDのORを計算するapply演算
tBddEdge
BddMgrClassic::or_op(tBddEdge f,
		     tBddEdge g)
{
  // エラー状態のチェック
  if ( check_error(f) || check_error(g) ) {
    // どちらかがエラー
    return kEdgeError;
  }
  if ( check_overflow(f) || check_overflow(g) ) {
    // どちらかがオーバーフロー
    return kEdgeOverflow;
  }
  tBddEdge ans = and_op(negate(f), negate(g));
  return negate_ifvalid(ans);
}

// 2つのBDDのXORを計算するapply演算
tBddEdge
BddMgrClassic::xor_op(tBddEdge f,
		      tBddEdge g)
{
  // エラー状態のチェック
  if ( check_error(f) || check_error(g) ) {
    // どちらかがエラー
    return kEdgeError;
  }
  if ( check_overflow(f) || check_overflow(g) ) {
    // どちらかがオーバーフロー
    return kEdgeOverflow;
  }

  // 特別な場合の処理
  // 1: 片方が0なら他方を返す．
  // 2: 片方が1なら他方の否定を返す．
  // 3: 同じBDD同士のXORは0を返す．
  // 4: 極性のみが異なる関数同士なら1を返す．
  if ( check_zero(f) ) {
    return g;
  }
  if ( check_zero(g) ) {
    return f;
  }
  if ( check_one(f) ) {
    return negate(g);
  }
  if ( check_one(g) ) {
    return negate(f);
  }
  if ( f == g ) {
    return kEdge0;
  }
  if ( check_reverse(f, g) ) {
    return kEdge1;
  }
  // この時点で f, g は終端ではない．

  // 極性情報は落してしまう．
  tPol f_pol = get_pol(f);
  tPol g_pol = get_pol(g);
  f = addpol(f, f_pol);
  g = addpol(g, g_pol);
  tPol ans_pol = f_pol * g_pol;

  // 対称演算なので正規化する．
  if ( f > g ) {
    tBddEdge tmp = f;
    f = g;
    g = tmp;
  }

  tBddEdge result = mXorTable->get(f, g);
  if ( result == kEdgeInvalid ) {
    tBddEdge f_0, f_1;
    tBddEdge g_0, g_1;
    Var* var = split(f, g, f_0, f_1, g_0, g_1);
    tBddEdge r_0 = xor_op(f_0, g_0);
    tBddEdge r_1 = xor_op(f_1, g_1);
    result = new_node(var, r_0, r_1);
    mXorTable->put(f, g, result);
  }

  return addpol(result, ans_pol);
}

// 2つの論理関数が交わっていれば kEdge1 を返す．
tBddEdge
BddMgrClassic::check_intersect(tBddEdge f,
			       tBddEdge g)
{
  // エラー状態のチェック
  if ( check_error(f) || check_error(g) ) {
    // どちらかがエラー
    return kEdgeError;
  }
  if ( check_overflow(f) || check_overflow(g) ) {
    // どちらかがオーバーフロー
    return kEdgeOverflow;
  }

  // 特別な場合の処理
  // 1: 片方が0なら絶対に交わらないので 0 を返す．
  // 2: 極性のみが異なっている場合にも交わらないので 0 を返す．
  // 3: 片方が1なら交わっているので 1 を返す．
  // 4: 同じ関数なら交わっているので 1 を返す．
  if ( check_zero(f) || check_zero(g) || check_reverse(f, g) ) {
    return kEdge0;
  }
  if ( check_one(f) || check_one(g) || f == g ) {
    return kEdge1;
  }
  // この時点で f, g は終端ではない．

  // 極性は正規化されている(否定属性は0枝のみ，葉は1のみ)
  // ので f も g も正極性のときは f(1,1,...,1) = g(1,1,...,1) = 1
  // つまりすべての入力を 1 にしたときの関数値は 1 であるので
  // 交わっていることがわかる．
  tPol f_pol = get_pol(f);
  tPol g_pol = get_pol(g);
  if ( f_pol == kPolPosi && g_pol == kPolPosi ) {
    return kEdge1;
  }

  if ( f > g ) {
    tBddEdge tmp = f;
    f = g;
    g = tmp;
    tPol tmp_pol = f_pol;
    f_pol = g_pol;
    g_pol = tmp_pol;
  }

  tBddEdge result = mIntTable->get(f, g);
  if ( result == kEdgeInvalid ) {
    Node* f_vp = get_node(f);
    Node* g_vp = get_node(g);
    Var* f_var = f_vp->var();
    Var* g_var = g_vp->var();
    tLevel f_level = f_var->level();
    tLevel g_level = g_var->level();
    tBddEdge f_0, f_1;
    tBddEdge g_0, g_1;
    if ( f_level < g_level ) {
      f_0 = f_vp->edge0(f_pol);
      f_1 = f_vp->edge1(f_pol);
      g_0 = g_1 = g;
    }
    else if ( f_level > g_level ) {
      f_0 = f_1 = f;
      g_0 = g_vp->edge0(g_pol);
      g_1 = g_vp->edge1(g_pol);
    }
    else {
      f_0 = f_vp->edge0(f_pol);
      f_1 = f_vp->edge1(f_pol);
      g_0 = g_vp->edge0(g_pol);
      g_1 = g_vp->edge1(g_pol);
    }
    result = check_intersect(f_0, g_0);
    if ( result == kEdge0 ) {
      result = check_intersect(f_1, g_1);
    }
    mIntTable->put(f, g, result);
  }
  return result;
}

END_NAMESPACE_YM_BDD
