#ifndef PACKEDVAL_H
#define PACKEDVAL_H

/// @file PackedVal.h
/// @brief 1ワードにパックしたビットベクタ型の定義ファイル
/// @author Yusuke Matsunaga (松永 裕介)
/// 
/// $Id: PackedVal.h 2043 2009-02-21 07:08:48Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.

#include "atpg_nsdef.h"


BEGIN_NAMESPACE_YM_ATPG

//////////////////////////////////////////////////////////////////////
/// @brief 1ワードのビットベクタを表す型
//////////////////////////////////////////////////////////////////////
typedef ymulong PackedVal;

/// @brief 全てのビットが0の定数
const PackedVal kPvAll0 = 0UL;

/// @brief 全てのビットが1の定数
const PackedVal kPvAll1 = ~0UL;

/// @brief PackedVal のビット長
const size_t kPvBitLen = SIZEOF_UNSIGNED_LONG * 8;


/// @brief word 中の1のビット数を数える．
/// @param[in] word 対象のワード
/// @return word 中の1のビット数
inline
size_t
count_ones(PackedVal word)
{
#if SIZEOF_UNSIGNED_LONG == 4
  const size_t mask1   = 0x55555555;
  const size_t mask2   = 0x33333333;
  const size_t mask4   = 0x0f0f0f0f;
  const size_t mask8   = 0x00ff00ff;
  const size_t mask16  = 0x0000ffff;

  word = (word & mask1)  + ((word >>  1) & mask1);
  word = (word & mask2)  + ((word >>  2) & mask2);
  word = (word & mask4)  + ((word >>  4) & mask4);
  word = (word & mask8)  + ((word >>  8) & mask8);
  word = (word & mask16) + ((word >> 16) & mask16);
  return word;
#else
#if SIZEOF_UNSIGNED_LONG == 8
  const size_t mask1  = 0x5555555555555555;
  const size_t mask2  = 0x3333333333333333;
  const size_t mask4  = 0x0f0f0f0f0f0f0f0f;
  const size_t mask8  = 0x00ff00ff00ff00ff;
  const size_t mask16 = 0x0000ffff0000ffff;
  const size_t mask32 = 0x00000000ffffffff;

  word = (word & mask1)  + ((word >>  1) & mask1);
  word = (word & mask2)  + ((word >>  2) & mask2);
  word = (word & mask4)  + ((word >>  4) & mask4);
  word = (word & mask8)  + ((word >>  8) & mask8);
  word = (word & mask16) + ((word >> 16) & mask16);
  word = (word & mask32) + ((word >> 32) & mask32);
  return word;
#else
  size_t n = SIZEOF_UNSIGNED_LONG * 8;
  size_t c = 0;
  for (size_t i = 0; i < n; ++ i) {
    if ( word & 1 ) {
      ++ c;
    }
    word >>= 1;
  }
  return word;
#endif
#endif
}

END_NAMESPACE_YM_ATPG

#endif // PACKEDVAL_H
