
/// @file libym_verilog/simulator/SimEngine.cc
/// @brief simulator の心臓部
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: SimEngine.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2006 Yusuke Matsunaga
/// All rights reserved.


#if HAVE_CONFIG_H
#include <ymconfig.h>
#endif

#include <ym_verilog/vpi/VpiScope.h>

#include "SimEngine.h"
#include "SimEventQueue.h"
#include "SimCaLinkQueue.h"
#include "SimNode.h"
#include "SimDelayNode.h"
#include "SimEventNode.h"
#include "SimForkNode.h"
#include "SimPcaNode.h"
#include "SimRepeatNode.h"
#include "SimCaLink.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// Simulator の心臓部
//////////////////////////////////////////////////////////////////////

// 唯一のオブジェクト
SimEngine* SimEngine::mTheObj = NULL;

// 唯一のオブジェクトを取り出すクラスメソッド
// いわゆる singleton パターン
SimEngine*
SimEngine::the_obj()
{
  if ( !mTheObj ) {
    mTheObj = new SimEngine;
  }
  return mTheObj;
}

// コンストラクタ
SimEngine::SimEngine() :
  mCurTime(0, 0)
{
  mCurEqSet = find_eqset(VlTime(0, 0));
  mCurEventQptr = new SimEventQueue;
  mMonitorQptr = new SimEventQueue;
  mCurActiveQptr = mCurEventQptr;
  mCurInactiveQptr = &mCurEqSet->mInactiveQueue;
  mCurNonblockingQptr = &mCurEqSet->mNonblockingQueue;
  mEqSetMgr = new SimEqSetMgr;
  mAvailEqSet = NULL;
}

// デストラクタ
SimEngine::~SimEngine()
{
  delete mCurEqSet;
  delete mCurEventQptr;
  delete mMonitorQptr;
  delete mEqSetMgr;

  SimEqSet* next;
  for (SimEqSet* eqset = mAvailEqSet; eqset; eqset = next) {
    next = eqset->mAvailLink;
    delete eqset;
  }

  clear_node();
  clear_calink();
}

// シミュレーションを行う．
void
SimEngine::run()
{
  if ( !mCurEqSet ) {
    // イベントキューが空なら終わる．
    return;
  }
  for ( ; ; ) {
    if ( !mCurActiveQptr->empty() ) {
      // active キューが空でなければ処理を続ける．
    }
    else if ( !mCurInactiveQptr->empty() ) {
      // inactive キューを active にする．
      // 昔の active キューを inactive キューにする．
      SimEventQueue* tmp = mCurActiveQptr;
      mCurActiveQptr = mCurInactiveQptr;
      mCurInactiveQptr = tmp;
    }
    else if ( !mCurNonblockingQptr->empty() ) {
      // nonblocking キューを active にする．
      // 昔の active キューを nonblocking キューにする．
      SimEventQueue* tmp = mCurActiveQptr;
      mCurActiveQptr = mCurNonblockingQptr;
      mCurNonblockingQptr = tmp;
    }
    else if ( !mMonitorQptr->empty() ) {
      // monitor キューを active にする．
      mCurActiveQptr = mMonitorQptr;
    }
    else if ( !mEqSetMgr->empty() ) {
      // 直近のイベント時刻を現在の時刻にする．
      delete_eqset(mCurEqSet);
      mCurEqSet = mEqSetMgr->get_min();
      mCurActiveQptr = &mCurEqSet->mInactiveQueue;
      mCurInactiveQptr = mCurEventQptr;
      mCurNonblockingQptr = &mCurEqSet->mNonblockingQueue;
    }
    else {
      // イベントが無くなった．
      break;
    }

    // ここまでの処理は deterministic
    // ここからは active queue のどのイベントを先に処理してもよい
    // --> nondeterminism の源となる．
    // 特に，update イベントの結果によって引き起こされる continuous assignment
    // の取扱いで処理順序は大きく変わる．

    // active queue の中身を実行する．
    SimNode* ev;
    while ( (ev = mCurActiveQptr->pop()) ) {
      ev->exec();
    }

    // continuous assignment の評価を行う．
    SimCaLink* ca;
    while ( (ca = mCaLinkQptr->pop()) ) {
      ca->clear_changed();
      ca->start()->exec();
    }
    
  }
}

// 現時刻を取り出す．
VlTime
SimEngine::cur_time() const
{
  return mCurTime;
}

