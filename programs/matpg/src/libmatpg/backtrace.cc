/*
 *	matpg - Mat's Automatic Testpattern Generator
 *	10/5/91 by Mat.
 *	$Log: backtrace.c,v $
 * Revision 2.5  1992/06/16  23:19:36  yusuke
 * add routines for OTHER_gate_t
 *
 * Revision 2.4  1992/01/31  00:20:11  yusuke
 * Febrary '92 version
 *
 * Revision 2.3  1991/12/28  16:58:12  yusuke
 * Final , Final revision
 *
 * Revision 2.3  1991/12/28  16:58:12  yusuke
 * Final , Final revision
 *
 * Revision 2.2  91/12/26  19:56:04  yusuke
 * Final revision of version 2
 * 
 * Revision 2.1  91/12/23  23:09:49  yusuke
 * a slightly speed-up
 * 
 * Revision 2.0  91/12/21  18:30:08  yusuke
 * '91 Cristmas version
 * 
 * Revision 1.5  1991/10/05  08:18:18  yusuke
 * add Log and RCSid for RCS
 *
 */

#include "matpg.h"
#include "gate.h"
#include "network.h"
#include "backtrace.h"
#include "search_mgr.h"


BEGIN_NAMESPACE_YM_MATPG

typedef bool (*p_chk_end_t)(gate_t* gate, val3 r_val);

static
bool
chk_end_D_g(gate_t* gate,
	    val3 r_val);

static
bool
chk_end_D_f(gate_t* gate,
	    val3 r_val);

static
bool
chk_end_PODEM_g(gate_t* gate,
		val3 r_val);

static
p_chk_end_t chk_end_g = chk_end_D_g;

static
p_chk_end_t chk_end_f = chk_end_D_f;

void
set_backtrace_mode( BT_MODE bt_mode )
{
  switch ( bt_mode ) {
  case BT_D:
    chk_end_g = chk_end_D_g;
    chk_end_f = chk_end_D_f;
    break;
    
  case BT_PODEM:
    chk_end_g = chk_end_PODEM_g;
    break;
  }
}

static
bool
chk_end_D_g( gate_t* gate,
	     val3 r_val )
{
  if ( gate->get_gval() == val_X ) {
    if ( !gate->chk_fcone() ||
	 gate->chk_dif() ||
	 gate->get_fval() != val_X ) {
      opt_assign( gate, r_val, val_X );
    }
    else {
      opt_assign(gate, r_val, r_val);
    }
    return true;
  }
  return false;
}

static
bool
chk_end_D_f( gate_t* gate,
	     val3 r_val )
{
  if ( gate->get_fval() == val_X ) {
    if ( !gate->chk_fcone() ) {
      opt_assign(gate, r_val, val_X);
    }
    else if ( gate->chk_dif() ||
	      gate->get_gval() != val_X ) {
      opt_assign(gate, val_X, r_val);
    }
    else {
      opt_assign(gate, r_val, r_val);
    }
    return true;
  }
  return false;
}

static
bool
chk_end_PODEM_g( gate_t* gate,
		 val3 r_val )
{
  if ( !gate->chk_fcone() ) {
    if ( r_val == val_0 ) {
      if ( gate->chk_cfna0() ) {
	opt_assign(gate, val_0, val_X);
	return true;
      }
    }
    else {
      if ( gate->chk_cfna1() ) {
	opt_assign(gate, val_1, val_X);
	return true;
      }
    }
  }
  return false;
}


bool
O_decision( gate_t* gate )
{
  for ( ; ; ) {
    int no = gate->get_act_no();
    if ( no == 0 ) {
      return false;
    }
    int i;
    for (i = 0; i < no; i ++) {
      gate_t* o_gate = gate->get_act_fogate(i);
      if ( o_gate->chk_pg() ) {
	gate = o_gate;
	break;
      }
    }
    if ( i >= no ) {
      FATAL("O_decision: Unexpected error!\n");
    }
    val3 gval = gate->get_gval();
    val3 fval = gate->get_fval();
    if ( gval == val_X ) {
      if ( fval == val_X ) {
	opt_assign(gate, val_0, val_X);
      }
      else {
	opt_assign(gate, neg3(fval), val_X);
      }
      return true;
    }
    else if ( fval == val_X ) {
      opt_assign(gate, val_X, neg3(gval));
      return true;
    }
    if ( gate->is_po() ) {
      // PO に到達していたらもう O_decision で割り当てる変数はない．
      return false;
    }
  }
}

