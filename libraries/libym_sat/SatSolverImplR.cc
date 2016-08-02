
/// @file libym_sat/SatSolverImplR.cc
/// @brief SatSolverImplR の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: SatSolverImplR.cc 2203 2009-04-16 05:04:40Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "SatSolverImplR.h"


BEGIN_NAMESPACE_YM_SAT

//////////////////////////////////////////////////////////////////////
// SatSolverImplR
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SatSolverImplR::SatSolverImplR(ostream& out,
			       SatAnalyzer* analyzer) :
  SatSolverImpl(analyzer),
  mOut(out)
{
}

// @brief デストラクタ
SatSolverImplR::~SatSolverImplR()
{
}

// @brief 変数を追加する．
// @return 新しい変数番号を返す．
// @note 変数番号は 0 から始まる．
tVarId
SatSolverImplR::new_var()
{
  tVarId id = SatSolverImpl::new_var();
  
  mOut << "N" << endl
       << "# varid = " << id << endl;

  return id;
}

// @brief 節を追加する．
// @param[in] lits リテラルのベクタ
void
SatSolverImplR::add_clause(const vector<Literal>& lits)
{
  mOut << "A";
  for (vector<Literal>::const_iterator p = lits.begin();
       p != lits.end(); ++ p) {
    Literal l = *p;
    mOut << " " << l.varid();
    if ( l.pol() == kPolPosi ) {
      mOut << "P";
    }
    else {
      mOut << "N";
    }
  }
  mOut << endl;

  SatSolverImpl::add_clause(lits);
}

// @brief SAT 問題を解く．
// @param[in] assumptions あらかじめ仮定する変数の値割り当てリスト
// @param[out] model 充足するときの値の割り当てを格納する配列．
// @retval kB3True 充足した．
// @retval kB3False 充足不能が判明した．
// @retval kB3X わからなかった．
// @note i 番めの変数の割り当て結果は model[i] に入る．
Bool3
SatSolverImplR::solve(const vector<Literal>& assumptions,
		      vector<Bool3>& model)
{
  mOut << "S";
  for (vector<Literal>::const_iterator p = assumptions.begin();
       p != assumptions.end(); ++ p) {
    Literal l = *p;
    mOut << " " << l.varid();
    if ( l.pol() == kPolPosi ) {
      mOut << "P";
    }
    else {
      mOut << "N";
    }
  }
  mOut << endl;

  Bool3 ans = SatSolverImpl::solve(assumptions, model);

  mOut << "# " << ans << endl;

  return ans;
}

END_NAMESPACE_YM_SAT
