
/// @file libym_verilog/common/BitVector.cc
/// @brief BitVector の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: BitVector.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_verilog/BitVector.h"


BEGIN_NAMESPACE_YM_VERILOG

BEGIN_NONAMESPACE

// @brief val を表す文字を返す．
inline
char
hexchar(int val)
{
  if ( val <= 9 ) {
    return static_cast<char>('0' + val);
  }
  else {
    return static_cast<char>('A' + val - 10);
  }
}

// @brief val を10進数で表した文字列を得る．
string
size_to_dec_str(ymuint32 val)
{
  string ans;
  if ( val >= 10 ) {
    ymuint32 q = val / 10;
    ans = size_to_dec_str(q);
  }
  ymuint32 r = val % 10;
  char c = static_cast<char>('0' + r);
  ans += c;
  return ans;
}

// @brief a * b の結果を2ワードで返す．
// - <ret-val>.first が high-word
// - <ret-val>.second が low-word
pair<ymuint32, ymuint32>
mult32(ymuint32 a,
       ymuint32 b)
{
  ymuint32 al = a & 0xffff;
  ymuint32 ah = a >> 16;
  ymuint32 bl = b & 0xffff;
  ymuint32 bh = b >> 16;

  ymuint32 tmp_ll = al * bl;
  ymuint32 tmp_lh = al * bh;
  ymuint32 tmp_hl = ah * bl;
  ymuint32 tmp_hh = ah * bh;

  ymuint32 tmp_lll = tmp_ll & 0xffff;
  ymuint32 tmp_llh = tmp_ll >> 16;
  ymuint32 tmp_lhl = tmp_lh & 0xffff;
  ymuint32 tmp_lhh = tmp_lh >> 16;
  ymuint32 tmp_hll = tmp_hl & 0xffff;
  ymuint32 tmp_hlh = tmp_hl >> 16;
  ymuint32 tmp_hhl = tmp_hh & 0xffff;
  ymuint32 tmp_hhh = tmp_hh >> 16;

  ymuint32 c0 = tmp_lll;
  ymuint32 c1 = tmp_llh + tmp_hll + tmp_lhl;
  ymuint32 c1h = c1 >> 16;
  c1 &= 0xffff;
  ymuint32 c2 = c1h + tmp_hlh + tmp_lhh + tmp_hhl;
  ymuint32 c2h = c2 >> 16;
  c2 &= 0xffff;
  ymuint32 c3 = c2h + tmp_hhh;
  
  ymuint32 cl = (c1 << 16) + c0;
  ymuint32 ch = (c3 << 16) + c2;

  return make_pair(ch, cl);
}

END_NONAMESPACE


// @brief ビット長からブロック数を得る．
inline
ymuint32
BitVector::block(ymuint32 size)
{
  return (size + kBlockSize - 1) / kBlockSize;
}

// @brief ビット長から最後のブロックのシフト数を得る．
inline
ymuint32
BitVector::shift(ymuint32 size)
{
  return size - (block(size) - 1) * kBlockSize;
}

// @brief size からマスクパタンを作る
inline
ymuint32
BitVector::mask(ymuint32 size)
{
  return kAll1 >> (kBlockSize - shift(size));
}

// @brief src を 10 で割る．
// @param[in] src 元の値
// @param[out] q 求まった商を格納する変数
// @return 余りを返す．
ymuint32
BitVector::div10(const ymuint32* src,
		 ymuint32 n,
		 ymuint32* q)
{
  ymuint32 l = n * kBlockSize;
  ymuint32 r = 0;
  for (ymuint32 i = l; i -- > 0; ) {
    r <<= 1;
    ymuint32 blk = i / kBlockSize;
    ymuint32 pos = i % kBlockSize;
    if ( (src[blk] >> pos) & 1 ) {
      r |= 1;
    }
    if ( r >= 10 ) {
      r -= 10;
      q[blk] |= 1 << pos;
    }
  }
  return r;
}

// @brief val を10進数で表した文字列を返す．
string
BitVector::dec_str_sub(const ymuint32* val,
		       ymuint32 n)
{
  ymuint32 val1[n];
  for (ymuint32 i = 0; i < n; ++ i) {
    val1[i] = 0;
  }
  ymuint32 r = div10(val, n, val1);
  bool zero = true;
  for (ymuint32 i = 0; i < n; ++ i) {
    if ( val1[i] ) {
      zero = false;
      break;
    }
  }
  string ans;
  if ( !zero ) {
    ans = dec_str_sub(val1, n);
  }
  ans += hexchar(r);
  return ans;
}


//////////////////////////////////////////////////////////////////////
// 定数値を表すクラス
//////////////////////////////////////////////////////////////////////

// 空のコンストラクタ
BitVector::BitVector(ymuint32 val) :
  mSize(0),
  mVal0(NULL),
  mVal1(NULL)
{
  set(~val, val, kVpiSizeInteger, false, false, 10);
}

// @brief 符号なし整数からの代入演算子
// @param[in] val 値
// @note 結果の型は
// - サイズは無し
// - 符号なし
// - 基数は10
const BitVector&
BitVector::operator=(ymuint32 val)
{
  set(~val, val, kVpiSizeInteger, false, false, 10);
  return *this;
}

// int からのキャスト用コンストラクタ
BitVector::BitVector(int val) :
  mSize(0),
  mVal0(NULL),
  mVal1(NULL)
{
  set(~val, val, kVpiSizeInteger, false, true, 10);
}
  
// @brief int からの代入演算子
// @param[in] val 値
// @note 結果の型は
// - サイズは無し
// - 符号あり
// - 基数は10
const BitVector&
BitVector::operator=(int val)
{
  set(~val, val, kVpiSizeInteger, false, true, 10);
  return *this;
}

// bool からの変換コンストラクタ
BitVector::BitVector(bool value) :
  mSize(0),
  mVal0(NULL),
  mVal1(NULL)
{
  if ( value ) {
    set(0, 1, 1, true, false, 2);
  }
  else {
    set(1, 0, 1, true, false, 2);
  }
}
  
// @brief bool からの代入演算子
// @param[in] value ブール値
// @note 結果の型は
// - サイズはあり(1ビット)
// - 符号なし
// - 基数は2
const BitVector&
BitVector::operator=(bool value)
{
  if ( value ) {
    set(0, 1, 1, true, false, 2);
  }
  else {
    set(1, 0, 1, true, false, 2);
  }
  return *this;
}

// @brief time 型からの変換コンストラクタ
BitVector::BitVector(const VlTime& time) :
  mSize(0),
  mVal0(NULL),
  mVal1(NULL)
{
  // サイズあり, 符号無し, 基数10
  resize(kVpiSizeTime);
  set_type(true, false, 10);
  mVal1[0] = time.low();
  mVal1[1] = time.high();
  mVal0[0] = ~mVal1[0];
  mVal0[1] = ~mVal1[1];
}

// @brief time 型からの代入演算子
// @param[in] time time 値
// @note 結果の型は
// - サイズはあり(64ビット)
// - 符号なし
// - 基数は10
const BitVector&
BitVector::operator=(const VlTime& time)
{
  // サイズあり, 符号無し, 基数10
  resize(kVpiSizeTime);
  set_type(true, false, 10);
  mVal1[0] = time.low();
  mVal1[1] = time.high();
  mVal0[0] = ~mVal1[0];
  mVal0[1] = ~mVal1[1];
  return *this;
}

// スカラ値からのキャスト用コンストラクタ
BitVector::BitVector(tVpiScalarVal value,
		     ymuint32 size)
{
  resize(size);
  set_type(true, false, 2);
  ymuint32 n = block(size);
  ymuint32 val0 = kAll0;
  ymuint32 val1 = kAll1;
  switch (value) {
  case kVpiScalar0:
    val0 = kAll1;
    val1 = kAll0;
    break;
  case kVpiScalar1:
    val0 = kAll0;
    val1 = kAll1;
    break;
  case kVpiScalarX:
    val0 = kAll1;
    val1 = kAll1;
    break;
  case kVpiScalarZ:
    val0 = kAll0;
    val1 = kAll0;
    break;
  }
  for (ymuint32 i = 0; i < n - 1; ++ i) {
    mVal0[i] = val0;
    mVal1[i] = val1;
  }
  ymuint32 m = mask(size);
  mVal0[n - 1] = val0 | (~m);
  mVal1[n - 1] = val1 & m;
}

// brief C文字列からの変換用コンストラクタ
BitVector::BitVector(const char* str) :
  mSize(0),
  mVal0(NULL),
  mVal1(NULL)
{
  operator=(str);
}
  
// @brief C文字列からの代入演算子
// @param[in] str 文字列 (C文字列)
// @note 結果の型は
// - サイズはあり
// - 符号なし
// - 基数は2
const BitVector&
BitVector::operator=(const char* str)
{
  if ( str ) {
    ymuint32 strsize = strlen(str);
    set_from_string(strsize, str);
  }
  else {
    // "\0" という文字列にしておく (4.2.3.3)
    set_from_string(1, "\0");
  }
  return *this;
}

// string 文字列からの変換用コンストラクタ
BitVector::BitVector(const string& str) :
  mSize(0),
  mVal0(NULL),
  mVal1(NULL)
{
  operator=(str);
}

// @brief string 文字列からの代入演算子
// @param[in] str 文字列 (string)
// @note 結果の型は
// - サイズはあり
// - 符号なし
// - 基数は2
const BitVector&
BitVector::operator=(const string& str)
{
  ymuint32 strsize = str.size();
  if ( strsize == 0 ) {
    set_from_string(1, "\0");
  }
  else {
    // 上の set_from_string() を使ってもできるけど
    // c_str() を呼びたくない．
    ymuint32 s = strsize * 8;

    resize(s);
    set_type(true, false, 2);

    ymuint32 j = 0;
    ymuint32 k = 0;
    ymuint32 tmp = 0;
    for (ymuint32 i = strsize; i -- > 0; ) {
      char c = str[i];
      tmp += (static_cast<ymuint32>(c) << (k * 8));
      ++ k;
      if ( k == 4 ) {
	mVal0[j] = ~tmp;
	mVal1[j] = tmp;
	++ j;
	k = 0;
	tmp = 0;
      }
    }
    if ( k != 0 ) {
      ymuint32 mask = kAll1 << (k * 8);
      mVal0[j] = ~tmp | mask;
      mVal1[j] = tmp;
    }
  }
  return *this;
}

// 浮動小数点数を整数に変換してからビットベクタにする
BitVector::BitVector(double val) :
  mSize(0),
  mVal0(NULL),
  mVal1(NULL)
{
  double r = rint(val);
  int intval = static_cast<int>(r);
  set(~intval, intval, kBlockSize, false, true, 10);
}

// @brief 浮動小数点数からの代入演算子
// @param[in] val 浮動小数点数
// @warning 整数で表せない範囲の場合には値は不定
// @todo 整数を経由しない方法に書き換えること
const BitVector&
BitVector::operator=(double val)
{
  double r = rint(val);
  int intval = static_cast<int>(r);
  set(~intval, intval, kBlockSize, false, true, 10);
  return *this;
}

// Verilog-HDL 形式の文字列からの変換コンストラクタ
BitVector::BitVector(ymuint32 size,
		     bool is_signed,
		     ymuint32 base,
		     const string& str) :
  mSize(0),
  mVal0(NULL),
  mVal1(NULL)
{
  bool is_sized = true;
  if ( size == 0 ) {
    size = kBlockSize;
    is_sized = false;
  }
  switch ( base ) {
  case 2: set_from_binstring(size, is_sized, is_signed, str, 0); break;
  case 8: set_from_octstring(size, is_sized, is_signed, str, 0); break;
  case 10: set_from_decstring(size, is_sized, is_signed, str, 0); break;
  case 16: set_from_hexstring(size, is_sized, is_signed, str, 0); break;
  default: cerr << "illegal base : " << base << endl;
  }
}

