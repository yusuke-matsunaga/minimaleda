#ifndef LIBYM_VERILOG_SIMULATOR_SIMPCANODE_H
#define LIBYM_VERILOG_SIMULATOR_SIMPCANODE_H

// $Id: SimPcaNode.h 238 2006-10-05 10:16:56Z matsunaga $

// procedural continuous assignment ノード

#include "SimNode.h"


BEGIN_NAMESPACE_YM_VERILOG

#if 0
class SimCaLink;

//////////////////////////////////////////////////////////////////////
// procedural continuous assignment を行うためのノード
//////////////////////////////////////////////////////////////////////
class SimPcaNode :
  public SimSeqNode
{
  friend class SimEngine;

protected:

  // コンストラクタ
  SimPcaNode(SimEngine* engine,
	     const VpiScope* scope,
	     BitVector* p_value,
	     SimEvalNode* rhs);

  // デストラクタ
  virtual
  ~SimPcaNode();


private:

  // 実行する
  virtual
  void exec_body();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 左辺値を持つオブジェクトのアドレス．
  BitVector* mValuePtr;

  // 右辺の評価ノード
  SimEvalNode* mRhs;

};


//////////////////////////////////////////////////////////////////////
// procedural continuous assign 文に対応するノード
//////////////////////////////////////////////////////////////////////
class SimAssignNode :
  public SimSeqNode
{
  friend class SimEngine;
protected:

  // コンストラクタ
  SimAssignNode(SimEngine* engine,
		const VpiScope* scope,
		const VpiAssignStmt* stmt,
		SimCaLink* ca_link);

  // デストラクタ
  virtual
  ~SimAssignNode();


private:

  // 実行する．
  virtual
  void exec();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // assign 文
  const VpiAssignStmt* mStmt;

  // assign に対応するノード
  SimCaLink* mCaLink;
  
};


//////////////////////////////////////////////////////////////////////
// force 文に対応するノード
//////////////////////////////////////////////////////////////////////
class SimForceNode :
  public SimSeqNode
{
  friend class SimEngine;
protected:

  // コンストラクタ
  SimForceNode(SimEngine* engine,
	       const VpiScope* scope,
	       const VpiForceStmt* stmt,
	       SimCaLink* ca_link);

  // デストラクタ
  virtual
  ~SimForceNode();

private:

  // 実行する．
  virtual
  void exec();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // force 文
  const VpiForceStmt* mStmt;

  // assign に対応するノード
  SimCaLink* mCaLink;
  
};
#endif

END_NAMESPACE_YM_VERILOG

#endif // LIBYM_VERILOG_SIMULATOR_SIMPCANODE_H
