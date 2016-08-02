
/// @file libym_bdd/bmm/bmm_isop.cc
/// @brief ISOP を求める関数の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: bmm_isop.cc 700 2007-05-31 00:41:30Z matsunaga $
///
/// Copyright (C) 2005-2006 Yusuke Matsunaga
/// All rights reserved.


#if HAVE_CONFIG_H
#include <ymconfig.h>
#endif

#include "BddMgrModern.h"
#include "BmmCompTbl.h"


BEGIN_NAMESPACE_YM_BDD

BEGIN_NONAMESPACE

// cov を積和形論理式と見なして各キューブにリテラルを追加する
LogExpr
sop_litand(const LogExpr& cov,
	   tVarId varid,
	   tPol pol)
{
  if ( cov.is_zero() ) {
    // 0ならそのまま
    return cov;
  }

  LogExpr lit = LogExpr::make_literal(varid, pol);
  if ( cov.is_one() ) {
    // 1なら lit を返せばよい
    return lit;
  }

  if ( cov.is_literal() || cov.is_and() ) {
    // リテラルかキューブの場合には lit を追加する．
    return cov & lit;
  }

  // あとはカバーの場合のみ
  size_t n = cov.child_num();
  LogExpr ans = LogExpr::make_zero();
  for (size_t i = 0; i < n; i ++) {
    LogExpr child = cov.child(i);
    ans |= child & lit;
  }
  return ans;
}

END_NONAMESPACE


// l と u をそれぞれ下限,上限した不完全指定論理関数
// を満たす非冗長積和形論理式を求める．
tBddEdge
BddMgrModern::isop(tBddEdge l,
		   tBddEdge u,
		   LogExpr& cover)
{
  if ( check_error(l) || check_error(u) ) {
    return kEdgeError;
  }
  if ( check_overflow(l) || check_overflow(u) ) {
    return kEdgeOverflow;
  }
  tBddEdge ans = isop_step(l, u, cover);
  mIsopTable->clear();
  return ans;
}

// l と u をそれぞれ下限,上限した不完全指定論理関数
// のすべての prime implicants からなる prime cover を求める．
LogExpr
BddMgrModern::prime_cover(tBddEdge l,
			  tBddEdge u)
{
  if ( check_invalid(l) || check_invalid(u) ) {
    return LogExpr();
  }

  LogExpr cover;
  (void) pc_step(l, u, cover);
  mPcTable->clear();
  return cover;
}

// [l, u] で表された不完全指定論理関数の極小サポート集合を列挙する．
// 答の関数の prime implicant が本当の極小サポート集合となっている．
tBddEdge
BddMgrModern::minimal_support(tBddEdge l,
			      tBddEdge u)
{
  if ( check_error(l) || check_error(u) ) {
    return kEdgeError;
  }
  if ( check_overflow(l) || check_overflow(u) ) {
    return kEdgeOverflow;
  }

  if ( check_zero(l) || check_one(u) ) {
    // もうオンセットとオフセットは交わらない
    return kEdge1;
  }
  if ( check_one(l) || check_zero(u) ) {
    // オンセットとオフセットが交わっている
    return kEdge0;
  }

  tBddEdge result = mMinsupTable->get(l, u);
  if ( result == kEdgeInvalid ) {
    tBddEdge l0, l1;
    tBddEdge u0, u1;
    Var* var = split(l, u, l0, l1, u0, u1);

    // level を含む場合の極小サポート
    tBddEdge r_dep = minimal_support(l0, u0);
    if ( !check_zero(r_dep) ) {
      tBddEdge r_dep1 = minimal_support(l1, u1);
      r_dep = and_op(r_dep, r_dep1);
    }

    // level を含まない場合の極小サポート
    tBddEdge r_indep = minimal_support(or_op(l0, l1), and_op(u0, u1));

    result = new_node(var, r_indep, r_dep);
    mMinsupTable->put(l, u, result);
  }
  return result;
}

