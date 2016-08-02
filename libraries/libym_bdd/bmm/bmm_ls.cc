
/// @file libym_bdd/bmm/bmm_ls.cc
/// @brief リテラル集合を扱う関数の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: bmm_ls.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2006 Yusuke Matsunaga
/// All rights reserved.


#if HAVE_CONFIG_H
#include <ymconfig.h>
#endif

#include "BddMgrModern.h"


BEGIN_NAMESPACE_YM_BDD

BEGIN_NONAMESPACE

// 比較用の関数オブジェクト
struct LitCompLess
{
  bool
  operator()(const Literal& lit1,
	     const Literal& lit2) const
  {
    if ( lit1.varid() < lit2.varid() ) {
      return true;
    }
    if ( lit1.varid() > lit2.varid() ) {
      return false;
    }
    if ( lit1.pol() == kPolPosi && lit2.pol() == kPolNega ) {
      return true;
    }
    return false;
  }
};

// kEdge0 でない方の枝を選ぶ．
// 0枝が選ばれた場合には which = 0
// 1枝が選ばれた場合には which = 1 とする．
inline
tBddEdge
select_edge(BmmNode* node,
	    tPol pol,
	    int& which)
{
  tBddEdge e0 = node->edge0(pol);
  tBddEdge e1 = node->edge1(pol);
  if ( e0 == kEdge0 ) {
    which = 1;
    return e1;
  }
  else {
    which = 0;
    return e0;
  }
}

END_NONAMESPACE

// 2つのリテラル集合の集合積を求める．
// メモリ不足のためにエラーとなる可能性がある．
tBddEdge
BddMgrModern::lscap(tBddEdge e1,
		    tBddEdge e2)
{
  if ( check_error(e1) || check_error(e2) ) {
    return kEdgeError;
  }
  if ( check_overflow(e1) || check_overflow(e2) ) {
    return kEdgeOverflow;
  }
  if ( check_zero(e1) || check_zero(e2) ) {
    return kEdgeError;
  }
  if ( check_one(e1) || check_one(e2) ) {
    return kEdge1;
  }

  Node* node1 = get_node(e1);
  Node* node2 = get_node(e2);
  tPol pol1 = get_pol(e1);
  tPol pol2 = get_pol(e2);
  Var* var1 = node1->var();
  Var* var2 = node2->var();
  tLevel level1 = var1->level();
  tLevel level2 = var2->level();
  for ( ; ; ) {
    if ( level1 == level2 ) {
      int which1;
      e1 = select_edge(node1, pol1, which1);
      int which2;
      e2 = select_edge(node2, pol2, which2);
      if ( which1 == which2 ) {
	tBddEdge tmp = lscap(e1, e2);
	if ( which1 == 0 ) {
	  return new_node(var1, tmp, kEdge0);
	}
	else {
	  return new_node(var1, kEdge0, tmp);
	}
      }
      if ( check_one(e1) || check_one(e2) ) {
	return kEdge1;
      }
      node1 = get_node(e1);
      node2 = get_node(e2);
      pol1 = get_pol(e1);
      pol2 = get_pol(e2);
      var1 = node1->var();
      var2 = node2->var();
      level1 = var1->level();
      level2 = var2->level();
    }
    else if ( level1 < level2 ) {
      int which1;
      e1 = select_edge(node1, pol1, which1);
      if ( check_one(e1) ) {
	return kEdge1;
      }
      node1 = get_node(e1);
      pol1 = get_pol(e1);
      var1 = node1->var();
      level1 = var1->level();
    }
    else { // level1 > level2
      int which2;
      e2 = select_edge(node2, pol2, which2);
      if ( check_one(e2) ) {
	return kEdge1;
      }
      node2 = get_node(e2);
      pol2 = get_pol(e2);
      var2 = node2->var();
      level2 = var2->level();
    }
  }
}

