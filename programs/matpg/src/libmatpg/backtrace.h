#ifndef H_MATPG_BACKTRACE
#define H_MATPG_BACKTRACE

/*
 *	matpg - Mat's Automatic Testpattern Generator
 *	10/5/91 by Mat.
 *	$Log: backtrace.h,v $
 * Revision 2.1  1992/01/22  23:49:14  yusuke
 * no difference
 *
 * Revision 2.1  1992/01/22  23:49:14  yusuke
 * no difference
 *
 * Revision 2.0  91/12/21  18:30:51  yusuke
 * '91 Cristmas version
 * 
 * Revision 1.5  1991/10/05  08:18:18  yusuke
 * add Log and RCSid for RCS
 *
 */

#include "matpg.h"


BEGIN_NAMESPACE_YM_MATPG

class gate_t;

extern bool C_decision(gate_t* f_node);
extern bool O_decision(gate_t* f_node);

END_NAMESPACE_YM_MATPG

#endif
