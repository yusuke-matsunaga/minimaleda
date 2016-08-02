#ifndef SRC_IO_CMDS_NTWKIOCMD_H
#define SRC_IO_CMDS_NTWKIOCMD_H

/// @file src/io_cmds/NtwkIoCmd.h
/// @brief NtwkIoCmd のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: NtwkIoCmd.h 2043 2009-02-21 07:08:48Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "AtpgCmd.h"
#include "ym_tgnet/TgBlifReader.h"
#include "ym_tgnet/TgIscas89Reader.h"


BEGIN_NAMESPACE_YM_ATPG

class AtpgMsgHandler;

//////////////////////////////////////////////////////////////////////
// blif 形式のファイルを読み込むコマンド
//////////////////////////////////////////////////////////////////////
class ReadBlif :
  public AtpgCmd
{
public:

  /// @brief コンストラクタ
  ReadBlif(AtpgMgr* mgr);

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
  
  // blif パーサ
  TgBlifReader mReader;
  
  // メッセージハンドラ
  AtpgMsgHandler* mMsgHandler;
  
};


//////////////////////////////////////////////////////////////////////
// iscas89 形式のファイルを読み込むコマンド
//////////////////////////////////////////////////////////////////////
class ReadIscas89 :
  public AtpgCmd
{
public:

  /// @brief コンストラクタ
  ReadIscas89(AtpgMgr* mgr);

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
  
  // iscas89 パーサ
  TgIscas89Reader mReader;
  
  // メッセージハンドラ
  AtpgMsgHandler* mMsgHandler;
  
};


//////////////////////////////////////////////////////////////////////
// TgNetwork の内容を出力するコマンド
//////////////////////////////////////////////////////////////////////
class WriteNetwork :
  public AtpgCmd
{
public:

  /// @brief コンストラクタ
  WriteNetwork(AtpgMgr* mgr);


protected:
  
  // コマンド処理関数
  virtual
  int
  cmd_proc(TclObjVector& objv);

};

END_NAMESPACE_YM_ATPG

#endif // SRC_IO_CMDS_NTWKIOCMD_H
