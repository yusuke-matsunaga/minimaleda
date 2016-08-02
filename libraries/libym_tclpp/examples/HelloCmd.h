#ifndef LIBYM_TCLPP_EXAMPLES_HELLOCMD_H
#define LIBYM_TCLPP_EXAMPLES_HELLOCMD_H

/// @file HelloCmd.h
/// @brief HelloCmd のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: HelloCmd.h 997 2007-09-07 09:58:29Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.

// コマンドの製作例


#include "ym_tclpp/TclCmd.h"


BEGIN_NAMESPACE_YM_TCLPP

//////////////////////////////////////////////////////////////////////
// "Hello, world" コマンド1
//////////////////////////////////////////////////////////////////////
class HelloCmd :
  public TclCmd
{
protected:

  // usage を出力する関数
  virtual
  void
  usage();
  
  // コマンド処理関数
  virtual
  int
  cmd_proc(TclObjVector& objv);

};


//////////////////////////////////////////////////////////////////////
// "Hello, world" コマンド2
//////////////////////////////////////////////////////////////////////
class HelloCmd2 :
  public TclCmd
{
protected:

  // usage を出力する関数
  virtual
  void
  usage();
  
  // コマンド処理関数
  virtual
  int
  cmd_proc(TclObjVector& objv);

};

END_NAMESPACE_YM_TCLPP

#endif // LIBYM_TCLPP_EXAMPLES_HELLOCMD_H
