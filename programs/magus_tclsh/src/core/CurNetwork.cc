
/// @file src/core/CurNetwork.cc
/// @brief CurNetwork の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: NetworkBaseCmd.cc 2274 2009-06-10 07:45:29Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "CurNetwork.h"
#include "NetHandle.h"


BEGIN_NAMESPACE_MAGUS

//////////////////////////////////////////////////////////////////////
// カレントネットワーク名を返すコマンド
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
CurNetwork::CurNetwork(MagMgr* mgr) :
  MagCmd(mgr)
{
}

// @brief デストラクタ
CurNetwork::~CurNetwork()
{
}

// コマンドを実行する．
int
CurNetwork::cmd_proc(TclObjVector& objv)
{
  size_t objc = objv.size();
  if( objc != 1 ) {
    print_usage();
    return TCL_ERROR;
  }

  NetHandle* neth = cur_nethandle();
  string name = neth->name();
  set_result(name);
  return TCL_OK;
}

END_NAMESPACE_MAGUS
