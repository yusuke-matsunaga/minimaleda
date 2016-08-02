#ifndef SRC_MATPG_PRINTSTATSCMD_H
#define SRC_MATPG_PRINTSTATSCMD_H

/// @file src/matpg/PrintStatsCmd.h
/// @brief PrintStatsCmd のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: PrintStatsCmd.h 2043 2009-02-21 07:08:48Z matsunaga $
///
/// Copyright (C) 2005-2008 Yusuke Matsunaga
/// All rights reserved.


#include "matpg_nsdef.h"
#include <ym_tclpp/TclCmd.h>
#include <ym_utils/StopWatch.h>


BEGIN_NAMESPACE_YM_MATPG

//////////////////////////////////////////////////////////////////////
// 情報を表示するコマンド
//////////////////////////////////////////////////////////////////////
class PrintStatsCmd :
  public TclCmd
{
public:

  /// @brief コンストラクタ
  PrintStatsCmd();

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

END_NAMESPACE_YM_MATPG

#endif // SRC_MATPG_PRINTSTATSCMD_H
