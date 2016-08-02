
/// @file libym_tgnet/TgBlifReader.cc
/// @brief TgBlifReader の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: TgBlifReader.cc 1978 2009-02-06 12:29:16Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_tgnet/TgBlifReader.h"
#include "TgBlifHandler.h"


BEGIN_NAMESPACE_YM_TGNET

//////////////////////////////////////////////////////////////////////
// blif 形式のファイルを読み込むためのクラス
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
TgBlifReader::TgBlifReader() :
  mHandler(new TgBlifHandler)
{
  mParser.add_handler(mHandler);
}

// @brief デストラクタ
TgBlifReader::~TgBlifReader()
{
  // mHandler は BlifParser が責任をもって破壊してくれる．
}
  
// @brief blif ファイルを読み込む．
bool
TgBlifReader::operator()(const string& filename,
			 TgNetwork& network)
{
  mHandler->set_network(network);
  bool stat = mParser.read(filename);
  return stat;
}

// @brief メッセージハンドラを追加する．
void
TgBlifReader::add_msg_handler(MsgHandler* msg_handler)
{
  mParser.add_msg_handler(msg_handler);
}

END_NAMESPACE_YM_TGNET
