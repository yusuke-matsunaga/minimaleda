
/// @file src/core/PopNetwork.cc
/// @brief PopNetwork の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: NtwkBaseCmd.cc 2274 2009-06-10 07:45:29Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "PopNetwork.h"


BEGIN_NAMESPACE_MAGUS

//////////////////////////////////////////////////////////////////////
// カレントネットワークのスタックを元に戻す．
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
PopNetwork::PopNetwork(MagMgr* mgr) :
  MagCmd(mgr)
{
}

// @brief デストラクタ
PopNetwork::~PopNetwork()
{
}

// コマンドを実行する．
int
PopNetwork::cmd_proc(TclObjVector& objv)
{
  size_t objc = objv.size();
  if ( objc != 1 && objc != 4 ) {
    // 通常は引数はなしだが，変数のトレースで呼び出される時に3つの引数
    // をとる．
    print_usage();
    return TCL_ERROR;
  }

  if ( !pop_cur_nethandle() ) {
    return TCL_ERROR;
  }

  return TCL_OK;
}

END_NAMESPACE_MAGUS
