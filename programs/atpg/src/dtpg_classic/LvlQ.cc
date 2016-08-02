
/// @file src/dtpg_classic/LvlQ.cc
/// @brief LvlQ の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: LvlQ.cc 2128 2009-03-12 07:25:33Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "LvlQ.h"


BEGIN_NAMESPACE_YM_ATPG_DTPGCL

LvlQ lvlq;

// @brief コンストラクタ
LvlQ::LvlQ()
{
}

// @brief デストラクタ
LvlQ::~LvlQ()
{
}

// @brief サイズを初期化する．
void
LvlQ::resize(ymuint req_size)
{
  mArray.resize(req_size);
}

// @brief 内容をクリアする．
void
LvlQ::clear()
{
  for (ymuint i = 0; i < mArray.size(); ++ i) {
    mArray[i] = NULL;
  }
  mMaxLevel = 0;
  mMinLevel = mArray.size();
  mNum = 0;
}

// @brief 外部出力に近い順に取り出す．
// @note 空なら NULL を返す．
Gate*
LvlQ::get_from_top()
{
  if ( mNum == 0 ) {
    return NULL;
  }
  Gate* gate;
  while ( (gate = mArray[mMaxLevel]) == NULL ) {
    -- mMaxLevel;
  }
  mArray[mMaxLevel] = gate->mNext;
  gate->mNext = NULL;
  gate->rst_lvlq();
  -- mNum;
  return gate;
}

// @brief 外部入力に近い順に取り出す．
// @note 空なら NULL を返す．
Gate*
LvlQ::get_from_bottom()
{
  if ( mNum == 0 ) {
    return NULL;
  }
  Gate* gate;
  while ( (gate = mArray[mMinLevel]) == NULL ) {
    ++ mMinLevel;
  }
  mArray[mMinLevel] = gate->mNext;
  gate->mNext = NULL;
  gate->rst_lvlq();
  -- mNum;
  return gate;
}

END_NAMESPACE_YM_ATPG_DTPGCL
