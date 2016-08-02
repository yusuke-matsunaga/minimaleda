/*
 *	matpg - Mat's Automatic Testpattern Generator
 *	10/5/91 by Mat.
 *	$Log: matpg.c,v $
 * Revision 2.2  1992/01/22  23:49:14  yusuke
 * no difference
 *
 * Revision 2.2  1992/01/22  23:49:14  yusuke
 * no difference
 *
 * Revision 2.1  91/12/28  16:59:04  yusuke
 * Final , Final revision
 * 
 * Revision 2.0  91/12/21  18:53:37  yusuke
 * '91 Cristmas version
 * 
 * Revision 1.6  1991/10/17  02:55:53  yusuke
 * made a new function eq_imp()
 *
 * Revision 1.5  1991/10/05  08:18:18  yusuke
 * add Log and RCSid for RCS
 *
 */


#if HAVE_CONFIG_H
#include "matpg_config.h"
#endif

#include "matpg.h"
#include "network.h"
#include "ReadBlifCmd.h"
#include "ReadIscas89Cmd.h"
#include "SetFaultCmd.h"
#include "PrintFaultCmd.h"
#include "PrintStatsCmd.h"
#include "PrintPatCmd.h"
#include "RtpgCmd.h"
#include "RfsimCmd.h"
#include "DtpgCmd.h"


BEGIN_NAMESPACE_YM_MATPG

int verbose_level;
FILE* ver_fp = stderr;

bool log_flag = true;
FILE* log_fp = stderr;

MStopWatch sw_timer(4);

int
matpg_init(Tcl_Interp* interp)
{
  //////////////////////////////////////////////////////////////////////
  // matpg という namespace を作っておく．
  //////////////////////////////////////////////////////////////////////
  if ( Tcl_Eval(interp, "namespace eval matpg {}") == TCL_ERROR ) {
    return TCL_ERROR;
  }


  //////////////////////////////////////////////////////////////////////
  // サブモジュールの初期化
  //////////////////////////////////////////////////////////////////////
  
  TclCmdBinder<ReadBlifCmd>::reg(interp, "::matpg::read_blif");
  TclCmdBinder<ReadIscas89Cmd>::reg(interp, "::matpg::read_iscas89");
  TclCmdBinder<SetFaultCmd>::reg(interp, "::matpg::set_fault");
  TclCmdBinder<PrintFaultCmd>::reg(interp, "::matpg::print_fault");
  TclCmdBinder<PrintStatsCmd>::reg(interp, "::matpg::print_stats");
  TclCmdBinder<PrintPatCmd>::reg(interp, "::matpg::print_pat");
  TclCmdBinder<RtpgCmd>::reg(interp, "::matpg::rtpg");
  TclCmdBinder<RfsimCmd>::reg(interp, "::matpg::rfsim");
  TclCmdBinder<DtpgCmd>::reg(interp, "::matpg::dtpg");

  
  //////////////////////////////////////////////////////////////////////
  // tclreadline 用の処理
  // すごい手抜き
  //////////////////////////////////////////////////////////////////////
  ostringstream buf;
  buf << "namespace eval tclreadline {" << endl
      << "namespace eval matpg {" << endl
      << "proc complete(read_blif) { t s e l p m } { return \"\" }" << endl
      << "proc complete(read_iscas89) { t s e l p m } { return \"\" }" << endl
      << "proc complete(set_fault) { t s e l p m } { return \"\" }" << endl
      << "proc complete(print_fault) { t s e l p m } { return \"\" }" << endl
      << "proc complete(print_stats) { t s e l p m } { return \"\" }" << endl
      << "proc complete(print_pat) { t s e l p m } { return \"\" }" << endl
      << "proc complete(rtpg) { t s e l p m } { return \"\" }" << endl
      << "proc complete(rfsim) { t s e l p m } { return \"\" }" << endl
      << "proc complete(dtpg) { t s e l p m } { return \"\" }" << endl
      << "}" << endl
      << "}" << endl;
  string str = buf.str();
  if ( Tcl_Eval(interp, str.c_str()) == TCL_ERROR ) {
    return TCL_ERROR;
  }
  
  
  //////////////////////////////////////////////////////////////////////
  // パッケージ宣言
  //////////////////////////////////////////////////////////////////////
  if ( Tcl_PkgProvide(interp, "Matpg", MATPG_VERSION) != TCL_OK ) {
    return TCL_ERROR;
  }
  
  return TCL_OK;
}

END_NAMESPACE_YM_MATPG
