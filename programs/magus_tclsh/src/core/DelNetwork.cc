
/// @file magus/logbase/DelNetwork.cc
/// @brief DelNetwork の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: DelNetwork.cc 2274 2009-06-10 07:45:29Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "DelNetwork.h"


BEGIN_NAMESPACE_MAGUS

//////////////////////////////////////////////////////////////////////
// ネットワークを削除するコマンド
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
DelNetwork::DelNetwork(MagMgr* mgr) :
  MagCmd(mgr)
{
  set_usage_string("<network-name>");
}

// @brief デストラクタ
DelNetwork::~DelNetwork()
{
}

// コマンドを実行する．
int
DelNetwork::cmd_proc(TclObjVector& objv)
{
  // このコマンドはネットワーク名以外の引数をとらない．
  if ( objv.size() != 2 ) {
    print_usage();
    return TCL_ERROR;
  }

  // ネットワークの削除を行う．
  bool result = delete_nethandle(objv[1]);
  if ( !result ) {
    // エラー
    return TCL_ERROR;
  }
  // 正常終了
  return TCL_OK;
}

END_NAMESPACE_MAGUS
