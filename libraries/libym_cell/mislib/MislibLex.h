#ifndef LIBYM_CELL_MISLIB_MISLIBLEX_H
#define LIBYM_CELL_MISLIB_MISLIBLEX_H

/// @file libym_cell/mislib/MislibLex.h
/// @brief MislibLex のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: MislibLex.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_cell/cell_nsdef.h"
#include "ym_utils/StrBuff.h"
#include "ym_utils/FileRegion.h"
#include "ym_utils/MsgHandler.h"


BEGIN_NAMESPACE_YM_CELL

//////////////////////////////////////////////////////////////////////
/// Mislib 用の LEX クラス
//////////////////////////////////////////////////////////////////////
class MislibLex
{
public:

  /// @brief コンストラクタ
  /// @param[in] msg_mgr メッセージマネージャ
  MislibLex(MsgMgr& msg_mgr);

  /// @brief デストラクタ
  ~MislibLex();


public:

  /// @brief 入力ストリームを設定する．
  /// @param[in] istr 入力ストリーム
  /// @param[in] file_desc ファイル記述子
  void
  init(istream& istr,
       const FileDesc* file_desc);

  /// @brief トークンを一つとってくる．
  int
  read_token();

  /// @brief 直前の read_token() に対応する文字列を返す．
  const char*
  cur_string() const;

  /// @brief cur_string() を double に変換したものを返す．
  double
  cur_num() const;

  /// @brief 現在のトークンの位置情報を返す．
  FileRegion
  cur_loc() const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  // 一文字読み出す．
  int
  get();

  // 直前の get() を無かったことにする．
  void
  unget();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // メッセージマネージャ
  MsgMgr& mMsgMgr;

  // 現在のファイル記述子
  const FileDesc* mFileDesc;

  // 入力ストリーム
  istream* mInput;

  // 直前に読んだ文字
  int mLastChar;

  // unget() 用の文字バッファ
  int mUngetChar;

  // 直前の文字が \r の時に true となるフラグ
  bool mCR;

  // read_token の結果の文字列を格納する
  StrBuff mCurString;

  // 現在の行番号
  ymuint32 mCurLine;

  // 現在のトークンの開始位置
  ymuint32 mFirstColumn;

  // 現在のコラム位置
  ymuint32 mCurColumn;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// 直前の read_token() に対応する文字列を返す．
inline
const char*
MislibLex::cur_string() const
{
  return mCurString.c_str();
}

// @brief cur_string() を double に変換したものを返す．
inline
double
MislibLex::cur_num() const
{
  return strtod(cur_string(), static_cast<char**>(NULL));
}

END_NAMESPACE_YM_CELL

#endif // LIBYM_CELL_MISLIB_MISLIBLEX_H
