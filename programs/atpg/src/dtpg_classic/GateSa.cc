
/// @file src/dtpg_classic/GateSa.cc
/// @brief GateSa の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: GateSa.cc 2203 2009-04-16 05:04:40Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "Gate.h"
#include "GateSa.h"


BEGIN_NAMESPACE_YM_ATPG_DTPGCL

//////////////////////////////////////////////////////////////////////
// クラス GateSa
//////////////////////////////////////////////////////////////////////

GateSa::GateSa() :
  Gate(0, NULL, 1)
{
}

/// @brief デストラクタ
GateSa::~GateSa()
{
}
  
bool
GateSa::is_f_site() const
{
  return true;
}

bool
GateSa::calc_obs(ymuint ipos) const
{
  return false;
}

bool
GateSa::fwd_imp0_h()
{
  assert_not_reached(__FILE__, __LINE__);
  return false;
}

bool
GateSa::fwd_imp1_h()
{
  assert_not_reached(__FILE__, __LINE__);
  return false;
}

bool
GateSa::fwd_imp0_g()
{
  return imp0_g();
}

bool
GateSa::fwd_imp1_g()
{
  return imp1_g();
}

bool
GateSa::bwd_imp0_h()
{
  assert_not_reached(__FILE__, __LINE__);
  return false;
}

bool
GateSa::bwd_imp1_h()
{
  assert_not_reached(__FILE__, __LINE__);
  return false;
}

bool
GateSa::bwd_imp0_g()
{
  Gate* i_gate = fanin(0);
  return i_gate->imp0_gb();
}

bool
GateSa::bwd_imp1_g()
{
  Gate* i_gate = fanin(0);
  return i_gate->imp1_gb();
}

bool
GateSa::bwd_imp0_f()
{
  return true;
}

bool
GateSa::bwd_imp1_f()
{
  return true;
}


//////////////////////////////////////////////////////////////////////
// クラス GateSa0
//////////////////////////////////////////////////////////////////////

const char*
GateSa0::name() const
{
  return "GateSa0";
}

tTgGateType
GateSa0::gate_type() const
{
  return kTgConst0;
}

// @brief 名前とゲート型を表す文字列を返す．
string
GateSa0::description() const
{
  return "STUCK-AT-0";
}

bool
GateSa0::fwd_imp0_f()
{
  return imp0_f();
}

bool
GateSa0::fwd_imp1_f()
{
  return imp0_f();
}


//////////////////////////////////////////////////////////////////////
// クラス GateSa1
//////////////////////////////////////////////////////////////////////

const char*
GateSa1::name() const
{
  return "GateSa1";
}

tTgGateType
GateSa1::gate_type() const
{
  return kTgConst1;
}

// @brief 名前とゲート型を表す文字列を返す．
string
GateSa1::description() const
{
  return "STUCK-AT-1";
}

bool
GateSa1::fwd_imp0_f()
{
  return imp1_f();
}

bool
GateSa1::fwd_imp1_f()
{
  return imp1_f();
}

END_NAMESPACE_YM_ATPG_DTPGCL
