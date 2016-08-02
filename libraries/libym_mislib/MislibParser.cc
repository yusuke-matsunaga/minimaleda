
/// @file libym_mislib/MisLibParser.cc
/// @brief MislibParser の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: MislibParser.cc 1978 2009-02-06 12:29:16Z matsunaga $
///
/// Copyright (C) 2005-2009 Yusuke Matsunaga
/// All rights reserved.

#if HAVE_CONFIG_H
#include <ymconfig.h>
#endif

#include <ym_mislib/MislibParser.h>
#include "MislibParserImpl.h"


BEGIN_NAMESPACE_YM_MISLIB

//////////////////////////////////////////////////////////////////////
// MisLibPt オブジェクトを管理するクラス
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
MislibParser::MislibParser()
{
  mRep = new MislibParserImpl;
}

// @brief デストラクタ
MislibParser::~MislibParser()
{
  delete mRep;
}
  
// @brief mislib ファイルを読み込む
// @param[in] filename ファイル名
// @retval true 読み込みが成功した．
// @retval false 読み込みが失敗した．
bool
MislibParser::read(const string& filename)
{
  return mRep->read(filename);
}

// @brief イベントハンドラの登録
// @param[in] handler 登録するハンドラ
// @note handler はこのインスタンスが破壊される時に同時に破壊される．
void
MislibParser::add_handler(MislibHandler* handler)
{
  mRep->add_handler(handler);
}
  
// @brief メッセージハンドラの登録
// @param[in] handler 登録するハンドラ
// @note handler はこのインスタンスが破壊される時に同時に破壊される．
void
MislibParser::add_msg_handler(MsgHandler* handler)
{
  mRep->msg_mgr().reg_handler(handler);
}

END_NAMESPACE_YM_MISLIB
