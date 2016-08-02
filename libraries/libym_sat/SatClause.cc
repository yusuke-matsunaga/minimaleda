
/// @file libym_sat/SatClause.cc
/// @brief SatClause の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: SatClause.cc 2203 2009-04-16 05:04:40Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "SatClause.h"
#include "SatReason.h"
#include "Watcher.h"


BEGIN_NAMESPACE_YM_SAT

//////////////////////////////////////////////////////////////////////
// クラス SatClause
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] lits リテラルのリスト
SatClause::SatClause(const vector<Literal>& lits,
		     bool learnt)
{
  size_t n = lits.size();
  mSizeLearnt = (n << 1) | static_cast<ymuint>(learnt);
  mActivity = 0.0;
  for (size_t i = 0; i < n; ++ i) {
    mLits[i] = lits[i];
  }
}

// @brief デストラクタ
SatClause::~SatClause()
{
}

// @brief SatClause の内容を出力する
ostream&
operator<<(ostream& s,
	   const SatClause& c)
{
  if ( c.size() == 2 ) {
    s << "(" << c.lit(0) << " + " << c.lit(1) << ")";
    //s << ~c.lit(1);
  }
  else {
    // 一旦 vector に入れてソートする．
    size_t n = c.size();
    vector<Literal> tmp(n);
    for (size_t i = 0; i < n; ++ i) {
      tmp[i] = c.lit(i);
    }
    sort(tmp.begin() + 1, tmp.end());
    
    s << "(";
    const char* plus = "";
    for (size_t i = 0; i < n; ++ i) {
      s << plus << tmp[i];
      plus = " + ";
    }
    s << ")";
  }
  return s;
}


//////////////////////////////////////////////////////////////////////
// クラス SatReason
//////////////////////////////////////////////////////////////////////

// @brief SatReason の内容を出力する
ostream&
operator<<(ostream& s,
	   const SatReason& r)
{
  if ( r.is_literal() ) {
    s << r.literal();
  }
  else {
    s << r.clause();
  }
  return s;
}


END_NAMESPACE_YM_SAT
