#ifndef H_MATPG_SENSE
#define H_MATPG_SENSE

/*
 *	matpg - Mat's Automatic Testpattern Generator
 *	10/5/91 by Mat.
 *	$Log: sense.h,v $
 * Revision 2.1  1992/01/22  23:49:14  yusuke
 * no difference
 *
 * Revision 2.1  1992/01/22  23:49:14  yusuke
 * no difference
 *
 * Revision 2.0  91/12/21  18:57:07  yusuke
 * '91 Cristmas version
 * 
 * Revision 1.5  1991/10/05  08:18:18  yusuke
 * add Log and RCSid for RCS
 *
 */

#include "matpg.h"

BEGIN_NAMESPACE_YM_MATPG

class gate_t;

extern void sensitize(gate_t* f_node);

END_NAMESPACE_YM_MATPG

#endif
