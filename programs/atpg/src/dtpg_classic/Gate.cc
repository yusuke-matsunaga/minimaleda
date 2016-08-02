
/// @file src/dtpg_classic/Gate.cc
/// @brief Gate の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: Gate.cc 2203 2009-04-16 05:04:40Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "Gate.h"
#include "ym_tgnet/TgNode.h"
#include "SearchMgr.h"
#include "PGraph.h"

//#define DEBUG 1


BEGIN_NAMESPACE_YM_ATPG_DTPGCL

// @brief コンストラクタ
// @param[in] id ID 番号
// @param[in] node もとのノード
// @param[in] ni 入力数
Gate::Gate(ymuint id,
	   const TgNode* node,
	   ymuint ni) :
  mId(id),
  mTgNode(node),
  mNi(ni),
  mFanins(NULL),
  mSortedPos(NULL),
  mNo(0),
  mFanouts(NULL),
  mActNo(0),
  mActFanouts(NULL)
{
  mFlags = 0U;
  mMarkCount = 0U;
  
  if ( ni ) {
    mFanins = new Gate*[ni];
    mSortedPos = new ymuint32[ni];
    for (ymuint i = 0; i < ni; ++ i) {
      mSortedPos[i] = i;
    }
  }
  
  mLevel = 0;
  mState = 0;
  
  mEqGate = this;
}

// @brief デストラクタ
Gate::~Gate()
{
  delete [] mFanins;
  delete [] mSortedPos;
  delete [] mFanouts;
  delete [] mActFanouts;
}

// @brief ファンアウト用配列の確保を行う．
void
Gate::init_fogate(ymuint no)
{
  if ( mFanouts == NULL ) {
    mFanouts = new Fob[no];
    mActFanouts = new Fob[no + 1];
  }
}

// @brief ゲート名を得る．
const char*
Gate::name() const
{
  return mTgNode->name();
}

// @brief ゲート型を得る．
tTgGateType
Gate::gate_type() const
{
  return mTgNode->type();
}

// @brief 名前とゲート型を表す文字列を返す．
string
Gate::description() const
{
  string ans;
  ans = string(name()) + string(": ");
  switch ( gate_type() ) {
  case kTgInput:  ans += "INPUT"; break;
  case kTgConst0: ans += "CONST0"; break;
  case kTgConst1: ans += "CONST1"; break;
  case kTgBuff:   ans += "BUF"; break;
  case kTgNot:    ans += "NOT"; break;
  case kTgAnd:    ans += "AND"; break;
  case kTgNand:   ans += "NAND"; break;
  case kTgOr:     ans += "OR"; break;
  case kTgNor:    ans += "NOR"; break;
  case kTgXor:    ans += "XOR"; break;
  case kTgXnor:   ans += "XNOR"; break;
  default:        ans += "UNKNOWN"; break;
  }
  return ans;
}

// @brief 外部入力の時 入力番号を返す．
ymuint
Gate::input_id() const
{
  return 0;
}

// @brief 制御値を得る．
Val3
Gate::c_val() const
{
  return kValX;
}

// @brief 制御出力値を得る．
Val3
Gate::o_val() const
{
  return kValX;
}

// @brief 故障ノードのときに true を返す．
bool
Gate::is_f_site() const
{
  return false;
}

bool
Gate::imp0_h()
{
#ifdef DEBUG
  cerr << name() << "::imp0_h()" << endl;
#endif

  Val3 old_val = gval();
  if ( old_val == kValX ) {
    sm.save_value(this);
    set_hval(kVal0);
    for (ymuint i = 0; i < act_nfo(); ++ i) {
      Gate* o_gate = act_fanout(i);
      if ( o_gate->chk_fcone() ) {
	if ( !o_gate->fwd_imp0_g() ||
	     !o_gate->fwd_imp0_f() ) {
	  return false;
	}
      }
      else {
	if ( !o_gate->fwd_imp0_h() ) {
	  return false;
	}
      }
    }
  }
  else if ( old_val == kVal1 ) {
#ifdef DEBUG
    cerr << "conflict with old value" << endl;
#endif
    return false;
  }
  
#ifdef DEBUG
  cerr << name() << "::imp0_h() end" << endl;
#endif
  return true;
}

