
/// @file libym_bdd/bmm/bmm_symmetry.cc
/// @brief 対称性をチェックする関数の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: bmm_symmetry.cc 700 2007-05-31 00:41:30Z matsunaga $
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

tLevel x_level;
tLevel y_level;

tBddEdge xy_edge;
tBddEdge y_edge;

END_NONAMESPACE

//
// xを含まないパスeがyも含まなければkEdge1(Trueのつもり)を返す．
//
tBddEdge
BddMgrModern::cs_step2(tBddEdge e)
{
  Node* vp = get_node(e);
  if ( vp == 0 ) {
    return kEdge1;
  }
  tLevel level = vp->level();
  if ( level > y_level ) {
    return kEdge1;
  }
  else if ( level == y_level ) {
    return kEdge0;
  }

  tBddEdge e2 = combine(vp, kPolPosi);
  tBddEdge result = mCs2Table->get(e2, y_edge);
  if ( result == kEdgeInvalid ) {
    result = cs_step2(vp->edge0());
    if ( result == kEdge1 ) {
      result = cs_step2(vp->edge1());
    }
    mCs2Table->put(e2, y_edge, result);
  }
  return result;
}
  
// xを0にしたパスe1と1にしたパスe2をくらべて，それぞれyを1/0にした結果が
// 同一ならばkEdge1(True)を返す．つまりこのパスに関してはxとyは対称
tBddEdge
BddMgrModern::cs_step1(tBddEdge e1,
		       tBddEdge e2,
		       tPol sympol)
{
  Node* vp1 = get_node(e1);
  Node* vp2 = get_node(e2);
  tLevel level1 = vp1 ? vp1->level() : kLevelMax;
  tLevel level2 = vp2 ? vp2->level() : kLevelMax;
  tLevel top_level = level1;
  if ( top_level > level2 ) {
    top_level = level2;
  }
  if ( e1 == e2 ) {
    return cs_step2(e1);
  }
  if ( top_level > y_level ) {
    return kEdge0;
  }

  tBddEdge result = mCs1Table->get(e1, e2, xy_edge);
  if ( result == kEdgeInvalid ) {
    tBddEdge e10;
    tBddEdge e11;
    tBddEdge e20;
    tBddEdge e21;
    if ( level1 == top_level ) {
      tPol pol1 = get_pol(e1);
      e10 = vp1->edge0(pol1);
      e11 = vp1->edge1(pol1);
    }
    else {
      e10 = e11 = e1;
    }
    if ( level2 == top_level ) {
      tPol pol2 = get_pol(e2);
      e20 = vp2->edge0(pol2);
      e21 = vp2->edge1(pol2);
    }
    else {
      e20 = e21 = e2;
    }
    if ( top_level < y_level ) {
      result = cs_step1(e10, e20, sympol);
      if ( result == kEdge1 ) {
	result = cs_step1(e11, e21, sympol);
      }
    }
    else { // top_level == y_level
      if ( sympol == kPolPosi ) {
	result = (e11 == e20) ? kEdge1 : kEdge0;
      }
      else {
	result = (e10 == e21) ? kEdge1 : kEdge0;
      }
    }
    mCs1Table->put(e1, e2, xy_edge, result);
  }
  return result;
}

tBddEdge
BddMgrModern::cs_step(tBddEdge e,
		      tPol sympol)
 {
  Node* vp = get_node(e);
  if ( vp == 0 ) {
    return kEdge1;
  }
  tLevel level = vp->level();
  if ( level > y_level ) {
    return kEdge1;
  }
  else if ( level == y_level ) {
    // ここまでのパスにxが含まれず，yが含まれているので false
    return kEdge0;
  }

  // 極性は落としてしまう．
  e = combine(vp, kPolPosi);
  tBddEdge result = mCsTable->get(e, xy_edge);
  if ( result == kEdgeInvalid ) {
    if ( level < x_level ) {
      result = cs_step(vp->edge0(), sympol);
      if ( result == kEdge1 ) {
	result = cs_step(vp->edge1(), sympol);
      }
    }
    else if ( level == x_level ) {
      result = cs_step1(vp->edge0(), vp->edge1(), sympol);
    }
    else {
      result = cs_step2(vp->edge0());
      if ( result == kEdge1 ) {
	result = cs_step2(vp->edge1());
      }
    }
    mCsTable->put(e, xy_edge, result);
  }
  return result;
}

// 関数 f に対して変数 x, y が互いに対称かを調べる．
// 結果はブール値であり新たなノードを作るわけではないが，
// 演算結果テーブルが拡張される可能性があるのでメモリ不足によって
// エラーとなる可能性がある．
bool
BddMgrModern::check_symmetry(tBddEdge e,
			     tVarId x,
			     tVarId y,
			     tPol pol)
{
  x_level = level(x);
  y_level = level(y);
  if ( y_level < x_level ) {
    tLevel tmp = x_level;
    x_level = y_level;
    y_level = tmp;
  }

  y_edge = BddMgr::make_bdd(y, kEdge0, kEdge1);
  xy_edge = BddMgr::make_bdd(x, kEdge0, addpol(y_edge, pol));
  activate(xy_edge);
  tBddEdge ans = cs_step(e, pol);
  deactivate(xy_edge);
  return ans == kEdge1;
}

END_NAMESPACE_YM_BDD
