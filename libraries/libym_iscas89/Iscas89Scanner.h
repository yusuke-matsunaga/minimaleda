#ifndef LIBYM_ISCAS89_ISCAS89SCANNER_H
#define LIBYM_ISCAS89_ISCAS89SCANNER_H

/// @file libym_iscas89/Iscas89Scanner.h
/// @brief Iscas89Scanner のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: Iscas89Scanner.h 1978 2009-02-06 12:29:16Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_iscas89/iscas89_nsdef.h"
#include "ym_utils/StrBuff.h"
#include "ym_utils/FileRegion.h"


BEGIN_NAMESPACE_YM_ISCAS89

//////////////////////////////////////////////////////////////////////
/// @class Iscas89Scanner Iscas89Scanner.h "Iscas89Scanner.h"
/// @brief iscas89 用の字句解析器
//////////////////////////////////////////////////////////////////////
class Iscas89Scanner
{
public:

  /// @brief コンストラクタ
  Iscas89Scanner();

  /// @brief デストラクタ
  ~Iscas89Scanner();


public:

  /// @brief 入力ストリームを設定する．
  /// @param[in] istr 入力ストリーム
  /// @param[in] file_desc ファイル記述子
  void
  init(istream& istr,
       const FileDesc* file_desc);

  /// @brief トークンを一つ読み出す．
  int
  get_token();

  /// @brief 最後の get_token() で読み出した字句の文字列を返す．
  const StrBuff&
  cur_string();

  /// @brief 最後の get_token() で読み出した字句の位置情報を返す．
  const FileRegion&
  cur_loc();


private:

  // 次の文字を先読みする．
  int
  peek_next();

  // 現在の文字を読み出す．
  int
  cur_char();

  // 実際に文字を読み出す
  int
  read_char();

  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 読み込み元の入力ストリーム
  istream* mInput;

  // ファイル記述子
  const FileDesc* mFileDesc;
  
  // 直前の文字が '\r' だったときに true となるフラグ
  bool mCR;

  // 文字バッファ
  int mCurChar;

  // 文字列バッファ
  StrBuff mCurString;

  // 現在処理中の行番号
  int mCurLineNo;

  // 現在処理中のコラム位置
  int mCurColumn;

  // 最後の get_token() で読み出されたトークンの位置情報
  FileRegion mCurTokenLoc;
  
};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 最後の get_token() で読み出した字句の文字列を返す．
inline
const StrBuff&
Iscas89Scanner::cur_string()
{
  return mCurString;
}

// @brief 最後の get_token() で読み出した字句の位置情報を返す．
inline
const FileRegion&
Iscas89Scanner::cur_loc()
{
  return mCurTokenLoc;
}

// 次の文字を先読みする．
inline
int
Iscas89Scanner::peek_next()
{
  if ( mCurChar == -1 ) {
    mCurChar = read_char();
  }
  return mCurChar;
}


END_NAMESPACE_YM_ISCAS89

#endif // LIBYM_ISCAS89_ISCAS89SCANNER_H