bool
C_decision( gate_t* gate )
{
  gn_clr_mark();
  for ( ; ; ) {
    val3 gval = gate->get_gval();
    val3 fval = gate->get_fval();
    if ( gval == val_X ||
	 fval == val_X ) {
      // 値が決まっていなければ正当化しようがない．
      return false;
    }
    
    gate->chk_just();
    if ( !gate->chk_gj() ) {
      // 正常値が正当化されていない．
      gate->backtrace_g(gval);
      return true;
    }

    if ( !gate->chk_fj() ) {
      // 故障値が正当化されていない．
      gate->backtrace_f(fval);
      return true;
    }

    // どちらの値も正当化されているのでファンアウトをたどる．
    int no = gate->get_act_no();
    int i;
    for ( i = 0; i < no; i ++ ) {
      gate_t* o_gate = gate->get_act_fogate(i);
      if ( o_gate->chk_pg() ) {
	gate = o_gate;
	break;
      }
    }
    if ( i >= no ) {
      // すべてのファンアウトに故障伝搬の可能性がない．
      return false;
    }
  }
}

void
gate_t::backtrace_g(val3)
{
  FATAL("gate_t::backtrace_g: unexpected error!\n");
}

void
gate_t::backtrace_f(val3)
{
  FATAL("gate_t::backtrace_f: unexpected error!\n");
}

