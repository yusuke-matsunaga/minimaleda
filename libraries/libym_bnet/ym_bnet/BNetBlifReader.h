#ifndef YM_BNET_BNETBLIFREADER_H
#define YM_BNET_BNETBLIFREADER_H

/// @file ym_bnet/BNetBlifReader.h
/// @brief BNetBlifReader のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: BNetBlifReader.h 1978 2009-02-06 12:29:16Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_bnet/bnet_nsdef.h"
#include "ym_blif/BlifParser.h"


BEGIN_NAMESPACE_YM_BNET

class BNetBlifHandler;

//////////////////////////////////////////////////////////////////////
/// @class BNetBlifReader BNetBlifReader.h "ym_bnet/BNetBlifReader.h"
/// @ingroup BnetGroup
/// @brief iscas89 形式のファイルを読み込んで BNetwork に設定するクラス
/// @sa BNetwork nsYm::nsBlif::BlifParser
//////////////////////////////////////////////////////////////////////
class BNetBlifReader
{
public:

  /// @brief コンストラクタ
  BNetBlifReader();

  /// @brief デストラクタ
  ~BNetBlifReader();

  
public:

  /// @brief blif 形式のファイルを読み込む
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
  BlifParser mParser;

  // ハンドラ
  BNetBlifHandler* mHandler;
  
};

END_NAMESPACE_YM_BNET

BEGIN_NAMESPACE_YM

// クラス名をインポートしておく
using nsBnet::BNetBlifReader;

END_NAMESPACE_YM

#endif // YM_BNET_BNETBLIFREADER_H
