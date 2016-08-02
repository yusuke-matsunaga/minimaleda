#ifndef LIBYM_VERILOG_SCANNER_LEXSTATE_H
#define LIBYM_VERILOG_SCANNER_LEXSTATE_H

/// @file libym_verilog/parser/LexState.h
/// @brief LexState のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: LexState.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_verilog/verilog.h"
#include "ym_utils/FileRegion.h"
#include "ym_utils/MsgHandler.h"
#include "RawLex.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class LexState Lex.h "Lex.h"
/// @ingroup VlParser
/// @brief Lex に付随した状態を保持する基底クラス
//////////////////////////////////////////////////////////////////////
class LexState
{
public:

  /// @brief コンストラクタ
  /// @param[in] lex 親の lexer オブジェクト
  LexState(RawLex& lex);
  
  /// @brief デストラクタ
  virtual
  ~LexState();


public:

  /// @brief 状態を初期化する．
  /// @note デフォルトの実装はなにもしない．
  virtual
  void
  resetall(const FileRegion& file_region);


protected:
  //////////////////////////////////////////////////////////////////////
  // 継承クラスで用いる便利関数
  //////////////////////////////////////////////////////////////////////

  /// @brief デバッグフラグのチェック
  /// @return デバッグフラグが立っていたら true を返す．
  bool
  debug() const;

  /// @brief 親の Lex の取得
  /// @return 親の Lex オブジェクト
  RawLex&
  lex() const;
  

protected:
  //////////////////////////////////////////////////////////////////////
  // メッセージ出力用の便利関数
  //////////////////////////////////////////////////////////////////////

  /// @brief メッセージを出力する．
  /// @param[in] src_file この関数を読んでいるソースファイル名
  /// @param[in] src_line この関数を読んでいるソースの行番号
  /// @param[in] file_loc ファイル位置
  /// @param[in] type メッセージの種類
  /// @param[in] label メッセージラベル
  /// @param[in] body メッセージ本文
  void
  put_msg(const char* src_file,
	  int src_line,
	  const FileRegion& file_loc,
	  tMsgType type,
	  const char* label,
	  const char* msg);
  
  /// @brief メッセージを出力する．
  /// @param[in] src_file この関数を読んでいるソースファイル名
  /// @param[in] src_line この関数を読んでいるソースの行番号
  /// @param[in] file_loc ファイル位置
  /// @param[in] type メッセージの種類
  /// @param[in] label メッセージラベル
  /// @param[in] body メッセージ本文
  void
  put_msg(const char* src_file,
	  int src_line,
	  const FileRegion& file_loc,
	  tMsgType type,
	  const char* label,
	  const string& msg);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 親の Lex
  RawLex& mLex;
  
};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 親の Lex の取得
// @return 親の Lex オブジェクト
inline
RawLex&
LexState::lex() const
{
  return mLex;
}

// @brief デバッグフラグのチェック
// @return デバッグフラグが立っていたら true を返す．
inline
bool
LexState::debug() const
{
  return mLex.debug();
}

// @brief メッセージを出力する．
// @param[in] src_file この関数を読んでいるソースファイル名
// @param[in] src_line この関数を読んでいるソースの行番号
// @param[in] file_loc ファイル位置
// @param[in] type メッセージの種類
// @param[in] label メッセージラベル
// @param[in] body メッセージ本文
inline
void
LexState::put_msg(const char* src_file,
		  int src_line,
		  const FileRegion& file_loc,
		  tMsgType type,
		  const char* label,
		  const char* msg)
{
  mLex.msg_mgr().put_msg(src_file, src_line, file_loc, type, label, msg);
}
  
// @brief メッセージを出力する．
// @param[in] src_file この関数を読んでいるソースファイル名
// @param[in] src_line この関数を読んでいるソースの行番号
// @param[in] file_loc ファイル位置
// @param[in] type メッセージの種類
// @param[in] label メッセージラベル
// @param[in] body メッセージ本文
inline
void
LexState::put_msg(const char* src_file,
		  int src_line,
		  const FileRegion& file_loc,
		  tMsgType type,
		  const char* label,
		  const string& msg)
{
  mLex.msg_mgr().put_msg(src_file, src_line, file_loc, type, label, msg);
}

END_NAMESPACE_YM_VERILOG

#endif // LIBYM_VERILOG_SCANNER_LEXSTATE_H
