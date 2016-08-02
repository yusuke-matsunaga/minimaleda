#ifndef YM_BNET_BNETISCAS89READER_H
#define YM_BNET_BNETISCAS89READER_H

/// @file ym_bnet/BNetIscas89Reader.h
/// @brief BNetIscas89Reader のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: BNetIscas89Reader.h 1978 2009-02-06 12:29:16Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_bnet/bnet_nsdef.h"
#include "ym_iscas89/Iscas89Parser.h"


BEGIN_NAMESPACE_YM_BNET

class BNetIscas89Handler;

//////////////////////////////////////////////////////////////////////
/// @class BNetIscas89Reader BNetIscas89Reader.h "ym_bnet/BNetIscas89Reader.h"
/// @ingroup BnetGroup
/// @brief iscas89 形式のファイルを読み込んで BNetwork に設定するクラス
/// @sa BNetwork nsYm::nsIscas89::Iscas89Parser
//////////////////////////////////////////////////////////////////////
class BNetIscas89Reader
{
public:

  /// @brief コンストラクタ
  BNetIscas89Reader();

  /// @brief デストラクタ
  ~BNetIscas89Reader();

  
public:

  /// @brief ISCAS89 形式のファイルを読み込む
  /// @param[in] filename ファイル名
  /// @param[in] network 読み込んだ内容を設定するネットワーク
  /// @retval true 正常に読み込めた
  /// @retval false 読み込み中にエラーが起こった．
  bool
  read(const string& filename,
       BNetwork& network);

  /// @brief メッセージハンドラを付加する．
  void
  add_msg_handler(MsgHandler* msg_handler);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // iscas89 パーサー
  Iscas89Parser mParser;

  // ハンドラ
  BNetIscas89Handler* mHandler;
  
};

END_NAMESPACE_YM_BNET

BEGIN_NAMESPACE_YM

// クラス名をインポートしておく
using nsBnet::BNetIscas89Reader;

END_NAMESPACE_YM

#endif // YM_BNET_BNETISCAS89READER_H
