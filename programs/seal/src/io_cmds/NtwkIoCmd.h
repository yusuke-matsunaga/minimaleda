#ifndef SRC_IO_CMDS_NTWKIOCMD_H
#define SRC_IO_CMDS_NTWKIOCMD_H

/// @file src/io_cmds/NtwkIoCmd.h
/// @brief NtwkIoCmd のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: NtwkIoCmd.h 1978 2009-02-06 12:29:16Z matsunaga $
///
/// Copyright (C) 2005-2008 Yusuke Matsunaga
/// All rights reserved.


#include "SealCmd.h"
#include <ym_tgnet/TgBlifReader.h>
#include <ym_tgnet/TgIscas89Reader.h>


BEGIN_NAMESPACE_YM_SEAL

class SealMsgHandler;

//////////////////////////////////////////////////////////////////////
// blif 形式のファイルを読み込むコマンド
//////////////////////////////////////////////////////////////////////
class ReadBlif :
  public SealCmd
{
public:

  /// @brief コンストラクタ
  ReadBlif(SealMgr* mgr);

  /// @brief デストラクタ
  virtual
  ~ReadBlif();


protected:
  
  // コマンド処理関数
  virtual
  int
  cmd_proc(TclObjVector& objv);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // メッセージハンドラ
  SealMsgHandler* mMsgHandler;

  // Blif ファイルを読み込むためのパーサ
  TgBlifReader mReader;
  
};


//////////////////////////////////////////////////////////////////////
// iscas89 形式のファイルを読み込むコマンド
//////////////////////////////////////////////////////////////////////
class ReadIscas89 :
  public SealCmd
{
public:

  /// @brief コンストラクタ
  ReadIscas89(SealMgr* mgr);

  /// @brief デストラクタ
  virtual
  ~ReadIscas89();


protected:
  
  // コマンド処理関数
  virtual
  int
  cmd_proc(TclObjVector& objv);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // メッセージハンドラ
  SealMsgHandler* mMsgHandler;

  // ISCAS89 ファイルを読み込むためのパーサ
  TgIscas89Reader mReader;
  
};


//////////////////////////////////////////////////////////////////////
// TgNetwork の内容を出力するコマンド
//////////////////////////////////////////////////////////////////////
class WriteNetwork :
  public SealCmd
{
public:

  /// @brief コンストラクタ
  WriteNetwork(SealMgr* mgr);


protected:
  
  // コマンド処理関数
  virtual
  int
  cmd_proc(TclObjVector& objv);

};

END_NAMESPACE_YM_SEAL

#endif // SRC_IO_CMDS_NTWKIOCMD_H
