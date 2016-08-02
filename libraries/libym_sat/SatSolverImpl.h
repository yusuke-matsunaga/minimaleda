#ifndef LIBYM_SAT_SATSOLVERIMPL_H
#define LIBYM_SAT_SATSOLVERIMPL_H

/// @file libym_sat/SatSolverImpl.h
/// @brief SatSolverImpl のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: SatSolverImpl.h 2274 2009-06-10 07:45:29Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_sat/SatSolver.h"
#include "ym_utils/Alloc.h"
#include "ym_utils/RandGen.h"
#include "ym_utils/StopWatch.h"
#include "SatClause.h"
#include "SatReason.h"
#include "AssignList.h"
#include "Watcher.h"


BEGIN_NAMESPACE_YM_SAT

class SatAnalyzer;

//////////////////////////////////////////////////////////////////////
/// @class Params SatSolverImpl.h "SatSolverImpl.h"
/// @brief SatSolverImpl の挙動を制御するパラメータ
//////////////////////////////////////////////////////////////////////
struct Params
{
  /// @brief 変数の decay 値
  double mVarDecay;

  /// @brief 変数選択用の閾値
  double mVarFreq;

  /// @brief 学習節の decay 値
  double mClauseDecay;
  
  /// @brief コンストラクタ
  Params() :
    mVarDecay(1.0),
    mVarFreq(0.0),
    mClauseDecay(1.0)
  {
  }

  /// @brief 値を指定したコンストラクタ
  Params(double var_decay,
	 double var_freq,
	 double clause_decay) :
    mVarDecay(var_decay),
    mVarFreq(var_freq),
    mClauseDecay(clause_decay)
  {
  }
  
};


