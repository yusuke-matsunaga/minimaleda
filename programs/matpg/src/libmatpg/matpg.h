#ifndef H_MATPG
#define H_MATPG

/*
 *	matpg - Mat's Automatic Testpattern Generator
 *	10/5/91 by Mat.
 *	$Log: matpg.h,v $
 * Revision 2.1  1992/01/22  23:49:14  yusuke
 * no difference
 *
 * Revision 2.1  1992/01/22  23:49:14  yusuke
 * no difference
 *
 * Revision 2.0  91/12/21  18:53:46  yusuke
 * '91 Cristmas version
 * 
 * Revision 1.5  1991/10/05  08:18:18  yusuke
 * add Log and RCSid for RCS
 *
 */

#include "matpg_nsdef.h"
#include <ym_utils/StopWatch.h>


BEGIN_NAMESPACE_YM_MATPG

#ifdef DEC_UCB
extern "C" void srandom(long);
extern "C" long random();
#endif

extern int verbose_level;
extern FILE* ver_fp;

extern bool log_flag;
extern FILE* log_fp;

extern MStopWatch sw_timer;

enum {
  TM_MISC = 0,
  TM_READ = 1,
  TM_DTPG = 2,
  TM_FSIM = 3
};

enum FSIM_MODE { FSIM_NONE, FSIM_SINGLE, FSIM_PARALLEL };
enum BT_MODE { BT_D, BT_PODEM };
enum DYN_MODE { DYN_NONE, DYN_SINGLE, DYN_ALL };

extern void rtpg(size_t min_f, size_t max_i, size_t max_pat, FILE* fp);
extern void rfsim();
extern void dtpg(FSIM_MODE f_mode, DYN_MODE dyn, bool scope);
extern void set_backtrace_mode(BT_MODE bt_mode);

inline
int
get_verbose_level()
{
  return verbose_level;
}

inline
void
FATAL(const char* msg)
{
  cout << msg << endl;
  assert_not_reached(__FILE__, __LINE__);
}

END_NAMESPACE_YM_MATPG

#endif
