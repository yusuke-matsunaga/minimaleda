
/// @file fsim/SnInput.cc
/// @brief SnInput の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
/// 
/// $Id: SnInput.cc 2203 2009-04-16 05:04:40Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "SnInput.h"


BEGIN_NAMESPACE_YM_ATPG_FSIM

//////////////////////////////////////////////////////////////////////
// SnInput
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SnInput::SnInput(ymuint32 id) :
  SimNode(id)
{
  set_level(0);
}

// @brief デストラクタ
SnInput::~SnInput()
{
}

// @brief ファンイン数を得る．
ymuint
SnInput::nfi() const
{
  return 0;
}

// @brief pos 番めのファンインを得る．
SimNode*
SnInput::fanin(ymuint pos) const
{
  assert_not_reached(__FILE__, __LINE__);
  return NULL;
}

// @brief 正常値の計算を行う．(2値版)
PackedVal
SnInput::_calc_gval2()
{
  return kPvAll0;
}

// @brief 故障値の計算を行う．(2値版)
PackedVal
SnInput::_calc_fval2()
{
  return kPvAll0;
}

// @brief ゲートの入力から出力までの可観測性を計算する．(2値版)
PackedVal
SnInput::calc_gobs2(ymuint ipos)
{
  return kPvAll0;
}

// @brief 内容をダンプする．
void
SnInput::dump(ostream& s) const
{
  s << "INPUT" << endl;
}


//////////////////////////////////////////////////////////////////////
// SnBuff
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SnBuff::SnBuff(ymuint32 id,
	       const vector<SimNode*>& inputs) :
  SnGate1(id, inputs)
{
}

// @brief デストラクタ
SnBuff::~SnBuff()
{
}

// @brief 正常値の計算を行う．(2値版)
PackedVal
SnBuff::_calc_gval2()
{
  return mFanin->gval();
}

// @brief 故障値の計算を行う．(2値版)
PackedVal
SnBuff::_calc_fval2()
{
  return mFanin->fval();
}

// @brief ゲートの入力から出力までの可観測性を計算する．(2値版)
PackedVal
SnBuff::calc_gobs2(ymuint ipos)
{
  return kPvAll1;
}

// @brief 内容をダンプする．
void
SnBuff::dump(ostream& s) const
{
  s << "BUFF(" << mFanin->id() << ")" << endl;
}


//////////////////////////////////////////////////////////////////////
// SnNot
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SnNot::SnNot(ymuint32 id,
	     const vector<SimNode*>& inputs) :
  SnBuff(id, inputs)
{
}

// @brief デストラクタ
SnNot::~SnNot()
{
}

// @brief 正常値の計算を行う．(2値版)
PackedVal
SnNot::_calc_gval2()
{
  return ~mFanin->gval();
}

// @brief 故障値の計算を行う．(2値版)
PackedVal
SnNot::_calc_fval2()
{
  return ~mFanin->fval();
}

// @brief 内容をダンプする．
void
SnNot::dump(ostream& s) const
{
  s << "NOT(" << mFanin->id() << ")" << endl;
}

END_NAMESPACE_YM_ATPG_FSIM
