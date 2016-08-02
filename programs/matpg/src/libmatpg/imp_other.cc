/*
 *	matpg - Mat's Automatic Testpattern Generator
 *	10/5/91 by Mat.
 *	$Log: imp_other.c,v $
 * Revision 2.4  1992/01/22  23:49:14  yusuke
 * no difference
 *
 * Revision 2.4  1992/01/22  23:49:14  yusuke
 * no difference
 *
 * Revision 2.3  1991/12/28  16:58:52  yusuke
 * Final , Final revision
 *
 * Revision 2.2  91/12/24  14:40:48  yusuke
 * 
 * a little bit modification
 * 
 * Revision 2.1  91/12/23  23:11:24  yusuke
 * a slightly speed-up
 * 
 * Revision 2.0  91/12/21  18:51:52  yusuke
 * '91 Cristmas version
 * 
 * Revision 1.6  1991/10/17  02:55:53  yusuke
 * made a new function eq_imp()
 *
 * Revision 1.5  1991/10/05  08:18:18  yusuke
 * add Log and RCSid for RCS
 *
 */

#include "gate.h"
#include "search_mgr.h"
#include "learn.h"
#include "pgraph.h"
#include "cfna.h"


BEGIN_NAMESPACE_YM_MATPG


/* dummy */
void
gate_t::fwd_imp_h(val3)
{
  FATAL("gate_t::fwd_imp_h never occur!\n");
}

void
gate_t::fwd_imp_g(val3)
{
  FATAL("gate_t::fwd_imp_g never occur!\n");
}

void
gate_t::fwd_imp_f(val3)
{
  FATAL("gate_t::fwd_imp_f never occur!\n");
}

void
gate_t::bwd_imp_h(val3,
		  gate_t*)
{
  FATAL("gate_t::bwd_imp_h never occur!\n");
}

void
gate_t::bwd_imp_g(val3,
		  gate_t*)
{
  FATAL("gate_t::bwd_imp_g never occur!\n");
}

void
gate_t::bwd_imp_f(val3,
		  gate_t*)
{
  FATAL("gate_t::bwd_imp_f never occur!\n");
}

void
gate_t::prop_h(val3 val)
{
#ifdef DEBUG
  fprintf(stderr, "set_hval(val_%c)\n", value_name1(val));
#endif
  save_value(this);
  set_hval(val);
  net_t* ptr = act_outputs;
  net_t* end = act_outputs + act_no;
  while (ptr != end) {
    gate_t* o_gate = (ptr ++)->gate;
    if (o_gate->chk_fcone() == true) {
      o_gate->fwd_imp_g(val);
      o_gate->fwd_imp_f(val);
    }
    else {
      o_gate->fwd_imp_h(val);
    }
  }
  gl_imp(val);
}

void
gate_t::prop_g(val3 val)
{
#ifdef DEBUG
  fprintf(stderr, "set_gval(val_%c)\n", value_name1(val));
#endif
  save_value(this);
  set_gval(val);
  if (get_fval() == val) {
#ifdef DEBUG
    fprintf(stderr, "became same value\n");
#endif
    pgraph_add_indif(this);
  }
  net_t* ptr = act_outputs;
  net_t* end = act_outputs + act_no;
  while (ptr != end) {
    (ptr ++)->gate->fwd_imp_g(val);
  }
  gl_imp(val);
}

void
gate_t::prop_f(val3 val)
{
#ifdef DEBUG
  fprintf(stderr, "set_fval(val_%c)\n", value_name1(val));
#endif
  save_value(this);
  set_fval(val);
  if (get_gval() == val) {
#ifdef DEBUG
    fprintf(stderr, "became same value\n");
#endif
    pgraph_add_indif(this);
  }
  net_t* ptr = act_outputs;
  net_t* end = act_outputs + act_no;
  while (ptr != end) {
    (ptr ++)->gate->fwd_imp_f(val);
  }
}