// @brief 連結演算用のコンストラクタ
// @param[in] src_list 連結する値のリスト
// src_list の内容を連結したものをセットする
BitVector::BitVector(const list<BitVector>& src_list) :
  mSize(0),
  mVal0(NULL),
  mVal1(NULL)
{
  ymuint32 tl = 0;
  for (list<BitVector>::const_iterator p = src_list.begin();
       p != src_list.end(); ++p ) {
    tl += p->size();
  }

  resize(tl);
  set_type(true, false, 2);

  ymuint32 blk = 0;
  ymuint32 pos = 0;
  for (list<BitVector>::const_iterator p = src_list.begin();
       p != src_list.end(); ++p ) {
    const BitVector& v = *p;
    ymuint32 l = v.size();
    ymuint32 b = block(l);
    ymuint32 s = shift(l);
    ymuint32 m = mask(l);
    if ( pos == 0 ) {
      for (ymuint32 i = 0; i < b - 1; ++ i) {
	mVal0[blk] = v.mVal0[i];
	mVal1[blk] = v.mVal1[i];
	++ blk;
      }
      mVal0[blk] = v.mVal0[b - 1] & m;
      mVal1[blk] = v.mVal1[b - 1] & m;
      pos = s;
      if ( pos == kBlockSize ) {
	pos = 0;
	++ blk;
      }
    }
    else {
      ymuint32 rpos = kBlockSize - pos;
      for (ymuint32 i = 0; i < b - 1; ++ i) {
	mVal0[blk] |= (v.mVal0[i] << pos);
	mVal1[blk] |= (v.mVal1[i] << pos);
	++ blk;
	mVal0[blk] |= (v.mVal0[i] >> rpos);
	mVal1[blk] |= (v.mVal1[i] >> rpos);
      }
      ymuint32 val0 = v.mVal0[b - 1] & m;
      ymuint32 val1 = v.mVal1[b - 1] & m;
      mVal0[blk] |= (val0 << pos);
      mVal1[blk] |= (val1 << pos);
      pos += s;
      if ( pos >= kBlockSize ) {
	++ blk;
	pos -= kBlockSize;
	if ( pos > 0 ) {
	  mVal0[blk] |= (val0 >> rpos);
	  mVal1[blk] |= (val1 >> rpos);
	}
      }
    }
  }
}

// @brief 連結演算用のコンストラクタ
// @param[in] src_list 連結する値のリスト
// src_list の内容を連結したものをセットする
BitVector::BitVector(const vector<BitVector>& src_list) :
  mSize(0),
  mVal0(NULL),
  mVal1(NULL)
{
  ymuint32 tl = 0;
  for (vector<BitVector>::const_iterator p = src_list.begin();
       p != src_list.end(); ++p ) {
    tl += p->size();
  }

  resize(tl);
  set_type(true, false, 2);

  ymuint32 blk = 0;
  ymuint32 pos = 0;
  for (vector<BitVector>::const_iterator p = src_list.begin();
       p != src_list.end(); ++p ) {
    const BitVector& v = *p;
    ymuint32 l = v.size();
    ymuint32 b = block(l);
    ymuint32 s = shift(l);
    ymuint32 m = mask(l);
    if ( pos == 0 ) {
      for (ymuint32 i = 0; i < b - 1; ++ i) {
	mVal0[blk] = v.mVal0[i];
	mVal1[blk] = v.mVal1[i];
	++ blk;
      }
      mVal0[blk] = v.mVal0[b - 1] & m;
      mVal1[blk] = v.mVal1[b - 1] & m;
      pos = s;
      if ( pos == kBlockSize ) {
	pos = 0;
	++ blk;
      }
    }
    else {
      ymuint32 rpos = kBlockSize - pos;
      for (ymuint32 i = 0; i < b - 1; ++ i) {
	mVal0[blk] |= (v.mVal0[i] << pos);
	mVal1[blk] |= (v.mVal1[i] << pos);
	++ blk;
	mVal0[blk] |= (v.mVal0[i] >> rpos);
	mVal1[blk] |= (v.mVal1[i] >> rpos);
      }
      ymuint32 val0 = v.mVal0[b - 1] & m;
      ymuint32 val1 = v.mVal1[b - 1] & m;
      mVal0[blk] |= (val0 << pos);
      mVal1[blk] |= (val1 << pos);
      pos += s;
      if ( pos >= kBlockSize ) {
	++ blk;
	pos -= kBlockSize;
	if ( pos > 0 ) {
	  mVal0[blk] |= (val0 >> rpos);
	  mVal1[blk] |= (val1 >> rpos);
	}
      }
    }
  }
}

// コピーコンストラクタ
BitVector::BitVector(const BitVector& src) :
  mSize(src.mSize),
  mFlags(src.mFlags)
{
  ymuint32 n = block(mSize);
  mVal0 = new ymuint32[n];
  mVal1 = new ymuint32[n];
  for (ymuint32 i = 0; i < n; ++ i) {
    mVal0[i] = src.mVal0[i];
    mVal1[i] = src.mVal1[i];
  }
}

// 代入演算子
const BitVector&
BitVector::operator=(const BitVector& src)
{
  if ( &src != this ) {
    mSize = src.mSize;
    mFlags = src.mFlags;
    ymuint32 n = block(mSize);
    mVal0 = new ymuint32[n];
    mVal1 = new ymuint32[n];
    for (ymuint32 i = 0; i < n; ++ i) {
      mVal0[i] = src.mVal0[i];
      mVal1[i] = src.mVal1[i];
    }
  }
  return *this;
}

// ビット長の変換を行うコピーコンストラクタもどき
BitVector::BitVector(const BitVector& src,
		     ymuint32 size) :
  mSize(0),
  mVal0(NULL),
  mVal1(NULL)
{
  set(src.mVal0, src.mVal1, src.size(), size, true,
      src.is_signed(), src.base());
}

// ビット長の変換と属性の変更を行うコピーコンストラクタもどき
BitVector::BitVector(const BitVector& src,
		     ymuint32 size,
		     bool is_sized,
		     bool is_signed,
		     ymuint32 base) :
  mSize(0),
  mVal0(NULL),
  mVal1(NULL)
{
  set(src.mVal0, src.mVal1, src.size(), size, is_sized, is_signed, base);
}
  
// @brief スカラ値からの代入演算子
// @param[in] value 値 (kVpiScalar{0, 1, X, Z}
// @note 結果の型は
// - サイズはあり(1ビット)
// - 符号なし
// - 基数は2
const BitVector&
BitVector::operator=(tVpiScalarVal value)
{
  resize(1);
  set_type(true, false, 2);
  switch ( value ) {
  case kVpiScalar0:
    mVal0[0] = 1;
    mVal1[0] = 0;
    break;
  case kVpiScalar1:
    mVal0[0] = 0;
    mVal1[0] = 1;
    break;
  case kVpiScalarX:
    mVal0[0] = 1;
    mVal1[0] = 1;
    break;
  case kVpiScalarZ:
    mVal0[0] = 0;
    mVal1[0] = 0;
    break;
  }
  return *this;
}

// @brief ビット長の変換と属性の変更を行う代入演算子もどき
// @param[in] src 変換元のオブジェクト
// @param[in] size 指定サイズ
// @param[in] is_sized サイズの有無
// @param[in] is_signed 符号の有無
// @param[in] base 基数
void
BitVector::set_with_attr(const BitVector& src,
			 ymuint32 size,
			 bool is_sized,
			 bool is_signed,
			 ymuint32 base)
{
  set(src.mVal0, src.mVal1, src.size(), size, is_sized, is_signed, base);
}

// Verilog-HDL (IEEE1364-2001) の形式の文字列からの値をセットする．
bool
BitVector::set_from_verilog_string(const string& str)
{
  ymuint32 size = kBlockSize; // デフォルト値
  bool is_sized = false; // デフォルト値
  bool is_signed = true; // デフォルト値
  ymuint32 base = 10; // デフォルト値

  // ビット長を求める．
  string::size_type pos = str.find_first_of('\'');
  if ( pos != string::npos ) {
    if ( pos != 0 ) {
      // ' よりも前の部分はビット長を表す文字列
      string lenstr = str.substr(0, pos);
      size = atoi(lenstr.c_str()); // 手抜き
      is_sized = true;
    }
    pos ++; // ' の次の位置を指す．

    // 基数を求める．
    is_signed = false;
    switch ( str[pos] ) {
    case 'b':
    case 'B': base = 2; pos ++; break;
    case 'o':
    case 'O': base = 8; pos ++; break;
    case 'd':
    case 'D': base = 10; pos ++; break;
    case 'h':
    case 'H': base = 16; pos ++; break;
    case 's':
    case 'S':
      {
	pos ++;
	is_signed = true;
	switch ( str[pos] ) {
	case 'b':
	case 'B': base = 2; pos ++; break;
	case 'o':
	case 'O': base = 8; pos ++; break;
	case 'd':
	case 'D': base = 10; pos ++; break;
	case 'h':
	case 'H': base = 16; pos ++; break;
	default:
	  // 本当は例外を投げるのがいいな．
	  cerr << "illegal character (" << str[pos] << ") in string "
	       << str << endl;
	  return false;
	}
      }
      break;
    default:
      // 本当は例外を投げるのがいいな．
      cerr << "illegal character (" << str[pos] << ") in string "
	   << str << endl;
      return false;
    }
  }
  else {
    pos = 0;
  }

  switch ( base ) {
  case 2: set_from_binstring(size, is_sized, is_signed, str, 0); break;
  case 8: set_from_octstring(size, is_sized, is_signed, str, 0); break;
  case 10: set_from_decstring(size, is_sized, is_signed, str, 0); break;
  case 16: set_from_hexstring(size, is_sized, is_signed, str, 0); break;
  default: cerr << "illegal base : " << base << endl;
  }
  
  return true;
}

// @brief 型変換を行う．
// @param[in] type 要求される型(サイズも含む)
void
BitVector::coerce(tVpiValueType type)
{
  if ( type == kVpiValueNone ) {
    return;
  }
  bool is_signed = is_signed_type(type);
  bool is_sized = is_sized_type(type);
  ymuint32 req_size = unpack_size(type);

  if ( size() == req_size ) {
    set_type(is_sized, is_signed, base());
  }
  else {
    set(mVal0, mVal1, size(), req_size, is_sized, is_signed, base());
  }
}

// 0 を表すオブジェクトを生成する
BitVector
BitVector::zero(ymuint32 size)
{
  return BitVector(kVpiScalar0, size);
}

// 1 を表すオブジェクトを生成する
BitVector
BitVector::one(ymuint32 size)
{
  return BitVector(kVpiScalar1, size);
}

// X を表すオブジェクトを生成する
BitVector
BitVector::x(ymuint32 size)
{
  return BitVector(kVpiScalarX, size);
}

// Z を表すオブジェクトを生成する
BitVector
BitVector::z(ymuint32 size)
{
  return BitVector(kVpiScalarZ, size);
}

// Verilog 形式の2進数から変換するための共通ルーティン
void
BitVector::set_from_binstring(ymuint32 size,
			      bool is_sized,
			      bool is_signed,
			      const string& str,
			      ymuint32 pos)
{
  // 文字列の表している値を求める．
  // 実際に格納するものはサイズなどの属性が異なるので set() を用いる．
  string::size_type end = str.size();
  ymuint32 src_size = end - pos;
  ymuint32 src_n = block(src_size);
  ymuint32 val0[src_n];
  ymuint32 val1[src_n];
  for (ymuint32 i = 0; i < src_n; ++ i) {
    val0[i] = kAll1;
    val1[i] = kAll0;
  }
  ymuint32 vpos = src_size - 1;
  for ( ; pos < end; pos ++, vpos --) {
    ymuint32 blk = vpos / kBlockSize;
    ymuint32 sft = vpos - blk * kBlockSize;
    ymuint32 ppat = 1 << sft;
    ymuint32 npat = ~ppat;
    char c = str[pos];
    if ( c == '0' ) {
      val0[blk] |= ppat;
      val1[blk] &= npat;
    }
    else if ( c == '1' ) {
      val0[blk] &= npat;
      val1[blk] |= ppat;
    }
    else if ( c == 'x' || c == 'X' ) {
      val0[blk] |= ppat;
      val1[blk] |= ppat;
    }
    else if ( c == 'z' || c == 'Z' || c == '?' ) {
      val0[blk] &= npat;
      val1[blk] &= npat;
    }
    else {
      // 本当は例外を投げるのがいいな．
      cerr << "illegal character (" << c << ") in string "
	   << str << endl;
      return;
    }
  }

  // この文字列の先頭に - はつかないので結果は必ず非負の数だが，
  // MSBが1の場合にこれをそのまま符号つき数とみなすと符号拡張して
  // しまうのでいったん符号なし数として拡張した後で符号付きに変えている．
  set(val0,val1, src_size, size, is_sized, false, 2);
  if ( is_signed ) {
    set_type(is_sized, true, 2);
  }
}

