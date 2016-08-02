/*
 *	matpg - Mat's Automatic Testpattern Generator
 *	10/5/91 by Mat.
 *	$Log: imp_sa.c,v $
 * Revision 2.4  1992/01/31  00:20:11  yusuke
 * Febrary '92 version
 *
 * Revision 2.3  1991/12/28  16:58:53  yusuke
 * Final , Final revision
 *
 * Revision 2.3  1991/12/28  16:58:53  yusuke
 * Final , Final revision
 *
 * Revision 2.2  91/12/24  14:40:49  yusuke
 * 
 * a little bit modification
 * 
 * Revision 2.1  91/12/23  23:11:27  yusuke
 * a slightly speed-up
 * 
 * Revision 2.0  91/12/21  18:52:03  yusuke
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
#include "sa_node.h"


BEGIN_NAMESPACE_YM_MATPG


/* sa_node */
void
sa_node::fwd_imp_g(val3 i_val)
{
#ifdef DEBUG
  fprintf(stderr, "%s::fwd_imp_g(val_%c)\n", get_name(), value_name1(i_val));
#endif

  val3 sa_val = is_sa0() ? val_0 : val_1;
  val3 val = get_gval();
  if (val == val_X) {
#ifdef DEBUG
    fprintf(stderr, "set_gval(val_%c)\n", value_name1(i_val));
#endif
    save_value(this);
    set_gval(i_val);
    if (i_val == sa_val) {
      pgraph_add_indif(this);
    }
    if (is_po() == false) {
      get_act_fogate(0)->fwd_imp_g(i_val);
    }
  }
  else if (val != i_val) {
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
sa_node::fwd_imp_f(val3 i_val)
{
#ifdef DEBUG
  fprintf(stderr, "%s::fwd_imp_f(val_%c)\n", get_name(), value_name1(i_val));
#endif

  val3 sa_val = is_sa0() ? val_0 : val_1;
  if (get_fval() == val_X) {
#ifdef DEBUG
    fprintf(stderr, "set_fjval(val_%c)\n", value_name1(sa_val));
#endif
    set_fjval(sa_val);
    if (is_po() == false) {
      get_act_fogate(0)->fwd_imp_f(sa_val);
    }
  }

#ifdef DEBUG
  fprintf(stderr, "%s::fwd_imp_f end\n", get_name());
#endif
}

void
sa_node::bwd_imp_g(val3 o_val,
		   gate_t* from)
{
#ifdef DEBUG
  fprintf(stderr, "%s::bwd_imp_g(val_%c, %s)\n", get_name(),
	  value_name1(o_val), (from ? from->get_name() : "None"));
#endif

  val3 sa_val = is_sa0() ? val_0 : val_1;
  val3 val = get_gval();
  if (val == val_X) {
#ifdef DEBUG
    fprintf(stderr, "set_gval(val_%c)\n", value_name1(o_val));
#endif
    save_value(this);
    set_gval(o_val);
    get_figate(0)->bwd_imp_g(o_val, this);
    if (o_val == sa_val) {
      pgraph_add_indif(this);
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
sa_node::bwd_imp_f(val3 o_val,
		   gate_t* from)
{
#ifdef DEBUG
  fprintf(stderr, "%s::bwd_imp_f(val_%c, %s)\n", get_name(),
	  value_name1(o_val), (from ? from->get_name() : "None"));
#endif

  val3 sa_val = is_sa0() ? val_0 : val_1;
  if (o_val != sa_val) {
#ifdef DEBUG
    fprintf(stderr, "conflict with old  value\n");
#endif
    imp_fail();
  }
  if (get_fval() == val_X) {
#ifdef DEBUG
    fprintf(stderr, "set_fjval(val_%c)\n", value_name1(sa_val));
#endif
    set_fjval(sa_val);
  }

#ifdef DEBUG
  fprintf(stderr, "%s::bwd_imp_f end\n", get_name());
#endif
}

END_NAMESPACE_YM_MATPG
