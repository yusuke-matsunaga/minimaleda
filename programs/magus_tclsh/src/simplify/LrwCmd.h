#ifndef SRC_SIMPLIFY_LRWCMD_H
#define SRC_SIMPLIFY_LRWCMD_H

/// @file src/simplify/LrwCmd.h
/// @brief LrwCmd のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: LrwCmd.h 2274 2009-06-10 07:45:29Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "BNetCmd.h"
#include "simplify.h"


BEGIN_NAMESPACE_MAGUS_SIMPLIFY

//////////////////////////////////////////////////////////////////////
// local rewriting を行うコマンド
//////////////////////////////////////////////////////////////////////
class LrwCmd :
  public BNetCmd
{
public:

  /// @brief コンストラクタ
  LrwCmd(MagMgr* mgr);

  /// @brief デストラクタ
  virtual
  ~LrwCmd();


protected:

  // コマンド処理関数
  virtual
  int
  cmd_proc(TclObjVector& objv);

};

END_NAMESPACE_MAGUS_SIMPLIFY

#endif // SRC_SIMPLIFY_LRWCMD_H
