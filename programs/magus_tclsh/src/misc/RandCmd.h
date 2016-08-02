#ifndef MAGUS_MAIN_RANDCMD_H
#define MAGUS_MAIN_RANDCMD_H

/// @file magus/main/RandCmd.h
/// @brief RandCmd のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: RandCmd.h 1417 2008-05-06 18:12:40Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.

// 乱数を発生させるコマンドクラス
// 実はこのパッケージは Magus には依存していないので汎用である．


#include "magus_nsdef.h"
#include "ym_tclpp/TclCmd.h"


BEGIN_NAMESPACE_MAGUS

//////////////////////////////////////////////////////////////////////
// 乱数初期化コマンド
//////////////////////////////////////////////////////////////////////
class RandInitCmd :
  public TclCmd
{
public:

  /// @brief コンストラクタ
  RandInitCmd();


protected:

  /// @brief コマンド処理関数
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
public:

  /// @brief コンストラクタ
  RandCmd();


protected:

  // コマンド処理関数
  virtual
  int
  cmd_proc(TclObjVector& objv);

};

END_NAMESPACE_MAGUS

#endif // MAGUS_MAIN_RANDCMD_H
