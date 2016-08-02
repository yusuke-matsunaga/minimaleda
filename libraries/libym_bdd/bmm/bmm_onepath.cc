
/// @file libym_bdd/bmm/bmm_onepath.cc
/// @brief パスを求める関数の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: bmm_onepath.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2006 Yusuke Matsunaga
/// All rights reserved.


#if HAVE_CONFIG_H
#include <ymconfig.h>
#endif

#include "BddMgrModern.h"


BEGIN_NAMESPACE_YM_BDD

// '1' の終端へ至る経路をひとつとってくる．
// 結果はその経路を表す論理積の形のBDDで返される．
tBddEdge
BddMgrModern::onepath(tBddEdge e)
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

  Node* vp = get_node(e);
  tPol pol = get_pol(e);
  Var* var = vp->var();
  tBddEdge l = vp->edge0(pol);
  tBddEdge h = vp->edge1(pol);
  tBddEdge result;
  if ( check_zero(h) ) {
    tBddEdge chd = onepath(l);
    result = new_node(var, chd, kEdge0);
  }
  else {
    tBddEdge chd = onepath(h);
    result = new_node(var, kEdge0, chd);
  }
  return result;
}

// 最短の1パスを求める．
// 結果はその経路を表す論理積の形のBDDで返される．
tBddEdge
BddMgrModern::shortest_onepath(tBddEdge e)
{
  if ( check_error(e) ) {
    return kEdgeError;
  }
  if ( check_overflow(e) ) {
    return kEdgeOverflow;
  }
  
  hash_map<tBddEdge, tBddEdge> sp_assoc;
  return sp_step(e, sp_assoc);
}

tBddEdge
BddMgrModern::sp_step(tBddEdge e,
		      hash_map<tBddEdge, tBddEdge>& sp_assoc)
{
  if ( check_one(e) ) {
    return kEdge1;
  }
  if ( check_zero(e) ) {
    return kEdge0;
  }

  tBddEdge result;
  hash_map<tBddEdge, tBddEdge>::iterator p = sp_assoc.find(e);
  if ( p == sp_assoc.end() ) {
    Node* vp = get_node(e);
    tPol pol = get_pol(e);
    tBddEdge l = sp_step(vp->edge0(pol), sp_assoc);
    tBddEdge h = sp_step(vp->edge1(pol), sp_assoc);
    ymint llen = sp_len(l);
    ymint hlen = sp_len(h);
    if ( hlen != -1 && llen > hlen + 1 ) {
      l = kEdge0;
    }
    else if ( llen != -1 && llen < hlen + 1 ) {
      h = kEdge0;
    }
    result = new_node(vp->var(), l, h);
    sp_assoc[e] = result;
  }
  else {
    result = p->second;
  }
  return result;
}

ymint
BddMgrModern::sp_len(tBddEdge e)
{
  if ( check_zero(e) ) {
    return -1;
  }
  int len = 0;
  while ( !check_one(e) ) {
    Node* vp = get_node(e);
    tPol pol = get_pol(e);
    tBddEdge l = vp->edge0(pol);
    if ( check_zero(l) ) {
      len ++;
      e = vp->edge1(pol);
    }
    else {
      e = l;
    }
  }
  return len;
}

// 最短1パスの長さを求める．
// 演算結果テーブルを拡張しようとしてエラーとなる可能性がある．
tVarSize
BddMgrModern::shortest_onepath_len(tBddEdge e)
{
  if ( check_invalid(e) ) {
    return 0;
  }
  hash_map<tBddEdge, ymint> assoc;
  ymint tmp = spl_step(e, assoc);
  assert_cond(tmp >= 0, __FILE__, __LINE__);
  return static_cast<tVarSize>(tmp);
}

ymint
BddMgrModern::spl_step(tBddEdge e,
		       hash_map<tBddEdge, ymint>& assoc)
{
  if ( check_one(e) ) {
    return 0;
  }
  if ( check_zero(e) ) {
    return -1;
  }

  ymint result;
  hash_map<tBddEdge, ymint>::iterator p = assoc.find(e);
  if ( p == assoc.end() ) {
    Node* vp = get_node(e);
    tPol pol = get_pol(e);
    ymint ans1 = spl_step(vp->edge0(pol), assoc);
    ymint ans2 = spl_step(vp->edge1(pol), assoc) + 1;
    if ( ans1 != -1 && ans1 < ans2 ) {
      result = ans1;
    }
    else {
      result = ans2;
    }
    assoc[e] = result;
  }
  else {
    result = p->second;
  }
  return result;
}

END_NAMESPACE_YM_BDD
