#ifndef MAGUS_LOGBASE_BNETINFOCMD_H
#define MAGUS_LOGBASE_BNETINFOCMD_H

/// @file magus/logbase/BNetInfoCmd.h
/// @brief ネットワークの情報を取得するコマンドのヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: BNetInfoCmd.h 2274 2009-06-10 07:45:29Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "BNetCmd.h"


BEGIN_NAMESPACE_MAGUS

//////////////////////////////////////////////////////////////////////
/// @class BNetInfo BNetInfoCmd.h "BNetInfoCmd.h"
/// @brief ネットワークの諸元を取り出すコマンド
//////////////////////////////////////////////////////////////////////
class BNetInfo :
  public BNetCmd
{
public:

  /// @brief コンストラクタ
  BNetInfo(MagMgr* mgr);

  /// @brief デストラクタ
  virtual
  ~BNetInfo();


protected:

  /// @brief コマンド処理関数
  virtual
  int
  cmd_proc(TclObjVector& objv);

};


//////////////////////////////////////////////////////////////////////
/// @class BNetAllInfo BNetInfoCmd.h "BNetInfoCmd.h"
/// @brief ネットワークの諸元を一括して取り出して連想配列に入れるコマンド
//////////////////////////////////////////////////////////////////////
class BNetAllInfo :
  public BNetCmd
{
public:

  /// @brief コンストラクタ
  BNetAllInfo(MagMgr* mgr);

  /// @brief デストラクタ
  virtual
  ~BNetAllInfo();


protected:

  /// @brief コマンド処理関数
  virtual
  int
  cmd_proc(TclObjVector& objv);

};


//////////////////////////////////////////////////////////////////////
/// @class BNetPrintStats BNetInfoCmd.h "BNetInfoCmd.h"
/// @brief ネットワークの緒元を出力するコマンド
//////////////////////////////////////////////////////////////////////
class BNetPrintStats :
  public BNetCmd
{
public:

  /// @brief コンストラクタ
  BNetPrintStats(MagMgr* mgr);

  /// @brief デストラクタ
  virtual
  ~BNetPrintStats();


protected:

  /// @brief コマンド処理関数
  virtual
  int cmd_proc(TclObjVector& objv);

};

END_NAMESPACE_MAGUS

#endif // MAGUS_LOGBASE_BNETINFOCMD_H
