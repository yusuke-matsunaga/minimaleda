#ifndef SRC_CORE_POPNETWORK_H
#define SRC_CORE_POPNETWORK_H

/// @file src/core/PopNetwork.h
/// @brief PopNetwork のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: NtwkBaseCmd.h 2274 2009-06-10 07:45:29Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "MagCmd.h"


BEGIN_NAMESPACE_MAGUS

//////////////////////////////////////////////////////////////////////
/// @class PopNetwork PopNetwork.h "PopNetwork.h"
/// @brief カレントネットワークのスタックを元に戻す．
//////////////////////////////////////////////////////////////////////
class PopNetwork :
  public MagCmd
{
public:

  /// @brief コンストラクタ
  PopNetwork(MagMgr* mgr);

  /// @brief デストラクタ
  virtual
  ~PopNetwork();


protected:

  /// @brief コマンド処理関数
  virtual
  int
  cmd_proc(TclObjVector& objv);

};

END_NAMESPACE_MAGUS

#endif // SRC_CORE_POPNETWORK_H
