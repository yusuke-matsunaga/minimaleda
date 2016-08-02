#ifndef SRC_CORE_DELNETWORK_H
#define SRC_CORE_DELNETWORK_H

/// @file src/core/DelNetwork.h
/// @brief DelNetwork のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: NtwkBaseCmd.h 2274 2009-06-10 07:45:29Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.

#include "MagCmd.h"


BEGIN_NAMESPACE_MAGUS

//////////////////////////////////////////////////////////////////////
/// @class DelNetwork DelNetwork.h "DelNetwork.h"
/// @brief ネットワークを削除するコマンド
//////////////////////////////////////////////////////////////////////
class DelNetwork :
  public MagCmd
{
public:

  /// @brief コンストラクタ
  DelNetwork(MagMgr* mgr);

  /// @brief デストラクタ
  virtual
  ~DelNetwork();


protected:

  /// @brief コマンド処理関数
  virtual
  int
  cmd_proc(TclObjVector& objv);

};

END_NAMESPACE_MAGUS

#endif // SRC_CORE_DELNETWORK_H
