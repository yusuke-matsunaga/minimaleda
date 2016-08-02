
/// @file libym_utils/StrBuff.h
/// @brief StrBuff の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: StrBuff.cc 958 2007-08-28 05:38:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.

#include "ym_utils/StrBuff.h"


BEGIN_NAMESPACE_YM

// 文字列を追加する．
void
StrBuff::put_str(const char* str)
{
  if ( str ) {
    size_type len = strlen(str) + mEnd;
    if ( len >= mSize ) {
      size_type new_size = mSize << 1;
      while ( len >= new_size ) {
	new_size <<= 1;
      }
      expand(new_size);
    }
    copy(str, mBuffer + mEnd);
    mEnd = len;
  }
}

// @brief 整数を文字列に変換して追加
void
StrBuff::put_digit(int d)
{
  if ( d >= 10 ) {
    put_digit(d / 10);
    d = d % 10;
  }
  put_char(static_cast<char>('0' + d));
}

// c が最初に現れる位置を返す．
StrBuff::size_type
StrBuff::find_first_of(char c) const
{
  for (size_type i = 0; i < mEnd; ++ i) {
    if ( mBuffer[i] == c ) return i;
  }
  return npos;
}

// first から last までの部分文字列を切り出す．
StrBuff
StrBuff::substr(size_type first,
		size_type last) const
{
  if ( last == npos ) {
    last = mEnd;
  }
  size_type len = mEnd - first;
  StrBuff ans(len + 1);
  const char* src = mBuffer + first;
  char* dst = ans.mBuffer;
  for (size_t i = 0; i < len; ++ i) {
    *dst ++ = *src ++;
  }
  *dst = '\0';
  ans.mEnd = len;
  return ans;
}

// バッファサイズを拡張する．
void
StrBuff::expand(size_type new_size)
{
  mSize = new_size;
  char* old = mBuffer;
  mBuffer = new char[new_size];
  copy(old, mBuffer);
  delete [] old;
}

END_NAMESPACE_YM
