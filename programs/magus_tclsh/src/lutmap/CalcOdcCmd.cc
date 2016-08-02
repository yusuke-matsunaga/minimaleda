
/// @file magus/lutmap/tcl_bindings/CalcOdcCmd.cc
/// @brief CalcOdcCmd の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: CalcOdcCmd.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "CalcOdcCmd.h"
#include "ym_tclpp/TclPopt.h"
#include "ym_bdd/Bdd.h"
#include "ym_bdd/BmcFactory.h"
#include "OdcMgr.h"


BEGIN_NAMESPACE_MAGUS

//////////////////////////////////////////////////////////////////////
// calc_odc コマンド
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
CalcOdcCmd::CalcOdcCmd(MagMgr* mgr) :
  MagBNetCmd(mgr)
{
}

// @brief デストラクタ
CalcOdcCmd::~CalcOdcCmd()
{
}
  
// @brief コマンドを実行する仮想関数
int
CalcOdcCmd::cmd_proc(TclObjVector& objv)
{
  BddMgrRef bddmgr(nsBdd::BmcFactory(""));
  
  BNetwork& network = *cur_network();
  OdcMgr odc_mgr;

  odc_mgr.calc_odc(bddmgr, network);

  return TCL_OK;
}

END_NAMESPACE_MAGUS
