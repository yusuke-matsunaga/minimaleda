
/// @file libym_sat/SatSolverFactory.cc
/// @brief SatSolverFactory の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: SatSolverFactory.cc 1978 2009-02-06 12:29:16Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "SatSolverImpl.h"
#include "SatSolverImplR.h"
#include "MiniSat/SatSolverMiniSat.h"
#include "SatAnalyzer.h"


BEGIN_NAMESPACE_YM_SAT

// 普通の SatSolver を生成する．
SatSolver*
SatSolverFactory::gen_solver(const string& option)
{
  SatAnalyzer* analyzer = SaFactory::gen_analyzer(option);
  return new SatSolverImpl(analyzer);
}

// ログ記録機能付き SatSolver を生成する．
SatSolver*
SatSolverFactory::gen_recsolver(ostream& out,
				const string& option)
{
  SatAnalyzer* analyzer = SaFactory::gen_analyzer(option);
  return new SatSolverImplR(out, analyzer);
}

// MiniSat 版 SatSolver を生成する．
SatSolver*
SatSolverFactory::gen_minisat()
{
  return new SatSolverMiniSat;
}

END_NAMESPACE_YM_SAT