//////////////////////////////////////////////////////////////////////
/// @class SatSolverImpl SatSolverImpl.h "SatSolverImpl.h"
/// @brief SatSolver の実装クラス
//////////////////////////////////////////////////////////////////////
class SatSolverImpl :
  public SatSolver
{
  friend class SatAnalyzer;
  
public:

  /// @brief コンストラクタ
  /// @param[in] analyzer 解析器のポインタ
  SatSolverImpl(SatAnalyzer* analyzer);

  /// @brief デストラクタ
  virtual
  ~SatSolverImpl();


public:
  //////////////////////////////////////////////////////////////////////
  // SatSolver で定義されている仮想関数
  //////////////////////////////////////////////////////////////////////
  
  /// @brief 正しい状態のときに true を返す．
  virtual
  bool
  sane() const;
  
  /// @brief 変数を追加する．
  /// @return 新しい変数番号を返す．
  /// @note 変数番号は 0 から始まる．
  virtual
  tVarId
  new_var();

  /// @brief 節を追加する．
  /// @param[in] lits リテラルのベクタ
  virtual
  void
  add_clause(const vector<Literal>& lits);

  /// @brief SAT 問題を解く．
  /// @param[in] assumptions あらかじめ仮定する変数の値割り当てリスト
  /// @param[out] model 充足するときの値の割り当てを格納する配列．
  /// @retval kB3True 充足した．
  /// @retval kB3False 充足不能が判明した．
  /// @retval kB3X わからなかった．
  /// @note i 番めの変数の割り当て結果は model[i] に入る．
  virtual
  Bool3
  solve(const vector<Literal>& assumptions,
	vector<Bool3>& model);

  /// @brief 現在の内部状態を得る．
  /// @param[out] stats 状態を格納する構造体
  virtual
  void
  get_stats(SatStats& stats) const;

  /// @brief 変数の数を得る．
  virtual
  ymuint
  variable_num() const;

  /// @brief 制約節の数を得る．
  virtual
  ymuint
  clause_num() const;

  /// @brief 制約節のリテラルの総数を得る．
  virtual
  ymuint
  literal_num() const;

  /// @brief conflict_limit の最大値
  /// @param[in] val 設定する値
  /// @return 以前の設定値を返す．
  virtual
  ymuint64
  set_max_conflict(ymuint64 val);

  /// @brief solve() 中のリスタートのたびに呼び出されるメッセージハンドラの登録
  /// @param[in] msg_handler 登録するメッセージハンドラ
  virtual
  void
  reg_msg_handler(SatMsgHandler* msg_handler);

  /// @brief 時間計測機能を制御する
  virtual
  void
  timer_on(bool enable);

  
private:
  //////////////////////////////////////////////////////////////////////
  // 実装用のプライベート関数
  //////////////////////////////////////////////////////////////////////

  // 探索を行う本体の関数
  Bool3
  search();

  // 割当てキューに基づいて implication を行う．
  SatReason
  implication();
  
  // level までバックトラックする
  void
  backtrack(int level);

  // 次の割り当てを選ぶ．
  // 割り当てられる変数がない場合には kLiteralX を返す．
  Literal
  next_decision();

  // 値の割当てか可能かチェックする．
  // 矛盾が起きたら false を返す．
  bool
  check_and_assign(Literal lit);

  // 値の割当てを行う．
  // 実際にはキューに積む．
  // 矛盾が起きたら false を返す．
  void
  assign(Literal lit,
	 SatReason reason = SatReason());

  // CNF を簡単化する．
  void
  simplifyDB();

  // 使われていない学習節を削除する．
  void
  reduceDB();

  // 学習節を追加する．
  void
  add_learnt_clause(const vector<Literal>& lits);

  // 新しい節を生成する．
  SatClause*
  new_clause(const vector<Literal>& lits,
	     bool learnt = false);

  // 節を削除する．
  void
  delete_clause(SatClause* clause);
    
  // watcher list を得る．
  WatcherList&
  watcher_list(Literal lit);
  
  // Watcher を追加する．
  void
  add_watcher(Literal watch_lit,
	      SatReason reason);
  
  // 変数の評価を行う．
  Bool3
  eval(tVarId id) const;
  
  // literal の評価を行う．
  Bool3
  eval(Literal l) const;

  // 現在の decision level を返す．
  int
  decision_level() const;

  // 変数の decision level を返す．
  int
  decision_level(tVarId varid) const;

  // 変数の割り当て理由を返す．
  SatReason
  reason(tVarId varid) const;
  
  // 学習節が使われているか調べる．
  bool
  is_locked(SatClause* clause) const;

  // 変数のアクティビティを増加させる．
  void
  bump_var_activity(tVarId var);

  // 変数のアクティビティを定率で減少させる．
  void
  decay_var_activity();

  // リスタート時の変数のアクティビティの低減率
  void
  decay_var_activity2();

  // 学習節のアクティビティを増加させる．
  void
  bump_clause_activity(SatClause* clause);

  // 学習節のアクティビティを定率で減少させる．
  void
  decay_clause_activity();
  
  // 実際に変数に関するデータ構造を生成する．
  void
  alloc_var();
  
  // 変数に関する配列を拡張する．
  void
  expand_var();
  
  
private:
  //////////////////////////////////////////////////////////////////////
  // ヒープ用の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 空にする．
  void
  heap_clear();

  /// @brief 要素が空の時 true を返す．
  bool
  heap_empty() const;
  
  /// @brief 変数を始めてヒープに追加する．
  /// @param[in] var 追加する変数
  void
  heap_add_var(tVarId var);
  
  /// @brief 変数を再びヒープに追加する．
  /// @param[in] var 追加する変数
  void
  heap_push(tVarId var);
  
  /// @brief アクティビティ最大の変数を取り出す．
  /// @note 該当の変数はヒープから取り除かれる．
  tVarId
  heap_pop_top();
  
  /// @brief 引数の位置にある要素を適当な位置まで沈めてゆく
  void
  heap_move_down(ymuint pos);

  /// @brief 引数の位置にある要素を適当な位置まで上げてゆく
  void
  heap_move_up(tVarId var);

  /// @brief 変数を配列にセットする．
  /// @note mHeap と mHeapPos の一貫性を保つためにはこの関数を使うこと．
  void
  heap_set(tVarId var,
	   ymuint pos);
  
  /// @brief 左の子供の位置を計算する
  static
  ymuint
  heap_left(ymuint pos);

  /// @brief 右の子供の位置を計算する．
  static
  ymuint
  heap_right(ymuint pos);

  /// @brief 親の位置を計算する．
  static
  ymuint
  heap_parent(ymuint pos);
  
  /// @brief 内容を出力する
  void
  heap_dump(ostream& s) const;

  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 解析器
  SatAnalyzer* mAnalyzer;

  // 正常の時に true となっているフラグ
  bool mSane;

  // SatClause のメモリ領域確保用のアロケータ
  SimpleAlloc mAlloc;

  // Watcher 用のアロケータ
  UnitAlloc mWatcherAlloc;

  // 制約節の配列
  vector<SatClause*> mConstrClause;

  // 学習節の配列
  vector<SatClause*> mLearntClause;
  
  // 変数の数
  ymuint32 mVarNum;
  
  // 前回の alloc_var で処理した時の変数の数
  ymuint32 mOldVarNum;
  
  // 変数関係の配列のサイズ
  ymuint32 mVarSize;
  
  // 値の配列
  Bool3* mVal;

  // 値が割り当てられたときのレベルの配列
  int* mDecisionLevel;

  // 値が割り当てられた理由の配列
  SatReason* mReason;

  // ヒープ上の位置の配列
  ymint32* mHeapPos;

  // アクティビティ
  double* mActivity;

  // watcher list
  WatcherList* mWatcherList;
  
  // ヒープ用の配列
  // サイズは mVarSize
  ymuint32* mHeap;

  // ヒープの要素数
  ymuint32 mHeapNum;
  
  // 矛盾の解析時にテンポラリに使用される節
  SatClause* mTmpBinClause;
  
  // search 開始時の decision level
  int mRootLevel;
  
  // 値割り当てを保持するリスト
  AssignList mAssignList;
      
  // 変数のアクティビティの増加量
  double mVarBump;

  // 変数のアクティビティの減衰量
  double mVarDecay;
  
  // 学習節のアクティビティの増加量
  double mClauseBump;

  // 学習節のアクティビティの減衰量
  double mClauseDecay;

  // 変数選択用乱数発生器
  RandGen mRandGen;

  // mTimer を使うとき true にするフラグ
  bool mTimerOn;

  // 時間計測器
  StopWatch mTimer;

  // 制御用パラメータ
  Params mParams;

  // restart 数
  ymuint mRestart;
  
  // 総コンフリクト数
  ymuint64 mConflictNum;

  // 総 decision 数
  ymuint64 mDecisionNum;

  // 総 implication 数
  ymuint64 mPropagationNum;

  // 二項制約節の数
  ymuint64 mConstrBinNum;
  
  // 制約節の総リテラル数
  ymuint64 mConstrLitNum;

  // 二項学習節の数
  ymuint64 mLearntBinNum;
  
  // 学習節の総リテラル数
  ymuint64 mLearntLitNum;

  // コンフリクト数の制限
  ymuint64 mConflictLimit;

  // 学習節の制限
  ymuint64 mLearntLimit;
  
  // mConflistLimit の最大値
  ymuint64 mMaxConflict;
  
  // メッセージハンドラのリスト
  list<SatMsgHandler*> mMsgHandlerList;
  
  // add_clause 用の作業領域
  vector<Literal> mAcTmp;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 変数の数を得る．
inline
ymuint
SatSolverImpl::variable_num() const
{
  return mVarNum;
}

// @brief 制約節の数を得る．
inline
ymuint
SatSolverImpl::clause_num() const
{
  return mConstrClause.size() + mConstrBinNum;
}

// @brief 制約節のリテラルの総数を得る．
inline
ymuint
SatSolverImpl::literal_num() const
{
  return mConstrLitNum;
}

// watcher list を得る．
inline
WatcherList&
SatSolverImpl::watcher_list(Literal lit)
{
  ymuint index = lit.index();
  return mWatcherList[index];
}

// 変数の評価を行う．
inline
Bool3
SatSolverImpl::eval(tVarId id) const
{
  return mVal[id];
}

// literal の評価を行う．
inline
Bool3
SatSolverImpl::eval(Literal l) const
{
  ymuint index = l.index();
  Bool3 val = eval(index / 2);
  int d = 1 - (index & 1) * 2;
  return static_cast<Bool3>(static_cast<int>(val) * d);
}

// 値の割当てか可能かチェックする．
// 矛盾が起きたら false を返す．
inline
bool
SatSolverImpl::check_and_assign(Literal lit)
{
  Bool3 old_val = eval(lit);
  if ( old_val != kB3X ) {
    return old_val == kB3True;
  }
  assign(lit);
  return true;
}

// 値の割当てを行う．
inline
void
SatSolverImpl::assign(Literal lit,
		      SatReason reason)
{
  tVarId varid = lit.varid();
  mVal[varid] = static_cast<Bool3>(1 - static_cast<int>(lit.pol()) * 2);
  mDecisionLevel[varid] = decision_level();
  mReason[varid] = reason;

  // mAssignList に格納する．
  mAssignList.put(lit);
}

// 現在の decision level を返す．
inline
int
SatSolverImpl::decision_level() const
{
  return mAssignList.cur_level();
}

// 変数の decision level を返す．
inline
int
SatSolverImpl::decision_level(tVarId varid) const
{
  return mDecisionLevel[varid];
}

// 変数の割り当て理由を返す．
inline
SatReason
SatSolverImpl::reason(tVarId varid) const
{
  return mReason[varid];
}

// @brief clase が含意の理由になっているか調べる．
inline
bool
SatSolverImpl::is_locked(SatClause* clause) const
{
  // 直感的には分かりにくいが，SatClause の最初のリテラルは
  // 残りのリテラルによって含意されていることになっている．
  // そこで最初のリテラルの変数の割り当て理由が自分自身か
  // どうかを調べれば clause が割り当て理由として用いられて
  // いるかわかる．
  return reason(clause->lit0().varid()) == SatReason(clause);
}

// @brief 時間計測機能を制御する
inline
void
SatSolverImpl::timer_on(bool enable)
{
  mTimerOn = enable;
}

// 実際に変数に関するデータ構造を生成する．
inline
void
SatSolverImpl::alloc_var()
{
  if ( mOldVarNum < mVarNum ) {
    if ( mVarSize < mVarNum ) {
      expand_var();
    }
    for (ymuint i = mOldVarNum; i < mVarNum; ++ i) {
      mVal[i] = kB3X;
      mActivity[i] = 0.0;
      heap_add_var(i);
    }
    mOldVarNum = mVarNum;
  }
}

// @brief 空にする．
inline
void
SatSolverImpl::heap_clear()
{
  mHeapNum = 0;
}
  
// @brief 要素が空の時 true を返す．
inline
bool
SatSolverImpl::heap_empty() const
{
  return mHeapNum == 0;
}
  
// @brief 変数を始めてヒープに追加する．
// @param[in] var 追加する変数
inline
void
SatSolverImpl::heap_add_var(tVarId var)
{
  heap_set(var, mHeapNum);
  ++ mHeapNum;
}

// @brief 要素を追加する．
inline
void
SatSolverImpl::heap_push(tVarId var)
{
  if ( mHeapPos[var] == -1 ) {
    ymuint pos = mHeapNum;
    ++ mHeapNum;
    heap_set(var, pos);
    heap_move_up(pos);
  }
}

// @brief もっともアクティビティの高い変数を返す．
inline
tVarId
SatSolverImpl::heap_pop_top()
{
  // この assert は重いのでコメントアウトしておく
  //assert_cond(mHeapNum > 0, __FILE__, __LINE__);
  tVarId ans = mHeap[0];
  mHeapPos[ans] = -1;
  -- mHeapNum;
  if ( mHeapNum > 0 ) {
    tVarId var = mHeap[mHeapNum];
    heap_set(var, 0);
    heap_move_down(0);
  }
  return ans;
}

// 引数の位置にある要素を適当な位置まで上げてゆく
inline
void
SatSolverImpl::heap_move_up(ymuint pos)
{
  tVarId var = mHeap[pos];
  double val = mActivity[var];
  while ( pos > 0 ) {
    ymuint pos_p = heap_parent(pos);
    tVarId var_p = mHeap[pos_p];
    double val_p = mActivity[var_p];
    if ( val_p >= val ) {
      break;
    }
    heap_set(var, pos_p);
    heap_set(var_p, pos);
    pos = pos_p;
  }
}

// 変数を配列にセットする．
inline
void
SatSolverImpl::heap_set(tVarId var,
			ymuint pos)
{
  mHeap[pos] = var;
  mHeapPos[var] = pos;
}
  
// @brief 左の子供の位置を計算する
inline
ymuint
SatSolverImpl::heap_left(ymuint pos)
{
  return pos + pos + 1;
}

// @brief 右の子供の位置を計算する．
inline
ymuint
SatSolverImpl::heap_right(ymuint pos)
{
  return pos + pos + 2;
}

// @brief 親の位置を計算する．
inline
ymuint
SatSolverImpl::heap_parent(ymuint pos)
{
  return (pos - 1) >> 1;
}

END_NAMESPACE_YM_SAT

#endif // LIBYM_SAT_SATSOLVERIMPL_H
