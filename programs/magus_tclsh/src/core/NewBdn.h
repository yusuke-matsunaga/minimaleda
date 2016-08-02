#ifndef SRC_CORE_NEWBDNORK_H
#define SRC_CORE_NEWBDNWORK_H

/// @file src/core/NewBdn.h
/// @brief NewBdn のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: NtwkBaseCmd.h 2274 2009-06-10 07:45:29Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "MagCmd.h"


BEGIN_NAMESPACE_MAGUS

//////////////////////////////////////////////////////////////////////
/// @class NewBdn NewBdn.h "NewBdn.h"
/// @brief 新しいネットワークを作成するコマンド
//////////////////////////////////////////////////////////////////////
class NewBdn :
  public MagCmd
{
public:

  /// @brief コンストラクタ
  NewBdn(MagMgr* mgr);

  /// @brief デストラクタ
  virtual
  ~NewBdn();


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

#endif // SRC_CORE_NEWBDN_H
