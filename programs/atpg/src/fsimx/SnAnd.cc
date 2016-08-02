
/// @file fsim/SnAnd.cc
/// @brief SnAnd の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
/// 
/// $Id: SnAnd.cc 2203 2009-04-16 05:04:40Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "SnAnd.h"


BEGIN_NAMESPACE_YM_ATPG_FSIMX

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

// @brief 正常値の計算を行う．(3値版)
PackedVal3
SnAnd::_calc_gval3()
{
  PackedVal3 val = mFanins[0]->gval();
  for (ymuint i = 1; i < mNfi; ++ i) {
    val &= mFanins[i]->gval();
  }
  return val;
}

// @brief 故障値の計算を行う．(3値版)
PackedVal3
SnAnd::_calc_fval3()
{
  PackedVal3 val = mFanins[0]->fval();
  for (ymuint i = 1; i < mNfi; ++ i) {
    val &= mFanins[i]->fval();
  }
  return val;
}

// @brief ゲートの入力から出力までの可観測性を計算する．(3値版)
PackedVal
SnAnd::calc_gobs3(ymuint ipos)
{
  PackedVal obs = kPvAll1;
  for (ymuint i = 0; i < ipos; ++ i) {
    PackedVal3 pat = mFanins[i]->gval();
    obs &= pat.extract_1();
  }
  for (ymuint i = ipos + 1; i < mNfi; ++ i) {
    PackedVal3 pat = mFanins[i]->gval();
    obs &= pat.extract_1();
  }
  return obs;
}

