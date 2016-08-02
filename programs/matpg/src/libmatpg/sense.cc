/*
 *	matpg - Mat's Automatic Testpattern Generator
 *	10/5/91 by Mat.
 *	$Log: sense.c,v $
 * Revision 2.3  1992/01/22  23:49:14  yusuke
 * no difference
 *
 * Revision 2.3  1992/01/22  23:49:14  yusuke
 * no difference
 *
 * Revision 2.2  91/12/28  16:59:19  yusuke
 * Final , Final revision
 * 
 * Revision 2.1  91/12/26  19:57:47  yusuke
 * Final revision of version 2
 * 
 * Revision 2.0  91/12/21  18:56:49  yusuke
 * '91 Cristmas version
 * 
 * Revision 1.5  1991/10/05  08:18:18  yusuke
 * add Log and RCSid for RCS
 *
 */

/* Unique sensitization */

#include "network.h"
#include "search_mgr.h"
#include "sense.h"
#include "maqueue.h"
#include "pgraph.h"


BEGIN_NAMESPACE_YM_MATPG


extern
int us_mode;

static
bool reach_to_po;

static
vector<gate_t*> dfs;

static
int max_p;

static
int max_s;

static
int po_sideval;

static
int max_level;

static
vector<gate_t*> blocks;

static
void
mark_ppath(gate_t* gate);

static
int
mark_spath(gate_t* gate);

static
void
set_block(gate_t* gate,
	  val3 val);

static
void
rst_block();

static
bool
chk_another_path(gate_t* gate);

inline
int&
side_val(gate_t* gate)
{
  return (int&)(gate->side_val);
}

static
void
get_df1(gate_t* gate)
{
  if (gate->chk_mark() == false && gate->chk_pg() == true) {
    gate->set_mark();
    if (gate->get_gval() == val_X || gate->get_fval() == val_X) {
      dfs.push_back(gate);
    }
    else if (gate->is_po() == false) {
      int no = gate->get_act_no();
      for ( int i = 0; i < no; ) {
	get_df1(gate->get_act_fogate(i ++));
      }
    }
    else {
      reach_to_po = true;
    }
  }
}

void
sensitize(gate_t* f_node)
{
  if (pgraph_chk_changes() == false) {
    return;
  }
  pgraph_clr_changes();

  max_level = gn_get_max_level() + 1;
  dfs.clear();
  reach_to_po = false;
  gn_clr_mark();
  int f_no = f_node->get_act_no();
  for ( int i = 0; i < f_no; i ++) {
    if (f_node->get_act_fogate(i)->is_f_site() == true) {
      get_df1(f_node->get_act_fogate(i));
    }
  }
  if (dfs.empty() || reach_to_po == true) {
    return;
  }
  
  /* mark 'primary path' */
  gn_clr_mark();
  int max_side_val = dfs[0]->lvl_i;
  mark_ppath(dfs[0]);
  for (size_t i = 1; i < dfs.size(); i ++) {
    int side_val = mark_spath(dfs[i]);
    if ( max_side_val < side_val ) {
      max_side_val = side_val;
    }
  }
  gate_t* gate = dfs[0];
  for ( ; ; ) {
    int ni = gate->get_ni();
    int no = gate->get_act_no();
    if (gate->lvl_i >= max_side_val) {
      /* gate is a single dominator */
      if (gate->get_gval() == val_X) {
	if (gate->get_fval() == val_0) {
	  maq_add(gate, val_1, val_X);
	}
	else if (gate->get_fval() == val_1) {
	  maq_add(gate, val_0, val_X);
	}
	else {
	  val3 nc_val = gate->get_nc_val();
	  if (nc_val != val_X) {
	    for (int i = ni; -- i >= 0; ) {
	      gate_t* i_gate = gate->get_figate(i);
	      if (i_gate->get_gval() == val_X) {
		if (i_gate->chk_fcone() == false) {
		  maq_add(i_gate, nc_val, val_X);
		}
		else if (i_gate->chk_dif() == false) {
		  maq_add(i_gate, nc_val, nc_val);
		}
	      }
	    }
	  }
	}
      }
      else if (gate->get_fval() == val_X) {
	if (gate->get_gval() == val_0) {
	  maq_add(gate, val_X, val_1);
	}
	else if (gate->get_gval() == val_1) {
	  maq_add(gate, val_X, val_0);
	}
      }
    }
    else if (us_mode > 1) {
      /* finding path controller */
      if (ni >= 2 && gate->get_c_val() != val_X) {
	for (int i = ni; -- i >= 0; ) {
	  gate_t* i_gate = gate->get_figate(i);
	  if (i_gate->chk_dif() == true
	      || i_gate->get_gval() != val_X) {
	    continue;
	  }
	  gate_t* root = i_gate;
	  val3 root_ncval = gate->get_nc_val();
	  val3 root_cval = gate->get_c_val();
#ifndef NAIVE_SENSE
	  while (root->get_ni() == 1) {
	    if ( root->get_gtype() == kTgNot ) {
	      root_cval = neg3(root_cval);
	      root_ncval = neg3(root_ncval);
	    }
	    root = root->get_figate(0);
	  }
#endif
	  max_p = -1;
	  max_s = -1;
	  set_block(root, root_cval);
	  if ( max_s < max_p ) {
	    max_s = max_p;
	  }
	  if ( blocks.size() > 1) {
	    gn_clr_mark();
	    int j;
	    for (j = dfs.size(); -- j >= 0; ) {
	      if (chk_another_path(dfs[j]) == true) {
		break;
	      }
	    }
	    if (j < 0) {	/* can not find another path */
	      if (root->chk_fcone() == false) {
		maq_add(root, root_ncval, val_X);
	      }
	      else {
		maq_add(root, root_ncval, root_ncval);
	      }
	    }
	  }
	  rst_block();
	}
      }
    }
    if (gate->is_po() == true) {
      break;
    }
    if ( max_side_val < side_val(gate) ) {
      max_side_val = side_val(gate);
    }

    /* find next primary path node */
    for (int i = 0; i < no; i ++) {
      gate_t* o_gate = gate->get_act_fogate(i);
      if (o_gate->chk_ppath() == true) {
	gate = o_gate;
	break;
      }
    }
  }
}

