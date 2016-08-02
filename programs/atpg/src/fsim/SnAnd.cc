
/// @file fsim/SnAnd.cc
/// @brief SnAnd の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
/// 
/// $Id: SnAnd.cc 2203 2009-04-16 05:04:40Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "SnAnd.h"


BEGIN_NAMESPACE_YM_ATPG_FSIM

//////////////////////////////////////////////////////////////////////
// SnAnd
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SnAnd::SnAnd(ymuint32 id,
	     const vector<SimNode*>& inputs) :
  SnGate(id, inputs)
{
}

// @brief デストラクタ
SnAnd::~SnAnd()
{
}

// @brief 正常値の計算を行う．(2値版)
PackedVal
SnAnd::_calc_gval2()
{
  ymuint n = mNfi;
  PackedVal new_val = mFanins[0]->gval();
  for (ymuint i = 1; i < n; ++ i) {
    new_val &= mFanins[i]->gval();
  }
  return new_val;
}

// @brief 故障値の計算を行う．(2値版)
PackedVal
SnAnd::_calc_fval2()
{
  ymuint n = mNfi;
  PackedVal new_val = mFanins[0]->fval();
  for (ymuint i = 1; i < n; ++ i) {
    new_val &= mFanins[i]->fval();
  }
  return new_val;
}

// @brief ゲートの入力から出力までの可観測性を計算する．(2値版)
PackedVal
SnAnd::calc_gobs2(ymuint ipos)
{
  PackedVal obs = kPvAll1;
  for (ymuint i = 0; i < ipos; ++ i) {
    obs &= mFanins[i]->gval();
  }
  for (ymuint i = ipos + 1; i < mNfi; ++ i) {
    obs &= mFanins[i]->gval();
  }
  return obs;
}

// @brief 内容をダンプする．
void
SnAnd::dump(ostream& s) const
{
  ymuint n = mNfi;
  s << "AND(" << mFanins[0]->id();
  for (ymuint i = 1; i < n; ++ i) {
    s << ", " << mFanins[i]->id();
  }
  s << ")" << endl;
}


//////////////////////////////////////////////////////////////////////
// SnAnd2
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SnAnd2::SnAnd2(ymuint32 id,
	       const vector<SimNode*>& inputs) :
  SnGate2(id, inputs)
{
}

// @brief デストラクタ
SnAnd2::~SnAnd2()
{
}

// @brief 正常値の計算を行う．(2値版)
PackedVal
SnAnd2::_calc_gval2()
{
  PackedVal pat0 = mFanins[0]->gval();
  PackedVal pat1 = mFanins[1]->gval();
  return pat0 & pat1;
}

// @brief 故障値の計算を行う．(2値版)
PackedVal
SnAnd2::_calc_fval2()
{
  PackedVal pat0 = mFanins[0]->fval();
  PackedVal pat1 = mFanins[1]->fval();
  return pat0 & pat1;
}

// @brief ゲートの入力から出力までの可観測性を計算する．(2値版)
PackedVal
SnAnd2::calc_gobs2(ymuint ipos)
{
  return mFanins[ipos ^ 1]->gval();
}

// @brief 内容をダンプする．
void
SnAnd2::dump(ostream& s) const
{
  s << "AND2(" << mFanins[0]->id();
  s << ", " << mFanins[1]->id();
  s << ")" << endl;
}


//////////////////////////////////////////////////////////////////////
// SnAnd3
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SnAnd3::SnAnd3(ymuint32 id,
	       const vector<SimNode*>& inputs) :
  SnGate3(id, inputs)
{
}

// @brief デストラクタ
SnAnd3::~SnAnd3()
{
}

// @brief 正常値の計算を行う．(2値版)
PackedVal
SnAnd3::_calc_gval2()
{
  PackedVal pat0 = mFanins[0]->gval();
  PackedVal pat1 = mFanins[1]->gval();
  PackedVal pat2 = mFanins[2]->gval();
  return pat0 & pat1 & pat2;
}

