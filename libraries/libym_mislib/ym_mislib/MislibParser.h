#ifndef YM_MISLIB_MISLIBPARSER_H
#define YM_MISLIB_MISLIBPARSER_H

/// @file libym_cell/MislibParser.h
/// @brief MislibParser のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: MislibParser.h 1978 2009-02-06 12:29:16Z matsunaga $
///
/// Copyright (C) 2005-2009 Yusuke Matsunaga
/// All rights reserved.


#include <ym_mislib/mislib_nsdef.h>
#include <ym_utils/MsgHandler.h>


BEGIN_NAMESPACE_YM_MISLIB

//////////////////////////////////////////////////////////////////////
/// @class MislibParser MislibParser.h "mislib/MislibParser.h"
/// @brief Mislib ファイルのパーサー
//////////////////////////////////////////////////////////////////////
class MislibParser
{
public:

  /// @brief コンストラクタ
  MislibParser();

  /// @brief デストラクタ
  ~MislibParser();
  

public:
  
  /// @brief mislib ファイルを読み込む
  /// @param[in] filename ファイル名
  /// @retval true 読み込みが成功した．
  /// @retval false 読み込みが失敗した．
  bool
  read(const string& filename);

  /// @brief イベントハンドラの登録
  /// @param[in] handler 登録するハンドラ
  /// @note handler はこのインスタンスが破壊される時に同時に破壊される．
  void
  add_handler(MislibHandler* handler);
  
  /// @brief メッセージハンドラの登録
  /// @param[in] handler 登録するハンドラ
  /// @note handler はこのインスタンスが破壊される時に同時に破壊される．
  void
  add_msg_handler(MsgHandler* handler);

  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////
  
  // パーサーの実体
  MislibParserImpl* mRep;

};

END_NAMESPACE_YM_MISLIB

#endif // YM_MISLIB_MISLIBPARSER_H
