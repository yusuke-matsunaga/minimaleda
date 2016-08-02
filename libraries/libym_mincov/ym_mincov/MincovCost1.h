#ifndef YM_MINCOV_MINCOVCOST1_H
#define YM_MINCOV_MINCOVOCST1_H

/// @file ym_mincov/MincovCost.h
/// @brief MincovCost のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
/// 
/// $Id: MincovCost1.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_mincov/mincov_nsdef.h"


BEGIN_NAMESPACE_YM_MINCOV

//////////////////////////////////////////////////////////////////////
/// @class MincovCost MincovCost1.h <ym_mincov/MincovCost1.h>
/// @brief Mincov 用のコストを表すクラス(2次元バージョン)
//////////////////////////////////////////////////////////////////////
class MincovCost
{
public:

  /// @brief コンストラクタ
  /// @param[in] value1 1次コスト
  MincovCost(double value1 = 1.0);
  
  /// @brief コピーコンストラクタ
  /// @param[in] src コピー元のオブジェクト
  MincovCost(const MincovCost& src);

  /// @brief 代入演算子
  /// @param[in] src コピー元のオブジェクト
  const MincovCost&
  operator=(const MincovCost& src);
  
  /// @brief ゼロを作るクラスメソッド
  static
  MincovCost
  zero();
  
  /// @brief デストラクタ
  ~MincovCost();

  
public:
  
  /// @brief 値を返す．
  double
  value() const;

  /// @brief 値への参照を返す．
  double&
  value();
  

public:

  /// @brief 加算付き代入演算子
  /// @param[in] right 右のオペランド
  const MincovCost&
  operator+=(const MincovCost& right);
  
  /// @brief 減算付き代入演算子
  /// @param[in] right 右のオペランド
  const MincovCost&
  operator-=(const MincovCost& right);

  /// @brief 乗算付き代入演算子
  /// @param[in] k 乗数
  const MincovCost&
  operator*=(double k);

  /// @brief 除算付き代入演算子
  /// @param[in] k 除数
  const MincovCost&
  operator/=(double k);


public:

  /// @brief 等価比較演算子
  /// @param[in] right 右のオペランド
  bool
  operator==(const MincovCost& right) const;

  /// @brief 大小比較演算子
  /// @param[in] right 右のオペランド
  bool
  operator<(const MincovCost& right) const;
  
  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////
  
  // コスト本体
  double mValue;
  
};


//////////////////////////////////////////////////////////////////////
// MincovCost に関係する関数
//////////////////////////////////////////////////////////////////////

/// @brief 加算
/// @param[in] left 左のオペランド
/// @param[in] right 右のオペランド
MincovCost
operator+(const MincovCost& left,
	  const MincovCost& right);

/// @brief 減算
/// @param[in] left 左のオペランド
/// @param[in] right 右のオペランド
MincovCost
operator-(const MincovCost& left,
	  const MincovCost& right);

/// @brief 乗算
/// @param[in] left 左のオペランド
/// @param[in] k 乗数
MincovCost
operator*(const MincovCost& left,
	  double k);

/// @brief 除算
/// @param[in] left 左のオペランド
/// @param[in] k 除数
MincovCost
operator/(const MincovCost& left,
	  double k);

/// @brief 非等価演算子
/// @param[in] left 左のオペランド
/// @param[in] right 右のオペランド
bool
operator!=(const MincovCost& left,
	   const MincovCost& right);

/// @brief gt 演算子
/// @param[in] left 左のオペランド
/// @param[in] right 右のオペランド
bool
operator>(const MincovCost& left,
	  const MincovCost& right);

/// @brief le 演算子
/// @param[in] left 左のオペランド
/// @param[in] right 右のオペランド
bool
operator<=(const MincovCost& left,
	   const MincovCost& right);

/// @brief ge 演算子
/// @param[in] left 左のオペランド
/// @param[in] right 右のオペランド
bool
operator>=(const MincovCost& left,
	   const MincovCost& right);


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] value1 1次コスト
inline
MincovCost::MincovCost(double value1) :
  mValue(value1)
{
}
  