// @brief 故障値の計算を行う．(2値版)
PackedVal
SnAnd3::_calc_fval2()
{
  PackedVal pat0 = mFanins[0]->fval();
  PackedVal pat1 = mFanins[1]->fval();
  PackedVal pat2 = mFanins[2]->fval();
  return pat0 & pat1 & pat2;
}

// @brief ゲートの入力から出力までの可観測性を計算する．(2値版)
PackedVal
SnAnd3::calc_gobs2(ymuint ipos)
{
  switch ( ipos ) {
  case 0: return mFanins[1]->gval() & mFanins[2]->gval();
  case 1: return mFanins[0]->gval() & mFanins[2]->gval();
  case 2: return mFanins[0]->gval() & mFanins[1]->gval();
  }
  return kPvAll0;
}

// @brief 内容をダンプする．
void
SnAnd3::dump(ostream& s) const
{
  s << "AND3(" << mFanins[0]->id();
  s << ", " << mFanins[1]->id();
  s << ", " << mFanins[2]->id();
  s << ")" << endl;
}


//////////////////////////////////////////////////////////////////////
// SnAnd4
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SnAnd4::SnAnd4(ymuint32 id,
	       const vector<SimNode*>& inputs) :
  SnGate4(id, inputs)
{
}

// @brief デストラクタ
SnAnd4::~SnAnd4()
{
}

// @brief 正常値の計算を行う．(2値版)
PackedVal
SnAnd4::_calc_gval2()
{
  PackedVal pat0 = mFanins[0]->gval();
  PackedVal pat1 = mFanins[1]->gval();
  PackedVal pat2 = mFanins[2]->gval();
  PackedVal pat3 = mFanins[3]->gval();
  return pat0 & pat1 & pat2 & pat3;
}

// @brief 故障値の計算を行う．(2値版)
PackedVal
SnAnd4::_calc_fval2()
{
  PackedVal pat0 = mFanins[0]->fval();
  PackedVal pat1 = mFanins[1]->fval();
  PackedVal pat2 = mFanins[2]->fval();
  PackedVal pat3 = mFanins[3]->fval();
  return pat0 & pat1 & pat2 & pat3;
}

// @brief ゲートの入力から出力までの可観測性を計算する．(2値版)
PackedVal
SnAnd4::calc_gobs2(ymuint ipos)
{
  switch ( ipos ) {
  case 0: return mFanins[1]->gval() & mFanins[2]->gval() & mFanins[3]->gval();
  case 1: return mFanins[0]->gval() & mFanins[2]->gval() & mFanins[3]->gval();
  case 2: return mFanins[0]->gval() & mFanins[1]->gval() & mFanins[3]->gval();
  case 3: return mFanins[0]->gval() & mFanins[1]->gval() & mFanins[2]->gval();
  }
  return kPvAll0;
}

// @brief 内容をダンプする．
void
SnAnd4::dump(ostream& s) const
{
  s << "AND4(" << mFanins[0]->id();
  s << ", " << mFanins[1]->id();
  s << ", " << mFanins[2]->id();
  s << ", " << mFanins[3]->id();
  s << ")" << endl;
}


//////////////////////////////////////////////////////////////////////
// SnNand
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SnNand::SnNand(ymuint32 id,
	       const vector<SimNode*>& inputs) :
  SnAnd(id, inputs)
{
}

// @brief デストラクタ
SnNand::~SnNand()
{
}

// @brief 正常値の計算を行う．(2値版)
PackedVal
SnNand::_calc_gval2()
{
  ymuint n = mNfi;
  PackedVal new_val = mFanins[0]->gval();
  for (ymuint i = 1; i < n; ++ i) {
    new_val &= mFanins[i]->gval();
  }
  return ~new_val;
}

// @brief 故障値の計算を行う．(2値版)
PackedVal
SnNand::_calc_fval2()
{
  ymuint n = mNfi;
  PackedVal new_val = mFanins[0]->fval();
  for (ymuint i = 1; i < n; ++ i) {
    new_val &= mFanins[i]->fval();
  }
  return ~new_val;
}

