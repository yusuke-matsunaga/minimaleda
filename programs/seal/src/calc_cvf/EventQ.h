#ifndef CALC_CVF_EVENTQ_H
#define CALC_CVF_EVENTQ_H

/// @file calc_cvf/EventQ.h
/// @brief EventQ のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
/// 
/// $Id: EventQ.h 1920 2008-12-20 15:52:42Z matsunaga $
///
/// Copyright (C) 2005-2008 Yusuke Matsunaga
/// All rights reserved.

#include "nsdef.h"
#include "SimNode.h"


BEGIN_NAMESPACE_YM_SEAL_CVF

//////////////////////////////////////////////////////////////////////
/// @class EventQ EventQ.h "EventQ.h"
/// @brief 故障シミュレーション用のイベントキュー
///
/// キューに詰まれる要素は SimNode で，各々のノードはレベルを持つ．
/// このキューではレベルの小さい順に処理してゆく．同じレベルのノード
/// 間の順序は任意でよい．
//////////////////////////////////////////////////////////////////////
class EventQ
{
public:

  /// @brief コンストラクタ
  EventQ();
  
  /// @brief デストラクタ
  ~EventQ();


public:

  /// @brief 初期化を行う．
  /// @param[in] max_level 最大レベル
  void
  init(size_t max_level);

  /// @brief キューに積む
  void
  put(SimNode* node);

  /// @brief キューから取り出す．
  /// @retval NULL キューが空だった．
  SimNode*
  get();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // mArray のサイズ
  size_t mArraySize;

  // キューの先頭ノードの配列
  SimNode** mArray;

  // 現在のレベル．
  size_t mCurLevel;

  // キューに入っているノード数
  size_t mNum;
  
};

// @brief キューに積む
inline
void
EventQ::put(SimNode* node)
{
  if ( !node->in_queue() ) {
    node->set_queue();
    size_t level = node->level();
    SimNode*& w = mArray[level];
    node->mLink = w;
    w = node;
    if ( mNum == 0 || mCurLevel > level ) {
      mCurLevel = level;
    }
    ++ mNum;
  }
}

// @brief キューから取り出す．
// @retval NULL キューが空だった．
inline
SimNode*
EventQ::get()
{
  if ( mNum > 0 ) {
    // mNum が正しければ mCurLevel がオーバーフローすることはない．
    for ( ; ; ++ mCurLevel) {
      SimNode*& w = mArray[mCurLevel];
      SimNode* node = w;
      if ( node ) {
	node->clear_queue();
	w = node->mLink;
	-- mNum;
	return node;
      }
    }
  }
  return NULL;
}

END_NAMESPACE_YM_SEAL_CVF

#endif // CALC_CVF_EVENTQ_H
