
/// @file libym_bdd/bmm/bmm_ite.cc
/// @brief ITE 演算を行う関数の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: bmm_ite.cc 700 2007-05-31 00:41:30Z matsunaga $
///
/// Copyright (C) 2005-2006 Yusuke Matsunaga
/// All rights reserved.


#if HAVE_CONFIG_H
#include <ymconfig.h>
#endif

#include "BddMgrModern.h"
#include "BmmCompTbl.h"


BEGIN_NAMESPACE_YM_BDD

// f が真の時 g を，偽の時 h を選ぶ関数
tBddEdge
BddMgrModern::ite_op(tBddEdge f,
		     tBddEdge g,
		     tBddEdge h)
{
  if ( check_error(f) || check_error(g) || check_error(h) ) {
    return kEdgeError;
  }
  if ( check_overflow(f) || check_overflow(g) || check_overflow(h) ) {
    return kEdgeOverflow;
  }
  if ( check_one(f) ) {
    return g;
  }
  if ( check_zero(f) ) {
    return h;
  }
  if ( g == h ) {
    return g;
  }
  if ( check_reverse(g, h) ) {
    return xor_op(f, h);
  }
  if ( check_one(g) || f == g ) {
    // f | h
    return or_op(f, h);
  }
  if ( check_zero(g) || check_reverse(f, g) ) {
    // ~f & h
    return and_op(negate(f), h);
  }
  if ( check_one(h) || check_reverse(f, h) ) {
    // ~f | g
    return or_op(negate(f), g);
  }
  if ( check_zero(h) || f == h ) {
    // f & g
    return and_op(f, g);
  }
  // この時点で f, g, h は終端ではない．

  // 演算結果テーブルが当たりやすくなるように順序を正規化する．
  if ( g > h ) {
    tBddEdge tmp = g;
    g = h;
    h = tmp;
    // then と else を入れ替えたんだから条件も反転しなければならない．
    f = negate(f);
  }

  // さらに g に否定属性を付けないように正規化する．
  tPol ans_pol = get_pol(g);
  g = addpol(g, ans_pol);
  h = addpol(h, ans_pol);

  Node* f_vp = get_node(f);
  Node* g_vp = get_node(g);
  Node* h_vp = get_node(h);
  Var* f_var = f_vp->var();
  Var* g_var = g_vp->var();
  Var* h_var = h_vp->var();
  tLevel f_level = f_var->level();
  tLevel g_level = g_var->level();
  tLevel h_level = h_var->level();

  tBddEdge result;

  tPol f_pol = get_pol(f);
  if ( check_zero(f_vp->edge0(f_pol)) &&
       check_one(f_vp->edge1(f_pol)) &&
       f_level < g_level && f_level < h_level ) {
    result = new_node(f_var, h, g);
  }
  else if ( check_one(f_vp->edge0(f_pol)) &&
	      check_zero(f_vp->edge1(f_pol)) &&
	      f_level < g_level && f_level < h_level ) {
    result = new_node(f_var, g, h);
  }
  else {
    result = mIteTable->get(f, g, h);
    if ( result == kEdgeInvalid ) {
      tPol g_pol = get_pol(g);
      tPol h_pol = get_pol(h);
      tLevel top = f_level;
      Var* var = f_var;
      if ( top > g_level) {
	top = g_level;
	var = g_var;
      }
      if ( top > h_level ) {
	top = h_level;
	var = h_var;
      }
      tBddEdge f_0, f_1;
      tBddEdge g_0, g_1;
      tBddEdge h_0, h_1;
      split1(top, f_level, f, f_vp, f_pol, f_0, f_1);
      split1(top, g_level, g, g_vp, g_pol, g_0, g_1);
      split1(top, h_level, h, h_vp, h_pol, h_0, h_1);
      tBddEdge r_0 = ite_op(f_0, g_0, h_0);
      tBddEdge r_1 = ite_op(f_1, g_1, h_1);
      result = new_node(var, r_0, r_1);
      mIteTable->put(f, g, h, result);
    }
  }
  return addpol(result, ans_pol);
}

// multiple compose 演算を行うために最初に呼ばれる関数．
void
BddMgrModern::compose_start()
{
  // 念のため
  clear_varmark();
  mLastLevel = 0;
}

// multiple compose 演算を行う変数と置き換え関数を登録する関数
void
BddMgrModern::compose_reg(tVarId id,
			  tBddEdge e)
{
  Var* var = var_of(id);
  if ( var ) {
    var->mMark = 1;
    var->mCompEdge = e;
    tLevel level = var->level();
    if ( mLastLevel < level ) {
      mLastLevel = level;
    }
  }
}

// multiple compose 演算の本体
tBddEdge
BddMgrModern::compose(tBddEdge e)
{
  tBddEdge ans = compose_step(e);
  clear_varmark();
  mCmpTable->clear();
  return ans;
}

