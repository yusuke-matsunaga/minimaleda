// $Id: SimRepeatNode.cc 700 2007-05-31 00:41:30Z matsunaga $

// 繰り返しノード

#if HAVE_CONFIG_H
#include <ymconfig.h>
#endif

#include "SimRepeatNode.h"

#include <ym_verilog/vpi/VpiExpr.h>
#include "SimEngine.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// repeat 構造を表すノード
//////////////////////////////////////////////////////////////////////

// コンストラクタ
SimRepeatNode::SimRepeatNode(SimEngine* engine,
			     const VpiScope* scope,
			     VpiExpr* repnum,
			     SimNode* start,
			     SimNode* end) :
  SimNode(engine, scope),
  mRepNum(repnum)
{
  mRepeatLink = new SimRepeatLink(engine, scope, this);
  mRepeatLink->mBody = start;
  //end->set_next(mRepeatLink);
}

// デストラクタ
SimRepeatNode::~SimRepeatNode()
{
}

// 繰り返しの開始処理
void
SimRepeatNode::init()
{
  mRepeatLink->mCurNum = mRepNum->eval_int();
  mRepeatLink->check();
}

// 実行する
void
SimRepeatNode::exec()
{
  init();
}

// 繰り返したあとの処理
void
SimRepeatNode::emit()
{
  //schedule_next();
}


//////////////////////////////////////////////////////////////////////
// repeat つきの nonblocking 代入文
//////////////////////////////////////////////////////////////////////

// コンストラクタ
SimNbRepeatNode::SimNbRepeatNode(SimEngine* engine,
				 const VpiScope* scope,
				 VpiExpr* repnum,
				 SimNode* start,
				 SimNode* end,
				 SimUpdateNode* update) :
  SimRepeatNode(engine, scope, repnum, start, end),
  mUpdate(update)
{
}

// デストラクタ
SimNbRepeatNode::~SimNbRepeatNode()
{
}

// 実行する
void
SimNbRepeatNode::exec()
{
  init();

  //schedule_next();
}

// 繰り返したあとの処理
void
SimNbRepeatNode::emit()
{
  assert_cond(mUpdate, __FILE__, __LINE__);
  engine()->reg_nonblocking_event(mUpdate);
}


//////////////////////////////////////////////////////////////////////
// repeat 用の待ち受けノード
//////////////////////////////////////////////////////////////////////

// コンストラクタ
SimRepeatLink::SimRepeatLink(SimEngine* engine,
			     const VpiScope* scope,
			     SimRepeatNode* parent) :
  SimNode(engine, scope),
  mParent(parent)
{
  mBody = NULL;
}

// デストラクタ
SimRepeatLink::~SimRepeatLink()
{
}

// 実行する
void
SimRepeatLink::exec()
{
  -- mCurNum;
  check();
}

// 繰り返しの判定処理
void
SimRepeatLink::check()
{
  if ( mCurNum > 0 ) {
    engine()->reg_active_event(mBody);
  }
  else {
    mParent->emit();
  }
}

END_NAMESPACE_YM_VERILOG