// 現時刻の active イベントを登録する．
// active イベントは現時刻以外あり得ない．
void
SimEngine::reg_active_event(SimNode* event)
{
  assert_cond(mCurActiveQptr, __FILE__, __LINE__);
  mCurActiveQptr->push(event);
}

// 現時刻の inactive イベントを登録する．
void
SimEngine::reg_inactive_event(SimNode* event)
{
  assert_cond(mCurInactiveQptr, __FILE__, __LINE__);
  mCurInactiveQptr->push(event);
}

// 現時刻の non-blocking イベントを登録する．
void
SimEngine::reg_nonblocking_event(SimNode* event)
{
  assert_cond(mCurNonblockingQptr, __FILE__, __LINE__);
  mCurNonblockingQptr->push(event);
}

// 現時刻の monitor イベントを登録する．
// monitor イベントは現時刻以外あり得ない．
void
SimEngine::reg_monitor_event(SimNode* event)
{
  mMonitorQptr->push(event);
}

// delay 時刻後に inactive イベントをスケジュールする．
void
SimEngine::reg_inactive_event(SimNode* event,
			      const VlTime& delay)
{
  // 絶対時刻になおす
  VlTime at = cur_time() + delay;
  SimEqSet* eqset = find_eqset(at);
  eqset->mInactiveQueue.push(event);
}

// delay 時刻後に non-blocking イベントをスケジュールする．
void
SimEngine::reg_nonblocking_event(SimNode* event,
				 const VlTime& delay)
{
  // 絶対時刻になおす
  VlTime at = cur_time() + delay;
  SimEqSet* eqset = find_eqset(at);
  eqset->mNonblockingQueue.push(event);
}

// 評価すべき continuous assignment を登録する．
void
SimEngine::reg_contassign(SimCaLink* calink)
{
  mCaLinkQptr->push(calink);
}


//////////////////////////////////////////////////////////////////////
// SimNode の管理関係
//////////////////////////////////////////////////////////////////////

// SimNode を登録する．
void
SimEngine::push_node(SimNode* node)
{
  mNodeList.push_back(node);
}

// 登録されている SimNode をすべて削除する．
void
SimEngine::clear_node()
{
  for (list<SimNode*>::iterator p = mNodeList.begin();
       p != mNodeList.end(); ++ p) {
    delete *p;
  }
  mNodeList.clear();
}


//////////////////////////////////////////////////////////////////////
// SimCaLink の管理関係
//////////////////////////////////////////////////////////////////////

// SimCaLink を生成する．
SimCaLink*
SimEngine::new_calink(SimNode* start)
{
  SimCaLink* calink = new SimCaLink(this, start);
  mCaLinkList.push_back(calink);
  return calink;
}

// 登録されている SimCaLink をすべて削除する．
void
SimEngine::clear_calink()
{
  for (list<SimCaLink*>::iterator p = mCaLinkList.begin();
       p != mCaLinkList.end(); ++ p) {
    delete *p;
  }
  mCaLinkList.end();
}


//////////////////////////////////////////////////////////////////////
// シミュレーション用のノードを生成する関数
//////////////////////////////////////////////////////////////////////

SimEvalNode*
SimEngine::new_evalnode(VpiScope* scope,
			VpiExpr* expr)
{
  return new SimEvalNode(this, scope, expr);
}

SimEventTrigger*
SimEngine::new_eventtrigger(VpiScope* scope,
			    VpiNamedEvent* event)
{
  return new SimEventTrigger(this, scope, event);
}

SimUpdateNode*
SimEngine::new_updatenode(VpiScope* scope,
			  VpiExpr* lhs,
			  SimEvalNode* rhs)
{
  return new SimUpdateNode(this, scope, lhs, rhs);
}

SimCondBranch*
SimEngine::new_condbranch(VpiScope* scope,
			  SimEvalNode* expr,
			  SimEvalNode* label,
			  int case_type)
{
  return new SimCondBranch(this, scope, expr, label, case_type);
}

SimDelayNode*
SimEngine::new_delaynode(VpiScope* scope,
			 VpiExpr* delay)
{
  return new SimDelayNode(this, scope, delay);
}

SimEventNode*
SimEngine::new_eventnode(VpiScope* scope,
			 const list<VpiExpr*>& event_list)
{
  return new SimEventNode(this, scope, event_list);
}

SimRepeatNode*
SimEngine::new_repeatnode(VpiScope* scope,
			  VpiExpr* num,
			  SimNode* start,
			  SimNode* end)
{
  return new SimRepeatNode(this, scope, num, start, end);
}

