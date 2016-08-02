
/// @file magus/src/simplify/simplify_init.cc
/// @brief Simplify パッケージの初期化ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: simplify_init.cc 1978 2009-02-06 12:29:16Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "LrwCmd.h"


BEGIN_NAMESPACE_MAGUS

int
simplify_init(Tcl_Interp* interp,
	      MagMgr* mgr)
{
  using nsSimplify::LrwCmd;
  
  TclCmdBinder1<nsSeal::LrCmd, MagMgr*>::reg(interp, mgr, "magus::local_rewrite");

  const char* init =
    "namespace eval tclreadline {\n"
    "namespace eval magus {\n"
    "proc complete(local_rewrite) { text start end line pos mod } { return \"\" }\n"
    "}\n"
    "}\n";

  if ( Tcl_Eval(interp, (char*) init) == TCL_ERROR ) {
    return TCL_ERROR;
  }

  return TCL_OK;

}

END_NAMESPACE_MAGUS
