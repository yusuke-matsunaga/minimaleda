
/// @file src/dtpg_classic/backtrace.cc
/// @brief バックトレースを行う関数の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: backtrace.cc 2203 2009-04-16 05:04:40Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "atpg_nsdef.h"
#include "dtpgcl_nsdef.h"
#include "backtrace.h"
#include "Gate.h"
#include "Network.h"
#include "SearchMgr.h"


BEGIN_NAMESPACE_YM_ATPG_DTPGCL

// 故障伝搬の可能性があり値が未確定なフロンティアを探す．
bool
O_decision( Gate* gate,
	    Gate*& o_gate,
	    Val3& o_gval,
	    Val3& o_fval )
{
  for ( ; ; ) {
    ymuint no = gate->act_nfo();
    assert_cond(no > 0, __FILE__, __LINE__);

    bool found = false;
    for (ymuint i = 0; i < no; ++ i) {
      Gate* o_gate = gate->act_fanout(i);
      if ( o_gate->chk_pg() ) {
	gate = o_gate;
	found = true;
	break;
      }
    }
    if ( !found ) {
      FATAL("O_decision: Unexpected error!\n");
    }
    Val3 gval = gate->gval();
    Val3 fval = gate->fval();
    if ( gval == kValX ) {
      if ( fval == kValX ) {
	o_gate = gate;
	o_gval = kVal0;
	o_fval = kValX;
      }
      else {
	o_gate = gate;
	o_gval = ~fval;
	o_fval = kValX;
      }
      return true;
    }
    else if ( fval == kValX ) {
      o_gate = gate;
      o_gval = kValX;
      o_fval = ~gval;
      return true;
    }
    if ( gate->is_po() ) {
      // PO に到達していたらもう O_decision で割り当てる変数はない．
      return false;
    }
  }
}


BEGIN_NONAMESPACE

// 正常値が正当化されていないゲートを求める．
void
backtrace_g(Gate* gate,
	    Val3 val,
	    Gate*& o_gate,
	    Val3& o_gval)
{
  for ( ; ; ) {
    if ( gate->gval() == kValX ) {
      o_gate = gate;
      o_gval = val;
      break;
    }
    switch ( gate->gate_type() ) {
    case kTgBuff:
      gate = gate->fanin(0);
      break;

    case kTgNot:
      gate = gate->fanin(0);
      val = ~val;
      break;
      
    case kTgAnd:
      if ( val == kVal0 ) {
	for (ymuint i = 0; i < gate->nfi(); ++ i) {
	  Gate* i_gate = gate->sorted_fanin(i);
	  if ( i_gate->gval() != kVal1 ) {
	    val = kVal0;
	    gate = i_gate;
	    break;
	  }
	}
      }
      else {
	for (ymuint i = 0; i < gate->nfi(); ++ i) {
	  Gate* i_gate = gate->sorted_fanin(i);
	  if ( !i_gate->chk_gj() ) {
	    val = kVal1;
	    gate = i_gate;
	    break;
	  }
	}
      }
      break;
	
    case kTgNand:
      if ( val == kVal1 ) {
	for (ymuint i = 0; i < gate->nfi(); ++ i) {
	  Gate* i_gate = gate->sorted_fanin(i);
	  if ( i_gate->gval() != kVal1 ) {
	    val = kVal0;
	    gate = i_gate;
	    break;
	  }
	}
      }
      else {
	for (ymuint i = 0; i < gate->nfi(); ++ i) {
	  Gate* i_gate = gate->sorted_fanin(i);
	  if ( !i_gate->chk_gj() ) {
	    val = kVal1;
	    gate = i_gate;
	    break;
	  }
	}
      }
      break;
      
    case kTgOr:
      if ( val == kVal1 ) {
	for (ymuint i = 0; i < gate->nfi(); ++ i) {
	  Gate* i_gate = gate->sorted_fanin(i);
	  if ( i_gate->gval() != kVal0 ) {
	    val = kVal1;
	    gate = i_gate;
	    break;
	  }
	}
      }
      else {
	for (ymuint i = 0; i < gate->nfi(); ++ i) {
	  Gate* i_gate = gate->sorted_fanin(i);
	  if ( !i_gate->chk_gj() ) {
	    val = kVal0;
	    gate = i_gate;
	    break;
	  }
	}
      }
      break;
      
    case kTgNor:
      if ( val == kVal0 ) {
	for (ymuint i = 0; i < gate->nfi(); ++ i) {
	  Gate* i_gate = gate->sorted_fanin(i);
	  if ( i_gate->gval() != kVal0 ) {
	    val = kVal1;
	    gate = i_gate;
	    break;
	  }
	}
      }
      else {
	for (ymuint i = 0; i < gate->nfi(); ++ i) {
	  Gate* i_gate = gate->sorted_fanin(i);
	  if ( !i_gate->chk_gj() ) {
	    val = kVal0;
	    gate = i_gate;
	    break;
	  }
	}
      }
      break;
      
    case kTgXor:
    case kTgXnor:
      for (ymuint i = 0; i < gate->nfi(); ++ i) {
	Gate* i_gate = gate->sorted_fanin(i);
	if ( !i_gate->chk_gj() ) {
	  val = i_gate->gval();
	  if ( val == kValX ) {
	    val = kVal1;
	  }
	  gate = i_gate;
	  break;
	}
      }
      break;
      
    default:
      assert_not_reached(__FILE__, __LINE__);
      break;
    }
  }
}

