#ifndef YM_CELL_DOTLIB_DOTLIBLEX_H
#define YM_CELL_DOTLIB_DOTLIBLEX_H

/// @file libym_cell/dotlib/DotLibLex.h
/// @brief DotLibLex のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: DotLibLex.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_cell/cell_nsdef.h"
#include "ym_utils/MsgHandler.h"
#include "ym_utils/FileDescMgr.h"
#include "ym_utils/FileRegion.h"
#include "ym_utils/StrBuff.h"


BEGIN_NAMESPACE_YM_CELL

/// @brief トークンの値
const int STR = 257;
const int ERROR = 300;

//////////////////////////////////////////////////////////////////////
/// DotLib 用の LEX クラス
//////////////////////////////////////////////////////////////////////
class DotLibLex
{
public:

  /// @brief コンストラクタ
  /// @param[in] msg_mgr メッセージマネージャ
  DotLibLex(MsgMgr& msg_mgr);

  /// @brief デストラクタ
  ~DotLibLex();


public:

  /// @brief ファイルをオープンする．
  /// @param[in] filename ファイル名
  /// @return ファイルのオープンに失敗したら false を返す．
  bool
  open_file(const string& filename);

  /// @brief ファイルをクローズする．
  void
  close_file();

  /// @brief トークンを一つとってくる．
  int
  read_token();

  /// @brief 直前の read_token() に対応する文字列を返す．
  const char*
  cur_string() const;

  // 現在のファイル位置
  FileRegion
  cur_file_region();

  /// @brief 現在の行番号を返す．
  int
  cur_line() const;

  /// @brief 現在のトークンの最初のコラム位置
  int
  first_column() const;

  /// @brief 現在のコラム位置
  int
  cur_column() const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  // 初期化を行う．
  void
  init();

  // 一文字読み出す．
  int
  get();

  // 直前の get() を無かったことにする．
  void
  unget();

  // 改行を読み込んだ時の処理
  void
  nl();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // メッセージマネージャ
  MsgMgr& mMsgMgr;

  // ファイルを管理するオブジェクト
  FileDescMgr mFileDescMgr;

  // 現在オープン中のファイル
  const FileDesc* mCurFileDesc;

  // 入力ストリーム
  ifstream mInput;

  // 直前に読んだ文字
  int mLastChar;

  // unget() 用の文字バッファ
  int mUngetChar;

  // 直前の文字が \r の時に true となるフラグ
  bool mCR;

  // read_token の結果の文字列を格納する
  StrBuff mCurString;

  // 現在の行番号
  int mCurLine;

  // 現在のトークンの開始位置
  int mFirstColumn;

  // 現在のコラム位置
  int mCurColumn;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// 直前の read_token() に対応する文字列を返す．
inline
const char*
DotLibLex::cur_string() const
{
  return mCurString.c_str();
}

// 現在のファイル位置
inline
FileRegion
DotLibLex::cur_file_region()
{
  return FileRegion(mCurFileDesc,
		    mCurLine, mFirstColumn,
		    mCurLine, mCurColumn);
}

// 現在の行番号を返す．
inline
int
DotLibLex::cur_line() const
{
  return mCurLine;
}

// 現在のトークンの開始位置
inline
int
DotLibLex::first_column() const
{
  return mFirstColumn;
}

// 現在のコラム位置を返す．
inline
int
DotLibLex::cur_column() const
{
  return mCurColumn;
}

END_NAMESPACE_YM_CELL

#endif // YM_CELL_DOTLIB_DOTLIBLEX_H
