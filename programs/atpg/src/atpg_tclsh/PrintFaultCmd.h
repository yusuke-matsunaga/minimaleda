#ifndef SRC_ATPG_TCLSH_PRINTFAULTCMD_H
#define SRC_ATPG_TCLSH_PRINTFAULTCMD_H

/// @file src/atpg_tclsh/PrintFaultCmd.h
/// @brief PrintFaultCmd のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: PrintFaultCmd.h 2043 2009-02-21 07:08:48Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "atpg_nsdef.h"
#include "AtpgCmd.h"


BEGIN_NAMESPACE_YM_ATPG

//////////////////////////////////////////////////////////////////////
// 情報を表示するコマンド
//////////////////////////////////////////////////////////////////////
class PrintFaultCmd :
  public AtpgCmd
{
public:

  /// @brief コンストラクタ
  PrintFaultCmd(AtpgMgr* mgr);

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

END_NAMESPACE_YM_ATPG

#endif // SRC_ATPG_TCLSH_PRINTSTATSCMD_H
