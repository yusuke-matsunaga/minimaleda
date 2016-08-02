
/// @file libym_gds/GdsRecored.cc
/// @brief GDS-II のひとかたまりのデータを表すクラス
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: GdsRecord.cc 997 2007-09-07 09:58:29Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_gds/GdsRecord.h"
#include "GdsRecTable.h"


BEGIN_NAMESPACE_YM_GDS

//////////////////////////////////////////////////////////////////////
// Record
//////////////////////////////////////////////////////////////////////

// 先頭のオフセットを取り出す．
size_t
GdsRecord::offset() const
{
  return mOffset;
}

// サイズを取り出す．
size_t
GdsRecord::size() const
{
  return mSize;
}

// レコード型を取り出す．
tGdsRtype
GdsRecord::rtype() const
{
  return mRtype;
}

// レコード型を表すトークンを返す．
int
GdsRecord::rtype_token() const
{
  return GdsRecTable::obj().rtype_token(mRtype);
}

// レコード型を表す文字列を返す．
const char*
GdsRecord::rtype_string() const
{
  return GdsRecTable::obj().rtype_string(mRtype);
}

// データ型を取り出す．
tGdsDtype
GdsRecord::dtype() const
{
  return mDtype;
}

// データ型を表す文字列を返す．
const char*
GdsRecord::dtype_string() const
{
  return GdsRecTable::obj().dtype_string(mDtype);
}

// データサイズを取り出す．
size_t
GdsRecord::dsize() const
{
  return size() - 4;
}

// pos 番目のバイトデータを返す．
tGdsByte
GdsRecord::conv_1byte(size_t pos) const
{
  return mData[pos];
}

// pos 番目の 2バイトのデータを符号つき数(2の補数表現)に変換する．
// kGds2Int 用の変換関数
int
GdsRecord::conv_2byte_int(size_t pos) const
{
  size_t offset = pos * 2;
  ymuint ans = (static_cast<ymuint>(mData[offset]) << 8);
  ans += static_cast<ymuint>(mData[offset + 1]);
  return static_cast<int>(ans);
}

// pos 番目の 4バイトのデータを符号つき数(2の補数表現)に変換する．
// kGds4Int 用の変換関数
int
GdsRecord::conv_4byte_int(size_t pos) const
{
  size_t offset = pos * 4;
  ymuint ans = (static_cast<ymuint>(mData[offset]) << 24);
  ans += (static_cast<ymuint>(mData[offset + 1]) << 16);
  ans += (static_cast<ymuint>(mData[offset + 2]) << 8);
  ans += static_cast<ymuint>(mData[offset + 3]);
  return static_cast<int>(ans);
}

// pos 番目の 4バイトのデータを浮動小数点数に変換する．
// kGds4Real 用の変換関数
double
GdsRecord::conv_4byte_real(size_t pos) const
{
  size_t offset = pos * 4;
  bool zero = true;
  ymuint v[4];
  for (size_t i = 0; i < 4; ++ i) {
    v[i] = static_cast<ymuint>(mData[i + offset]);
    if ( v[i] ) {
      zero = false;
    }
  }
  if ( zero ) {
    // すべてのビットが0なら0
    return 0.0;
  }
  ymuint sign = (v[0] >> 7) & 1;
  ymuint exp = (v[0] & 127); // +64 のゲタをはいている．
  ymuint mag = (v[1] << 16) + (v[2] << 8) + v[0];
  double ans = 0.0;
  double w = 0.5;
  if ( exp >= 64 ) {
    size_t sn = exp - 64;
    for (size_t i = 0; i < sn; ++ i) {
      w *= 16.0;
    }
  }
  else {
    size_t sn = 64 - exp;
    for (size_t i = 0; i < sn; ++ i) {
      w /= 16.0;
    }
  }
  ymuint mask = (1 << 23);
  for (size_t i = 0; i < 24; ++ i) {
    if ( mag & mask ) {
      ans += w;
    }
    mask >>= 1;
    w /= 2.0;
  }
  if ( sign ) {
    ans = -ans;
  }
  return ans;
}

// pos 番目の 8バイトのデータを浮動小数点数に変換する．
// kGds8Real 用の変換関数
double
GdsRecord::conv_8byte_real(size_t pos) const
{
  size_t offset = pos * 8;
  bool zero = true;
  ymuint v[8];
  for (size_t i = 0; i < 8; ++ i) {
    v[i] = static_cast<ymuint>(mData[i + offset]);
    if ( v[i] ) {
      zero = false;
    }
  }
  if ( zero ) {
    // すべてのビットが0なら0
    return 0.0;
  }
  ymuint sign = (v[0] >> 7) & 1;
  ymuint exp = (v[0] & 127); // +64 のゲタをはいている．
  double ans = 0.0;
  double w = 0.5;
  if ( exp >= 64 ) {
    size_t sn = exp - 64;
    for (size_t i = 0; i < sn; ++ i) {
      w *= 16.0;
    }
  }
  else {
    size_t sn = 64 - exp;
    for (size_t i = 0; i < sn; ++ i) {
      w /= 16.0;
    }
  }
  size_t block = 1;
  ymuint mask = (1 << 7);
  for (size_t i = 0; i < 56; ++ i) {
    if ( v[block] & mask ) {
      ans += w;
    }
    mask >>= 1;
    if ( mask == 0 ) {
      ++ block;
      mask = (1 << 7);
    }
    w /= 2.0;
  }
  if ( sign ) {
    ans = -ans;
  }
  return ans;
}

// ptr からはじまる最大 n バイトのデータを文字列に変換する．
// 末尾に '\0'(EndOfString) があるとは限らない．
// kGdsString 用の変換関数
string
GdsRecord::conv_string() const
{
  size_t n = dsize();
  size_t len = n;
  for (size_t i = 0; i < n; ++ i) {
    if ( mData[i] == '\0' ) {
      len = i;
      break;
    }
  }
  string buf;
  for (size_t i = 0; i < len; ++ i) {
    buf.push_back(mData[i]);
  }

  return buf;
}

END_NAMESPACE_YM_GDS
