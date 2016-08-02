#ifndef H_MATPG_MASTACK
#define H_MATPG_MASTACK

/*
 *	matpg - Mat's Automatic Testpattern Generator
 *	10/5/91 by Mat.
 *	$Log: maqueue.h,v $
 * Revision 2.1  1992/01/22  23:49:14  yusuke
 * no difference
 *
 * Revision 2.1  1992/01/22  23:49:14  yusuke
 * no difference
 *
 * Revision 2.0  91/12/21  18:53:27  yusuke
 * '91 Cristmas version
 * 
 * Revision 1.5  1991/10/05  08:18:18  yusuke
 * add Log and RCSid for RCS
 *
 */

#include "matpg.h"
#include "val.h"

BEGIN_NAMESPACE_YM_MATPG

class gate_t;

extern void maq_add(gate_t* gate, val3 gval, val3 fval);
extern bool maq_get();
extern void maq_clr(bool first = false);

extern void const_clr();
extern void const0_add(gate_t* gate);
extern void const1_add(gate_t* gate);

END_NAMESPACE_YM_MATPG

#endif
