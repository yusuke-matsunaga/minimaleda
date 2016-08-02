#ifndef LIBYM_AIG_FRAIGSIG_H
#define LIBYM_AIG_FRAIGSIG_H

/// @file libym_aig/FraigSig.h
/// @brief FraigSig のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
/// 
/// $Id: FraigSig.h 2203 2009-04-16 05:04:40Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_aig/aig_nsdef.h"
#include "ym_utils/RandGen.h"


BEGIN_NAMESPACE_YM_AIG

//////////////////////////////////////////////////////////////////////
/// @class FraigSig FraigSig.h "FraigSig.h"
/// @brief Fraig のノードを区別するためのシグネチャを表すクラス
/// @note ハッシュ値の計算時には，1ビット目(最下位ビット)が0になるよう
/// に調節している．
//////////////////////////////////////////////////////////////////////
class FraigSig
{
public:
  
  /// @brief コンストラクタ
  /// @param[in] size サイズ
  explicit
  FraigSig(ymuint size);
  
  /// @brief コピーコンストラクタ
  FraigSig(const FraigSig& src);
  
  /// @brief デストラクタ
  ~FraigSig();
  

public:

  /// @brief サイズを得る．
  ymuint
  size() const;

  /// @brief ハッシュ値の計算時に反転していたら true を返す．
  bool
  inv() const;
  
  /// @brief 1 の数を返す．
  ymuint
  one_count() const;

  /// @brief 0 の数を返す．
  ymuint
  zero_count() const;
  
  /// @brief 内容を得る．
  /// @param[in] pos 位置番号 ( 0 <= pos < size() )
  ymuint32
  pat(ymuint pos) const;
  
  /// @brief 内容を設定する．
  /// @note サイズがあっていることを前提としている．
  void
  set(const vector<ymuint32>& src);

  /// @brief 内容をランダムにセットする．
  void
  set_random(RandGen& randgen);

  /// @brief ファンインのシグネチャから計算する．
  void
  calc(const FraigSig& right,
       bool right_inv,
       const FraigSig& left,
       bool left_inv);
  
  /// @brief 等価比較
  bool
  operator==(const FraigSig& right) const;
  
  /// @brief 否定したものが等価かどうか調べる．
  bool
  ncomp(const FraigSig& right) const;
  
  /// @brief 大小比較 (lt)
  bool
  operator<(const FraigSig& right) const;

  /// @brief ハッシュ値を返す．
  ymuint
  hash() const;
  
  /// @brief 内容を出力する．
  void
  dump(ostream& s) const;
  
  
private:

  /// @brief ハッシュ値を計算する．
  void
  calc_hash();
  
  /// @brief pat 中の 1 の数を数える．
  ymuint
  count_ones(ymuint32 pat);

  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////
  
  // サイズ
  ymuint32 mSize;

  // 本体
  ymuint32* mBody;
  
  // ハッシュ値
  ymuint32 mHash;
  
  // 1 の数
  ymuint32 mOne;
  
  // ハッシュ用の素数配列
  static
  ymuint32 mPrimes[128];
  
};

/// @relates FraigSig
/// @brief 非等価比較
bool
operator!=(const FraigSig& left,
	   const FraigSig& right);

/// @relates FraigSig
/// @brief 大小比較 (gt)
bool
operator>(const FraigSig& left,
	  const FraigSig& right);

/// @relates FraigSig
/// @brief 大小比較 (le)
bool
operator<=(const FraigSig& left,
	   const FraigSig& right);

/// @relates FraigSig
/// @brief 大小比較 (ge)
bool
operator>=(const FraigSig& left,
	   const FraigSig& right);


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] size サイズ
inline
FraigSig::FraigSig(ymuint size) :
  mSize(size << 1),
  mBody(new ymuint32[size]),
  mHash(0)
{
}
  
// @brief コピーコンストラクタ
inline
FraigSig::FraigSig(const FraigSig& src) :
  mSize(src.mSize),
  mBody(new ymuint32[mSize]),
  mHash(src.mHash)
{
  for (ymuint i = 0; i < mSize; ++ i) {
    mBody[i] = src.mBody[i];
  }
}
  
// @brief デストラクタ
inline
FraigSig::~FraigSig()
{
  delete [] mBody;
}

// @brief サイズを得る．
inline
ymuint
FraigSig::size() const
{
  return mSize >> 1;
}

// @brief ハッシュ値の計算時に反転していたら true を返す．
inline
bool
FraigSig::inv() const
{
  return static_cast<bool>(mSize & 1U);
}
  
// @brief 1 の数を返す．
inline
ymuint
FraigSig::one_count() const
{
  return mOne;
}

// @brief 0 の数を返す．
inline
ymuint
FraigSig::zero_count() const
{
  return (size() * 32) - mOne;
}

// @brief 内容を得る．
// @param[in] pos 位置番号 ( 0 <= pos < size() )
inline
ymuint32
FraigSig::pat(ymuint pos) const
{
  return mBody[pos];
}
  
