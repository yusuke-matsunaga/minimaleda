/*
 *	matpg - Mat's Automatic Testpattern Generator
 *	10/5/91 by Mat.
 *	$Log: used_gate.c,v $
 * Revision 2.2  1992/01/22  23:49:14  yusuke
 * no difference
 *
 * Revision 2.2  1992/01/22  23:49:14  yusuke
 * no difference
 *
 * Revision 2.1  91/12/28  16:59:30  yusuke
 * Final , Final revision
 * 
 * Revision 2.0  91/12/21  18:58:58  yusuke
 * '91 Cristmas version
 * 
 * Revision 1.5  1991/10/05  08:18:18  yusuke
 * add Log and RCSid for RCS
 *
 */

#include "used_gate.h"


BEGIN_NAMESPACE_YM_MATPG


gate_t** used_gate = NULL;
int used_gate_size = 0;
int used_gate_num = 0;

void
used_gate_init(int ngate)
{
  if (used_gate_size < ngate) {
    if (used_gate) {
      delete [] used_gate;
    }
    used_gate_size = 1;
    do {
      used_gate_size <<= 1;
    } while (used_gate_size < ngate);
    used_gate = new gate_t*[used_gate_size];
  }
  used_gate_num = 0;
}

void
used_gate_clr()
{
  gate_t** ptr = used_gate;
  gate_t** e_ptr = ptr + used_gate_num;
  while (ptr != e_ptr) {
    gate_t* const gate = *ptr ++;
    gate->state = 0L;
    gate->rst_used();
  }
  used_gate_num = 0;
}

END_NAMESPACE_YM_MATPG
