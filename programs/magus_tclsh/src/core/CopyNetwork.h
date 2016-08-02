#ifndef SRC_CORE_COPYNETWORK_H
#define SRC_CORE_COPYNETWORK_H

/// @file src/core/CopyNetwork.h
/// @brief CopyNetwork のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: NtwkBaseCmd.h 2274 2009-06-10 07:45:29Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "MagCmd.h"


BEGIN_NAMESPACE_MAGUS

//////////////////////////////////////////////////////////////////////
/// @class CopyNetwork CopyNetwork.h "CopyNetwork.h"
/// @brief コピーを行うコマンド
//////////////////////////////////////////////////////////////////////
class CopyNetwork :
  public MagCmd
{
public:

  /// @brief コンストラクタ
  CopyNetwork(MagMgr* mgr);

  /// @brief デストラクタ
  virtual
  ~CopyNetwork();


protected:

  /// @brief コマンド処理関数
  virtual
  int
  cmd_proc(TclObjVector& objv);

};

END_NAMESPACE_MAGUS

#endif // SRC_CORE_COPYNETWORKCMD_H
