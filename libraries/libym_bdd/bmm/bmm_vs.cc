
/// @file libym_bdd/bmm/bmm_vs.cc
/// @brief 変数集合を扱う関数の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: bmm_vs.cc 700 2007-05-31 00:41:30Z matsunaga $
///
/// Copyright (C) 2005-2006 Yusuke Matsunaga
/// All rights reserved.


#if HAVE_CONFIG_H
#include <ymconfig.h>
#endif

#include "BddMgrModern.h"


BEGIN_NAMESPACE_YM_BDD

// 2つの変数集合の集合積を求める．
// メモリ不足のためにエラーとなる可能性がある．
tBddEdge
BddMgrModern::vscap(tBddEdge e1,
		    tBddEdge e2)
{
  if ( check_error(e1) || check_error(e2) ) {
    return kEdgeError;
  }
  if ( check_overflow(e1) || check_overflow(e2) ) {
    return kEdgeOverflow;
  }
  if ( check_one(e1) || check_one(e2) ) {
    return kEdge1;
  }
  if ( check_zero(e1) || check_zero(e2) ) {
    return kEdgeError;
  }

  Node* node1 = get_node(e1);
  Node* node2 = get_node(e2);
  Var* var1 = node1->var();
  Var* var2 = node2->var();
  tLevel level1 = var1->level();
  tLevel level2 = var2->level();
  for ( ; ; ) {
    if ( level1 == level2 ) {
      tBddEdge tmp = vscap(node1->edge1(), node2->edge1());
      return new_node(var1, kEdge0, tmp);
    }
    else if ( level1 < level2 ) {
      e1 = node1->edge1();
      if ( check_one(e1) ) {
	return kEdge1;
      }
      node1 = get_node(e1);
      var1 = node1->var();
      level1 = var1->level();
    }
    else { // level1 > level2
      e2 = node2->edge1();
      if ( check_one(e2) ) {
	return kEdge1;
      }
      node2 = get_node(e2);
      var2 = node2->var();
      level2 = var2->level();
    }
  }
}

// e1 の変数集合から e2 の変数集合を引く．(集合差演算)
tBddEdge
BddMgrModern::vsdiff(tBddEdge e1,
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
  if ( check_one(e1) ) {
    return kEdge1;
  }
  if ( check_one(e2) ) {
    return e1;
  }

  Node* node1 = get_node(e1);
  Node* node2 = get_node(e2);
  Var* var1 = node1->var();
  Var* var2 = node2->var();
  tLevel level1 = var1->level();
  tLevel level2 = var2->level();
  for ( ; ; ) {
    if ( level1 < level2 ) {
      tBddEdge tmp = vsdiff(node1->edge1(), e2);
      return new_node(var1, kEdge0, tmp);
    }
    if ( level1 > level2 ) {
      e2 = node2->edge1();
      if ( e2 == kEdge1 ) {
	return e1;
      }
      node2 = get_node(e2);
      var2 = node2->var();
      level2 = var2->level();
    }
    else {
      e1 = node1->edge1();
      e2 = node2->edge1();
      if ( check_one(e1) ) {
	return kEdge1;
      }
      if ( check_one(e2) ) {
	return e1;
      }
      node1 = get_node(e1);
      node2 = get_node(e2);
      var1 = node1->var();
      var2 = node2->var();
      level1 = var1->level();
      level2 = var2->level();
    }
  }
}

// e1 の変数集合と e2 の変数集合が共通部分を持っていたら true を返す．
bool
BddMgrModern::vsintersect(tBddEdge e1,
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
  tLevel level1 = node1->level();
  tLevel level2 = node2->level();
  for ( ; ; ) {
    if ( level1 == level2 ) {
      return true;
    }
    if ( level1 < level2 ) {
      e1 = node1->edge1();
      if ( check_one(e1) ) {
	return false;
      }
      node1 = get_node(e1);
      level1 = node1->level();
    }
    else {
      e2 = node2->edge1();
      if ( check_one(e2) ) {
	return false;
      }
      node2 = get_node(e2);
      level2 = node2->level();
    }
  }
}

END_NAMESPACE_YM_BDD
