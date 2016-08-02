
/// @file magus/src/logbase/logbase_init.cc
/// @brief コアパッケージの初期化を行う．
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: GateObj.cc 347 2007-01-16 02:37:45Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include <tcl.h>

#include "MagMgr.h"
#include "MvnReadVerilog.h"


BEGIN_NAMESPACE_MAGUS

// Magus の初期化関数
int
mvn_init(Tcl_Interp* interp,
	 MagMgr* mgr)
{
  //////////////////////////////////////////////////////////////////////
  // コマンドの登録
  //////////////////////////////////////////////////////////////////////

  // ネットワークのファイル入出力コマンド
  TclCmdBinder1<MvnReadVerilog, MagMgr*>::reg(interp, mgr,
					      "magus::mvn::read_verilog");

  //////////////////////////////////////////////////////////////////////
  // tclreadline 用の処理
  // すごい手抜き
  //////////////////////////////////////////////////////////////////////
  const char* completer =
    "namespace eval tclreadline {\n"
    "namespace eval magus {\n"
    "namespace eval mvn {\n"
    "proc complete(read_verilog) { t s e l p m } { return \"\" }\n"
    "}\n"
    "}\n"
    "}\n";
  if ( Tcl_Eval(interp, completer) == TCL_ERROR ) {
    return TCL_ERROR;
  }

  // 正常終了
  return TCL_OK;
}

END_NAMESPACE_MAGUS
