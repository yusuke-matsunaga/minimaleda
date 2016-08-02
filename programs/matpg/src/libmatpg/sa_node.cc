
/*
 *	matpg - Mat's Automatic Testpattern Generator
 *	10/5/91 by Mat.
 *	$Log: sa_node.c,v $
 * Revision 1.1  1992/01/22  23:49:14  yusuke
 * Initial revision
 *
 * Revision 1.1  1992/01/22  23:49:14  yusuke
 * Initial revision
 *
 * Revision 2.4  1992/01/11  08:57:20  yusuke
 * bug fixed on FSIM_NONE mode
 *
 * Revision 2.3  1991/12/28  16:58:41  yusuke
 * Final , Final revision
 *
 * Revision 2.2  91/12/26  19:56:59  yusuke
 * Final revision of version 2
 * 
 * Revision 2.1  91/12/23  23:11:02  yusuke
 * a slightly speed-up
 * 
 * Revision 2.0  91/12/21  18:50:09  yusuke
 * '91 Cristmas version
 * 
 * Revision 1.7  1991/10/05  08:18:18  yusuke
 * add Log and RCSid for RCS
 *
 */

#include "gate.h"
#include "sa_node.h"


BEGIN_NAMESPACE_YM_MATPG
  
sa_node::sa_node() :
  gate_t(NULL, 1)
{
  init_fogate(NULL, 0);
}
  
bool
sa_node::is_f_site() const
{
  return true;
}

void
sa_node::insert(gate_t* i_gate)
{
  i_gate->rst_po();
  set_po();
  int no = i_gate->get_act_no();
  i_gate->set_act_fogate(no, this, 0);
  i_gate->set_act_no(no + 1);
  set_figate(0, i_gate);
  set_act_no(0);
}

void
sa_node::insert(gate_t* o_gate,
		int ipos)
{
  gate_t* i_gate = o_gate->get_figate(ipos);
  for (int i = i_gate->get_act_no(); -- i >= 0; ) {
    if ( i_gate->get_act_fogate(i) == o_gate &&
	 i_gate->get_act_fogate_ipos(i) == ipos ) {
      i_gate->set_act_fogate(i, this, 0);
      set_figate(0, i_gate);
      set_act_fogate(0, o_gate, ipos);
      o_gate->set_figate(ipos, this);
      set_act_no(1);
      return;
    }
  }
  assert_not_reached(__FILE__, __LINE__);
}

void
sa_node::remove()
{
  gate_t* i_gate = get_figate(0);
  if (get_act_no() == 0) {
    rst_po();
    i_gate->set_po();
    i_gate->set_act_no(i_gate->get_act_no() - 1);
  }
  else {
    gate_t* o_gate = get_act_fogate(0);
    int ipos = get_act_fogate_ipos(0);
    int i;
    for (i = i_gate->get_act_no(); -- i >= 0; ) {
      if (i_gate->get_act_fogate(i) == this) {
	break;
      }
    }
    if (i < 0) {
      fputs("sa_node::remove(): Internal error!\n", stderr);
      exit(1);
    }
    i_gate->set_act_fogate(i, o_gate, ipos);
    o_gate->set_figate(ipos, i_gate);
    set_act_no(0);
  }
}

const char*
sa0_node::get_name() const
{
  return "sa0_node";
}

tTgGateType
sa0_node::get_gtype() const
{
  return kTgConst0;
}

bool
sa0_node::is_sa0() const
{
  return true;
}

const char*
sa1_node::get_name() const
{
  return "sa1_node";
}

tTgGateType
sa1_node::get_gtype() const
{
  return kTgConst1;
}

bool
sa1_node::is_sa1() const
{
  return true;
}

END_NAMESPACE_YM_MATPG
