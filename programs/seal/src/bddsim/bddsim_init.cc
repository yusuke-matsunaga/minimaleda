
/// @file src/fsim/fsim_init.cc
/// @brief ネットワーク関連のコマンド初期化
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: bddsim_init.cc 1920 2008-12-20 15:52:42Z matsunaga $
///
/// Copyright (C) 2005-2006 Yusuke Matsunaga
/// All rights reserved.


#if HAVE_CONFIG_H
#include "seal_config.h"
#endif

#include <tcl.h>
#include "ym_tclpp/TclCmd.h"
#include "BddsimCmd.h"


BEGIN_NAMESPACE_YM_SEAL

int
bddsim_init(Tcl_Interp* interp,
	    SealMgr* mgr)
{
  using nsBddsim::BddsimCmd;
  
  //////////////////////////////////////////////////////////////////////
  // コマンドの登録
  //////////////////////////////////////////////////////////////////////

  TclCmdBinder1<BddsimCmd, SealMgr*>::reg(interp, mgr,
					  "seal::bddsim");
  

  //////////////////////////////////////////////////////////////////////
  // tclreadline 用の処理
  // すごい手抜き
  //////////////////////////////////////////////////////////////////////
  const char* completer =
    "namespace eval tclreadline {\n"
    "namespace eval seal {\n"
    "proc complete(bddsim) { t s e l p m } { return \"\" }\n"
    "}\n"
    "}\n";
  if ( Tcl_Eval(interp, completer) == TCL_ERROR ) {
    return TCL_ERROR;
  }
  
  return TCL_OK;
}

END_NAMESPACE_YM_SEAL
