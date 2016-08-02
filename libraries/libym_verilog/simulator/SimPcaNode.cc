// $Id: SimPcaNode.cc 700 2007-05-31 00:41:30Z matsunaga $

// procedural continuous assignment ノード

#if HAVE_CONFIG_H
#include <ymconfig.h>
#endif

#include "SimPcaNode.h"

#include <ym_verilog/vpi/VpiExpr.h>
#include <ym_verilog/vpi/VpiStmt.h>
#include "SimEngine.h"
#include "SimCaLink.h"


BEGIN_NAMESPACE_YM_VERILOG

#if 0
//////////////////////////////////////////////////////////////////////
// procedural continuous assignment を行うためのノード
//////////////////////////////////////////////////////////////////////

// コンストラクタ
SimPcaNode::SimPcaNode(SimEngine* engine,
		       const VpiScope* scope,
		       BitVector* p_value,
		       SimEvalNode* rhs) :
  SimSeqNode(engine, scope),
  mValuePtr(p_value),
  mRhs(rhs)
{
}

// デストラクタ
SimPcaNode::~SimPcaNode()
{
}

// 実行する
void
SimPcaNode::exec_body()
{
  *mValuePtr = mRhs->value();
}


//////////////////////////////////////////////////////////////////////
// procedurual assign 文に対応するノード
//////////////////////////////////////////////////////////////////////

// コンストラクタ
SimAssignNode::SimAssignNode(SimEngine* engine,
			     const VpiScope* scope,
			     const VpiAssignStmt* stmt,
			     SimCaLink* ca_link) :
  SimSeqNode(engine, scope),
  mStmt(stmt),
  mCaLink(ca_link)
{
}

// デストラクタ
SimAssignNode::~SimAssignNode()
{
}

// 実行する．
void
SimAssignNode::exec()
{
  mCaLink->activate();
#if 0 // VPI
  int size = mStmt->lhs()->size();
  mStmt->lhs()->set_assign(mStmt, size - 1, 0);
#endif
}


//////////////////////////////////////////////////////////////////////
// force 文に対応するノード
//////////////////////////////////////////////////////////////////////

// コンストラクタ
SimForceNode::SimForceNode(SimEngine* engine,
			   const VpiScope* scope,
			   const VpiForceStmt* stmt,
			   SimCaLink* ca_link) :
  SimSeqNode(engine, scope),
  mStmt(stmt),
  mCaLink(ca_link)
{
}

// デストラクタ
SimForceNode::~SimForceNode()
{
}

// 実行する．
void
SimForceNode::exec()
{
  mCaLink->activate();
#if 0 // VPI
  int size = mStmt->lhs()->size();
  mStmt->lhs()->set_force(mStmt, size - 1, 0);
#endif
}
#endif

END_NAMESPACE_YM_VERILOG
