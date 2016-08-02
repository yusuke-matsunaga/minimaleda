// $Id: SimNode.cc 1052 2007-10-24 11:08:51Z matsunaga $

// シミュレーション用のコントロールフローグラフ

#if HAVE_CONFIG_H
#include <ymconfig.h>
#endif

#include "SimNode.h"

#include <ym_verilog/vpi/VpiExpr.h>
#include <ym_verilog/vpi/VpiScope.h>

#include "SimEngine.h"
#include "SimScope.h"



BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// シミュレーション用のコントロールフローグラフのノードの基底クラス //
//////////////////////////////////////////////////////////////////////

// コンストラクタ
SimNode::SimNode(SimEngine* engine,
		 const VpiScope* scope) :
  mEngine(engine)
  //  mScope(scope->find_simscope(engine))
{
  engine->push_node(this);
}

// デストラクタ
SimNode::~SimNode()
{
}

// 親の SimEngine を返す．
SimEngine*
SimNode::engine() const
{
  return mEngine;
}

// 所属しているスコープを返す．
SimScope*
SimNode::scope() const
{
  return mScope;
}


//////////////////////////////////////////////////////////////////////
// 評価を行うノード
//////////////////////////////////////////////////////////////////////

// コンストラクタ
SimEvalNode::SimEvalNode(SimEngine* engine,
			 const VpiScope* scope,
			 VpiExpr* expr) :
  SimNode(engine, scope),
  mExpr(expr)
{
  mConst = mExpr->is_const();
  if ( mConst ) {
    mExpr->eval_bitvector(mValue);
  }
}

// デストラクタ
SimEvalNode::~SimEvalNode()
{
}

// 直前の実行結果を得る．
const BitVector&
SimEvalNode::value() const
{
  return mValue;
}

// 実行する．
// 結果は mValue に格納される．
void
SimEvalNode::exec()
{
  if ( !mConst ) {
    mExpr->eval_bitvector(mValue);
  }
}


//////////////////////////////////////////////////////////////////////
// イベントの生起を表すノード
//////////////////////////////////////////////////////////////////////

// コンストラクタ
SimEventTrigger::SimEventTrigger(SimEngine* engine,
				 const VpiScope* scope,
				 VpiNamedEvent* event) :
  SimNode(engine, scope),
  mEvent(event)
{
}

// デストラクタ
SimEventTrigger::~SimEventTrigger()
{
}

// 実行する．
void
SimEventTrigger::exec()
{
  //mEvent->emit_event();
}


//////////////////////////////////////////////////////////////////////
// 値の変更を行うノード
//////////////////////////////////////////////////////////////////////

// コンストラクタ
SimUpdateNode::SimUpdateNode(SimEngine* engine,
			     const VpiScope* scope,
			     VpiExpr* lhs,
			     SimEvalNode* rhs) :
  SimNode(engine, scope),
  mLhs(lhs),
  mRhs(rhs)
{
}

// デストラクタ
SimUpdateNode::~SimUpdateNode()
{
}

// 実行する
void
SimUpdateNode::exec()
{
  mLhs->set(mRhs->value());
}


//////////////////////////////////////////////////////////////////////
// 条件分岐を行うノード
//////////////////////////////////////////////////////////////////////

// コンストラクタ
SimCondBranch::SimCondBranch(SimEngine* engine,
			     const VpiScope* scope,
			     SimEvalNode* expr,
			     SimEvalNode* label,
			     int case_type) :
  SimNode(engine, scope),
  mExpr(expr),
  mLabel(label),
  mCaseType(case_type)
{
  mThenLink = NULL;
  mElseLink = NULL;
}

// デストラクタ
SimCondBranch::~SimCondBranch()
{
}

// expr == label が成り立ったときに実行するノードを設定する．
void
SimCondBranch::set_then(SimNode* node)
{
  mThenLink = node;
}

// expr == label が成り立たなかったときに実行するノードを設定する．
void
SimCondBranch::set_else(SimNode* node)
{
  mElseLink = node;
}

// 実行する．
void
SimCondBranch::exec()
{
  BitVector eval = mExpr->value();
  BitVector lval = mLabel->value();

  bool result = false;
  switch ( mCaseType ) {
  case vpiCaseExact:
    result = conv_to_bool(eq(eval, lval));
    #warning "結果が X になったときどうするべきかをチェック"
    break;

  case vpiCaseX:
    result = eq_with_x(eval, lval);
    break;

  case vpiCaseZ:
    result = eq_with_xz(eval, lval);
    break;
  }

  if ( result ) {
    if ( mThenLink ) {
      engine()->reg_active_event(mThenLink);
    }
  }
  else {
    if ( mElseLink ) {
      engine()->reg_active_event(mElseLink);
    }
  }
}


//////////////////////////////////////////////////////////////////////
// nonblocking 代入文ノード
//////////////////////////////////////////////////////////////////////

// コンストラクタ
SimNonblocking::SimNonblocking(SimEngine* engine,
			       const VpiScope* scope,
			       SimUpdateNode* update) :
  SimNode(engine, scope),
  mUpdate(update)
{
}

// デストラクタ
SimNonblocking::~SimNonblocking()
{
}

// 実行する．
void
SimNonblocking::exec()
{
  assert_cond(mUpdate, __FILE__, __LINE__);
  engine()->reg_nonblocking_event(mUpdate);
}


//////////////////////////////////////////////////////////////////////
// タスク呼び出しを行うノード
//////////////////////////////////////////////////////////////////////

// コンストラクタ
SimTaskEnable::SimTaskEnable(SimEngine* engine,
			     const VpiScope* scope) :
  SimNode(engine, scope)
{
}

// デストラクタ
SimTaskEnable::~SimTaskEnable()
{
}

// 実行する
void
SimTaskEnable::exec()
{
}


//////////////////////////////////////////////////////////////////////
// スコープを enable するためのノード
//////////////////////////////////////////////////////////////////////

// コンストラクタ
SimEnableNode::SimEnableNode(SimEngine* engine,
			     const VpiScope* scope,
			     const VpiScope* enable) :
  SimNode(engine, scope)
{
  //mEnable = enable->find_simscope(engine);
}

// デストラクタ
SimEnableNode::~SimEnableNode()
{
}
  
// 実行する．
void
SimEnableNode::exec()
{
  mEnable->enable();
}


//////////////////////////////////////////////////////////////////////
// disable 文に対応するノード
//////////////////////////////////////////////////////////////////////

// コンストラクタ
SimDisableNode::SimDisableNode(SimEngine* engine,
			       const VpiScope* scope,
			       const VpiScope* disable) :
  SimNode(engine, scope)
{
  //mDisable = disable->find_simscope(engine);
}
    
// デストラクタ
SimDisableNode::~SimDisableNode()
{
}

// 実行する．
void
SimDisableNode::exec()
{
  mDisable->disable();
}

END_NAMESPACE_YM_VERILOG
