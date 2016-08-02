
/// @file src/dtpg_classic/GateConst.cc
/// @brief GateConst の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: GateConst.cc 2203 2009-04-16 05:04:40Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "GateConst.h"


BEGIN_NAMESPACE_YM_ATPG_DTPGCL

//////////////////////////////////////////////////////////////////////
// クラス GateConst
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] id ゲートID
// @param[in] node 元のノード
GateConst::GateConst(ymuint id,
		     const TgNode* node) :
  Gate(id, node, 0)
{
}

// デストラクタ
GateConst::~GateConst()
{
}

bool
GateConst::calc_obs(ymuint) const
{
  return true;
}

// @brief 正常値/故障値の前方含意を行う．
bool
GateConst::fwd_imp0_h()
{
  assert_not_reached(__FILE__, __LINE__);
  return false;
}

// @brief 正常値/故障値の前方含意を行う．
bool
GateConst::fwd_imp1_h()
{
  assert_not_reached(__FILE__, __LINE__);
  return false;
}

// @brief 正常値の前方含意を行う．
bool
GateConst::fwd_imp0_g()
{
  assert_not_reached(__FILE__, __LINE__);
  return false;
}

// @brief 正常値の前方含意を行う．
bool
GateConst::fwd_imp1_g()
{
  assert_not_reached(__FILE__, __LINE__);
  return false;
}

// @brief 故障値の前方含意を行う．
bool
GateConst::fwd_imp0_f()
{
  assert_not_reached(__FILE__, __LINE__);
  return false;
}

// @brief 故障値の前方含意を行う．
bool
GateConst::fwd_imp1_f()
{
  assert_not_reached(__FILE__, __LINE__);
  return false;
}

// @brief 正常値の後方含意を行う．
bool
GateConst::bwd_imp0_g()
{
  return true;
}

// @brief 正常値の後方含意を行う．
bool
GateConst::bwd_imp1_g()
{
  return true;
}

// @brief 故障値の後方含意を行う．
bool
GateConst::bwd_imp0_f()
{
  return true;
}

// @brief 故障値の後方含意を行う．
bool
GateConst::bwd_imp1_f()
{
  return true;
}


//////////////////////////////////////////////////////////////////////
// クラス GateConst0
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] id ゲートID
// @param[in] node 元のノード
GateConst0::GateConst0(ymuint id,
		       const TgNode* node) :
  GateConst(id, node)
{
}

// デストラクタ
GateConst0::~GateConst0()
{
}

tTgGateType
GateConst0::gate_type() const
{
  return kTgConst0;
}

bool
GateConst0::bwd_imp0_h()
{
  set_hjval(kVal0);
  return true;
}

bool
GateConst0::bwd_imp1_h()
{
  return false;
}


//////////////////////////////////////////////////////////////////////
// クラス GateConst1
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] id ゲートID
// @param[in] node 元のノード
GateConst1::GateConst1(ymuint id,
		       const TgNode* node) :
  GateConst(id, node)
{
}

// デストラクタ
GateConst1::~GateConst1()
{
}

tTgGateType
GateConst1::gate_type() const
{
  return kTgConst1;
}

bool
GateConst1::bwd_imp0_h()
{
  set_hjval(kVal1);
  return true;
}

bool
GateConst1::bwd_imp1_h()
{
  return false;
}

END_NAMESPACE_YM_ATPG_DTPGCL
