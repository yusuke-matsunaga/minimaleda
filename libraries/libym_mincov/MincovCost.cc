
/// @file libym_mincov/MincovCost.cc
/// @brief MincovCost の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
/// 
/// $Id: MincovCost.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_mincov/MincovCost.h"


#ifdef USE_MINCOVCOST

BEGIN_NAMESPACE_YM_MINCOV

//////////////////////////////////////////////////////////////////////
// クラス MincovCost
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] size 次元
MincovCost::MincovCost(ymuint size) :
  mSize(size),
  mValArray(new double[size])
{
  for (ymuint i = 0; i < size; ++ i) {
    mValArray[i] = 0.0;
  }
}
  
// @brief コピーコンストラクタ
// @param[in] src コピー元のオブジェクト
MincovCost::MincovCost(const MincovCost& src) :
  mSize(src.size()),
  mValArray(new double[mSize])
{
  for (ymuint i = 0; i < mSize; ++ i) {
    mValArray[i] = src.mValArray[i];
  }
}

// @brief 代入演算子
// @param[in] src コピー元のオブジェクト
const MincovCost&
MincovCost::operator=(const MincovCost& src)
{
  if ( this != &src ) {
    if ( size() != src.size() ) {
      delete [] mValArray;
      mSize = src.size();
      mValArray = new double[mSize];
    }
    for (ymuint i = 0; i < mSize; ++ i) {
      mValArray[i] = src.mValArray[i];
    }
  }
  return *this;
}

// @brief ゼロを作るクラスメソッド
MincovCost
MincovCost::zero(ymuint size)
{
  return MincovCost(size);
}
  
// @brief デストラクタ
MincovCost::~MincovCost()
{
  delete [] mValArray;
}

// @brief 加算付き代入演算子
// @param[in] right 右のオペランド
const MincovCost&
MincovCost::operator+=(const MincovCost& right)
{
  assert_cond(mSize == right.size(), __FILE__, __LINE__);
  for (ymuint i = 0; i < mSize; ++ i) {
    mValArray[i] += right.mValArray[i];
  }
  return *this;
}
  
// @brief 減算付き代入演算子
// @param[in] right 右のオペランド
const MincovCost&
MincovCost::operator-=(const MincovCost& right)
{
  assert_cond(mSize == right.size(), __FILE__, __LINE__);
  for (ymuint i = 0; i < mSize; ++ i) {
    mValArray[i] -= right.mValArray[i];
  }
  return *this;
}

// @brief 乗算付き代入演算子
// @param[in] k 乗数
const MincovCost&
MincovCost::operator*=(double k)
{
  for (ymuint i = 0; i < mSize; ++ i) {
    mValArray[i] *= k;
  }
  return *this;
}

// @brief 除算付き代入演算子
// @param[in] k 除数
const MincovCost&
MincovCost::operator/=(double k)
{
  for (ymuint i = 0; i < mSize; ++ i) {
    mValArray[i] /= k;
  }
  return *this;
}

// @brief 等価比較演算子
// @param[in] right 右のオペランド
bool
MincovCost::operator==(const MincovCost& right) const
{
  assert_cond(mSize == right.size(), __FILE__, __LINE__);
  for (ymuint i = 0; i < mSize; ++ i) {
    if ( mValArray[i] != right.mValArray[i] ) {
      return false;
    }
  }
  return true;
}

// @brief 大小比較演算子
// @param[in] right 右のオペランド
bool
MincovCost::operator<(const MincovCost& right) const
{
  assert_cond(mSize == right.size(), __FILE__, __LINE__);
  for (ymuint i = 0; i < mSize; ++ i) {
    if ( mValArray[i] < right.mValArray[i] ) {
      return true;
    }
    if ( mValArray[i] > right.mValArray[i] ) {
      return false;
    }
    // ループするのは mValArray[i] == right.mValArray[i] の時のみ
  }
  return true;
}

END_NAMESPACE_YM_MINCOV

#endif // USE_MINCOVCOST
