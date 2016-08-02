#ifndef YM_UTILS_STRBUFF_H
#define YM_UTILS_STRBUFF_H

/// @file ym_utils/StrBuff.h
/// @brief StrBuff のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: StrBuff.h 958 2007-08-28 05:38:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ymtools.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class StrBuff StrBuff.h <ym_utils/StrBuff.h>
/// @ingroup YmUtils
/// @brief 文字列バッファ
///
/// 基本的にはただの文字列バッファなので string でも代用できるが
/// たぶんこちらのほうが効率がよい．
//////////////////////////////////////////////////////////////////////
class StrBuff
{
public:
  /// @brief サイズを表す型の定義
  /// @note std::basic_string のまね
  typedef size_t size_type;

  /// @brief 末尾を表す定数
  /// @note std::basic_string のまね
  static const size_type npos = static_cast<size_type>(-1);
  

public:

  /// @brief C文字列からの変換用コンストラクタ
  /// @param[in] str 文字列
  /// @note デフォルトの空コンストラクタでもある．
  StrBuff(const char* str = NULL);

  /// @brief コピーコンストラクタ (StrBuff)
  /// @param[in] src コピー元のオブジェクト (StrBuff)
  StrBuff(const StrBuff& src);

  /// @brief コピーコンストラクタ (string)
  /// @param[in] src コピー元のオブジェクト (string)
  StrBuff(const string& src);

  /// @brief 代入演算子 (StrBuff)
  /// @param[in] src コピー元の文字列 (StrBuff)
  /// @return 自分自身
  const StrBuff&
  operator=(const StrBuff& src);

  /// @brief 代入演算子 (C文字列)
  /// @param[in] src コピー元の文字列 (C文字列)
  /// @return 自分自身
  const StrBuff&
  operator=(const char* src);

  /// @brief 代入演算子 (string)
  /// @param[in] src コピー元の文字列 (string)
  /// @return 自分自身
  const StrBuff&
  operator=(const string& src);

  /// @brief デストラクタ
  ~StrBuff();


  //////////////////////////////////////////////////////////////////////
  /// @name 制御用の関数
  /// @{
  
  /// @brief クリアする．
  void
  clear();

  /// @brief 必要なサイズを指定する．
  void
  reserve(size_t size);

  /// @}
  //////////////////////////////////////////////////////////////////////


  //////////////////////////////////////////////////////////////////////
  /// @name 内容を追加する関数
  /// @{

  /// @brief 一文字追加
  /// @param[in] c 追加する文字
  void
  put_char(int c);

  /// @brief 文字列の追加 (StrBuff)
  /// @param[in] str 追加する文字列 (StrBuff)
  void
  put_str(const StrBuff& str);

  /// @brief 文字列の追加 (C文字列)
  /// @param[in] str 追加する文字列 (C文字列)
  void
  put_str(const char* str);

  /// @brief 文字列の追加 (string)
  /// @param[in] str 追加する文字列 (string)
  void
  put_str(const string& str);

  /// @brief 整数を文字列に変換して追加
  void
  put_digit(int d);

  /// @}
  //////////////////////////////////////////////////////////////////////

  
  //////////////////////////////////////////////////////////////////////
  /// @name 内容の取得
  /// @{

  /// @brief 文字列の長さの取得
  /// @return 文字列の長さ(末尾の '\\0' を含まない)を返す．
  size_type
  size() const;

  /// @brief pos 番目の文字の取得
  /// @param[in] pos 取得する文字の位置
  /// @retval pos 番目の文字
  /// @retval '\\0' 範囲外の場合
  char
  operator[](size_type pos) const;

  /// @brief c が最初に現れる位置の検索
  /// @param[in] c 検索対象の文字
  /// @retval c が最初に現れる位置
  /// @retval npos 見つからない場合
  size_type
  find_first_of(char c) const;

  /// @brief 部分文字列の取得
  /// @param[in] first 部分文字列の開始位置
  /// @param[in] last 部分文字列の終了位置
  /// @return first から last までの部分文字列を切り出す．
  StrBuff
  substr(size_type first,
	 size_type last) const;

  /// @brief Cスタイルの文字列への変換
  /// @return Cスタイルに変換した文字列を返す．
  const char*
  c_str() const;

  /// @brief string への変換
  operator string() const;

  /// @}
  //////////////////////////////////////////////////////////////////////


private:

  // 内部で用いられるコンストラクタ
  // サイズを指定する．
  StrBuff(size_type size);
  

private:

  // src から dst にコピーする．
  void
  copy(const char* src,
       char* dst);
  
  // バッファサイズを拡張する．
  void
  expand(size_type new_size);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // バッファのサイズ
  size_type mSize;

  // 末尾の位置
  size_type mEnd;

  // バッファ本体
  char* mBuffer;
  
};


/// @relates StrBuff
/// @brief 等価比較
/// @param[in] src1, src2 オペランド
/// @return 2つの文字列が等しいときに true を返す．
bool
operator==(const StrBuff& src1,
	   const StrBuff& src2);

/// @relates StrBuff
/// @brief 非等価比較
/// @param[in] src1, src2 オペランド
/// @return 2つの文字列が等しくないときに true を返す．
bool
operator!=(const StrBuff& src1,
	   const StrBuff& src2);

