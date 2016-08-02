
/// @file libym_sat/SatSolver.cc
/// @brief SatSolver の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: SatSolver.cc 2203 2009-04-16 05:04:40Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_sat/SatSolver.h"


BEGIN_NAMESPACE_YM_SAT

//////////////////////////////////////////////////////////////////////
// SatSolver
//////////////////////////////////////////////////////////////////////

// @brief 1項の節を追加する．
void
SatSolver::add_clause(Literal lit1)
{
  vector<Literal> tmp(1);
  tmp[0] = lit1;
  add_clause(tmp);
}

// @brief 2項の節を追加する．
void
SatSolver::add_clause(Literal lit1,
		      Literal lit2)
{
  vector<Literal> tmp(2);
  tmp[0] = lit1;
  tmp[1] = lit2;
  add_clause(tmp);
}

// @brief 3項の節を追加する．
void
SatSolver::add_clause(Literal lit1,
		      Literal lit2,
		      Literal lit3)
{
  vector<Literal> tmp(3);
  tmp[0] = lit1;
  tmp[1] = lit2;
  tmp[2] = lit3;
  add_clause(tmp);
}

// @brief SAT 問題を解く．
// @param[out] model 充足するときの値の割り当てを格納する配列．
// @retval kB3True 充足した．
// @retval kB3False 充足不能が判明した．
// @retval kB3X わからなかった．
// @note i 番めの変数の割り当て結果は model[i] に入る．
Bool3
SatSolver::solve(vector<Bool3>& model)
{
  // 空の assumptions を付けて solve() を呼ぶだけ
  return solve(vector<Literal>(), model);
}

END_NAMESPACE_YM_SAT