bool
Gate::imp1_h()
{
#ifdef DEBUG
  cerr << name() << "::imp1_h()" << endl;
#endif

  Val3 old_val = gval();
  if ( old_val == kValX ) {
    sm.save_value(this);
    set_hval(kVal1);
    for (ymuint i = 0; i < act_nfo(); ++ i) {
      Gate* o_gate = act_fanout(i);
      if ( o_gate->chk_fcone() ) {
	if ( !o_gate->fwd_imp1_g() ||
	     !o_gate->fwd_imp1_f() ) {
	  return false;
	}
      }
      else {
	if ( !o_gate->fwd_imp1_h() ) {
	  return false;
	}
      }
    }
  }
  else if ( old_val == kVal0 ) {
#ifdef DEBUG
    cerr << "conflict with old value" << endl;
#endif
    return false;
  }
  
#ifdef DEBUG
  cerr << name() << "::imp1_h() end" << endl;
#endif
  return true;
}

bool
Gate::imp0_hb()
{
#ifdef DEBUG
  cerr << name() << "::imp0_hb()" << endl;
#endif

  Val3 old_val = gval();
  if ( old_val == kValX ) {
    sm.save_value(this);
    set_hval(kVal0);
    for (ymuint i = 0; i < act_nfo(); ++ i) {
      Gate* o_gate = act_fanout(i);
      if ( o_gate->chk_fcone() ) {
	if ( !o_gate->fwd_imp0_g() ||
	     !o_gate->fwd_imp0_f() ) {
	  return false;
	}
      }
      else {
	if ( !o_gate->fwd_imp0_h() ) {
	  return false;
	}
      }
    }
    if ( !chk_term0() ) {
      if ( !bwd_imp0_h() ) {
	return false;
      }
    }
    return eq_imp0();
  }
  else if ( old_val == kVal1 ) {
#ifdef DEBUG
    cerr << "conflict with old value" << endl;
#endif
    return false;
  }
  
#ifdef DEBUG
  cerr << name() << "::imp0_hb() end" << endl;
#endif
  return true;
}

bool
Gate::imp1_hb()
{
#ifdef DEBUG
  cerr << name() << "::imp1_hb()" << endl;
#endif

  Val3 old_val = gval();
  if ( old_val == kValX ) {
    sm.save_value(this);
    set_hval(kVal1);
    for (ymuint i = 0; i < act_nfo(); ++ i) {
      Gate* o_gate = act_fanout(i);
      if ( o_gate->chk_fcone() ) {
	if ( !o_gate->fwd_imp1_g() ||
	     !o_gate->fwd_imp1_f() ) {
	  return false;
	}
      }
      else {
	if ( !o_gate->fwd_imp1_h() ) {
	  return false;
	}
      }
    }
    if ( !chk_term1() ) {
      if ( !bwd_imp1_h() ) {
	return false;
      }
    }
    return eq_imp1();
  }
  else if ( old_val == kVal0 ) {
#ifdef DEBUG
    cerr << "conflict with old value" << endl;
#endif
    return false;
  }
  
#ifdef DEBUG
  cerr << name() << "::imp1_hb() end" << endl;
#endif
  return true;
}

bool
Gate::imp0_g()
{
#ifdef DEBUG
  cerr << name() << "::imp0_g()" << endl;
#endif

  Val3 old_val = gval();
  if ( old_val == kValX ) {
    sm.save_value(this);
    set_gval(kVal0);
    if ( fval() == kVal0 ) {
#ifdef DEBUG
      cerr << "became same value" << endl;
#endif
      pgraph.add_indif(this);
    }
    for (ymuint i = 0; i < act_nfo(); ++ i) {
      Gate* o_gate = act_fanout(i);
      if ( !o_gate->fwd_imp0_g() ) {
	return false;
      }
    }
  }
  else if ( old_val == kVal1 ) {
#ifdef DEBUG
    cerr << "conflict with old value" << endl;
#endif
    return false;
  }
  
#ifdef DEBUG
  cerr << name() << "::imp0_g() end" << endl;
#endif
  return true;
}

