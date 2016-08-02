
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
#include "BNetInfoCmd.h"
#include "BNetIoCmd.h"
#include "BNetElimCmd.h"
#include "BNetNdCmd.h"


BEGIN_NAMESPACE_MAGUS

// Magus の初期化関数
int
bnet_init(Tcl_Interp* interp,
	  MagMgr* mgr)
{
  //////////////////////////////////////////////////////////////////////
  // コマンドの登録
  //////////////////////////////////////////////////////////////////////

  // ネットワークの諸元を取り出すコマンド
  TclCmdBinder1<BNetInfo, MagMgr*>::reg(interp, mgr,   "magus::network_info");
  TclCmdBinder1<BNetAllInfo, MagMgr*>::reg(interp, mgr, "magus::network_allinfo");
  TclCmdBinder1<BNetPrintStats, MagMgr*>::reg(interp, mgr, "magus::print_stats");

  // ネットワークのファイル入出力コマンド
  TclCmdBinder1<ReadBlif, MagMgr*>::reg(interp, mgr, "magus::read_blif");
  TclCmdBinder1<ReadIscas89, MagMgr*>::reg(interp, mgr, "magus::read_iscas89");
  TclCmdBinder1<WriteBlif, MagMgr*>::reg(interp, mgr, "magus::write_blif");
  TclCmdBinder1<WriteEqu, MagMgr*>::reg(interp, mgr, "magus::write_equ");
  TclCmdBinder1<WriteVerilog, MagMgr*>::reg(interp, mgr, "magus::write_verilog");

  // ネットワーク上での処理コマンド(その2)
  TclCmdBinder1<ElimCmd, MagMgr*>::reg(interp, mgr,    "magus::eliminate");
  TclCmdBinder1<SimplifyCmd, MagMgr*>::reg(interp, mgr, "magus::simplify");
  TclCmdBinder1<SweepCmd, MagMgr*>::reg(interp, mgr,      "magus::sweep");
  TclCmdBinder1<CleanCmd, MagMgr*>::reg(interp, mgr,      "magus::clean_up");
  TclCmdBinder1<DecompCmd, MagMgr*>::reg(interp, mgr,     "magus::decomp");
  TclCmdBinder1<GateDecompCmd, MagMgr*>::reg(interp, mgr, "magus::gate_decomp");
  TclCmdBinder1<SopDecompCmd, MagMgr*>::reg(interp, mgr,  "magus::sop_decomp");
  TclCmdBinder1<ConvCombiCmd, MagMgr*>::reg(interp, mgr,  "magus::conv_combi");

  // ノードに対するコマンド
  TclCmdBinder1<NodeInfo, MagMgr*>::reg(interp, mgr,   "magus::node_info");
  TclCmdBinder1<ListNode, MagMgr*>::reg(interp, mgr,   "magus::list_node");
  TclCmdBinder1<ElimNode, MagMgr*>::reg(interp, mgr,   "magus::eliminate_node");
  TclCmdBinder1<DelNode, MagMgr*>::reg(interp, mgr,    "magus::delete_node");
  TclCmdBinder1<SimplifyNode, MagMgr*>::reg(interp, mgr, "magus::simplify_node");


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


  //////////////////////////////////////////////////////////////////////
  // tclreadline 用の処理
  // すごい手抜き
  //////////////////////////////////////////////////////////////////////
  const char* completer =
    "namespace eval tclreadline {\n"
    "namespace eval magus {\n"
    "proc complete(network_info) { t s e l p m } { return \"\" }\n"
    "proc complete(network_allinfo) { t s e l p m } { return \"\" }\n"
    "proc complete(print_stats) { t s e l p m } { return \"\" }\n"
    "proc complete(read_blif) { t s e l p m } { return \"\" }\n"
    "proc complete(read_iscas89) { t s e l p m } { return \"\" }\n"
    "proc complete(write_blif) { t s e l p m } { return \"\" }\n"
    "proc complete(write_equ) { t s e l p m } { return \"\" }\n"
    "proc complete(write_verilog) { t s e l p m } { return \"\" }\n"
    "proc complete(eliminate) { t s e l p m } { return \"\" }\n"
    "proc complete(sweep) { t s e l p m } { return \"\" }\n"
    "proc complete(clean_up) { t s e l p m } { return \"\" }\n"
    "proc complete(decomp) { t s e l p m } { return \"\" }\n"
    "proc complete(gate_decomp) { t s e l p m } { return \"\" }\n"
    "proc complete(sop_decomp) { t s e l p m } { return \"\" }\n"
    "proc complete(node_info) { t s e l p m } { return \"\" }\n"
    "proc complete(list_node) { t s e l p m } { return \"\" }\n"
    "proc complete(eliminate_node) { t s e l p m } { return \"\" }\n"
    "proc complete(delete_node) { t s e l p m } { return \"\" }\n"
    "}\n"
    "}\n";
  if ( Tcl_Eval(interp, completer) == TCL_ERROR ) {
    return TCL_ERROR;
  }

  // 正常終了
  return TCL_OK;
}

END_NAMESPACE_MAGUS
