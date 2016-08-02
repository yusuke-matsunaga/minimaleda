/*
 *	matpg - Mat's Automatic Testpattern Generator
 *	10/5/91 by Mat.
 *	$Log: search_mgr.c,v $
 * Revision 2.3  1992/01/22  23:49:14  yusuke
 * no difference
 *
 * Revision 2.3  1992/01/22  23:49:14  yusuke
 * no difference
 *
 * Revision 2.2  91/12/28  16:59:17  yusuke
 * Final , Final revision
 * 
 * Revision 2.1  91/12/23  23:12:12  yusuke
 * a slightly speed-up
 * 
 * Revision 2.0  91/12/21  18:56:24  yusuke
 * '91 Cristmas version
 * 
 * Revision 1.5  1991/10/05  08:18:18  yusuke
 * add Log and RCSid for RCS
 *
 */

#include "matpg.h"
#include "gate.h"
#include "search_mgr.h"
#include "maqueue.h"
#include "pgraph.h"


BEGIN_NAMESPACE_YM_MATPG


static
const int DEFAULT_BACKTRACK_LIMIT = 16;
static
const int INIT_SIZE = 64;

jmp_buf search_mgr_env;

static
int bt_num0 = 0;

static
int bt_num = 0;

static
int bt_lim = DEFAULT_BACKTRACK_LIMIT;

static
bool abort_flag = false;

static
bool red_flag = false;

static
gate_t* cur_gate = NULL;

static
val3 cur_gval;
static
val3 cur_fval;

int search_mgr_size = INIT_SIZE;
value_stack* search_mgr_stack = NULL;
value_stack* search_mgr_stack_end = NULL;
value_stack* search_mgr_ptr = NULL;

inline
void
set_next(gate_t* gate,
	 val3 gval,
	 val3 fval)
{
  cur_gate = gate;
  cur_gval = gval;
  cur_fval = fval;
}

void
resize_stack()
{
  if ( search_mgr_stack == NULL ) {
    search_mgr_stack = new value_stack[search_mgr_size];
    search_mgr_stack_end = search_mgr_stack + search_mgr_size;
    search_mgr_ptr = search_mgr_stack;
  }
  else {
    value_stack* old_stack = search_mgr_stack;
    value_stack* old_stack_ptr = search_mgr_ptr;
    search_mgr_size <<= 1;
    search_mgr_stack = new value_stack[search_mgr_size];
    search_mgr_stack_end = search_mgr_stack + search_mgr_size;
    value_stack* ptr = old_stack;
    search_mgr_ptr = search_mgr_stack;
    for ( ; ptr != old_stack_ptr; ptr ++, search_mgr_ptr ++) {
      search_mgr_ptr->gate = ptr->gate;
      search_mgr_ptr->state = ptr->state;
    }
    delete [] old_stack;
  }
}

inline
void
push_alt(gate_t* gate,
	 val3 gval,
	 val3 fval)
{
  if (search_mgr_ptr == search_mgr_stack_end) {
    resize_stack();
  }
  search_mgr_ptr->gate = gate;
  search_mgr_ptr->state = gval | (fval << F_SHIFT);
  search_mgr_ptr ++;
  if (search_mgr_ptr == search_mgr_stack_end) {
    resize_stack();
  }
  search_mgr_ptr->gate = NULL;	/* end-mark */
  search_mgr_ptr ++;
}

bool
is_aborted()
{
  return abort_flag;
}

void
one_more_chance()
{
  abort_flag = false;
  bt_num0 += bt_num;
  bt_num = 0;
}

bool
is_redundant()
{
  return red_flag;
}

void
set_default_backtrack_limit()
{
  bt_lim = DEFAULT_BACKTRACK_LIMIT;
}

int
get_backtrack_limit()
{
  return bt_lim;
}

void
set_backtrack_limit(int i)
{
  bt_lim = i;
}

int
get_backtrack_count()
{
  return bt_num0 + bt_num;
}

