#ifndef LIBYM_VERILOG_SIMULATOR_SIMDELAYLINK_H
#define LIBYM_VERILOG_SIMULATOR_SIMDELAYLINK_H

/// @file libym_verilog/simulator/SimDelayLink.h
/// @brief 遅延リンクを表すクラスのヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: SimEngine.h 241 2006-10-21 17:50:19Z matsunaga $
///
/// Copyright (C) 2005-2006 Yusuke Matsunaga
/// All rights reserved.


#include <ym_verilog/verilog.h>
#include <ym_verilog/VlTime.h>


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// 遅延リンクを表すクラス
//////////////////////////////////////////////////////////////////////
class SimDelayLink
{
  friend class SimEngine;
protected:

  // コンストラクタ
  SimDelayLink(SimEngine* engine,
	       const VpiScope* scope,
	       VpiExpr* delay);

  // デストラクタ
  virtual
  ~SimDelayLink();


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
