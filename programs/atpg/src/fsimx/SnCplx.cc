
/// @file fsim/SnCplx.cc
/// @brief SnCplx の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
/// 
/// $Id: SnCplx.cc 2203 2009-04-16 05:04:40Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "SnCplx.h"


BEGIN_NAMESPACE_YM_ATPG_FSIMX

//////////////////////////////////////////////////////////////////////
// SnCplx
//////////////////////////////////////////////////////////////////////

BEGIN_NONAMESPACE
// 値の計算用の作業領域
vector<PackedVal> tmp0;
vector<PackedVal> tmp1;
END_NONAMESPACE

// @brief コンストラクタ
SnCplx::SnCplx(ymuint32 id,
	       const LogExpr& lexp,
	       const vector<SimNode*>& inputs) :
  SnGate(id, inputs)
{
  mExpr[0] = ~lexp;
  mExpr[1] = lexp;
}

// @brief デストラクタ
SnCplx::~SnCplx()
{
}

// @brief 正常値の計算を行う．(3値版)
PackedVal3
SnCplx::_calc_gval3()
{
  tmp0.resize(mNfi);
  tmp1.resize(mNfi);
  for (ymuint i = 0; i < mNfi; ++ i) {
    SimNode* inode = mFanins[i];
    PackedVal3 tmp = inode->gval();
    tmp0[i] = tmp._pat0();
    tmp1[i] = tmp._pat1();
  }
  PackedVal val0 = mExpr[0].eval(tmp0);
  PackedVal val1 = mExpr[1].eval(tmp1);
  return PackedVal3(val0, val1);
}

// @brief 故障値の計算を行う．(3値版)
PackedVal3
SnCplx::_calc_fval3()
{
  tmp0.resize(mNfi);
  tmp1.resize(mNfi);
  for (ymuint i = 0; i < mNfi; ++ i) {
    SimNode* inode = mFanins[i];
    PackedVal3 tmp = inode->fval();
    tmp0[i] = tmp._pat0();
    tmp1[i] = tmp._pat1();
  }
  PackedVal val0 = mExpr[0].eval(tmp0);
  PackedVal val1 = mExpr[1].eval(tmp1);
  return PackedVal3(val0, val1);
}

// @brief ゲートの入力から出力までの可観測性を計算する．(3値版)
PackedVal
SnCplx::calc_gobs3(ymuint ipos)
{
  tmp0.resize(mNfi);
  tmp1.resize(mNfi);
  for (ymuint i = 0; i < ipos; ++ i) {
    SimNode* inode = mFanins[i];
    PackedVal3 tmp = inode->gval();
    tmp0[i] = tmp._pat0();
    tmp1[i] = tmp._pat1();
  }
  // 0 と 1 が逆になっていることに注意
  PackedVal3 tmp = mFanins[ipos]->gval();
  tmp0[ipos] = tmp._pat1();
  tmp1[ipos] = tmp._pat0();
  for (ymuint i = ipos + 1; i < mNfi; ++ i) {
    SimNode* inode = mFanins[i];
    PackedVal3 tmp = inode->gval();
    tmp0[i] = tmp._pat0();
    tmp1[i] = tmp._pat1();
  }
  PackedVal fval_0 = mExpr[0].eval(tmp0);
  PackedVal fval_1 = mExpr[1].eval(tmp1);
  PackedVal3 new_fval(fval_0, fval_1);
  return new_fval.diff(gval());
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
  s << ")" << "\t" << mExpr[1] << endl;
}

END_NAMESPACE_YM_ATPG_FSIMX
