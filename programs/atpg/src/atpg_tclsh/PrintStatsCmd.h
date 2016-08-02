#ifndef SRC_ATPG_TCLSH_PRINTSTATSCMD_H
#define SRC_ATPG_TCLSH_PRINTSTATSCMD_H

/// @file src/atpg_tclsh/PrintStatsCmd.h
/// @brief PrintStatsCmd のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: PrintStatsCmd.h 2043 2009-02-21 07:08:48Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "atpg_nsdef.h"
#include "AtpgCmd.h"
#include "ym_utils/StopWatch.h"


BEGIN_NAMESPACE_YM_ATPG

//////////////////////////////////////////////////////////////////////
// 情報を表示するコマンド
//////////////////////////////////////////////////////////////////////
class PrintStatsCmd :
  public AtpgCmd
{
public:

  /// @brief コンストラクタ
  PrintStatsCmd(AtpgMgr* mgr);

  /// @brief デストラクタ
  virtual
  ~PrintStatsCmd();


protected:

  /// @brief コマンド処理関数
  virtual
  int
  cmd_proc(TclObjVector& objv);
  

private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 計時用のタイマー
  StopWatch mStopWatch;
  
};

END_NAMESPACE_YM_ATPG

#endif // SRC_ATPG_TCLSH_PRINTSTATSCMD_H