/// @relates StrBuff
/// @brief StrBuff の内容を出力する
/// @param[in] s 出力ストリーム
/// @param[in] strbuf 出力対象の文字列
/// @return s
ostream&
operator<<(ostream& s,
	   const StrBuff& strbuf);


//////////////////////////////////////////////////////////////////////
// インライン定義
//////////////////////////////////////////////////////////////////////

// C文字列からの変換用コンストラクタ
inline
StrBuff::StrBuff(const char* str)
{
  if ( str ) {
    mEnd = strlen(str);
    mSize = mEnd + 1;
    mBuffer = new char[mSize];
    copy(str, mBuffer);
  }
  else {
    mEnd = 0;
    mSize = mEnd + 1;
    mBuffer = new char[mSize];
    mBuffer[mEnd] = '\0';
  }
}

// 内部で用いられるコンストラクタ
// サイズを指定する．
inline
StrBuff::StrBuff(size_type size) :
  mSize(size),
  mEnd(0),
  mBuffer(new char[mSize])
{
  mBuffer[0] = '\0';
}

// コピーコンストラクタ
inline
StrBuff::StrBuff(const StrBuff& src) :
  mSize(src.mSize),
  mEnd(src.mEnd),
  mBuffer(new char[mSize])
{
  copy(src.mBuffer, mBuffer);
}

// コピーコンストラクタ
inline
StrBuff::StrBuff(const string& src) :
  mSize(src.size() + 1),
  mEnd(mSize - 1),
  mBuffer(new char[mSize])
{
  copy(src.c_str(), mBuffer);
}

// 代入演算子
inline
const StrBuff&
StrBuff::operator=(const StrBuff& str)
{
  clear();
  put_str(str);
  return *this;
}
inline
const StrBuff&
StrBuff::operator=(const char* str)
{
  clear();
  put_str(str);
  return *this;
}
inline
const StrBuff&
StrBuff::operator=(const string& str)
{
  clear();
  put_str(str);
  return *this;
}

// デストラクタ
inline
StrBuff::~StrBuff()
{
  delete [] mBuffer;
}

// クリアする．
inline
void
StrBuff::clear()
{
  mBuffer[0] = '\0';
  mEnd = 0;
}

// @brief 必要なサイズを指定する．
inline
void
StrBuff::reserve(size_t size)
{
  if ( mSize < size ) {
    expand(size);
  }
}

// 一文字追加する．
inline
void
StrBuff::put_char(int c)
{
  if ( mEnd >= mSize - 1 ) {
    // バッファの大きさを2倍にする．
    expand(mSize << 1);
  }
  mBuffer[mEnd] = c;
  ++ mEnd;
  mBuffer[mEnd] = '\0';
}

// 文字列を追加する．
inline
void
StrBuff::put_str(const string& str)
{
  put_str(str.c_str());
}

// 文字列を追加する．
inline
void
StrBuff::put_str(const StrBuff& str)
{
  put_str(str.c_str());
}

// 文字列の長さ(末尾の '\0' を含まない)を返す．
inline
StrBuff::size_type
StrBuff::size() const
{
  return mEnd;
}

// pos 番目の文字を返す．
// 範囲外なら '\0' を返す．
inline
char
StrBuff::operator[](size_type pos) const
{
  if ( pos < mEnd ) {
    return mBuffer[pos];
  }
  else {
    // 範囲外
    return '\0';
  }
}

// 文字列を取り出す．
inline
const char*
StrBuff::c_str() const
{
  return mBuffer;
}

// @brief string への変換
inline
StrBuff::operator string() const
{
  return string(c_str());
}

// src から dst にコピーする．
inline
void
StrBuff::copy(const char* src,
	      char* dst)
{
  while ( (*dst ++ = *src ++) ) ;
}

// 等価比較
inline
bool
operator==(const StrBuff& src1,
	   const StrBuff& src2)
{
  return strcmp(src1.c_str(), src2.c_str()) == 0;
}

inline
bool
operator!=(const StrBuff& src1,
	   const StrBuff& src2)
{
  return !operator==(src1, src2);
}

// ストリーム出力
inline
ostream&
operator<<(ostream& s, const StrBuff& strbuf)
{
  return s << strbuf.c_str();
}

END_NAMESPACE_YM

BEGIN_NAMESPACE_HASH

// StrBuff をキーとしたハッシュを使うために必要なクラス定義

template <>
struct hash<nsYm::StrBuff> {
  size_t
  operator()(const nsYm::StrBuff& __x) const {
    unsigned long __h = 0; 
    for (const char* __s = __x.c_str(); *__s; ++ __s)
      __h = 37*__h + *__s;
    return size_t(__h);
  }
};

template <>
struct hash<const nsYm::StrBuff> {
  size_t
  operator()(const nsYm::StrBuff& __x) const {
    unsigned long __h = 0; 
    for (const char* __s = __x.c_str(); *__s; ++ __s)
      __h = 37*__h + *__s;
    return size_t(__h);
  }
};

END_NAMESPACE_HASH

#endif // YM_UTILS_STRBUFF_H
