#ifndef SRC_MATPG_MATPGMSGHANDLER_H
#define SRC_MATPG_MATPGMSGHANDLER_H

/// @file src/matpg/MatpgMsgHandler.h
/// @brief MatpgMsgHandler のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: MatpgMsgHandler.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2009 Yusuke Matsunaga
/// All rights reserved.


#include "matpg_nsdef.h"
#include <ym_utils/MsgHandler.h>
#include <ym_tclpp/TclObj.h>


BEGIN_NAMESPACE_YM_MATPG

//////////////////////////////////////////////////////////////////////
/// @class MatpgMsgHandler MatpgMsgHandler.h
/// @brief ReadBlif/ReadIscas89 用のメッセージハンドラ
//////////////////////////////////////////////////////////////////////
class MatpgMsgHandler :
  public MsgHandler
{
public:

  /// @brief コンストラクタ
  MatpgMsgHandler();

  /// @brief デストラクタ
  virtual
  ~MatpgMsgHandler();


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

END_NAMESPACE_YM_MATPG

#endif // SRC_MATPG_MATPGMSGHANDLER_H