// @brief 内容を設定する．
// @note サイズがあっていることを前提としている．
inline
void
FraigSig::set(const vector<ymuint32>& src)
{
  for (ymuint i = 0; i < size(); ++ i) {
    mBody[i] = src[i];
  }
  calc_hash();
}

// @brief 内容をランダムにセットする．
inline
void
FraigSig::set_random(RandGen& randgen)
{
  for (ymuint i = 0; i < size(); ++ i) {
    mBody[i] = randgen.int32();
  }
  mBody[0] &= ~1U;
  calc_hash();
}

// @brief ファンインのシグネチャから計算する．
inline
void
FraigSig::calc(const FraigSig& right,
	       bool right_inv,
	       const FraigSig& left,
	       bool left_inv)
{
  if ( right_inv ) {
    if ( left_inv ) {
      for (ymuint i = 0; i < size(); ++ i) {
	mBody[i] = ~(right.mBody[i] | left.mBody[i]);
      }
    }
    else {
      for (ymuint i = 0; i < size(); ++ i) {
	mBody[i] = ~right.mBody[i] & left.mBody[i];
      }
    }
  }
  else {
    if ( left_inv ) {
      for (ymuint i = 0; i < size(); ++ i) {
	mBody[i] = right.mBody[i] & ~left.mBody[i];
      }
    }
    else {
      for (ymuint i = 0; i < size(); ++ i) {
	mBody[i] = right.mBody[i] & left.mBody[i];
      }
    }
  }
  calc_hash();
}

// @brief 等価比較
inline
bool
FraigSig::operator==(const FraigSig& right) const
{
  for (ymuint i = 0; i < size(); ++ i) {
    if ( mBody[i] != right.mBody[i] ) {
      return false;
    }
  }
  return true;
}
  
// @brief 否定したものが等価かどうか調べる．
inline
bool
FraigSig::ncomp(const FraigSig& right) const
{
  for (ymuint i = 0; i < size(); ++ i) {
    if ( mBody[i] != ~right.mBody[i] ) {
      return false;
    }
  }
  return true;
}

// @relates FraigSig
// @brief 非等価比較
inline
bool
operator!=(const FraigSig& left,
	   const FraigSig& right)
{
  return !left.operator==(right);
}

// @brief 大小比較
inline
bool
FraigSig::operator<(const FraigSig& right) const
{
  for (ymuint i = 0; i < size(); ++ i) {
    if ( mBody[i] < right.mBody[i] ) {
      return true;
    }
  }
  return false;
}

// @relates FraigSig
// @brief 大小比較 (gt)
inline
bool
operator>(const FraigSig& left,
	  const FraigSig& right)
{
  return right.operator<(left);
} 

// @relates FraigSig
// @brief 大小比較 (le)
inline
bool
operator<=(const FraigSig& left,
	   const FraigSig& right)
{
  return !right.operator<(left);
}

// @relates FraigSig
// @brief 大小比較 (ge)
inline
bool
operator>=(const FraigSig& left,
	   const FraigSig& right)
{
  return !left.operator<(right);
}

// @brief ハッシュ値を返す．
inline
ymuint
FraigSig::hash() const
{
  return mHash;
}

// @brief ハッシュ値を計算する．
inline
void
FraigSig::calc_hash()
{
  mHash = 0;
  mOne = 0;
  if ( mBody[0] & 1U ) {
    // 逆相
    for (ymuint i = 0; i < size(); ++ i) {
      mHash ^= (~mBody[i]) * mPrimes[i];
      mOne += count_ones(mBody[i]);
    }
    mSize |= 1U;
  }
  else {
    for (ymuint i = 0; i < size(); ++ i) {
      mHash ^= mBody[i] * mPrimes[i];
      mOne += count_ones(mBody[i]);
    }
  }
}
  
// @brief pat 中の 1 の数を数える．
inline
ymuint
FraigSig::count_ones(ymuint32 pat)
{
  const ymuint32 mask1  = 0x55555555;
  const ymuint32 mask2  = 0x33333333;
  const ymuint32 mask4  = 0x0f0f0f0f;
  const ymuint32 mask8  = 0x00ff00ff;
  const ymuint32 mask16 = 0x0000ffff;

  pat = (pat & mask1)  + ((pat >>  1) & mask1);
  pat = (pat & mask2)  + ((pat >>  2) & mask2);
  pat = (pat & mask4)  + ((pat >>  4) & mask4);
  pat = (pat & mask8)  + ((pat >>  8) & mask8);
  pat = (pat & mask16) + ((pat >> 16) & mask16);
  
  return pat;
}
  
// @brief 内容を出力する．
inline
void
FraigSig::dump(ostream& s) const
{
  for (ymuint i = 0; i < size(); ++ i) {
    s << hex << mBody[i];
  }
  s << dec;
}

END_NAMESPACE_YM_AIG

#endif // LIBYM_AIG_FRAIGSIG_H
