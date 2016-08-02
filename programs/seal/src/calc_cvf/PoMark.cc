
/// @file calc_cvf/PoMark.cc
/// @brief PoMark の実装
/// @author Yusuke Matsunaga (松永 裕介)
/// 
/// $Id: PoMark.cc 1920 2008-12-20 15:52:42Z matsunaga $
///
/// Copyright (C) 2005-2008 Yusuke Matsunaga
/// All rights reserved.

#if HAVE_CONFIG_H
#include "seal_config.h"
#endif


#include "PoMark.h"


BEGIN_NAMESPACE_YM_SEAL_CVF

// @brief コンストラクタ
// @param[in] no 外部出力数
PoMark::PoMark(size_t no)
{
  if ( no > 0 ) {
    init(no);
  }
  else {
    mBlockNum = 0;
    mBitVector = NULL;
  }
}
  
// @brief デストラクタ
PoMark::~PoMark()
{
  delete [] mBitVector;
}

// @brief サイズを(再)設定する．
// @param[in] no 外部入力数
void
PoMark::init(size_t no)
{
  mBlockNum = (no + kPvBitLen - 1) / kPvBitLen;
  mBitVector = new tPackedVal[mBlockNum];
  for (size_t i = 0; i < mBlockNum; ++ i) {
    mBitVector[i] = 0UL;
  }
  mStart = mBlockNum;
  mEnd = 0;
}

// @brief クリアする．
void
PoMark::clear()
{
  for (size_t i = mStart; i < mEnd; ++ i) {
    mBitVector[i] = 0UL;
  }
  mStart = mBlockNum;
  mEnd = 0;
}

// @brief 指定した位置のビットを立てる．
// @param[in] pos ビットを立てる位置
void
PoMark::set(size_t pos)
{
  size_t idx = pos / kPvBitLen;
  size_t sft = pos % kPvBitLen;
  mBitVector[idx] |= (1UL << sft);
  if ( mStart > idx ) {
    mStart = idx;
  }
  if ( mEnd < idx + 1 ) {
    mEnd = idx + 1;
  }
}
  
// @brief 1 のビットの数を返す．
size_t
PoMark::size() const
{
  size_t ans = 0UL;
  for (size_t i = mStart; i < mEnd; ++ i) {
    ans += count_ones(mBitVector[i]);
  }
  return ans;
}

END_NAMESPACE_YM_SEAL_CVF

