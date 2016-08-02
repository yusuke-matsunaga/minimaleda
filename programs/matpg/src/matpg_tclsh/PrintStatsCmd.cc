
/// @file atpg/src/matpg/PrintStatsCmd.cc
/// @brief ランダムパタン生成を行うコマンド
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: PrintStatsCmd.cc 2043 2009-02-21 07:08:48Z matsunaga $
///
/// Copyright (C) 2005-2008 Yusuke Matsunaga
/// All rights reserved.


#if HAVE_CONFIG_H
#include "matpg_config.h"
#endif


#include "PrintStatsCmd.h"
#include "matpg.h"
#include "fault.h"
#include "testpat.h"


BEGIN_NAMESPACE_YM_MATPG

extern
MStopWatch sw_timer;

//////////////////////////////////////////////////////////////////////
// 情報を表示するコマンド
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
PrintStatsCmd::PrintStatsCmd()
{
  mStopWatch.start();
}

// @brief デストラクタ
PrintStatsCmd::~PrintStatsCmd()
{
}

// コマンド処理関数
int
PrintStatsCmd::cmd_proc(TclObjVector& objv)
{
  size_t objc = objv.size();
  if ( objc != 1 ) {
    print_usage();
    return TCL_ERROR;
  }
  
  USTime lap = mStopWatch.time();
  USTime r_time = sw_timer.time(TM_READ) + sw_timer.time(TM_MISC);
  USTime d_time = sw_timer.time(TM_DTPG);
  USTime f_time = sw_timer.time(TM_FSIM);

  fprintf(stdout, "#A: # of total faults       = %7d\n", fault_w_num());
  fprintf(stdout, "#B: # of detected faults    = %7d\n", fault_d_num());
  fprintf(stdout, "#C: # of redundant faults   = %7d\n", fault_r_num());
  fprintf(stdout, "#D: # of undetected faults  = %7d\n", fault_u_num());
  fprintf(stdout, "#E: # of generated patterns = %7d\n", tp_get_num());
  fprintf(stdout, "#F:   Total CPU time        = %7.2fu %7.2fs\n",
	  lap.usr_time(), lap.sys_time());
  fprintf(stdout, "#G:    (read time           = %7.2f)\n",
	  r_time.usr_time());
  fprintf(stdout, "#H:    (dtpg time           = %7.2f)\n",
	  d_time.usr_time());
  fprintf(stdout, "#I:    (fsim time           = %7.2f)\n",
	  f_time.usr_time());
  
  return TCL_OK;
}

END_NAMESPACE_YM_MATPG
