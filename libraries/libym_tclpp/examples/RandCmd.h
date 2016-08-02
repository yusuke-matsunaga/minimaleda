#ifndef LIBYM_TCLPP_EXAMPLES_RANDCMD_H
#define LIBYM_TCLPP_EXAMPLES_RANDCMD_H

/// @file RandCmd.h
/// @brief RandCmd のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: RandCmd.h 997 2007-09-07 09:58:29Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.

// 乱数を発生させるコマンドクラス


#include "ym_tclpp/TclCmd.h"


BEGIN_NAMESPACE_YM_TCLPP

//////////////////////////////////////////////////////////////////////
// 乱数初期化コマンド
//////////////////////////////////////////////////////////////////////
class RandInitCmd :
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
// 乱数発生コマンド
//////////////////////////////////////////////////////////////////////
class RandCmd :
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

#endif // LIBYM_TCLPP_EXAMPLES_RANDCMD_H
