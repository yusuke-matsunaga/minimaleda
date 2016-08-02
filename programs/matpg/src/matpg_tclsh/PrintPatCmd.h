#ifndef SRC_MATPG_PRINTPATCMD_H
#define SRC_MATPG_PRINTPATCMD_H

/// @file src/matpg/PrintPatCmd.h
/// @brief PrintPatCmd のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: PrintPatCmd.h 2043 2009-02-21 07:08:48Z matsunaga $
///
/// Copyright (C) 2005-2008 Yusuke Matsunaga
/// All rights reserved.


#include "matpg_nsdef.h"
#include <ym_tclpp/TclCmd.h>


BEGIN_NAMESPACE_YM_MATPG

//////////////////////////////////////////////////////////////////////
// テストパタンを表示するコマンド
//////////////////////////////////////////////////////////////////////
class PrintPatCmd :
  public TclCmd
{
public:

  /// @brief コンストラクタ
  PrintPatCmd();

  /// @brief デストラクタ
  virtual
  ~PrintPatCmd();


protected:

  /// @brief コマンド処理関数
  virtual
  int
  cmd_proc(TclObjVector& objv);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // hex オプションの解析用オブジェクト
  TclPopt* mPoptHex;

  // num オプションの解析用オブジェクト
  TclPopt* mPoptNum;
  
};

END_NAMESPACE_YM_MATPG

#endif // SRC_MATPG_PRINTPATCMD_H
