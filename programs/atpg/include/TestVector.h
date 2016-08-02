#ifndef BASE_TESTVECTOR_H
#define BASE_TESTVECTOR_H

/// @file src/base/TestVectort.h
/// @brief TestVector のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
/// 
/// $Id: TestVector.h 2203 2009-04-16 05:04:40Z matsunaga $
///
/// Copyright (C) 2005-2009 Yusuke Matsunaga
/// All rights reserved.

#include "atpg_nsdef.h"
#include "Val3.h"
#include "PackedVal.h"
#include <ym_utils/RandGen.h>


BEGIN_NAMESPACE_YM_ATPG

class TvMgr;
class SaFault;

//////////////////////////////////////////////////////////////////////
/// @class TestVector TestVector.h "TestVector.h"
/// @brief テストベクタを表すクラス
//////////////////////////////////////////////////////////////////////
class TestVector
{
  friend class TvMgr;
  
public:

  /// @brief 入力数を得る．
  ymuint
  ni() const;

  /// @brief pos 番めの値を得る．
  Val3
  val3(ymuint pos) const;

  /// @brief このパタンで検出した故障のリストを返す．
  const list<SaFault*>&
  det_faults() const;
  
  /// @brief 内容を BIN 形式で出力する．
  void
  dump_bin(ostream& s) const;
  
  /// @brief 内容を HEX 形式で出力する．
  /// @note X を含む場合の出力は不定
  void
  dump_hex(ostream& s) const;
  
  // 暫定的に用意する関数
  void
  dump(FILE* fp) const;

  
public:
  
  /// @brief すべて未定(X) で初期化する．
  void
  init();
  
  /// @breif pos 番めの値を設定する．
  void
  set_val(ymuint pos,
	  Val3 val);

  /// @brief HEX文字列から内容を設定する．
  /// @param[in] hex_string HEX 文字列
  /// @return hex_string に不適切な文字が含まれていたら false を返す．
  /// @note hex_string が短い時には残りは0で初期化される．
  /// @note hex_string が長い時には余りは捨てられる．
  bool
  set_from_hex(const string& hex_string);

  /// @brief 乱数パタンを設定する．
  /// @param[in] randgen 乱数生成器
  /// @note 結果はかならず 0 か 1 になる．(Xは含まれない)
  void
  set_from_random(RandGen& randgen);
  
  /// @brief テストベクタをコピーする．
  /// @param[in] src コピー元のテストベクタ
  /// @note X の部分はコピーしない．
  void
  copy(const TestVector& src);
  
  /// @brief このパタンで検出した故障を追加する．
  void
  add_fault(SaFault* fault);


private:

  /// @brief ブロック数を返す．
  static
  ymuint
  block_num(ymuint ni);

  /// @brief HEX文字列の長さを返す．
  static
  ymuint
  hex_length(ymuint ni);
  
  // 入力位置からブロック番号を得る．
  static
  ymuint
  block_idx(ymuint ipos);

  // 入力位置からシフト量を得る．
  static
  ymuint
  shift_num(ymuint ipos);



private:
  //////////////////////////////////////////////////////////////////////
  // 特殊なアロケーションをしているのでコンストラクタ関係は
  // プライベートにしている．
  //////////////////////////////////////////////////////////////////////

  /// @brief コンストラクタ
  /// @param[in] 入力数を指定する．
  explicit
  TestVector(ymuint ni);
  
  /// @brief デストラクタ
  ~TestVector();
  
  /// @brief コピーコンストラクタ
  /// @param[in] src コピー元のソース
  TestVector(const TestVector& src);

  /// @brief 代入演算子
  /// @param[in] src コピー元のソース
  const TestVector&
  operator=(const TestVector& src);

  
public:
  
  // 暫定的に用意するメンバ
  int det_count;
  

private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 入力数
  ymuint mNi;
  
  // このパタンで検出した故障のリスト
  list<SaFault*> mDetFaults;
  
  // ベクタ本体(ただしサイズは可変)
  PackedVal mPat[1];

  
private:
  //////////////////////////////////////////////////////////////////////
  // このクラスに固有の定数
  //////////////////////////////////////////////////////////////////////

  // 1ワードあたりのHEX文字数
  static
  const ymuint HPW = kPvBitLen / 4;
  
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
ymuint
TestVector::ni() const
{
  return mNi;
}

// @brief pos 番めの値を得る．
inline
Val3
TestVector::val3(ymuint pos) const
{
  ymuint shift = shift_num(pos);
  ymuint block0 = block_idx(pos);
  ymuint block1 = block0 + 1;
  int v0 = (mPat[block0] >> shift) & 1UL;
  int v1 = (mPat[block1] >> shift) & 1UL;
  return static_cast<Val3>(v1 + v1 + v0);
}

// @breif pos 番めの値を設定する．
inline
void
TestVector::set_val(ymuint pos,
		    Val3 val)
{
  ymuint shift = shift_num(pos);
  ymuint block0 = block_idx(pos);
  ymuint block1 = block0 + 1;
  PackedVal mask = 1UL << shift;
  if ( val == kVal0 ) {
    mPat[block0] |= mask;
    mPat[block1] &= ~mask;
  }
  else if ( val == kVal1 ) {
    mPat[block0] &= ~mask;
    mPat[block1] |= mask;
  }
  else { // val == kValX
    mPat[block0] &= ~mask;
    mPat[block1] &= ~mask;
  }
}

// @brief このパタンで検出した故障のリストを返す．
inline
const list<SaFault*>&
TestVector::det_faults() const
{
  return mDetFaults;
}

// @brief このパタンで検出した故障を追加する．
inline
void
TestVector::add_fault(SaFault* fault)
{
  mDetFaults.push_back(fault);
}

// @brief ブロック数を返す．
inline
ymuint
TestVector::block_num(ymuint ni)
{
  return ((ni + kPvBitLen - 1) / kPvBitLen) * 2;
}

// @brief HEX文字列の長さを返す．
inline
ymuint
TestVector::hex_length(ymuint ni)
{
  return (ni + 3) / 4;
}

// 入力位置からブロック番号を得る．
inline
ymuint
TestVector::block_idx(ymuint ipos)
{
  return (ipos / kPvBitLen) * 2;
}

// 入力位置からシフト量を得る．
inline
ymuint
TestVector::shift_num(ymuint ipos)
{
  return (kPvBitLen - 1 - ipos) % kPvBitLen;
}

// @brief 内容を出力する．
inline
ostream&
operator<<(ostream& s,
	   const TestVector& tv)
{
  tv.dump_bin(s);
  return s;
}

// @brief 内容を出力する．
inline
ostream&
operator<<(ostream& s,
	   const TestVector* tvp)
{
  tvp->dump_bin(s);
  return s;
}

END_NAMESPACE_YM_ATPG

#endif // ATPG_TESTVECTOR_H
