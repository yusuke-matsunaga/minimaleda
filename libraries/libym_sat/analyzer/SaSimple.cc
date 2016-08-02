
/// @file libym_sat/SaSimple.cc
/// @brief SaSimple の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: SaSimple.cc 1293 2008-02-21 02:25:52Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "SaSimple.h"
#include "SatSolverImpl.h"
#include "SatClause.h"


BEGIN_NAMESPACE_YM_SAT

// @brief コンストラクタ
SaSimple::SaSimple()
{
}

// @brief デストラクタ
SaSimple::~SaSimple()
{
}

// conflict を解析する．
int
SaSimple::analyze(SatReason creason,
		  vector<Literal>& learnt)
{
  capture(creason, learnt);
  simplify(learnt);
  clear_marks();
  return reorder(learnt);
}

void
SaSimple::capture(SatReason creason,
		  vector<Literal>& learnt)
{
  Literal p;
  size_t c = 0;
  size_t last = last_assign();
  for ( ; ; ) {
    if ( creason.is_clause() ) {
      SatClause& cclause = creason.clause();

      // cclause が学習節なら activity をあげる．
      if ( cclause.is_learnt() ) {
	bump_clause_activity(&cclause);
      }

      // conflict 節に含まれるリテラルが以前の decision level
      // で割り当てられていたら学習節に加える．
      size_t n = cclause.size();
      for (size_t i = 0; i < n; ++ i) {
	Literal q = cclause.lit(i);
	if ( q == p ) {
	  continue;
	}
	tVarId var = q.varid();
	if ( !get_mark(var) && decision_level(var) > 0 ) {
	  set_mark_and_putq(var);
	  bump_var_activity(var);
	  ++ c;
	}
      }
      if ( c == 0 ) {
	break;
      }
    }
    else {
      Literal q = creason.literal();
      if ( q != p ) {
	tVarId var = q.varid();
	if ( !get_mark(var) && decision_level(var) > 0 ) {
	  set_mark_and_putq(var);
	  bump_var_activity(var);
	  ++ c;
	}
      }
    }

    // mAssignList に入っている最近の変数で mark の付いたものを探す．
    for ( ; ; ) {
      Literal q = get_assign(last);
      -- last;
      tVarId var = q.varid();
      if ( get_mark(var) ) {
	set_mark(var, false);
	-- c;
	creason = reason(q.varid());
	if ( creason == kNullSatReason ) {
	  learnt.push_back(~q);
	  if ( c == 0 ) {
	    break;
	  }
	}
	else {
	  p = q;
	  break;
	}
      }
    }
    if ( creason == kNullSatReason ) {
      break;
    }
  }

  // もっともレベルの高いリテラルを先頭に持ってくる．
  Literal l0 = learnt[0];
  int level = decision_level(l0.varid());
  size_t max_pos = 0;
  for (size_t i = 1; i < learnt.size(); ++ i) {
    Literal l1 = learnt[i];
    int level1 = decision_level(l1.varid());
    if ( level < level1 ) {
      level = level1;
      max_pos = i;
    }
  }
  if ( max_pos != 0 ) {
    Literal tmp = learnt[0];
    learnt[0] = learnt[max_pos];
    learnt[max_pos] = tmp;
  }
}

END_NAMESPACE_YM_SAT
