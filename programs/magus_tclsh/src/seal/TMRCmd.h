#ifndef SRC_SEAL_TMRCMD_H
#define SRC_SEAL_TMRCMD_H

/// @file src/seal/TMRCmd.h
/// @brief TMRCmd のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: TMRCmd.h 2274 2009-06-10 07:45:29Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "BNetCmd.h"


BEGIN_NAMESPACE_MAGUS

//////////////////////////////////////////////////////////////////////
// 回路を3重化するコマンド
//////////////////////////////////////////////////////////////////////
class TMRCmd :
  public BNetCmd
{
public:

  /// @brief コンストラクタ
  TMRCmd(MagMgr* mgr);

  /// @brief デストラクタ
  virtual
  ~TMRCmd();


protected:

  // コマンド処理関数
  virtual
  int
  cmd_proc(TclObjVector& objv);

};

END_NAMESPACE_MAGUS

#endif // SRC_SEAL_TMRCMD_H
