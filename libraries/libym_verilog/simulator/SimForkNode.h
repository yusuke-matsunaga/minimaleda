#ifndef LIBYM_VERILOG_SIMULATOR_SIMFORKNODE_H
#define LIBYM_VERILOG_SIMULATOR_SIMFORKNODE_H

// $Id: SimForkNode.h 232 2006-09-19 08:45:43Z matsunaga $

// 分岐ノード

#include "SimNode.h"


BEGIN_NAMESPACE_YM_VERILOG

class SimJoinChild;

//////////////////////////////////////////////////////////////////////
// 複数のノードの実行を並行に開始するノード
//////////////////////////////////////////////////////////////////////
class SimForkNode :
  public SimNode
{
  friend class SimEngine;
protected:

  // コンストラクタ
  SimForkNode(SimEngine* engine,
	      const VpiScope* scope);

  // デストラクタ
  virtual
  ~SimForkNode();


public:

  // 子供を追加する．
  void add_child(const SimStmtGraph& sg);

  // 子供の実行がすべて終わったかチェックする．
  // 終わっていたら exec() を呼ぶ．
  void check();
  

private:

  // 実行する
  virtual
  void exec();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 子供のリスト
  list<SimJoinChild*> mChildList;
  
};


//////////////////////////////////////////////////////////////////////
// SimForkNode のための待ち受けノード
//////////////////////////////////////////////////////////////////////
class SimJoinChild :
  public SimNode
{
  friend class SimForkNode;

private:

  // コンストラクタ
  SimJoinChild(SimEngine* engine,
	       const VpiScope* scope,
	       SimForkNode* parent,
	       SimNode* start);

  // デストラクタ
  virtual
  ~SimJoinChild();

  // 実行する．
  virtual
  void exec();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 実行完了待ちの時 true となるフラグ
  bool mWaiting;

  // 親のノード
  SimForkNode* mParent;

  // 実行すべき開始点のノード
  SimNode* mStart;

};

END_NAMESPACE_YM_VERILOG

#endif // LIBYM_VERILOG_SIMULATOR_SIMFORKNODE_H
