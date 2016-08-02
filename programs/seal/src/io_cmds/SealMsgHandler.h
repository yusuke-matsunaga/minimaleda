#ifndef SRC_IO_CMDS_SEALMSGHANDLER_H
#define SRC_IO_CMDS_SEALMSGHANDLER_H

/// @file src/io_cmds/SealMsgHandler.h
/// @brief SealMsgHandler のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: SealMsgHandler.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2009 Yusuke Matsunaga
/// All rights reserved.

// ネットワークを操作するコマンド群

#include "seal_nsdef.h"
#include <ym_utils/MsgHandler.h>
#include <ym_tclpp/TclObj.h>


BEGIN_NAMESPACE_YM_SEAL

//////////////////////////////////////////////////////////////////////
/// @class SealMsgHandler SealMsgHandler.h
/// @brief ReadBlif 用のメッセージハンドラ
//////////////////////////////////////////////////////////////////////
class SealMsgHandler :
  public MsgHandler
{
public:

  /// @brief コンストラクタ
  SealMsgHandler();

  /// @brief デストラクタ
  virtual
  ~SealMsgHandler();


public:
  
  /// @brief メッセージが登録されるたびに呼ばれる仮想関数
  /// @param[in] src_file この関数を読んでいるソースファイル名
  /// @param[in] src_line この関数を読んでいるソースの行番号
  /// @param[in] loc ファイル位置
  /// @param[in] type メッセージの種類
  /// @param[in] label メッセージラベル
  /// @param[in] body メッセージ本文
  virtual
  void
  put_msg(const char* src_file,
	  int src_line,
	  const FileRegion& loc,
	  tMsgType type,
	  const char* label,
	  const char* body);

  /// @brief 内容をクリアする．
  void
  clear();


public:

  /// @brief メッセージオブジェクトを取り出す．
  TclObj
  msg_obj() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  TclObj mMsg;

};

END_NAMESPACE_YM_SEAL

#endif // SRC_IO_CMDS_SEALMSGHANDLER_H
