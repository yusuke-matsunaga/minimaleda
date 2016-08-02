
/// @file fsim/SnXor.cc
/// @brief SnXor の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
/// 
/// $Id: SnXor.cc 2203 2009-04-16 05:04:40Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "SnXor.h"


BEGIN_NAMESPACE_YM_ATPG_FSIM3

//////////////////////////////////////////////////////////////////////
// SnXor
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SnXor::SnXor(ymuint32 id,
	     const vector<SimNode*>& inputs) :
  SnGate(id, inputs)
{
}

// @brief デストラクタ
SnXor::~SnXor()
{
}

// @brief 正常値の計算を行う．(3値版)
Val3
SnXor::_calc_gval3()
{
  Val3 val = mFanins[0]->gval();
  for (ymuint i = 1; i < mNfi; ++ i) {
    val = val ^ mFanins[i]->gval();
  }
  return val;
}

// @brief 故障値の計算を行う．(3値版)
Val3
SnXor::_calc_fval3()
{
  Val3 val = mFanins[0]->fval();
  for (ymuint i = 1; i < mNfi; ++ i) {
    val = val ^ mFanins[i]->fval();
  }
  return val;
}

// @brief ゲートの入力から出力までの可観測性を計算する．(3値版)
bool
SnXor::calc_gobs3(ymuint ipos)
{
  return true;
}

// @brief 内容をダンプする．
void
SnXor::dump(ostream& s) const
{
  s << "XOR(" << mFanins[0]->id();
  for (ymuint i = 1; i < mNfi; ++ i) {
    s << ", " << mFanins[i]->id();
  }
  s << ")" << endl;
}


//////////////////////////////////////////////////////////////////////
// SnXor2
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SnXor2::SnXor2(ymuint32 id,
	       const vector<SimNode*>& inputs) :
  SnGate2(id, inputs)
{
}

// @brief デストラクタ
SnXor2::~SnXor2()
{
}

// @brief 正常値の計算を行う．(3値版)
Val3
SnXor2::_calc_gval3()
{
  Val3 val;
  val =       mFanins[0]->gval();
  val = val ^ mFanins[1]->gval();
  return val;
}

// @brief 故障値の計算を行う．(3値版)
Val3
SnXor2::_calc_fval3()
{
  Val3 val;
  val =       mFanins[0]->fval();
  val = val ^ mFanins[1]->fval();
  return val;
}

// @brief ゲートの入力から出力までの可観測性を計算する．(3値版)
bool
SnXor2::calc_gobs3(ymuint ipos)
{
  return true;
}

// @brief 内容をダンプする．
void
SnXor2::dump(ostream& s) const
{
  s << "XOR2(" << mFanins[0]->id();
  s << ", " << mFanins[1]->id();
  s << ")" << endl;
}


//////////////////////////////////////////////////////////////////////
// SnXnor
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SnXnor::SnXnor(ymuint32 id,
	       const vector<SimNode*>& inputs) :
  SnXor(id, inputs)
{
}

// @brief デストラクタ
SnXnor::~SnXnor()
{
}

// @brief 正常値の計算を行う．(3値版)
Val3
SnXnor::_calc_gval3()
{
  Val3 val = mFanins[0]->gval();
  for (ymuint i = 1; i < mNfi; ++ i) {
    val = val ^ mFanins[i]->gval();
  }
  return ~val;
}

// @brief 故障値の計算を行う．(3値版)
Val3
SnXnor::_calc_fval3()
{
  Val3 val = mFanins[0]->fval();
  for (ymuint i = 1; i < mNfi; ++ i) {
    val = val ^ mFanins[i]->fval();
  }
  return ~val;
}

// @brief 内容をダンプする．
void
SnXnor::dump(ostream& s) const
{
  s << "XNOR(" << mFanins[0]->id();
  for (ymuint i = 1; i < mNfi; ++ i) {
    s << ", " << mFanins[i]->id();
  }
  s << ")" << endl;
}


//////////////////////////////////////////////////////////////////////
// SnXnor2
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SnXnor2::SnXnor2(ymuint32 id,
		 const vector<SimNode*>& inputs) :
  SnXor2(id, inputs)
{
}

// @brief デストラクタ
SnXnor2::~SnXnor2()
{
}

// @brief 正常値の計算を行う．(3値版)
Val3
SnXnor2::_calc_gval3()
{
  Val3 val;
  val =       mFanins[0]->gval();
  val = val ^ mFanins[1]->gval();
  return ~val;
}

// @brief 故障値の計算を行う．(3値版)
Val3
SnXnor2::_calc_fval3()
{
  Val3 val;
  val =       mFanins[0]->fval();
  val = val ^ mFanins[1]->fval();
  return ~val;
}

// @brief 内容をダンプする．
void
SnXnor2::dump(ostream& s) const
{
  s << "XNOR2(" << mFanins[0]->id();
  s << ", " << mFanins[1]->id();
  s << ")" << endl;
}

END_NAMESPACE_YM_ATPG_FSIM3
