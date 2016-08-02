
/// @file libym_tgnet/TgIscas89Reader
/// @brief TgIscas89Reaer の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: TgIscas89Reader.cc 1978 2009-02-06 12:29:16Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_tgnet/TgIscas89Reader.h"
#include "TgIscas89Handler.h"


BEGIN_NAMESPACE_YM_TGNET

//////////////////////////////////////////////////////////////////////
// ISCAS89 形式のファイルを読み込むためのクラス
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
TgIscas89Reader::TgIscas89Reader() :
  mHandler(new TgIscas89Handler)
{
  mParser.add_handler(mHandler);
}

// @brief デストラクタ
TgIscas89Reader::~TgIscas89Reader()
{
  // mHandler は Iscas89Reader が責任をもって破壊してくれる．
}
  
// @brief blif ファイルを読み込む．
bool
TgIscas89Reader::operator()(const string& filename,
			    TgNetwork& network)
{
  mHandler->set_network(network);
  bool stat = mParser.read(filename);
  return stat;
}

// @brief メッセージハンドラを追加する．
void
TgIscas89Reader::add_msg_handler(MsgHandler* msg_handler)
{
  mParser.add_msg_handler(msg_handler);
}

END_NAMESPACE_YM_TGNET
