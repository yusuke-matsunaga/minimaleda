
/// @file magus/src/seal/seal_init.cc
/// @brief Seal パッケージの初期化ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: seal_init.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "SealCmd.h"
#include "TMRCmd.h"
#include "DupCmd.h"
#include "McaCmd.h"
#include "McaCmd2.h"


BEGIN_NAMESPACE_MAGUS

int
seal_init(Tcl_Interp* interp,
	  MagMgr* mgr)
{
  TclCmdBinder1<SealCmd, MagMgr*>::reg(interp, mgr, "magus::seal");
  TclCmdBinder1<TMRCmd, MagMgr*>::reg(interp, mgr, "magus::tmr");
  TclCmdBinder1<DupCmd, MagMgr*>::reg(interp, mgr, "magus::dup_node");
  TclCmdBinder1<McaCmd, MagMgr*>::reg(interp, mgr, "magus::mc_analysis");
  TclCmdBinder1<McaCmd2, MagMgr*>::reg(interp, mgr, "magus::mc_analysis2");

  const char* init =
    "namespace eval tclreadline {\n"
    "namespace eval magus {\n"
    "proc complete(seal) { text start end line pos mod } { return \"\" }\n"
    "proc complete(tmr) { text start end line pos mod } { return \"\" }\n"
    "proc complete(dup_node) { text start end line pos mod } { return \"\" }\n"
    "proc complete(mc_analysis) { text start end line pos mod } { return \"\" }\n"
    "proc complete(mc_analysis2) { text start end line pos mod } { return \"\" }\n"
    "}\n"
    "}\n";

  if ( Tcl_Eval(interp, (char*) init) == TCL_ERROR ) {
    return TCL_ERROR;
  }

  return TCL_OK;

}

END_NAMESPACE_MAGUS
