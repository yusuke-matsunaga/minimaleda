
/// @file src/core/core_init.cc
/// @brief コアパッケージの初期化を行う．
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: GateObj.cc 347 2007-01-16 02:37:45Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include <tcl.h>

#include "NewBNetwork.h"
#include "NewBdn.h"
#include "NewMvn.h"
#include "ClrNetwork.h"
#include "DelNetwork.h"
#include "CopyNetwork.h"
#include "ChgNetwork.h"
#include "PushNetwork.h"
#include "PopNetwork.h"
#include "CurNetwork.h"
#include "ListNetwork.h"
#include "ForNetwork.h"


BEGIN_NAMESPACE_MAGUS

// Magus の初期化関数
int
core_init(Tcl_Interp* interp,
	  MagMgr* mgr)
{
  //////////////////////////////////////////////////////////////////////
  // コマンドの登録
  //////////////////////////////////////////////////////////////////////

  // ネットワークの生成／削除／複写コマンド
  TclCmdBinder1<NewBNetwork, MagMgr*>::reg(interp, mgr,  "magus::new_bnetwork");
  TclCmdBinder1<NewBdn, MagMgr*>::reg(interp, mgr,  "magus::new_bdnetwork");
  TclCmdBinder1<NewMvn, MagMgr*>::reg(interp, mgr,  "magus::new_mvnetwork");
  TclCmdBinder1<DelNetwork, MagMgr*>::reg(interp, mgr,  "magus::delete_network");
  TclCmdBinder1<CopyNetwork, MagMgr*>::reg(interp, mgr, "magus::::copy_network");
  TclCmdBinder1<ClrNetwork, MagMgr*>::reg(interp, mgr,  "magus::::clear_network");

  // 操作対象のネットワークを指定するコマンド
  TclCmdBinder1<PushNetwork, MagMgr*>::reg(interp, mgr, "magus::push_current_network");
  TclCmdBinder1<PopNetwork, MagMgr*>::reg(interp, mgr,  "magus::pop_current_network");
  TclCmdBinder1<ChgNetwork, MagMgr*>::reg(interp, mgr, "magus::change_current_network");
  TclCmdBinder1<CurNetwork, MagMgr*>::reg(interp, mgr,  "magus::current_network");

  // ネットワーク名の列挙を行うコマンド
  TclCmdBinder1<ListNetwork, MagMgr*>::reg(interp, mgr, "magus::network_list");
  TclCmdBinder1<ForNetwork, MagMgr*>::reg(interp, mgr,  "magus::foreach_network");


  //////////////////////////////////////////////////////////////////////
  // デフォルト値を入れる変数の初期化
  //////////////////////////////////////////////////////////////////////
  // 直接 Tcl のスクリプトを実行させる．
  const char* const script =
    "set ::magus::default(arrival_slope)  0.0\n"
    "set ::magus::default(slew_slope)     0.0\n"
    "set ::magus::default(required_slope) 0.0\n"
    "set ::magus::default(fanout_no)        1\n"
    "set ::magus::default(timescale)  1.0e-12\n"
    "set ::magus::default(elim_limit) 1000\n"
    "set ::magus::default(elim_autolimit) true\n"
    "set ::magus::default(xsim_pat_num) 1000\n";

  if ( Tcl_Eval(interp, (char*) script) != TCL_OK ) {
    return TCL_ERROR;
  }

  // デフォルトネットワークを作成する．
  if ( Tcl_Eval(interp, "magus::new_bnetwork default_network")
       == TCL_ERROR ) {
    return TCL_ERROR;
  }

  // それをカレントネットワークにする．
  if ( Tcl_Eval(interp, "magus::push_current_network default_network")
      == TCL_ERROR ) {
    return TCL_ERROR;
  }


  //////////////////////////////////////////////////////////////////////
  // tclreadline 用の処理
  // すごい手抜き
  //////////////////////////////////////////////////////////////////////
  const char* completer =
    "namespace eval tclreadline {\n"
    "namespace eval magus {\n"
    "proc complete(new_bnetwork) { t s e l p m } { return \"\" }\n"
    "proc complete(new_bdnetwork) { t s e l p m } { return \"\" }\n"
    "proc complete(new_mvnetwork) { t s e l p m } { return \"\" }\n"
    "proc complete(delete_network) { t s e l p m } { return \"\" }\n"
    "proc complete(copy_network) { t s e l p m } { return \"\" }\n"
    "proc complete(clear_network) { t s e l p m } { return \"\" }\n"
    "proc complete(push_current_network) { t s e l p m } { return \"\" }\n"
    "proc complete(pop_current_network) { t s e l p m } { return \"\" }\n"
    "proc complete(change_current_network) { t s e l p m } { return \"\" }\n"
    "proc complete(current_network) { t s e l p m } { return \"\" }\n"
    "proc complete(network_list) { t s e l p m } { return \"\" }\n"
    "proc complete(foreach_network) { t s e l p m } { return \"\" }\n"
    "}\n"
    "}\n";
  if ( Tcl_Eval(interp, completer) == TCL_ERROR ) {
    return TCL_ERROR;
  }

  // 正常終了
  return TCL_OK;
}

END_NAMESPACE_MAGUS
