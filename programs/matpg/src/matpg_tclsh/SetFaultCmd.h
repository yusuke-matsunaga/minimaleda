#ifndef SRC_MATPG_SETFAULTCMD_H
#define SRC_MATPG_SETFAULTCMD_H

/// @file src/matpg/SetFaultCmd.h
/// @brief SetFaultCmd のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: SetFaultCmd.h 2043 2009-02-21 07:08:48Z matsunaga $
///
/// Copyright (C) 2005-2008 Yusuke Matsunaga
/// All rights reserved.


#include "matpg_nsdef.h"
#include <ym_tclpp/TclCmd.h>


BEGIN_NAMESPACE_YM_MATPG

//////////////////////////////////////////////////////////////////////
// 故障の設定を行うコマンド
//////////////////////////////////////////////////////////////////////
class SetFaultCmd :
  public TclCmd
{
public:

  /// @brief コンストラクタ
  SetFaultCmd();

  /// @brief デストラクタ
  virtual
  ~SetFaultCmd();


protected:

  /// @brief コマンド処理関数
  virtual
  int
  cmd_proc(TclObjVector& objv);
  
};

END_NAMESPACE_YM_MATPG

#endif // SRC_MATPG_SETFAULTCMD_H
