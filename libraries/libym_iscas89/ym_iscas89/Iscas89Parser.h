#ifndef YM_ISCAS89_ISCAS89PARSER_H
#define YM_ISCAS89_ISCAS89PARSER_H

/// @file ym_iscas89/Iscas89Parser.h
/// @brief Iscas89Parser のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: Iscas89Parser.h 1978 2009-02-06 12:29:16Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_iscas89/iscas89_nsdef.h"
#include "ym_utils/MsgHandler.h"


BEGIN_NAMESPACE_YM_ISCAS89

//////////////////////////////////////////////////////////////////////
/// @class Iscas89Parser Iscas89Parser.h <ym_iscas89/Iscas89Parser.h>
/// @ingroup Iscas89Group
/// @brief ISCAS89(.bench) 形式のファイルを読み込むパーサークラス
/// @sa Iscas89Handler Iscas89MsgHandler
//////////////////////////////////////////////////////////////////////
class Iscas89Parser
{
public:

  /// @brief コンストラクタ
  Iscas89Parser();

  /// @brief デストラクタ
  ~Iscas89Parser();


public:
  
  /// @brief 読み込みを行う．
  /// @param[in] filename ファイル名
  /// @retval true 読み込みが成功した．
  /// @retval false 読み込みが失敗した．
  bool
  read(const string& filename);

  /// @brief イベントハンドラの登録
  /// @param[in] handler 登録する対象のハンドラ
  /// @note handler はこのインスタンスが破壊される時に同時に破壊される．
  void
  add_handler(Iscas89Handler* handler);

  /// @brief メッセージハンドラの登録
  /// @param[in] handler 登録する対象のハンドラ
  /// @note handler はこのインスタンスが破壊される時に同時に破壊される．
  void
  add_msg_handler(MsgHandler* handler);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // パーサーの実体
  Iscas89ParserImpl* mRep;

};

END_NAMESPACE_YM_ISCAS89

#endif // YM_ISCAS89_ISCAS89PARSER_H
