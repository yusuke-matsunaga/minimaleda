
/// @file src/tgnet/FaultCmd.cc
/// @brief TgNetwork 操作関連のコマンド
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: FaultCmd.cc 2128 2009-03-12 07:25:33Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "FaultCmd.h"
#include "AtpgMgr.h"
#include "FaultMgr.h"


BEGIN_NAMESPACE_YM_ATPG

//////////////////////////////////////////////////////////////////////
// 故障の設定を行うコマンド
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SetFault::SetFault(AtpgMgr* mgr) :
  AtpgCmd(mgr)
{
}
  
// コマンド処理関数
int
SetFault::cmd_proc(TclObjVector& objv)
{
  size_t objc = objv.size();

  if ( objc > 1 ) {
    print_usage();
    return TCL_ERROR;
  }
  
  TgNetwork& network = _network();
  FaultMgr& fault_mgr = _fault_mgr();
  fault_mgr.set_ssa_fault(network);

  after_update_faults();
  
  return TCL_OK;
}

END_NAMESPACE_YM_ATPG
