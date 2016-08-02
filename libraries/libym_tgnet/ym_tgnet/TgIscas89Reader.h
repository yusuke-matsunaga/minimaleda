#ifndef YM_TGNET_TGISCAS89READER_H
#define YM_TGNET_TGISCAS89READER_H

/// @file ym_tgnet/TgIscas89Reader.h
/// @brief TgIscas89Reader のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: TgIscas89Reader.h 1978 2009-02-06 12:29:16Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_tgnet/tgnet_nsdef.h"
#include "ym_iscas89/Iscas89Parser.h"


BEGIN_NAMESPACE_YM_TGNET

class TgIscas89Handler;

//////////////////////////////////////////////////////////////////////
/// @class TgIscas89Reader TgIscas89Reader.h <ym_tgnet/TgIscas89Reader.h>
/// @brief iscas89 形式のファイルを読み込むためのクラス
//////////////////////////////////////////////////////////////////////
class TgIscas89Reader
{
public:

  /// @brief コンストラクタ
  TgIscas89Reader();

  /// @brief デストラクタ
  ~TgIscas89Reader();


public:
  
  /// @brief blif ファイルを読み込む．
  /// @param[in] filename ファイル名
  /// @param[in] network 設定対象のネットワーク
  bool
  operator()(const string& filename,
	     TgNetwork& network);

  /// @brief メッセージハンドラを追加する．
  void
  add_msg_handler(MsgHandler* msg_handler);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ISCAS89 ファイルを読み込むためのパーサ
  Iscas89Parser mParser;

  // ISCAS89 ファイルを TgNetwork に変換するためのハンドラ
  TgIscas89Handler* mHandler;
  
};

END_NAMESPACE_YM_TGNET

#endif // YM_TGNET_TGISCAS89READER_H