tBddEdge
BddMgrModern::compose_step(tBddEdge f)
{
  if ( check_error(f) ) {
    return kEdgeError;
  }
  if ( check_overflow(f) ) {
    return kEdgeOverflow;
  }
  if ( check_leaf(f) ) {
    return f;
  }

  Node* f_vp = get_node(f);
  Var* f_var = f_vp->var();
  if ( f_var->level() > mLastLevel ) {
    return f;
  }

  // 極性の反転に関して線形性を有するので極性は取り除く．
  tPol f_pol = get_pol(f);
  f = addpol(f, f_pol);

  tBddEdge result = mCmpTable->get(f);
  if ( result == kEdgeInvalid ) {
    tBddEdge f_0 = f_vp->edge0();
    tBddEdge f_1 = f_vp->edge1();
    tBddEdge r_0 = compose_step(f_0);
    tBddEdge r_1 = compose_step(f_1);
    tBddEdge tmp;
    if ( f_var->mMark == 1 ) {
      tmp = f_var->mCompEdge;
    }
    else {
      tmp = new_node(f_var, kEdge0, kEdge1);
    }
    result = ite_op(tmp, r_1, r_0);
    mCmpTable->put(f, result);
  }

  return addpol(result, f_pol);
}

// x_level の変数を y_level まで「押し込む」．
// pol が kPolNega の時は 0-枝と 1-枝を取り替える．
tBddEdge
BddMgrModern::push_down(tBddEdge e,
			tLevel x_level,
			tLevel y_level,
			tPol pol)
{
  if ( check_error(e) ) {
    return kEdgeError;
  }
  if ( check_overflow(e) ) {
    return kEdgeOverflow;
  }

#if 0
  y_edge = BddMgr::make_bdd(varid(y_level), kEdge0, kEdge1);
  xy_edge = BddMgr::make_bdd(varid(x_level), kEdge0, addpol(y_edge, pol));
  activate(xy_edge);
  tBddEdge ans = pd_step(e, x_level, y_level, pol);
  deactivate(xy_edge);
  return ans;
#else
  return kEdge0;
#endif
}

// generalized cofactor 演算
tBddEdge
BddMgrModern::gcofactor(tBddEdge e1,
			tBddEdge e2)
{
  if ( check_error(e1) || check_error(e2) ) {
    return kEdgeError;
  }
  if ( check_overflow(e1) || check_overflow(e2) ) {
    return kEdgeOverflow;
  }
  if ( check_zero(e2) ) {
    return kEdgeError;
  }
  if ( check_cube(e2) ) {
    return cube_division(e1, e2);
  }
  return gcofactor_step(e1, e2);
}

// constrain演算
// 注: c はkEdge0であってはならない．
tBddEdge
BddMgrModern::gcofactor_step(tBddEdge f,
			     tBddEdge c)
{
  assert_cond( !check_zero(c), __FILE__, __LINE__);

  if ( check_leaf(f) || check_one(c) ) {
    return f;
  }
  if ( f == c ) {
    return kEdge1;
  }
  if ( check_reverse(f, c) ) {
    return kEdge0;
  }
  // この時点で f, c は終端ではない．

  // (~f) / c の結果は ~(f / c) なので f を正規化する．
  tPol f_pol = get_pol(f);
  f = addpol(f, f_pol);
  tBddEdge result = mCofacTable->get(f, c);
  if ( result == kEdgeInvalid ) {
    Node* f_v = get_node(f);
    Node* c_v = get_node(c);
    tPol c_p = get_pol(c);
    Var* f_var = f_v->var();
    Var* c_var = c_v->var();
    tLevel f_level = f_var->level();
    tLevel c_level = c_var->level();

    tBddEdge f_0, f_1;
    if ( f_level <= c_level ) {
      f_0 = f_v->edge0();
      f_1 = f_v->edge1();
    }
    else {
      f_0 = f_1 = f;
    }

    if ( f_level < c_level ) {
      tBddEdge r0 = gcofactor_step(f_0, c);
      tBddEdge r1 = gcofactor_step(f_1, c);
      result = new_node(f_var, r0, r1);
    }
    else {
      tBddEdge c_0 = c_v->edge0(c_p);
      tBddEdge c_1 = c_v->edge1(c_p);
      if ( check_zero(c_0) ) {
	result = gcofactor_step(f_1, c_1);
      }
      else if ( check_zero(c_1) ) {
	result = gcofactor_step(f_0, c_0);
      }
      else {
	tBddEdge r0 = gcofactor_step(f_0, c_0);
	tBddEdge r1 = gcofactor_step(f_1, c_1);
	result = new_node(c_var, r0, r1);
      }
    }
    mCofacTable->put(f, c, result);
  }

  // 極性を元に戻す．
  result = addpol(result, f_pol);

  return result;
}

