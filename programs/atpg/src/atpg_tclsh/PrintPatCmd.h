#ifndef SRC_ATPG_TCLSH_PRINTPATCMD_H
#define SRC_ATPG_TCLSH_PRINTPATCMD_H

/// @file src/atpg_tclsh/PrintPatCmd.h
/// @brief PrintPatCmd のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: PrintPatCmd.h 2043 2009-02-21 07:08:48Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "atpg_nsdef.h"
#include "AtpgCmd.h"


BEGIN_NAMESPACE_YM_ATPG

//////////////////////////////////////////////////////////////////////
// テストパタンを表示するコマンド
//////////////////////////////////////////////////////////////////////
class PrintPatCmd :
  public AtpgCmd
{
public:

  /// @brief コンストラクタ
  PrintPatCmd(AtpgMgr* mgr);

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

END_NAMESPACE_YM_ATPG

#endif // SRC_ATPG_TCLSH_PRINTPATCMD_H
