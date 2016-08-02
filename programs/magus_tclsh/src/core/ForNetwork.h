#ifndef SRC_CORE_FORNETWORK_H
#define SRC_CoRE_FORNETWORK_H

/// @file src/core/ForNetwork.h
/// @brief ForNetwork のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: NtwkBaseCmd.h 2274 2009-06-10 07:45:29Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "MagCmd.h"


BEGIN_NAMESPACE_MAGUS

//////////////////////////////////////////////////////////////////////
/// @class ForNetwork ForNetwork.h "ForNetwork.h"
/// @brief ネットワークを一つづつ取り出して処理するコマンド
/// その都度，カレントネットワークを切替える．
//////////////////////////////////////////////////////////////////////
class ForNetwork :
  public MagCmd
{
public:

  /// @brief コンストラクタ
  ForNetwork(MagMgr* mgr);

  /// @brief デストラクタ
  virtual
  ~ForNetwork();


protected:

  /// @brief コマンド処理関数
  virtual
  int
  cmd_proc(TclObjVector& objv);

};

END_NAMESPACE_MAGUS

#endif // SRC_CORE_FORNETWORK_H
