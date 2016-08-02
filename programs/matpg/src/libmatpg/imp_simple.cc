/*
 *	matpg - Mat's Automatic Testpattern Generator
 *	10/5/91 by Mat.
 *	$Log: imp_simple.c,v $
 * Revision 2.3  1992/01/31  00:20:11  yusuke
 * Febrary '92 version
 *
 * Revision 2.2  1991/12/28  16:58:55  yusuke
 * Final , Final revision
 *
 * Revision 2.2  1991/12/28  16:58:55  yusuke
 * Final , Final revision
 *
 * Revision 2.1  91/12/23  23:11:29  yusuke
 * a slightly speed-up
 * 
 * Revision 2.0  91/12/21  18:52:17  yusuke
 * '91 Cristmas version
 * 
 * Revision 1.7  1991/10/17  02:55:53  yusuke
 * made a new function eq_imp()
 *
 * Revision 1.6  1991/10/05  08:18:18  yusuke
 * add Log and RCSid for RCS
 *
 */

#include "gate.h"
#include "search_mgr.h"
#include "learn.h"
#include "pgraph.h"
#include "cfna.h"


BEGIN_NAMESPACE_YM_MATPG

/* SIMPLE_gate_t */
void
SIMPLE_gate_t::fwd_imp_h(val3 i_val)
{
#ifdef DEBUG
  fprintf(stderr, "%s::fwd_imp_h(val_%c)\n", get_name(), value_name1(i_val));
#endif

  val3 old_val = get_gval();
  if (i_val == c_val) {
    if (old_val == val_X) {
      prop_h(o_val);
    }
    else if (old_val != o_val) {
#ifdef DEBUG
      fprintf(stderr, "conflict with old value\n");
#endif
      imp_fail();
    }
  }
  else {
    if (old_val == val_X) {
      val3 nc_val = neg3(c_val);
      int i;
      for (i = get_ni(); -- i >= 0; ) {
	if (get_figate(i)->get_gval() != nc_val) {
	  break;
	}
      }
      if (i < 0) {
	prop_h(neg3(o_val));
      }
    }
    else if (old_val == o_val) {
      int ucount = 0;
      gate_t* last = NULL;
      int i;
      for (i = get_ni(); -- i >= 0; ) {
	gate_t* i_gate = get_figate(i);
	val3 val = i_gate->get_gval();
	if (val == val_X) {
	  ucount ++;
	  last = i_gate;
	}
	else if (val == c_val) {
	  ucount = -1;
	  break;
	}
      }
      if (ucount == 0) {
#ifdef DEBUG
	fprintf(stderr, "unable to justify\n");
#endif
	imp_fail();
      }
      if (ucount == 1) {
	last->bwd_imp_h(c_val, this);
      }
    }
  }
  
#ifdef DEBUG
  fprintf(stderr, "%s::fwd_imp_h end\n", get_name());
#endif
}

void
SIMPLE_gate_t::fwd_imp_g(val3 i_val)
{
#ifdef DEBUG
  fprintf(stderr, "%s::fwd_imp_g(val_%c)\n", get_name(), value_name1(i_val));
#endif

  val3 old_val = get_gval();
  if (i_val == c_val) {
    if (old_val == val_X) {
      prop_g(o_val);
    }
    else if (old_val != o_val) {
#ifdef DEBUG
      fprintf(stderr, "conflict with old value\n");
#endif
      imp_fail();
    }
  }
  else {
    if (old_val == val_X) {
      val3 nc_val = neg3(c_val);
      int i;
      for (i = get_ni(); -- i >= 0; ) {
	if (get_figate(i)->get_gval() != nc_val) {
	  break;
	}
      }
      if (i < 0) {
	prop_g(neg3(o_val));
      }
    }
    else if (old_val == o_val) {
      int ucount = 0;
      gate_t* last = NULL;
      int i;
      for (i = get_ni(); -- i >= 0; ) {
	gate_t* i_gate = get_figate(i);
	val3 val = i_gate->get_gval();
	if (val == val_X) {
	  ucount ++;
	  last = i_gate;
	}
	else if (val == c_val) {
	  ucount = -1;
	  break;
	}
      }
      if (ucount == 0) {
#ifdef DEBUG
	fprintf(stderr, "unable to justify\n");
#endif
	imp_fail();
      }
      if (ucount == 1) {
	if (last->chk_fcone() == true) {
	  last->bwd_imp_g(c_val, this);
	}
	else {
	  last->bwd_imp_h(c_val, NULL);
	}
      }
    }
  }
  
#ifdef DEBUG
  fprintf(stderr, "%s::fwd_imp_g end\n", get_name());
#endif
}

