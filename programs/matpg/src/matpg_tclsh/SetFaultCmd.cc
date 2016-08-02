
/// @file atpg/src/matpg/SetFaultCmd.cc
/// @brief ランダムパタン生成を行うコマンド
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: SetFaultCmd.cc 2043 2009-02-21 07:08:48Z matsunaga $
///
/// Copyright (C) 2005-2008 Yusuke Matsunaga
/// All rights reserved.


#if HAVE_CONFIG_H
#include "matpg_config.h"
#endif


#include "SetFaultCmd.h"
#include "matpg.h"
#include "fault.h"
#include <ym_tclpp/TclPopt.h>


BEGIN_NAMESPACE_YM_MATPG

//////////////////////////////////////////////////////////////////////
// 情報を表示するコマンド
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SetFaultCmd::SetFaultCmd()
{
}

// @brief デストラクタ
SetFaultCmd::~SetFaultCmd()
{
}

// コマンド処理関数
int
SetFaultCmd::cmd_proc(TclObjVector& objv)
{
  size_t objc = objv.size();
  if ( objc != 1 ) {
    print_usage();
    return TCL_ERROR;
  }
  
  fault_set_all();
  
  return TCL_OK;
}

END_NAMESPACE_YM_MATPG
