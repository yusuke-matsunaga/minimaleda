
/// @file src/io_cmds/SealMsgHandler.cc
/// @brief SealMsgHandler の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: SealMsgHandler.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2009 Yusuke Matsunaga
/// All rights reserved.

// ネットワークのファイル入出力関係のコマンド

#if HAVE_CONFIG_H
#include "seal_config.h"
#endif

#include "SealMsgHandler.h"
#include <ym_utils/FileRegion.h>


BEGIN_NAMESPACE_YM_SEAL

// @brief コンストラクタ
SealMsgHandler::SealMsgHandler()
{
}

// @brief デストラクタ
SealMsgHandler::~SealMsgHandler()
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
SealMsgHandler::put_msg(const char* src_file,
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
SealMsgHandler::clear()
{
  mMsg.clear();
}

// @brief メッセージオブジェクトを取り出す．
TclObj
SealMsgHandler::msg_obj() const
{
  return mMsg;
}

END_NAMESPACE_YM_SEAL
