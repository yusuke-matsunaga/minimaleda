// $Id: SimForkNode.cc 700 2007-05-31 00:41:30Z matsunaga $

// シミュレーション用のコントロールフローグラフ

#if HAVE_CONFIG_H
#include <ymconfig.h>
#endif

#include "SimForkNode.h"

#include "SimEngine.h"
#include "SimStmtGraph.h"
#include "SimScope.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// 複数のノードの実行を並行に開始するノード
//////////////////////////////////////////////////////////////////////

// コンストラクタ
SimForkNode::SimForkNode(SimEngine* engine,
			 const VpiScope* scope) :
  SimNode(engine, scope)
{

}

// デストラクタ
SimForkNode::~SimForkNode()
{
}

// 子供を追加する．
void
SimForkNode::add_child(const SimStmtGraph& sg)
{
  // scope()->scope() は紛らわしいが SimScope から VpiScope
  // を取り出している．処理的にも冗長である．
  SimJoinChild* jc
    = new SimJoinChild(engine(), scope()->scope(), this, sg.start());
  //sg.end()->set_next(jc);
  mChildList.push_back(jc);
}

// 実行する
void
SimForkNode::exec()
{
  for (list<SimJoinChild*>::const_iterator p = mChildList.begin();
       p != mChildList.end(); ++ p) {
    SimJoinChild* jc = *p;
    jc->mWaiting = true;
    engine()->reg_active_event(jc->mStart);
  }
}

// 子供の実行がすべて終わったかチェックする．
void
SimForkNode::check()
{
  for (list<SimJoinChild*>::const_iterator p = mChildList.begin();
       p != mChildList.end(); ++ p) {
    if ( (*p)->mWaiting ) {
      // まだ終わっていないノードがある．
      return;
    }
  }

  //schedule_next();
}


//////////////////////////////////////////////////////////////////////
// SimForkNode のための待ち受けノード
//////////////////////////////////////////////////////////////////////

// コンストラクタ
SimJoinChild::SimJoinChild(SimEngine* engine,
			   const VpiScope* scope,
			   SimForkNode* parent,
			   SimNode* start) :
  SimNode(engine, scope),
  mParent(parent),
  mStart(start)
{
  mWaiting = false;
}
  
// デストラクタ
SimJoinChild::~SimJoinChild()
{
}

// 実行する
void
SimJoinChild::exec()
{
  mWaiting = false;
  mParent->check();
}

END_NAMESPACE_YM_VERILOG
