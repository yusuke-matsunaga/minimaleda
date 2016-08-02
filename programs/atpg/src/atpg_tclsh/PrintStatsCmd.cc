
/// @file atpg/src/atpg_tclsh/PrintStatsCmd.cc
/// @brief ランダムパタン生成を行うコマンド
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: PrintStatsCmd.cc 2128 2009-03-12 07:25:33Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "PrintStatsCmd.h"
#include "AtpgMgr.h"
#include "FaultMgr.h"
#include "TvMgr.h"


BEGIN_NAMESPACE_YM_ATPG

//////////////////////////////////////////////////////////////////////
// 情報を表示するコマンド
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
PrintStatsCmd::PrintStatsCmd(AtpgMgr* mgr) :
  AtpgCmd(mgr)
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
  MStopWatch timer = mgr()._timer();
  USTime r_time = timer.time(TM_READ) + timer.time(TM_MISC);
  USTime d_time = timer.time(TM_DTPG);
  USTime f_time = timer.time(TM_FSIM);
  USTime s_time = timer.time(TM_SAT);

  FaultMgr& fmgr = _fault_mgr();
  size_t num = mgr()._tv_list().size();
  
  fprintf(stdout, "#A: # of total faults       = %7d\n", fmgr.all_rep_num());
  fprintf(stdout, "#B: # of detected faults    = %7d\n", fmgr.det_num());
  fprintf(stdout, "#C: # of redundant faults   = %7d\n", fmgr.untest_num());
  fprintf(stdout, "#D: # of undetected faults  = %7d\n", fmgr.remain_num());
  fprintf(stdout, "#E: # of generated patterns = %7d\n", num);
  fprintf(stdout, "#F:   Total CPU time        = %7.2fu %7.2fs\n",
	  lap.usr_time(), lap.sys_time());
  fprintf(stdout, "#G:    (read time           = %7.2f)\n",
	  r_time.usr_time());
  fprintf(stdout, "#H:    (dtpg time           = %7.2f)\n",
	  d_time.usr_time());
  fprintf(stdout, "#I:    (fsim time           = %7.2f)\n",
	  f_time.usr_time());
  fprintf(stdout, "#J:    (sat  time           = %7.2f)\n",
	  s_time.usr_time());
  
  return TCL_OK;
}

END_NAMESPACE_YM_ATPG
