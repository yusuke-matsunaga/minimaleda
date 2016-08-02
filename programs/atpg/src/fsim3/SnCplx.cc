
/// @file fsim/SnCplx.cc
/// @brief SnCplx の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
/// 
/// $Id: SnCplx.cc 2203 2009-04-16 05:04:40Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "SnCplx.h"


BEGIN_NAMESPACE_YM_ATPG_FSIM3

//////////////////////////////////////////////////////////////////////
// SnCplx
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SnCplx::SnCplx(ymuint32 id,
	       const LogExpr& lexp,
	       const vector<SimNode*>& inputs) :
  SnGate(id, inputs),
  mExpr(lexp)
{
}

// @brief デストラクタ
SnCplx::~SnCplx()
{
}

// @brief 正常値の計算を行う．(3値版)
Val3
SnCplx::_calc_gval3()
{
  // 未完
  return kValX;
}

// @brief 故障値の計算を行う．(3値版)
Val3
SnCplx::_calc_fval3()
{
  // 未完
  return kValX;
}

// @brief ゲートの入力から出力までの可観測性を計算する．(3値版)
bool
SnCplx::calc_gobs3(ymuint ipos)
{
  // 未完
  return false;
}

// @brief 内容をダンプする．
void
SnCplx::dump(ostream& s) const
{
  s << "CPLX(";
  const char* comma = "";
  for (ymuint i = 0; i < mNfi; ++ i) {
    s << comma << mFanins[i]->id();
    comma = ", ";
  }
  s << ")" << "\t" << mExpr << endl;
}

END_NAMESPACE_YM_ATPG_FSIM3
