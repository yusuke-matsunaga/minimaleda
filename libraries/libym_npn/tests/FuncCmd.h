#ifndef LIBYM_NPN_TESTS_FUNCCMD_H
#define LIBYM_NPN_TESTS_FUNCCMD_H

/// @file libym_npn/tests/FuncCmd.h
/// @brief FuncCmd のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: FuncCmd.h 33 2006-07-16 14:58:17Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_tclpp/TclCmd.h"
#include "ym_npn/common.h"


BEGIN_NAMESPACE_YM_NPN

//////////////////////////////////////////////////////////////////////
/// 全ての関数を列挙するコマンド
//////////////////////////////////////////////////////////////////////
class ForallFuncCmd :
  public TclCmd
{
protected:

  /// @brief usage を出力する関数
  virtual
  void usage();
  
  /// @brief コマンドを実行する仮想関数
  virtual
  int cmd_proc(TclObjVector& objv);
  
};


//////////////////////////////////////////////////////////////////////
/// ランダムに関数を生成するコマンド
//////////////////////////////////////////////////////////////////////
class RandomFuncCmd :
  public TclCmd
{
protected:

  /// @brief usage を出力する関数
  virtual
  void usage();
  
  /// @brief コマンドを実行する仮想関数
  virtual
  int cmd_proc(TclObjVector& objv);
  
};


//////////////////////////////////////////////////////////////////////
/// ビットベクタ形式のファイルから関数を生成するコマンド
//////////////////////////////////////////////////////////////////////
class ReadFuncCmd :
  public TclCmd
{
protected:

  /// @brief usage を出力する関数
  virtual
  void usage();
  
  /// @brief コマンドを実行する仮想関数
  virtual
  int cmd_proc(TclObjVector& objv);
  
};


//////////////////////////////////////////////////////////////////////
/// 関数のパタンを出力するコマンド
//////////////////////////////////////////////////////////////////////
class DumpFuncCmd :
  public TclCmd
{
protected:

  /// @brief usage を出力する関数
  virtual
  void usage();
  
  /// @brief コマンドを実行する仮想関数
  virtual
  int cmd_proc(TclObjVector& objv);
  
};



END_NAMESPACE_YM_NPN

#endif // LIBYM_NPN_TESTS_FUNCCMD_H
