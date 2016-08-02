#ifndef LIBYM_VERILOG_SIMULATOR_SIMNODE_H
#define LIBYM_VERILOG_SIMULATOR_SIMNODE_H

/// @file libym_verilog/simulator/SimNode.h
/// @brief SimNode のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: SimNode.h 237 2006-10-05 10:13:09Z matsunaga $
///
/// Copyright (C) 2005-2006 Yusuke Matsunaga
/// All rights reserved.


#include <ym_verilog/verilog.h>
#include <ym_verilog/BitVector.h>


BEGIN_NAMESPACE_YM_VERILOG

class VpiScope;
class VpiNamedEvent;
class VpiExpr;
class SimEngine;
class SimScope;
class SimStmtGraph;

//////////////////////////////////////////////////////////////////////
// シミュレーション用のコントロールフローグラフのノードの基底クラス
//////////////////////////////////////////////////////////////////////
class SimNode
{
  friend class SimEngine;

protected:

  // コンストラクタ
  SimNode(SimEngine* engine,
	  const VpiScope* scope);

  // デストラクタ
  virtual
  ~SimNode();


public:

  // 親の SimEngine を返す．
  SimEngine* engine() const;

  // 所属しているスコープを返す．
  SimScope* scope() const;
  

private:

  // 実行する
  // 派生クラスが実装すること
  virtual
  void exec() = 0;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 親の SimEngine
  SimEngine* mEngine;

  // 属しているスコープ
  SimScope* mScope;

};


//////////////////////////////////////////////////////////////////////
// 評価を行うノード
//////////////////////////////////////////////////////////////////////
class SimEvalNode :
  public SimNode
{
  friend class VpiContAssign;
  friend class SimEngine;

protected:

  // コンストラクタ
  SimEvalNode(SimEngine* engine,
	      const VpiScope* scope,
	      VpiExpr* expr);

  // デストラクタ
  virtual
  ~SimEvalNode();


public:

  // 直前の実行結果を得る．
  const BitVector& value() const;


private:

  // 実行する．
  // 結果は mValue に格納される．
  virtual
  void exec();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 式
  VpiExpr* mExpr;

  // 定数式で再評価の必要がないとき true となるフラグ
  bool mConst;

  // 評価結果
  BitVector mValue;

};


//////////////////////////////////////////////////////////////////////
// イベントの生起を表すノード
//////////////////////////////////////////////////////////////////////
class SimEventTrigger :
  public SimNode
{
  friend class SimEngine;

protected:

  // コンストラクタ
  SimEventTrigger(SimEngine* engine,
		  const VpiScope* scope,
		  VpiNamedEvent* event);

  // デストラクタ
  virtual
  ~SimEventTrigger();


private:

  // 実行する．
  virtual
  void exec();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 名前つきイベント
  VpiNamedEvent* mEvent;

};


//////////////////////////////////////////////////////////////////////
// 値の変更を行うノード
//////////////////////////////////////////////////////////////////////
class SimUpdateNode :
  public SimNode
{
  friend class SimEngine;


protected:

  // コンストラクタ
  SimUpdateNode(SimEngine* engine,
		const VpiScope* scope,
		VpiExpr* lhs,
		SimEvalNode* rhs);

  // デストラクタ
  virtual
  ~SimUpdateNode();


private:

  // 実行する
  virtual
  void exec();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 左辺式
  VpiExpr* mLhs;

  // 右辺の評価ノード
  SimEvalNode* mRhs;

};


//////////////////////////////////////////////////////////////////////
// 条件分岐を行うノード
//////////////////////////////////////////////////////////////////////
class SimCondBranch :
  public SimNode
{
  friend class SimEngine;


protected:

  // コンストラクタ
  SimCondBranch(SimEngine* engine,
		const VpiScope* scope,
		SimEvalNode* expr,
		SimEvalNode* label,
		int case_type);

  // デストラクタ
  virtual
  ~SimCondBranch();


public:

  // expr == label が成り立ったときに実行するノードを設定する．
  void set_then(SimNode* node);

  // expr == label が成り立たなかったときに実行するノードを設定する．
  void set_else(SimNode* node);


private:

  // 実行する．
  virtual
  void exec();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 値を持っている評価ノード
  SimEvalNode* mExpr;

  // 比較すべきラベルの値を持っている評価ノード
  SimEvalNode* mLabel;

  // 評価の仕方
  // vpi_user.h の vpiCaseType 参照
  int mCaseType;

  // 成り立ったときに実行するノード
  SimNode* mThenLink;

  // 成り立たなかったときに実行するノード
  SimNode* mElseLink;

};


//////////////////////////////////////////////////////////////////////
// nonblocking 代入文ノード
//////////////////////////////////////////////////////////////////////
class SimNonblocking :
  public SimNode
{
  friend class SimEngine;


protected:

  // コンストラクタ
  SimNonblocking(SimEngine* engine,
		 const VpiScope* scope,
		 SimUpdateNode* update);

  // デストラクタ
  virtual
  ~SimNonblocking();


private:

  // 実行する．
  virtual
  void exec();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 代入文
  SimUpdateNode* mUpdate;

};


//////////////////////////////////////////////////////////////////////
// タスク呼び出しを行うノード
//////////////////////////////////////////////////////////////////////
class SimTaskEnable :
  public SimNode
{
  friend class SimEngine;


protected:

  // コンストラクタ
  SimTaskEnable(SimEngine* engine,
		const VpiScope* scope);

  // デストラクタ
  virtual
  ~SimTaskEnable();


private:

  // 実行する
  virtual
  void exec();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////


};


//////////////////////////////////////////////////////////////////////
// スコープを enable するためのノード
//////////////////////////////////////////////////////////////////////
class SimEnableNode :
  public SimNode
{
  friend class SimEngine;


protected:

  // コンストラクタ
  SimEnableNode(SimEngine* engine,
		const VpiScope* scope,
		const VpiScope* enable);

  // デストラクタ
  virtual
  ~SimEnableNode();


private:

  // 実行する．
  virtual
  void exec();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // enable すべきスコープ
  SimScope* mEnable;

};


//////////////////////////////////////////////////////////////////////
// disable 文に対応するノード
//////////////////////////////////////////////////////////////////////
class SimDisableNode :
  public SimNode
{
  friend class SimEngine;


protected:

  // コンストラクタ
  SimDisableNode(SimEngine* engine,
		 const VpiScope* scope,
		 const VpiScope* disable);

  // デストラクタ
  virtual
  ~SimDisableNode();

  
private:

  // 実行する．
  virtual
  void exec();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // disable すべきスコープ
  SimScope* mDisable;

};


END_NAMESPACE_YM_VERILOG

#endif // LIBYM_VERILOG_SIMULATOR_SIMNODE_H
