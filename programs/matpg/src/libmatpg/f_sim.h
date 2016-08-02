#ifndef H_MATPG_F_SIM
#define H_MATPG_F_SIM

/*
 *	matpg - Mat's Automatic Testpattern Generator
 *	10/5/91 by Mat.
 *	$Log: f_sim.h,v $
 * Revision 2.1  1992/01/22  23:49:14  yusuke
 * no difference
 *
 * Revision 2.1  1992/01/22  23:49:14  yusuke
 * no difference
 *
 * Revision 2.0  91/12/21  18:50:36  yusuke
 * '91 Cristmas version
 * 
 * Revision 1.5  1991/10/05  08:18:18  yusuke
 * add Log and RCSid for RCS
 *
 */

#include "matpg.h"


BEGIN_NAMESPACE_YM_MATPG

class fault_t;
class testpat_t;

extern
void
fault_sim(testpat_t** tps,
	  int bitlen,
	  vector<list<fault_t*> >& def_faults);

END_NAMESPACE_YM_MATPG

#endif