SimNonblocking*
SimEngine::new_nonblocking(VpiScope* scope,
			   SimUpdateNode* update)
{
  return new SimNonblocking(this, scope, update);
}

SimNbDelayNode*
SimEngine::new_nbdelaynode(VpiScope* scope,
			   VpiExpr* delay,
			   SimUpdateNode* update)
{
  return new SimNbDelayNode(this, scope, delay, update);
}

SimNbEventNode*
SimEngine::new_nbeventnode(VpiScope* scope,
			   const list<VpiExpr*>& event_list,
			   SimUpdateNode* update)
{
  return new SimNbEventNode(this, scope, event_list, update);
}

SimNbRepeatNode*
SimEngine::new_nbrepeatnode(VpiScope* scope,
			    VpiExpr* num,
			    SimNode* start,
			    SimNode* end,
			    SimUpdateNode* update)
{
  return new SimNbRepeatNode(this, scope, num, start, end, update);
}

SimForkNode*
SimEngine::new_forknode(VpiScope* scope)
{
  return new SimForkNode(this, scope);
}

SimTaskEnable*
SimEngine::new_taskenable(VpiScope* scope)
{
  return new SimTaskEnable(this, scope);
}

SimEnableNode*
SimEngine::new_enablenode(VpiScope* scope,
			  const VpiScope* enable)
{
  return new SimEnableNode(this, scope, enable);
}

SimDisableNode*
SimEngine::new_disablenode(VpiScope* scope,
			   const VpiScope* disable)
{
  return new SimDisableNode(this, scope, disable);
}

#if 0
SimPcaNode*
SimEngine::new_pcanode(VpiScope* scope,
		       Value* p_value,
		       SimEvalNode* rhs)
{
  return new SimPcaNode(this, scope, p_value, rhs);
}

SimAssignNode*
SimEngine::new_assignnode(VpiScope* scope,
			  const VpiAssignStmt* as_stmt,
			  SimCaLink* calink)
{
  return new SimAssignNode(this, scope, as_stmt, calink);
}

SimDeassignNode*
SimEngine::new_deassignnode(VpiScope* scope,
			    VpiExpr* lhs)
{
  #warning "not yet done"
  return NULL;
}

SimForceNode*
SimEngine::new_forcenode(VpiScope* scope,
			 const VpiForceStmt* force_stmt,
			 SimCaLink* calink)
{
  return new SimForceNode(this, scope, force_stmt, calink);
}

SimReleaseNode*
SimEngine::new_releasenode(VpiScope* scope,
			   VpiExpr* lhs)
{

  #warning "not yet done"
  return NULL;
}
#endif


//////////////////////////////////////////////////////////////////////
// SimEqSet の管理関係
//////////////////////////////////////////////////////////////////////

// SimEqSet を破棄する．
void
SimEngine::delete_eqset(SimEqSet* eqset)
{
  // 実際には mAvailEqSet のリストにつなぐだけ
  // あとで new_eqset() によって再利用される可能性がある．
  eqset->mAvailLink = mAvailEqSet;
  mAvailEqSet = eqset;
}

// time の時刻のイベントを取り出す．
// なければ新規に作る．
SimEqSet*
SimEngine::find_eqset(const VlTime& time)
{
  SimEqSet* eqset = mEqSetMgr->find(time);
  if ( eqset ) {
    return eqset;
  }

  // その時刻のイベントは登録されていないので新たなイベントキュー
  // をつくって登録する．
  eqset = mAvailEqSet;
  if ( eqset) {
    // まず mAvailEqSet が空でなければそちらを使う
    mAvailEqSet = eqset->mAvailLink;
  }
  else {
    // そうでなければしょうがないので新しい要素をアロケートする．
    eqset = new SimEqSet;
  }
  eqset->mTime = time;

  mEqSetMgr->add(eqset);

  return eqset;
}


//////////////////////////////////////////////////////////////////////
// @brief SimEqSet の集合を管理するクラス
// 具体的には，時刻をキーにしたヒープと時刻をキーにしたハッシュ表
// の機能を持つ．
//////////////////////////////////////////////////////////////////////

// コンストラクタ
SimEqSetMgr::SimEqSetMgr() :
  mHash(NULL),
  mHashSize(0)
{
  resize(16);
}

// デストラクタ
SimEqSetMgr::~SimEqSetMgr()
{
  delete [] mHash;
}

