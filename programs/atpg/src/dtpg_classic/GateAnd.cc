
/// @file src/dtpg_classic/GateAnd.cc
/// @brief GateAnd の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: GateAnd.cc 2203 2009-04-16 05:04:40Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "GateAnd.h"


BEGIN_NAMESPACE_YM_ATPG_DTPGCL

//////////////////////////////////////////////////////////////////////
// クラス GateAnd
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] id ゲートID
// @param[in] node 元のノード
// @param[in] ni ファンイン数
GateAnd::GateAnd(ymuint id,
		 const TgNode* node,
		 ymuint ni) :
  Gate(id, node, ni)
{
}

// @brief デストラクタ
GateAnd::~GateAnd()
{
}

tTgGateType
GateAnd::gate_type() const
{
  return kTgAnd;
}

bool
GateAnd::calc_obs(ymuint ipos) const
{
  for (ymuint i = 0; i < ipos; ++ i) {
    if ( fanin(i)->gval() != kVal1 ) {
      return false;
    }
  }
  for (ymuint i = ipos + 1; i < nfi(); ++ i) {
    if ( fanin(i)->gval() != kVal1 ) {
      return false;
    }
  }
  return true;
}

bool
GateAnd::fwd_imp0_h()
{
  return imp0_h();
}

bool
GateAnd::fwd_imp1_h()
{
  Val3 old_val = gval();
  if ( old_val == kValX ) {
    for (ymuint i = 0; i < nfi(); ++ i) {
      if ( fanin(i)->gval() != kVal1 ) {
	return true;
      }
    }
    return imp1_h();
  }
  else if ( old_val == kVal0 ) {
    return check_last_h();
  }
  return true;
}

bool
GateAnd::fwd_imp0_g()
{
  return imp0_g();
}

bool
GateAnd::fwd_imp1_g()
{
  Val3 old_val = gval();
  if ( old_val == kValX ) {
    for (ymuint i = 0; i < nfi(); ++ i) {
      if ( fanin(i)->gval() != kVal1 ) {
	return true;
      }
    }
    return imp1_g();
  }
  else if ( old_val == kVal0 ) {
    return check_last_g();
  }
  return true;
}

bool
GateAnd::fwd_imp0_f()
{
  return imp0_f();
}

bool
GateAnd::fwd_imp1_f()
{
  Val3 old_val = fval();
  if ( old_val == kValX ) {
    for (ymuint i = 0; i < nfi(); ++ i) {
      if ( fanin(i)->fval() != kVal1 ) {
	return true;
      }
    }
    return imp1_f();
  }
  else if ( old_val == kVal0 ) {
    return check_last_f();
  }
  return true;
}

bool
GateAnd::bwd_imp0_h()
{
  return check_last_h();
}

bool
GateAnd::bwd_imp1_h()
{
  for (ymuint i = 0; i < nfi(); ++ i) {
    if ( !fanin(i)->imp1_hb() ) {
      return false;
    }
  }
  return true;
}

bool
GateAnd::bwd_imp0_g()
{
  return check_last_g();
}

bool
GateAnd::bwd_imp1_g()
{
  for (ymuint i = 0; i < nfi(); ++ i) {
    if ( !fanin(i)->imp1_gb() ) {
      return false;
    }
  }
  return true;
}

bool
GateAnd::bwd_imp0_f()
{
  return check_last_f();
}

bool
GateAnd::bwd_imp1_f()
{
  for (ymuint i = 0; i < nfi(); ++ i) {
    if ( !fanin(i)->imp1_fb() ) {
      return false;
    }
  }
  return true;
}

bool
GateAnd::check_last_h()
{
  int ucount = 0;
  Gate* last = NULL;
  for (ymuint i = 0; i < nfi(); ++ i) {
    Gate* i_gate = fanin(i);
    Val3 val1 = i_gate->gval();
    if ( val1 == kValX ) {
      ++ ucount;
      last = i_gate;
    }
    else if ( val1 == kVal0 ) {
      return true;
    }
  }
  if ( ucount == 0 ) {
#ifdef DEBUG
    cerr << "unable to justify" << endl;
#endif
    return false;
  }
  if ( ucount == 1 ) {
    return last->imp0_hb();
  }
  return true;
}

