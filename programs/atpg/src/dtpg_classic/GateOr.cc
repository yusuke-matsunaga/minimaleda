
/// @file src/dtpg_classic/GateOr.cc
/// @brief GateOr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: GateOr.cc 2203 2009-04-16 05:04:40Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "GateOr.h"


BEGIN_NAMESPACE_YM_ATPG_DTPGCL

//////////////////////////////////////////////////////////////////////
// クラス GateOr
//////////////////////////////////////////////////////////////////////
  
// @brief コンストラクタ
// @param[in] id ゲートID
// @param[in] node 元のノード
// @param[in] ni ファンイン数
GateOr::GateOr(ymuint id,
	       const TgNode* node,
	       ymuint ni) :
  Gate(id, node, ni)
{
}

// @brief デストラクタ
GateOr::~GateOr()
{
}

tTgGateType
GateOr::gate_type() const
{
  return kTgOr;
}

bool
GateOr::calc_obs(ymuint ipos) const
{
  for (ymuint i = 0; i < ipos; ++ i) {
    if ( fanin(i)->gval() != kVal0 ) {
      return false;
    }
  }
  for (ymuint i = ipos + 1; i < nfi(); ++ i) {
    if ( fanin(i)->gval() != kVal0 ) {
      return false;
    }
  }
  return true;
}

bool
GateOr::fwd_imp0_h()
{
  Val3 old_val = gval();
  if ( old_val == kValX ) {
    for (ymuint i = 0; i < nfi(); ++ i) {
      if ( fanin(i)->gval() != kVal0 ) {
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
GateOr::fwd_imp1_h()
{
  return imp1_h();
}

bool
GateOr::fwd_imp0_g()
{
  Val3 old_val = gval();
  if ( old_val == kValX ) {
    for (ymuint i = 0; i < nfi(); ++ i) {
      if ( fanin(i)->gval() != kVal0 ) {
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
GateOr::fwd_imp1_g()
{
  return imp1_g();
}

bool
GateOr::fwd_imp0_f()
{
  Val3 old_val = fval();
  if ( old_val == kValX ) {
    for (ymuint i = 0; i < nfi(); ++ i) {
      if ( fanin(i)->fval() != kVal0 ) {
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
GateOr::fwd_imp1_f()
{
  return imp1_f();
}

bool
GateOr::bwd_imp0_h()
{
  for (ymuint i = 0; i < nfi(); ++ i) {
    if ( !fanin(i)->imp0_hb() ) {
      return false;
    }
  }
  return true;
}

bool
GateOr::bwd_imp1_h()
{
  return check_last_h();
}

bool
GateOr::bwd_imp0_g()
{
  for (ymuint i = 0; i < nfi(); ++ i) {
    if ( !fanin(i)->imp0_gb() ) {
      return false;
    }
  }
  return true;
}

bool
GateOr::bwd_imp1_g()
{
  return check_last_g();
}

bool
GateOr::bwd_imp0_f()
{
  for (ymuint i = 0; i < nfi(); ++ i) {
    if ( !fanin(i)->imp0_fb() ) {
      return false;
    }
  }
  return true;
}

bool
GateOr::bwd_imp1_f()
{
  return check_last_f();
}

bool
GateOr::check_last_h()
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
    else if ( val1 == kVal1 ) {
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
    return last->imp1_hb();
  }
  return true;
}

bool
GateOr::check_last_g()
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
    else if ( val1 == kVal1 ) {
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
    return last->imp1_gb();
  }
  return true;
}

bool
GateOr::check_last_f()
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
    else if ( val1 == kVal1 ) {
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
    return last->imp1_fb();
  }
  return true;
}

Val3
GateOr::c_val() const
{
  return kVal1;
}

Val3
GateOr::o_val() const
{
  return kVal1;
}


//////////////////////////////////////////////////////////////////////
// クラス GateNor
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] id ゲートID
// @param[in] node 元のノード
// @param[in] ni ファンイン数
GateNor::GateNor(ymuint id,
		 const TgNode* node,
		 ymuint ni) :
  GateOr(id, node, ni)
{
}

// @brief デストラクタ
GateNor::~GateNor()
{
}

tTgGateType
GateNor::gate_type() const
{
  return kTgNor;
}

bool
GateNor::fwd_imp0_h()
{
  Val3 old_val = gval();
  if ( old_val == kValX ) {
    for (ymuint i = 0; i < nfi(); ++ i) {
      if ( fanin(i)->gval() != kVal0 ) {
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
GateNor::fwd_imp1_h()
{
  return imp0_h();
}

bool
GateNor::fwd_imp0_g()
{
  Val3 old_val = gval();
  if ( old_val == kValX ) {
    for (ymuint i = 0; i < nfi(); ++ i) {
      if ( fanin(i)->gval() != kVal0 ) {
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
GateNor::fwd_imp1_g()
{
  return imp0_g();
}

bool
GateNor::fwd_imp0_f()
{
  Val3 old_val = fval();
  if ( old_val == kValX ) {
    for (ymuint i = 0; i < nfi(); ++ i) {
      if ( fanin(i)->fval() != kVal0 ) {
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
GateNor::fwd_imp1_f()
{
  return imp0_f();
}

bool
GateNor::bwd_imp0_h()
{
  return check_last_h();
}

bool
GateNor::bwd_imp1_h()
{
  for (ymuint i = 0; i < nfi(); ++ i) {
    if ( !fanin(i)->imp0_hb() ) {
      return false;
    }
  }
  return true;
}

bool
GateNor::bwd_imp0_g()
{
  return check_last_g();
}

bool
GateNor::bwd_imp1_g()
{
  for (ymuint i = 0; i < nfi(); ++ i) {
    if ( !fanin(i)->imp0_gb() ) {
      return false;
    }
  }
  return true;
}

bool
GateNor::bwd_imp0_f()
{
  return check_last_f();
}

bool
GateNor::bwd_imp1_f()
{
  for (ymuint i = 0; i < nfi(); ++ i) {
    if ( !fanin(i)->imp0_fb() ) {
      return false;
    }
  }
  return true;
}

Val3
GateNor::o_val() const
{
  return kVal0;
}

END_NAMESPACE_YM_ATPG_DTPGCL
