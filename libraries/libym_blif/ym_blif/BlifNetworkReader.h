#ifndef YM_BLIF_BLIFNETWORKREADER_H
#define YM_BLIF_BLIFNETWORKREADER_H

/// @file ym_blif/BlifNetwork.h
/// @brief BlifNetwork のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: BlifNetworkReader.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_blif/blif_nsdef.h"
#include "ym_blif/BlifParser.h"


BEGIN_NAMESPACE_YM_BLIF

class BlifNetworkHandler;

//////////////////////////////////////////////////////////////////////
/// @class BlifNetworkReader BlifNetwork.h <ym_blif/BlifNetwork.h>
/// @ingroup BlifGroup
/// @brief blif 形式のファイルを読み込んで BlifNetwork に設定するクラス
/// @sa BlifNetwork
//////////////////////////////////////////////////////////////////////
class BlifNetworkReader
{
public:

  /// @brief コンストラクタ
  BlifNetworkReader();

  /// @brief デストラクタ
  ~BlifNetworkReader();

  
public:

  /// @brief BLIF 形式のファイルを読み込む
  /// @param[in] filename ファイル名
  /// @param[out] network 読み込んだ内容を設定するネットワーク
  /// @retval true 正常に読み込めた
  /// @retval false 読み込み中にエラーが起こった．
  bool
  read(const string& filename,
       BlifNetwork& network);

  /// @brief メッセージハンドラを付加する．
  void
  add_msg_handler(MsgHandler* msg_handler);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // blif パーサー
  BlifParser mParser;
  
  // ハンドラ
  BlifNetworkHandler* mHandler;
  
};

END_NAMESPACE_YM_BLIF

#endif // YM_BLIF_BLIFNETWORKREADER_H