// 一つの変数に対する cofactor を計算する．
tBddEdge
BddMgrModern::scofactor(tBddEdge e1,
			tVarId id,
			tPol pol)
{
  clear_varmark();
  Var* var = var_of(id);
  if ( !var ) {
    // var がないということは e1 中に含まれていない
    return e1;
  }
  if ( pol == kPolPosi ) {
    var->mMark = 1;
  }
  else {
    var->mMark = 2;
  }
  mLastLevel = var->level();
  tBddEdge ans = cubediv_step(e1);
  mCubedivTable->clear();
  clear_varmark();
  return ans;
}

// C がキューブの場合の拘束演算
tBddEdge
BddMgrModern::cube_division(tBddEdge f,
			    tBddEdge c)
{
  clear_varmark();
  tBddEdge e = c;
  Node* vp = get_node(e);
  tPol pol = get_pol(e);
  mLastLevel = 0;
  while ( vp != 0 ) {
    tBddEdge e0 = vp->edge0(pol);
    tBddEdge e1 = vp->edge1(pol);
    Var* var = vp->var();
    if ( check_zero(e0) ) {
      var->mMark = 1;
      e = e1;
    }
    else {
      var->mMark = 2;
      e = e0;
    }
    mLastLevel = var->level();
    vp = get_node(e);
    pol = get_pol(e);
  }
  tBddEdge ans = cubediv_step(f);
  mCubedivTable->clear();
  clear_varmark();
  return ans;
}

// restricter がキューブの時は必ず O(n):nは第一引数のBDDのサイズ
// でできる
tBddEdge
BddMgrModern::cubediv_step(tBddEdge f)
{
  if ( check_leaf(f) ) {
    return f;
  }
  // この時点で f, g は終端ではない．

  Node* f_vp = get_node(f);
  Var* f_var = f_vp->var();
  if ( f_var->level() > mLastLevel ) {
    return f;
  }

  // (~f) / c の結果は ~(f / c) なので f を正規化する．
  tPol f_pol = get_pol(f);
  f = addpol(f, f_pol);

  tBddEdge result = mCubedivTable->get(f);
  if ( result == kEdgeInvalid ) {
    if ( f_var->mMark == 1 ) {
      // 肯定のリテラルが現れているので 1枝の結果を返す．
      tBddEdge f_1 = f_vp->edge1();
      result = cubediv_step(f_1);
    }
    else if ( f_var->mMark == 2 ) {
      // 否定のリテラルが現れているので 0枝の結果を返す．
      tBddEdge f_0 = f_vp->edge0();
      result = cubediv_step(f_0);
    }
    else {
      // この変数はキューブには現れない
      tBddEdge f_0 = f_vp->edge0();
      tBddEdge f_1 = f_vp->edge1();

      tBddEdge r_0 = cubediv_step(f_0);
      tBddEdge r_1 = cubediv_step(f_1);
      
      result = new_node(f_var, r_0, r_1);
    }
    mCubedivTable->put(f, result);
  }

  // 極性を元に戻す．
  result = addpol(result, f_pol);

  return result;
}

// Davio展開のモーメント項($f_{\overline{x}} \oplus f_x$)を
// 求める処理
// メモリ不足のためにエラーとなる可能性がある．
tBddEdge
BddMgrModern::xor_moment(tBddEdge e,
			 tVarId idx)
{
  if ( check_error(e) ) {
    return kEdgeError;
  }
  if ( check_overflow(e) ) {
    return kEdgeOverflow;
  }
  Var* var = var_of(idx);
  if ( !var ) {
    // この変数がないということは答は0
    return kEdge0;
  }
  mLastLevel = var->level();
  tBddEdge ans = xcofactor_step(e);
  mXcofactorTable->clear();
  return ans;
}

// Davio 展開もモーメント項を求める．
tBddEdge
BddMgrModern::xcofactor_step(tBddEdge f)
{
  if ( check_leaf(f) ) {
    // 終端ならば x でコファクターをとっても違いがないので
    // 答は 0 となる．
    return kEdge0;
  }

  // この時点で e は終端ではない．
  Node* node = get_node(f);
  Var* var = node->var();
  tLevel level = var->level();
  if ( level > mLastLevel ) {
    return kEdge0;
  }

  // f の結果と ~f の結果は等しいので
  // 極性を正規化する．
  tPol f_pol = get_pol(f);
  f = addpol(f, f_pol);

  // まずハッシュを探す．
  tBddEdge result = mXcofactorTable->get(f);
  if ( result == kEdgeInvalid ) {
    // ハッシュになかった．
    tBddEdge e_0 = node->edge0();
    tBddEdge e_1 = node->edge1();
    if ( level == mLastLevel ) {
      result = xor_op(e_0, e_1);
    }
    else { // level < mLastLevel
      tBddEdge r_0 = xcofactor_step(e_0);
      tBddEdge r_1 = xcofactor_step(e_1);
      result = new_node(var, r_0, r_1);
    }
    // ハッシュに登録しておく．
    mXcofactorTable->put(f, result);
  }
  return result;
}

END_NAMESPACE_YM_BDD
