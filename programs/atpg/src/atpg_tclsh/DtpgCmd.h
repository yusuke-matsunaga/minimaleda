#ifndef SRC_DTPG_DTPGCMD_H
#define SRC_DTPG_DTPGCMD_H

/// @file src/dtpg/DtpgCmd.h
/// @brief DtpgCmd のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: DtpgCmd.h 2203 2009-04-16 05:04:40Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "AtpgCmd.h"
#include "Dtpg.h"


BEGIN_NAMESPACE_YM_ATPG

//////////////////////////////////////////////////////////////////////
// テストパタン生成を行うコマンド
//////////////////////////////////////////////////////////////////////
class DtpgCmd :
  public AtpgCmd
{
public:

  /// @brief コンストラクタ
  DtpgCmd(AtpgMgr* mgr);

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
  
  // sat モードオプションの解析用オブジェクト
  TclPopt* mPoptSat;
  
  // satrec モードオプションの解析様オブジェクト
  TclPopt* mPoptSatRec;
  
  // minisat モードオプションの解析様オブジェクト
  TclPopt* mPoptMiniSat;
  
  // fsim モードオプションの解析用オブジェクト
  TclPoptStr* mPoptFsim;

  // dynamic implication モードオプションの解析用オブジェクト
  TclPoptStr* mPoptDyn;

  // scoping オプションの解析用オブジェクト
  TclPopt* mPoptScope;

  // backtrack limit オプションの解析用オブジェクト
  TclPoptInt* mPoptBacktrack;
  
  // print_stats オプションの解析用オブジェクト
  TclPopt* mPoptPrintStats;
  
  // DTPG を行うクラス
  Dtpg mDtpg;
  
};

END_NAMESPACE_YM_ATPG

#endif // SRC_DTPG_DTPGCMD_H
