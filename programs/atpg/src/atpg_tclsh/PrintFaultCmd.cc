
/// @file atpg/src/atpg_tclsh/PrintFaultCmd.cc
/// @brief ランダムパタン生成を行うコマンド
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: PrintFaultCmd.cc 2128 2009-03-12 07:25:33Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "PrintFaultCmd.h"
#include "AtpgMgr.h"
#include "FaultMgr.h"
#include "SaFault.h"
#include "ym_tclpp/TclPopt.h"


BEGIN_NAMESPACE_YM_ATPG

//////////////////////////////////////////////////////////////////////
// 情報を表示するコマンド
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
PrintFaultCmd::PrintFaultCmd(AtpgMgr* mgr) :
  AtpgCmd(mgr)
{
  mPoptType = new TclPoptStr(this, "type",
			     "spefify fault type "
			     "(detected, untestable, remain)");
}

// @brief デストラクタ
PrintFaultCmd::~PrintFaultCmd()
{
}

// コマンド処理関数
int
PrintFaultCmd::cmd_proc(TclObjVector& objv)
{
  size_t objc = objv.size();
  if ( objc != 1 ) {
    print_usage();
    return TCL_ERROR;
  }
  
  FaultMgr& fmgr = _fault_mgr();
  
  vector<SaFault*>::const_iterator b = fmgr.remain_list().begin();
  vector<SaFault*>::const_iterator e = fmgr.remain_list().end();
  
  if ( mPoptType->is_specified() ) {
    string tmp = mPoptType->val();
    if ( tmp == "detected" ) {
      b = fmgr.det_list().begin();
      e = fmgr.det_list().end();
    }
    else if ( tmp == "untestable" ) {
      b = fmgr.untest_list().begin();
      e = fmgr.untest_list().end();
    }
    else if ( tmp == "remain" ) {
      b = fmgr.remain_list().begin();
      e = fmgr.remain_list().end();
    }
    else {
      print_usage();
    }
  }

  for (vector<SaFault*>::const_iterator p = b; p != e; ++ p) {
    SaFault* f = *p;
    cout << f->str() << endl;
  }
  
  return TCL_OK;
}

END_NAMESPACE_YM_ATPG
