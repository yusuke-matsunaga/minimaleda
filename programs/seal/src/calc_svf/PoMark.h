#ifndef CALC_SVF_POMARK_H
#define CALC_SVF_POMARK_H

/// @file calc_svf/PoMark.h
/// @brief PoMark のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
/// 
/// $Id: PoMark.h 1920 2008-12-20 15:52:42Z matsunaga $
///
/// Copyright (C) 2005-2008 Yusuke Matsunaga
/// All rights reserved.

#include "nsdef.h"
#include "seal_utils.h"


BEGIN_NAMESPACE_YM_SEAL_SVF

//////////////////////////////////////////////////////////////////////
/// @class PoMark PoMark.h "PoMark.h"
/// @brief PO の到達可能性を表すビットベクタ
//////////////////////////////////////////////////////////////////////
class PoMark
{
public:

  /// @brief コンストラクタ
  /// @param[in] no 外部出力数
  PoMark(size_t no = 0);
  
  /// @brief デストラクタ
  ~PoMark();


public:

  /// @brief サイズを(再)設定する．
  /// @param[in] no 外部入力数
  void
  init(size_t no);

  /// @brief クリアする．
  void
  clear();
  
  /// @brief 指定した位置のビットを立てる．
  /// @param[in] pos ビットを立てる位置
  void
  set(size_t pos);

  /// @brief right をマージ(bitwise-OR)する．
  /// @param[in] right マージするビットベクタ
  void
  merge(const PoMark& right);

  /// @brief right とのインターセクションを調べる．
  /// @param[in] right 比較対象のビットベクタ
  /// @return インターセクションが1ビットでもあれば true を返す．
  bool
  operator&&(const PoMark& right) const;
  
  /// @brief 要素数を返す．
  size_t
  size() const;
  
  friend
  int
  check(const PoMark& left,
	const PoMark& right);

  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////
  
  // ブロック数
  size_t mBlockNum;
  
  // 要素の集合を表すビットベクタ
  tPackedVal* mBitVector;

  // 非0 要素の最初のブロック
  size_t mStart;

  // 非0 要素の最後のブロックの次
  size_t mEnd;
  
};
  
/// @brief left と right のインターセクションを調べる．
/// @param[in] left, right 比較対象のビットベクタ
/// @retval 0 インターセクションなし
/// @retval 1 right は left に含まれる．
/// @retval 2 left は right に含まれる．
/// @retval 3 インターセクションはあるが包含関係なし．
int
check(const PoMark& left,
      const PoMark& right);


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief right をマージ(bitwise-OR)する．
// @param[in] right マージするビットベクタ
inline
void
PoMark::merge(const PoMark& right)
{
  for (size_t i = right.mStart; i < right.mEnd; ++ i) {
    mBitVector[i] |= right.mBitVector[i];
  }
  if ( mStart > right.mStart ) {
    mStart = right.mStart;
  }
  if ( mEnd < right.mEnd ) {
    mEnd = right.mEnd;
  }
}

// @brief right とのインターセクションを調べる．
// @param[in] right 比較対象のビットベクタ
// @return インターセクションが1ビットでもあれば true を返す．
inline
bool
PoMark::operator&&(const PoMark& right) const
{
  if ( mEnd <= right.mStart || mStart >= right.mEnd ) return false;
  for (size_t i = mStart; i < mEnd; ++ i) {
    if ( mBitVector[i] & right.mBitVector[i] ) return true;
  }
  return false;
}
  
// @brief left と right のインターセクションを調べる．
// @param[in] left, right 比較対象のビットベクタ
// @retval 0 インターセクションなし
// @retval 1 right は left に含まれる．
// @retval 2 left は right に含まれる．
// @retval 3 left と right は等しい．
// @retval 4 インターセクションはあるが包含関係なし．
inline
int
check(const PoMark& left,
      const PoMark& right)
{
  bool lonly = false;
  bool ronly = false;
  size_t s = left.mStart;
  size_t e = left.mEnd;
  if ( s < right.mStart ) {
    lonly = true;
    s = right.mStart;
  }
  else if ( s > right.mStart ) {
    ronly = true;
  }
  if ( e > right.mEnd ) {
    lonly = true;
    e = right.mEnd;
  }
  else if ( e < right.mEnd ) {
    ronly = true;
  }
  if ( s >= e ) {
    return 0;
  }
  bool overlap = false;
  for (size_t i = s; i < e; ++ i) {
    tPackedVal l = left.mBitVector[i];
    tPackedVal r = right.mBitVector[i];
    if ( l & r ) {
      overlap = true;
      if ( l & ~r ) {
	lonly = true;
      }
      if ( r & ~l ) {
	ronly = true;
      }
    }
  }
  if ( overlap ) {
    if ( lonly ) {
      if ( ronly ) {
	return 4;
      }
      else {
	return 1;
      }
    }
    else {
      if ( ronly ) {
	return 2;
      }
      else {
	return 3;
      }
    }
  }
  else {
    return 0;
  }
}

END_NAMESPACE_YM_SEAL_SVF

#endif // CALC_SVF_POMARK_H
