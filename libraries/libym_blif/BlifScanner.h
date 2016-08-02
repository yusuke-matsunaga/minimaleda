#ifndef LIBYM_BLIF_BLIFSCANNER_H
#define LIBYM_BLIF_BLIFSCANNER_H

/// @file libym_blif/BlifScanner.h
/// @brief BlibScanner のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: BlifScanner.h 1978 2009-02-06 12:29:16Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_blif/blif_nsdef.h"
#include "ym_utils/StrBuff.h"
#include "ym_utils/FileRegion.h"
#include "BlifDic.h"


BEGIN_NAMESPACE_YM_BLIF

//////////////////////////////////////////////////////////////////////
/// @class BlifScanner BlifScanner.h "BlifScanner.h"
/// @brief blif 用の字句解析器
//////////////////////////////////////////////////////////////////////
class BlifScanner
{
public:

  /// @brief コンストラクタ
  BlifScanner();

  /// @brief デストラクタ
  ~BlifScanner();


public:

  /// @brief 入力ストリームを設定する．
  /// @param[in] istr 入力ストリーム
  /// @param[in] file_desc ファイル記述子
  void
  init(istream& istr,
       const FileDesc* file_desc);

  /// @brief トークンを一つ読み出す．
  tToken
  get_token();

  /// @brief トークンを戻す．
  void
  unget_token(tToken token,
	      const FileRegion& loc);

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

  // 読み戻されたトークン
  tToken mUngetToken;

  // mUngetToken に対応する位置情報
  FileRegion mUngetTokenLoc;

  // 予約語テーブル
  BlifDic mDic;

  // 文字列バッファ
  StrBuff mCurString;

  // 現在処理中の行番号
  int mCurLineNo;

  // 現在処理中のコラム位置
  int mCurColumn;

  // 現在の読み出し位置
  FileRegion mCurLoc;
  
};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief トークンを戻す．
inline
void
BlifScanner::unget_token(tToken token,
			 const FileRegion& loc)
{
  mUngetToken = token;
  mUngetTokenLoc = loc;
}

// @brief 最後の get_token() で読み出した字句の文字列を返す．
inline
const StrBuff&
BlifScanner::cur_string()
{
  return mCurString;
}

// @brief 最後の get_token() で読み出した字句の位置情報を返す．
inline
const FileRegion&
BlifScanner::cur_loc()
{
  return mCurLoc;
}

// 次の文字を先読みする．
inline
int
BlifScanner::peek_next()
{
  if ( mCurChar == -1 ) {
    mCurChar = read_char();
  }
  return mCurChar;
}


END_NAMESPACE_YM_BLIF

#endif // LIBYM_BLIF_BLIFSCANNER_H
