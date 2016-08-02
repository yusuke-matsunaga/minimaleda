#ifndef YM_MINCOV_MINCOVCOST2_H
#define YM_MINCOV_MINCOVOCST2_H

/// @file ym_mincov/MincovCost.h
/// @brief MincovCost のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
/// 
/// $Id: MincovCost2.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_mincov/mincov_nsdef.h"


BEGIN_NAMESPACE_YM_MINCOV

//////////////////////////////////////////////////////////////////////
/// @class MincovCost MincovCost2.h <ym_mincov/MincovCost2.h>
/// @brief Mincov 用のコストを表すクラス(2次元バージョン)
//////////////////////////////////////////////////////////////////////
class MincovCost
{
public:

  /// @brief コンストラクタ
  /// @param[in] value1 1次コスト
  /// @param[in] value2 2次コスト
  MincovCost(double value1 = 1.0,
	     double value2 = 0.0);
  
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

  /// @brief 要素を返す．
  /// @param[in] pos 位置番号
  double
  operator[](ymuint pos) const;

  /// @brief 要素への参照を返す．
  double&
  operator[](ymuint pos);
  

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
  
  // コスト本体のベクタ
  double mValArray[2];
  
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
// @param[in] value2 2次コスト
inline
MincovCost::MincovCost(double value1,
		       double value2)
{
  mValArray[0] = value1;
  mValArray[1] = value2;
}
  
// @brief コピーコンストラクタ
// @param[in] src コピー元のオブジェクト
inline
MincovCost::MincovCost(const MincovCost& src)
{
  mValArray[0] = src.mValArray[0];
  mValArray[1] = src.mValArray[1];
}

// @brief 代入演算子
// @param[in] src コピー元のオブジェクト
inline
const MincovCost&
MincovCost::operator=(const MincovCost& src)
{
  if ( this != &src ) {
    mValArray[0] = src.mValArray[0];
    mValArray[1] = src.mValArray[1];
  }
  return *this;
}

// @brief ゼロを作るクラスメソッド
inline
MincovCost
MincovCost::zero()
{
  return MincovCost(0.0, 0.0);
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
  mValArray[0] += right.mValArray[0];
  mValArray[1] += right.mValArray[1];
  return *this;
}
  
// @brief 減算付き代入演算子
// @param[in] right 右のオペランド
inline
const MincovCost&
MincovCost::operator-=(const MincovCost& right)
{
  mValArray[0] -= right.mValArray[0];
  mValArray[1] -= right.mValArray[1];
  return *this;
}

// @brief 乗算付き代入演算子
// @param[in] k 乗数
inline
const MincovCost&
MincovCost::operator*=(double k)
{
  mValArray[0] *= k;
  mValArray[1] *= k;
  return *this;
}

// @brief 除算付き代入演算子
// @param[in] k 除数
inline
const MincovCost&
MincovCost::operator/=(double k)
{
  mValArray[0] /= k;
  mValArray[1] /= k;
  return *this;
}

// @brief 等価比較演算子
// @param[in] right 右のオペランド
inline
bool
MincovCost::operator==(const MincovCost& right) const
{
  return mValArray[0] == right.mValArray[0] && mValArray[1] == right.mValArray[1];
}

// @brief 大小比較演算子
// @param[in] right 右のオペランド
inline
bool
MincovCost::operator<(const MincovCost& right) const
{
  if ( mValArray[0] < right.mValArray[0] ) {
    return true;
  }
  if ( mValArray[0] == right.mValArray[1] ) {
    return mValArray[1] < right.mValArray[1];
  }
  return false;
}

// @brief 要素を返す．
// @param[in] pos 位置番号
inline
double
MincovCost::operator[](ymuint pos) const
{
  return mValArray[pos];
}

// @brief 要素への参照を返す．
inline
double&
MincovCost::operator[](ymuint pos)
{
  return mValArray[pos];
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

#endif // YM_MINCOV_MINCOVCOST2_H
