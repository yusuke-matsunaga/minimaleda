
/// @file seal/src/TestVector.cc
/// @brief TestVector の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
/// 
/// $Id: TestVector.cc 1920 2008-12-20 15:52:42Z matsunaga $
///
/// Copyright (C) 2005-2007 Yusuke Matsunaga
/// All rights reserved.

#if HAVE_CONFIG_H
#include "seal_config.h"
#endif


#include "TestVector.h"


BEGIN_NAMESPACE_YM_SEAL

// @brief テストベクタを生成する．
TestVector*
TestVector::new_vector(size_t ni)
{
  size_t nb = block_num(ni);
  size_t size = sizeof(TestVector) + kPvBitLen * (nb - 1);
  char* p = new char[size];
  TestVector* tv = new (p) TestVector(ni);
  return tv;
}

// @brief テストベクタを削除する．
void
TestVector::delete_vector(TestVector* tv)
{
  delete [] reinterpret_cast<char*>(tv);
}

// @brief コンストラクタ
// @param[in] 入力数を指定する．
TestVector::TestVector(size_t ni) :
  mNi(ni)
{
}

// @brief デストラクタ
TestVector::~TestVector()
{
}

// @brief HEX文字列から内容を設定する．
// @param[in] hex_string HEX 文字列
// @return hex_string に不適切な文字が含まれていたら false を返す．
// @note hex_string が短い時には残りは0で初期化される．
// @note hex_string が長い時には余りは捨てられる．
bool
TestVector::set_from_hex(const string& hex_string)
{
  // よく問題になるが，ここでは最下位ビット側から入力する．
  size_t nl = hex_length(ni());
  size_t sft = 0;
  size_t blk = 0;
  tPackedVal pat = kPvAll0;
  for (size_t i = 0; i < nl; ++ i) {
    char c = (i < hex_string.size()) ? hex_string[i] : '0';
    tPackedVal pat1 = kPvAll0;
    if ( '0' <= c && c <= '9' ) {
      pat1 = static_cast<tPackedVal>(c - '0');
    }
    else if ( 'a' <= c && c <= 'f' ) {
      pat1 = static_cast<tPackedVal>(c - 'a' + 10);
    }
    else if ( 'A' <= c && c <= 'F' ) {
      pat1 = static_cast<tPackedVal>(c - 'A' + 10);
    }
    else {
      return false;
    }
    pat |= (pat1 << sft);
    sft += 4;
    if ( sft == kPvBitLen ) {
      mPat[blk] = pat;
      sft = 0;
      blk ++;
      pat = kPvAll0;
    }
  }
  if ( sft != 0 ) {
    mPat[blk] = pat;
  }
  
  return true;
}

// @brief 乱数パタンを設定する．
// @param[in] randgen 乱数生成器
void
TestVector::set_from_random(RandGen& randgen)
{
  size_t nb = block_num(ni());
  for (size_t i = 0; i < nb; ++ i) {
    mPat[i] = randgen.ulong();
  }
}

// @brief 内容を BIN 形式で出力する．
void
TestVector::dump_bin(ostream& s) const
{
  // よく問題になるが，ここでは最下位ビット側から出力する．
  for (size_t i = 0; i < ni(); ++ i) {
    if ( val(i) ) {
      s << '1';
    }
    else {
      s << '0';
    }
  }
}

// @brief 内容を HEX 形式で出力する．
void
TestVector::dump_hex(ostream& s) const
{
  // よく問題になるが，ここでは最下位ビット側から出力する．
  size_t nl = hex_length(ni());
  size_t prev_blk = block_num(ni());
  tPackedVal prev_pat = kPvAll0;
  for (size_t i = 0; i < nl; ++ i) {
    size_t blk = i / HPW;
    if ( blk != prev_blk ) {
      prev_pat = mPat[blk];
      prev_blk = blk;
    }
    size_t sft = (i % HPW) * 4;
    tPackedVal pat1 = (prev_pat >> sft) & 0xF;
    if ( pat1 <= 9 ) {
      s << static_cast<char>('0' + pat1);
    }
    else {
      s << static_cast<char>('A' + pat1 - 10);
    }
  }
}

END_NAMESPACE_YM_SEAL
