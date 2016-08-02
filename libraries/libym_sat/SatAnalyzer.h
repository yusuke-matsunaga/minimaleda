#ifndef LIBYM_SAT_SATANALYZER_H
#define LIBYM_SAT_SATANALYZER_H

/// @file libym_sat/SatAnalyzer.h
/// @brief SatAnalyzer のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: SatAnalyzer.h 2203 2009-04-16 05:04:40Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_sat/SatSolver.h"
#include "SatSolverImpl.h"


BEGIN_NAMESPACE_YM_SAT

class SatClause;

//////////////////////////////////////////////////////////////////////
/// @class SatAnalyzer SatAnalyzer.h "SatAnalyzer.h"
/// @brief 矛盾の解析/学習を行うクラス
///
/// このクラスの役割は矛盾の原因となった節を解析して，矛盾の解消に必要
/// な学習節(のためのリテラル集合)を生成することである．
/// ただし，学習節の生成法は唯一ではないので，SatAnalyzer を純粋仮想
/// 基底クラスにして派生クラスでさまざまな手法を実装できるようにしてい
/// る．
/// そのため，SatAnalyzer の大きな役割は SatSolverImpl とのインターフェ
/// イスを提供することである．もう一つの仕事は，派生クラスが
/// SatSolverImpl の private メンバ関数にアクセスするための代理関数を
/// 提供することである．
//////////////////////////////////////////////////////////////////////
class SatAnalyzer
{
  friend class SatSolverImpl;
  
protected:

  /// @brief デストラクタ
  virtual
  ~SatAnalyzer() { }


public:

  /// @brief solver をセットする．
  /// @param[in] solver SatSolverImpl のポインタ
  void
  set_solver(SatSolverImpl* solver);
  
  /// @brief 解析を行う．
  /// @param[in] creason 矛盾の原因
  /// @param[out] learnt 学習された節を表すリテラルのベクタ
  /// @return バックトラックレベル
  virtual
  int
  analyze(SatReason creason,
	  vector<Literal>& learnt) = 0;
  
  /// @brief 新しい変数が追加されたときに呼ばれる仮想関数
  virtual
  void
  alloc_var(ymuint size) = 0;


protected:
  //////////////////////////////////////////////////////////////////////
  // 派生クラスに直接 SatSolverImpl をアクセスさせないための代理関数
  //////////////////////////////////////////////////////////////////////

  // 現在の decision level を取り出す．
  int
  decision_level() const;

  // 割り当てリストの末尾を得る．
  size_t
  last_assign();

  // 割り当てリストの pos 番めの要素を得る．
  Literal
  get_assign(size_t pos);
  
  // 変数の decision level を得る．
  int
  decision_level(tVarId varid) const;

  // 変数の割り当て理由を得る．
  SatReason
  reason(tVarId varid) const;

  // 変数のアクティビティを増加させる．
  void
  bump_var_activity(tVarId varid);
  
  // 節のアクティビティを上げる．
  void
  bump_clause_activity(SatClause* clause);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // SatSolverImpl へのポインタ
  SatSolverImpl* mSolver;

};


//////////////////////////////////////////////////////////////////////
/// @class SaFactory SatAnalyzer.h "SatAnalyzer.h"
/// @brief SatAnalyzer(の派生クラス)を生成するファクトリ
//////////////////////////////////////////////////////////////////////
class SaFactory
{
public:
  
  /// @brief SatAnalyzerの派生クラスを生成する．
  /// @param[in] option どのクラスを生成するかを決めるオプション文字列
  static
  SatAnalyzer*
  gen_analyzer(const string& option = string());
  
};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief solver をセットする．
// @param[in] solver SatSolverImpl のポインタ
inline
void
SatAnalyzer::set_solver(SatSolverImpl* solver)
{
  mSolver = solver;
}

// 現在の decision level を取り出す．
inline
int
SatAnalyzer::decision_level() const
{
  return mSolver->decision_level();
}

// 割り当てリストの末尾を得る．
inline
size_t
SatAnalyzer::last_assign()
{
  return mSolver->mAssignList.size() - 1;
}

// 割り当てリストの pos 番めの要素を得る．
inline
Literal
SatAnalyzer::get_assign(size_t pos)
{
  return mSolver->mAssignList.get(pos);
}

// 変数の decision level を得る．
inline
int
SatAnalyzer::decision_level(tVarId varid) const
{
  return mSolver->decision_level(varid);
}

// 変数の割り当て理由を得る．
inline
SatReason
SatAnalyzer::reason(tVarId varid) const
{
  return mSolver->reason(varid);
}

// 変数のアクティビティを増加させる．
inline
void
SatAnalyzer::bump_var_activity(tVarId var)
{
  mSolver->bump_var_activity(var);
}

// 学習節のアクティビティを増加させる．
inline
void
SatAnalyzer::bump_clause_activity(SatClause* clause)
{
  mSolver->bump_clause_activity(clause);
}

END_NAMESPACE_YM_SAT

#endif // LIBYM_SAT_SATANALYZER_H