// Verilog 形式の8進数から変換するための共通ルーティン
void
BitVector::set_from_octstring(ymuint32 size,
			      bool is_sized,
			      bool is_signed,
			      const string& str,
			      ymuint32 pos)
{
  // 文字列の表している値を求める．
  // 実際に格納するものはサイズなどの属性が異なるので set() を用いる．
  string::size_type end = str.size();
  ymuint32 src_size = (end - pos) * 3;
  ymuint32 src_n = block(src_size);
  ymuint32 val0[src_n];
  ymuint32 val1[src_n];
  for (ymuint32 i = 0; i < src_n; ++ i) {
    val0[i] = kAll1;
    val1[i] = kAll0;
  }
  ymuint32 vpos = src_size - 3;
  for ( ; pos < end; pos ++, vpos -= 3) {
    ymuint32 blk = vpos / kBlockSize;
    ymuint32 sft = vpos - blk * kBlockSize;
    char c = str[pos];
    if ( '0' <= c && c <= '7' ) {
      ymuint32 val = c - '0';
      ymuint32 lppat = val << sft;
      ymuint32 rppat = val >> (kBlockSize - sft);
      ymuint32 lnpat = ~lppat;
      ymuint32 rnpat = ~rppat;
      val0[blk] &= lnpat;
      val1[blk] |= lppat;
      if ( rppat ) {
	val0[blk + 1] &= rnpat;
	val1[blk + 1] |= rppat;
      }
    }
    else if ( c == 'x' || c == 'X' ) {
      ymuint32 lppat = 7 << sft;
      ymuint32 rppat = 7 >> (kBlockSize - sft);
      val0[blk] |= lppat;
      val1[blk] |= lppat;
      if ( rppat ) {
	val0[blk + 1] |= rppat;
	val1[blk + 1] |= rppat;
      }
    }
    else if ( c == 'z' || c == 'Z' || c == '?' ) {
      ymuint32 lnpat = ~(7 << sft);
      ymuint32 rnpat = ~(7 >> (kBlockSize - sft));
      val0[blk] &= lnpat;
      val1[blk] &= lnpat;
      if ( rnpat != kAll1 ) {
	val0[blk + 1] &= rnpat;
	val1[blk + 1] &= rnpat;
      }
    }
    else {
      // 本当は例外を投げるのがいいな．
      cerr << "illegal character (" << c << ") in string "
	   << str << endl;
      return;
    }
  }

  // この文字列の先頭に - はつかないので結果は必ず非負の数だが，
  // MSBが1の場合にこれをそのまま符号つき数とみなすと符号拡張して
  // しまうのでいったん符号なし数として拡張した後で符号付きに変えている．
  set(val0, val1, src_size, size, is_sized, false, 8);
  if ( is_signed ) {
    set_type(is_sized, true, 8);
  }
}

// Verilog 形式の10進数から変換するための共通ルーティン
void
BitVector::set_from_decstring(ymuint32 size,
			      bool is_sized,
			      bool is_signed,
			      const string& str,
			      ymuint32 pos)
{
  // 文字列の表している値を求める．
  // 実際に格納するものはサイズなどの属性が異なるので set() を用いる．
  vector<ymuint32> val0;
  vector<ymuint32> val1;
  ymuint32 src_size = 0;
  string::size_type end = str.size();
  const ymuint32 sft = kBlockSize - 4;
  for ( ; pos < end; pos ++) {
    char c = str[pos];
    if ( c < '0' || '9' < c ) {
      // 本当は例外を投げるのがいいな．
      cerr << "illegal character (" << c << ") in string "
	   << str << endl;
      return;
    }
    else {
      ymuint32 n = val1.size();
      ymuint32 carry = c - '0';
      for (ymuint32 i = 0; i < n; ++ i) {
	ymuint32 tmp = val1[i];
	ymuint32 u = tmp >> sft;
	ymuint32 l = tmp - (u << sft);
	ymuint32 u10 = u * 10;
	ymuint32 l10 = l * 10;
	val1[i] = carry + l10 + ((u10 % 16) << sft);
	val0[i] = ~val1[i];
	carry = u10 / 16;
      }
      if ( carry ) {
	val1.push_back(carry);
	val0.push_back(~carry);
      }
    }
  }
  ymuint32 last = val1.size();
  if ( last ) {
    last --;
    for (ymuint32 vpos = kBlockSize; vpos -- > 0; ) {
      if ( val1[last] & (1 << vpos) ) {
	src_size = vpos + last * kBlockSize + 1;
	break;
      }
    }
  }
  else {
    // 1ビットの0を入れておく
    val1.push_back(0);
    val0.push_back(1);
    src_size = 1;
  }

  // この文字列の先頭に - はつかないので結果は必ず非負の数だが，
  // MSBが1の場合にこれをそのまま符号つき数とみなすと符号拡張して
  // しまうのでいったん符号なし数として拡張した後で符号付きに変えている．
  set(val0, val1, src_size, size, is_sized, false, 10);
  if ( is_signed ) {
    set_type(is_sized, true, 10);
  }
}

// Verilog 形式の16進数から変換するための共通ルーティン
void
BitVector::set_from_hexstring(ymuint32 size,
			      bool is_sized,
			      bool is_signed,
			      const string& str,
			      ymuint32 pos)
{
  // 文字列の表している値を求める．
  // 実際に格納するものはサイズなどの属性が異なるので set() を用いる．
  string::size_type end = str.size();
  ymuint32 src_size = (end - pos) * 4;
  ymuint32 src_n = block(src_size);
  ymuint32 val0[src_n];
  ymuint32 val1[src_n];
  for (ymuint32 i = 0; i < src_n; ++ i) {
    val0[i] = kAll1;
    val1[i] = kAll0;
  }
  ymuint32 vpos = src_size - 4;
  for ( ; pos < end; pos ++, vpos -= 4) {
    ymuint32 blk = vpos / kBlockSize;
    ymuint32 sft = vpos - blk * kBlockSize;
    char c = str[pos];
    if ( '0' <= c && c <= '9' ) {
      ymuint32 val = c - '0';
      ymuint32 ppat = val << sft;
      ymuint32 npat = ~ppat;
      val0[blk] &= npat;
      val1[blk] |= ppat;
    }
    else if ( 'a' <= c && c <= 'f' ) {
      ymuint32 val = c - 'a' + 10;
      ymuint32 ppat = val << sft;
      ymuint32 npat = ~ppat;
      val0[blk] &= npat;
      val1[blk] |= ppat;
    }
    else if ( 'A' <= c && c <= 'F' ) {
      ymuint32 val = c - 'A' + 10;
      ymuint32 ppat = val << sft;
      ymuint32 npat = ~ppat;
      val0[blk] &= npat;
      val1[blk] |= ppat;
    }
    else if ( c == 'x' || c == 'X' ) {
      ymuint32 ppat = 15 << sft;
      val0[blk] |= ppat;
      val1[blk] |= ppat;
    }
    else if ( c == 'z' || c == 'Z' || c == '?' ) {
      ymuint32 npat = ~(15 << sft);
      val0[blk] &= npat;
      val1[blk] &= npat;
    }
    else {
      // 本当は例外を投げるのがいいな．
      cerr << "illegal character (" << c << ") in string "
	   << str << endl;
      return;
    }
  }

  // この文字列の先頭に - はつかないので結果は必ず非負の数だが，
  // MSBが1の場合にこれをそのまま符号つき数とみなすと符号拡張して
  // しまうのでいったん符号なし数として拡張した後で符号付きに変えている．
  set(val0, val1, src_size, size, is_sized, false, 16);
  if ( is_signed ) {
    set_type(is_sized, true, 16);
  }
}

// 文字列からの変換用コンストラクタの共通ルーティン
void
BitVector::set_from_string(ymuint32 strsize,
			   const char* str)
{
  ymuint32 s = strsize * 8;

  resize(s);
  set_type(true, false, 2);

  ymuint32 j = 0;
  ymuint32 k = 0;
  ymuint32 tmp = 0;
  for (ymuint32 i = strsize; i -- > 0; ) {
    char c = str[i];
    tmp += (static_cast<ymuint32>(c) << (k * 8));
    ++ k;
    if ( k == 4 ) {
      mVal0[j] = ~tmp;
      mVal1[j] = tmp;
      ++ j;
      k = 0;
      tmp = 0;
    }
  }
  if ( k != 0 ) {
    ymuint32 mask = kAll1 << (k * 8);
    mVal0[j] = ~tmp | mask;
    mVal1[j] = tmp;
  }
}


//////////////////////////////////////////////////////////////////////
// 4.1.5 Arithmetic operators
// 結果の型は以下のように決定される．
// - 符号: ともに signed -> signed
//         上記以外      -> unsigned
// - size: unsized のオペランドは integer とおなじとみなす．
//         で，2つのオペランドの size の大きい方に合わせる．
//         ただし，どちらも unsized なら結果も unsized
// - 基数はなし(10)
//////////////////////////////////////////////////////////////////////

// 2の補数を計算し自分自身に代入する．
const BitVector&
BitVector::complement()
{
  // 中身はほとんど上と同じ
  if ( has_xz() ) {
    return *this = BitVector::x(size());
  }

  // 全ビットを反転して1を足す
  ymuint32 n = block(size());
  bool carry = true;
  ymuint32 m = mask(size());
  for (ymuint32 i = 0; i < n; ++ i) {
    mVal1[i] = mVal0[i];
    if ( carry ) {
      if ( mVal1[i] == kAll1 ) {
	mVal1[i] = kAll0;
	carry = true;
      }
      else {
	mVal1[i] += 1;
	carry = false;
      }
    }
    mVal0[i] = ~mVal1[i];
    if ( i == n - 1) {
      mVal0[i] |= ~m;
      mVal1[i] &= m;
    }
  }

  return *this;
}

// 加算つき代入
const BitVector&
BitVector::operator+=(const BitVector& src)
{
  ymuint32 ans_size = size();
  if ( ans_size < src.size() ) {
    ans_size = src.size();
  }
  bool ans_sized = is_sized() || src.is_sized();
  bool ans_signed = is_signed() && src.is_signed();
  ymuint32 ans_base = base();
  if ( ans_base != src.base() ) {
    // てきとー
    ans_base = 10;
  }

  if ( size() < ans_size ) {
    set(mVal0, mVal1, size(), ans_size, ans_sized, ans_signed, ans_base);
  }

  if ( src.size() < ans_size ) {
    return operator+=(BitVector(src, ans_size));
  }

  if ( has_xz() || src.has_xz() ) {
    return *this = BitVector::x(ans_size);
  }

  set_type(ans_sized, ans_signed, ans_base);

  ymuint32 n = block(size());
  ymuint32 carry = 0;
  for (ymuint32 i = 0; i < n; ++ i) {
    ymuint32 old_val = mVal1[i];
    mVal1[i] += src.mVal1[i] + carry;
    mVal0[i] = ~mVal1[i];
    // carry 条件ってなに？
    // ans のほうが見掛け上小さくなったとき
    if ( mVal1[i] < old_val ) {
      carry = 1;
    }
    else {
      carry = 0;
    }
  }

  return *this;
}

// 減算つき代入
const BitVector&
BitVector::operator-=(const BitVector& src)
{
  ymuint32 ans_size = size();
  if ( ans_size < src.size() ) {
    ans_size = src.size();
  }
  bool ans_sized = is_sized() || src.is_sized();
  bool ans_signed = is_signed() && src.is_signed();
  ymuint32 ans_base = base();
  if ( ans_base != src.base() ) {
    // てきとー
    ans_base = 10;
  }

  if ( size() < ans_size ) {
    set(mVal0, mVal1, size(), ans_size, ans_sized, ans_signed, ans_base);
  }

  if ( src.size() < ans_size ) {
    return operator-=(BitVector(src, ans_size));
  }

  if ( has_xz() || src.has_xz() ) {
    return *this = BitVector::x(ans_size);
  }

  set_type(ans_sized, ans_signed, ans_base);

  ymuint32 n = block(size());
  ymuint32 carry = 1;
  for (ymuint32 i = 0; i < n; ++ i) {
    ymuint32 old_val = mVal1[i];
    mVal1[i] += ~src.mVal1[i] + carry;
    mVal0[i] = ~mVal1[i];
    // carry 条件ってなに？
    // ans のほうが見掛け上小さくなったとき
    if ( mVal1[i] < old_val ) {
      carry = 1;
    }
    else {
      carry = 0;
    }
  }

  return *this;
}

