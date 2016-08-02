#ifndef SRC_CORE_PUSHNETWORK_H
#define SRC_CORE_PUSHNETWORK_H

/// @file src/core/PushNetwork.h
/// @brief PushNetwork のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: NtwkBaseCmd.h 2274 2009-06-10 07:45:29Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ChgNetwork.h"


BEGIN_NAMESPACE_MAGUS

//////////////////////////////////////////////////////////////////////
/// @class PushNetwork PushNetwork.h "PushNetwork.h"
/// @brief カレントネットワークスタックにプッシュするコマンド
//////////////////////////////////////////////////////////////////////
class PushNetwork :
  public ChgNetwork
{
public:

  /// @brief コンストラクタ
  PushNetwork(MagMgr* mgr);

  /// @brief デストラクタ
  virtual
  ~PushNetwork();


protected:

  /// @brief コマンド処理関数
  virtual
  int
  cmd_proc(TclObjVector& objv);

};

END_NAMESPACE_MAGUS

#endif // SRC_CORE_PUSHNETWORK_H
