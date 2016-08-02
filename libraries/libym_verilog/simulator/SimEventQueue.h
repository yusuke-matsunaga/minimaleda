#ifndef LIBYM_VERILOG_SIMULATOR_SIMEVENTQUEUE_H
#define LIBYM_VERILOG_SIMULATOR_SIMEVENTQUEUE_H

/// @file libym_verilog/simulator/SimEventQueue.h
/// @brief イベントキューを表すクラスのヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: SimEventQueue.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2006 Yusuke Matsunaga
/// All rights reserved.


#include "SimEngine.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @brief イベントキュー
//////////////////////////////////////////////////////////////////////
class SimEventQueue
{
  friend class SimEqSet;
  friend class SimEngine;
  
private:

  // コンストラクタ
  SimEventQueue();

  // デストラクタ
  ~SimEventQueue();


public:

  // イベントを追加する．
  void push(SimNode* event);

  // イベントを取り出す．
  SimNode* pop();

  // 中身が空の時に true を返す．
  bool empty() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // キュー本体
  list<SimNode*> mQueue;

};


//////////////////////////////////////////////////////////////////////
/// @brief 一つの時刻におけるイベントキューのセット
/// - inactive キュー
/// - non-blocking キュー
/// を持つ．
/// 現在時刻以外では active キューと monitor キューは存在しない．
//////////////////////////////////////////////////////////////////////
class SimEqSet
{
  friend class SimEngine;
  friend class SimEqSetMgr;

private:
  
  // コンストラクタ
  SimEqSet();

  // デストラクタ
  ~SimEqSet();


public:

  // 時刻を得る．
  const VlTime& time() const;
  

private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 時刻
  VlTime mTime;

  // inactive イベントのキュー
  SimEventQueue mInactiveQueue;

  // non-blocking イベントのキュー
  SimEventQueue mNonblockingQueue;

  // avail list 用のリンク
  SimEqSet* mAvailLink;

  // ハッシュ表用のリンク
  SimEqSet* mHashLink;

};


//////////////////////////////////////////////////////////////////////
/// @brief SimEqSet の集合を管理するクラス
/// 具体的には，時刻をキーにしたヒープと時刻をキーにしたハッシュ表
/// の機能を持つ．
//////////////////////////////////////////////////////////////////////
class SimEqSetMgr
{
public:

  // コンストラクタ
  SimEqSetMgr();

  // デストラクタ
  ~SimEqSetMgr();

  // 要素を追加する．
  void add(SimEqSet* eqset);

  // time の時刻を持つ要素を取り出す．
  // なければ NULL を返す．
  SimEqSet* find(const VlTime& time) const;

  // イベントキューが空の時 true を返す．
  bool empty() const;

  // 最小の時刻を持つ要素を取り出す．
  SimEqSet* get_min();


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いる便利関数
  //////////////////////////////////////////////////////////////////////

  // 登録されている要素数
  ymuint32 num() const;

  // ハッシュ表のサイズを拡張する．
  void resize(ymuint32 size);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ヒープ
  vector<SimEqSet*> mHeap;

  // ハッシュ表
  SimEqSet** mHash;

  // ハッシュ表のサイズ
  ymuint32 mHashSize;

  // 要素数がこの値を超えたらテーブルを拡張する．
  ymuint32 mNextLimit;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// コンストラクタ
inline
SimEventQueue::SimEventQueue()
{
}

// デストラクタ
inline
SimEventQueue::~SimEventQueue()
{
}

// イベントを追加する．
inline
void
SimEventQueue::push(SimNode* node)
{
  mQueue.push_back(node);
}

// イベントを取り出す．
inline
SimNode*
SimEventQueue::pop()
{
  if ( empty() ) {
    return NULL;
  }
  else {
    SimNode* ans = mQueue.front();
    mQueue.pop_front();
    return ans;
  }
}

// 中身が空の時に true を返す．
inline
bool
SimEventQueue::empty() const
{
  return mQueue.empty();
}

// コンストラクタ
inline
SimEqSet::SimEqSet() :
  mAvailLink(NULL)
{
}

// デストラクタ
inline
SimEqSet::~SimEqSet()
{
}

// 時刻を得る．
inline
const VlTime&
SimEqSet::time() const
{
  return mTime;
}

// イベントキューが空の時 true を返す．
inline
bool
SimEqSetMgr::empty() const
{
  return mHeap.empty();
}

// 登録されている要素数
inline
ymuint32
SimEqSetMgr::num() const
{
  return mHeap.size();
}


END_NAMESPACE_YM_VERILOG

#endif // LIBYM_VERILOG_SIMULATOR_SIMEVENTQUEUE_H