// 乗算つき代入
const BitVector&
BitVector::operator*=(const BitVector& src)
{
  ymuint32 ans_size = size();
  if ( ans_size < src.size() ) {
    ans_size = src.size();
  }
  bool ans_sized = is_sized() || src.is_sized();
  bool ans_signed = is_signed() && src.is_signed();
  ymuint32 ans_base = base();
  if ( ans_base != src.base() ) {
    ans_base = 10;
  }

  if ( size() < ans_size ) {
    set(mVal0, mVal1, size(), ans_size, ans_sized, ans_signed, ans_base);
  }
  if ( src.size() < ans_size ) {
    return operator*=(BitVector(src, ans_size));
  }
  if ( has_xz() || src.has_xz() ) {
    return *this = BitVector::x(ans_size);
  }

  // 符号あわせをしておく．
  BitVector tmp1 = ans_signed && is_negative() ? - *this : *this;
  BitVector tmp2 = ans_signed && src.is_negative() ? - src : src;
  bool invert = ans_signed ? is_negative() ^ src.is_negative() : false;

  set_type(ans_sized, ans_signed, ans_base);

  ymuint32 n = block(size());
  ymuint32 carry = 0;
  for (ymuint32 i = 0; i < n; ++ i) {
    ymuint32 v = carry;
    carry = 0;
    for (ymuint32 j = 0; j <= i; ++ j) {
      ymuint32 old_v = v;
      pair<ymuint32, ymuint32> tmp = mult32(tmp1.mVal1[j], tmp2.mVal1[i - j]);
      v += tmp.second;
      if ( v < old_v ) {
	++ carry;
      }
    }
    for (ymuint32 j = 0; j < i; ++ j) {
      ymuint32 old_v = v;
      pair<ymuint32, ymuint32> tmp = mult32(tmp1.mVal1[j], tmp2.mVal1[i - j - 1]);
      v += tmp.first;
      if ( v < old_v ) {
	++ carry;
      }
    }
    mVal1[i] = v;
    mVal0[i] = ~v;
  }
  // 上位ビットをマスクしておく
  ymuint32 m = mask(ans_size);
  mVal0[n - 1] |= ~m;
  mVal1[n - 1] &= m;

  if ( invert ) {
    complement();
  }

  return *this;
}

// 除算つき代入
const BitVector&
BitVector::operator/=(const BitVector& src)
{
  ymuint32 ans_size = size();
  if ( ans_size < src.size() ) {
    ans_size = src.size();
  }
  bool ans_sized = is_sized() || src.is_sized();
  bool ans_signed = is_signed() && src.is_signed();
  ymuint32 ans_base = base();
  if ( ans_base != src.base() ) {
    ans_base = 10;
  }

  if ( size() < ans_size ) {
    set(mVal0, mVal1, size(), ans_size, ans_sized, ans_signed, ans_base);
  }
  if ( src.size() < ans_size ) {
    return operator/=(BitVector(src, ans_size));
  }
  if ( has_xz() || src.has_xz() ) {
    return *this = BitVector::x(ans_size);
  }

  // 符号あわせをしておく．
  BitVector tmp1 = ans_signed && is_negative() ? - *this : *this;
  BitVector tmp2 = ans_signed && src.is_negative() ? - src : src;
  bool invert = ans_signed ? is_negative() ^ src.is_negative() : false;

  set_type(ans_sized, ans_signed, ans_base);

  tmp1.set_type(tmp1.is_sized(), false, tmp1.base());
  tmp2.set_type(tmp2.is_sized(), false, tmp2.base());

  BitVector p(kVpiScalar0, ans_size);
  for (ymuint32 i = ans_size; i -- > 0; ) {
    p <<= 1;
    if ( tmp1.value(i) == kVpiScalar1 ) {
      p.mVal0[0] &= ~1U;
      p.mVal1[0] |= 1U;
    }
    if ( p >= tmp2 ) {
      p -= tmp2;
      mVal0[i] &= ~1U;
      mVal1[i] |= 1U;
    }
  }

  if ( invert ) {
    complement();
  }

  return *this;
}

// 剰余算
const BitVector&
BitVector::operator%=(const BitVector& src)
{
  ymuint32 ans_size = size();
  if ( ans_size < src.size() ) {
    ans_size = src.size();
  }
  bool ans_sized = is_sized() || src.is_sized();
  bool ans_signed = is_signed() && src.is_signed();
  ymuint32 ans_base = src.base();
  if ( ans_base != src.base() ) {
    ans_base = 10;
  }

  if ( size() < ans_size ) {
    set(mVal0, mVal1, size(), ans_size, ans_sized, ans_signed, ans_base);
  }
  if ( src.size() < ans_size ) {
    return operator%=(BitVector(src, ans_size));
  }
  if ( has_xz() || src.has_xz() ) {
    return *this = BitVector::x(ans_size);
  }

  // 符号あわせをしておく．
  BitVector tmp1 = ans_signed && is_negative() ? - *this : *this;
  BitVector tmp2 = ans_signed && src.is_negative() ? - src : src;
  bool invert = ans_signed ? is_negative() : false;

  tmp2.set_type(tmp2.is_sized(), false, tmp2.base());

  set_type(ans_sized, ans_signed, ans_base);
  *this = BitVector(kVpiScalar0, ans_size);

  for (ymuint32 i = ans_size; i -- > 0; ) {
    operator<<=(1);
    if ( tmp1.value(i) == kVpiScalar1 ) {
      mVal0[0] &= ~1U;
      mVal1[0] |= 1U;
    }
    if ( *this >= tmp2 ) {
      operator-=(tmp2);
    }
  }

  if ( invert ) {
    complement();
  }

  return *this;
}

// 巾乗
const BitVector&
BitVector::power(const BitVector& src)
{
  ymuint32 ans_size = size();
  if ( ans_size < src.size() ) {
    ans_size = src.size();
  }
  bool ans_sized = is_sized() || src.is_sized();
  bool ans_signed = is_signed() && src.is_signed();
  ymuint32 ans_base = base();
  if ( ans_base != src.base() ) {
    ans_base = 10;
  }

  if ( size() < ans_size ) {
    set(mVal0, mVal1, size(), ans_size, ans_sized, ans_signed, ans_base);
  }
  if ( src.size() < ans_size ) {
    return power(BitVector(src, ans_size));
  }
  if ( has_xz() || src.has_xz() ) {
    return *this = BitVector::x(ans_size);
  }

  // 特殊な例: 自分が2ならシフトで代用できる．
  if ( *this == BitVector(2) ) {
    *this = 1;
    return alshift(src);
  }

  // 馬鹿げた実装
  BitVector ans(*this);
  BitVector i(src);
  BitVector c1(1);
  BitVector c0(0);
  for (i = i - c1; i > c0; i = i - c1) {
    ans *= *this;
  }
  return *this = ans;
}


//////////////////////////////////////////////////////////////////////
// 4.1.7 Relational operators
// 結果が x になる場合があるので bool ではなく BitVector を返す．
// 2つのオペランドのサイズが異なっていたら長い方に合わせる．
// ともに符号付きの場合には符号付き数として比較する．
// そうでなければ符号なし数として比較する．
// 結果の型は以下のとおり
// - 符号なし
// - サイズあり(1)
// - 基数は2
//////////////////////////////////////////////////////////////////////

// 小なり
bool
BitVector::lt_base(const BitVector& src1,
		   const BitVector& src2)
{
  bool invert = false;
  if ( src1.is_signed() && src2.is_signed() ) {
    // 符号だけでわかる場合もある．
    bool neg1 = src1.is_negative();
    bool neg2 = src2.is_negative();
    if ( neg1 && !neg2 ) {
      return true;
    }
    if ( !neg1 && neg2 ) {
      return false;
    }
    if ( neg1 && neg2 ) {
      invert = true;
    }
  }

  ymuint32 n = block(src1.size());
  for (ymuint32 i = n; i -- > 0; ) {
    if ( src1.mVal1[i] < src2.mVal1[i] ) {
      return !invert;
    }
    else if ( src1.mVal1[i] > src2.mVal1[i] ) {
      return invert;
    }
  }
  return invert;
}

// 小なり
tVpiScalarVal
lt(const BitVector& src1,
   const BitVector& src2)
{
  if ( src1.has_xz() || src2.has_xz() ) {
    return kVpiScalarX;
  }

  bool ans = false;
  if ( src1.size() < src2.size() ) {
    ans = BitVector::lt_base(BitVector(src1, src2.size()), src2);
  }
  else if ( src1.size() > src2.size() ) {
    ans = BitVector::lt_base(src1, BitVector(src2, src1.size()));
  }
  else {
    ans = BitVector::lt_base(src1, src2);
  }
  if ( ans ) {
    return kVpiScalar1;
  }
  return kVpiScalar0;
}

// 小なり
// こちらは bool にキャストしたバージョン
bool
operator<(const BitVector& src1,
	  const BitVector& src2)
{
  if ( src1.has_xz() || src2.has_xz() ) {
    return false;
  }

  if ( src1.size() < src2.size() ) {
    return BitVector::lt_base(BitVector(src1, src2.size()), src2);
  }
  if ( src1.size() > src2.size() ) {
    return BitVector::lt_base(src1, BitVector(src2, src1.size()));
  }
  return BitVector::lt_base(src1, src2);
}


//////////////////////////////////////////////////////////////////////
// 4.1.8 Equality operators
// 結果が x になる場合があるので bool ではなく BitVector を返す．
// 2つのオペランドのサイズが異なっていたら長い方に合わせる．
// 結果の型は以下のとおり
// - 符号なし
// - サイズあり(1)
// - 基数は2
//////////////////////////////////////////////////////////////////////

// @brief 比較演算のベースとなる関数
// @param[in] src1, src2 オペランド
// @param[in] mode:
//   - 1 普通の等価比較
//   - 2 x をワイルドカードと見なした等価比較
//   - 3 x と z をワイルドカードと見なした等価比較
// @return src1 と src2 が等しいと見なせるとき true を返す．
bool
BitVector::eq_base(const BitVector& src1,
		   const BitVector& src2,
		   int mode)
{
  if ( mode == 1 ) {
    // 通常の等価比較
    return src1.mVal0 == src2.mVal0;
  }
  
  if ( mode == 2 ) {
    // x を 0 または 1 と見なす等価比較
    ymuint32 n = block(src1.size());
    for (ymuint32 i = 0; i < n - 1; ++ i) {
      if ( (src1.mVal0[i] & src2.mVal0[i] | src1.mVal1[i] & src2.mVal1[i])
	   != kAll1 ) {
	return false;
      }
    }
    ymuint32 m = mask(src1.size());
    if ( (src1.mVal0[n - 1] & src2.mVal0[n - 1] |
	  src1.mVal1[n - 1] & src2.mVal1[n - 1] |
	  ~m) != kAll1 ) {
      return false;
    }
    return true;
  }

  // x と z を 0 または 1 と見なす等価比較
  ymuint32 n = block(src1.size());
  for (ymuint32 i = 0; i < n - 1; ++ i) {
    ymuint32 val01 = src1.mVal0[i];
    ymuint32 val11 = src1.mVal1[i];
    ymuint32 val02 = src2.mVal0[i];
    ymuint32 val12 = src2.mVal1[i];
    if ( ((val01 & val02) | (val11 & val12)) != kAll1 &&
	 ((val01 | val02) & (val11 | val12)) != kAll0 ) {
      return false;
    }
  }

  ymuint32 m = mask(src1.size());
  ymuint32 val01 = src1.mVal0[n - 1];
  ymuint32 val11 = src1.mVal1[n - 1];
  ymuint32 val02 = src2.mVal0[n - 1];
  ymuint32 val12 = src2.mVal1[n - 1];
  if ( ((val01 & val02) | (val11 & val12) | ~m ) != kAll1 &&
       ((val01 | val02) & (val11 | val12) &  m ) != kAll0 ) {
    return false;
  }
  return true;
}

