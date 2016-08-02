
/// @file src/core/PushNetwork.cc
/// @brief PushNetwork の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: NtwkBaseCmd.cc 2274 2009-06-10 07:45:29Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "PushNetwork.h"


BEGIN_NAMESPACE_MAGUS

//////////////////////////////////////////////////////////////////////
// カレントネットワークスタックにプッシュするコマンド
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
PushNetwork::PushNetwork(MagMgr* mgr) :
  ChgNetwork(mgr)
{
  set_usage_string("<network-name>");
}

// @brief デストラクタ
PushNetwork::~PushNetwork()
{
}

// コマンドを実行する．
int
PushNetwork::cmd_proc(TclObjVector& objv)
{
  size_t objc = objv.size();
  if ( objc != 2 ) {
    print_usage();
    return TCL_ERROR;
  }

  TclObj arg = objv[1];
  int stat = new_opt(arg);
  if ( stat != TCL_OK ) {
    return stat;
  }

  // カレントネットワークスタックにいれる．
  if ( !push_cur_nethandle(arg) ) {
    return TCL_ERROR;
  }

  // 正常終了
  return TCL_OK;
}

END_NAMESPACE_MAGUS
