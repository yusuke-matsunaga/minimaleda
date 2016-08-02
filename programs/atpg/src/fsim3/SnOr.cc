
/// @file fsim/SnOr.cc
/// @brief SnOr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
/// 
/// $Id: SnOr.cc 2203 2009-04-16 05:04:40Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "SnOr.h"


BEGIN_NAMESPACE_YM_ATPG_FSIM3

//////////////////////////////////////////////////////////////////////
// SnOr
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SnOr::SnOr(ymuint32 id,
	   const vector<SimNode*>& inputs) :
  SnGate(id, inputs)
{
}

// @brief デストラクタ
SnOr::~SnOr()
{
}

// @brief 正常値の計算を行う．(3値版)
Val3
SnOr::_calc_gval3()
{
  Val3 val = mFanins[0]->gval();
  for (ymuint i = 1; i < mNfi; ++ i) {
    val = val | mFanins[i]->gval();
  }
  return val;
}

// @brief 故障値の計算を行う．(3値版)
Val3
SnOr::_calc_fval3()
{
  Val3 val = mFanins[0]->fval();
  for (ymuint i = 1; i < mNfi; ++ i) {
    val = val | mFanins[i]->fval();
  }
  return val;
}

// @brief ゲートの入力から出力までの可観測性を計算する．(3値版)
bool
SnOr::calc_gobs3(ymuint ipos)
{
  for (ymuint i = 0; i < ipos; ++ i) {
    Val3 pat = mFanins[i]->gval();
    if ( pat != kVal0 ) {
      return false;
    }
  }
  for (ymuint i = ipos + 1; i < mNfi; ++ i) {
    Val3 pat = mFanins[i]->gval();
    if ( pat != kVal0 ) {
      return false;
    }
  }
  return true;
}

// @brief 内容をダンプする．
void
SnOr::dump(ostream& s) const
{
  s << "OR(" << mFanins[0]->id();
  for (ymuint i = 1; i < mNfi; ++ i) {
    s << ", " << mFanins[i]->id();
  }
  s << ")" << endl;
}


//////////////////////////////////////////////////////////////////////
// SnOr2
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SnOr2::SnOr2(ymuint32 id,
	     const vector<SimNode*>& inputs) :
  SnGate2(id, inputs)
{
}

// @brief デストラクタ
SnOr2::~SnOr2()
{
}

// @brief 正常値の計算を行う．(3値版)
Val3
SnOr2::_calc_gval3()
{
  Val3 val;
  val =       mFanins[0]->gval();
  val = val | mFanins[1]->gval();
  return val;
}

// @brief 故障値の計算を行う．(3値版)
Val3
SnOr2::_calc_fval3()
{
  Val3 val;
  val =       mFanins[0]->fval();
  val = val | mFanins[1]->fval();
  return val;
}

// @brief ゲートの入力から出力までの可観測性を計算する．(3値版)
bool
SnOr2::calc_gobs3(ymuint ipos)
{
  Val3 pat = mFanins[ipos ^ 1]->gval();
  return pat == kVal0;
}

// @brief 内容をダンプする．
void
SnOr2::dump(ostream& s) const
{
  s << "OR2(" << mFanins[0]->id();
  s << ", " << mFanins[1]->id();
  s << ")" << endl;
}


//////////////////////////////////////////////////////////////////////
// SnOr3
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SnOr3::SnOr3(ymuint32 id,
	     const vector<SimNode*>& inputs) :
  SnGate3(id, inputs)
{
}

// @brief デストラクタ
SnOr3::~SnOr3()
{
}

// @brief 正常値の計算を行う．(3値版)
Val3
SnOr3::_calc_gval3()
{
  Val3 val;
  val =       mFanins[0]->gval();
  val = val | mFanins[1]->gval();
  val = val | mFanins[2]->gval();
  return val;
}

// @brief 故障値の計算を行う．(3値版)
Val3
SnOr3::_calc_fval3()
{
  Val3 val;
  val =       mFanins[0]->fval();
  val = val | mFanins[1]->fval();
  val = val | mFanins[2]->fval();
  return val;
}

// @brief ゲートの入力から出力までの可観測性を計算する．(3値版)
bool
SnOr3::calc_gobs3(ymuint ipos)
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
  bool pat0 = (mFanins[idx1]->gval() == kVal0);
  bool pat1 = (mFanins[idx2]->gval() == kVal0);
  return pat0 & pat1;
}

// @brief 内容をダンプする．
void
SnOr3::dump(ostream& s) const
{
  s << "OR3(" << mFanins[0]->id();
  s << ", " << mFanins[1]->id();
  s << ", " << mFanins[2]->id();
  s << ")" << endl;
}


//////////////////////////////////////////////////////////////////////
// SnOr4
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SnOr4::SnOr4(ymuint32 id,
	     const vector<SimNode*>& inputs) :
  SnGate4(id, inputs)
{
}

// @brief デストラクタ
SnOr4::~SnOr4()
{
}

// @brief 正常値の計算を行う．(3値版)
Val3
SnOr4::_calc_gval3()
{
  Val3 val;
  val =       mFanins[0]->gval();
  val = val | mFanins[1]->gval();
  val = val | mFanins[2]->gval();
  val = val | mFanins[3]->gval();
  return val;
}

// @brief 故障値の計算を行う．(3値版)
Val3
SnOr4::_calc_fval3()
{
  Val3 val;
  val =       mFanins[0]->fval();
  val = val | mFanins[1]->fval();
  val = val | mFanins[2]->fval();
  val = val | mFanins[3]->fval();
  return val;
}