// @brief 等価比較演算子
// @param[in] src1, src2 オペランド
// @retval 1 src1 == src2 の時
// @retval 0 src1 != src2 の時
// @retval X 比較不能の時
tVpiScalarVal
eq(const BitVector& src1,
   const BitVector& src2)
{
  if ( src1.has_xz() || src2.has_xz() ) {
    return kVpiScalarX;
  }

  bool ans = false;
  if ( src1.size() < src2.size() ) {
    ans = BitVector::eq_base(BitVector(src1, src2.size()), src2, 1);
  }
  else if ( src1.size() > src2.size() ) {
    ans = BitVector::eq_base(src1, BitVector(src2, src1.size()), 1);
  }
  else {
    ans = BitVector::eq_base(src1, src2, 1);
  }
  if ( ans ) {
    return kVpiScalar1;
  }
  return kVpiScalar0;
}

// @brief x が 0 および 1 と等価と見なせるとした場合の等価比較演算子
// @param[in] src1, src2 オペランド
// @return 等価と見なせるとき true を返す．
bool
eq_with_x(const BitVector& src1,
	  const BitVector& src2)
{
  if ( src1.size() < src2.size() ) {
    return BitVector::eq_base(BitVector(src1, src2.size()), src2, 2);
  }
  if ( src1.size() > src2.size() ) {
    return BitVector::eq_base(src1, BitVector(src2, src1.size()), 2);
  }
  return BitVector::eq_base(src1, src2, 2);
}

// @brief x および z が 0 および 1 と等価と見なせるとした場合の等価比較演算子
// @param[in] src1, src2 オペランド
// @return 等価と見なせるとき true を返す．
bool
eq_with_xz(const BitVector& src1,
	   const BitVector& src2)
{
  if ( src1.size() < src2.size() ) {
    return BitVector::eq_base(BitVector(src1, src2.size()), src2, 3);
  }
  if ( src1.size() > src2.size() ) {
    return BitVector::eq_base(src1, BitVector(src2, src1.size()), 3);
  }
  return BitVector::eq_base(src1, src2, 3);
}

// @brief 等価比較演算子 (bool)
// @param[in] src1, src2 オペランド
// @return 1 src1 == src2 を返す．
bool
operator==(const BitVector& src1,
	   const BitVector& src2)
{
  if ( src1.has_xz() || src2.has_xz() ) {
    return false;
  }

  if ( src1.size() < src2.size() ) {
    return BitVector::eq_base(BitVector(src1, src2.size()), src2, 1);
  }
  if ( src1.size() > src2.size() ) {
    return BitVector::eq_base(src1, BitVector(src2, src1.size()), 1);
  }
  return BitVector::eq_base(src1, src2, 1);
}


//////////////////////////////////////////////////////////////////////
// 4.1.9 Logical operators (scalar)
// 結果が x になる場合があるので bool ではなく BitVector を返す．
// オペランドがスカラーで無い場合には強制的にスカラーに変換する．
// 具体的には最下位ビットをそのままスカラー値だと思う．
// 結果の型は以下のとおり
// - 符号なし
// - サイズあり(1)
// - 基数は2
//////////////////////////////////////////////////////////////////////

// 否定
tVpiScalarVal
operator!(const BitVector& src)
{
  return !src.to_logic();
}

// 論理積
tVpiScalarVal
operator&&(const BitVector& src1,
	   const BitVector& src2)
{
  tVpiScalarVal v1 = src1.to_logic();
  tVpiScalarVal v2 = src2.to_logic();
  return v1 && v2;
}

// 論理和
tVpiScalarVal
operator||(const BitVector& src1,
	   const BitVector& src2)
{
  tVpiScalarVal v1 = src1.to_logic();
  tVpiScalarVal v2 = src2.to_logic();
  return v1 || v2;
}


//////////////////////////////////////////////////////////////////////
// 4.1.10 Bit-wise operators
// 2つのオペランドの長さが合わないときは長い方に合わせる．
// この場合は常に0が埋められる．
// 結果の型は以下のとおり
// - 符号: ともに signed -> signed
//         上記以外      -> unsigned
// - size: unsized のオペランドは integer とおなじとみなす．
//         で，2つのオペランドの size の大きい方に合わせる．
//         ただし，どちらも unsized なら結果も unsized
// - 基数は2
//////////////////////////////////////////////////////////////////////

// 自分自身の値をビットごとに反転させる
const BitVector&
BitVector::negate()
{
  // 基本的には val0 と val1 を交換すればよいが，
  // z を含んでいたときが例外となる．
  // z のビットは x にするのでともに1と立てる
  ymuint32 n = block(size());
  for (ymuint32 i = 0; i < n; ++ i) {
    ymuint32 pat0 = mVal0[i];
    ymuint32 pat1 = mVal1[i];
    ymuint32 zpat = ~pat0 & ~pat1; // 1 なら Z
    mVal0[i] = pat1 | zpat;
    mVal1[i] = pat0 | zpat;
  }
  return *this;
}

// 論理積つき代入
const BitVector&
BitVector::operator&=(const BitVector& src)
{
  ymuint32 ans_size = size();
  if ( ans_size < src.size() ) {
    ans_size = src.size();
  }
  bool ans_sized = is_sized() || src.is_sized();
  bool ans_signed = is_signed() && src.is_signed();
  ymuint32 ans_base = base();
  if ( ans_base != src.base() ) {
    // てきとー
    ans_base = 2;
  }

  if ( size() < ans_size ) {
    set(mVal0, mVal1, size(), ans_size, ans_sized, ans_signed, ans_base);
  }

  if ( src.size() < ans_size ) {
    return operator&=(BitVector(src, ans_size));
  }

  set_type(ans_sized, ans_signed, ans_base);

  // 基本的には val0 を or して val1 を and すればよいが，
  // z を含んでいたときが例外となる．
  ymuint32 n = block(size());
  for (ymuint32 i = 0; i < n; ++ i) {
    ymuint32 val1_0 = mVal0[i];
    ymuint32 val1_1 = mVal1[i];
    ymuint32 zpat;
    // z のパタンを x に直しておく．
    zpat = ~val1_0 & ~val1_1;
    val1_0 |= zpat;
    val1_1 |= zpat;
    ymuint32 val2_0 = src.mVal0[i];
    ymuint32 val2_1 = src.mVal1[i];
    // z のパタンを x に直しておく．
    zpat = ~val2_0 & ~val2_1;
    val2_0 |= zpat;
    val2_1 |= zpat;

    mVal0[i] = val1_0 | val2_0;
    mVal1[i] = val1_1 & val2_1;
  }

  return *this;
}

// 論理和つき代入
const BitVector&
BitVector::operator|=(const BitVector& src)
{
  ymuint32 ans_size = size();
  if ( ans_size < src.size() ) {
    ans_size = src.size();
  }
  bool ans_sized = is_sized() || src.is_sized();
  bool ans_signed = is_signed() && src.is_signed();
  ymuint32 ans_base = base();
  if ( ans_base != src.base() ) {
    // てきとー
    ans_base = 2;
  }

  if ( size() < ans_size ) {
    set(mVal0, mVal1, size(), ans_size, ans_sized, ans_signed, ans_base);
  }

  if ( src.size() < ans_size ) {
    return operator|=(BitVector(src, ans_size));
  }

  set_type(ans_sized, ans_signed, ans_base);

  // 基本的には val0 を and して val1 を or すればよいが，
  // z を含んでいたときが例外となる．
  ymuint32 n = block(size());
  for (ymuint32 i = 0; i < n; ++ i) {
    ymuint32 val1_0 = mVal0[i];
    ymuint32 val1_1 = mVal1[i];
    ymuint32 zpat;
    // z のパタンを x に直しておく．
    zpat = ~val1_0 & ~val1_1;
    val1_0 |= zpat;
    val1_1 |= zpat;
    ymuint32 val2_0 = src.mVal0[i];
    ymuint32 val2_1 = src.mVal1[i];
    // z のパタンを x に直しておく．
    zpat = ~val2_0 & ~val2_1;
    val2_0 |= zpat;
    val2_1 |= zpat;

    mVal0[i] = val1_0 & val2_0;
    mVal1[i] = val1_1 | val2_1;
  }

  return *this;
}

// 排他的論理和
const BitVector&
BitVector::operator^=(const BitVector& src)
{
  ymuint32 ans_size = size();
  if ( ans_size < src.size() ) {
    ans_size = src.size();
  }
  bool ans_sized = is_sized() || src.is_sized();
  bool ans_signed = is_signed() && src.is_signed();
  ymuint32 ans_base = base();
  if ( ans_base != src.base() ) {
    // てきとー
    ans_base = 2;
  }

  if ( size() < ans_size ) {
    set(mVal0, mVal1, size(), ans_size, ans_sized, ans_signed, ans_base);
  }

  if ( src.size() < ans_size ) {
    return operator^=(BitVector(src, ans_size));
  }

  set_type(ans_sized, ans_signed, ans_base);

  // 計算の仕方は否定とANDとORを組み合わせたもの
  ymuint32 n = block(size());
  for (ymuint32 i = 0; i < n; ++ i) {
    ymuint32 val1_0 = mVal0[i];
    ymuint32 val1_1 = mVal1[i];
    ymuint32 zpat;
    // z のパタンを x に直しておく．
    zpat = ~val1_0 & ~val1_1;
    val1_0 |= zpat;
    val1_1 |= zpat;
    ymuint32 val2_0 = mVal0[i];
    ymuint32 val2_1 = mVal1[i];
    // z のパタンを x に直しておく．
    zpat = ~val2_0 & ~val2_1;
    val2_0 |= zpat;
    val2_1 |= zpat;

    mVal0[i] = (val1_0 | val2_1) & (val1_1 | val2_0);
    mVal1[i] = (val1_1 & val2_0) | (val1_0 & val2_1);
  }

  return *this;
}


//////////////////////////////////////////////////////////////////////
// 4.1.11 Reduction operators
// 結果の型は以下のとおり
// - 符号なし
// - サイズあり(1)
// - 基数は2
//////////////////////////////////////////////////////////////////////

// リダクションAND
tVpiScalarVal
BitVector::reduction_and() const
{
  if ( has_xz() ) {
    return kVpiScalarX;
  }

  ymuint32 n = block(size());
  ymuint32 m = mask(size());
  for (ymuint32 i = 0; i < n - 1; ++ i) {
    if ( mVal0[i] ) {
      // 1 ビットでも 0 のビットがあれば結果は0
      return kVpiScalar0;
    }
  }
  if ( mVal0[n - 1] & m ) {
    // 1 ビットでも 0 のビットがあれば結果は0
    return kVpiScalar0;
  }
  return kVpiScalar1;
}

// リダクションNAND
tVpiScalarVal
BitVector::reduction_nand() const
{
  if ( has_xz() ) {
    return kVpiScalarX;
  }

  ymuint32 n = block(size());
  ymuint32 m = mask(size());
  for (ymuint32 i = 0; i < n - 1; ++ i) {
    if ( mVal0[i] ) {
      // 1 ビットでも 0 のビットがあれば結果は1
      return kVpiScalar1;
    }
  }
  if ( mVal0[n - 1] & m ) {
    // 1 ビットでも 0 のビットがあれば結果は1
    return kVpiScalar1;
  }
  return kVpiScalar0;
}

// リダクションOR
tVpiScalarVal
BitVector::reduction_or() const
{
  if ( has_xz() ) {
    return kVpiScalarX;
  }
  ymuint32 n = block(size());
  ymuint32 m = mask(size());
  for (ymuint32 i = 0; i < n - 1; ++ i) {
    if ( mVal1[i] ) {
      // 1 ビットでも 1 のビットがあれば結果は1
      return kVpiScalar1;
    }
  }
  if ( mVal1[n - 1] & m ) {
    // 1 ビットでも 1 のビットがあれば結果は1
    return kVpiScalar1;
  }
  return kVpiScalar0;
}

// リダクションNOR
tVpiScalarVal
BitVector::reduction_nor() const
{
  if ( has_xz() ) {
    return kVpiScalarX;
  }
  ymuint32 n = block(size());
  ymuint32 m = mask(size());
  for (ymuint32 i = 0; i < n - 1; ++ i) {
    if ( mVal1[i] ) {
      // 1 ビットでも 1 のビットがあれば結果は0
      return kVpiScalar0;
    }
  }
  if ( mVal1[n - 1] & m ) {
    // 1 ビットでも 1 のビットがあれば結果は0
    return kVpiScalar0;
  }
  return kVpiScalar1;
}

