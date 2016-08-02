#ifndef SEAL_TESTVECTOR_H
#define SEAL_TESTVECTOR_H

/// @file include/TestVectort.h
/// @brief TestVector のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
/// 
/// $Id: TestVector.h 1920 2008-12-20 15:52:42Z matsunaga $
///
/// Copyright (C) 2005-2008 Yusuke Matsunaga
/// All rights reserved.

#include "seal_nsdef.h"
#include "seal_utils.h"
#include <ym_utils/RandGen.h>


BEGIN_NAMESPACE_YM_SEAL

class SaFault;

//////////////////////////////////////////////////////////////////////
/// @class TestVector TestVector.h "TestVector.h"
/// @brief テストベクタを表すクラス
//////////////////////////////////////////////////////////////////////
class TestVector
{
public:
  
  /// @brief テストベクタを生成する．
  static
  TestVector*
  new_vector(size_t ni);

  /// @brief テストベクタを削除する．
  static
  void
  delete_vector(TestVector* tv);
  
  
public:

  /// @brief 入力数を得る．
  size_t
  ni() const;

  /// @brief pos 番めの値を得る．
  /// @note 返り値は 0/1 のいずれか
  int
  val(size_t pos) const;
  
  /// @brief 内容を BIN 形式で出力する．
  void
  dump_bin(ostream& s) const;
  
  /// @brief 内容を HEX 形式で出力する．
  void
  dump_hex(ostream& s) const;


public:

  /// @breif pos 番めの値を設定する．
  /// @note val は 0 か非0 かで区別される．
  void
  set_val(size_t pos,
	  int val);

  /// @brief HEX文字列から内容を設定する．
  /// @param[in] hex_string HEX 文字列
  /// @return hex_string に不適切な文字が含まれていたら false を返す．
  /// @note hex_string が短い時には残りは0で初期化される．
  /// @note hex_string が長い時には余りは捨てられる．
  bool
  set_from_hex(const string& hex_string);

  /// @brief 乱数パタンを設定する．
  /// @param[in] randgen 乱数生成器
  void
  set_from_random(RandGen& randgen);


private:

  /// @brief ブロック数を返す．
  static
  size_t
  block_num(size_t ni);

  /// @brief HEX文字列の長さを返す．
  static
  size_t
  hex_length(size_t ni);
  
  // 入力位置からブロック番号を得る．
  static
  size_t
  block_idx(size_t ipos);

  // 入力位置からシフト量を得る．
  static
  size_t
  shift_num(size_t ipos);



private:
  //////////////////////////////////////////////////////////////////////
  // 特殊なアロケーションをしているのでコンストラクタ関係は
  // プライベートにしている．
  //////////////////////////////////////////////////////////////////////

  /// @brief コンストラクタ
  /// @param[in] 入力数を指定する．
  explicit
  TestVector(size_t ni);
  
  /// @brief デストラクタ
  ~TestVector();
  
  /// @brief コピーコンストラクタ
  /// @param[in] src コピー元のソース
  TestVector(const TestVector& src);

  /// @brief 代入演算子
  /// @param[in] src コピー元のソース
  const TestVector&
  operator=(const TestVector& src);
  

private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 入力数
  size_t mNi;
  
  // ベクタ本体(ただしサイズは可変)
  tPackedVal mPat[1];
  

private:

  // 1ワードあたりのHEX文字数
  static
  const size_t HPW = kPvBitLen / 4;
  
};


/// @brief 内容を出力する．
ostream&
operator<<(ostream& s,
	   const TestVector& tv);

/// @brief 内容を出力する．
ostream&
operator<<(ostream& s,
	   const TestVector* tvp);


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 入力数を得る．
inline
size_t
TestVector::ni() const
{
  return mNi;
}

// @brief pos 番めの値を得る．
// @note 返り値は 0/1 のいずれか
inline
int
TestVector::val(size_t pos) const
{
  size_t shift = shift_num(pos);
  size_t block = block_idx(pos);
  return (mPat[block] >> shift) & 1UL;
}

// @breif pos 番めの値を設定する．
// @note val は 0 か非0 かで区別される．
inline
void
TestVector::set_val(size_t pos,
		    int val)
{
  size_t shift = shift_num(pos);
  size_t block = block_idx(pos);
  tPackedVal mask = 1UL << shift;
  if ( val ) {
    mPat[block] |= mask;
  }
  else {
    mPat[block] &= ~mask;
  }
}

// @brief ブロック数を返す．
inline
size_t
TestVector::block_num(size_t ni)
{
  return (ni + kPvBitLen - 1) / kPvBitLen;
}

// @brief HEX文字列の長さを返す．
inline
size_t
TestVector::hex_length(size_t ni)
{
  return (ni + 3) / 4;
}

// 入力位置からブロック番号を得る．
inline
size_t
TestVector::block_idx(size_t ipos)
{
  return ipos / kPvBitLen;
}

// 入力位置からシフト量を得る．
inline
size_t
TestVector::shift_num(size_t ipos)
{
  return (kPvBitLen - 1 - ipos) % kPvBitLen;
}

// @brief 内容を出力する．
inline
ostream&
operator<<(ostream& s,
	   const TestVector& tv)
{
  tv.dump_hex(s);
  return s;
}

// @brief 内容を出力する．
inline
ostream&
operator<<(ostream& s,
	   const TestVector* tvp)
{
  tvp->dump_hex(s);
  return s;
}


END_NAMESPACE_YM_SEAL

#endif // SEAL_TESTVECTOR_H