bool
GateAnd::check_last_g()
{
  int ucount = 0;
  Gate* last = NULL;
  for (ymuint i = 0; i < nfi(); ++ i) {
    Gate* i_gate = fanin(i);
    Val3 val1 = i_gate->gval();
    if ( val1 == kValX ) {
      ++ ucount;
      last = i_gate;
    }
    else if ( val1 == kVal0 ) {
      return true;
    }
  }
  if ( ucount == 0 ) {
#ifdef DEBUG
    cerr << "unable to justify" << endl;
#endif
    return false;
  }
  if ( ucount == 1 ) {
    return last->imp0_gb();
  }
  return true;
}

bool
GateAnd::check_last_f()
{
  int ucount = 0;
  Gate* last = NULL;
  for (ymuint i = 0; i < nfi(); ++ i) {
    Gate* i_gate = fanin(i);
    Val3 val1 = i_gate->fval();
    if ( val1 == kValX ) {
      ++ ucount;
      last = i_gate;
    }
    else if ( val1 == kVal0 ) {
      return true;
    }
  }
  if ( ucount == 0 ) {
#ifdef DEBUG
    cerr << "unable to justify" << endl;
#endif
    return false;
  }
  if ( ucount == 1 ) {
    return last->imp0_fb();
  }
  return true;
}

Val3
GateAnd::c_val() const
{
  return kVal0;
}

Val3
GateAnd::o_val() const
{
  return kVal0;
}


//////////////////////////////////////////////////////////////////////
// クラス GateNand
//////////////////////////////////////////////////////////////////////
  
// @brief コンストラクタ
// @param[in] id ゲートID
// @param[in] node 元のノード
// @param[in] ni ファンイン数
GateNand::GateNand(ymuint id,
		     const TgNode* node,
		     ymuint ni) :
  GateAnd(id, node, ni)
{
}
  
// @brief デストラクタ
GateNand::~GateNand()
{
}

tTgGateType
GateNand::gate_type() const
{
  return kTgNand;
}

bool
GateNand::fwd_imp0_h()
{
  return imp1_h();
}

bool
GateNand::fwd_imp1_h()
{
  Val3 old_val = gval();
  if ( old_val == kValX ) {
    for (ymuint i = 0; i < nfi(); ++ i) {
      if ( fanin(i)->gval() != kVal1 ) {
	return true;
      }
    }
    return imp0_h();
  }
  else if ( old_val == kVal1 ) {
    return check_last_h();
  }
  return true;
}

bool
GateNand::fwd_imp0_g()
{
  return imp1_g();
}

bool
GateNand::fwd_imp1_g()
{
  Val3 old_val = gval();
  if ( old_val == kValX ) {
    for (ymuint i = 0; i < nfi(); ++ i) {
      if ( fanin(i)->gval() != kVal1 ) {
	return true;
      }
    }
    return imp0_g();
  }
  else if ( old_val == kVal1 ) {
    return check_last_g();
  }
  return true;
}

bool
GateNand::fwd_imp0_f()
{
  return imp1_f();
}

bool
GateNand::fwd_imp1_f()
{
  Val3 old_val = fval();
  if ( old_val == kValX ) {
    for (ymuint i = 0; i < nfi(); ++ i) {
      if ( fanin(i)->fval() != kVal1 ) {
	return true;
      }
    }
    return imp0_f();
  }
  else if ( old_val == kVal1 ) {
    return check_last_f();
  }
  return true;
}

bool
GateNand::bwd_imp0_h()
{
  for (ymuint i = 0; i < nfi(); ++ i) {
    if ( !fanin(i)->imp1_hb() ) {
      return false;
    }
  }
  return true;
}

bool
GateNand::bwd_imp1_h()
{
  return check_last_h();
}

bool
GateNand::bwd_imp0_g()
{
  for (ymuint i = 0; i < nfi(); ++ i) {
    if ( !fanin(i)->imp1_gb() ) {
      return false;
    }
  }
  return true;
}

bool
GateNand::bwd_imp1_g()
{
  return check_last_g();
}

bool
GateNand::bwd_imp0_f()
{
  for (ymuint i = 0; i < nfi(); ++ i) {
    if ( !fanin(i)->imp1_fb() ) {
      return false;
    }
  }
  return true;
}

bool
GateNand::bwd_imp1_f()
{
  return check_last_f();
}

Val3
GateNand::o_val() const
{
  return kVal1;
}

END_NAMESPACE_YM_ATPG_DTPGCL
