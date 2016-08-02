/*
 *	matpg - Mat's Automatic Testpattern Generator
 *	10/5/91 by Mat.
 *	$Log: learn.c,v $
 * Revision 2.3  1992/01/22  23:49:14  yusuke
 * no difference
 *
 * Revision 2.3  1992/01/22  23:49:14  yusuke
 * no difference
 *
 * Revision 2.2  91/12/28  16:58:57  yusuke
 * Final , Final revision
 * 
 * Revision 2.1  91/12/23  23:11:34  yusuke
 * a slightly speed-up
 * 
 * Revision 2.0  91/12/21  18:52:36  yusuke
 * '91 Cristmas version
 * 
 * Revision 1.6  1991/10/17  02:55:53  yusuke
 * made a new function eq_imp()
 *
 * Revision 1.5  1991/10/05  08:18:18  yusuke
 * add Log and RCSid for RCS
 *
 */

#include <setjmp.h>
#include "matpg.h"
#include "gate.h"
#include "network.h"
#include "learn.h"
#include "used_gate.h"
#include "fault.h"
#include "maqueue.h"


BEGIN_NAMESPACE_YM_MATPG


static
bool learn_flag = false;
static
bool learn_flag0 = false;

void
use_learning()
{
  learn_flag = true;
}

void
dont_use_learning()
{
  learn_flag = false;
}

void
push_learn_flag()
{
  learn_flag0 = learn_flag;
}

void
pop_learn_flag()
{
  learn_flag = learn_flag0;
}

void
forget_learning()
{
  learn_flag = false;
  for (int i = gn_get_ngate(); -- i >= 0; ) {
    gate_t* gate = gn_get_gate(i);
    gate_list_t* list;
    gate_list_t* next;
    for (list = gate->imp0; list; list = next) {
      next = list->next;
      delete list;
    }
    for (list = gate->imp1; list; list = next) {
      next = list->next;
      delete list;
    }
    gate->imp0 = gate->imp1 = NULL;
  }
}

void
gate_t::gl_imp(val3 val)
{
  if (learn_flag == true) {
    gate_list_t* nlist;
    if (val == val_0) {
      nlist = imp0;
    }
    else {
      nlist = imp1;
    }
    for ( ; nlist; nlist = nlist->next) {
      gate_t* imp_gate = nlist->gate;
      val3 val = nlist->val;
      if (imp_gate->is_active() == true && imp_gate->get_gval() != val) {
	if (imp_gate->chk_fcone() == true) {
	  imp_gate->bwd_imp_g(val, NULL);
	}
	else {
	  imp_gate->bwd_imp_h(val, NULL);
	}
      }
    }
  }
}

static
jmp_buf env;
static
gate_t* source_gate;
static
val3 source_val;
static
int imp_num;

static
void
mark_tfi_tfo(gate_t* gate);

static
void
record_implication(gate_t* gate,
		   val3 val);


void
learning()
{
  StopWatch timer;
  timer.start();
  imp_num = 0;
  lvlq_clr();
  gn_clr_mark();
  for (int i = 0; i < fault_u_num(); i ++) {
    mark_tfi_tfo(fault_get(i)->get_input_gate());
  }
  while ( (source_gate = lvlq_get_from_bottom()) ) {
    if (source_gate->chk_learned() == true) {
      continue;
    }
    source_gate->set_learned();
    if (source_gate->get_ni() <= 1) {
      continue;
    }
    if (setjmp(env) == 0) {
      source_val = val_0;
      source_gate->l_bwd_imp(val_0, NULL);
    }
    else {
      const1_add(source_gate);
    }
    used_gate_clr();
    if (setjmp(env) == 0) {
      source_val = val_1;
      source_gate->l_bwd_imp(val_1, NULL);
    }
    else {
      const0_add(source_gate);
    }
    used_gate_clr();
  }
  if (log_flag == true) {
    timer.stop();
    USTime time = timer.time();
    double utime = time.usr_time();
    double stime = time.sys_time();
    fprintf(log_fp, "********* learning *********\n");
    fprintf(log_fp, "%10d : # of learned implications.\n", imp_num);
    fprintf(log_fp, "%10.2fu %10.2fs : CPU time\n", utime, stime);
  }
}

static
void
mark_tfi(gate_t* gate)
{
  if (gate->chk_mark() == true) {
    return;
  }
  gate->set_mark();
  lvlq_put(gate);
  for (int i = gate->get_ni(); -- i >= 0; ) {
    mark_tfi(gate->get_figate(i));
  }
}