void
gate_t::chk_just()
{
  if ( chk_mark() ) {
    // すでに処理済み
    return;
  }
  set_mark();

  if ( chk_gj() && chk_fj() ) {
    // 両方の値が正当化されている．
    return;
  }

  val3 gval = get_gval();
  val3 fval = get_fval();

  if ( !chk_fcone() &&
       ((gval == val_0 && chk_cfna0()) ||
	(gval == val_1 && chk_cfna1())) ) {
    // 故障の影響を受けないところで CFNA なら正当化できる．
    save_value(this);
    set_gj();
    set_fj();
    return;
  }

  bool gj_flag = false;
  bool fj_flag = false;
  val3 c_val = get_c_val();
  val3 o_val = get_o_val();

  if ( !chk_gj() ) {
    // 正常値が正当化されていない．
    if ( gval == o_val ) {
      // 出力が制御値によって決まる値
      for (int i = 0; i < ni; ++ i) {
	gate_t* i_gate = get_sorted_figate(i);
	if ( i_gate->get_gval() == c_val ) {
	  // 制御値の入力を正当化できるか調べる．
	  i_gate->chk_just();
	  if ( i_gate->chk_gj() ) {
	    gj_flag = true;
	    break;
	  }
	}
      }
    }
    else if ( gval != val_X ) {
      // 出力が非制御値で決まる値
      gj_flag = true;
      for (int i = 0; i < ni; ++ i) {
	gate_t* i_gate = get_sorted_figate(i);
	if ( i_gate->get_gval() == val_X ) {
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
      if ( fval == o_val ) {
	// 出力が制御値によって決まる値
	for (int i = 0; i < ni; ++ i) {
	  gate_t* i_gate = get_sorted_figate(i);
	  if ( i_gate->get_fval() == c_val ) {
	    // 制御値の入力を正当化できるか調べる．
	    i_gate->chk_just();
	    if ( i_gate->chk_fj() ) {
	      fj_flag = true;
	      break;
	    }
	  }
	}
      }
      else if ( fval != val_X ) {
	// 出力が非制御値で決まる値
	fj_flag = true;
	for (int i = 0; i < ni; ++ i) {
	  gate_t* i_gate = get_sorted_figate(i);
	  if ( i_gate->get_fval() == val_X ) {
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
    save_value(this);
    set_gj();
    if ( fj_flag ) {
      set_fj();
    }
  }
  else if ( fj_flag ) {
    save_value(this);
    set_fj();
  }
}

void
BUF_gate_t::chk_just()
{
  if ( chk_mark() ) {
    return;
  }
  set_mark();

  if ( chk_gj() && chk_fj() ) {
    return;
  }
  if ( get_gval() == val_X && get_fval() == val_X ) {
    return;
  }

  gate_t* i_gate = get_figate(0);
  i_gate->chk_just();
  if ( i_gate->chk_gj() ) {
    save_value(this);
    set_gj();
    if ( i_gate->chk_fj() ) {
      set_fj();
    }
  }
  else if ( i_gate->chk_fj() ) {
    save_value(this);
    set_fj();
  }
}

void
BUF_gate_t::backtrace_g(val3 val)
{
  if ( !chk_end_g(this, val) ) {
    get_figate(0)->backtrace_g(val);
  }
}

void
BUF_gate_t::backtrace_f(val3 val)
{
  if ( !chk_end_f(this, val) ) {
    get_figate(0)->backtrace_f(val);
  }
}

void
NOT_gate_t::backtrace_g(val3 val)
{
  if ( !chk_end_g(this, val) ) {
    get_figate(0)->backtrace_g(neg3(val));
  }
}

void
NOT_gate_t::backtrace_f(val3 val)
{
  if ( !chk_end_f(this, val) ) {
    get_figate(0)->backtrace_f(neg3(val));
  }
}

void
SIMPLE_gate_t::backtrace_g(val3 r_val)
{
  if ( chk_end_g(this, r_val) ) {
    return;
  }
  
  val3 c_val = get_c_val();
  val3 nc_val = get_nc_val();
  val3 o_val = get_o_val();
  if ( r_val == o_val ) {
    for (int i = 0; i < ni; i ++) {
      gate_t* i_gate = get_sorted_figate(i);
      if ( i_gate->get_gval() != nc_val ) {
	i_gate->backtrace_g(c_val);
	return;
      }
    }
  }
  else {
    for (int i = 0; i < ni; i ++) {
      gate_t* i_gate = get_sorted_figate(i);
      if ( !i_gate->chk_gj() ) {
	i_gate->backtrace_g(nc_val);
	return;
      }
    }
  }
  FATAL("SIMPLE_gate_t::backtrace_g: Internal error!\n");
}

void
SIMPLE_gate_t::backtrace_f(val3 r_val)
{
  if ( chk_end_f(this, r_val) ) {
    return;
  }
  
  val3 c_val = get_c_val();
  val3 nc_val = get_nc_val();
  val3 o_val = get_o_val();
  if (r_val == o_val) {
    for (int i = 0; i < ni; i ++) {
      gate_t* i_gate = get_sorted_figate(i);
      if ( i_gate->get_fval() != nc_val ) {
	i_gate->backtrace_f(c_val);
	return;
      }
    }
  }
  else {
    for (int i = 0; i < ni; i ++) {
      gate_t* i_gate = get_sorted_figate(i);
      if ( !i_gate->chk_fj() ) {
	i_gate->backtrace_f(nc_val);
	return;
      }
    }
  }
  FATAL("SIMPLE_gate_t::backtrace_f: Internal error!\n");
}

void
XOR_gate_t::backtrace_g(val3 r_val)
{
  if ( chk_end_g(this, r_val) ) {
    return;
  }
  
  for (int i = 0; i < ni; i ++) {
    gate_t* i_gate = get_sorted_figate(i);
    if ( !i_gate->chk_gj() ) {
      val3 i_val = i_gate->get_gval();
      if ( i_val == val_X ) {
	i_val = val_1;
      }
      i_gate->backtrace_g(i_val);
      return;
    }
  }
  FATAL("XOR_gate_t::backtrace_g: Internal error!\n");
}

void
XOR_gate_t::backtrace_f(val3 r_val)
{
  if ( chk_end_f(this, r_val) ) {
    return;
  }

  for (int i = 0; i < ni; i ++) {
    gate_t* i_gate = get_sorted_figate(i);
    if ( !i_gate->chk_fj() ) {
      val3 i_val = i_gate->get_fval();
      if ( i_val == val_X ) {
	i_val = val_1;
      }
      i_gate->backtrace_f(i_val);
      return;
    }
  }
  FATAL("XOR_gate_t::backtrace_f: Internal error!\n");
}

END_NAMESPACE_YM_MATPG