// isop 用の再帰関数
tBddEdge
BddMgrModern::isop_step(tBddEdge l,
			tBddEdge u,
			LogExpr& cov)
{
  if ( check_zero(l) ) {
    // 0 がもっとも簡単な答え
    cov = LogExpr::make_zero();
    return kEdge0;
  }

  if ( check_one(u) ) {
    // 1 がもっとも簡単な答え
    cov = LogExpr::make_one();
    return kEdge1;
  }

  // 演算結果テーブルを探す．
  tBddEdge result = mIsopTable->get(l, u, cov);
  if ( result == kEdgeInvalid ) {
    // 見つからなかった．
    tBddEdge l_0, l_1;
    tBddEdge u_0, u_1;
    Var* var = split(l, u, l_0, l_1, u_0, u_1);
    tBddEdge var_edge = new_node(var, kEdge0, kEdge1);
    tVarId varid = var->varid();
    tBddEdge z_0 = and_op(l_0, negate(u_1));
    LogExpr p_0;
    tBddEdge c_0 = isop_step(z_0, u_0, p_0);
    tBddEdge cc_0 = and_op(c_0, negate(var_edge));
    p_0 = sop_litand(p_0, varid, kPolNega);

    tBddEdge z_1 = and_op(l_1, negate(u_0));
    LogExpr p_1;
    tBddEdge c_1 = isop_step(z_1, u_1, p_1);
    tBddEdge cc_1 = and_op(c_1, var_edge);
    p_1 = sop_litand(p_1, varid, kPolPosi);

    tBddEdge h_01 = and_op(l_0, negate(c_0));
    tBddEdge h_02 = and_op(l_1, negate(c_1));
    tBddEdge h_0  = or_op(h_01, h_02);
    tBddEdge h_1  = and_op(u_0, u_1);
    LogExpr p_2;
    tBddEdge r_0 = isop_step(h_0, h_1, p_2);

    result = or_op(or_op(cc_0, cc_1), r_0);
    cov = p_0 | p_1 | p_2;
    mIsopTable->put(l, u, result, cov);
  }

  return result;
}

// prime_cover 用の再帰関数
tBddEdge
BddMgrModern::pc_step(tBddEdge l,
		      tBddEdge u,
		      LogExpr& cov)
{
  if ( check_zero(l) ) {
    // 0 がもっとも簡単な答え
    cov = LogExpr::make_zero();
    return kEdge0;
  }

  if ( check_one(u) ) {
    // 1 がもっとも簡単な答え
    cov = LogExpr::make_one();
    return kEdge1;
  }

  // 演算結果テーブルを探す．
  tBddEdge result = mPcTable->get(l, u, cov);
  if ( result == kEdgeInvalid ) {
    // 見つからなかった．
    tBddEdge l_0, l_1;
    tBddEdge u_0, u_1;
    Var* var = split(l, u, l_0, l_1, u_0, u_1);
    tBddEdge var_edge = new_node(var, kEdge0, kEdge1);
    tVarId varid = var->varid();

    // \bar{x} を含む prime implicants の生成
    tBddEdge z_0 = and_op(l_0, negate(u_1));
    LogExpr p_0;
    tBddEdge c_0 = pc_step(z_0, u_0, p_0);
    tBddEdge cc_0 = and_op(c_0, negate(var_edge));
    p_0 = sop_litand(p_0, varid, kPolNega);

    // x を含む prime implicatns の生成
    tBddEdge z_1 = and_op(l_1, negate(u_0));
    LogExpr p_1;
    tBddEdge c_1 = pc_step(z_1, u_1, p_1);
    tBddEdge cc_1 = and_op(c_1, var_edge);
    p_1 = sop_litand(p_1, varid, kPolPosi);

    // x/\bar{x} を含まない prime implicants の生成
    tBddEdge h_01 = and_op(l_0, negate(c_0));
    tBddEdge h_02 = and_op(l_1, negate(c_1));
    tBddEdge h_0  = or_op(h_01, h_02);
    tBddEdge h_1  = and_op(u_0, u_1);
    LogExpr p_2;
    tBddEdge r_0 = pc_step(h_0, h_1, p_2);

    result = or_op(or_op(cc_0, cc_1), r_0);
    cov = p_0 | p_1 | p_2;
    mPcTable->put(l, u, result, cov);
  }

  return result;
}

END_NAMESPACE_YM_BDD
