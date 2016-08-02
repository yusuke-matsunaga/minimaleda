#ifndef SRC_CORE_NEWMVN_H
#define SRC_CORE_NEWMVN_H

/// @file src/core/NewMvn.h
/// @brief NewMvn のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: NtwkBaseCmd.h 2274 2009-06-10 07:45:29Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "MagCmd.h"


BEGIN_NAMESPACE_MAGUS

//////////////////////////////////////////////////////////////////////
/// @class NewMvn NewMvn.h "NewMvn.h"
/// @brief 新しいネットワークを作成するコマンド
//////////////////////////////////////////////////////////////////////
class NewMvn :
  public MagCmd
{
public:

  /// @brief コンストラクタ
  NewMvn(MagMgr* mgr);

  /// @brief デストラクタ
  virtual
  ~NewMvn();


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

#endif // SRC_CORE_NEWMVN_H