static
void
mark_tfi_tfo(gate_t* gate)
{
  if (gate->chk_mark() == false) {
    gate->set_mark();
    lvlq_put(gate);
    int i;
    for (i = gate->get_no(); -- i >= 0; ) {
      mark_tfi_tfo(gate->get_fogate(i));
    }
    for (i = gate->get_ni(); -- i >= 0; ) {
      mark_tfi(gate->get_figate(i));
    }
  }
}

static
void
l_gl_imp(gate_t* gate,
	 val3 val)
{
  gate_list_t* nlist;
  if (val == val_0) {
    nlist = gate->imp0;
  }
  else {
    nlist = gate->imp1;
  }
  for ( ; nlist; nlist = nlist->next) {
    gate_t* imp_gate = nlist->gate;
    val3 val = nlist->val;
    if (imp_gate->get_gval() != val) {
      imp_gate->l_bwd_imp(val, NULL);
    }
  }
}

void
gate_t::l_fwd_imp(val3)
{
  FATAL("gate_t::l_fwd_imp() error!\n");
}

void
gate_t::l_bwd_imp(val3,
		  gate_t*)
{
  FATAL("gate_t::l_bwd_imp() error!\n");
}

void
C0_gate_t::l_bwd_imp(val3 val,
		     gate_t* from)
{
  if (val != val_0) {
    longjmp(env, 1);
  }
  if (get_gval() != val_X) {
    return;
  }
  used_gate_add(this);
  set_gval(val_0);
  for (int i = get_no(); -- i >= 0; ) {
    gate_t* o_gate = get_fogate(i);
    if (o_gate != from) {
      o_gate->l_fwd_imp(val);
    }
  }
}

void
C1_gate_t::l_bwd_imp(val3 val,
		     gate_t* from)
{
  if (val != val_1) {
    longjmp(env, 1);
  }
  if (get_gval() != val_X) {
    return;
  }
  used_gate_add(this);
  set_gval(val_1);
  for (int i = get_no(); -- i >= 0; ) {
    gate_t* o_gate = get_fogate(i);
    if (o_gate != from) {
      o_gate->l_fwd_imp(val);
    }
  }
}

void
BUF_gate_t::l_fwd_imp(val3 val)
{
  if (get_gtype() == kTgNot) {
    val = neg3(val);
  }
  if (get_gval() == val_X) {
    used_gate_add(this);
    set_gval(val);
    for (int i = get_no(); -- i >= 0; ) {
      get_fogate(i)->l_fwd_imp(val);
    }
  }
  else if (val != get_gval()) {
    longjmp(env, 1);
  }
}

void
BUF_gate_t::l_bwd_imp(val3 val,
		      gate_t* from)
{
  if (get_gval() == val_X) {
    used_gate_add(this);
    set_gval(val);
    for (int i = get_no(); -- i >= 0; ) {
      gate_t* o_gate = get_fogate(i);
      if (o_gate != from) {
	o_gate->l_fwd_imp(val);
      }
    }
    if (get_gtype() == kTgNot) {
      val = neg3(val);
    }
    get_figate(0)->l_bwd_imp(val, this);
  }
  else if (val != get_gval()) {
    longjmp(env, 1);
  }
}

void
SIMPLE_gate_t::l_fwd_imp(val3 val)
{
  if (val == c_val) {
    if (get_gval() == val_X) {
      used_gate_add(this);
      set_gval(o_val);
      for (int i = get_no(); -- i >= 0; ) {
	get_fogate(i)->l_fwd_imp(o_val);
      }
      l_gl_imp(this, o_val);
    }
    else if (get_gval() != o_val) {
      longjmp(env, 1);
    }
  }
  else {
    val3 nc_val = neg3(c_val);
    val3 no_val = neg3(o_val);
    if (get_gval() == val_X) {
      int i;
      for (i = get_ni(); -- i >= 0; ) {
	if (get_figate(i)->get_gval() != nc_val) {
	  break;
	}
      }
      if (i < 0) {
	used_gate_add(this);
	set_gval(no_val);
	record_implication(this, no_val);
	for (i = get_no(); -- i >= 0; ) {
	  get_fogate(i)->l_fwd_imp(no_val);
	}
	l_gl_imp(this, no_val);
      }
    }
    else if (get_gval() == o_val) {
      int u_num = 0;
      gate_t* last = NULL;
      int i;
      for (i = get_ni(); -- i >= 0; ) {
	gate_t* i_gate = get_figate(i);
	val3 i_val = i_gate->get_gval();
	if (i_val == val_X) {
	  u_num ++;
	  last = i_gate;
	}
	else if (i_val == c_val) {
	  break;
	}
      }
      if (i < 0) {
	if (u_num == 0) {
	  longjmp(env, 1);
	}
	if (u_num == 1) {
	  last->l_bwd_imp(c_val, this);
	}
      }
    }
  }
}