// @brief 内容をダンプする．
void
SnNand::dump(ostream& s) const
{
  ymuint n = mNfi;
  s << "NAND(" << mFanins[0]->id();
  for (ymuint i = 1; i < n; ++ i) {
    s << ", " << mFanins[i]->id();
  }
  s << ")" << endl;
}


//////////////////////////////////////////////////////////////////////
// SnNand2
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SnNand2::SnNand2(ymuint32 id,
		 const vector<SimNode*>& inputs) :
  SnAnd2(id, inputs)
{
}

// @brief デストラクタ
SnNand2::~SnNand2()
{
}

// @brief 正常値の計算を行う．(2値版)
PackedVal
SnNand2::_calc_gval2()
{
  PackedVal pat0 = mFanins[0]->gval();
  PackedVal pat1 = mFanins[1]->gval();
  return ~(pat0 & pat1);
}

// @brief 故障値の計算を行う．(2値版)
PackedVal
SnNand2::_calc_fval2()
{
  PackedVal pat0 = mFanins[0]->fval();
  PackedVal pat1 = mFanins[1]->fval();
  return ~(pat0 & pat1);
}

// @brief 内容をダンプする．
void
SnNand2::dump(ostream& s) const
{
  s << "NAND2(" << mFanins[0]->id();
  s << ", " << mFanins[1]->id();
  s << ")" << endl;
}


//////////////////////////////////////////////////////////////////////
// SnNand3
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SnNand3::SnNand3(ymuint32 id,
		 const vector<SimNode*>& inputs) :
  SnAnd3(id, inputs)
{
}

// @brief デストラクタ
SnNand3::~SnNand3()
{
}

// @brief 正常値の計算を行う．(2値版)
PackedVal
SnNand3::_calc_gval2()
{
  PackedVal pat0 = mFanins[0]->gval();
  PackedVal pat1 = mFanins[1]->gval();
  PackedVal pat2 = mFanins[2]->gval();
  return ~(pat0 & pat1 & pat2);
}

// @brief 故障値の計算を行う．(2値版)
PackedVal
SnNand3::_calc_fval2()
{
  PackedVal pat0 = mFanins[0]->fval();
  PackedVal pat1 = mFanins[1]->fval();
  PackedVal pat2 = mFanins[2]->fval();
  return ~(pat0 & pat1 & pat2);
}

// @brief 内容をダンプする．
void
SnNand3::dump(ostream& s) const
{
  s << "NAND3(" << mFanins[0]->id();
  s << ", " << mFanins[1]->id();
  s << ", " << mFanins[2]->id();
  s << ")" << endl;
}


//////////////////////////////////////////////////////////////////////
// SnNand4
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SnNand4::SnNand4(ymuint32 id,
		 const vector<SimNode*>& inputs) :
  SnAnd4(id, inputs)
{
}

// @brief デストラクタ
SnNand4::~SnNand4()
{
}

// @brief 正常値の計算を行う．(2値版)
PackedVal
SnNand4::_calc_gval2()
{
  PackedVal pat0 = mFanins[0]->gval();
  PackedVal pat1 = mFanins[1]->gval();
  PackedVal pat2 = mFanins[2]->gval();
  PackedVal pat3 = mFanins[3]->gval();
  return ~(pat0 & pat1 & pat2 & pat3);
}

// @brief 故障値の計算を行う．(2値版)
PackedVal
SnNand4::_calc_fval2()
{
  PackedVal pat0 = mFanins[0]->fval();
  PackedVal pat1 = mFanins[1]->fval();
  PackedVal pat2 = mFanins[2]->fval();
  PackedVal pat3 = mFanins[3]->fval();
  return ~(pat0 & pat1 & pat2 & pat3);
}

// @brief 内容をダンプする．
void
SnNand4::dump(ostream& s) const
{
  s << "NAND4(" << mFanins[0]->id();
  s << ", " << mFanins[1]->id();
  s << ", " << mFanins[2]->id();
  s << ", " << mFanins[3]->id();
  s << ")" << endl;
}

END_NAMESPACE_YM_ATPG_FSIM
