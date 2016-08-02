/*
 *	matpg - Mat's Automatic Testpattern Generator
 *	10/5/91 by Mat.
 *	$Log: imp_xor.c,v $
 * Revision 2.3  1992/01/31  00:20:11  yusuke
 * Febrary '92 version
 *
 * Revision 2.2  1991/12/28  16:58:56  yusuke
 * Final , Final revision
 *
 * Revision 2.2  1991/12/28  16:58:56  yusuke
 * Final , Final revision
 *
 * Revision 2.1  91/12/23  23:11:31  yusuke
 * a slightly speed-up
 * 
 * Revision 2.0  91/12/21  18:52:26  yusuke
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


/* XOR_gate_t */
void
XOR_gate_t::fwd_imp_h(val3 i_val)
{
#ifdef DEBUG
  fprintf(stderr, "%s::fwd_imp_h(val_%c)\n", get_name(), value_name1(i_val));
#endif

  val3 xval = (get_gtype() == kTgXor) ? val_0 : val_1;
  val3 val = get_gval();
  if (val == val_X) {
    int i;
    for (i = get_ni(); -- i >= 0; ) {
      val3 i_val = get_figate(i)->get_gval();
      if (i_val == val_X) {
	break;
      }
      if (i_val == val_1) {
	xval = neg3(xval);
      }
    }
    if (i < 0) {
      prop_h(xval);
    }
  }
  else {
    if (val == val_1) {
      xval = neg3(xval);
    }
    int ucount = 0;
    gate_t* last = NULL;
    for (int i = get_ni(); -- i >= 0; ) {
      gate_t* i_gate = get_figate(i);
      val3 i_val = i_gate->get_gval();
      if (i_val == val_X) {
	ucount ++;
	last = i_gate;
      }
      else if (i_val == val_1) {
	xval = neg3(xval);
      }
    }
    if (ucount == 0) {
      if (xval == val_1) {
#ifdef DEBUG
	fprintf(stderr, "unable to justify\n");
#endif
	imp_fail();
      }
    }
    else if (ucount == 1) {
      last->bwd_imp_h(xval, this);
    }
  }

#ifdef DEBUG
  fprintf(stderr, "%s::fwd_imp_h end\n", get_name());
#endif
}

void
XOR_gate_t::fwd_imp_g(val3 i_val)
{
#ifdef DEBUG
  fprintf(stderr, "%s::fwd_imp_g(val_%c)\n", get_name(), value_name1(i_val));
#endif

  val3 xval = (get_gtype() == kTgXor) ? val_0 : val_1;
  val3 val = get_gval();
  if (val == val_X) {
    int i;
    for (i = get_ni(); -- i >= 0; ) {
      val3 i_val = get_figate(i)->get_gval();
      if (i_val == val_X) {
	break;
      }
      if (i_val == val_1) {
	xval = neg3(xval);
      }
    }
    if (i < 0) {
      prop_g(xval);
    }
  }
  else {
    if (val == val_1) {
      xval = neg3(xval);
    }
    int ucount = 0;
    gate_t* last = NULL;
    for (int i = get_ni(); -- i >= 0; ) {
      gate_t* i_gate = get_figate(i);
      val3 i_val = i_gate->get_gval();
      if (i_val == val_X) {
	ucount ++;
	last = i_gate;
      }
      else if (i_val == val_1) {
	xval = neg3(xval);
      }
    }
    if (ucount == 0) {
      if (xval == val_1) {
#ifdef DEBUG
	fprintf(stderr, "unable to justify\n");
#endif
	imp_fail();
      }
    }
    else if (ucount == 1) {
      if (last->chk_fcone() == true) {
	last->bwd_imp_g(xval, this);
      }
      else {
	last->bwd_imp_h(xval, NULL);
      }
    }
  }

#ifdef DEBUG
  fprintf(stderr, "%s::fwd_imp_g end\n", get_name());
#endif
}

void
XOR_gate_t::fwd_imp_f(val3 i_val)
{
#ifdef DEBUG
  fprintf(stderr, "%s::fwd_imp_f(val_%c)\n", get_name(), value_name1(i_val));
#endif

  val3 xval = (get_gtype() == kTgXor) ? val_0 : val_1;
  val3 val = get_fval();
  if (val == val_X) {
    int i;
    for (i = get_ni(); -- i >= 0; ) {
      val3 i_val = get_figate(i)->get_fval();
      if (i_val == val_X) {
	break;
      }
      if (i_val == val_1) {
	xval = neg3(xval);
      }
    }
    if (i < 0) {
      prop_f(xval);
    }
  }
  else {
    if (val == val_1) {
      xval = neg3(xval);
    }
    int ucount = 0;
    gate_t* last = NULL;
    for (int i = get_ni(); -- i >= 0; ) {
      gate_t* i_gate = get_figate(i);
      val3 i_val = i_gate->get_fval();
      if (i_val == val_X) {
	ucount ++;
	last = i_gate;
      }
      else if (i_val == val_1) {
	xval = neg3(xval);
      }
    }
    if (ucount == 0) {
      if (xval == val_1) {
#ifdef DEBUG
	fprintf(stderr, "unable to justify\n");
#endif
	imp_fail();
      }
    }
    else if (ucount == 1) {
      if (last->chk_fcone() == true) {
	last->bwd_imp_f(xval, this);
      }
      else {
	last->bwd_imp_h(xval, NULL);
      }
    }
  }
  
#ifdef DEBUG
  fprintf(stderr, "%s::fwd_imp_f end\n", get_name());
#endif
}