void
set_request(gate_t* gate,
	    val3 gval,
	    val3 fval)
{
  if (get_verbose_level() > 1) {
    fprintf(ver_fp, "REQUEST: %s (%s)\n", gate->get_name(),
	    value_name(gval, fval));
  }
  search_mgr_ptr = search_mgr_stack;
  bt_num0 = 0;
  bt_num = 0;
  abort_flag = false;
  red_flag = false;
  set_next(gate, gval, fval);
}

void
opt_assign(gate_t* gate,
	   val3 gval,
	   val3 fval)
{
  if (get_verbose_level() > 1) {
    fprintf(ver_fp, "OPTIONAL_ASSIGN: %s (%s)\n", gate->get_name(),
	    value_name(gval, fval));
  }
  set_next(gate, gval, fval);
  push_alt(gate, neg3(gval), neg3(fval));
}

void
mand_assign(gate_t* gate,
	    val3 gval,
	    val3 fval)
{
  if (get_verbose_level() > 1) {
    fprintf(ver_fp, "MANDATORY_ASSIGN: %s (%s)\n", gate->get_name(),
	    value_name(gval, fval));
  }
  set_next(gate, gval, fval);
}

bool
implication()
{
  if (cur_gate == NULL) {
    fprintf(stderr, "NULL implication\n");
    exit(1);
  }
  if (setjmp(search_mgr_env) != 0) {
    cur_gate = NULL;
    return false;
  }
  else {
    pgraph_clr_indif();
    do {
      if (cur_gate->chk_fcone() == true) {
	if (cur_gval != val_X) {
	  cur_gate->bwd_imp_g(cur_gval, NULL);
	}
	if (cur_fval != val_X) {
	  cur_gate->bwd_imp_f(cur_fval, NULL);
	}
      }
      else {
	if (cur_gval != val_X) {
	  cur_gate->bwd_imp_h(cur_gval, NULL);
	}
      }
    } while (maq_get() == true);
    cur_gate = NULL;
    return pgraph_update();
  }
}

bool
implication2()
{
  if (cur_gate != NULL) {
    if (setjmp(search_mgr_env) != 0) {
      cur_gate = NULL;
      return false;
    }
    else {
      do {
	if (cur_gval != val_X) {
	  cur_gate->bwd_imp_h(cur_gval, NULL);
	}
      } while (maq_get() == true);
      cur_gate = NULL;
    }
  }
  return true;
}

bool
dyn_imp(gate_t* gate,
	val3 gval,
	val3 fval)
{
  set_next(gate, gval, fval);
  if (search_mgr_ptr == search_mgr_stack_end) {
    resize_stack();
  }
  search_mgr_ptr->gate = NULL;	/* end-mark */
  search_mgr_ptr ++;
  bool result = implication();
  for ( ; ; ) {
    gate_t* gate = (-- search_mgr_ptr)->gate;
    if ( gate ) {
      gate->state = search_mgr_ptr->state;
    }
    else {
      break;
    }
  }
  set_next(NULL, val_X, val_X);
  return result;
}

bool
back_track()
{
  if (get_verbose_level() > 1) {
    fprintf(ver_fp, "BACK_TRACK\n");
  }
  maq_clr();
  while (search_mgr_ptr -- != search_mgr_stack) {
    if (search_mgr_ptr->gate == NULL) {	/* delimiter */
      value_stack* p_vs0 = -- search_mgr_ptr;
      set_next(p_vs0->gate, (val3(p_vs0->state & GVAL3_MASK)),
	       val3((p_vs0->state & FVAL3_MASK) >> F_SHIFT));
      if (++ bt_num >= bt_lim) {
	abort_flag = true;
	return false;	/* aborted fault */
      }
      return true;
    }
    else {
      search_mgr_ptr->gate->state = search_mgr_ptr->state;
    }
  }
  search_mgr_ptr ++;
  red_flag = true;
  return false;	/* redundant fault */
}

void
restore_value()
{
  while (search_mgr_ptr -- != search_mgr_stack) {
    if (search_mgr_ptr->gate != NULL) {
      search_mgr_ptr->gate->state = 0L;
    }
  }
  search_mgr_ptr ++;
}

END_NAMESPACE_YM_MATPG