// @brief ゲートの入力から出力までの可観測性を計算する．(3値版)
bool
SnOr4::calc_gobs3(ymuint ipos)
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
    
  default:
    assert_not_reached(__FILE__, __LINE__);
    break;
  }
  bool pat0 = (mFanins[idx1]->gval() == kVal0);
  bool pat1 = (mFanins[idx2]->gval() == kVal0);
  bool pat2 = (mFanins[idx3]->gval() == kVal0);
  return pat0 & pat1 & pat2;
}

// @brief 内容をダンプする．
void
SnOr4::dump(ostream& s) const
{
  s << "OR4(" << mFanins[0]->id();
  s << ", " << mFanins[1]->id();
  s << ", " << mFanins[2]->id();
  s << ", " << mFanins[3]->id();
  s << ")" << endl;
}


//////////////////////////////////////////////////////////////////////
// SnNor
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SnNor::SnNor(ymuint32 id,
	     const vector<SimNode*>& inputs) :
  SnOr(id, inputs)
{
}

// @brief デストラクタ
SnNor::~SnNor()
{
}

// @brief 正常値の計算を行う．(3値版)
Val3
SnNor::_calc_gval3()
{
  Val3 val = mFanins[0]->gval();
  for (ymuint i = 1; i < mNfi; ++ i) {
    val = val | mFanins[i]->gval();
  }
  return ~val;
}

// @brief 故障値の計算を行う．(3値版)
Val3
SnNor::_calc_fval3()
{
  Val3 val = mFanins[0]->fval();
  for (ymuint i = 1; i < mNfi; ++ i) {
    val = val | mFanins[i]->fval();
  }
  return ~val;
}

// @brief 内容をダンプする．
void
SnNor::dump(ostream& s) const
{
  s << "NOR(" << mFanins[0]->id();
  for (ymuint i = 1; i < mNfi; ++ i) {
    s << ", " << mFanins[i]->id();
  }
  s << ")" << endl;
}


//////////////////////////////////////////////////////////////////////
// SnNor2
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SnNor2::SnNor2(ymuint32 id,
	       const vector<SimNode*>& inputs) :
  SnOr2(id, inputs)
{
}

// @brief デストラクタ
SnNor2::~SnNor2()
{
}

// @brief 正常値の計算を行う．(3値版)
Val3
SnNor2::_calc_gval3()
{
  Val3 val;
  val =       mFanins[0]->gval();
  val = val | mFanins[1]->gval();
  return ~val;
}

// @brief 故障値の計算を行う．(3値版)
Val3
SnNor2::_calc_fval3()
{
  Val3 val;
  val =       mFanins[0]->fval();
  val = val | mFanins[1]->fval();
  return ~val;
}

// @brief 内容をダンプする．
void
SnNor2::dump(ostream& s) const
{
  s << "NOR2(" << mFanins[0]->id();
  s << ", " << mFanins[1]->id();
  s << ")" << endl;
}


//////////////////////////////////////////////////////////////////////
// SnNor3
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SnNor3::SnNor3(ymuint32 id,
	       const vector<SimNode*>& inputs) :
  SnOr3(id, inputs)
{
}

// @brief デストラクタ
SnNor3::~SnNor3()
{
}

// @brief 正常値の計算を行う．(3値版)
Val3
SnNor3::_calc_gval3()
{
  Val3 val;
  val =       mFanins[0]->gval();
  val = val | mFanins[1]->gval();
  val = val | mFanins[2]->gval();
  return ~val;
}

// @brief 故障値の計算を行う．(3値版)
Val3
SnNor3::_calc_fval3()
{
  Val3 val;
  val =       mFanins[0]->fval();
  val = val | mFanins[1]->fval();
  val = val | mFanins[2]->fval();
  return ~val;
}

// @brief 内容をダンプする．
void
SnNor3::dump(ostream& s) const
{
  s << "NOR3(" << mFanins[0]->id();
  s << ", " << mFanins[1]->id();
  s << ", " << mFanins[2]->id();
  s << ")" << endl;
}


//////////////////////////////////////////////////////////////////////
// SnNor4
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SnNor4::SnNor4(ymuint32 id,
	       const vector<SimNode*>& inputs) :
  SnOr4(id, inputs)
{
}

// @brief デストラクタ
SnNor4::~SnNor4()
{
}

// @brief 正常値の計算を行う．(3値版)
Val3
SnNor4::_calc_gval3()
{
  Val3 val;
  val =       mFanins[0]->gval();
  val = val | mFanins[1]->gval();
  val = val | mFanins[2]->gval();
  val = val | mFanins[3]->gval();
  return ~val;
}

// @brief 故障値の計算を行う．(3値版)
Val3
SnNor4::_calc_fval3()
{
  Val3 val;
  val =       mFanins[0]->fval();
  val = val | mFanins[1]->fval();
  val = val | mFanins[2]->fval();
  val = val | mFanins[3]->fval();
  return ~val;
}

// @brief 内容をダンプする．
void
SnNor4::dump(ostream& s) const
{
  s << "NOR4(" << mFanins[0]->id();
  s << ", " << mFanins[1]->id();
  s << ", " << mFanins[2]->id();
  s << ", " << mFanins[3]->id();
  s << ")" << endl;
}

END_NAMESPACE_YM_ATPG_FSIM3
