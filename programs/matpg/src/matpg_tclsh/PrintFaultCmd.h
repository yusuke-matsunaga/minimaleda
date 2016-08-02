#ifndef SRC_MATPG_PRINTFAULTCMD_H
#define SRC_MATPG_PRINTFAULTCMD_H

/// @file src/matpg/PrintFaultCmd.h
/// @brief PrintFaultCmd のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: PrintFaultCmd.h 2043 2009-02-21 07:08:48Z matsunaga $
///
/// Copyright (C) 2005-2008 Yusuke Matsunaga
/// All rights reserved.


#include "matpg_nsdef.h"
#include <ym_tclpp/TclCmd.h>


BEGIN_NAMESPACE_YM_MATPG

//////////////////////////////////////////////////////////////////////
// 情報を表示するコマンド
//////////////////////////////////////////////////////////////////////
class PrintFaultCmd :
  public TclCmd
{
public:

  /// @brief コンストラクタ
  PrintFaultCmd();

  /// @brief デストラクタ
  virtual
  ~PrintFaultCmd();


protected:

  /// @brief コマンド処理関数
  virtual
  int
  cmd_proc(TclObjVector& objv);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // type オプションの解析用オブジェクト
  TclPoptStr* mPoptType;
  
};

END_NAMESPACE_YM_MATPG

#endif // SRC_MATPG_PRINTSTATSCMD_H
