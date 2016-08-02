
/// @file atpg/src/matpg/PrintPatCmd.cc
/// @brief ランダムパタン生成を行うコマンド
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: PrintPatCmd.cc 2043 2009-02-21 07:08:48Z matsunaga $
///
/// Copyright (C) 2005-2008 Yusuke Matsunaga
/// All rights reserved.


#if HAVE_CONFIG_H
#include "matpg_config.h"
#endif


#include "PrintPatCmd.h"
#include "matpg.h"
#include "testpat.h"
#include <ym_tclpp/TclPopt.h>


BEGIN_NAMESPACE_YM_MATPG

//////////////////////////////////////////////////////////////////////
// 情報を表示するコマンド
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
PrintPatCmd::PrintPatCmd()
{
  mPoptHex = new TclPopt(this, "hex",
			 "print in hex mode");
  mPoptNum = new TclPopt(this, "num",
			 "print with index numbering");
}

// @brief デストラクタ
PrintPatCmd::~PrintPatCmd()
{
}

// コマンド処理関数
int
PrintPatCmd::cmd_proc(TclObjVector& objv)
{
  size_t objc = objv.size();
  if ( objc != 1 ) {
    print_usage();
    return TCL_ERROR;
  }
  
  bool hex_flag = mPoptHex->is_specified();
  bool num_flag = mPoptNum->is_specified();

  tp_print(stdout, num_flag, hex_flag);

  return TCL_OK;
  
  return TCL_OK;
}

END_NAMESPACE_YM_MATPG
