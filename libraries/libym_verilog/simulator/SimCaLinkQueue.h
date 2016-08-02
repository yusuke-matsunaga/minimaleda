#ifndef LIBYM_VERILOG_SIMULATOR_SIMCALINKQUEUE_H
#define LIBYM_VERILOG_SIMULATOR_SIMCALINKQUEUE_H

/// @file libym_verilog/simulator/SimCaLinkQueue.h
/// @brief simulator の部品クラスのヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: SimCaLinkQueue.h 355 2007-01-31 08:35:12Z matsunaga $
///
/// Copyright (C) 2005-2006 Yusuke Matsunaga
/// All rights reserved.


#include "SimEngine.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// continuous assignment を入れておくキュー
//////////////////////////////////////////////////////////////////////
class SimCaLinkQueue
{
public:

  // コンストラクタ
  SimCaLinkQueue();

  // デストラクタ
  ~SimCaLinkQueue();

  // continuous assignment を登録する．
  void push(SimCaLink* ca);

  // continuous assignment を取り出す．
  // 要素が無い場合には NULL を返す．
  SimCaLink* pop();

  // 登録されている内容をクリアする．
  void clear();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  list<SimCaLink*> mQueue;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// コンストラクタ
inline
SimCaLinkQueue::SimCaLinkQueue()
{
}

// デストラクタ
inline
SimCaLinkQueue::~SimCaLinkQueue()
{
}

// continuous assignment を登録する．
inline
void
SimCaLinkQueue::push(SimCaLink* ca)
{
  mQueue.push_back(ca);
}

// continuous assignment を取り出す．
// 要素が無い場合には NULL を返す．
inline
SimCaLink*
SimCaLinkQueue::pop()
{
  if ( mQueue.empty() ) {
    return NULL;
  }
  else {
    SimCaLink* ca = mQueue.front();
    mQueue.pop_front();
    return ca;
  }
}

// 登録されている内容をクリアする．
inline
void
SimCaLinkQueue::clear()
{
  mQueue.clear();
}

END_NAMESPACE_YM_VERILOG

#endif // LIBYM_VERILOG_SIMULATOR_SIMCALINKQUEUE_H
