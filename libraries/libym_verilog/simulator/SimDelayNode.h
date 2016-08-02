#ifndef LIBYM_VERILOG_SIMULATOR_SIMDELAYNODE_H
#define LIBYM_VERILOG_SIMULATOR_SIMDELAYNODE_H

// $Id: SimDelayNode.h 232 2006-09-19 08:45:43Z matsunaga $

// 遅延ノード

#include "SimNode.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// delay を持ったノード
//////////////////////////////////////////////////////////////////////
class SimDelayNode :
  public SimNode
{
  friend class SimEngine;
protected:

  // コンストラクタ
  SimDelayNode(SimEngine* engine,
	       const VpiScope* scope,
	       VpiExpr* delay);

  // デストラクタ
  virtual
  ~SimDelayNode();


public:

  // 遅延値を得る．
  VlTime delay() const;


private:

  // 実行する．
  virtual
  void exec();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 遅延値
  VpiExpr* mDelay;

};


//////////////////////////////////////////////////////////////////////
// delay を持った nonblocking 代入文ノード
//////////////////////////////////////////////////////////////////////
class SimNbDelayNode :
  public SimDelayNode
{
  friend class SimEngine;
protected:

  // コンストラクタ
  SimNbDelayNode(SimEngine* engine,
		 const VpiScope* scope,
		 VpiExpr* delay,
		 SimUpdateNode* update);

  // デストラクタ
  virtual
  ~SimNbDelayNode();


private:

  // 実行する．
  virtual
  void exec();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // nonblocking 代入文
  SimUpdateNode* mUpdate;
  
};

END_NAMESPACE_YM_VERILOG

#endif // LIBYM_VERILOG_SIMULATOR_SIMDELAYNODE_H
