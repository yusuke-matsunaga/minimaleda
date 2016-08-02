#ifndef H_MATPG_CMDS
#define H_MATPG_CMDS

/*
 *	matpg - Mat's Automatic Testpattern Generator
 *	10/5/91 by Mat.
 *	$Log: cmds.h,v $
 * Revision 2.1  1992/01/22  23:49:14  yusuke
 * no difference
 *
 * Revision 2.1  1992/01/22  23:49:14  yusuke
 * no difference
 *
 * Revision 2.0  91/12/21  18:49:57  yusuke
 * '91 Cristmas version
 * 
 * Revision 1.5  1991/10/05  08:18:18  yusuke
 * add Log and RCSid for RCS
 *
 */

#include "gen_def.h"

extern int cmd_clr_fault(int, Cchar**);
extern int cmd_dtpg(int, Cchar**);
extern int cmd_rfsim(int, Cchar**);
extern int cmd_print_fault(int, Cchar**);
extern int cmd_print_pattern(int, Cchar**);
extern int cmd_print_stats(int, Cchar**);
extern int cmd_read_blif(int, Cchar**);
extern int cmd_read_lib(int, Cchar**);
extern int cmd_rtpg(int, Cchar**);
extern int cmd_set_fault(int, Cchar**);
extern int cmd_time(int, Cchar**);
extern int cmd_write_blif(int, Cchar**);

#endif
