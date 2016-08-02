
/// @file magus/main/RandCmd.cc
/// @brief RandCmd の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: RandCmd.cc 1417 2008-05-06 18:12:40Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.

// 乱数を発生させるコマンドクラス
// 実はこのパッケージは Magus には依存していないので汎用になる．


#include "RandCmd.h"

#include "ym_utils/random.h"


BEGIN_NAMESPACE_MAGUS

//////////////////////////////////////////////////////////////////////
// 乱数初期化コマンド
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
RandInitCmd::RandInitCmd()
{
  set_usage_string("seed[ULONG]");
}

// 乱数初期化コマンド
int
RandInitCmd::cmd_proc(TclObjVector& objv)
{
  size_t objc = objv.size();
  if ( objc != 2 ) {
    print_usage();
    return TCL_ERROR;
  }

  ymulong seed = 0;
  int stat = ulong_conv(objv[1], seed);
  if ( stat != TCL_OK ) {
    return stat;
  }
  init_random_seed(seed);
  
  reset_result();

  return TCL_OK;
}


//////////////////////////////////////////////////////////////////////
// 乱数発生コマンド
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
RandCmd::RandCmd()
{
  set_usage_string("?range[ULONG]?");
}

// 乱数発生コマンド
int
RandCmd::cmd_proc(TclObjVector& objv)
{
  size_t objc = objv.size();
  if ( objc > 2 ) {
    print_usage();
    return TCL_ERROR;
  }

  ymulong limit = 0;
  if ( objc == 2 ) {
    int stat = ulong_conv(objv[1], limit);
    if ( stat != TCL_OK ) {
      return stat;
    }
  }
  long rand = random_num();
  if ( limit ) {
    rand = rand % limit;
  }
  
  set_result(rand);

  return TCL_OK;
}

END_NAMESPACE_MAGUS