void
SIMPLE_gate_t::fwd_imp_f(val3 i_val)
{
#ifdef DEBUG
  fprintf(stderr, "%s::fwd_imp_f(val_%c)\n", get_name(), value_name1(i_val));
#endif

  val3 old_val = get_fval();
  if (i_val == c_val) {
    if (old_val == val_X) {
      prop_f(o_val);
    }
    else if (old_val != o_val) {
#ifdef DEBUG
      fprintf(stderr, "conflict with old value\n");
#endif
      imp_fail();
    }
  }
  else {
    if (old_val == val_X) {
      val3 nc_val = neg3(c_val);
      int i;
      for (i = get_ni(); -- i >= 0; ) {
	if (get_figate(i)->get_fval() != nc_val) {
	  break;
	}
      }
      if (i < 0) {
	prop_f(neg3(o_val));
      }
    }
    else if (old_val == o_val) {
      int ucount = 0;
      gate_t* last = NULL;
      int i;
      for (i = get_ni(); -- i >= 0; ) {
	gate_t* i_gate = get_figate(i);
	val3 val = i_gate->get_fval();
	if (val == val_X) {
	  ucount ++;
	  last = i_gate;
	}
	else if (val == c_val) {
	  ucount = -1;
	  break;
	}
      }
      if (ucount == 0) {
#ifdef DEBUG
	fprintf(stderr, "unable to justify\n");
#endif
	imp_fail();
      }
      if (ucount == 1) {
	if (last->chk_fcone() == true) {
	  last->bwd_imp_f(c_val, this);
	}
	else {
	  last->bwd_imp_h(c_val, NULL);
	}
      }
    }
  }
  
#ifdef DEBUG
  fprintf(stderr, "%s::fwd_imp_f end\n", get_name());
#endif
}

void
SIMPLE_gate_t::bwd_imp_h(val3 i_val,
			 gate_t* from)
{
#ifdef DEBUG
  fprintf(stderr, "%s::bwd_imp_h(val_%c, %s)\n", get_name(),
	  value_name1(i_val), (from ? from->get_name() : "None"));
#endif

  val3 val = get_gval();

  if (val == val_X) {
    if (i_val == val_0 && chk_term0() == true ||
	i_val == val_1 && chk_term1() == true) {
      if (chk_const() == false) {
	cfna_add(this);
      }
      refl_h(i_val, from);
      eq_imp(i_val);
    }
    else {
      refl_h(i_val, from);
      if (i_val == o_val) {
	int ucount = 0;
	gate_t* last = NULL;
	int i;
	for (i = get_ni(); -- i >= 0; ) {
	  gate_t* i_gate = get_figate(i);
	  val3 val1 = i_gate->get_gval();
	  if (val1 == val_X) {
	    ucount ++;
	    last = i_gate;
	  }
	  else if (val1 == c_val) {
	    ucount = -1;
	    break;
	  }
	}
	if (ucount == 0) {
#ifdef DEBUG
	  fprintf(stderr, "unable to justify\n");
#endif
	  imp_fail();
	}
	if (ucount == 1) {
	  last->bwd_imp_h(c_val, this);
	}
	else {
	  eq_imp(i_val);
	}
      }
      else {
	val3 nc_val = neg3(c_val);
	for (int i = get_ni(); -- i >= 0; ) {
	  get_figate(i)->bwd_imp_h(nc_val, this);
	}
      }
    }
  }
  else if (val != i_val) {
#ifdef DEBUG
    fprintf(stderr, "conflict with old value\n");
#endif
    imp_fail();
  }
  
#ifdef DEBUG
  fprintf(stderr, "%s::bwd_imp_h end\n", get_name());
#endif
}

