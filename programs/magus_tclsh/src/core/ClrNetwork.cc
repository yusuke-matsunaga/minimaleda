
/// @file src/core/ClrNetwork.cc
/// @brief ClrNetwork の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: NtwkBaseCmd.cc 2274 2009-06-10 07:45:29Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ClrNetwork.h"
#include "NetHandle.h"


BEGIN_NAMESPACE_MAGUS

//////////////////////////////////////////////////////////////////////
// ネットワークの内容をクリアするコマンド
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
ClrNetwork::ClrNetwork(MagMgr* mgr) :
  MagCmd(mgr)
{
}

// @brief デストラクタ
ClrNetwork::~ClrNetwork()
{
}

// コマンドを実行する．
int
ClrNetwork::cmd_proc(TclObjVector& objv)
{
  // このコマンドは引数を取らない．
  if ( objv.size() != 1 ) {
    print_usage();
    return TCL_ERROR;
  }

  // ネットワークをクリアする．
  cur_nethandle()->clear();

  // このコマンドはエラーとならない．

  // 正常終了
  return TCL_OK;
}

END_NAMESPACE_MAGUS
