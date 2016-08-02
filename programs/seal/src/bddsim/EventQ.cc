
/// @file bddsim/EventQ.cc
/// @brief EventQ の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
/// 
/// $Id: EventQ.cc 1920 2008-12-20 15:52:42Z matsunaga $
///
/// Copyright (C) 2005-2007 Yusuke Matsunaga
/// All rights reserved.

#if HAVE_CONFIG_H
#include "seal_config.h"
#endif


#include "EventQ.h"
#include "SimNode.h"


BEGIN_NAMESPACE_YM_SEAL_BDDSIM

//////////////////////////////////////////////////////////////////////
// 故障シミュレーション用のイベントキューを表すクラス
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
EventQ::EventQ() :
  mArraySize(0),
  mArray(NULL)
{
  // 適当なサイズで初期化しておく
  init(128);
} 
  
// @brief デストラクタ
EventQ::~EventQ()
{
  delete [] mArray;
}

// @brief 初期化を行う．
// @param[in] max_level 最大レベル
void
EventQ::init(size_t max_level)
{
  if ( max_level >= mArraySize ) {
    delete [] mArray;
    mArraySize = max_level + 1;
    mArray = new SimNode*[mArraySize];
  }
  mCurLevel = 0;
  mMaxLevel = 0;
  for (size_t i = 0; i < max_level; ++ i) {
    mArray[i] = NULL;
  }
}

// @brief キューに積む
void
EventQ::put(SimNode* node)
{
  if ( !node->in_queue() ) {
    node->set_queue();
    size_t level = node->level();
    node->mLink = mArray[level];
    mArray[level] = node;
    if ( mMaxLevel < level ) {
      mMaxLevel = level;
    }
    if ( mCurLevel > level ) {
      mCurLevel = level;
    }
  }
}

// @brief キューから取り出す．
// @retval NULL キューが空だった．
SimNode*
EventQ::get()
{
  for ( ; mCurLevel <= mMaxLevel; ++ mCurLevel) {
    SimNode* node = mArray[mCurLevel];
    if ( node ) {
      node->clear_queue();
      mArray[mCurLevel] = node->mLink;
      return node;
    }
  }
  return NULL;
}

END_NAMESPACE_YM_SEAL_BDDSIM
