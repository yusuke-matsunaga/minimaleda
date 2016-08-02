#ifndef SRC_CORE_NEWBNETWORK_H
#define SRC_CORE_NEWBNETWORK_H

/// @file src/core/NewBNetwork.h
/// @brief NewBNetwork のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: NtwkBaseCmd.h 2274 2009-06-10 07:45:29Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "MagCmd.h"


BEGIN_NAMESPACE_MAGUS

//////////////////////////////////////////////////////////////////////
/// @class NewBNetwork NewBNetwork.h "NewBNetwork.h"
/// @brief 新しいネットワークを作成するコマンド
//////////////////////////////////////////////////////////////////////
class NewBNetwork :
  public MagCmd
{
public:

  /// @brief コンストラクタ
  NewBNetwork(MagMgr* mgr);

  /// @brief デストラクタ
  virtual
  ~NewBNetwork();


protected:

  /// @brief コマンド処理関数
  virtual
  int
  cmd_proc(TclObjVector& objv);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

};

END_NAMESPACE_MAGUS

#endif // SRC_CORE_NEWBNETWORK_H
