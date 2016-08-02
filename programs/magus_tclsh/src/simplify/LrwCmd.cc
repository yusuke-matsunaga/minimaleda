
/// @file magus/src/simplify/LrwCmd.cc
/// @brief LrwCmd の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: LrwCmd.cc 2274 2009-06-10 07:45:29Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "LrwCmd.h"
#include "NetHandle.h"
#include "ym_bnet/BNetManip.h"


BEGIN_NAMESPACE_MAGUS_SIMPLIFY

//////////////////////////////////////////////////////////////////////
// ソフトエラー解析を行うコマンド
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
LrwCmd::LrwCmd(MagMgr* mgr) :
  BNetCmd(mgr)
{
}

// @brief デストラクタ
LrwCmd::~LrwCmd()
{
}

// コマンド処理関数
int
LrwCmd::cmd_proc(TclObjVector& objv)
{
  BNetwork& network = *cur_nethandle()->_bnetwork();
  BNetManip manip(&network);

  return TCL_OK;
}

END_NAMESPACE_MAGUS_SIMPLIFY
