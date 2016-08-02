#ifndef H_MATPG_PGraph
#define H_MATPG_PGraph

/*
 *	matpg - Mat's Automatic Testpattern Generator
 *	10/5/91 by Mat.
 *	$Log: pgraph.h,v $
 * Revision 2.1  1992/01/22  23:49:14  yusuke
 * no difference
 *
 * Revision 2.1  1992/01/22  23:49:14  yusuke
 * no difference
 *
 * Revision 2.0  91/12/21  18:55:26  yusuke
 * '91 Cristmas version
 * 
 * Revision 1.5  1991/10/05  08:18:18  yusuke
 * add Log and RCSid for RCS
 *
 */

#include "matpg.h"


BEGIN_NAMESPACE_YM_MATPG

class gate_t;

extern void pgraph_construct(gate_t* gate);
extern void pgraph_destruct();
extern void pgraph_clr_changes();
extern bool pgraph_chk_changes();
extern bool pgraph_update();
extern void pgraph_add_indif(gate_t* gate);
extern void pgraph_clr_indif();
extern void pgraph_dump(FILE* fp);

END_NAMESPACE_YM_MATPG

#endif
