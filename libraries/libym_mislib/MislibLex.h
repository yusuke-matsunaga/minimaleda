#ifndef YM_MISLIB_MISLIBLEX_H
#define YM_MISLIB_MISLIBLEX_H

/// @file libym_cell/MislibLex.h
/// @brief MislibLex のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: MislibLex.h 1978 2009-02-06 12:29:16Z matsunaga $
///
/// Copyright (C) 2005-2009 Yusuke Matsunaga
/// All rights reserved.


#include <ym_mislib/mislib_nsdef.h>
#include <ym_utils/StrBuff.h>
#include <ym_utils/FileRegion.h>
#include <ym_utils/MsgHandler.h>


BEGIN_NAMESPACE_YM_MISLIB

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

END_NAMESPACE_YM_MISLIB

#endif // YM_MISLIB_MISLIBLEX_H
