
/// @file HelloCmd.cc
/// @brief HelloCmd の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: HelloCmd.cc 700 2007-05-31 00:41:30Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "HelloCmd.h"


BEGIN_NAMESPACE_YM_TCLPP

//////////////////////////////////////////////////////////////////////
// "Hello, world" コマンド1
//////////////////////////////////////////////////////////////////////

// usage を出力する関数
void
HelloCmd::usage()
{
  const char* usage =
    "USAGE: hello ?message?";
  set_result(usage);
}


// 乱数初期化コマンド
int
HelloCmd::cmd_proc(TclObjVector& objv)
{
  // 引数の数を数える．
  size_t objc = objv.size();
  if ( objc > 2 ) {
    usage();
    return TCL_ERROR;
  }

  if ( objc == 1 ) {
    // const char* から TclObj への暗黙の変換
    set_result("Hello, world");
  }
  else {
    TclObj result("Hello, ");

    result.append(objv[1]);
    set_result(result);
  }

  return TCL_OK;
}


//////////////////////////////////////////////////////////////////////
// "Hello, world" コマンド2
//////////////////////////////////////////////////////////////////////

// usage を出力する関数
void
HelloCmd2::usage()
{
  const char* usage =
    "USAGE: hello2 ?number?";
  set_result(usage);
}


// 乱数発生コマンド
int
HelloCmd2::cmd_proc(TclObjVector& objv)
{
  size_t objc = objv.size();
  if ( objc > 2 ) {
    usage();
    return TCL_ERROR;
  }

  // 第2引数を整数型に変換する．
  int n;
  if ( int_conv(objv[1], n) == TCL_ERROR ) {
    return TCL_ERROR;
  }

  TclObj result;
  for (int i = 0; i < n; ++ i) {
    result.append("Hello, world\n");
  }
  set_result(result);

  return TCL_OK;
}

END_NAMESPACE_YM_TCLPP
