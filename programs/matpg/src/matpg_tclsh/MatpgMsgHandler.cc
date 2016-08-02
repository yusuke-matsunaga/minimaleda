
/// @file src/io_cmds/MatpgMsgHandler.cc
/// @brief MatpgMsgHandler の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: MatpgMsgHandler.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2009 Yusuke Matsunaga
/// All rights reserved.


#if HAVE_CONFIG_H
#include "matpg_config.h"
#endif

#include "MatpgMsgHandler.h"
#include <ym_utils/FileRegion.h>


BEGIN_NAMESPACE_YM_MATPG

// @brief コンストラクタ
MatpgMsgHandler::MatpgMsgHandler()
{
}

// @brief デストラクタ
MatpgMsgHandler::~MatpgMsgHandler()
{
}

// @brief メッセージが登録されるたびに呼ばれる仮想関数
// @param[in] src_file この関数を読んでいるソースファイル名
// @param[in] src_line この関数を読んでいるソースの行番号
// @param[in] loc ファイル位置
// @param[in] type メッセージの種類
// @param[in] label メッセージラベル
// @param[in] body メッセージ本文
void
MatpgMsgHandler::put_msg(const char* src_file,
			 int src_line,
			 const FileRegion& loc,
			 tMsgType type,
			 const char* label,
			 const char* body)
{
  ostringstream buf;
  buf << loc << type << " [" << label << "]: " << body << endl;
  mMsg << buf.str();
}

// @brief 内容をクリアする．
void
MatpgMsgHandler::clear()
{
  mMsg.clear();
}

// @brief メッセージオブジェクトを取り出す．
TclObj
MatpgMsgHandler::msg_obj() const
{
  return mMsg;
}

END_NAMESPACE_YM_MATPG
