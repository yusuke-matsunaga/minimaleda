#ifndef LIBYM_VERILOG_SCANNER_TOKENINFO_H
#define LIBYM_VERILOG_SCANNER_TOKENINFO_H

/// @file libym_verilog/scanner/TokenInfo.h
/// @brief TokenInfo のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: TokenInfo.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_verilog/verilog.h"
#include "ym_utils/StrBuff.h"
#include "ym_utils/FileRegion.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class TokenInfo TokenInfo.h "TokenInfo.h"
/// @ingroup VlParser
/// @brief トークンの情報を表す構造体
//////////////////////////////////////////////////////////////////////
class TokenInfo
{
  friend class TokenList;

public:

  /// @brief 空のコンストラクタ
  TokenInfo();

  /// @brief マクロの置き換え用の位置パラメータを表すときのコンストラクタ
  /// @param[in] pos 位置パラメータ
  TokenInfo(int pos);
  
  /// @brief 値をセットするためのコンストラクタ
  /// @param[in] loc ファイル上の位置
  /// @param[in] id トークン番号
  /// @param[in] str 文字列
  TokenInfo(const FileRegion& loc,
	    int id,
	    const char* str);
  
  /// @brief デストラクタ
  ~TokenInfo();


public:

  /// @brief 位置パラメータの取得
  /// @return 位置パラメータのときその位置を返す．\n
  /// そうでなければ -1 を返す．
  int
  pos() const;
  
  /// @brief トークン番号を取り出す．
  int
  id() const;

  /// @brief 文字列を取り出す．
  const char*
  str() const;

  /// @brief ファイル位置を取り出す．
  const FileRegion&
  loc() const;

  /// @brief 次の要素を取り出す．
  const TokenInfo*
  next() const;
  

private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル位置を表す構造体
  FileRegion mLoc;

  // トークン番号
  int mId;

  // 実際の文字列
  StrBuff mString;

  // 次の要素
  TokenInfo* mLink;
  
};

/// @relates TokenInfo
/// @brief TokenInfo の内容を出力するための関数
/// @param[in] s 出力ストリーム
/// @param[in] token トークン情報
/// @return s を返す．
ostream&
operator<<(ostream& s,
	   const TokenInfo& token);


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 空のコンストラクタ
inline
TokenInfo::TokenInfo() :
  mLink(NULL)
{
}

// @brief マクロの置き換え用の位置パラメータを表すときのコンストラクタ
// @param[in] pos 位置パラメータ
inline
TokenInfo::TokenInfo(int pos) :
  mId(- (pos + 1)),
  mLink(NULL)
{
}

// @brief 値をセットするためのコンストラクタ
// @param[in] loc ファイル上の位置
// @param[in] id トークン番号
// @param[in] str 文字列
inline
TokenInfo::TokenInfo(const FileRegion& loc,
		     int id,
		     const char* str) :
  mLoc(loc),
  mId(id),
  mString(str),
  mLink(NULL)
{
}

// @brief デストラクタ
inline
TokenInfo::~TokenInfo()
{
}

// @brief 位置パラメータのときその位置を返す．
// そうでなければ -1 を返す．
inline
int
TokenInfo::pos() const
{
  if ( mId < 0 ) {
    return - (mId + 1);
  }
  else {
    return -1;
  }
}

// @brief トークン番号を取り出す．
inline
int
TokenInfo::id() const
{
  return mId;
}

// @brief 文字列を取り出す．
inline
const char*
TokenInfo::str() const
{
  return mString.c_str();
}

// @brief ファイル位置を取り出す．
inline
const FileRegion&
TokenInfo::loc() const
{
  return mLoc;
}

// @brief 次の要素を取り出す．
inline
const TokenInfo*
TokenInfo::next() const
{
  return mLink;
}

END_NAMESPACE_YM_VERILOG

#endif // LIBYM_VERILOG_SCANNER_TOKENINFO_H
