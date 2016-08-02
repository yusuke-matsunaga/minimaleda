
/// @file src/core/CopyNetwork.cc
/// @brief CopyNetwork の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: NtwkBaseCmd.cc 2274 2009-06-10 07:45:29Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "CopyNetwork.h"
#include "NetHandle.h"


BEGIN_NAMESPACE_MAGUS

//////////////////////////////////////////////////////////////////////
// コピーを行うコマンド
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
CopyNetwork::CopyNetwork(MagMgr* mgr) :
  MagCmd(mgr)
{
  set_usage_string("<src-network-name> <dst-network-name>");
}

// @brief デストラクタ
CopyNetwork::~CopyNetwork()
{
}

// コマンドを実行する．
int
CopyNetwork::cmd_proc(TclObjVector& objv)
{
  // このコマンドは厳密に2つの引数をとる．
  if ( objv.size() != 3 ) {
    print_usage();
    return TCL_ERROR;
  }

  // コピー元のネットワーク名を取ってくる．
  NetHandle* s_neth = find_nethandle(objv[1]);
  if ( !s_neth ) {
    // ソースとなるネットワークが存在しなかった．
    return TCL_ERROR;
  }

  // コピー先のネットワーク名をとってくる．
  NetHandle* d_neth = find_or_new_nethandle(objv[2], s_neth->type());
  if ( !d_neth ) {
    // 何らかのエラーが起こった．
    return TCL_ERROR;
  }

  // コピーを行う．
  if ( d_neth->copy(s_neth) ) {
    // 正常終了
    return TCL_OK;
  }
  else {
    // エラー
    TclObj emsg;
    emsg << "Error occured. Perhaps, network types are different.";
    set_result(emsg);
    return TCL_ERROR;
  }
}

END_NAMESPACE_MAGUS
