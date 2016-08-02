
/// @file atpg/src/RtpgCmd.cc
/// @brief 故障シミュレーションを行うコマンド
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: RtpgCmd.cc 2128 2009-03-12 07:25:33Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "RtpgCmd.h"
#include <ym_tclpp/TclPopt.h>


BEGIN_NAMESPACE_YM_ATPG

//////////////////////////////////////////////////////////////////////
// 故障シミュレーションを実行するコマンド
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
RtpgCmd::RtpgCmd(AtpgMgr* mgr) :
  AtpgCmd(mgr),
  mRtpg(mgr)
{
  mPoptNpat = new TclPoptInt(this, "npat",
			     "specify number of patterns to be simulated");
  mPoptMin = new TclPoptInt(this, "min_fault",
			    "specify the minimum limit of detected faults at a time");
  mPoptMax = new TclPoptInt(this, "max_interval",
			    "specify the maximum limit of non-detecting interval");
  mPoptSeed = new TclPoptUint(this, "seed",
			      "sepcify the random seed");
  mPoptFile = new TclPoptStr(this, "file",
			     "specify the file name containg patterns");
  mPoptPrintStats = new TclPopt(this, "print_stats",
				"print statistics");
}

// @brief デストラクタ
RtpgCmd::~RtpgCmd()
{
}

// コマンド処理関数
int
RtpgCmd::cmd_proc(TclObjVector& objv)
{
  size_t objc = objv.size();
  if ( objc != 1 ) {
    print_usage();
    return TCL_ERROR;
  }
  
  bool n_flag = false;
  ymuint max_pat = 10000;
  ymuint max_i = 4;
  ymuint min_f = 0;

  if ( mPoptNpat->is_specified() ) {
    max_pat = mPoptNpat->val();
    n_flag = true;
  }

  if ( mPoptMin->is_specified() ) {
    min_f = mPoptMin->val();
  }

  if ( mPoptMax->is_specified() ) {
    max_i = mPoptMax->val();
  }

  if ( mPoptSeed->is_specified() ) {
    // 未実装
  }

  if ( mPoptFile->is_specified() ) {
    // 未実装
  }
  
  bool print_stats = mPoptPrintStats->is_specified();
  
  if ( n_flag ) {
    min_f = 0;
    max_i = 0;
  }
  
  mRtpg(min_f, max_i, max_pat);
  
  after_update_faults();

  if ( print_stats ) {
    const RtpgStats& stats = mRtpg.stats();
    cout << "********** rtpg **********" << endl
	 << setw(10) << stats.detected_faults()
	 << ": # of detected faults" << endl
	 << setw(10) << stats.simulated_patterns()
	 << ": # of simulated patterns" << endl
	 << setw(10) << stats.effective_patterns()
	 << ": # of effective patterns" << endl
	 << stats.time() << endl;
  }
  
  return TCL_OK;
}

END_NAMESPACE_YM_ATPG
