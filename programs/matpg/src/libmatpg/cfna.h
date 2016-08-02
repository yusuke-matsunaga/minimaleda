#ifndef H_MATPG_CFNA
#define H_MATPG_CFNA

/*
 *	matpg - Mat's Automatic Testpattern Generator
 *	10/5/91 by Mat.
 *	$Log: cfna.h,v $
 * Revision 2.2  1992/01/22  23:49:14  yusuke
 * no difference
 *
 * Revision 2.2  1992/01/22  23:49:14  yusuke
 * no difference
 *
 * Revision 2.1  91/12/28  16:58:38  yusuke
 * Final , Final revision
 * 
 * Revision 2.0  91/12/21  18:49:27  yusuke
 * '91 Cristmas version
 * 
 * Revision 1.5  1991/10/05  08:18:18  yusuke
 * add Log and RCSid for RCS
 *
 */

#include "matpg.h"

BEGIN_NAMESPACE_YM_MATPG

class gate_t;
class testpat_t;

extern void cfna_init();
extern void cfna_erase();
extern void cfna_clr();
extern void cfna_add(gate_t* gate);
extern void cfna_recpat(testpat_t* tp);


END_NAMESPACE_YM_MATPG

#endif
