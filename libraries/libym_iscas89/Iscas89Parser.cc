
/// @file libym_iscas89/Iscas89Parser.cc
/// @brief BlibParser の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: Iscas89Parser.cc 1978 2009-02-06 12:29:16Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "Iscas89ParserImpl.h"
#include "ym_iscas89/Iscas89Parser.h"


BEGIN_NAMESPACE_YM_ISCAS89

// @brief コンストラクタ
Iscas89Parser::Iscas89Parser()
{
  mRep = new Iscas89ParserImpl;
}

// @brief デストラクタ
Iscas89Parser::~Iscas89Parser()
{
  delete mRep;
}
  
// @brief 読み込みを行う．
bool
Iscas89Parser::read(const string& filename)
{
  return mRep->read(filename);
}

// @brief イベントハンドラの登録
void
Iscas89Parser::add_handler(Iscas89Handler* handler)
{
  mRep->add_handler(handler);
}

// @brief メッセージハンドラの登録
void
Iscas89Parser::add_msg_handler(MsgHandler* handler)
{
  mRep->msg_mgr().reg_handler(handler);
}

END_NAMESPACE_YM_ISCAS89
