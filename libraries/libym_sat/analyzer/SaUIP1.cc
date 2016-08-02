
/// @file libym_sat/SaUIP1.cc
/// @brief SaUIP1 の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: SaUIP1.cc 1293 2008-02-21 02:25:52Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "SaUIP1.h"
#include "SatSolverImpl.h"
#include "SatClause.h"


BEGIN_NAMESPACE_YM_SAT

// @brief コンストラクタ
SaUIP1::SaUIP1()
{
}

// @brief デストラクタ
SaUIP1::~SaUIP1()
{
}

// conflict を解析する．
int
SaUIP1::analyze(SatReason creason,
		vector<Literal>& learnt)
{
  capture(creason, learnt);
  simplify(learnt);
  clear_marks();
  return reorder(learnt);
}

// creason の矛盾の原因になっている割り当てのうち，
// - もっとも近い unique identification point
// - 現在のレベルよりも低いレベルの割り当て
// からなるセパレータ集合を learnt に入れる．
void
SaUIP1::capture(SatReason creason,
		vector<Literal>& learnt)
{
  learnt.clear();
  learnt.push_back(Literal()); // place holder

  size_t first_pos = 0;
  size_t count = 0;
  size_t last = last_assign();
  for ( ; ; ) {
    if ( creason.is_clause() ) {
      SatClause& cclause = creason.clause();

      // cclause が学習節なら activity をあげる．
      if ( cclause.is_learnt() ) {
	bump_clause_activity(&cclause);
      }

      // cclause 節に含まれるリテラルが以前の decision level
      // で割り当てられていたら学習節に加える．
      // 現在の decision level なら count を増やすだけ．
      // あとで mAssignList をたどれば該当のリテラルは捜し出せる．
      size_t n = cclause.size();
      for (size_t i = first_pos; i < n; ++ i) {
	Literal q = cclause.lit(i);
	tVarId var = q.varid();
	int var_level = decision_level(var);
	if ( !get_mark(var) && var_level > 0 ) {
	  set_mark_and_putq(var);
	  bump_var_activity(var);
	  if ( var_level < decision_level() ) {
	    learnt.push_back(q);
	  }
	  else {
	    ++ count;
	  }
	}
      }
    }
    else {
      assert_cond(first_pos == 1, __FILE__, __LINE__);
      Literal q = creason.literal();
      tVarId var = q.varid();
      int var_level = decision_level(var);
      if ( !get_mark(var) && var_level > 0 ) {
	set_mark_and_putq(var);
	bump_var_activity(var);
	if ( var_level < decision_level() ) {
	  learnt.push_back(q);
	}
	else {
	  ++ count;
	}
      }
    }
    first_pos = 1;

    // mAssignList に入っている最近の変数で mark の付いたものを探す．
    for ( ; ; -- last) {
      Literal q = get_assign(last);
      tVarId var = q.varid();
      if ( get_mark(var) ) {
	set_mark(var, false);
	learnt[0] = ~q;
	creason = reason(q.varid());
	-- last;
	-- count;
	break;
      }
#if defined(DEBUG)
      // ここは重いのでコメントアウトしておく
      assert_cond(last > 0, __FILE__, __LINE__);
#endif
    }
    if ( count == 0 ) {
      // q は first UIP だった．
      break;
    }
  }
}

END_NAMESPACE_YM_SAT