static
void
mark_ppath(gate_t* gate)
{
  gate->set_mark();
  gate->set_ppath();
  int max_side_val = gate->lvl_i;
  if (gate->is_po() == true) {
    po_sideval = max_side_val;
    return;
  }
  gate_t* best_fo = NULL;
  int best_lvl = max_level + 1;
  int no = gate->get_act_no();
  for (int i = 0; i < no; i ++) {
    gate_t* gate1 = gate->get_act_fogate(i);
    if (gate1->chk_pg() == true) {
      if (gate1->min_lvl_i < best_lvl) {
	best_lvl = gate1->min_lvl_i;
	best_fo = gate1;
      }
    }
    else {
      gate1->rst_ppath();
    }
  }
  if (best_fo == NULL) {
    fputs("sensitize: Internal error!\n", stderr);
    exit(1);
  }
  mark_ppath(best_fo);
  for (int i = 0; i < no; i ++) {
    gate_t* gate1 = gate->get_act_fogate(i);
    if (gate1->chk_pg() == true) {
      if (gate1 == best_fo) continue;
      int side_val1 = mark_spath(gate1);
      if (side_val1 > max_side_val) {
	max_side_val = side_val1;
      }
    }
    else {
      gate1->rst_ppath();
    }
  }
  side_val(gate) = max_side_val;
}

static
int
mark_spath(gate_t* gate)
{
  if (gate->chk_mark() == true) {
    if (gate->chk_ppath() == true) {
      return gate->lvl_i;
    }
    return side_val(gate);
  }
  gate->set_mark();
  gate->rst_ppath();
  if (gate->is_po() == true
#ifndef NAIVE_SENSE
      || gate->lvl_i >= po_sideval
#endif
      ) {
    return side_val(gate) = max_level;
  }
  int max_side_val = -1;
  int no = gate->get_act_no();
  for ( int i = 0; i < no; i ++) {
    gate_t* gate1 = gate->get_act_fogate(i);
    if (gate1->chk_pg() == true) {
      int side_val1 = mark_spath(gate1);
      if (side_val1 > max_side_val) {
	max_side_val = side_val1;
      }
    }
  }
  return side_val(gate) = max_side_val;
}

static
void
set_block(gate_t* gate,
	  val3 cval)
{
  blocks.clear();
  for (int i = gate->get_act_no(); -- i >= 0; ) {
    gate_t* o_gate = gate->get_act_fogate(i);
    if (o_gate->chk_pg() == false) {
#ifndef NAIVE_SENSE
      if (o_gate->get_ni() != 1) {
	if (o_gate->get_c_val() == cval) {
	  set_block(o_gate, o_gate->get_o_val());
	}
      }
      else {
	if ( o_gate->get_gtype() == kTgNot ) {
	  set_block(o_gate, neg3(cval));
	}
	else {
	  set_block(o_gate, cval);
	}
      }
#endif
    }
    else {
      if (o_gate->get_c_val() != cval) {
	continue;
      }
      o_gate->set_block();
      blocks.push_back(o_gate);
      if (o_gate->chk_ppath() == true) {
	if ( max_p < o_gate->lvl_i ) {
	  max_p = o_gate->lvl_i;
	}
      }
      else {
	if ( max_s < o_gate->lvl_i ) {
	  max_s = o_gate->lvl_i;
	}
      }
#ifndef NAIVE_SENSE
      set_block(o_gate, o_gate->get_o_val());
#endif
    }
  }
}

static
void
rst_block()
{
  for (vector<gate_t*>::iterator p = blocks.begin();
       p != blocks.end(); ++ p) {
    (*p)->rst_block();
  }
}

static
bool
chk_another_path(gate_t* gate)
{
  if (gate->chk_block() == true) {
    return false;
  }
  if (gate->chk_mark() == true) {
    return false;
  }
  gate->set_mark();
  if (gate->is_po() == true) {
    return true;
  }
  if (gate->chk_ppath() == true) {
    if (gate->lvl_i > max_p) {
      return true;
    }
  }
  else {
    if (gate->lvl_i > max_s) {
      return true;
    }
  }
  int no = gate->get_act_no();
  for ( int i = 0; i < no; i ++) {
    gate_t* o_gate = gate->get_act_fogate(i);
    if (o_gate->chk_pg() == true) {
      if (chk_another_path(o_gate) == true) {
	return true;
      }
    }
  }
  return false;
}

END_NAMESPACE_YM_MATPG
