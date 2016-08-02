
/// @file magus/main/misc_init.cc
/// @brief misc パッケージの初期化を行う．
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: misc_init.cc 2043 2009-02-21 07:08:48Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include <tcl.h>

#include "TimeCmd.h"


BEGIN_NAMESPACE_YM_ATPG

// 初期化関数
int
misc_init(Tcl_Interp* interp)
{
  //////////////////////////////////////////////////////////////////////
  // コマンドの登録
  //////////////////////////////////////////////////////////////////////

  // 時間計測/乱数発生コマンド
  TclCmdBinder<StpwatchCls>::reg(interp, "atpg::stopwatch");
  TclCmdBinder<TimeCmd>::reg(interp,     "atpg::time");

  
  //////////////////////////////////////////////////////////////////////
  // tclreadline 用の処理
  // すごい手抜き
  //////////////////////////////////////////////////////////////////////
  const char* completer =
    "namespace eval tclreadline {\n"
    "namespace eval atpg {\n"
    "proc complete(stopwatch) { t s e l p m } { return \"\" }\n"
    "proc complete(time) { t s e l p m } { return \"\" }\n"
    "}\n"
    "}\n";
  if ( Tcl_Eval(interp, completer) == TCL_ERROR ) {
    return TCL_ERROR;
  }

  // 正常終了
  return TCL_OK;
}

END_NAMESPACE_YM_ATPG
