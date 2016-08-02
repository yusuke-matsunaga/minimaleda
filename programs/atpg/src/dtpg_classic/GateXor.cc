
/// @file src/dtpg_classic/GateXor.cc
/// @brief GateXor の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: GateXor.cc 2203 2009-04-16 05:04:40Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "GateXor.h"


BEGIN_NAMESPACE_YM_ATPG_DTPGCL

//////////////////////////////////////////////////////////////////////
// クラス GateXor
//////////////////////////////////////////////////////////////////////
  
// @brief コンストラクタ
// @param[in] id ゲートID
// @param[in] node 元のノード
// @param[in] ni ファンイン数
GateXor::GateXor(ymuint id,
		 const TgNode* node,
		 ymuint ni) :
  Gate(id, node, ni)
{
}

// @brief デストラクタ
GateXor::~GateXor()
{
}

tTgGateType
GateXor::gate_type() const
{
  return kTgXor;
}

bool
GateXor::calc_obs(ymuint ipos) const
{
  for (ymuint i = 0; i < ipos; ++ i) {
    if ( fanin(i)->gval() == kValX ) {
      return false;
    }
  }
  for (ymuint i = ipos + 1; i < nfi(); ++ i) {
    if ( fanin(i)->gval() == kValX ) {
      return false;
    }
  }
  return true;
}

bool
GateXor::fwd_imp0_h()
{
  Val3 old_val = gval();
  if ( old_val == kValX ) {
    Val3 xval = (gate_type() == kTgXor) ? kVal0 : kVal1;
    bool all_fixed = true;
    for (ymuint i = 0; i < nfi(); ++ i) {
      Val3 i_val = fanin(i)->gval();
      if ( i_val == kValX ) {
	all_fixed = false;
	break;
      }
      if ( i_val == kVal1 ) {
	xval = ~xval;
      }
    }
    if ( all_fixed ) {
      if ( xval == kVal0 ) {
	return imp0_h();
      }
      else {
	return imp1_h();
      }
    }
  }
  else if ( old_val == kVal0 ) {
    return bwd_imp0_h();
  }
  else {
    return bwd_imp1_h();
  }
  return true;
}

bool
GateXor::fwd_imp1_h()
{
  Val3 old_val = gval();
  if ( old_val == kValX ) {
    Val3 xval = (gate_type() == kTgXor) ? kVal0 : kVal1;
    bool all_fixed = true;
    for (ymuint i = 0; i < nfi(); ++ i) {
      Val3 i_val = fanin(i)->gval();
      if ( i_val == kValX ) {
	all_fixed = false;
	break;
      }
      if ( i_val == kVal1 ) {
	xval = ~xval;
      }
    }
    if ( all_fixed ) {
      if ( xval == kVal0 ) {
	return imp0_h();
      }
      else {
	return imp1_h();
      }
    }
  }
  else if ( old_val == kVal0 ) {
    return bwd_imp0_h();
  }
  else {
    return bwd_imp1_h();
  }
  return true;
}

bool
GateXor::fwd_imp0_g()
{
  Val3 old_val = gval();
  if ( old_val == kValX ) {
    Val3 xval = (gate_type() == kTgXor) ? kVal0 : kVal1;
    bool all_fixed = true;
    for (ymuint i = 0; i < nfi(); ++ i) {
      Val3 i_val = fanin(i)->gval();
      if ( i_val == kValX ) {
	all_fixed = false;
	break;
      }
      if ( i_val == kVal1 ) {
	xval = ~xval;
      }
    }
    if ( all_fixed ) {
      if ( xval == kVal0 ) {
	return imp0_g();
      }
      else {
	return imp1_g();
      }
    }
  }
  else if ( old_val == kVal0 ) {
    return bwd_imp0_g();
  }
  else {
    return bwd_imp1_g();
  }
  return true;
}

bool
GateXor::fwd_imp1_g()
{
  Val3 old_val = gval();
  if ( old_val == kValX ) {
    Val3 xval = (gate_type() == kTgXor) ? kVal0 : kVal1;
    bool all_fixed = true;
    for (ymuint i = 0; i < nfi(); ++ i) {
      Val3 i_val = fanin(i)->gval();
      if ( i_val == kValX ) {
	all_fixed = false;
	break;
      }
      if ( i_val == kVal1 ) {
	xval = ~xval;
      }
    }
    if ( all_fixed ) {
      if ( xval == kVal0 ) {
	return imp0_g();
      }
      else {
	return imp1_g();
      }
    }
  }
  else if ( old_val == kVal0 ) {
    return bwd_imp0_g();
  }
  else {
    return bwd_imp1_g();
  }
  return true;
}

bool
GateXor::fwd_imp0_f()
{
  Val3 old_val = fval();
  if ( old_val == kValX ) {
    Val3 xval = (gate_type() == kTgXor) ? kVal0 : kVal1;
    bool all_fixed = true;
    for (ymuint i = 0; i < nfi(); ++ i) {
      Val3 i_val = fanin(i)->fval();
      if ( i_val == kValX ) {
	break;
      }
      if ( i_val == kVal1 ) {
	xval = ~xval;
      }
    }
    if ( all_fixed ) {
      if ( xval == kVal0 ) {
	return imp0_f();
      }
      else {
	return imp1_f();
      }
    }
  }
  else if ( old_val == kVal0 ) {
    return bwd_imp0_f();
  }
  else {
    return bwd_imp1_f();
  }
  return true;
}

