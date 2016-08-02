
/// @file libym_utils/DlList.cc
/// @brief DlList の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: DlList.cc 461 2007-03-21 18:07:40Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_utils/DlList.h"


BEGIN_NAMESPACE_YM


//////////////////////////////////////////////////////////////////////
// @class DlListBase DlList.h "DlList.h"
// @brief Dl のリスト
// @sa DlElem
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @note 空のリストになる
DlListBase::DlListBase() :
  mNum(0)
{
}

// @brief デストラクタ
DlListBase::~DlListBase()
{
  clear();
}

// @brief 内容を空にする．
void
DlListBase::clear()
{
  DlElem* n;
  for (DlElem* t = mDummyTop.mNextLink; t != &mDummyTop; t = n) {
    n = t->mNextLink;
    t->mNextLink = t;
    t->mPrevLink = t;
  }
  mDummyTop.mNextLink = &mDummyTop;
  mDummyTop.mPrevLink = &mDummyTop;
  mNum = 0;
}

END_NAMESPACE_YM
