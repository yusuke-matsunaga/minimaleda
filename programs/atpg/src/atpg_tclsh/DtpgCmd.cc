
/// @file atpg/src/atpg_tclsh/DtpgCmd.cc
/// @brief テストパタン生成を行うコマンド
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: DtpgCmd.cc 2203 2009-04-16 05:04:40Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "DtpgCmd.h"
#include "ym_tclpp/TclPopt.h"


BEGIN_NAMESPACE_YM_ATPG

//////////////////////////////////////////////////////////////////////
// テストパタン生成を行うコマンド
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
DtpgCmd::DtpgCmd(AtpgMgr* mgr) :
  AtpgCmd(mgr),
  mDtpg(mgr)
{
  mPoptVerbose = new TclPoptInt(this, "verbose",
				"specify verbose level (0, 1, 2, ...)");
  mPoptSat = new TclPopt(this, "sat",
			 "SAT mode");
  mPoptSatRec = new TclPopt(this, "satrec",
			 "SATREC mode");
  mPoptMiniSat = new TclPopt(this, "minisat",
			     "MINISAT mode");
  mPoptFsim = new TclPoptStr(this, "fsim",
			     "specify FSIM mode (none, single, ppsfp)");
  mPoptDyn = new TclPoptStr(this, "dyn",
			    "specify DYN mode (none, once, always)");
  mPoptScope = new TclPopt(this, "scoping",
			   "do scoping");
  mPoptBacktrack = new TclPoptInt(this, "backtrack",
				  "specify backtrack limit");
  mPoptPrintStats = new TclPopt(this, "print_stats",
				"print statistics");
  new_popt_group(mPoptSat, mPoptMiniSat, mPoptSatRec);
}

// @brief デストラクタ
DtpgCmd::~DtpgCmd()
{
}

// コマンド処理関数
int
DtpgCmd::cmd_proc(TclObjVector& objv)
{
  using nsDtpgCl::tDynMode;
  using nsDtpgCl::kDynNone;
  using nsDtpgCl::kDynOnce;
  using nsDtpgCl::kDynAlways;
  
  size_t objc = objv.size();
  if ( objc != 1 ) {
    print_usage();
    return TCL_ERROR;
  }

  // verbose level の設定
  if ( mPoptVerbose->is_specified() ) {
    int verbose_level = mPoptVerbose->val();
    mDtpg.set_verbose_level(verbose_level);
  }
  
  // SAT mode の設定
  if ( mPoptSat->is_specified() ) {
    mDtpg.set_sat(true);
    mDtpg.set_sat_mode(0);
  }
  else if ( mPoptSatRec->is_specified() ) {
    mDtpg.set_sat(true);
    mDtpg.set_sat_mode(1);
  }
  else if ( mPoptMiniSat->is_specified() ) {
    mDtpg.set_sat(true);
    mDtpg.set_sat_mode(2);
  }
  
  // fsim_mode の設定
  tSimMode fsim_mode = kSimPpsfp;
  if ( mPoptFsim->is_specified() ) {
    string str = mPoptFsim->val();
    if ( str == "none" ) {
      fsim_mode = kSimNone;
    }
    else if ( str == "single" ) {
      fsim_mode = kSimSingle;
    }
    else if ( str == "ppsfp" ) {
      fsim_mode = kSimPpsfp;
    }
    else {
      print_usage();
      return TCL_ERROR;
    }
  }

  // dyn_mode の設定
  tDynMode dyn_mode = kDynNone;
  if ( mPoptDyn->is_specified() ) {
    string str = mPoptDyn->val();
    if ( str == "none" ) {
      dyn_mode = kDynNone;
    }
    else if ( str == "once" ) {
      dyn_mode = kDynOnce;
    }
    else if ( str == "always" ) {
      dyn_mode = kDynAlways;
    }
    else {
      print_usage();
      return TCL_ERROR;
    }
  }
  mDtpg.set_dyn_mode(dyn_mode);

  // scoping option の設定
  bool scoping = mPoptScope->is_specified();
  mDtpg.set_scoping(scoping);
  
  // backtrack limit の設定
  if ( mPoptBacktrack->is_specified() ) {
    int btnum = mPoptBacktrack->val();
    mDtpg.set_backtrack_limit(btnum);
  }
  else {
    mDtpg.set_default_backtrack_limit();
  }
  
  bool print_stats = mPoptPrintStats->is_specified();
  
  // GO!
  mDtpg(fsim_mode);
  
  after_update_faults();

  if ( print_stats ) {
    const DtpgStats& stats = mDtpg.stats();
    cout << "********** dtpg **********" << endl
	 << setw(10) << stats.detected_faults()
	 << ": # of detected faults" << endl
	 << setw(10) << stats.untestable_faults()
	 << ": # fo redundant faults" << endl
	 << setw(10) << stats.aborted_faults()
	 << ": # of aborted faults" << endl
	 << setw(10) << stats.generated_patterns()
	 << ": # of generated patterns" << endl
	 << setw(10) << stats.call_count()
	 << ": # of dtpg runs" << endl
	 << setw(10) << stats.backtrack_count()
	 << ": # of total backtracks" << endl
	 << stats.time() << endl;
  }
  
  return TCL_OK;
}

END_NAMESPACE_YM_ATPG
