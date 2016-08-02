#ifndef LIBYM_VERILOG_SIMULATOR_SIMENGINE_H
#define LIBYM_VERILOG_SIMULATOR_SIMENGINE_H

/// @file libym_verilog/simulator/SimEngine.h
/// @brief simulator の心臓部
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: SimEngine.h 247 2006-10-30 02:52:13Z matsunaga $
///
/// Copyright (C) 2005-2006 Yusuke Matsunaga
/// All rights reserved.


#include <ym_verilog/verilog.h>
#include <ym_verilog/VlTime.h>


BEGIN_NAMESPACE_YM_VERILOG

class VpiScope;
class VpiNamedEvent;
class VpiExpr;

class SimEqSetMgr;
class SimAssignNode;
class SimCondBranch;
class SimCaLink;
class SimCaLinkQueue;
class SimDeassignNode;
class SimDelayNode;
class SimDisableNode;
class SimEnableNode;
class SimEqSet;
class SimEvalNode;
class SimEventQueue;
class SimEventTrigger;
class SimForkNode;
class SimEventNode;
class SimForceNode;
class SimNbDelayNode;
class SimNbEventNode;
class SimNbRepeatNode;
class SimNode;
class SimNonblocking;
class SimPcaNode;
class SimReleaseNode;
class SimRepeatNode;
class SimTaskEnable;
class SimUpdateNode;

//////////////////////////////////////////////////////////////////////
// Simulator の心臓部
//////////////////////////////////////////////////////////////////////
class SimEngine
{
public:

  // 唯一のオブジェクトを取り出すクラスメソッド
  // いわゆる singleton パターン
  static
  SimEngine* the_obj();

private:
  // そのため，コンストラクタとデストラクタは隠す必要がある．

  // コンストラクタ
  SimEngine();

  // デストラクタ
  ~SimEngine();


public:

  // シミュレーションを行う．
  void run();

  // 現時刻を取り出す．
  VlTime cur_time() const;

  // 現時刻の active イベントを登録する．
  // active イベントの登録は現時刻以外あり得ない．
  void reg_active_event(SimNode* event);

  // 現時刻の inactive イベントを登録する．
  void reg_inactive_event(SimNode* event);

  // 現時刻の non-blocking イベントを登録する．
  void reg_nonblocking_event(SimNode* event);

  // 現時刻の monitor イベントを登録する．
  // monitor イベントの登録は現時刻以外あり得ない．
  void reg_monitor_event(SimNode* event);

  // delay 時刻後に inactive イベントをスケジュールする．
  void reg_inactive_event(SimNode* event,
			  const VlTime& delay);

  // delay 時刻後に non-blocking イベントをスケジュールする．
  void reg_nonblocking_event(SimNode* event,
			     const VlTime& delay);

  // 評価すべき continuous assignment を登録する．
  void reg_contassign(SimCaLink* contassign);


  //////////////////////////////////////////////////////////////////////
  // SimNode の管理関係
  //////////////////////////////////////////////////////////////////////

  //  SimNode を登録する．
  void push_node(SimNode* node);

  // 登録されている SimNode をすべて削除する．
  void clear_node();

  
  //////////////////////////////////////////////////////////////////////
  // SimCaLink の管理関係
  //////////////////////////////////////////////////////////////////////

  // SimCaLink を生成する．
  SimCaLink* new_calink(SimNode* start);

  // 登録されている SimCaLink をすべて削除する．
  void clear_calink();

  
  //////////////////////////////////////////////////////////////////////
  // シミュレーション用のノードを生成する関数
  //////////////////////////////////////////////////////////////////////

  SimEvalNode* new_evalnode(VpiScope* scope,
			    VpiExpr* expr);
  SimEventTrigger* new_eventtrigger(VpiScope* scope,
				    VpiNamedEvent* event);
  SimUpdateNode* new_updatenode(VpiScope* scope,
				VpiExpr* lhs,
				SimEvalNode* rhs);
  SimCondBranch* new_condbranch(VpiScope* scope,
				SimEvalNode* expr,
				SimEvalNode* label,
				int case_type);
  SimDelayNode* new_delaynode(VpiScope* scope,
			      VpiExpr* delay);
  SimEventNode* new_eventnode(VpiScope* scope,
			      const list<VpiExpr*>& event_list);
  SimRepeatNode* new_repeatnode(VpiScope* scope,
				VpiExpr* num,
				SimNode* start,
				SimNode* end);
  SimNonblocking* new_nonblocking(VpiScope* scope,
				  SimUpdateNode* update);
  SimNbDelayNode* new_nbdelaynode(VpiScope* scope,
				  VpiExpr* delay,
				  SimUpdateNode* update);
  SimNbEventNode* new_nbeventnode(VpiScope* scope,
				  const list<VpiExpr*>& event_list,
				  SimUpdateNode* update);
  SimNbRepeatNode* new_nbrepeatnode(VpiScope* scope,
				    VpiExpr* num,
				    SimNode* start,
				    SimNode* end,
				    SimUpdateNode* update);
  SimForkNode* new_forknode(VpiScope* scope);
  SimTaskEnable* new_taskenable(VpiScope* scope);
  SimEnableNode* new_enablenode(VpiScope* scope,
				const VpiScope* enable);
  SimDisableNode* new_disablenode(VpiScope* scope,
				  const VpiScope* disable);
#if 0
  SimPcaNode* new_pcanode(VpiScope* scope,
			  Value* p_value,
			  SimEvalNode* rhs);
  SimAssignNode* new_assignnode(VpiScope* scope,
				const VpiAssignStmt* as_stmt,
				SimCaLink* calink);
  SimDeassignNode* new_deassignnode(VpiScope* scope,
				    VpiExpr* lhs);
  SimForceNode* new_forcenode(VpiScope* scope,
			      const VpiForceStmt* force_stmt,
			      SimCaLink* calink);
  SimReleaseNode* new_releasenode(VpiScope* scope,
				  VpiExpr* lhs);
#endif

private:
  //////////////////////////////////////////////////////////////////////
  // SimEqSet の管理関係
  //////////////////////////////////////////////////////////////////////

  // SimEqSet を破棄する．
  void delete_eqset(SimEqSet* eqset);
  
  // time の時刻のイベントを取り出す．
  // なければ新規に作る．
  SimEqSet* find_eqset(const VlTime& time);

  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 現在の時刻
  VlTime mCurTime;

  // 現在の時刻に対応するイベントキュー
  // ここにあるということはすでに mEventHeap からは取り除かれている．
  SimEqSet* mCurEqSet;

  // (補助的な)イベントキュー
  SimEventQueue* mCurEventQptr;

  // 現在の active キュー
  SimEventQueue* mCurActiveQptr;

  // 現在の inactive キュー
  SimEventQueue* mCurInactiveQptr;

  // 現在の noblocking キュー
  SimEventQueue* mCurNonblockingQptr;

  // monitor イベントのキュー
  SimEventQueue* mMonitorQptr;

  // イベントキューを管理するクラス
  SimEqSetMgr* mEqSetMgr;

  // 現在のシミュレーションサイクル中に処理すべき
  // continuous assignment を入れる構造体
  SimCaLinkQueue* mCaLinkQptr;

  // SimNode のリスト
  list<SimNode*> mNodeList;

  // SimCaLink のリスト
  list<SimCaLink*> mCaLinkList;

  // 使用可能な SimEqSet
  SimEqSet* mAvailEqSet;
  
  // 唯一のオブジェクト
  static SimEngine* mTheObj;

};

END_NAMESPACE_YM_VERILOG

#endif // LIBYM_VERILOG_SIMULATOR_SIMENGINE_H
