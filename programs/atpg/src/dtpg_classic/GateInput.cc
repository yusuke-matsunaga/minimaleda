
/// @file src/dtpg_classic/GateInput.cc
/// @brief GateInput の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: GateInput.cc 2203 2009-04-16 05:04:40Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#if HAVE_CONFIG_H
#include "atpg_config.h"
#endif


#include "GateInput.h"


BEGIN_NAMESPACE_YM_ATPG_DTPGCL

//////////////////////////////////////////////////////////////////////
// クラス GateInput
//////////////////////////////////////////////////////////////////////
  
// @biref コンストラクタ
// @param[in] id ゲートID
// @param[in] node 元のノード
// @param[in] input_id 入力番号
GateInput::GateInput(ymuint id,
		     const TgNode* node,
		     ymuint input_id) :
  Gate(id, node, 0),
  mInputId(input_id)
{
  set_pi();
}

// @brief デストラクタ
GateInput::~GateInput()
{
}

ymuint
GateInput::input_id() const
{
  return mInputId;
}

bool
GateInput::calc_obs(ymuint) const
{
  return true;
}

// @brief 正常値/故障値の前方含意を行う．
bool
GateInput::fwd_imp0_h()
{
  assert_not_reached(__FILE__, __LINE__);
  return false;
}

// @brief 正常値/故障値の前方含意を行う．
bool
GateInput::fwd_imp1_h()
{
  assert_not_reached(__FILE__, __LINE__);
  return false;
}

// @brief 正常値の前方含意を行う．
bool
GateInput::fwd_imp0_g()
{
  assert_not_reached(__FILE__, __LINE__);
  return false;
}

// @brief 正常値の前方含意を行う．
bool
GateInput::fwd_imp1_g()
{
  assert_not_reached(__FILE__, __LINE__);
  return false;
}

// @brief 故障値の前方含意を行う．
bool
GateInput::fwd_imp0_f()
{
  assert_not_reached(__FILE__, __LINE__);
  return false;
}

// @brief 故障値の前方含意を行う．
bool
GateInput::fwd_imp1_f()
{
  assert_not_reached(__FILE__, __LINE__);
  return false;
}

// @brief 正常値/故障値の後方含意を行う．
bool
GateInput::bwd_imp0_h()
{
  return true;
}

// @brief 正常値/故障値の後方含意を行う．
bool
GateInput::bwd_imp1_h()
{
  return true;
}

// @brief 正常値の後方含意を行う．
bool
GateInput::bwd_imp0_g()
{
  return true;
}

// @brief 正常値の後方含意を行う．
bool
GateInput::bwd_imp1_g()
{
  return true;
}

// @brief 故障値の後方含意を行う．
bool
GateInput::bwd_imp0_f()
{
  return true;
}

// @brief 故障値の後方含意を行う．
bool
GateInput::bwd_imp1_f()
{
  return true;
}

END_NAMESPACE_YM_ATPG_DTPGCL
