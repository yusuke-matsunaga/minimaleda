#ifndef SRC_SEAL_SEALCMD_H
#define SRC_SEAL_SEALCMD_H

/// @file src/seal/SealCmd.h
/// @brief SealCmd のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: SealCmd.h 2274 2009-06-10 07:45:29Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.

// ネットワークを操作するコマンド群

#include "BNetCmd.h"


BEGIN_NAMESPACE_MAGUS

//////////////////////////////////////////////////////////////////////
// ソフトエラーの解析を実行するコマンド
//////////////////////////////////////////////////////////////////////
class SealCmd :
  public BNetCmd
{
public:

  /// @brief コンストラクタ
  SealCmd(MagMgr* mgr);

  /// @brief デストラクタ
  virtual
  ~SealCmd();


protected:

  // コマンド処理関数
  virtual
  int
  cmd_proc(TclObjVector& objv);

};

END_NAMESPACE_MAGUS

#endif // SRC_SEAL_SEALCMD_H
