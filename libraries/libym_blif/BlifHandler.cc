
/// @file libym_blif/BlifHandler.cc
/// @brief BlibHandler の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: BlifHandler.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_blif/BlifHandler.h"
#include "BlifParserImpl.h"


BEGIN_NAMESPACE_YM_BLIF

//////////////////////////////////////////////////////////////////////
// BlifHandler
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
BlifHandler::BlifHandler()
{
}

// @brief デストラクタ
BlifHandler::~BlifHandler()
{
}
  
// @brief メッセージマネージャの取得
MsgMgr&
BlifHandler::msg_mgr()
{
  return mParser->msg_mgr();
}

// @brief ID番号から文字列を得る．
const char*
BlifHandler::id2str(ymuint32 id)
{
  return mParser->id2str(id);
}

// @brief ID番号からそれに関連した位置情報を得る．
const FileRegion&
BlifHandler::id2loc(ymuint32 id)
{
  return mParser->id2loc(id);
}

// @brief ID番号からそれに関連した位置情報を得る．
const FileRegion&
BlifHandler::id2def_loc(ymuint32 id)
{
  return mParser->id2def_loc(id);
}

END_NAMESPACE_YM_BLIF
