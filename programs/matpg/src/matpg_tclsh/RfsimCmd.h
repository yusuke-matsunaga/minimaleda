#ifndef SRC_MATPG_RFSIMCMD_H
#define SRC_MATPG_RFSIMCMD_H

/// @file src/matpg/RfsimCmd.h
/// @brief RfsimCmd のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: RfsimCmd.h 2043 2009-02-21 07:08:48Z matsunaga $
///
/// Copyright (C) 2005-2008 Yusuke Matsunaga
/// All rights reserved.


#include "matpg_nsdef.h"
#include <ym_tclpp/TclCmd.h>


BEGIN_NAMESPACE_YM_MATPG

//////////////////////////////////////////////////////////////////////
// 逆順故障シミュレーションを行うコマンド
//////////////////////////////////////////////////////////////////////
class RfsimCmd :
  public TclCmd
{
public:

  /// @brief コンストラクタ
  RfsimCmd();

  /// @brief デストラクタ
  virtual
  ~RfsimCmd();


protected:

  /// @brief コマンド処理関数
  virtual
  int
  cmd_proc(TclObjVector& objv);
  
};

END_NAMESPACE_YM_MATPG

#endif // SRC_MATPG_RFSIMCMD_H
