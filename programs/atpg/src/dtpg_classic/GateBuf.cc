
/// @file src/dtpg_classic/GateBuf.cc
/// @brief GateBuf の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: GateBuf.cc 2203 2009-04-16 05:04:40Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "GateBuf.h"


BEGIN_NAMESPACE_YM_ATPG_DTPGCL

//////////////////////////////////////////////////////////////////////
// クラス GateBuf
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] id ゲートID
// @param[in] node 元のノード
GateBuf::GateBuf(ymuint id,
		 const TgNode* node) :
  Gate(id, node, 1)
{
}

GateBuf::~GateBuf()
{
}

tTgGateType
GateBuf::gate_type() const
{
  return kTgBuff;
}

bool
GateBuf::calc_obs(ymuint) const
{
  return true;
}

bool
GateBuf::fwd_imp0_h()
{
  return imp0_h();
}

bool
GateBuf::fwd_imp1_h()
{
  return imp1_h();
}

bool
GateBuf::fwd_imp0_g()
{
  return imp0_g();
}

bool
GateBuf::fwd_imp1_g()
{
  return imp1_g();
}

bool
GateBuf::fwd_imp0_f()
{
  return imp0_f();
}

bool
GateBuf::fwd_imp1_f()
{
  return imp1_f();
}

bool
GateBuf::bwd_imp0_h()
{
  Gate* i_gate = fanin(0);
  return i_gate->imp0_hb();
}

bool
GateBuf::bwd_imp1_h()
{
  Gate* i_gate = fanin(0);
  return i_gate->imp1_hb();
}

bool
GateBuf::bwd_imp0_g()
{
  Gate* i_gate = fanin(0);
  return i_gate->imp0_gb();
}

bool
GateBuf::bwd_imp1_g()
{
  Gate* i_gate = fanin(0);
  return i_gate->imp1_gb();
}

bool
GateBuf::bwd_imp0_f()
{
  Gate* i_gate = fanin(0);
  return i_gate->imp0_fb();
}

bool
GateBuf::bwd_imp1_f()
{
  Gate* i_gate = fanin(0);
  return i_gate->imp1_fb();
}


//////////////////////////////////////////////////////////////////////
// クラス GateNot
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] id ゲートID
// @param[in] node 元のノード
GateNot::GateNot(ymuint id,
		 const TgNode* node) :
  GateBuf(id, node)
{
}

GateNot::~GateNot()
{
}

tTgGateType
GateNot::gate_type() const
{
  return kTgNot;
}

bool
GateNot::fwd_imp0_h()
{
  return imp1_h();
}

bool
GateNot::fwd_imp1_h()
{
  return imp0_h();
}

bool
GateNot::fwd_imp0_g()
{
  return imp1_g();
}

bool
GateNot::fwd_imp1_g()
{
  return imp0_g();
}

bool
GateNot::fwd_imp0_f()
{
  return imp1_f();
}

bool
GateNot::fwd_imp1_f()
{
  return imp0_f();
}

bool
GateNot::bwd_imp0_h()
{
  Gate* i_gate = fanin(0);
  return i_gate->imp1_hb();
}

bool
GateNot::bwd_imp1_h()
{
  Gate* i_gate = fanin(0);
  return i_gate->imp0_hb();
}

bool
GateNot::bwd_imp0_g()
{
  Gate* i_gate = fanin(0);
  return i_gate->imp1_gb();
}

bool
GateNot::bwd_imp1_g()
{
  Gate* i_gate = fanin(0);
  return i_gate->imp0_gb();
}

bool
GateNot::bwd_imp0_f()
{
  Gate* i_gate = fanin(0);
  return i_gate->imp1_fb();
}

bool
GateNot::bwd_imp1_f()
{
  Gate* i_gate = fanin(0);
  return i_gate->imp0_fb();
}

END_NAMESPACE_YM_ATPG_DTPGCL