// @brief 内容をダンプする．
void
SnAnd::dump(ostream& s) const
{
  s << "AND(" << mFanins[0]->id();
  for (ymuint i = 1; i < mNfi; ++ i) {
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

// @brief 正常値の計算を行う．(3値版)
PackedVal3
SnAnd2::_calc_gval3()
{
  PackedVal3 val;
  val =  mFanins[0]->gval();
  val &= mFanins[1]->gval();
  return val;
}

// @brief 故障値の計算を行う．(3値版)
PackedVal3
SnAnd2::_calc_fval3()
{
  PackedVal3 val;
  val =  mFanins[0]->fval();
  val &= mFanins[1]->fval();
  return val;
}

// @brief ゲートの入力から出力までの可観測性を計算する．(3値版)
PackedVal
SnAnd2::calc_gobs3(ymuint ipos)
{
  PackedVal3 pat = mFanins[ipos ^ 1]->gval();
  return pat.extract_1();
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

// @brief 正常値の計算を行う．(3値版)
PackedVal3
SnAnd3::_calc_gval3()
{
  PackedVal3 val;
  val =  mFanins[0]->gval();
  val &= mFanins[1]->gval();
  val &= mFanins[2]->gval();
  return val;
}

// @brief 故障値の計算を行う．(3値版)
PackedVal3
SnAnd3::_calc_fval3()
{
  PackedVal3 val;
  val =  mFanins[0]->fval();
  val &= mFanins[1]->fval();
  val &= mFanins[2]->fval();
  return val;
}

// @brief ゲートの入力から出力までの可観測性を計算する．(3値版)
PackedVal
SnAnd3::calc_gobs3(ymuint ipos)
{
  ymuint idx1 = 0;
  ymuint idx2 = 0;
  switch ( ipos ) {
  case 0:
    idx1 = 1;
    idx2 = 2;
    break;

  case 1:
    idx1 = 0;
    idx2 = 2;
    break;

  case 2:
    idx1 = 0;
    idx2 = 1;
    break;
    
  default:
    assert_not_reached(__FILE__, __LINE__);
    break;
  }
  
  PackedVal pat0 = mFanins[idx1]->gval().extract_1();
  PackedVal pat1 = mFanins[idx2]->gval().extract_1();
  return pat0 & pat1;
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

// @brief 正常値の計算を行う．(3値版)
PackedVal3
SnAnd4::_calc_gval3()
{
  PackedVal3 val;
  val =  mFanins[0]->gval();
  val &= mFanins[1]->gval();
  val &= mFanins[2]->gval();
  val &= mFanins[3]->gval();
  return val;
}

// @brief 故障値の計算を行う．(3値版)
PackedVal3
SnAnd4::_calc_fval3()
{
  PackedVal3 val;
  val =  mFanins[0]->fval();
  val &= mFanins[1]->fval();
  val &= mFanins[2]->fval();
  val &= mFanins[3]->fval();
  return val;
}

// @brief ゲートの入力から出力までの可観測性を計算する．(3値版)
PackedVal
SnAnd4::calc_gobs3(ymuint ipos)
{
  ymuint idx1 = 0;
  ymuint idx2 = 0;
  ymuint idx3 = 0;
  switch ( ipos ) {
  case 0:
    idx1 = 1;
    idx2 = 2;
    idx3 = 3;
    break;

  case 1:
    idx1 = 0;
    idx2 = 2;
    idx3 = 3;
    break;

  case 2:
    idx1 = 0;
    idx2 = 1;
    idx3 = 3;
    break;

  case 3:
    idx1 = 0;
    idx2 = 1;
    idx3 = 2;
    break;
    
  }
  PackedVal pat0 = mFanins[idx1]->gval().extract_1();
  PackedVal pat1 = mFanins[idx2]->gval().extract_1();
  PackedVal pat2 = mFanins[idx3]->gval().extract_1();
  return pat0 & pat1 & pat2;
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

// @brief 正常値の計算を行う．(3値版)
PackedVal3
SnNand::_calc_gval3()
{
  PackedVal3 val = mFanins[0]->gval();
  for (ymuint i = 1; i < mNfi; ++ i) {
    val &= mFanins[i]->gval();
  }
  return ~val;
}

// @brief 故障値の計算を行う．(3値版)
PackedVal3
SnNand::_calc_fval3()
{
  PackedVal3 val = mFanins[0]->fval();
  for (ymuint i = 1; i < mNfi; ++ i) {
    val &= mFanins[i]->fval();
  }
  return ~val;
}

// @brief 内容をダンプする．
void
SnNand::dump(ostream& s) const
{
  s << "NAND(" << mFanins[0]->id();
  for (ymuint i = 1; i < mNfi; ++ i) {
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

// @brief 正常値の計算を行う．(3値版)
PackedVal3
SnNand2::_calc_gval3()
{
  PackedVal3 val;
  val =  mFanins[0]->gval();
  val &= mFanins[1]->gval();
  return ~val;
}

// @brief 故障値の計算を行う．(3値版)
PackedVal3
SnNand2::_calc_fval3()
{
  PackedVal3 val;
  val =  mFanins[0]->fval();
  val &= mFanins[1]->fval();
  return ~val;
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

// @brief 正常値の計算を行う．(3値版)
PackedVal3
SnNand3::_calc_gval3()
{
  PackedVal3 val;
  val =  mFanins[0]->gval();
  val &= mFanins[1]->gval();
  val &= mFanins[2]->gval();
  return ~val;
}

// @brief 故障値の計算を行う．(3値版)
PackedVal3
SnNand3::_calc_fval3()
{
  PackedVal3 val;
  val =  mFanins[0]->fval();
  val &= mFanins[1]->fval();
  val &= mFanins[2]->fval();
  return ~val;
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

// @brief 正常値の計算を行う．(3値版)
PackedVal3
SnNand4::_calc_gval3()
{
  PackedVal3 val;
  val  = mFanins[0]->gval();
  val &= mFanins[1]->gval();
  val &= mFanins[2]->gval();
  val &= mFanins[3]->gval();
  return ~val;
}

// @brief 故障値の計算を行う．(3値版)
PackedVal3
SnNand4::_calc_fval3()
{
  PackedVal3 val;
  val =  mFanins[0]->fval();
  val &= mFanins[1]->fval();
  val &= mFanins[2]->fval();
  val &= mFanins[3]->fval();
  return ~val;
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

END_NAMESPACE_YM_ATPG_FSIMX
