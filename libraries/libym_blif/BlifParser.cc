
/// @file libym_blif/BlifParser.cc
/// @brief BlibParser の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: BlifParser.cc 1978 2009-02-06 12:29:16Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "BlifParserImpl.h"
#include "ym_blif/BlifParser.h"


BEGIN_NAMESPACE_YM_BLIF

// @brief コンストラクタ
BlifParser::BlifParser()
{
  mRep = new BlifParserImpl;
}

// @brief デストラクタ
BlifParser::~BlifParser()
{
  delete mRep;
}
  
// @brief 読み込みを行う．
bool
BlifParser::read(const string& filename)
{
  return mRep->read(filename);
}

// @brief イベントハンドラの登録
void
BlifParser::add_handler(BlifHandler* handler)
{
  mRep->add_handler(handler);
}

// @brief メッセージハンドラの登録
void
BlifParser::add_msg_handler(MsgHandler* handler)
{
  mRep->msg_mgr().reg_handler(handler);
}

END_NAMESPACE_YM_BLIF
