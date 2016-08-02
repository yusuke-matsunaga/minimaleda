
/// @file fsim/SnCplx.cc
/// @brief SnCplx の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
/// 
/// $Id: SnCplx.cc 2203 2009-04-16 05:04:40Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "SnCplx.h"


BEGIN_NAMESPACE_YM_ATPG_FSIM

//////////////////////////////////////////////////////////////////////
// SnCplx
//////////////////////////////////////////////////////////////////////

BEGIN_NONAMESPACE
// 値の計算用の作業領域
vector<PackedVal> tmp1;
END_NONAMESPACE

// @brief コンストラクタ
SnCplx::SnCplx(ymuint32 id,
	       const LogExpr& lexp,
	       const vector<SimNode*>& inputs) :
  SnGate(id, inputs)
{
  mExpr = lexp;
}

// @brief デストラクタ
SnCplx::~SnCplx()
{
}

// @brief 正常値の計算を行う．(2値版)
PackedVal
SnCplx::_calc_gval2()
{
  ymuint ni = mNfi;
  tmp1.resize(ni);
  for (ymuint i = 0; i < ni; ++ i) {
    SimNode* inode = mFanins[i];
    tmp1[i] = inode->gval();
  }
  return mExpr.eval(tmp1);
}

// @brief 故障値の計算を行う．(2値版)
PackedVal
SnCplx::_calc_fval2()
{
  ymuint ni = mNfi;
  tmp1.resize(ni);
  for (ymuint i = 0; i < ni; ++ i) {
    SimNode* inode = mFanins[i];
    tmp1[i] = inode->fval();
  }
  return mExpr.eval(tmp1);
}

// @brief ゲートの入力から出力までの可観測性を計算する．(2値版)
PackedVal
SnCplx::calc_gobs2(ymuint ipos)
{
  ymuint ni = mNfi;
  tmp1.resize(ni);
  for (ymuint i = 0; i < ipos; ++ i) {
    tmp1[i] = mFanins[i]->gval();
  }
  tmp1[ipos] = ~mFanins[ipos]->gval();
  for (ymuint i = ipos + 1; i < ni; ++ i) {
    tmp1[i] = mFanins[i]->gval();
  }
  PackedVal val = mExpr.eval(tmp1);
  return val ^ gval();
}

// @brief 内容をダンプする．
void
SnCplx::dump(ostream& s) const
{
  s << "CPLX(";
  const char* comma = "";
  ymuint ni = mNfi;
  for (ymuint i = 0; i < ni; ++ i) {
    s << comma << mFanins[i]->id();
    comma = ", ";
  }
  s << ")" << "\t" << mExpr << endl;
}

END_NAMESPACE_YM_ATPG_FSIM
