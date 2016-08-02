
/// @file libym_techmap/Match.cc
/// @brief Match の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "Match.h"
#include "ym_npn/NpnMap.h"


BEGIN_NAMESPACE_YM_TECHMAP

//////////////////////////////////////////////////////////////////////
// クラス Match
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] nl 葉の数(入力数)
Match::Match(ymuint nl) :
  mLeafNum(nl),
  mInvArray(0U)
{
  if ( nl > 0 ) {
    mLeafArray = new const SbjNode*[nl];
  }
  else {
    mLeafArray = NULL;
  }
}

// @brief コピーコンストラクタ
Match::Match(const Match& src) :
  mLeafNum(src.leaf_num()),
  mInvArray(src.mInvArray)
{
  if ( mLeafNum > 0 ) {
    mLeafArray = new const SbjNode*[mLeafNum];
    for (ymuint i = 0; i < mLeafNum; ++ i) {
      mLeafArray[i] = src.mLeafArray[i];
    }
  }
  else {
    mLeafArray = NULL;
  }
}

// @brief 代入演算子
const Match&
Match::operator=(const Match& src)
{
  resize(src.leaf_num());
  for (ymuint i = 0; i < mLeafNum; ++ i) {
    mLeafArray[i] = src.mLeafArray[i];
  }
  mInvArray = src.mInvArray;

  return *this;
}

// @brief デストラクタ
Match::~Match()
{
  delete [] mLeafArray;
}

END_NAMESPACE_YM_TECHMAP
