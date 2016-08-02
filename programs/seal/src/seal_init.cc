
/// @file src/seal_init.cc
/// @brief seal のパッケージ初期化
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: seal_init.cc 1920 2008-12-20 15:52:42Z matsunaga $
///
/// Copyright (C) 2005-2006 Yusuke Matsunaga
/// All rights reserved.


#if HAVE_CONFIG_H
#include "seal_config.h"
#endif

#include <tcl.h>
#include <ym_tclpp/TclCmd.h>
#include "SealMgr.h"
#include "SealCmd.h"
#include "calc_cvf/CvfCmd.h"
#include "calc_svf/SvfCmd.h"


BEGIN_NAMESPACE_YM_SEAL

int
make_ntwk_io_commands(Tcl_Interp* interp,
		      SealMgr* mgr,
		      const string& ns);

int
bddsim_init(Tcl_Interp*,
	    SealMgr*);

int
misc_init(Tcl_Interp*);

int
seal_init(Tcl_Interp* interp)
{
  //////////////////////////////////////////////////////////////////////
  // seal という namespace を作っておく．
  //////////////////////////////////////////////////////////////////////
  if ( Tcl_Eval(interp, "namespace eval seal {}") == TCL_ERROR ) {
    return TCL_ERROR;
  }


  //////////////////////////////////////////////////////////////////////
  // サブモジュールの初期化
  //////////////////////////////////////////////////////////////////////

  { // default
    SealMgr* mgr = new SealMgr;
    string ns = "seal";
    make_ntwk_io_commands(interp, mgr, ns);
    make_cvf_commands(interp, mgr, ns);
    make_svf_commands(interp, mgr, ns);
    if ( bddsim_init(interp, mgr) == TCL_ERROR ) {
      return TCL_ERROR;
    }
  }

  if ( misc_init(interp) == TCL_ERROR ) {
    return TCL_ERROR;
  }

  // もしも SEAL_LIBRARY という環境変数が定義されていたらそれを auto_path
  // に加える．そうでなければ configure 時に指定されたパスを auto_path
  // に加える．
  // 同時にそのパスを ::seal::libdir という変数にセットする．
  // SEAL_DATA と SEAL_BIN も同様
  const char* script =
    "namespace eval ::seal { }\n"
    "if [info exists env(SEAL_LIBRARY)] {"
    "    set ::seal::libdir $env(SEAL_LIBRARY)"
    "} else {"
    "    set ::seal::libdir " SEAL_LIBDIR
    "}\n"
    "lappend auto_path $::seal::libdir\n"
    "if [info exists env(SEAL_DATA)] {"
    "    set ::seal::datadir $env(SEAL_DATA)"
    "} else {"
    "    set ::seal::datadir " SEAL_DATADIR
    "}\n"
    "if [info exists env(SEAL_BIN)] {"
    "    set ::seal::bindir $env(SEAL_BIN)"
    "} else {"
    "    set ::seal::bindir " SEAL_BINDIR
    "}\n";
  if (Tcl_Eval(interp, script) != TCL_OK) {
    return TCL_ERROR;
  }
  
  //////////////////////////////////////////////////////////////////////
  // パッケージ宣言
  //////////////////////////////////////////////////////////////////////
  if ( Tcl_PkgProvide(interp, "Seal", SEAL_VERSION) != TCL_OK ) {
    return TCL_ERROR;
  }
  
  return TCL_OK;
}

END_NAMESPACE_YM_SEAL