void
SIMPLE_gate_t::l_bwd_imp(val3 val,
			 gate_t* from)
{
  if (get_gval() == val_X) {
    used_gate_add(this);
    set_gval(val);
    if (val != o_val) {
      record_implication(this, val);
    }
    for (int i = get_no(); -- i >= 0; ) {
      gate_t* o_gate = get_fogate(i);
      if (o_gate != from) {
	o_gate->l_fwd_imp(val);
      }
    }
    l_gl_imp(this, val);
    if (is_pi() == true || is_basis() == true) {
      return;
    }
    if (val == o_val) {
      int u_num = 0;
      gate_t* last = NULL;
      int i;
      for (i = get_ni(); -- i >= 0; ) {
	gate_t* i_gate = get_figate(i);
	val3 i_val = i_gate->get_gval();
	if (i_val == val_X) {
	  u_num ++;
	  last = i_gate;
	}
	else if (i_val == c_val) {
	  break;
	}
      }
      if (i < 0) {
	if (u_num == 0) {
	  longjmp(env, 1);
	}
	if (u_num == 1) {
	  last->l_bwd_imp(c_val, this);
	}
      }
    }
    else {
      val3 nc_val = neg3(c_val);
      for (int i = get_ni(); -- i >= 0; ) {
	get_figate(i)->l_bwd_imp(nc_val, this);
      }
    }
  }
  else if (get_gval() != val) {
    longjmp(env, 1);
  }
}

void
XOR_gate_t::l_fwd_imp(val3 val)
{
  if (get_gtype() == kTgXor) {
    val = val_0;
  }
  else {
    val = val_1;
  }
  int u_num = 0;
  gate_t* last = NULL;
  for (int i = get_ni(); -- i >= 0; ) {
    gate_t* i_gate = get_figate(i);
    val3 i_val = i_gate->get_gval();
    if (i_val == val_X) {
      u_num ++;
      last = i_gate;
    }
    else if (i_val == val_1) {
      val = neg3(val);
    }
  }
  val3 gval = get_gval();
  if (u_num == 0) {
    if (gval == val_X) {
      used_gate_add(this);
      set_gval(val);
      record_implication(this, val);
      for (int i = get_no(); -- i >= 0; ) {
	get_fogate(i)->l_fwd_imp(val);
      }
      l_gl_imp(this, val);
    }
    else if (gval != val) {
      longjmp(env, 1);
    }
  }
  else if (u_num == 1 && gval != val_X) {
    val3 i_val = val_0;
    if (gval != val) {
      i_val = val_1;
    }
    last->l_bwd_imp(i_val, this);
  }
}

void
XOR_gate_t::l_bwd_imp(val3 val,
		      gate_t* from)
{
  if (get_gval() == val_X) {
    used_gate_add(this);
    set_gval(val);
    record_implication(this, val);
    for (int i = get_no(); -- i >= 0; ) {
      gate_t* o_gate = get_fogate(i);
      if (o_gate != from) {
	o_gate->l_fwd_imp(val);
      }
    }
    l_gl_imp(this, val);
    if (is_basis() == true) {
      return;
    }
    int u_num = 0;
    gate_t* last = NULL;
    if (get_gtype() == kTgXnor) {
      val = neg3(val);
    }
    for (int i = get_ni(); -- i >= 0; ) {
      gate_t* i_gate = get_figate(i);
      val3 i_val = i_gate->get_gval();
      if (i_val == val_X) {
	u_num ++;
	last = i_gate;
      }
      else if (i_val == val_1) {
	val = neg3(val);
      }
    }
    if (u_num == 0) {
      if (val == val_1) {
	longjmp(env, 1);
      }
    }
    else if (u_num == 1) {
      last->l_bwd_imp(val, this);
    }
  }
  else if (get_gval() != val) {
    longjmp(env, 1);
  }
}

static
void
record_implication(gate_t* gate,
		   val3 val)
{
  if (gate == source_gate) {
    return;
  }
  gate_t* temp = source_gate->eq_gate;
  if (temp) {
    do {
      if (temp == gate) {
	return;
      }
      temp = temp->eq_gate;
    } while (temp != source_gate);
  }
  gate_list_t* list = new gate_list_t;
  list->gate = source_gate;
  list->val = neg3(source_val);
  if (val == val_0) {
    list->next = gate->imp1;
    gate->imp1 = list;
  }
  else {
    list->next = gate->imp0;
    gate->imp0 = list;
  }
  imp_num ++;
}

END_NAMESPACE_YM_MATPG
