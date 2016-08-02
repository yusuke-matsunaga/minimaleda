/*
 *	matpg - Mat's Automatic Testpattern Generator
 *	10/5/91 by Mat.
 *	$Log: imp_buf.c,v $
 * Revision 2.4  1992/01/31  00:20:11  yusuke
 * Febrary '92 version
 *
 * Revision 2.3  1991/12/28  16:58:51  yusuke
 * Final , Final revision
 *
 * Revision 2.3  1991/12/28  16:58:51  yusuke
 * Final , Final revision
 *
 * Revision 2.2  91/12/24  14:40:46  yusuke
 * 
 * a little bit modification
 * 
 * Revision 2.1  91/12/23  23:11:21  yusuke
 * a slightly speed-up
 * 
 * Revision 2.0  91/12/21  18:51:41  yusuke
 * '91 Cristmas version
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


/* BUF_gate_t */
void
BUF_gate_t::fwd_imp_h(val3 i_val)
{
#ifdef DEBUG
  fprintf(stderr, "%s::fwd_imp_h(val_%c)\n", get_name(), value_name1(i_val));
#endif

  val3 o_val = (get_gtype() == kTgBuff) ? i_val : neg3(i_val);
  val3 val = get_gval();
  if (val == val_X) {
    prop_h(o_val);
  }
  else if (val != o_val) {
#ifdef DEBUG
    fprintf(stderr, "conflict with old  value\n");
#endif
    imp_fail();
  }

#ifdef DEBUG
  fprintf(stderr, "%s::fwd_imp_h end\n", get_name());
#endif
}

void
BUF_gate_t::fwd_imp_g(val3 i_val)
{
#ifdef DEBUG
  fprintf(stderr, "%s::fwd_imp_g(val_%c)\n", get_name(), value_name1(i_val));
#endif

  val3 o_val = (get_gtype() == kTgBuff) ? i_val : neg3(i_val);
  val3 val = get_gval();
  if (val == val_X) {
    prop_g(o_val);
  }
  else if (val != o_val) {
#ifdef DEBUG
    fprintf(stderr, "conflict with old  value\n");
#endif
    imp_fail();
  }
  
#ifdef DEBUG
  fprintf(stderr, "%s::fwd_imp_g end\n", get_name());
#endif
}

void
BUF_gate_t::fwd_imp_f(val3 i_val)
{
#ifdef DEBUG
  fprintf(stderr, "%s::fwd_imp_f(val_%c)\n", get_name(), value_name1(i_val));
#endif

  val3 o_val = (get_gtype() == kTgBuff) ? i_val : neg3(i_val);
  val3 val = get_fval();
  if (val == val_X) {
    prop_f(o_val);
  }
  else if (val != o_val) {
#ifdef DEBUG
    fprintf(stderr, "conflict with old  value\n");
#endif
    imp_fail();
  }

#ifdef DEBUG
  fprintf(stderr, "%s::fwd_imp_f end\n", get_name());
#endif
}

void
BUF_gate_t::bwd_imp_h(val3 o_val,
		      gate_t* from)
{
#ifdef DEBUG
  fprintf(stderr, "%s::bwd_imp_h(val_%c, %s)\n", get_name(),
	  value_name1(o_val), (from ? from->get_name() : "None"));
#endif

  val3 val = get_gval();
  if (val == val_X) {
    refl_h(o_val, from);
    val3 i_val = (get_gtype() == kTgBuff) ? o_val : neg3(o_val);
    get_figate(0)->bwd_imp_h(i_val, this);
  }
  else if (val != o_val) {
#ifdef DEBUG
    fprintf(stderr, "conflict with old  value\n");
#endif
    imp_fail();
  }

#ifdef DEBUG
  fprintf(stderr, "%s::bwd_imp_h end\n", get_name());
#endif
}

void
BUF_gate_t::bwd_imp_g(val3 o_val,
		      gate_t* from)
{
#ifdef DEBUG
  fprintf(stderr, "%s::bwd_imp_g(val_%c, %s)\n", get_name(),
	  value_name1(o_val), (from ? from->get_name() : "None"));
#endif

  val3 val = get_gval();
  if (val == val_X) {
    refl_g(o_val, from);
    val3 i_val = (get_gtype() == kTgBuff) ? o_val : neg3(o_val);
    gate_t* i_gate = get_figate(0);
    if (i_gate->chk_fcone() == true) {
      i_gate->bwd_imp_g(i_val, this);
    }
    else {
      i_gate->bwd_imp_h(i_val, NULL);
    }
  }
  else if (val != o_val) {
#ifdef DEBUG
    fprintf(stderr, "conflict with old  value\n");
#endif
    imp_fail();
  }
  
#ifdef DEBUG
  fprintf(stderr, "%s::bwd_imp_g end\n", get_name());
#endif
}

void
BUF_gate_t::bwd_imp_f(val3 o_val,
		      gate_t* from) {
#ifdef DEBUG
  fprintf(stderr, "%s::bwd_imp_f(val_%c, %s)\n", get_name(),
	  value_name1(o_val), (from ? from->get_name() : "None"));
#endif

  val3 val = get_fval();
  if (val == val_X) {
    refl_f(o_val, from);
    val3 i_val = (get_gtype() == kTgBuff) ? o_val : neg3(o_val);
    gate_t* i_gate = get_figate(0);
    if (i_gate->chk_fcone() == true) {
      i_gate->bwd_imp_f(i_val, this);
    }
    else {
      i_gate->bwd_imp_h(i_val, NULL);
    }
  }
  else if (val != o_val) {
#ifdef DEBUG
    fprintf(stderr, "conflict with old  value\n");
#endif
    imp_fail();
  }

#ifdef DEBUG
  fprintf(stderr, "%s::bwd_imp_f end\n", get_name());
#endif
}

END_NAMESPACE_YM_MATPG
