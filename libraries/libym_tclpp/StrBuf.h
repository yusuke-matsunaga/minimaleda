#ifndef LIBYM_TCLPP_STRBUF_H
#define LIBYM_TCLPP_STRBUF_H

/// @file libym_tclpp/StrBuf.h
/// @brief StrBuf のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: StrBuf.h 1343 2008-03-25 17:15:35Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_tclpp/tclpp_nsdef.h"


BEGIN_NAMESPACE_YM_TCLPP

//////////////////////////////////////////////////////////////////////
/// @class StrBuf StrBuf.h "StrBuf.h"
/// @brief const char* から char* のコピーを作るためのクラス
///
/// なんでこんなクラスが必要かというと Tcl の API が const char*
/// ではなく char* をとる仕様になっているから．
///
/// 実際なかには文字列を書き換える関数も存在するので書き換えられても
/// いいようにここで作業領域を確保してこのオブジェクト内の領域を
/// Tcl 関数に渡す．
//////////////////////////////////////////////////////////////////////
class StrBuf
{
public:

  /// @brief 空のコンストラクタ
  ///
  /// 中身は NULL となる．
  StrBuf();
  
  /// @brief const char* を引数としたコンストラクタ
  StrBuf(const char* src);
  
  /// @brief const string& を引数としたコンストラクタ
  StrBuf(const string& src);
  
  /// @brief const char* からの代入演算子
  const StrBuf&
  operator=(const char* src);
  
  /// @brief const string& からの代入演算子
  const StrBuf&
  operator=(const string& src);
  
  /// @brief デストラクタ
  ~StrBuf();
  
  /// @brief char* へのキャスト演算子
  operator char*();
  
  /// @brief const char* へのキャスト演算子
  operator const char*() const;
  

private:

  // 内容をセットする共通関数
  void
  set(const char* src);


private:
  //////////////////////////////////////////////////////////////////////
  // メンバ変数
  //////////////////////////////////////////////////////////////////////

  // 文字列を保持しておくバッファ領域
  char* mBuf;

  // バッファのサイズ
  size_t mSize;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// const char* を引数としたコンストラクタ
inline
StrBuf::StrBuf(const char* src) :
  mBuf(NULL),
  mSize(0)
{
  set(src);
}

// const string& を引数としたコンストラクタ
inline
StrBuf::StrBuf(const string& src) :
  mBuf(NULL),
  mSize(0)
{
  set(src.c_str());
}

// const char* からの代入演算子
inline
const StrBuf&
StrBuf::operator=(const char* src)
{
  set(src);
  return *this;
}

// const string& からの代入演算子
inline
const StrBuf&
StrBuf::operator=(const string& src)
{
  set(src.c_str());
  return *this;
}

// デストラクタ
inline
StrBuf::~StrBuf()
{
  if ( mBuf ) {
    delete [] mBuf;
  }
}

// 中身を取り出す．
inline
StrBuf::operator char*()
{
  return mBuf;
}

// @brief const char* へのキャスト演算子
inline
StrBuf::operator const char*() const
{
  return mBuf;
}

// 内容をセットする共通関数
inline
void
StrBuf::set(const char* src)
{
  if ( src ) {
    size_t len = strlen(src) + 1;
    if ( mSize < len ) {
      delete [] mBuf;
      mBuf = new char[len];
      mSize = len;
    }
    const char* s = src;
    char* d = mBuf;
    while ( (*d ++ = *s ++) != '\0' ) { }
  }
  else {
    if ( mBuf ) {
      delete [] mBuf;
    }
    mBuf = NULL;
    mSize = 0;
  }
}

END_NAMESPACE_YM_TCLPP

#endif // LIBYM_TCLPP_STRBUF_H
