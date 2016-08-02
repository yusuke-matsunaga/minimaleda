
/// @file libym_bdd/bmm/bmm_sup.cc
/// @brief サポート変数を求める関数の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: bmm_sup.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2006 Yusuke Matsunaga
/// All rights reserved.


#if HAVE_CONFIG_H
#include <ymconfig.h>
#endif

#include "BddMgrModern.h"
#include "BmmCompTbl.h"


BEGIN_NAMESPACE_YM_BDD

// eを根とするBDDのサポートに印をつける．
tVarSize
BddMgrModern::mark_support(tBddEdge e)
{
  clear_varmark();
  sup_step(e);
  clear_pnmark(e);
  return mVarSet.size();
}

// edge_list に含まれる枝を根とするBDDのサポートに印をつける．
tVarSize
BddMgrModern::mark_support(const list<tBddEdge>& edge_list)
{
  clear_varmark();
  // サポート変数にマークをつける．
  for (list<tBddEdge>::const_iterator p = edge_list.begin();
       p != edge_list.end(); ++ p) {
    sup_step(*p);
  }
  for (list<tBddEdge>::const_iterator p = edge_list.begin();
       p != edge_list.end(); ++ p) {
    clear_pnmark(*p);
  }
  return mVarSet.size();
}

// サポート変数に印をつける．
void
BddMgrModern::sup_step(tBddEdge e)
{
  for ( ; ; ) {
    Node* vp = get_node(e);
    if ( vp == 0 || vp->pmark() ) {
      return;
    }

    vp->pmark(1);
    Var* v = vp->var();
    if ( !v->mMark ) {
      v->mMark = 1;
      mVarSet.push_back(v);
    }
    sup_step(vp->edge0());
    e = vp->edge1();
  }
}

// var_mark を列挙してマークのついた変数を vars に入れる．
tVarSize
BddMgrModern::mark_to_vector(VarVector& vars)
{
  tVarSize n = mVarSet.size();
  vars.clear();
  vars.reserve(n);
  for (list<Var*>::iterator p = mVarSet.begin(); p != mVarSet.end(); ++ p) {
    Var* v = *p;
    tVarId varid = v->varid();
    vars.push_back(varid);
  }
  clear_varmark();
  sort(vars.begin(), vars.end());
  return vars.size();
}

// var_mark を列挙してマークのついた変数を vars に入れる．
tVarSize
BddMgrModern::mark_to_list(VarList& vars)
{
  vars.clear();
  for (list<Var*>::iterator p = mVarSet.begin(); p != mVarSet.end(); ++ p) {
    Var* v = *p;
    tVarId varid = v->varid();
    vars.push_back(varid);
  }
  clear_varmark();
  vars.sort();
  return vars.size();
}

// var_mark を列挙してマークのついた変数を vars に入れる．
tBddEdge
BddMgrModern::mark_to_bdd()
{
  if ( mVarSet.empty() ) {
    return kEdge1;
  }
  list<Var*>::iterator p = mVarSet.begin();
  tBddEdge tmp = make_posiliteral((*p)->varid());
  for (++ p; p != mVarSet.end(); ++ p) {
    tmp = and_op(tmp, make_posiliteral((*p)->varid()));
  }
  clear_varmark();
  return tmp;
}

// Smallest Cube Containing F を求める．
// メモリ不足のためにエラーとなる可能性がある．
tBddEdge
BddMgrModern::SCC(tBddEdge e)
{
  if ( check_error(e) ) {
    return kEdgeError;
  }
  if ( check_overflow(e) ) {
    return kEdgeOverflow;
  }
  if ( check_one(e) ) {
    return kEdge1;
  }
  if ( check_zero(e) ) {
    return kEdge0;
  }

  // まずサポートを求める．
  mark_support(e);
  tBddEdge sup = mark_to_bdd();

  // サポートを使って SCC を求める．
  clear_varmark();
  scc_step(e, sup);
  clear_pnmark(e);

  tBddEdge ans = kEdge1;
  for (list<Var*>::iterator p = mVarSet.begin(); p != mVarSet.end(); ++ p) {
    Var* v = *p;
    if ( v->mMark == 1 ) {
      ans = and_op(ans, make_negaliteral(v->varid()));
    }
    else if ( v->mMark == 2 ) {
      ans = and_op(ans, make_posiliteral(v->varid()));
    }
  }
  clear_varmark();

  return ans;
}

// Smallest Cube Containing F を求めるためのサブルーティン．
void
BddMgrModern::scc_step(tBddEdge e,
		       tBddEdge s)
{
  if ( check_one(e) ) {
    while ( !check_one(s) ) {
      assert_cond(!check_zero(s), __FILE__, __LINE__);
      Node* svp = get_node(s);
      Var* svar = svp->var();
      svar->mMark |= 3;
      s = svp->edge1();
    }
    return;
  }

  Node* vp = get_node(e);
  if ( vp && !mark(e) ) {
    setmark(e);
    Var* var = vp->var();
    Node* svp = get_node(s);
    Var* svar = svp->var();
    tBddEdge s2 = svp->edge1();
    if ( svar->level() < var->level() ) {
      svar->mMark |= 3;
      scc_step(e, s2);
    }
    else {
      tPol pol = get_pol(e);
      tBddEdge e0 = vp->edge0(pol);
      tBddEdge e1 = vp->edge1(pol);
      if ( !var->mMark ) {
	mVarSet.push_back(var);
      }
      if ( !check_zero(e0) ) {
	var->mMark |= 1;
	scc_step(e0, s2);
      }
      if ( !check_zero(e1) ) {
	var->mMark |= 2;
	scc_step(e1, s2);
      }
    }
  }
}