// リダクションXOR
tVpiScalarVal
BitVector::reduction_xor() const
{
  if ( has_xz() ) {
    return kVpiScalarX;
  }
  ymuint32 n = block(size());
  // xor は地道に数えるしかない
  // 高速に行うには8ビット単位ぐらいで表引きをすればよい．
  ymuint32 v = 0;
  for (ymuint32 i = 0; i < n - 1; ++ i) {
    for (ymuint32 b = 0; b < kBlockSize; b ++) {
      if ( (mVal1[i] >> b) & 1 ) {
	v ^= 1;
      }
    }
  }
  ymuint32 s = shift(size());
  for (ymuint32 b = 0; b < s; b ++) {
    if ( (mVal1[n - 1] >> b) & 1 ) {
      v ^= 1;
    }
  }
  if ( v ) {
    return kVpiScalar1;
  }
  else {
    return kVpiScalar0;
  }
}

// リダクションXNOR
tVpiScalarVal
BitVector::reduction_xnor() const
{
  if ( has_xz() ) {
    return kVpiScalarX;
  }
  ymuint32 n = block(size());
  // xor は地道に数えるしかない
  // 高速に行うには8ビット単位ぐらいで表引きをすればよい．
  ymuint32 v = 0;
  for (ymuint32 i = 0; i < n - 1; ++ i) {
    for (ymuint32 b = 0; b < kBlockSize; b ++) {
      if ( (mVal1[i] >> b) & 1 ) {
	v ^= 1;
      }
    }
  }
  ymuint32 s = shift(size());
  for (ymuint32 b = 0; b < s; b ++) {
    if ( (mVal1[n - 1] >> b) & 1 ) {
      v ^= 1;
    }
  }
  if ( v ) {
    return kVpiScalar0;
  }
  else {
    return kVpiScalar1;
  }
}


//////////////////////////////////////////////////////////////////////
// 4.1.12 Shift operators
// 結果の型は以下のとおり
// - 符号   : 第1オペランドと同じ
// - サイズ : 第1オペランドと同じ
// - 基数   : 第1オペランドと同じ
//////////////////////////////////////////////////////////////////////

// 論理左シフトつき代入
const BitVector&
BitVector::operator<<=(const BitVector& src)
{
  if ( src.has_xz() ) {
    return *this = BitVector::x(size());
  }

  if ( !src.is_uint32() ) {
    cerr << "error in operator<<: right operand too big" << endl;
    return *this = BitVector::x(size());
  }

  return operator<<=(src.to_uint32());
}

// 論理左シフトつき代入
// src2 が ymuint32 のバージョン
const BitVector&
BitVector::operator<<=(ymuint32 sft)
{
  if ( sft == 0 ) {
    // シフト量が0のときはそのまま返す
    return *this;
  }

  ymuint32 n = block(size());
  ymuint32 bit_sft = sft % kBlockSize; // ブロック内のシフト量
  ymuint32 blk_sft = sft / kBlockSize; // ブロック単位のシフト量
  ymuint32 rbit_sft = kBlockSize - bit_sft;
  ymuint32 en = n - blk_sft;

  if ( bit_sft == 0 ) {
    // この向きでコピーしないと破壊してしまう．
    for (ymuint32 i = en; i -- > 0; ) {
      mVal0[i + blk_sft] = mVal0[i];
      mVal1[i + blk_sft] = mVal1[i];
    }
  }
  else {
    for (ymuint32 i = en; i -- > 0; ) {
      ymuint32 lval0 = (mVal0[i] << bit_sft);
      ymuint32 lval1 = (mVal1[i] << bit_sft);
      ymuint32 rval0 = ((i > 0) ? mVal0[i - 1] : kAll1) >> rbit_sft;
      ymuint32 rval1 = ((i > 0) ? mVal1[i - 1] : kAll0) >> rbit_sft;
      mVal0[i + blk_sft] = lval0 | rval0;
      mVal1[i + blk_sft] = lval1 | rval1;
    }
  }
  // 空のブロックを詰める
  for (ymuint32 i = 0; i < blk_sft; ++ i) {
    mVal0[i] = kAll1;
    mVal1[i] = kAll0;
  }

  return *this;
}

// 論理右シフトつき代入
const BitVector&
BitVector::operator>>=(const BitVector& src)
{
  if ( src.has_xz() ) {
    return *this = BitVector::x(size());
  }

  if ( !src.is_uint32() ) {
    cerr << "error in operator>>: right operand too big" << endl;
    return *this = BitVector::x(size());
  }

  return operator>>=(src.to_uint32());
}

// 論理右シフトつき代入
// src2 が ymuint32 のバージョン
const BitVector&
BitVector::operator>>=(ymuint32 sft)
{
  if ( sft == 0 ) {
    // シフト量が0の時はそのまま返す．
    return *this;
  }

  ymuint32 n = block(size());
  ymuint32 bit_sft = sft % kBlockSize; // ブロック内のシフト量
  ymuint32 blk_sft = sft / kBlockSize; // ブロック単位のシフト量
  ymuint32 lbit_sft = kBlockSize - bit_sft;

  if ( bit_sft == 0 ) {
    for (ymuint32 i = blk_sft; i < n; ++ i) {
      mVal0[i - blk_sft] = mVal0[i];
      mVal1[i - blk_sft] = mVal1[i];
    }
  }
  else {
    for (ymuint32 i = blk_sft; i < n; ++ i) {
      ymuint32 r0 = mVal0[i] >> bit_sft;
      ymuint32 r1 = mVal1[i] >> bit_sft;
      ymuint32 l0 = ((i < n - 1) ? mVal0[i + 1] : kAll1) << lbit_sft;
      ymuint32 l1 = ((i < n - 1) ? mVal1[i + 1] : kAll0) << lbit_sft;
      mVal0[i - blk_sft] = l0 | r0;
      mVal1[i - blk_sft] = l1 | r1;
    }
  }
  // 上位ビットに0を詰めておく．
  for (ymuint32 i = n - blk_sft; i < n; ++ i) {
    mVal0[i] = kAll1;
    mVal1[i] = kAll0;
  }
  return *this;
}

// 算術右シフトつき代入
const BitVector&
BitVector::arshift(const BitVector& src)
{
  if ( src.has_xz() ) {
    return *this = BitVector::x(size());
  }
  
  if ( !src.is_uint32() ) {
    cerr << "error in operator<<: right operand too big" << endl;
    return *this = BitVector::x(size());
  }

  return arshift(src.to_uint32());
}

// 算術右シフトつき代入
// src が ymuint32 のバージョン
const BitVector&
BitVector::arshift(ymuint32 sft)
{
  if ( sft == 0 ) {
    // シフト量が0の時はそのまま返す．
    return *this;
  }

  ymuint32 n = block(size());
  ymuint32 bit_sft = sft % kBlockSize; // ブロック内のシフト量
  ymuint32 blk_sft = sft / kBlockSize; // ブロック単位のシフト量
  ymuint32 lbit_sft = kBlockSize - bit_sft;

  ymuint32 pad0 = kAll1;
  ymuint32 pad1 = kAll0;
  if ( is_negative() ) {
    pad0 = kAll0;
    pad1 = kAll1;
  }

  if ( bit_sft == 0 ) {
    for (ymuint32 i = blk_sft; i < n; ++ i) {
      mVal0[i - blk_sft] = mVal0[i];
      mVal1[i - blk_sft] = mVal1[i];
    }
  }
  else {
    for (ymuint32 i = blk_sft; i < n; ++ i) {
      ymuint32 r0 = mVal0[i] >> bit_sft;
      ymuint32 r1 = mVal1[i] >> bit_sft;
      ymuint32 l0 = ((i < n - 1) ? mVal0[i + 1] : pad0) << lbit_sft;
      ymuint32 l1 = ((i < n - 1) ? mVal1[i + 1] : pad1) << lbit_sft;
      mVal0[i - blk_sft] = l0 | r0;
      mVal1[i - blk_sft] = l1 | r1;
    }
  }
  for (ymuint32 i = n - blk_sft; i < n; ++ i) {
    mVal0[i] = pad0;
    mVal1[i] = pad1;
  }
  return *this;
}


//////////////////////////////////////////////////////////////////////
// 4.1.13 Conditional operators
// 第1オペランドは強制的にスカラーに変換される．
// 結果の型は以下のとおり
// - 符号   : 第2,第3オペランドがともに signed -> signed
//            上記以外 -> unsigned
// - サイズ : 第2,第3オペランドの長い方に合わせる．
//            ただし unsized なら integer と同じと見なす．
//            ともに unsized なら 結果も unsized
// - 基数   : 第1オペランドが 1 の時には 第2オペランドの基数
//            0 の時には第3オペランドの基数
//            x, z の時に第2オペランドと第3オペランドの基数が異
//            なっていたら2とする．
//////////////////////////////////////////////////////////////////////

// 条件演算
BitVector
ite(const BitVector& src1,
    const BitVector& src2,
    const BitVector& src3)
{
  bool has_size = src1.is_sized() || src2.is_sized();
  ymuint32 size = src1.size();
  if ( size < src2.size() ) {
    size = src2.size();
  }
  bool has_sign = src1.is_signed() && src2.is_signed();
  switch ( src1.to_logic() ) {
  case kVpiScalar0:
    return BitVector(src3, size, has_size, has_sign, src3.base());
  case kVpiScalar1:
    return BitVector(src2, size, has_size, has_sign, src2.base());
  default:
    return BitVector(src2).merge(src3);
  }
}


//////////////////////////////////////////////////////////////////////
// 4.1.14 Concatenation
// 結果の型は以下の通り
// - 符号   : 常に unsigned
// - サイズ : オペランドのサイズの和．もしもオペランドに unsized
//            な値が含まれていたらエラー(今はintegerと見なす)
// - 基数   : 2
//////////////////////////////////////////////////////////////////////

// @brief 連結演算
// @param[in] src_list 連結する値のリスト
// @return 連結した値
BitVector
concat(const list<BitVector>& src_list)
{
  return BitVector(src_list);
}

// @brief 連結演算
// @param[in] src_list 連結する値のリスト
// @return 連結した値
BitVector
concat(const vector<BitVector>& src_list)
{
  return BitVector(src_list);
}

// @brief 繰り返し連結演算
// @param[in] rep 繰り返し数
// @param[in] src_list 連結する値のリスト
// @return 連結した値
BitVector
multi_concat(const BitVector& rep,
	     const list<BitVector>& src_list)
{
  if ( !rep.is_uint32() ) {
    // 手抜き
    return BitVector::x();
  }

  // まず src_list の連結を行う．
  BitVector unit(src_list);

  // それを rep 回繰り返す．
  ymuint32 n = rep.to_uint32();
  vector<BitVector> tmp_list(n, unit);
  return BitVector(tmp_list);
}

// @brief 繰り返し連結演算
// @param[in] rep 繰り返し数
// @param[in] src_list 連結する値のリスト
// @return 連結した値
BitVector
multi_concat(const BitVector& rep,
	     const vector<BitVector>& src_list)
{
  if ( !rep.is_uint32() ) {
    // 手抜き
    return BitVector::x();
  }

  // まず src_list の連結を行う．
  BitVector unit(src_list);

  // それを rep 回繰り返す．
  ymuint32 n = rep.to_uint32();
  vector<BitVector> tmp_list(n, unit);
  return BitVector(tmp_list);
}


//////////////////////////////////////////////////////////////////////
// 4.2.1 Vector bit-select and part-select addressing
// 範囲外は x を返す．
// 結果の型は以下の通り
// - 符号   : 常に unsigned
// - サイズ : 指定された値
// - 基数   : 2
//////////////////////////////////////////////////////////////////////

// part-select 演算子
BitVector
BitVector::part_select(int msb,
		       int lsb) const
{
  if ( msb < lsb ) {
    // 狂ってる
    return BitVector::x();
  }

  ymuint32 new_size = msb - lsb + 1;
  if ( lsb >= static_cast<int>(size()) || msb < 0 ) {
    // 完全に範囲外
    return BitVector::x(new_size);
  }

  BitVector ans(kVpiScalarX, new_size);
  // とりあえず参照コードということでビット単位に転送してます．
  // 本当は part_select 書き込みのようなブロック転送のコードの方が速い
  ymuint32 start = 0;
  ymuint32 end = new_size - 1;
  if ( lsb < 0 ) {
    start = -lsb;
  }
  if ( msb >= static_cast<int>(size()) ) {
    end -= msb - size() + 1;
  }
  for (ymuint32 i = start; i <= end; ++ i) {
    ans.set_value(i, value(i + lsb));
  }
  return ans;
}