bool
GateXor::fwd_imp1_f()
{
  Val3 old_val = fval();
  if ( old_val == kValX ) {
    Val3 xval = (gate_type() == kTgXor) ? kVal0 : kVal1;
    bool all_fixed = true;
    for (ymuint i = 0; i < nfi(); ++ i) {
      Val3 i_val = fanin(i)->fval();
      if ( i_val == kValX ) {
	break;
      }
      if ( i_val == kVal1 ) {
	xval = ~xval;
      }
    }
    if ( all_fixed ) {
      if ( xval == kVal0 ) {
	return imp0_f();
      }
      else {
	return imp1_f();
      }
    }
  }
  else if ( old_val == kVal0 ) {
    return bwd_imp0_f();
  }
  else {
    return bwd_imp1_f();
  }
  return true;
}

bool
GateXor::bwd_imp0_h()
{
  Val3 xval = (gate_type() == kTgXor) ? kVal0 : kVal1;
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
      xval = ~xval;
    }
  }
  if ( ucount == 0 ) {
    if ( xval == kVal1 ) {
#ifdef DEBUG
      cerr << "unable to justify" << endl;
#endif
      return false;
    }
  }
  else if ( ucount == 1 ) {
    if ( xval == kVal0 ) {
      return last->imp0_hb();
    }
    else {
      return last->imp1_hb();
    }
  }
  return true;
}

bool
GateXor::bwd_imp1_h()
{
  // 上と逆になっていることに注意
  Val3 xval = (gate_type() == kTgXor) ? kVal1 : kVal0;
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
      xval = ~xval;
    }
  }
  if ( ucount == 0 ) {
    if ( xval == kVal1 ) {
#ifdef DEBUG
      cerr << "unable to justify" << endl;;
#endif
      return false;
    }
  }
  else if ( ucount == 1 ) {
    if ( xval == kVal0 ) {
      return last->imp0_hb();
    }
    else {
      return last->imp1_hb();
    }
  }
  return true;
}

bool
GateXor::bwd_imp0_g()
{
  Val3 xval = (gate_type() == kTgXor) ? kVal0 : kVal1;
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
      xval = ~xval;
    }
  }
  if ( ucount == 0 ) {
    if ( xval == kVal1 ) {
#ifdef DEBUG
      cerr << "unable to justify" << endl;
#endif
      return false;
    }
  }
  else if ( ucount == 1 ) {
    if ( xval == kVal0 ) {
      return last->imp0_gb();
    }
    else {
      return last->imp1_gb();
    }
  }
  return true;
}

bool
GateXor::bwd_imp1_g()
{
  // 上と逆になっていることに注意
  Val3 xval = (gate_type() == kTgXor) ? kVal1 : kVal0;
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
      xval = ~xval;
    }
  }
  if ( ucount == 0 ) {
    if ( xval == kVal1 ) {
#ifdef DEBUG
      cerr << "unable to justify" << endl;
#endif
      return false;
    }
  }
  else if ( ucount == 1 ) {
    if ( xval == kVal0 ) {
      return last->imp0_gb();
    }
    else {
      return last->imp1_gb();
    }
  }
  return true;
}

bool
GateXor::bwd_imp0_f()
{
  Val3 xval = (gate_type() == kTgXor) ? kVal0 : kVal1;
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
      xval = ~xval;
    }
  }
  if ( ucount == 0 ) {
    if ( xval == kVal1 ) {
#ifdef DEBUG
      cerr << "unable to justify" << endl;
#endif
      return false;
    }
  }
  else if ( ucount == 1 ) {
    if ( xval == kVal0 ) {
      return last->imp0_fb();
    }
    else {
      return last->imp1_fb();
    }
  }
  return true;
}

bool
GateXor::bwd_imp1_f()
{
  // 上と逆になっていることに注意
  Val3 xval = (gate_type() == kTgXor) ? kVal1 : kVal0;
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
      xval = ~xval;
    }
  }
  if ( ucount == 0 ) {
    if ( xval == kVal1 ) {
#ifdef DEBUG
      cerr << "unable to justify" << endl;
#endif
      return false;
    }
  }
  else if ( ucount == 1 ) {
    if ( xval == kVal0 ) {
      return last->imp0_fb();
    }
    else {
      return last->imp1_fb();
    }
  }
  return true;
}


//////////////////////////////////////////////////////////////////////
// クラス XGateNor
//////////////////////////////////////////////////////////////////////
  
// @brief コンストラクタ
// @param[in] id ゲートID
// @param[in] node 元のノード
// @param[in] ni ファンイン数
XGateNor::XGateNor(ymuint id,
		   const TgNode* node,
		   ymuint ni) :
  GateXor(id, node, ni)
{
}

// @brief デストラクタ
XGateNor::~XGateNor()
{
}

tTgGateType
XGateNor::gate_type() const
{
  return kTgXnor;
}

END_NAMESPACE_YM_ATPG_DTPGCL
