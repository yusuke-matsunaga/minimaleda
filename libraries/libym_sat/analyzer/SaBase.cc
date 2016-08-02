
/// @file libym_sat/SaBase.cc
/// @brief SaBase の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: SaBase.cc 2203 2009-04-16 05:04:40Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "SaBase.h"
#include "SatSolverImpl.h"
#include "SatClause.h"


BEGIN_NAMESPACE_YM_SAT

// @brief コンストラクタ
SaBase::SaBase()
{
}

// @brief デストラクタ
SaBase::~SaBase()
{
}

// @brief 新しい変数が追加されたときに呼ばれる仮想関数
void
SaBase::alloc_var(ymuint size)
{
  mMark.resize(size, false);
}

// 再帰的なチェックを行う簡単化
// learnt に含まれるリテラルのうち，その理由となっている割り当て，
// もしくはそのまた理由となっている割り当てを再帰的にたどり，
// そのなかに一つでもマークの付いていない決定割り当て(理由をもたない割り当て)
// があればそのリテラルを残す．
// 要するに，矛盾の起きた割り当てに対する極小セパレーター集合を求めている．
void
SaBase::simplify(vector<Literal>& learnt)
{
  size_t nl = learnt.size();

  // learnt に含まれているリテラルのレベルのビットマップ
  // ただし 64 のモジュロをとっている．
  ymuint64 lmask = 0UL;
  for (size_t i = 0; i < nl; ++ i) {
    Literal p = learnt[i];
    int level = decision_level(p.varid());
    lmask |= (1UL << (level & 63));
  }

  size_t wpos = 0;
  for (size_t i = 0; i < nl; ++ i) {
    Literal p = learnt[i];
    tVarId var = p.varid();
    mClearQueue2.clear();
    if ( check_recur(var, lmask) ) {
      if ( wpos != i ) {
	learnt[wpos] = p;
      }
      ++ wpos;
    }
    for (vector<tVarId>::iterator p = mClearQueue2.begin();
	 p != mClearQueue2.end(); ++ p) {
      tVarId var = *p;
      set_mark(var, false);
    }
  }
  if ( wpos < nl ) {
    learnt.erase(learnt.begin() + wpos, learnt.end());
  }
}

// simplify_recur のサブルーティン
// varid が極小セパレータの要素であるとき true を返す．
// 探索でたどったノードにはマークを付け，mClearQueue2 に積む．
// マークの付いたノードは2度と探索する必要はない．
// 以前の探索の結果が true ならその場で再帰関数は終わるので2度と
// たどることはないし，以前の結果が false ならそのままでよい．
bool
SaBase::check_recur(tVarId varid,
		    ymuint64 lmask)
{
  SatReason r = reason(varid);
  if ( r == kNullSatReason ) {
    // varid は決定ノードだった．
    return true;
  }

  if ( ((1UL << (decision_level(varid) & 63)) & lmask) == 0UL ) {
    // varid の割り当てレベルと同じレベルの割り当ては learnt に含まれていない．
    // ということはこのまま再帰を続けると varid と同じレベルの決定ノード
    // に到達可能であることがわかる．
    return true;
  }

  if ( r.is_clause() ) {
    SatClause& clause = r.clause();
    size_t n = clause.size();
    for (size_t i = 1; i < n; ++ i) {
      Literal q = clause.lit(i);
      tVarId var1 = q.varid();
      if ( !get_mark(var1) && decision_level(var1) > 0 ) {
	set_mark(var1, true);
	mClearQueue2.push_back(var1);
	if ( check_recur(var1, lmask) ) {
	  return true;
	}
      }
    }
  }
  else {
    Literal q = r.literal();
    tVarId var1 = q.varid();
    if ( !get_mark(var1) && decision_level(var1) > 0 ) {
      set_mark(var1, true);
      mClearQueue2.push_back(var1);
      if ( check_recur(var1, lmask) ) {
	return true;
      }
    }
  }
  return false;
}

// decision level の高いリテラルを 2番め (learnt[1] の位置) に持ってくる
// 2番めのリテラルのレベルを返す．
int
SaBase::reorder(vector<Literal>& learnt)
{
  size_t n = learnt.size();
  if ( n < 2 ) {
    return 0;
  }
  Literal lit1 = learnt[1];
  int level = decision_level(lit1.varid());
  size_t pos = 1;
  for (size_t i = 2; i < n; ++ i) {
    Literal lit2 = learnt[i];
    int level2 = decision_level(lit2.varid());
    if ( level < level2 ) {
      level = level2;
      pos = i;
    }
  }
  if ( pos != 1 ) {
    learnt[1] = learnt[pos];
    learnt[pos] = lit1;
  }
  return level;
}

// var->mMark を設定してキューに積む
void
SaBase::set_mark_and_putq(tVarId var)
{
  set_mark(var, true);
  mClearQueue.push_back(var);
}

// mClearQueue につまれた変数のマークを消す．
void
SaBase::clear_marks()
{
  // var->mMark をクリアする．
  for (vector<tVarId>::iterator p = mClearQueue.begin();
       p != mClearQueue.end(); ++ p) {
    tVarId var = *p;
    set_mark(var, false);
  }
  mClearQueue.clear();
}

END_NAMESPACE_YM_SAT
