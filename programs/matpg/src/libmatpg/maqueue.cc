/*
 *	matpg - Mat's Automatic Testpattern Generator
 *	10/5/91 by Mat.
 *	$Log: maqueue.c,v $
 * Revision 2.2  1992/01/22  23:49:14  yusuke
 * no difference
 *
 * Revision 2.2  1992/01/22  23:49:14  yusuke
 * no difference
 *
 * Revision 2.1  91/12/28  16:59:02  yusuke
 * Final , Final revision
 * 
 * Revision 2.0  91/12/21  18:53:09  yusuke
 * '91 Cristmas version
 * 
 * Revision 1.5  1991/10/05  08:18:18  yusuke
 * add Log and RCSid for RCS
 *
 */

#include "gate.h"
#include "maqueue.h"
#include "search_mgr.h"


BEGIN_NAMESPACE_YM_MATPG


struct aq_t
{
  aq_t(gate_t* gate1,
       val3 gval1,
       val3 fval1) :
    gate(gate1),
    gval(gval1),
    fval(fval1) { }
  
  gate_t* gate;
  val3 gval;
  val3 fval;
};

static
vector<aq_t> m_array;
static
size_t gpos = 0;
static
vector<gate_t*> const0;
static
vector<gate_t*> const1;


void
const_clr()
{
  const0.clear();
  const1.clear();
}

void
const0_add(gate_t* gate)
{
  const0.push_back(gate);
  gate->set_const();
}

void
const1_add(gate_t* gate)
{
  const1.push_back(gate);
  gate->set_const();
}

void
maq_clr(bool first)
{
  m_array.clear();
  gpos = 0;
  if (first == true) {
    for (vector<gate_t*>::iterator p = const0.begin();
	 p != const0.end(); ++ p) {
      gate_t* gate = *p;
      if (gate->is_active() == true) {
	maq_add(gate, val_0, val_X);
      }
    }
    for (vector<gate_t*>::iterator p = const1.begin();
	 p != const1.end(); ++ p) {
      gate_t* gate = *p;
      if (gate->is_active() == true) {
	maq_add(gate, val_1, val_X);
      }
    }
  }
}

void
maq_add(gate_t* gate,
	val3 gval,
	val3 fval)
{
  m_array.push_back(aq_t(gate, gval, fval));
}

bool
maq_get()
{
  if ( gpos == m_array.size() ) {
    m_array.clear();
    gpos = 0;
    return false;
  }
  aq_t& p = m_array[gpos];
  mand_assign(p.gate, p.gval, p.fval);
  ++ gpos;
  return true;
}

END_NAMESPACE_YM_MATPG