// part-select 書き込み
// [msb:lsb] の範囲に val を書き込む
// 範囲外ならなにもしない．
void
BitVector::part_select(int msb,
		       int lsb,
		       const BitVector& val)
{
  if ( msb < lsb ) {
    // 狂ってる．
    return;
  }
  
  ymuint32 l = msb - lsb + 1;
  ymuint32 src_blk = block(l);
  ymuint32 src_mask = mask(l);

  ymuint32 blk0 = lsb / kBlockSize;
  ymuint32 sft0 = lsb - blk0 * kBlockSize;

  if ( sft0 == 0 ) {
    for (ymuint32 i = 0; i < src_blk - 1; ++ i) {
      mVal0[i + blk0] = val.mVal0[i];
      mVal1[i + blk0] = val.mVal1[i];
    }
    mVal0[src_blk + blk0 - 1] &= ~src_mask;
    mVal0[src_blk + blk0 - 1] |=  src_mask & val.mVal0[src_blk - 1];
    mVal1[src_blk + blk0 - 1] &= ~src_mask;
    mVal1[src_blk + blk0 - 1] |=  src_mask & val.mVal1[src_blk - 1];
  }
  else {
    if ( src_blk == 1 ) {
      ymuint32 val0 = val.mVal0[0] & src_mask;
      ymuint32 val1 = val.mVal1[0] & src_mask;
      ymuint32 lval0 = val0 << sft0;
      ymuint32 rval0 = val0 >> (kBlockSize - sft0);
      ymuint32 lval1 = val1 << sft0;
      ymuint32 rval1 = val1 >> (kBlockSize - sft0);
      ymuint32 lsrc_mask = src_mask << sft0;
      ymuint32 rsrc_mask = src_mask >> (kBlockSize - sft0);
      mVal0[blk0] &= ~lsrc_mask;
      mVal0[blk0] |=  lval0;
      mVal1[blk0] &= ~lsrc_mask;
      mVal1[blk0] |=  lval1;
      if ( rsrc_mask ) {
	mVal0[blk0 + 1] &= ~rsrc_mask;
	mVal0[blk0 + 1] |=  rval0;
	mVal1[blk0 + 1] &= ~rsrc_mask;
	mVal1[blk0 + 1] |=  rval1;
      }
    }
    else {
      ymuint32 val0 = val.mVal0[0];
      ymuint32 val1 = val.mVal1[0];
      ymuint32 lval0 = val0 << sft0;
      ymuint32 rval0 = val0 >> (kBlockSize - sft0);
      ymuint32 lval1 = val1 << sft0;
      ymuint32 rval1 = val1 >> (kBlockSize - sft0);
      ymuint32 lmask = kAll1 << sft0;
      ymuint32 rmask = kAll1 >> (kBlockSize - sft0);
      mVal0[blk0] &= ~lmask;
      mVal0[blk0] |= lval0;
      mVal1[blk0] &= ~lmask;
      mVal1[blk0] |= lval1;
      for (ymuint32 i = 1; i < src_blk - 1; ++ i) {
	val0 = val.mVal0[i];
	val1 = val.mVal1[i];
	lval0 = val0 << sft0;
	lval1 = val1 << sft0;
	mVal0[blk0 + i] = lval0 | rval0;
	mVal1[blk0 + i] = lval1 | rval1;
	rval0 = val0 >> (kBlockSize - sft0);
	rval1 = val1 >> (kBlockSize - sft0);
      }
      lval0 = (val.mVal0[src_blk - 1] & src_mask) << sft0;
      lval1 = (val.mVal1[src_blk - 1] & src_mask) << sft0;
      ymuint32 lsrc_mask = src_mask << sft0;
      mVal0[blk0 + src_blk - 1] &= ~(lsrc_mask | rmask);
      mVal0[blk0 + src_blk - 1] |= lval0 | rval0;
      mVal1[blk0 + src_blk - 1] &= ~(lsrc_mask | rmask);
      mVal1[blk0 + src_blk - 1] |= lval1 | rval1;
    }
  }
}

// bit-select 書き込み
// bpos の位置に val を書き込む
// 範囲外ならなにもしない．
void
BitVector::bit_select(int bpos,
		      tVpiScalarVal val)
{
# warning "TODO: 専用のコードを書く"

  // 効率悪いけどコードを共通化した方がバグが少なくなる．
  part_select(bpos, bpos, BitVector(val));
}


//////////////////////////////////////////////////////////////////////
// その他のメンバ関数
//////////////////////////////////////////////////////////////////////

// srcの値をビットごとにマージする．
// 異なっているビットは X となる．
const BitVector&
BitVector::merge(const BitVector& src)
{
  ymuint32 ans_size = size();
  if ( ans_size < src.size() ) {
    ans_size = src.size();
  }
  bool ans_sized = is_sized() || src.is_sized();
  bool ans_signed = is_signed() && src.is_signed();
  ymuint32 ans_base = base();
  if ( ans_base != src.base() ) {
    ans_base = 2;
  }
  if ( size() < ans_size ) {
    set(mVal0, mVal1, size(), ans_size, ans_sized, ans_signed, ans_base);
  }
  if ( src.size() < ans_size ) {
    return merge(BitVector(src, ans_size));
  }

  set_type(ans_sized, ans_signed, ans_base);
  
  ymuint32 n = block(size());
  for (ymuint32 i = 0; i < n; ++ i) {
    ymuint32 val1_0 = mVal0[i];
    ymuint32 val1_1 = mVal1[i];
    ymuint32 val2_0 = src.mVal0[i];
    ymuint32 val2_1 = src.mVal1[i];
    // z を x に変換する
    ymuint32 zpat1 = ~val1_0 & ~val1_1;
    val1_0 |= zpat1;
    val1_1 |= zpat1;
    ymuint32 zpat2 = ~val2_0 & ~val2_1;
    val2_0 |= zpat2;
    val2_1 |= zpat2;
    mVal0[i] = val1_0 | val2_0;
    mVal1[i] = val1_1 | val2_1;
  }

  return *this;
}

// pos ビット目の値を得る．
// pos が範囲を越えていたら kX を返す．
tVpiScalarVal
BitVector::value(int pos) const
{
  if ( pos < 0 || pos >= static_cast<int>(size()) ) {
    return kVpiScalarX;
  }
  
  ymuint32 blk = pos / kBlockSize;
  ymuint32 sft = pos - blk * kBlockSize;
  ymuint32 msk = 1 << sft;
  if ( mVal1[blk] & msk ) {
    if ( mVal0[blk] & msk ) {
      return kVpiScalarX;
    }
    else {
      return kVpiScalar1;
    }
  }
  else {
    if ( mVal0[blk] & msk ) {
      return kVpiScalar0;
    }
    else {
      return kVpiScalarZ;
    }
  }
}

// pos ビット目の値をセットする．
// pos が範囲を越えていたら何もしない
void
BitVector::set_value(int pos,
		     tVpiScalarVal val)
{
  if ( pos < 0 || pos >= static_cast<int>(size()) ) {
    return;
  }
  
  ymuint32 blk = pos / kBlockSize;
  ymuint32 sft = pos - blk * kBlockSize;
  ymuint32 msk = 1 << sft;
  switch ( val ) {
  case kVpiScalar0:
    mVal0[blk] |= msk;
    mVal1[blk] &= ~msk;
    break;
  case kVpiScalar1:
    mVal0[blk] &= ~msk;
    mVal1[blk] |= msk;
    break;
  case kVpiScalarX:
    mVal0[blk] |= msk;
    mVal1[blk] |= msk;
    break;
  case kVpiScalarZ:
    mVal0[blk] &= ~msk;
    mVal1[blk] &= ~msk;
    break;
  }
}

// x 値を含んでいたら true を返す
bool
BitVector::has_x() const
{
  ymuint32 n = block(size());
  for (ymuint32 i = 0; i < n - 1; ++ i) {
    ymuint32 tmp = mVal0[i] & mVal1[i];
    if ( tmp ) {
      return true;
    }
  }

  ymuint32 m = mask(size());
  if ( mVal0[n - 1] & mVal1[n - 1] & m ) {
    return true;
  }
  return false;
}

// z 値を含んでいたら true を返す
bool
BitVector::has_z() const
{
  ymuint32 n = block(size());
  for (ymuint32 i = 0; i < n - 1; ++ i) {
    if ( (mVal0[i] | mVal1[i]) != kAll1 ) {
      return true;
    }
  }

  ymuint32 m = mask(size());
  if ( (mVal0[n - 1] | mVal1[n - 1] | ~m) != kAll1 ) {
    return true;
  }
  return false;
}

// x か z を含んでいたら true を返す
bool
BitVector::has_xz() const
{
  ymuint32 n = block(size());
  for (ymuint32 i = 0; i < n - 1; ++ i) {
    if ( (mVal0[i] ^ mVal1[i]) != kAll1 ) {
      return true;
    }
  }

  ymuint32 m = mask(size());
  if ( (mVal0[n - 1] ^ mVal1[n - 1] | ~m) != kAll1 ) {
    return true;
  }
  return false;
}

// z を x に変える．
// ほとんどの演算において入力の z は x と区別されない
void
BitVector::z_to_x()
{
  ymuint32 n = block(size());
  for (ymuint32 i = 0; i < n - 1; ++ i) {
    ymuint32 zpat = ~mVal0[i] & ~mVal1[i];
    mVal0[i] |= zpat;
    mVal1[i] |= zpat;
  }
  ymuint32 m = mask(size());
  ymuint32 zpat = ~mVal0[n - 1] & ~mVal1[n - 1] & m;
  mVal0[n - 1] |= zpat;
  mVal1[n - 1] |= zpat;
}

// x/z を 0 に変える．
// Verilog-HDL で4値を2値に強引に変換するときのやり方
void
BitVector::xz_to_0()
{
  ymuint32 n = block(size());
  for (ymuint32 i = 0; i < n - 1; ++ i) {
    ymuint32 pat = ~mVal0[i] & mVal1[i];
    mVal0[i] = ~pat;
    mVal1[i] = pat;
  }
  ymuint32 m = mask(size());
  ymuint32 pat = ~mVal0[n - 1] & mVal1[n - 1];
  ymuint32 andpat = ~pat | ~m;
  ymuint32 orpat = pat & m;
  mVal0[n - 1] = mVal0[n - 1] | orpat & andpat;
  mVal1[n - 1] = mVal1[n - 1] | orpat & andpat;
}

// 値を近い double 型に変換する．
// X/Z は0と見なす．
double
BitVector::to_real() const
{
  BitVector tmp = *this;
  tmp.xz_to_0();
  ymuint32 n = block(size());
  double ans = 0.0;
  for (ymuint32 i = 0; i < n; ++ i) {
    const double mag = static_cast<double>(1 << (i * kBlockSize));
    ans += static_cast<double>(mVal1[i]) * mag;
  }
  return ans;
}

// @brief 論理値として評価する．
// @retval kVpiScalar0 0 の時
// @retval kVpiScalar1 0 以外の確定値の時
// @retval kVpiScalarX 不定値を1ビットでも含む場合
tVpiScalarVal
BitVector::to_logic() const
{
  ymuint32 n = block(size());
  for (ymuint32 i = 0; i < n - 1; ++ i) {
    ymuint32 pat0 = mVal0[i];
    ymuint32 pat_xor = pat0 ^ mVal1[i];
    if ( pat_xor != kAll1 ) {
      // X/Z のパタンがあった
      return kVpiScalarX;
    }
    if ( pat0 != kAll0 ) {
      return kVpiScalar1;
    }
  }
  ymuint32 m = mask(size());
  ymuint32 pat0 = mVal0[n - 1];
  ymuint32 pat_xor = (pat0 ^ mVal1[n - 1]) | ~m;
  if ( pat_xor != kAll1 ) {
    // X/Z のパタンがあった
    return kVpiScalarX;
  }
  if ( pat0 != kAll0 ) {
    return kVpiScalar1;
  }
  return kVpiScalar0;
}

