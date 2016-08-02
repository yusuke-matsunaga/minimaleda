
/// @file atpg/src/atpg_tclsh/RfsimCmd.cc
/// @brief RfsimCmd の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: RfsimCmd.cc 2203 2009-04-16 05:04:40Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "RfsimCmd.h"


BEGIN_NAMESPACE_YM_ATPG

//////////////////////////////////////////////////////////////////////
// 逆順故障シミュレーションを行うコマンド
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
RfsimCmd::RfsimCmd(AtpgMgr* mgr) :
  AtpgCmd(mgr)
{
}

// @brief デストラクタ
RfsimCmd::~RfsimCmd()
{
}

// コマンド処理関数
int
RfsimCmd::cmd_proc(TclObjVector& objv)
{
  size_t objc = objv.size();
  if ( objc != 1 ) {
    print_usage();
    return TCL_ERROR;
  }

  // GO!
  //mRfsim();
  
  return TCL_OK;
}

END_NAMESPACE_YM_ATPG