bool
Gate::imp1_g()
{
#ifdef DEBUG
  cerr << name() << "::imp1_g()" << endl;
#endif

  Val3 old_val = gval();
  if ( old_val == kValX ) {
    sm.save_value(this);
    set_gval(kVal1);
    if ( fval() == kVal1 ) {
#ifdef DEBUG
      cerr << "became same value" << endl;
#endif
      pgraph.add_indif(this);
    }
    for (ymuint i = 0; i < act_nfo(); ++ i) {
      Gate* o_gate = act_fanout(i);
      if ( !o_gate->fwd_imp1_g() ) {
	return false;
      }
    }
  }
  else if ( old_val == kVal0 ) {
#ifdef DEBUG
    cerr << "conflict with old value" << endl;
#endif
    return false;
  }
  
#ifdef DEBUG
  cerr << name() << "::imp1_g() end" << endl;
#endif
  return true;
}

bool
Gate::imp0_gb()
{
  if ( chk_fcone() ) {
#ifdef DEBUG
    cerr << name() << "::imp0_gb()" << endl;
#endif

    Val3 old_val = gval();
    if ( old_val == kValX ) {
      sm.save_value(this);
      set_gval(kVal0);
      if ( fval() == kVal0 ) {
#ifdef DEBUG
	cerr << "became same value" << endl;
#endif
	pgraph.add_indif(this);
      }
      for (ymuint i = 0; i < act_nfo(); ++ i) {
	Gate* o_gate = act_fanout(i);
	if ( !o_gate->fwd_imp0_g() ) {
	  return false;
	}
      }
      return bwd_imp0_g() && eq_imp0();
    }
    else if ( old_val == kVal1 ) {
#ifdef DEBUG
      cerr << "conflict with old value" << endl;
#endif
      return false;
    }

#ifdef DEBUG
    cerr << name() << "::imp0_gb() end" << endl;
#endif
    return true;
  }

  return imp0_hb();
}

bool
Gate::imp1_gb()
{
  if ( chk_fcone() ) {
#ifdef DEBUG
    cerr << name() << "::imp1_gb()" << endl;
#endif

    Val3 old_val = gval();
    if ( old_val == kValX ) {
      sm.save_value(this);
      set_gval(kVal1);
      if ( fval() == kVal1 ) {
#ifdef DEBUG
	cerr << "became same value" << endl;
#endif
	pgraph.add_indif(this);
      }
      for (ymuint i = 0; i < act_nfo(); ++ i) {
	Gate* o_gate = act_fanout(i);
	if ( !o_gate->fwd_imp1_g() ) {
	  return false;
	}
      }
      return bwd_imp1_g() && eq_imp1();
    }
    else if ( old_val == kVal0 ) {
#ifdef DEBUG
      cerr << "conflict with old value" << endl;
#endif
      return false;
    }

#ifdef DEBUG
    cerr << name() << "::imp1_gb() end" << endl;
#endif
    return true;
  }

  return imp1_hb();
}

bool
Gate::imp0_f()
{
#ifdef DEBUG
  cerr << name() << "::imp0_f()" << endl;
#endif
    
  Val3 old_val = fval();
  if ( old_val == kValX ) {
    sm.save_value(this);
    set_fval(kVal0);
    if ( gval() == kVal0 ) {
#ifdef DEBUG
      cerr << "became same value" << endl;
#endif
      pgraph.add_indif(this);
    }
    for (ymuint i = 0; i < act_nfo(); ++ i) {
      Gate* o_gate = act_fanout(i);
      if ( !o_gate->fwd_imp0_f() ) {
	return false;
      }
    }
  }
  else if ( old_val == kVal1 ) {
#ifdef DEBUG
    cerr << "conflict with old value" << endl;
#endif
    return false;
  }
  
#ifdef DEBUG
  cerr << name() << "::imp0_f() end" << endl;
#endif
  return true;
}

