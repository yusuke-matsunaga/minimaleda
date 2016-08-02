/*
 *	matpg - Mat's Automatic Testpattern Generator
 *	10/5/91 by Mat.
 *	$Log: pgraph.c,v $
 * Revision 2.2  1992/01/31  00:20:11  yusuke
 * Febrary '92 version
 *
 * Revision 2.1  1991/12/28  16:59:11  yusuke
 * Final , Final revision
 *
 * Revision 2.1  1991/12/28  16:59:11  yusuke
 * Final , Final revision
 *
 * Revision 2.0  91/12/21  18:55:12  yusuke
 * '91 Cristmas version
 * 
 * Revision 1.5  1991/10/05  08:18:18  yusuke
 * add Log and RCSid for RCS
 *
 */

#include "network.h"
#include "pgraph.h"
#include "search_mgr.h"


BEGIN_NAMESPACE_YM_MATPG


static
bool change_flag = false;

static
gate_t* f_gate;

/*  <<  construct propagation graph  >>  */
static
void
pgconst(gate_t* gate);

void
pgraph_construct(gate_t* gate)
{
  f_gate = gate;
  for ( int i = f_gate->get_act_no(); -- i >= 0; ) {
    gate_t* o_gate = f_gate->get_act_fogate(i);
    if (o_gate->is_f_site() == true) {
      pgconst(o_gate);
    }
  }
  change_flag = true;
}

static
void
pgconst(gate_t* gate)
{
  gate->set_fcone();
  gate->set_pg_dif();
  for ( int i = gate->get_act_no(); -- i >= 0; ) {
    gate_t* o_gate = gate->get_act_fogate(i);
    if (o_gate->chk_fcone() == false) {
      pgconst(o_gate);
    }
  }
}

/*  <<  destruct propagation graph  >>  */
static
void
pgdest(gate_t* gate);

void
pgraph_destruct()
{
  for ( int i = f_gate->get_act_no(); -- i >= 0; ) {
    gate_t* o_gate = f_gate->get_act_fogate(i);
    if (o_gate->is_f_site() == true) {
      pgdest(o_gate);
    }
  }
}

static
void
pgdest(gate_t* gate)
{
  gate->rst_fcone();
  gate->rst_pg_dif();
  for ( int i = gate->get_act_no(); -- i >= 0; ) {
    gate_t* o_gate = gate->get_act_fogate(i);
    if (o_gate->chk_fcone() == true) {
      pgdest(o_gate);
    }
  }
}

/*  <<  INDIFferentiated nodes  >>  */
static
vector<gate_t*> indif;

void
pgraph_clr_indif()
{
  indif.clear();
}

void
pgraph_add_indif(gate_t* gate)
{
  gate->rst_pg();
  gate->rst_dif();
  indif.push_back(gate);
}

void
pgraph_clr_changes()
{
  change_flag = false;
}

bool
pgraph_chk_changes()
{
  return change_flag;
}

/*  <<  update propagation grpah using INDIF nodes  >>  */
bool
pgraph_update()
{
  if ( indif.empty() ) {
    return true;
  }
  change_flag = true;

  gn_clr_mark();
  lvlq_clr();
  for (size_t i = 0; i < indif.size(); ++ i) {
    gate_t* gate = indif[i];
    for (int j = gate->get_act_no(); -- j >= 0; ) {
      gate_t* gate1 = gate->get_act_fogate(j);
      if (gate1->chk_dif() == true && gate1->chk_mark() == false) {
	gate1->set_mark();
	lvlq_put(gate1);
      }
    }
  }
  for (gate_t* gate; (gate = lvlq_get_from_bottom());  ) {
    int i;
    for (i = gate->get_ni(); -- i >= 0; ) {
      if (gate->get_figate(i)->chk_dif() == true) {
	break;
      }
    }
    if (i < 0) {
      save_value(gate);
      gate->rst_pg();
      gate->rst_dif();
      for (int j = gate->get_act_no(); -- j >= 0; ) {
	gate_t* gate1 = gate->get_act_fogate(j);
	if (gate1->chk_mark() == false) {
	  gate1->set_mark();
	  lvlq_put(gate1);
	}
      }
    }
  }
  
  gn_clr_mark();
  lvlq_clr();
  for (size_t i = 0; i < indif.size(); ++ i) {
    gate_t* gate = indif[i];
    for (int j = gate->get_ni(); -- j >= 0; ) {
      gate_t* gate1 = gate->get_figate(j);
      if (gate1->chk_pg() == true &&
	  gate1->is_po() == false &&
	  gate1->chk_mark() == false) {
	gate1->set_mark();
	if (gate1->is_f_site() == true) {
	  save_value(gate1);
	  gate1->rst_pg();
	}
	else {			
	  lvlq_put(gate1);
	}
      }
    }
  }
  for (gate_t* gate; (gate = lvlq_get_from_top()); ) {
    int i;
    for (i = gate->get_act_no(); -- i >= 0; ) {
      if (gate->get_act_fogate(i)->chk_pg() == true) {
	break;
      }
    }
    if (i < 0) {
      save_value(gate);
      gate->rst_pg();
      for (int i = gate->get_ni(); -- i >= 0; ) {
	gate_t* gate1 = gate->get_figate(i);
	if (gate1->chk_pg() == true &&
	    gate1->is_po() == false &&
	    gate1->chk_mark() == false) {
	  gate1->set_mark();
	  if (gate1->is_f_site() == true) {
	    save_value(gate1);
	    gate1->rst_pg();
	  }
	  else {			
	    lvlq_put(gate1);
	  }
	}
      }
    }
  }
  for (int i = f_gate->get_act_no(); -- i >= 0; ) {
    if (f_gate->get_act_fogate(i)->chk_pg() == true) {
      return true;
    }
  }
#ifdef DEBUG
  fprintf(stderr, "propagation graph disappeares...\n");
#endif
  return false;
}

/*  <<  for debug  >>  */
static
void
pgdump(gate_t* gate,
       FILE* fp);

void
pgraph_dump(FILE* fp)
{
  gn_clr_mark();
  pgdump(f_gate, fp);
}

static
void
pgdump(gate_t* gate,
       FILE* fp)
{
  if (gate->chk_mark() == true) {
    return;
  }
  gate->set_mark();
  if (gate->is_po() == true) {
    fputs("<PO> ", fp);
  }
  fprintf(fp, "%s: value = %s (%s)\n", gate->get_name(),
	  value_name(gate->get_gval(), gate->get_fval()),
	  (gate->chk_pg() == true) ? "DIFF" : "____");
  
  int i;
  gate_t* gate1;
  int ni = gate->get_ni();
  for (i = 0; i < ni; i ++) {
    gate1 = gate->get_figate(i);
    fprintf(stderr, "    [%d] %s: value = %s\n", i + 1, gate1->get_name(),
	    value_name(gate1->get_gval(), gate1->get_fval()));
  }
  
  int no = gate->get_act_no();
  for (i = 0; i < no; i ++) {
    gate1 = gate->get_act_fogate(i);
    if (gate1->chk_fcone() == true) {
      pgdump(gate1, fp);
    }
  }
}

END_NAMESPACE_YM_MATPG
