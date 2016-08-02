#ifndef H_MATPG_USED_GATE
#define H_MATPG_USED_GATE

/*
 *	matpg - Mat's Automatic Testpattern Generator
 *	10/5/91 by Mat.
 *	$Log: used_gate.h,v $
 * Revision 2.1  1992/01/22  23:49:14  yusuke
 * no difference
 *
 * Revision 2.1  1992/01/22  23:49:14  yusuke
 * no difference
 *
 * Revision 2.0  91/12/21  18:59:09  yusuke
 * '91 Cristmas version
 * 
 * Revision 1.5  1991/10/05  08:18:18  yusuke
 * add Log and RCSid for RCS
 *
 */

#include "matpg.h"
#include "gate.h"

BEGIN_NAMESPACE_YM_MATPG

extern gate_t** used_gate;
extern int used_gate_size;
extern int used_gate_num;

extern void used_gate_init(int ngate);
extern void used_gate_clr();

inline
void
used_gate_add(gate_t* gate)
{
  if (gate->chk_used() == false) {
    gate->set_used();
    used_gate[used_gate_num ++] = gate;
  }
}

END_NAMESPACE_YM_MATPG

#endif
