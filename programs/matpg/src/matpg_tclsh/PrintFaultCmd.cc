
/// @file atpg/src/matpg/PrintFaultCmd.cc
/// @brief ランダムパタン生成を行うコマンド
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: PrintFaultCmd.cc 2043 2009-02-21 07:08:48Z matsunaga $
///
/// Copyright (C) 2005-2008 Yusuke Matsunaga
/// All rights reserved.


#if HAVE_CONFIG_H
#include "matpg_config.h"
#endif


#include "PrintFaultCmd.h"
#include "matpg.h"
#include "fault.h"
#include <ym_tclpp/TclPopt.h>


BEGIN_NAMESPACE_YM_MATPG

//////////////////////////////////////////////////////////////////////
// 情報を表示するコマンド
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
PrintFaultCmd::PrintFaultCmd()
{
  mPoptType = new TclPoptStr(this, "type",
			     "spefify fault type "
			     "(detected, untestable, remain)");
}

// @brief デストラクタ
PrintFaultCmd::~PrintFaultCmd()
{
}

// コマンド処理関数
int
PrintFaultCmd::cmd_proc(TclObjVector& objv)
{
  size_t objc = objv.size();
  if ( objc != 1 ) {
    print_usage();
    return TCL_ERROR;
  }

  FTYPE ftype = FTYPE_U;
  if ( mPoptType->is_specified() ) {
    string tmp = mPoptType->val();
    if ( tmp == "detected" ) {
      ftype = FTYPE_D;
    }
    else if ( tmp == "untestable" ) {
      ftype = FTYPE_R;
    }
    else if ( tmp == "remain" ) {
      ftype = FTYPE_U;
    }
    else {
      print_usage();
    }
  }
  
  fault_print(stdout, ftype);
  
  return TCL_OK;
}

END_NAMESPACE_YM_MATPG