void
SIMPLE_gate_t::bwd_imp_g(val3 i_val,
			 gate_t* from)
{
#ifdef DEBUG
  fprintf(stderr, "%s::bwd_imp_g(val_%c, %s)\n", get_name(),
	  value_name1(i_val), (from ? from->get_name() : "None"));
#endif

  val3 val = get_gval();

  if (val == val_X) {
    refl_g(i_val, from);
    if (i_val == o_val) {
      int ucount = 0;
      gate_t* last = NULL;
      for (int i = get_ni(); -- i >= 0; ) {
	gate_t* i_gate = get_figate(i);
	val3 val1 = i_gate->get_gval();
	if (val1 == val_X) {
	  ucount ++;
	  last = i_gate;
	}
	else if (val1 == c_val) {
	  ucount = -1;
	  break;
	}
      }
      if (ucount == 0) {
#ifdef DEBUG
	fprintf(stderr, "unable to justify\n");
#endif
	imp_fail();
      }
      if (ucount == 1) {
	if (last->chk_fcone() == true) {
	  last->bwd_imp_g(c_val, this);
	}
	else {
	  last->bwd_imp_h(c_val, NULL);
	}
      }
      else {
	eq_imp(i_val);
      }
    }
    else {
      val3 nc_val = neg3(c_val);
      for (int i = get_ni(); -- i >= 0; ) {
	gate_t* i_gate = get_figate(i);
	if (i_gate->chk_fcone() == true) {
	  i_gate->bwd_imp_g(nc_val, this);
	}
	else {
	  i_gate->bwd_imp_h(nc_val, NULL);
	}
      }
    }
  }
  else if (val != i_val) {
#ifdef DEBUG
    fprintf(stderr, "conflict with old value\n");
#endif
    imp_fail();
  }

#ifdef DEBUG
  fprintf(stderr, "%s::bwd_imp_g end\n", get_name());
#endif
}

void
SIMPLE_gate_t::bwd_imp_f(val3 i_val,
			 gate_t* from)
{
#ifdef DEBUG
  fprintf(stderr, "%s::bwd_imp_f(val_%c, %s)\n", get_name(),
	  value_name1(i_val), (from ? from->get_name() : "None"));
#endif

  val3 old_val = get_fval();

  if (old_val == val_X) {
    refl_f(i_val, from);
    if (i_val == o_val) {
      int ucount = 0;
      gate_t* last = NULL;
      for (int i = get_ni(); -- i >= 0; ) {
	gate_t* i_gate = get_figate(i);
	val3 val1 = i_gate->get_fval();
	if (val1 == val_X) {
	  ucount ++;
	  last = i_gate;
	}
	else if (val1 == c_val) {
	  ucount = -1;
	  break;
	}
      }
      if (ucount == 0) {
#ifdef DEBUG
	fprintf(stderr, "unable to justify\n");
#endif
	imp_fail();
      }
      if (ucount == 1) {
	if (last->chk_fcone() == true) {
	  last->bwd_imp_f(c_val, this);
	}
	else {
	  last->bwd_imp_h(c_val, NULL);
	}
      }
    }
    else {
      val3 nc_val = neg3(c_val);
      for (int i = get_ni(); -- i >= 0; ) {
	gate_t* i_gate = get_figate(i);
	if (i_gate->chk_fcone() == true) {
	  i_gate->bwd_imp_f(nc_val, this);
	}
	else {
	  i_gate->bwd_imp_h(nc_val, NULL);
	}
      }
    }
  }
  else if (old_val != i_val) {
#ifdef DEBUG
    fprintf(stderr, "conflict with old value\n");
#endif
    imp_fail();
  }

#ifdef DEBUG
  fprintf(stderr, "%s::bwd_imp_f end\n", get_name());
#endif
}

END_NAMESPACE_YM_MATPG