// 要素を追加する．
void
SimEqSetMgr::add(SimEqSet* eqset)
{
  // ヒープに追加
  mHeap.push_back(eqset);
  ymuint32 pos = mHeap.size() - 1;
  while ( pos > 0 ) {
    SimEqSet* val = mHeap[pos];
    ymuint32 parent = (pos - 1) >> 1;
    SimEqSet* val_p = mHeap[parent];
    if ( val_p->time() > val->time() ) {
      SimEqSet* tmp = val_p;
      val_p = val;
      val = tmp;
      pos = parent;
    }
    else {
      pos = 0;
    }
  }

  // ハッシュに追加
  if ( num() > mNextLimit ) {
    resize(mHashSize * 2);
  }
  const VlTime& time = eqset->time();
  pos = time.hash() % mHashSize;
  eqset->mHashLink = mHash[pos];
  mHash[pos] = eqset;
}

// time の時刻を持つ要素を取り出す．
// なければ NULL を返す．
SimEqSet*
SimEqSetMgr::find(const VlTime& time) const
{
  ymuint32 pos = time.hash() % mHashSize;
  for (SimEqSet* eqset = mHash[pos]; eqset; eqset = eqset->mHashLink) {
    if ( eqset->time() == time ) {
      return eqset;
    }
  }
  return NULL;
}

// 最小の時刻を持つ要素を取り出す．
SimEqSet*
SimEqSetMgr::get_min()
{
  assert_cond( !mHeap.empty(), __FILE__, __LINE__);

  // 最小の要素は常に先頭にある．
  SimEqSet* min_eqset = mHeap[0];

  // ヒープの先頭の要素を削除する．
  ymuint32 pos = mHeap.size() - 1;
  if ( pos > 0 ) {
    // 末尾の要素を先頭に持ってくる．
    mHeap[0] = mHeap[pos];
    mHeap.pop_back();

    ymuint32 parent = 0;
    ymuint32 n = mHeap.size();
    for ( ; ; ) {
      // ヒープ木の性質から親から子の位置がわかる
      ymuint32 left = parent + parent + 1;
      ymuint32 right = left + 1;
      if ( right > n ) {
	// 左右の子どもを持たない場合
	break;
      }
      SimEqSet* val_p = mHeap[parent];
      SimEqSet* val_l = mHeap[left];
      if ( right == n ) {
	// 右の子どもを持たない場合
	if ( val_p->time() > val_l->time() ) {
	  // 逆転
	  SimEqSet* tmp = val_p;
	  val_p = val_l;
	  val_l = tmp;
	}
	break;
      }
      else {
	SimEqSet* val_r = mHeap[right];
	if ( val_p->time() > val_l->time() &&
	     val_l->time() <= val_r->time() ) {
	  // 左の子どもと取り替える．次は左の子で同じ処理をする
	  SimEqSet* tmp = val_p;
	  val_p = val_l;
	  val_l = tmp;
	  parent = left;
	}
	else if ( val_p->time() > val_r->time() &&
		  val_r->time() < val_l->time() ) {
	  // 右の子どもと取り替える．次は右の子で同じ処理をする
	  SimEqSet* tmp = val_p;	
	  val_p = val_r;
	  val_r = tmp;
	  parent = right;
	}
	else {
	  break;
	}
      }
    }
  }

  // ハッシュから削除する．
  pos = min_eqset->time().hash() % mHashSize;
  SimEqSet** pprev = &mHash[pos];
  for ( ; ; ) {
    SimEqSet* tmp = *pprev;
    assert_cond(tmp, __FILE__, __LINE__);
    if ( tmp == min_eqset ) {
      *pprev = min_eqset->mHashLink;
      break;
    }
    pprev = &tmp->mHashLink;
  }

  return min_eqset;
}

// ハッシュ表のサイズを拡張する．
void
SimEqSetMgr::resize(ymuint32 size)
{
  SimEqSet** old_hash = mHash;
  ymuint32 old_size = mHashSize;

  mHashSize = size;
  mHash = new SimEqSet*[mHashSize];
  for (ymuint32 i = 0; i < mHashSize; ++ i) {
    mHash[i] = NULL;
  }

  for (ymuint32 i = 0; i < old_size; ++ i) {
    SimEqSet* next;
    for (SimEqSet* eqset = old_hash[i]; eqset; eqset = next) {
      next = eqset->mHashLink;
      ymuint32 pos = eqset->time().hash() % mHashSize;
      eqset->mHashLink = mHash[pos];
      mHash[pos] = eqset;
    }
  }

  mNextLimit = mHashSize * 2;
}

END_NAMESPACE_YM_VERILOG
