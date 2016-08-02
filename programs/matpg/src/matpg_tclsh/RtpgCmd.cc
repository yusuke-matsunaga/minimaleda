
/// @file atpg/src/matpg/RtpgCmd.cc
/// @brief ランダムパタン生成を行うコマンド
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: RtpgCmd.cc 2043 2009-02-21 07:08:48Z matsunaga $
///
/// Copyright (C) 2005-2008 Yusuke Matsunaga
/// All rights reserved.


#if HAVE_CONFIG_H
#include "matpg_config.h"
#endif


#include "RtpgCmd.h"
#include "matpg.h"
#include <ym_tclpp/TclPopt.h>


BEGIN_NAMESPACE_YM_MATPG

//////////////////////////////////////////////////////////////////////
// ランダムパタン生成を行うコマンド
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
RtpgCmd::RtpgCmd()
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
  int max_pat = 10000;
  int max_i = 4;
  int min_f = 0;

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

  if ( n_flag ) {
    min_f = 0;
    max_i = -1;
  }
  
  // GO!
  rtpg(min_f, max_i, max_pat, NULL);
  
  return TCL_OK;
}

END_NAMESPACE_YM_MATPG
