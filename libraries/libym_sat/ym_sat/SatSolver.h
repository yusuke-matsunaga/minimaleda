#ifndef YM_SAT_SATSOLVER_H
#define YM_SAT_SATSOLVER_H

/// @file ym_sat/SatSolver.h
/// @brief SatSolver のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: SatSolver.h 2203 2009-04-16 05:04:40Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_sat/sat_nsdef.h"
#include "ym_sat/Bool3.h"
#include "ym_lexp/Literal.h"
#include "ym_utils/StopWatch.h"


BEGIN_NAMESPACE_YM_SAT

//////////////////////////////////////////////////////////////////////
/// @class SatStats SatSolver.h <ym_sat/SatSolver.h>
/// @ingroup SatGroup
/// @brief SAT ソルバの状態を表すクラス
/// @sa SatSolver
//////////////////////////////////////////////////////////////////////
struct SatStats
{
  /// @brief restartの回数
  ymuint mRestart;

  /// @brief 変数の数．
  ymuint mVarNum;

  /// @brief 制約節の数
  ymuint64 mConstrClauseNum;

  /// @brief 制約節のリテラル数
  ymuint64 mConstrLitNum;

  /// @brief 学習節の数
  ymuint64 mLearntClauseNum;

  /// @brief 学習節のリテラル数
  ymuint64 mLearntLitNum;

  /// @brief コンフリクト数
  ymuint64 mConflictNum;

  /// @brief decision 数
  ymuint64 mDecisionNum;

  /// @brief implication数
  ymuint64 mPropagationNum;

  /// @brief コンフリクト数の制限
  ymuint64 mConflictLimit;

  /// @brief 学習節の制限
  ymuint64 mLearntLimit;

  /// @brief 計算時間
  USTime mTime;

};


//////////////////////////////////////////////////////////////////////
/// @class SatMsgHandler SatSolver.h <ym_sat/SatSolver.h>
/// @ingroup SatGroup
/// @brief SAT ソルバの内部状態を出力するためのメッセージハンドラ
/// @sa SatSolver
//////////////////////////////////////////////////////////////////////
class SatMsgHandler
{
protected:

  /// @brief デストラクタ
  virtual
  ~SatMsgHandler() { }


public:

  /// @brief 内部状態の出力を行う仮想関数
  /// @param[in] stats SAT ソルバの内部状態
  virtual
  void
  operator()(const SatStats& stats) = 0;

};


//////////////////////////////////////////////////////////////////////
/// @class SatSolver SatSolver.h <ym_sat/SatSolver.h>
/// @ingroup SatGroup
/// @brief CNF-SAT エンジンのインターフェイスを表すクラス
//////////////////////////////////////////////////////////////////////
class SatSolver
{
public:
  
  /// @brief デストラクタ
  virtual
  ~SatSolver() { }


public:
  
  /// @brief 正しい状態のときに true を返す．
  virtual
  bool
  sane() const = 0;
  
  /// @brief 変数を追加する．
  /// @return 新しい変数番号を返す．
  /// @note 変数番号は 0 から始まる．
  virtual
  tVarId
  new_var() = 0;

  /// @brief 節を追加する．
  /// @param[in] lits リテラルのベクタ
  virtual
  void
  add_clause(const vector<Literal>& lits) = 0;

  /// @brief 1項の節(リテラル)を追加する．
  void
  add_clause(Literal lit1);

  /// @brief 2項の節を追加する．
  void
  add_clause(Literal lit1,
	     Literal lit2);

  /// @brief 3項の節を追加する．
  void
  add_clause(Literal lit1,
	     Literal lit2,
	     Literal lit3);
  
  /// @brief SAT 問題を解く．
  /// @param[out] model 充足するときの値の割り当てを格納する配列．
  /// @retval kB3True  充足した．
  /// @retval kB3False 充足不能が判明した．
  /// @retval kB3X     わからなかった．
  /// @note i 番めの変数の割り当て結果は model[i] に入る．
  /// 通常は complete な割り当てなので値は true/false だけになるはず
  Bool3
  solve(vector<Bool3>& model);

  /// @brief assumption 付きの SAT 問題を解く．
  /// @param[in] assumptions あらかじめ仮定する変数の値割り当てリスト
  /// @param[out] model 充足するときの値の割り当てを格納する配列．
  /// @retval kSat 充足した．
  /// @retval kUnsat 充足不能が判明した．
  /// @retval kUndet わからなかった．
  /// @note i 番めの変数の割り当て結果は model[i] に入る．
  virtual
  Bool3
  solve(const vector<Literal>& assumptions,
	vector<Bool3>& model) = 0;
  
  /// @brief 現在の内部状態を得る．
  /// @param[out] stats 状態を格納する構造体
  virtual
  void
  get_stats(SatStats& stats) const = 0;

  /// @brief 変数の数を得る．
  virtual
  ymuint
  variable_num() const = 0;

  /// @brief 制約節の数を得る．
  virtual
  ymuint
  clause_num() const = 0;

  /// @brief 制約節のリテラルの総数を得る．
  virtual
  ymuint
  literal_num() const = 0;

  /// @brief conflict_limit の最大値
  /// @param[in] val 設定する値
  /// @return 以前の設定値を返す．
  virtual
  ymuint64
  set_max_conflict(ymuint64 val) = 0;
  
  /// @brief solve() 中のリスタートのたびに呼び出されるメッセージハンドラの登録
  /// @param[in] msg_handler 登録するメッセージハンドラ
  virtual
  void
  reg_msg_handler(SatMsgHandler* msg_handler) = 0;

  /// @brief 時間計測機能を制御する
  virtual
  void
  timer_on(bool enable) = 0;

};


//////////////////////////////////////////////////////////////////////
/// @class SatSolverFactory SatSolver.h <ym_sat/SatSolver.h>
/// @ingroup SatGroup
/// @brief SatSolver のインスタンスを生成するクラス
/// @sa SatSolver
//////////////////////////////////////////////////////////////////////
class SatSolverFactory
{
public:

  /// @brief 通常の SatSolver を生成する．
  /// @param[in] option ソルバ生成時に渡すオプション文字列
  static
  SatSolver*
  gen_solver(const string& option = string());

  /// @brief ログ記録機能付き SatSolver を生成する．
  /// @param[in] out ログの出力先のストリーム
  /// @param[in] option ソルバ生成時に渡すオプション文字列
  static
  SatSolver*
  gen_recsolver(ostream& out,
		const string& option = string());
  
  /// @brief MiniSat 版 SatSolver を生成する．
  static
  SatSolver*
  gen_minisat();

};

END_NAMESPACE_YM_SAT

#endif // YM_SAT_SATSOLVER_H