// @brief コピーコンストラクタ
// @param[in] src コピー元のオブジェクト
inline
MincovCost::MincovCost(const MincovCost& src) :
  mValue(src.mValue)
{
}

// @brief 代入演算子
// @param[in] src コピー元のオブジェクト
inline
const MincovCost&
MincovCost::operator=(const MincovCost& src)
{
  if ( this != &src ) {
    mValue = src.mValue;
  }
  return *this;
}

// @brief ゼロを作るクラスメソッド
inline
MincovCost
MincovCost::zero()
{
  return MincovCost(0.0);
}
  
// @brief デストラクタ
inline
MincovCost::~MincovCost()
{
}

// @brief 加算付き代入演算子
// @param[in] right 右のオペランド
inline
const MincovCost&
MincovCost::operator+=(const MincovCost& right)
{
  mValue += right.mValue;
  return *this;
}
  
// @brief 減算付き代入演算子
// @param[in] right 右のオペランド
inline
const MincovCost&
MincovCost::operator-=(const MincovCost& right)
{
  mValue -= right.mValue;
  return *this;
}

// @brief 乗算付き代入演算子
// @param[in] k 乗数
inline
const MincovCost&
MincovCost::operator*=(double k)
{
  mValue *= k;
  return *this;
}

// @brief 除算付き代入演算子
// @param[in] k 除数
inline
const MincovCost&
MincovCost::operator/=(double k)
{
  mValue /= k;
  return *this;
}

// @brief 等価比較演算子
// @param[in] right 右のオペランド
inline
bool
MincovCost::operator==(const MincovCost& right) const
{
  return mValue == right.mValue;
}

// @brief 大小比較演算子
// @param[in] right 右のオペランド
inline
bool
MincovCost::operator<(const MincovCost& right) const
{
  return mValue < right.mValue;
}

// @brief 値を返す．
inline
double
MincovCost::value() const
{
  return mValue;
}

// @brief 値への参照を返す．
inline
double&
MincovCost::value()
{
  return mValue;
}

// @brief 加算
// @param[in] left 左のオペランド
// @param[in] right 右のオペランド
inline
MincovCost
operator+(const MincovCost& left,
	  const MincovCost& right)
{
  return MincovCost(left).operator+=(right);
}

// @brief 減算
// @param[in] left 左のオペランド
// @param[in] right 右のオペランド
inline
MincovCost
operator-(const MincovCost& left,
	  const MincovCost& right)
{
  return MincovCost(left).operator-=(right);
}

// @brief 乗算
// @param[in] left 左のオペランド
// @param[in] k 乗数
inline
MincovCost
operator*(const MincovCost& left,
	  double k)
{
  return MincovCost(left).operator*=(k);
}

// @brief 除算
// @param[in] left 左のオペランド
// @param[in] k 除数
inline
MincovCost
operator/(const MincovCost& left,
	  double k)
{
  return MincovCost(left).operator/=(k);
}

// @brief 非等価演算子
// @param[in] left 左のオペランド
// @param[in] right 右のオペランド
inline
bool
operator!=(const MincovCost& left,
	   const MincovCost& right)
{
  return !(left == right);
}

// @brief gt 演算子
// @param[in] left 左のオペランド
// @param[in] right 右のオペランド
inline
bool
operator>(const MincovCost& left,
	  const MincovCost& right)
{
  return right < left;
}

// @brief le 演算子
// @param[in] left 左のオペランド
// @param[in] right 右のオペランド
inline
bool
operator<=(const MincovCost& left,
	   const MincovCost& right)
{
  return !(right < left);
}

// @brief ge 演算子
// @param[in] left 左のオペランド
// @param[in] right 右のオペランド
inline
bool
operator>=(const MincovCost& left,
	   const MincovCost& right)
{
  return !(left < right);
}

END_NAMESPACE_YM_MINCOV

#endif // YM_MINCOV_MINCOVCOST1_H
