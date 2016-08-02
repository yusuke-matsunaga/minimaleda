#ifndef LIBYM_VERILOG_SIMULATOR_SIMEVENTNODE_H
#define LIBYM_VERILOG_SIMULATOR_SIMEVENTNODE_H

// $Id: SimEventNode.h 232 2006-09-19 08:45:43Z matsunaga $

// イベント待ちを行うノード

#include "SimNode.h"


BEGIN_NAMESPACE_YM_VERILOG

class SimEventLink;

//////////////////////////////////////////////////////////////////////
// event 待ちのためのノード
//////////////////////////////////////////////////////////////////////
class SimEventNode :
  public SimNode
{
  friend class SimEngine;
protected:

  // コンストラクタ
  SimEventNode(SimEngine* engine,
	       const VpiScope* scope,
	       const list<VpiExpr*>& event_list);

  // デストラクタ
  virtual
  ~SimEventNode();

  // アクティブにする．
  void activate();

  // アクティブでなくす．
  void deactivate();
  

public:

  // アクティブなら true を返す．
  bool is_active() const;

  // イベントが起きたときの処理
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

  // Watch 対象のイベント条件のリスト
  list<SimEventLink*> mEventList;
  
  // 現在, アクティブかどうかを示すフラグ
  bool mActive;

};


//////////////////////////////////////////////////////////////////////
// event 待ちつきの nonblocking 代入文ノード
//////////////////////////////////////////////////////////////////////
class SimNbEventNode :
  public SimEventNode
{
  friend class SimEngine;
protected:

  // コンストラクタ
  SimNbEventNode(SimEngine* engine,
		 const VpiScope* scope,
		 const list<VpiExpr*>& event_list,
		 SimUpdateNode* update);
  
  // デストラクタ
  virtual
  ~SimNbEventNode();


public:
  
  // イベントが起きたときの処理
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
// イベント待ちで使われるクラス
//////////////////////////////////////////////////////////////////////
class SimEventLink
{
  friend class SimEventNode;
private:

  // コンストラクタ
  // index_list   : 多次元配列のインデックスリスト or 空リスト
  // expr         : 実際のイベント式
  SimEventLink(SimEventNode* parent,
	       const vector<VpiExpr*>& index_list,
	       VpiExpr* expr);

  // デストラクタ
  ~SimEventLink();

  // インデックスリストを返す．
  const vector<VpiExpr*>& index_list() const;


public:

  // 値の変化があったときに呼ばれる関数
  // 自分が待っていた変化ならば親の仮想関数 exec() を起動する．
  void prop_vc(const vector<VpiExpr*>& index_list,
	       const BitVector& old_value,
	       const BitVector& new_value) const;

  // 値の変化があったときに呼ばれる関数
  // 自分が待っていた変化ならば親の仮想関数 exec() を起動する．
  void prop_vc(const vector<VpiExpr*>& index_list,
	       double old_value,
	       double new_value) const;

  // 名前つきイベントが起ったときに呼ばれる関数
  // 自分が待っていたイベントならば親の仮想関数 exec() を起動する．
  void prop_nm(const vector<VpiExpr*>& index_list) const;
  
  // デバッグ用の表示関数
  ostream& put(ostream& s) const;


private:
  
  // index_list が等しいかチェックする．
  // 等しいときに true を返す．
  bool check_index_list(const vector<VpiExpr*>& index_list) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 親
  SimEventNode* mParent;

  // インデックスリスト
  vector<VpiExpr*> mIndexList;

  // イベント式
  VpiExpr* mExpr;

};

END_NAMESPACE_YM_VERILOG

#endif // LIBYM_VERILOG_SIMULATOR_SIMEVENTNODE_H
