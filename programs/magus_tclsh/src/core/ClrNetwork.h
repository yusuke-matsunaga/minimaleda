#ifndef SRC_CORE_CLRNETWORK_H
#define SRC_CORE_CLRNETWORK_H

/// @file src/core/ClrNetwork.h
/// @brief ClrNetwork のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: ClrNetwork.h 2274 2009-06-10 07:45:29Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "MagCmd.h"


BEGIN_NAMESPACE_MAGUS

//////////////////////////////////////////////////////////////////////
/// @class ClrNetwork ClrNetwork.h "ClrNetwork.h"
/// @brief ネットワークの内容をクリアするコマンド
//////////////////////////////////////////////////////////////////////
class ClrNetwork :
  public MagCmd
{
public:

  /// @brief コンストラクタ
  ClrNetwork(MagMgr* mgr);

  /// @brief デストラクタ
  virtual
  ~ClrNetwork();


protected:

  /// @brief コマンド処理関数
  virtual
  int
  cmd_proc(TclObjVector& objv);

};

END_NAMESPACE_MAGUS

#endif // SRC_CORE_CLRNETWORK_H
