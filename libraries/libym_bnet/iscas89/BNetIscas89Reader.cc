
/// @file libym_bnetiscas89reader/BNetIscas89Reader.cc
/// @brief BNetIscas89Reader の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: BNetIscas89Reader.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_bnet/BNetIscas89Reader.h"
#include "BNetIscas89Handler.h"


BEGIN_NAMESPACE_YM_BNET

//////////////////////////////////////////////////////////////////////
// BNetIscas89Reader
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
BNetIscas89Reader::BNetIscas89Reader() :
  mHandler(new BNetIscas89Handler)
{
  mParser.add_handler(mHandler);
}

// @brief デストラクタ
BNetIscas89Reader::~BNetIscas89Reader()
{
  // mHandler は Iscas89Parser が責任を持って破壊してくれる．
}

// @brief BLIF 形式のファイルを読み込む
// @param[in] filename ファイル名
// @param[in] network 読み込んだ内容を設定するネットワーク
// @param[in] msg_handler メッセージハンドラ
// @retval true 正常に読み込めた
// @retval false 読み込み中にエラーが起こった．
bool
BNetIscas89Reader::read(const string& filename,
			BNetwork& network)
{
  mHandler->set_network(&network);
  
  bool stat = mParser.read(filename);
  if ( !stat ) {
    return false;
  }
  
  return true;
}

// @brief メッセージハンドラを付加する．
void
BNetIscas89Reader::add_msg_handler(MsgHandler* msg_handler)
{
  mParser.add_msg_handler(msg_handler);
}

END_NAMESPACE_YM_BNET