// 故障値が正当化されていないゲートを求める．
void
backtrace_f(Gate* gate,
	    Val3 val,
	    Gate*& o_gate,
	    Val3& o_fval)
{
  for ( ; ; ) {
    if ( gate->fval() == kValX ) {
      o_gate = gate;
      o_fval = val;
      break;
    }
    switch ( gate->gate_type() ) {
    case kTgBuff:
      gate = gate->fanin(0);
      break;

    case kTgNot:
      val = ~val;
      gate = gate->fanin(0);
      break;
      
    case kTgAnd:
      if ( val == kVal0 ) {
	for (ymuint i = 0; i < gate->nfi(); ++ i) {
	  Gate* i_gate = gate->sorted_fanin(i);
	  if ( i_gate->fval() != kVal1 ) {
	    val = kVal0;
	    gate = i_gate;
	    break;
	  }
	}
      }
      else {
	for (ymuint i = 0; i < gate->nfi(); ++ i) {
	  Gate* i_gate = gate->sorted_fanin(i);
	  if ( !i_gate->chk_fj() ) {
	    val = kVal1;
	    gate = i_gate;
	    break;
	  }
	}
      }
      break;
      
    case kTgNand:
      if ( val == kVal1 ) {
	for (ymuint i = 0; i < gate->nfi(); ++ i) {
	  Gate* i_gate = gate->sorted_fanin(i);
	  if ( i_gate->fval() != kVal1 ) {
	    val = kVal0;
	    gate = i_gate;
	    break;
	  }
	}
      }
      else {
	for (ymuint i = 0; i < gate->nfi(); ++ i) {
	  Gate* i_gate = gate->sorted_fanin(i);
	  if ( !i_gate->chk_fj() ) {
	    val = kVal1;
	    gate = i_gate;
	    break;
	  }
	}
      }
      break;
      
    case kTgOr:
      if ( val == kVal1 ) {
	for (ymuint i = 0; i < gate->nfi(); ++ i) {
	  Gate* i_gate = gate->sorted_fanin(i);
	  if ( i_gate->fval() != kVal0 ) {
	    val = kVal1;
	    gate = i_gate;
	    break;
	  }
	}
      }
      else {
	for (ymuint i = 0; i < gate->nfi(); ++ i) {
	  Gate* i_gate = gate->sorted_fanin(i);
	  if ( !i_gate->chk_fj() ) {
	    val = kVal0;
	    gate = i_gate;
	    break;
	  }
	}
      }
      break;
      
    case kTgNor:
      if ( val == kVal0 ) {
	for (ymuint i = 0; i < gate->nfi(); ++ i) {
	  Gate* i_gate = gate->sorted_fanin(i);
	  if ( i_gate->fval() != kVal0 ) {
	    val = kVal1;
	    gate = i_gate;
	    break;
	  }
	}
      }
      else {
	for (ymuint i = 0; i < gate->nfi(); ++ i) {
	  Gate* i_gate = gate->sorted_fanin(i);
	  if ( !i_gate->chk_fj() ) {
	    val = kVal0;
	    gate = i_gate;
	    break;
	  }
	}
      }
      break;
      
    case kTgXor:
    case kTgXnor:
      for (ymuint i = 0; i < gate->nfi(); ++ i) {
	Gate* i_gate = gate->sorted_fanin(i);
	if ( !i_gate->chk_fj() ) {
	  val = i_gate->fval();
	  if ( val == kValX ) {
	    val = kVal1;
	  }
	  gate = i_gate;
	  break;
	}
      }
      break;
      
    default:
      assert_not_reached(__FILE__, __LINE__);
      break;
    }
  }
}

END_NONAMESPACE