// existential quntification(smoothing)
// svars に含まれる変数を消去する．
// メモリ不足のためにエラーとなる可能性がある．
tBddEdge
BddMgrModern::esmooth(tBddEdge e1,
		      tBddEdge e2)
{
  if ( check_error(e1) || check_error(e2) ) {
    return kEdgeError;
  }
  if ( check_overflow(e1) || check_overflow(e2) ) {
    return kEdgeOverflow;
  }

  clear_varmark();
  mLastLevel = 0;
  {
    Node* vp = get_node(e2);
    while ( vp != 0 ) {
      Var* v = vp->var();
      mLastLevel = v->level();
      v->mMark = 1;
      vp = get_node(vp->edge1());
    }
  }
  tBddEdge ans = esmooth_step(e1);
  mSmTable->clear();
  clear_varmark();
  return ans;
}

tBddEdge
BddMgrModern::esmooth_step(tBddEdge e)
{
  if ( check_leaf(e) ) {
    return e;
  }

  Node* vp = get_node(e);
  Var* var = vp->var();
  tLevel level = var->level();
  if ( level > mLastLevel ) {
    return e;
  }

  tBddEdge result = mSmTable->get(e);
  if ( result == kEdgeInvalid ) {
    tPol pol = get_pol(e);
    tBddEdge e0 = vp->edge0(pol);
    tBddEdge e1 = vp->edge1(pol);
    if ( var->mMark ) {
      // 消去対象の変数だった．
      tBddEdge tmp = or_op(e0, e1);
      result = esmooth_step(tmp);
    }
    else {
      tBddEdge r_0 = esmooth_step(e0);
      tBddEdge r_1 = esmooth_step(e1);
      result = new_node(var, r_0, r_1);
    }
    mSmTable->put(e, result);
  }

  return result;
}

// src1 と src2 との論理積を計算しつつスムージングを行う．
tBddEdge
BddMgrModern::and_exist(tBddEdge e1,
			tBddEdge e2,
			tBddEdge e3)
{
  if ( check_error(e1) || check_error(e2) || check_error(e3) ) {
    return kEdgeError;
  }
  if ( check_overflow(e1) || check_overflow(e2) || check_overflow(e3) ) {
    return kEdgeOverflow;
  }

  clear_varmark();
  mLastLevel = 0;
  {
    Node* vp = get_node(e3);
    while ( vp != 0 ) {
      Var* v = vp->var();
      mLastLevel = v->level();
      v->mMark = 1;
      vp = get_node(vp->edge1());
    }
  }
  tBddEdge ans = andexist_step(e1, e2);
  mSmTable->clear();
  mAeTable->clear();
  clear_varmark();
  return ans;
}

tBddEdge
BddMgrModern::andexist_step(tBddEdge f,
			    tBddEdge g)
{
  if ( check_zero(f) || check_zero(g) ) {
    return kEdge0;
  }
  if ( check_one(f) && check_one(g) ) {
    return kEdge1;
  }
  if ( check_one(f) ) {
    return esmooth_step(g);
  }
  if ( check_one(g) ) {
    return esmooth_step(f);
  }

  if ( f > g ) {
    tBddEdge tmp = f;
    f = g;
    g = tmp;
  }

  Node* f_vp = get_node(f);
  Node* g_vp = get_node(g);
  tPol f_pol = get_pol(f);
  tPol g_pol = get_pol(g);
  Var* f_var = f_vp->var();
  Var* g_var = g_vp->var();
  tLevel f_level = f_var->level();
  tLevel g_level = g_var->level();
  tLevel level = f_level;
  Var* var = f_var;
  if ( level > g_level ) {
    level = g_level;
    var = g_var;
  }
  if ( level > mLastLevel ) {
    return and_op(f, g);
  }

  tBddEdge result = mAeTable->get(f, g);
  if ( result == kEdgeInvalid ) {
    tBddEdge f_0, f_1;
    tBddEdge g_0, g_1;
    if ( var->mMark ) {
      if ( f_level > level ) {
	g_0 = g_vp->edge0(g_pol);
	g_1 = g_vp->edge1(g_pol);
	tBddEdge tmp = or_op(g_0, g_1);
	result = andexist_step(f, tmp);
      }
      else if ( g_level > level ) {
	f_0 = f_vp->edge0(f_pol);
	f_1 = f_vp->edge1(f_pol);
	tBddEdge tmp = or_op(f_0, f_1);
	result = andexist_step(tmp, g);
      }
      else {
	f_0 = f_vp->edge0(f_pol);
	f_1 = f_vp->edge1(f_pol);
	g_0 = g_vp->edge0(g_pol);
	g_1 = g_vp->edge1(g_pol);
	tBddEdge tmp1 = andexist_step(f_0, g_0);
	tBddEdge tmp2 = andexist_step(f_1, g_1);
	result = or_op(tmp1, tmp2);
      }
    }
    else {
      f_0 = f_vp->edge0(f_pol);
      f_1 = f_vp->edge1(f_pol);
      g_0 = g_vp->edge0(g_pol);
      g_1 = g_vp->edge1(g_pol);
      tBddEdge r_0 = andexist_step(f_0, g_0);
      tBddEdge r_1 = andexist_step(f_1, g_1);
      result = new_node(var, r_0, r_1);
    }
    mAeTable->put(f, g, result);
  }

  return result;
}

END_NAMESPACE_YM_BDD
