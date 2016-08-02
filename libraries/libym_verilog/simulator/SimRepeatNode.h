#ifndef LIBYM_VERILOG_SIMULATOR_SIMREPEATNODE_H
#define LIBYM_VERILOG_SIMULATOR_SIMREPEATNODE_H

// $Id: SimRepeatNode.h 232 2006-09-19 08:45:43Z matsunaga $

// 繰り返しノード

#include "SimNode.h"


BEGIN_NAMESPACE_YM_VERILOG

class SimRepeatLink;

//////////////////////////////////////////////////////////////////////
// repeat 構造を表すノード
//////////////////////////////////////////////////////////////////////
class SimRepeatNode :
  public SimNode
{
  friend class SimEngine;
protected:

  // コンストラクタ
  SimRepeatNode(SimEngine* engine,
		const VpiScope* scope,
		VpiExpr* repnum,
		SimNode* start,
		SimNode* end);

  // デストラクタ
  virtual
  ~SimRepeatNode();

  // 繰り返しの開始処理
  void init();


public:

  // 繰り返したあとの処理
  virtual
  void emit();


private:

  // 実行する
  virtual
  void exec();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 繰り返し回数
  VpiExpr* mRepNum;

  // 待ち受けノード
  SimRepeatLink* mRepeatLink;

};


//////////////////////////////////////////////////////////////////////
// repeat つきの nonblocking 代入文
//////////////////////////////////////////////////////////////////////
class SimNbRepeatNode :
  public SimRepeatNode
{
  friend class SimEngine;
protected:

  // コンストラクタ
  SimNbRepeatNode(SimEngine* engine,
		  const VpiScope* scope,
		  VpiExpr* repnum,
		  SimNode* start,
		  SimNode* end,
		  SimUpdateNode* update);

  // デストラクタ
  virtual
  ~SimNbRepeatNode();


public:

  // 繰り返したあとの処理
  virtual
  void emit();


private:

  // 実行する
  virtual
  void exec();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // nonblocking 代入文
  SimUpdateNode* mUpdate;

};


//////////////////////////////////////////////////////////////////////
// repeat 用の待ち受けノード
//////////////////////////////////////////////////////////////////////
class SimRepeatLink :
  public SimNode
{
  friend class SimRepeatNode;
protected:

  // コンストラクタ
  SimRepeatLink(SimEngine* engine,
		const VpiScope* scope,
		SimRepeatNode* parent);

  // デストラクタ
  virtual
  ~SimRepeatLink();


private:

  // 繰り返しの開始処理
  void init(int count);

  // 実行する
  virtual
  void exec();

  // 繰り返しの判定処理
  void check();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 親のノード
  SimRepeatNode* mParent;

  // 現在の回数
  int mCurNum;

  // 繰り返し実行する本体へのリンク
  SimNode* mBody;

};

END_NAMESPACE_YM_VERILOG

#endif // LIBYM_VERILOG_SIMULATOR_SIMREPEATNODE_H
