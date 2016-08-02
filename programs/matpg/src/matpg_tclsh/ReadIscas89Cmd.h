#ifndef SRC_MATPG_READISCAS89CMD_H
#define SRC_MATPG_READISCAS89CMD_H

/// @file src/matpg/ReadIscas89Cmd.h
/// @brief ReadIscas89Cmd のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: ReadIscas89Cmd.h 2043 2009-02-21 07:08:48Z matsunaga $
///
/// Copyright (C) 2005-2008 Yusuke Matsunaga
/// All rights reserved.


#include "matpg_nsdef.h"
#include <ym_tclpp/TclCmd.h>
#include <ym_tgnet/TgNetwork.h>
#include <ym_tgnet/TgIscas89Reader.h>


BEGIN_NAMESPACE_YM_MATPG

class MatpgMsgHandler;

//////////////////////////////////////////////////////////////////////
// 故障の設定を行うコマンド
//////////////////////////////////////////////////////////////////////
class ReadIscas89Cmd :
  public TclCmd
{
public:

  /// @brief コンストラクタ
  ReadIscas89Cmd();

  /// @brief デストラクタ
  virtual
  ~ReadIscas89Cmd();


protected:

  /// @brief コマンド処理関数
  virtual
  int
  cmd_proc(TclObjVector& objv);
  

private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////
  
  // iscas89 パーサ
  TgIscas89Reader mReader;

  // メッセージハンドラ
  MatpgMsgHandler* mMsgHandler;
  
  // 読み込んだネットワークを保存しておくクラス
  TgNetwork mNetwork;

};

END_NAMESPACE_YM_MATPG

#endif // SRC_MATPG_READISCAS89CMD_H