void
gate_t::refl_h(val3 val,
	       gate_t* from)
{
#ifdef DEBUG
  fprintf(stderr, "set_hval(val_%c)\n", value_name1(val));
#endif
  save_value(this);
  set_hval(val);
  net_t* ptr = act_outputs;
  net_t* end = act_outputs + act_no;
  while (ptr != end) {
    gate_t* o_gate = (ptr ++)->gate;
    if (o_gate != from) {
      if (o_gate->chk_fcone() == true) {
	o_gate->fwd_imp_g(val);
	o_gate->fwd_imp_f(val);
      }
      else {
	o_gate->fwd_imp_h(val);
      }
    }
  }
  gl_imp(val);
}

void
gate_t::refl_g(val3 val,
	       gate_t* from)
{
#ifdef DEBUG
  fprintf(stderr, "set_gval(val_%c)\n", value_name1(val));
#endif
  save_value(this);
  set_gval(val);
  if (get_fval() == val) {
    pgraph_add_indif(this);
  }
  net_t* ptr = act_outputs;
  net_t* end = act_outputs + act_no;
  while (ptr != end) {
    gate_t* o_gate = (ptr ++)->gate;
    if (o_gate != from) {
      o_gate->fwd_imp_g(val);
    }
  }
  gl_imp(val);
}

void
gate_t::refl_f(val3 val,
	       gate_t* from)
{
#ifdef DEBUG
  fprintf(stderr, "set_fval(val_%c)\n", value_name1(val));
#endif
  save_value(this);
  set_fval(val);
  if (get_gval() == val) {
    pgraph_add_indif(this);
  }
  net_t* ptr = act_outputs;
  net_t* end = act_outputs + act_no;
  while (ptr != end) {
    gate_t* o_gate = (ptr ++)->gate;
    if (o_gate != from) {
      o_gate->fwd_imp_f(val);
    }
  }
}

void
gate_t::eq_imp(val3 val)
{
  gate_t* node1;
  if ((node1 = eq_gate) != NULL) {
    for ( ; node1 != this; node1 = node1->eq_gate) {
      if (node1->is_active() == true && node1->get_gval() != val) {
	if (node1->chk_fcone() == true) {
	  node1->bwd_imp_g(val, NULL);
	}
	else {
	  node1->bwd_imp_h(val, NULL);
	}
      }
    }
  }
}

/* C0_gate_t */
void
C0_gate_t::bwd_imp_h(val3 val,
		     gate_t* from)
{
  if (val == val_1) {
#ifdef DEBUG
    fprintf(stderr, "try to assign val_1 to Const_0\n");
#endif
    imp_fail();
  }
  if (get_gval() == val_X) {
    set_hjval(val_0);
    if (from != NULL && from->chk_fcone() == true) {
      from = NULL;
    }
    for ( int i = get_act_no(); -- i >= 0; ) {
      gate_t* o_gate = get_act_fogate(i);
      if (o_gate != from) {
	if (o_gate->chk_fcone() == true) {
	  o_gate->fwd_imp_g(val_0);
	  o_gate->fwd_imp_f(val_0);
	}
	else {
	  o_gate->fwd_imp_h(val_0);
	}
      }
    }
  }
}

/* C1_gate_t */
void
C1_gate_t::bwd_imp_h(val3 val,
		     gate_t* from)
{
  if (val == val_0) {
#ifdef DEBUG
    fprintf(stderr, "try to assign val_0 to Const_1\n");
#endif
    imp_fail();
  }
  if (get_gval() == val_X) {
    set_hjval(val_1);
    if (from != NULL && from->chk_fcone() == true) {
      from = NULL;
    }
    for ( int i = get_act_no(); -- i >= 0; ) {
      gate_t* o_gate = get_act_fogate(i);
      if (o_gate != from) {
	if (o_gate->chk_fcone() == true) {
	  o_gate->fwd_imp_g(val_1);
	  o_gate->fwd_imp_f(val_1);
	}
	else {
	  o_gate->fwd_imp_h(val_1);
	}
      }
    }
  }
}

END_NAMESPACE_YM_MATPG
