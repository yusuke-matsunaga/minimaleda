
/// @file atpg/src/matpg/DtpgCmd.cc
/// @brief テストパタン生成を行うコマンド
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: DtpgCmd.cc 2043 2009-02-21 07:08:48Z matsunaga $
///
/// Copyright (C) 2005-2008 Yusuke Matsunaga
/// All rights reserved.


#if HAVE_CONFIG_H
#include "matpg_config.h"
#endif


#include "DtpgCmd.h"
#include "matpg.h"
#include "search_mgr.h"
#include <ym_tclpp/TclPopt.h>


BEGIN_NAMESPACE_YM_MATPG

extern
int verbose_level;

//////////////////////////////////////////////////////////////////////
// テストパタン生成を行うコマンド
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
DtpgCmd::DtpgCmd()
{
  mPoptVerbose = new TclPoptInt(this, "verbose",
				"specify verbose level (0, 1, 2, ...)");
  mPoptFsim = new TclPoptStr(this, "fsim",
			     "specify FSIM mode (none, single, ppsfp)");
  mPoptDyn = new TclPoptStr(this, "dyn",
			    "specify DYN mode (none, once, always)");
  mPoptScope = new TclPopt(this, "scoping",
			   "do scoping");
  mPoptBacktrack = new TclPoptInt(this, "backtrack",
				  "specify backtrack limit");
}

// @brief デストラクタ
DtpgCmd::~DtpgCmd()
{
}

// コマンド処理関数
int
DtpgCmd::cmd_proc(TclObjVector& objv)
{
  size_t objc = objv.size();
  if ( objc != 1 ) {
    print_usage();
    return TCL_ERROR;
  }

  // verbose level の設定
  if ( mPoptVerbose->is_specified() ) {
    verbose_level = mPoptVerbose->val();
  }
  
  // fsim_mode の設定
  FSIM_MODE fsim_mode = FSIM_PARALLEL;
  if ( mPoptFsim->is_specified() ) {
    string str = mPoptFsim->val();
    if ( str == "none" ) {
      fsim_mode = FSIM_NONE;
    }
    else if ( str == "single" ) {
      fsim_mode = FSIM_SINGLE;
    }
    else if ( str == "ppsfp" ) {
      fsim_mode = FSIM_PARALLEL;
    }
    else {
      print_usage();
      return TCL_ERROR;
    }
  }

  // dyn_mode の設定
  DYN_MODE dyn_mode = DYN_NONE;
  if ( mPoptDyn->is_specified() ) {
    string str = mPoptDyn->val();
    if ( str == "none" ) {
      dyn_mode = DYN_NONE;
    }
    else if ( str == "once" ) {
      dyn_mode = DYN_SINGLE;
    }
    else if ( str == "always" ) {
      dyn_mode = DYN_ALL;
    }
    else {
      print_usage();
      return TCL_ERROR;
    }
  }

  // scoping option の設定
  bool scope = mPoptScope->is_specified();
  
  // backtrack limit の設定
  if ( mPoptBacktrack->is_specified() ) {
    int btnum = mPoptBacktrack->val();
    set_backtrack_limit(btnum);
  }
  else {
    set_default_backtrack_limit();
  }

  // GO!
  dtpg(fsim_mode, dyn_mode, scope);
  
  return TCL_OK;
}

END_NAMESPACE_YM_MATPG
