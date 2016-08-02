#ifndef SRC_MATPG_DTPGCMD_H
#define SRC_MATPG_DTPGCMD_H

/// @file src/matpg/DtpgCmd.h
/// @brief DtpgCmd のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: DtpgCmd.h 2043 2009-02-21 07:08:48Z matsunaga $
///
/// Copyright (C) 2005-2008 Yusuke Matsunaga
/// All rights reserved.


#include "matpg_nsdef.h"
#include <ym_tclpp/TclCmd.h>


BEGIN_NAMESPACE_YM_MATPG

//////////////////////////////////////////////////////////////////////
// テストパタン生成を行うコマンド
//////////////////////////////////////////////////////////////////////
class DtpgCmd :
  public TclCmd
{
public:

  /// @brief コンストラクタ
  DtpgCmd();

  /// @brief デストラクタ
  virtual
  ~DtpgCmd();


protected:

  /// @brief コマンド処理関数
  virtual
  int
  cmd_proc(TclObjVector& objv);
  

private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////
  
  // verbose オプションの解析用オブジェクト
  TclPoptInt* mPoptVerbose;
  
  // fsim モードオプションの解析用オブジェクト
  TclPoptStr* mPoptFsim;

  // dynamic implication モードオプションの解析用オブジェクト
  TclPoptStr* mPoptDyn;

  // scoping オプションの解析用オブジェクト
  TclPopt* mPoptScope;

  // backtrack limit オプションお解析用オブジェクト
  TclPoptInt* mPoptBacktrack;
  
};

END_NAMESPACE_YM_MATPG

#endif // SRC_MATPG_DTPGCMD_H