bool
Gate::imp1_f()
{
#ifdef DEBUG
  cerr << name() << "::imp1_f()" << endl;
#endif
    
  Val3 old_val = fval();
  if ( old_val == kValX ) {
    sm.save_value(this);
    set_fval(kVal1);
    if ( gval() == kVal1 ) {
#ifdef DEBUG
      cerr << "became same value" << endl;
#endif
      pgraph.add_indif(this);
    }
    for (ymuint i = 0; i < act_nfo(); ++ i) {
      Gate* o_gate = act_fanout(i);
      if ( !o_gate->fwd_imp1_f() ) {
	return false;
      }
    }
  }
  else if ( old_val == kVal0 ) {
#ifdef DEBUG
    cerr << "conflict with old value" << endl;
#endif
    return false;
  }
  
#ifdef DEBUG
  cerr << name() << "::imp1_f() end" << endl;
#endif
  return true;
}

bool
Gate::imp0_fb()
{
  if ( chk_fcone() ) {
#ifdef DEBUG
    cerr << name() << "::imp0_fb()" << endl;
#endif
    
    Val3 old_val = fval();
    if ( old_val == kValX ) {
      sm.save_value(this);
      set_fval(kVal0);
      if ( gval() == kVal0 ) {
#ifdef DEBUG
	cerr << "became same value" << endl;
#endif
	pgraph.add_indif(this);
      }
      for (ymuint i = 0; i < act_nfo(); ++ i) {
	Gate* o_gate = act_fanout(i);
	if ( !o_gate->fwd_imp0_f() ) {
	  return false;
	}
      }
      return bwd_imp0_f();
    }
    else if ( old_val == kVal1 ) {
#ifdef DEBUG
      cerr << "conflict with old value" << endl;
#endif
      return false;
    }

#ifdef DEBUG
    cerr << name() << "::imp0_fb() end" << endl;
#endif
    return true;
  }

  return imp0_hb();
}

bool
Gate::imp1_fb()
{
  if ( chk_fcone() ) {
#ifdef DEBUG
    cerr << name() << "::imp1_fb()" << endl;
#endif
    
    Val3 old_val = fval();
    if ( old_val == kValX ) {
      sm.save_value(this);
      set_fval(kVal1);
      if ( gval() == kVal1 ) {
#ifdef DEBUG
	cerr << "became same value" << endl;
#endif
	pgraph.add_indif(this);
      }
      for (ymuint i = 0; i < act_nfo(); ++ i) {
	Gate* o_gate = act_fanout(i);
	if ( !o_gate->fwd_imp1_f() ) {
	  return false;
	}
      }
      return bwd_imp1_f();
    }
    else if ( old_val == kVal0 ) {
#ifdef DEBUG
      cerr << "conflict with old value";
#endif
      return false;
    }

#ifdef DEBUG
    cerr << name() << "::imp1_fb() end" << endl;
#endif
    return true;
  }

  return imp1_hb();
}

bool
Gate::eq_imp0()
{
  Gate* node1 = eq_gate();
  if ( node1 != NULL ) {
    for ( ; node1 != this; node1 = node1->eq_gate()) {
      if ( node1->is_active() && node1->gval() != kVal0 ) {
	if ( !node1->imp0_gb() ) {
	  return false;
	}
      }
    }
  }
  return true;
}

bool
Gate::eq_imp1()
{
  Gate* node1 = eq_gate();
  if ( node1 != NULL ) {
    for ( ; node1 != this; node1 = node1->eq_gate()) {
      if ( node1->is_active() && node1->gval() != kVal1 ) {
	if ( !node1->imp1_gb() ) {
	  return false;
	}
      }
    }
  }
  return true;
}

END_NAMESPACE_YM_ATPG_DTPGCL