// 値をバイトベクターと見なして文字列に変換する．
// 長さが8の倍数でない場合にはパディングする．
string
BitVector::to_string() const
{
  ymuint32 n = size();
  ymuint32 nc = (n + 7) / 8;
  string ans;
  for (ymuint32 i = nc; i -- > 0; ) {
    char c = 0;
    ymuint32 base = i << 3;
    for (ymuint32 j = 0; j < 8; ++ j) {
      if ( value(base + j) == kVpiScalar1 ) {
	c |= (1 << j);
      }
    }
    ans += c;
  }

  return ans;
}

// Verilog-HDL の形式で出力する．
// 内容を Verilog-HDL (IEEE1364-2001) の形式の文字列に変換する．
string
BitVector::verilog_string(ymuint32 opt_base) const
{
  // まず最初に符号付きでマイナスの場合には補数を取る．
  if ( is_negative() ) {
    BitVector tmp = -(*this);
    return "-" + tmp.verilog_string(opt_base);
  }

  string ans;

  if ( opt_base != 2 &&
       opt_base != 8 &&
       opt_base != 10 &&
       opt_base != 16 ) {
    // 安全策
    opt_base = base();
  }

  // サイズなしの場合には上位の0は無視する．
  bool skip_zeros = !is_sized();
  
  ymuint32 l = size();
  if ( l == 1 ) {
    switch ( value(0) ) {
    case kVpiScalar0: ans += '0'; break;
    case kVpiScalar1: ans += '1'; break;
    case kVpiScalarX: ans += 'X'; break;
    case kVpiScalarZ: ans += 'Z'; break;
    }
  }
  else {
    if ( is_sized() ) {
      ans += size_to_dec_str(l);
    }
    switch ( opt_base ) {
    case 2:
      if ( is_signed() ) {
	ans += "\'sb";
      }
      else {
	ans += "\'b";
      }
      ans += bin_str(skip_zeros);
      break;

    case 8:
      if ( is_signed() ) {
	ans += "\'so";
      }
      else {
	ans += "\'o";
      }
      ans += oct_str(skip_zeros);
      break;

    case 10:
      if ( has_xz() ) {
	if ( is_signed() ) {
	  ans += "\'sb";
	}
	else {
	  ans += "\'b";
	}
	ans += bin_str(skip_zeros);
      }
      else {
	if ( is_signed() ) {
	  if ( is_sized() ) {
	    ans += "\'sd";
	  }
	}
	else {
	  ans += "\'d";
	}
	ans += dec_str();
      }
      break;

    case 16:
      if ( is_signed() ) {
	ans += "\'sh";
      }
      else {
	ans += "\'h";
      }
      ans += hex_str(skip_zeros);
      break;
    }
  }
  return ans;
}

// 内容を10進数で表した文字列を返す．
string
BitVector::dec_str() const
{
  return dec_str_sub(mVal1, block(size()));
}

// 内容を2進数で表した文字列を返す．
string
BitVector::bin_str(bool skip_zeros) const
{
  ymuint32 l = size();
  bool first = true;
  string ans;
  for (ymuint32 i = l; i -- > 0; ) {
    if ( skip_zeros && value(i) == kVpiScalar0 ) {
      continue;
    }
    skip_zeros = false;
    if ( i % 4 == 3 && !first ) {
      ans += '_';
    }
    switch ( value(i) ) {
    case kVpiScalar0: ans += '0'; break;
    case kVpiScalar1: ans += '1'; break;
    case kVpiScalarX: ans += 'X'; break;
    case kVpiScalarZ: ans += 'Z'; break;
    }
    first = false;
  }
  if ( skip_zeros ) {
    // 全部0だった
    ans += '0';
  }
  return ans;
}

// 内容を8進数で表した文字列を返す．
string
BitVector::oct_str(bool skip_zeros) const
{
  ymuint32 l = size();
  ymuint32 blk = block(l);
  ymuint32 pos = shift(l);
  ymuint32 tmp0 = 7;
  ymuint32 tmp1 = 0;
  string ans;
  while ( blk -- > 0 ) {
    for (ymuint32 i = pos; i -- > 0; ) {
      ymuint32 bit0 = (mVal0[blk] >> i) & 1;
      ymuint32 bit1 = (mVal1[blk] >> i) & 1;
      tmp0 = ((tmp0 << 1) + bit0) & 7;
      tmp1 = ((tmp1 << 1) + bit1) & 7;
      l --;
      if ( l % 3 == 0 ) {
	if ( tmp0 + tmp1 == 7 ) {
	  if ( tmp1 ) {
	    // 0 ではない．
	    skip_zeros = false;
	  }
	  if ( !skip_zeros ) {
	    ans += hexchar(tmp1);
	  }
	}
	else if ( tmp0 == 0 && tmp1 == 0 ) {
	  skip_zeros = false;
	  ans += 'z';
	}
	else if ( tmp0 == 7 && tmp1 == 7 ) {
	  skip_zeros = false;
	  ans += 'x';
	}
	else {
	  skip_zeros = false;
	  // あってはならない．
	  // というかこの基底では表現できない
	  ans += '#';
	}
	tmp0 = 0;
	tmp1 = 0;
      }
    }
    pos = kBlockSize;
  }
  if ( skip_zeros ) {
    // 全部0だった
    ans += '0';
  }
  return ans;
}

// 内容を16進で表した文字列をかえす．
string
BitVector::hex_str(bool skip_zeros) const
{
  ymuint32 l = size();
  ymuint32 blk = block(l);
  ymuint32 pos = shift(l);
  ymuint32 tmp0 = 15;
  ymuint32 tmp1 = 0;
  string ans;
  while ( blk -- > 0 ) {
    for (ymuint32 i = pos; i -- > 0; ) {
      ymuint32 bit0 = (mVal0[blk] >> i) & 1;
      ymuint32 bit1 = (mVal1[blk] >> i) & 1;
      tmp0 = ((tmp0 << 1) + bit0) & 15;
      tmp1 = ((tmp1 << 1) + bit1) & 15;
      l --;
      if ( l % 4 == 0 ) {
	if ( tmp0 + tmp1 == 15 ) {
	  if ( tmp1 ) {
	    // 0 ではない．
	    skip_zeros = false;
	  }
	  if ( !skip_zeros ) {
	    ans += hexchar(tmp1);
	  }
	}
	else if ( tmp0 == 0 && tmp1 == 0 ) {
	  skip_zeros = false;
	  ans += 'z';
	}
	else if ( tmp0 == 15 && tmp1 == 15 ) {
	  skip_zeros = false;
	  ans += 'x';
	}
	else {
	  skip_zeros = false;
	  // あってはならない．
	  // というかこの基底では表現できない
	  ans += '#';
	}
	tmp0 = 15;
	tmp1 = 0;
      }
    }
    pos = kBlockSize;
  }
  if ( skip_zeros ) {
    // 全部0だった
    ans += '0';
  }
  return ans;
}

// 値をセットする関数
// これは1語に収まる時に用いる．
void
BitVector::set(ymuint32 val0,
	       ymuint32 val1,
	       ymuint32 size,
	       bool has_size,
	       bool has_sign,
	       int base)
{
  assert_cond(size <= kBlockSize, __FILE__, __LINE__);
  
  resize(size);
  set_type(has_size, has_sign, base);
  ymuint32 m = mask(size);
  mVal0[0] = val0 | ~m;
  mVal1[0] = val1 & m;
}

// 値をセットする関数
void
BitVector::set(const ymuint32* val0,
	       const ymuint32* val1,
	       ymuint32 src_size,
	       ymuint32 size,
	       bool has_size,
	       bool has_sign,
	       int base)
{
  resize(size);
  set_type(has_size, has_sign, base);

  ymuint32 n = block(size);
  ymuint32 src_n = block(src_size);

  // val0, val1 の最後のビットの値
  // デフォルトでは 0 を表すビットパタンだが x や z の場合もある．
  // 最上位ビットが1で符号付きの場合には1を詰める．
  ymuint32 last_val0 = kAll1;
  ymuint32 last_val1 = kAll0;

  ymuint32 src_s = shift(src_size);
  ymuint32 last_bit0 = (val0[src_n - 1] >> (src_s - 1)) & 1;
  ymuint32 last_bit1 = (val1[src_n - 1] >> (src_s - 1)) & 1;
  if ( last_bit0 && last_bit1 ) { // x
    last_val1 = kAll1;
  }
  else if ( !last_bit0 && !last_bit1 ) { // z
    last_val0 = kAll0;
  }
  else if ( !last_bit0 && last_bit1 && is_signed() ) { // 1
    last_val0 = kAll0;
    last_val1 = kAll1;
  }

  // コピーする
  for (ymuint32 i = 0; i < n; ++ i) {
    if ( i < src_n - 1 ) {
      mVal0[i] = val0[i];
      mVal1[i] = val1[i];
    }
    else if ( i == src_n - 1 ) {
      ymuint32 sm = mask(src_size);
      mVal0[i] = last_val0 & ~sm | val0[i] & sm;
      mVal1[i] = last_val1 & ~sm | val1[i] & sm;
    }
    else {
      mVal0[i] = last_val0;
      mVal1[i] = last_val1;
    }
  }

  // 上位ビットをトリミングしておく
  ymuint32 m = mask(size);
  mVal0[n - 1] |= ~m;
  mVal1[n - 1] &= m;
}

// 値をセットする関数
void
BitVector::set(const vector<ymuint32>& val0,
	       const vector<ymuint32>& val1,
	       ymuint32 src_size,
	       ymuint32 size,
	       bool has_size,
	       bool has_sign,
	       int base)
{
  resize(size);
  set_type(has_size, has_sign, base);

  ymuint32 n = block(size);
  ymuint32 src_n = block(src_size);

  // val0, val1 の最後のビットの値
  // デフォルトでは 0 を表すビットパタンだが x や z の場合もある．
  // 最上位ビットが1で符号付きの場合には1を詰める．
  ymuint32 last_val0 = kAll1;
  ymuint32 last_val1 = kAll0;

  ymuint32 src_s = shift(src_size);
  ymuint32 last_bit0 = (val0[src_n - 1] >> (src_s - 1)) & 1;
  ymuint32 last_bit1 = (val1[src_n - 1] >> (src_s - 1)) & 1;
  if ( last_bit0 && last_bit1 ) { // x
    last_val1 = kAll1;
  }
  else if ( !last_bit0 && !last_bit1 ) { // z
    last_val0 = kAll0;
  }
  else if ( !last_bit0 && last_bit1 && is_signed() ) { // 1
    last_val0 = kAll0;
    last_val1 = kAll1;
  }

  // コピーする
  for (ymuint32 i = 0; i < n; ++ i) {
    if ( i < src_n - 1 ) {
      mVal0[i] = val0[i];
      mVal1[i] = val1[i];
    }
    else if ( i == src_n - 1 ) {
      ymuint32 sm = mask(src_size);
      mVal0[i] = last_val0 & ~sm | val0[i] & sm;
      mVal1[i] = last_val1 & ~sm | val1[i] & sm;
    }
    else {
      mVal0[i] = last_val0;
      mVal1[i] = last_val1;
    }
  }

  // 上位ビットをトリミングしておく
  ymuint32 m = mask(size);
  mVal0[n - 1] |= ~m;
  mVal1[n - 1] &= m;
}

// mVal0, mVal1 のリサイズをする．
void
BitVector::resize(ymuint32 size)
{
  ymuint32 old_bsize = block(mSize);
  mSize = size;
  ymuint32 new_bsize = block(mSize);
  if ( new_bsize > old_bsize ) {
    delete [] mVal0;
    delete [] mVal1;
    mVal0 = new ymuint32[new_bsize];
    mVal1 = new ymuint32[new_bsize];
  }
}

// 属性(サイズの有無, 符号の有無,基数)をセットする．
void
BitVector::set_type(bool has_size,
		    bool has_sign,
		    ymuint32 base)
{
  mFlags = 0;
  if ( has_size ) {
    mFlags |= 1;
  }
  if ( has_sign ) {
    mFlags |= 2;
  }
  mFlags |= (base << 2);
}

// Verilog-HDL (IEEE1364-2001) の形式で出力する．
ostream&
operator<<(ostream& s,
	   const BitVector& bitval)
{
  return s << bitval.verilog_string();
}

END_NAMESPACE_YM_VERILOG
