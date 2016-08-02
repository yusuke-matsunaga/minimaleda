#ifndef H_MATPG_LEARN
#define H_MATPG_LEARN

/*
 *	matpg - Mat's Automatic Testpattern Generator
 *	10/5/91 by Mat.
 *	$Log: learn.h,v $
 * Revision 2.2  1992/01/22  23:49:14  yusuke
 * no difference
 *
 * Revision 2.2  1992/01/22  23:49:14  yusuke
 * no difference
 *
 * Revision 2.1  91/12/23  23:11:36  yusuke
 * a slightly speed-up
 * 
 * Revision 2.0  91/12/21  18:52:47  yusuke
 * '91 Cristmas version
 * 
 * Revision 1.5  1991/10/05  08:18:18  yusuke
 * add Log and RCSid for RCS
 *
 */

#include "matpg.h"

BEGIN_NAMESPACE_YM_MATPG

class gate_t;

extern void learning();
extern void forget_learning();
extern void use_learning();
extern void dont_use_learning();
extern void push_learn_flag();
extern void pop_learn_flag();

END_NAMESPACE_YM_MATPG

#endif
