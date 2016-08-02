// $Id: SimEventLink.cc 700 2007-05-31 00:41:30Z matsunaga $

// イベント待ちのためのノード

#if HAVE_CONFIG_H
#include <ymconfig.h>
#endif

#include "SimEventNode.h"

#include <ym_verilog/vpi/VpiExpr.h>
#include "SimEngine.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// event 待ちのためのノード
//////////////////////////////////////////////////////////////////////

// コンストラクタ
SimEventNode::SimEventNode(SimEngine* engine,
			   const VpiScope* scope,
			   const list<VpiExpr*>& event_list) :
  SimNode(engine, scope)
{
  mActive = false;

#if 0 // VPI
  // event_list を解析して実際のリンクを張る．
  for (list<VpiExpr*>::const_iterator p = event_list.begin();
       p != event_list.end(); ++ p) {
    VpiExpr* expr = *p;
    VpiArray* array;
    vector<VpiExpr*> index_list;
    VpiExpr* primary;
    if ( !expr->event_decompose(array, index_list, primary) ) {
      // エラー
      continue;
    }
    SimEventLink* event = new SimEventLink(this, index_list, primary);
    array->add_event(event);
    mEventList.push_back(event);
  }
#endif
}

// デストラクタ
SimEventNode::~SimEventNode()
{
  for (list<SimEventLink*>::iterator p = mEventList.begin();
       p != mEventList.end(); ++ p) {
    delete *p;
  }
}

// アクティブにする．
void
SimEventNode::activate()
{
  mActive = true;
}

// アクティブでなくす．
void
SimEventNode::deactivate()
{
  mActive = false;
}

// アクティブなら true を返す．
bool
SimEventNode::is_active() const
{
  return mActive;
}

// 実行する
void
SimEventNode::exec()
{
  activate();
}

// イベントが起きたときの処理
void
SimEventNode::emit()
{
  deactivate();

  //schedule_next();
}


//////////////////////////////////////////////////////////////////////
// event 待ちの nonblocking 代入文ノード
//////////////////////////////////////////////////////////////////////

// コンストラクタ
SimNbEventNode::SimNbEventNode(SimEngine* engine,
			       const VpiScope* scope,
			       const list<VpiExpr*>& event_list,
			       SimUpdateNode* update) :
  SimEventNode(engine, scope, event_list),
  mUpdate(update)
{
}

// デストラクタ
SimNbEventNode::~SimNbEventNode()
{
}

// 実行する
void
SimNbEventNode::exec()
{
  activate();

  //schedule_next();
}

// イベントが起きたときの処理
void
SimNbEventNode::emit()
{
  deactivate();

  assert_cond(mUpdate, __FILE__, __LINE__);
  engine()->reg_nonblocking_event(mUpdate);
}


//////////////////////////////////////////////////////////////////////
// イベントの種類を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
// index_list   : 多次元配列のインデックスリスト or 空リスト
// expr         : 実際のイベント式
SimEventLink::SimEventLink(SimEventNode* parent,
			   const vector<VpiExpr*>& index_list,
			   VpiExpr* expr) :
  mParent(parent),
  mIndexList(index_list),
  mExpr(expr)
{
}

// デストラクタ
SimEventLink::~SimEventLink()
{
}

// インデックスリストを返す．
const vector<VpiExpr*>&
SimEventLink::index_list() const
{
  return mIndexList;
}

// 値の変化があったときに呼ばれる関数
// 自分が待っていた変化ならば親の仮想関数 exec() を起動する．
void
SimEventLink::prop_vc(const vector<VpiExpr*>& index_list,
		      const BitVector& old_value,
		      const BitVector& new_value) const
{
#if 0 // VPI
  if ( mParent->is_active() &&
       check_index_list(index_list) &&
       mExpr->event_check(old_value, new_value, 0) ) {
    mParent->emit();
  }
#endif
}

// 値の変化があったときに呼ばれる関数
// 自分が待っていた変化ならば親の仮想関数 exec() を起動する．
void
SimEventLink::prop_vc(const vector<VpiExpr*>& index_list,
		      double old_value,
		      double new_value) const
{
#if 0 // VPI
  if ( mParent->is_active() &&
       check_index_list(index_list) &&
       mExpr->event_check(old_value, new_value, 0) ) {
    mParent->emit();
  }
#endif
}

// 名前つきイベントが起ったときに呼ばれる関数
// 自分が待っていたイベントならば親の仮想関数 exec() を起動する．
void
SimEventLink::prop_nm(const vector<VpiExpr*>& index_list) const
{
  if ( mParent->is_active() &&
       check_index_list(index_list) ) {
    mParent->emit();
  }
}

// index_list が等しいかチェックする．
// 等しいときに true を返す．
bool
SimEventLink::check_index_list(const vector<VpiExpr*>& index_list) const
{
  if ( index_list.size() != mIndexList.size() ) {
    return false;
  }
  vector<VpiExpr*>::const_iterator p = index_list.begin();
  vector<VpiExpr*>::const_iterator q = mIndexList.begin();
  while ( p != index_list.end() ) {
    VpiExpr* expr1 = *p;
    VpiExpr* expr2 = *q;
    if ( expr1->eval_int() != expr2->eval_int() ) {
      return false;
    }
    ++ p;
    ++ q;
  }
  return true;
}
		  
// デバッグ用の表示関数
ostream&
SimEventLink::put(ostream& s) const
{
  for (vector<VpiExpr*>::const_iterator p = mIndexList.begin();
       p != mIndexList.end(); ++ p) {
    s << "[" << *p << "]";
  }
  s << mExpr;
 
  return s;
}

END_NAMESPACE_YM_VERILOG