// e1 のリテラル集合から e2 のリテラル集合を引く．(集合差演算)
// 同じ変数番号であっても極性が異なれば異なる要素とみなす．
// lsdiff() のためのサブルーティン
tBddEdge
BddMgrModern::lsdiff(tBddEdge e1,
		     tBddEdge e2)
{
  if ( check_error(e1) || check_error(e2) ) {
    return kEdgeError;
  }
  if ( check_overflow(e1) || check_overflow(e2) ) {
    return kEdgeOverflow;
  }
  if ( check_zero(e1) || check_zero(e2) ) {
    return kEdgeError;
  }
  if ( check_one(e1) || check_one(e2) ) {
    return e1;
  }

  Node* node1 = get_node(e1);
  Node* node2 = get_node(e2);
  tPol pol1 = get_pol(e1);
  tPol pol2 = get_pol(e2);
  Var* var1 = node1->var();
  Var* var2 = node2->var();
  tLevel level1 = var1->level();
  tLevel level2 = var2->level();
  for ( ; ; ) {
    if ( level1 < level2 ) {
      int which1;
      e1 = select_edge(node1, pol1, which1);
      tBddEdge tmp = lsdiff(e1, e2);
      if ( which1 == 0 ) {
	return new_node(var1, tmp, kEdge0);
      }
      else {
	return new_node(var1, kEdge0, tmp);
      }
    }
    if ( level1 > level2 ) {
      int which2;
      e2 = select_edge(node2, pol2, which2);
      if ( check_one(e2) ) {
	return e1;
      }
      node2 = get_node(e2);
      pol2 = get_pol(e2);
      var2 = node2->var();
      level2 = var2->varid();
    }
    else {
      int which1;
      e1 = select_edge(node1, pol1, which1);
      int which2;
      e2 = select_edge(node2, pol2, which2);
      if ( which1 != which2 ) {
	tBddEdge tmp = lsdiff(e1, e2);
	if ( which1 == 0 ) {
	  return new_node(var1, tmp, kEdge0);
	}
	else {
	  return new_node(var1, kEdge0, tmp);
	}
      }
      if ( check_one(e1) || check_one(e2) ) {
	return e1;
      }
      node1 = get_node(e1);
      node2 = get_node(e2);
      pol1 = get_pol(e1);
      pol2 = get_pol(e2);
      var1 = node1->var();
      var2 = node2->var();
      level1 = var1->level();
      level2 = var2->level();
    }
  }
}

// e1 の変数集合と e2 の変数集合が共通部分を持っていたら true を返す．
bool
BddMgrModern::lsintersect(tBddEdge e1,
			  tBddEdge e2)
{
  if ( check_error(e1) || check_error(e2) ) {
    return false;
  }
  if ( check_overflow(e1) || check_overflow(e2) ) {
    return false;
  }
  if ( check_zero(e1) || check_zero(e2) ) {
    return false;
  }
  if ( check_one(e1) || check_one(e2) ) {
    return false;
  }

  Node* node1 = get_node(e1);
  Node* node2 = get_node(e2);
  tPol pol1 = get_pol(e1);
  tPol pol2 = get_pol(e2);
  tLevel level1 = node1->level();
  tLevel level2 = node2->level();
  for ( ; ; ) {
    if ( level1 == level2 ) {
      int which1;
      e1 = select_edge(node1, pol1, which1);
      int which2;
      e2 = select_edge(node2, pol2, which2);
      if ( which1 == which2 ) {
	return true;
      }
      if ( check_one(e1) || check_one(e2) ) {
	return false;
      }
      node1 = get_node(e1);
      node2 = get_node(e2);
      pol1 = get_pol(e1);
      pol2 = get_pol(e2);
      level1 = node1->level();
      level2 = node2->level();
    }
    else if ( level1 < level2 ) {
      int which1;
      e1 = select_edge(node1, pol1, which1);
      if ( check_one(e1) ) {
	return false;
      }
      node1 = get_node(e1);
      pol1 = get_pol(e1);
      level1 = node1->level();
    }
    else {
      int which2;
      e2 = select_edge(node2, pol2, which2);
      if ( check_one(e2) ) {
	return false;
      }
      node2 = get_node(e2);
      pol2 = get_pol(e2);
      level2 = node2->level();
    }
  }
}

// リテラルの vector に変換する．
tVarSize
BddMgrModern::to_literalvector(tBddEdge e,
			       LiteralVector& dst)
{
  dst.clear();

  if ( check_invalid(e) ) {
    return 0;
  }

  size_t n = size(e);
  dst.reserve(n);

  Node* vp = get_node(e);
  tPol pol = get_pol(e);
  while ( vp ) {
    tVarId varid = vp->varid();
    tBddEdge e0 = vp->edge0(pol);
    tBddEdge e1 = vp->edge1(pol);
    if ( e0 == kEdge0 ) {
      dst.push_back(Literal(varid, kPolPosi));
      vp = get_node(e1);
      pol = get_pol(e1);
    }
    else {
      dst.push_back(Literal(varid, kPolNega));
      vp = get_node(e0);
      pol = get_pol(e0);
    }
  }
  sort(dst.begin(), dst.begin(), LitCompLess());
  
  return n;
}

// リテラルの list に変換する．
tVarSize
BddMgrModern::to_literallist(tBddEdge e,
			     LiteralList& dst)
{
  dst.clear();

  if ( check_invalid(e) ) {
    return 0;
  }

  Node* vp = get_node(e);
  tPol pol = get_pol(e);
  while ( vp ) {
    tVarId varid = vp->varid();
    tBddEdge e0 = vp->edge0(pol);
    tBddEdge e1 = vp->edge1(pol);
    if ( e0 == kEdge0 ) {
      dst.push_back(Literal(varid, kPolPosi));
      vp = get_node(e1);
      pol = get_pol(e1);
    }
    else {
      dst.push_back(Literal(varid, kPolNega));
      vp = get_node(e0);
      pol = get_pol(e0);
    }
  }
  dst.sort(LitCompLess());
  
  return dst.size();
}

END_NAMESPACE_YM_BDD