void
XOR_gate_t::bwd_imp_h(val3 o_val,
		      gate_t* from)
{
#ifdef DEBUG
  fprintf(stderr, "%s::bwd_imp_h(val_%c, %s)\n", get_name(),
	  value_name1(o_val), (from ? from->get_name() : "None"));
#endif

  val3 val = get_gval();
  if (val == val_X) {
    if (o_val == val_0 && chk_term0() == true ||
	o_val == val_1 && chk_term1() == true) {
      if (chk_const() == false) {
	cfna_add(this);
      }
      refl_h(o_val, from);
      eq_imp(o_val);
    }
    else {
      refl_h(o_val, from);
      val3 xval = (get_gtype() == kTgXor) ? val_0 : val_1;
      if (o_val == val_1) {
	xval = neg3(xval);
      }
      int ucount = 0;
      gate_t* last = NULL;
      for (int i = get_ni(); -- i >= 0; ) {
	gate_t* i_gate = get_figate(i);
	val3 val1 = i_gate->get_gval();
	if (val1 == val_X) {
	  ucount ++;
	  last = i_gate;
	}
	else if (val1 == val_1) {
	  xval = neg3(xval);
	}
      }
      if (ucount == 1) {
	last->bwd_imp_h(xval, this);
      }
      else {
	eq_imp(o_val);
      }
    }
  }
  else if (val != o_val) {
#ifdef DEBUG
    fprintf(stderr, "connflict with old value\n");
#endif
    imp_fail();
  }

#ifdef DEBUG
  fprintf(stderr, "%s::bwd_imp_h end\n", get_name());
#endif
}

void
XOR_gate_t::bwd_imp_g(val3 o_val,
		      gate_t* from)
{
#ifdef DEBUG
  fprintf(stderr, "%s::bwd_imp_g(val_%c, %s)\n", get_name(),
	  value_name1(o_val), (from ? from->get_name() : "None"));
#endif

  val3 val = get_gval();
  if (val == val_X) {
    refl_g(o_val, from);
    val3 xval = (get_gtype() == kTgXor) ? val_0 : val_1;
    if (o_val == val_1) {
      xval = neg3(xval);
    }
    int ucount = 0;
    gate_t* last = NULL;
    for (int i = get_ni(); -- i >= 0; ) {
      gate_t* i_gate = get_figate(i);
      val3 val1 = i_gate->get_gval();
      if (val1 == val_X) {
	ucount ++;
	last = i_gate;
      }
      else if (val1 == val_1) {
	xval = neg3(xval);
      }
    }
    if (ucount == 1) {
      if (last->chk_fcone() == true) {
	last->bwd_imp_g(xval, this);
      }
      else {
	last->bwd_imp_h(xval, NULL);
      }
    }
    else {
      eq_imp(o_val);
    }
  }
  else if (val != o_val) {
#ifdef DEBUG
    fprintf(stderr, "connflict with old value\n");
#endif
    imp_fail();
  }

#ifdef DEBUG
  fprintf(stderr, "%s::bwd_imp_g end\n", get_name());
#endif
}

void
XOR_gate_t::bwd_imp_f(val3 o_val,
		      gate_t* from)
{
#ifdef DEBUG
  fprintf(stderr, "%s::bwd_imp_f(val_%c, %s)\n", get_name(),
	  value_name1(o_val), (from ? from->get_name() : "None"));
#endif

  val3 val = get_fval();
  if (val == val_X) {
    refl_f(o_val, from);
    val3 xval = (get_gtype() == kTgXor) ? val_0 : val_1;
    if (o_val == val_1) {
      xval = neg3(xval);
    }
    int ucount = 0;
    gate_t* last = NULL;
    for (int i = get_ni(); -- i >= 0; ) {
      gate_t* i_gate = get_figate(i);
      val3 val1 = i_gate->get_fval();
      if (val1 == val_X) {
	ucount ++;
	last = i_gate;
      }
      else if (val1 == val_1) {
	xval = neg3(xval);
      }
    }
    if (ucount == 1) {
      if (last->chk_fcone() == true) {
	last->bwd_imp_f(xval, this);
      }
      else {
	last->bwd_imp_h(xval, NULL);
      }
    }
  }
  else if (val != o_val) {
#ifdef DEBUG
    fprintf(stderr, "connflict with old value\n");
#endif
    imp_fail();
  }

#ifdef DEBUG
  fprintf(stderr, "%s::bwd_imp_f end\n", get_name());
#endif
}

END_NAMESPACE_YM_MATPG