bool
C_decision( Gate* gate,
	    Gate*& o_gate,
	    Val3& o_gval,
	    Val3& o_fval )
{
  gn.clear_mark();
  for ( ; ; ) {
    Val3 gval = gate->gval();
    Val3 fval = gate->fval();
    if ( gval == kValX || fval == kValX ) {
      // 値が決まっていなければ正当化しようがない．
      return false;
    }

    // gate の値が正当化されているか調べる．
    gate->chk_just();
    if ( !gate->chk_gj() ) {
      // 正常値が正当化されていない．
      backtrace_g(gate, gval, o_gate, o_gval);
      if ( !o_gate->chk_fcone() ||
	   o_gate->chk_dif() ||
	   o_gate->fval() != kValX ) {
	o_fval = kValX;
      }
      else {
	o_fval = o_gval;
      }
      return true;
    }

    if ( !gate->chk_fj() ) {
      // 故障値が正当化されていない．
      backtrace_f(gate, fval, o_gate, o_fval);
      if ( !o_gate->chk_fcone() ) {
	o_gval = o_fval;
	o_fval = kValX;
      }
      else if ( o_gate->chk_dif() ||
		o_gate->gval() != kValX ) {
	o_gval = kValX;
      }
      else {
	o_gval = o_fval;
      }
      return true;
    }

    // どちらの値も正当化されているのでファンアウトをたどる．
    ymuint no = gate->act_nfo();
    bool found = false;
    for (ymuint i = 0; i < no; ++ i) {
      Gate* o_gate = gate->act_fanout(i);
      if ( o_gate->chk_pg() ) {
	gate = o_gate;
	found = true;
	break;
      }
    }
    if ( !found ) {
      return false;
    }
  }
}

// @brief 正当化のチェックを行う．
void
Gate::chk_just()
{
  if ( chk_gj() && chk_fj() ) {
    // 両方の値が正当化されている．
    return;
  }
  
  if ( chk_mark() ) {
    // すでに処理済み
    return;
  }
  set_mark();

  Val3 gv = gval();
  Val3 fv = fval();

  if ( !chk_fcone() && chk_cfna(gv) ) {
    // 故障の影響を受けないところで CFNA なら正当化できる．
    sm.save_value(this);
    set_gj();
    set_fj();
    return;
  }

  bool gj_flag = false;
  bool fj_flag = false;

  if ( !chk_gj() ) {
    // 正常値が正当化されていない．
    if ( gv == o_val() ) {
      // 出力が制御値によって決まる値
      for (ymuint i = 0; i < nfi(); ++ i) {
	Gate* i_gate = sorted_fanin(i);
	if ( i_gate->gval() == c_val() ) {
	  // 制御値を持つのファンインを正当化できるか調べる．
	  i_gate->chk_just();
	  if ( i_gate->chk_gj() ) {
	    gj_flag = true;
	    break;
	  }
	}
      }
    }
    else if ( gv != kValX ) {
      // 出力が非制御値で決まる値
      gj_flag = true;
      for (ymuint i = 0; i < nfi(); ++ i) {
	Gate* i_gate = sorted_fanin(i);
	if ( i_gate->gval() == kValX ) {
	  // 一つでも未定の入力があれば即アウト
	  gj_flag = false;
	  break;
	}
	i_gate->chk_just();
	if ( !i_gate->chk_gj() ) {
	  // 正当化できない非制御値の入力があってもアウト
	  gj_flag = false;
	  break;
	}
      }
    }
  }
  if ( chk_fcone() ) {
    // 故障の影響をうける場合には故障値も別個に考える．
    if ( !chk_fj() ) {
      // 故障値が正当化されていない．
      if ( fv == o_val() ) {
	// 出力が制御値によって決まる値
	for (ymuint i = 0; i < nfi(); ++ i) {
	  Gate* i_gate = sorted_fanin(i);
	  if ( i_gate->fval() == c_val() ) {
	    // 制御値を持つファンインを正当化できるか調べる．
	    i_gate->chk_just();
	    if ( i_gate->chk_fj() ) {
	      fj_flag = true;
	      break;
	    }
	  }
	}
      }
      else if ( fv != kValX ) {
	// 出力が非制御値で決まる値
	fj_flag = true;
	for (ymuint i = 0; i < nfi(); ++ i) {
	  Gate* i_gate = sorted_fanin(i);
	  if ( i_gate->fval() == kValX ) {
	    // 未定の入力があれば即アウト
	    fj_flag = false;
	    break;
	  }
	  i_gate->chk_just();
	  if ( !i_gate->chk_fj() ) {
	    // 正当化できない入力があってもアウト
	    fj_flag = false;
	    break;
	  }
	}
      }
    }
  }
  else {
    fj_flag = gj_flag;
  }

  // gj_flag, fj_flag の値にしたがって印をつける．
  if ( gj_flag ) {
    sm.save_value(this);
    set_gj();
    if ( fj_flag ) {
      set_fj();
    }
  }
  else if ( fj_flag ) {
    sm.save_value(this);
    set_fj();
  }
}

END_NAMESPACE_YM_ATPG_DTPGCL
