
/// @file atpg/src/matpg/RfsimCmd.cc
/// @brief ランダムパタン生成を行うコマンド
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: RfsimCmd.cc 2043 2009-02-21 07:08:48Z matsunaga $
///
/// Copyright (C) 2005-2008 Yusuke Matsunaga
/// All rights reserved.


#if HAVE_CONFIG_H
#include "matpg_config.h"
#endif


#include "RfsimCmd.h"
#include "matpg.h"
#include <ym_tclpp/TclPopt.h>


BEGIN_NAMESPACE_YM_MATPG

//////////////////////////////////////////////////////////////////////
// 逆順故障シミュレーションを行うコマンド
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
RfsimCmd::RfsimCmd()
{
}

// @brief デストラクタ
RfsimCmd::~RfsimCmd()
{
}

// コマンド処理関数
int
RfsimCmd::cmd_proc(TclObjVector& objv)
{
  size_t objc = objv.size();
  if ( objc != 1 ) {
    print_usage();
    return TCL_ERROR;
  }
  
  // GO!
  rfsim();
  
  return TCL_OK;
}

END_NAMESPACE_YM_MATPG
