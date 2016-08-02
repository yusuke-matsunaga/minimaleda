#ifndef BDDSIM_EVENTQ_H
#define BDDSIM_EVENTQ_H

/// @file bddsim/EventQ.h
/// @brief EventQ のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
/// 
/// $Id: EventQ.h 1920 2008-12-20 15:52:42Z matsunaga $
///
/// Copyright (C) 2005-2007 Yusuke Matsunaga
/// All rights reserved.

#include "nsdef.h"


BEGIN_NAMESPACE_YM_SEAL_BDDSIM

class SimNode;

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

  // キューに入っている最大のレベル．
  size_t mMaxLevel;
  
};

END_NAMESPACE_YM_SEAL_BDDSIM

#endif // BDDSIM_EVENTQ_H
