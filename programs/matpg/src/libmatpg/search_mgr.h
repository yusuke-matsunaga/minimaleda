#ifndef H_MATPG_SEARCH_MGR
#define H_MATPG_SEARCH_MGR

/*
 *	matpg - Mat's Automatic Testpattern Generator
 *	10/5/91 by Mat.
 *	$Log: search_mgr.h,v $
 * Revision 2.2  1992/01/22  23:49:14  yusuke
 * no difference
 *
 * Revision 2.2  1992/01/22  23:49:14  yusuke
 * no difference
 *
 * Revision 2.1  91/12/23  23:12:15  yusuke
 * a slightly speed-up
 * 
 * Revision 2.0  91/12/21  18:56:39  yusuke
 * '91 Cristmas version
 * 
 * Revision 1.5  1991/10/05  08:18:18  yusuke
 * add Log and RCSid for RCS
 *
 */

#include <setjmp.h>
#include "gate.h"


BEGIN_NAMESPACE_YM_MATPG

struct value_stack
{
  gate_t* gate;
  ymulong state;
};

extern
void
set_request(gate_t* gate,
	    val3 gval,
	    val3 fval);

extern
void
opt_assign(gate_t* gate,
	   val3 gval,
	   val3 fval);

extern
void
mand_assign(gate_t* gate,
	    val3 gval,
	    val3 fval);

extern
bool
implication();

extern
bool
implication2();

extern
bool
dyn_imp(gate_t* gate,
	val3 gval,
	val3 fval);

extern
bool
back_track();

extern
void
one_more_chance();

extern
void
restore_value();

extern
void
push_DFmark();

/* get/set information */
extern
bool
is_aborted();

extern
bool
is_redundant();

extern
void
set_default_backtrack_limit();

extern
int
get_backtrack_limit();

extern
void
set_backtrack_limit(int i);

extern
int
get_backtrack_count();

extern
void
resize_stack();

extern
value_stack* search_mgr_stack;
extern
value_stack* search_mgr_stack_end;
extern
value_stack* search_mgr_ptr;
extern
int search_mgr_size;
extern
jmp_buf search_mgr_env;

inline
void
imp_fail()
{
  longjmp(search_mgr_env, 2);
}

inline
void
save_value(gate_t* gate)
{
  if (search_mgr_ptr == search_mgr_stack_end) {
    resize_stack();
  }
  search_mgr_ptr->gate = gate;
  search_mgr_ptr->state = gate->state;
  search_mgr_ptr ++;
}

END_NAMESPACE_YM_MATPG

#endif
