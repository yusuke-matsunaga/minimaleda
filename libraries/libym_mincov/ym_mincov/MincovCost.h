#ifndef YM_MINCOV_MINCOVCOST_H
#define YM_MINCOV_MINCOVOCST_H

/// @file ym_mincov/MincovCost.h
/// @brief MincovCost のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
/// 
/// $Id: MincovCost.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.

#include "ym_mincov/mincov_nsdef.h"


//#define USE_MINCOVCOST1 1
#define USE_MINCOVCOST2 1

#define USE_MINCOVCOST 1

#ifdef USE_MINCOVCOST1
#include <ym_mincov/MincovCost1.h>
#undef USE_MINCOVCOST2
#undef USE_MINCOVCOST
#endif
#ifdef USE_MINCOVCOST2
#include <ym_mincov/MincovCost2.h>
#undef USE_MINCOVCOST
#endif

#ifdef USE_MINCOVCOST


BEGIN_NAMESPACE_YM_MINCOV

//////////////////////////////////////////////////////////////////////
/// @class MincovCost MincovCost.h <ym_mincov/MincovCost.h>
/// @brief Mincov 用のコストを表すクラス
//////////////////////////////////////////////////////////////////////
class MincovCost
{
public:

  /// @brief コンストラクタ
  /// @param[in] size 次元
  MincovCost(ymuint size);
  
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
  zero(ymuint size);
  
  /// @brief デストラクタ
  ~MincovCost();
  
  
public:
  
  /// @brief 次元を返す．
  ymuint
  size() const;

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
  
  // コストの次元
  ymuint32 mSize;
  
  // コスト本体のベクタ
  double* mValArray;
  
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
  
// @brief 次元を返す．
inline
ymuint
MincovCost::size() const
{
  return mSize;
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

#endif // USE_MINCOVCOST

#endif // YM_MINCOV_MINCOVCOST_H
