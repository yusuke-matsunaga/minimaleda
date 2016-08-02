#ifndef SRC_TGNET_FAULTCMD_H
#define SRC_TGNET_FAULTCMD_H

/// @file src/tgnet/NtwkCmd.h
/// @brief NtwkCmd のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: FaultCmd.h 2043 2009-02-21 07:08:48Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "AtpgCmd.h"


BEGIN_NAMESPACE_YM_ATPG

//////////////////////////////////////////////////////////////////////
// 故障の設定を行うコマンド
//////////////////////////////////////////////////////////////////////
class SetFault :
  public AtpgCmd
{
public:

  /// @brief コンストラクタ
  SetFault(AtpgMgr* mgr);


protected:
  
  // コマンド処理関数
  virtual
  int
  cmd_proc(TclObjVector& objv);

};

END_NAMESPACE_YM_ATPG

#endif // SRC_TGNET_FAULTCMD_H
