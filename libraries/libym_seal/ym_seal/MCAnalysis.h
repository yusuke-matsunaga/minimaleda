#ifndef YM_SEAL_MCANALYSIS_H
#define YM_SEAL_MCANALYSIS_H

/// @file ym_seal/MCAnalysis.h
/// @brief MCAnalysis のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: MCAnalysis.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_seal/seal_nsdef.h"
#include "ym_bnet/bnet_nsdef.h"


BEGIN_NAMESPACE_YM_SEAL

typedef string State;

typedef string StatePair;


/// @brief 遷移確率を表す構造体
struct TransProb
{
  TransProb(ymuint next_state,
	    double prob) :
    mNextState(next_state),
    mProb(prob)
  {
  }
  
  /// @brief 次状態番号
  ymuint mNextState;

  /// @brief 遷移確率
  double mProb;

};


//////////////////////////////////////////////////////////////////////
// 順序回路のソフトエラーレートの解析を行うクラス
//////////////////////////////////////////////////////////////////////
class MCAnalysis
{
public:

  /// @brief コンストラクタ
  MCAnalysis();

  /// @brief デストラクタ
  ~MCAnalysis();
  

public:

  /// @brief 解析を行う．
  /// @param[in] network 対象のネットワーク
  /// @param[in] init_states 初期状態の集合
  void
  analyze(const BNetwork& network,
	  const vector<State>& init_states,
	  bool use_bdd = true,
	  bool dump_trans = false);

  /// @brief 解析を行う．
  void
  analyze2(istream& s);

  
private:

  /// @brief 与えられた回路の到達可能状態および遷移確率を求める．
  void
  enum_states(const BNetwork& network,
	      const vector<State>& init_states);
  
  /// @brief 正常回路の定常状態確率を求める．
  /// @note 結果は mSteadyProb に格納される．
  void
  calc_steady_prob();
  
  /// @brief エラー状態の初期確率を求める．
  /// @param[in] error_rate 各フリップフロップのエラー確率
  /// @note 結果は mInitialProb に格納される．
  void
  calc_error_prob(const vector<double>& error_rate);
  
  /// @brief エラー状態の吸収確率を求める．
  /// @note 結果は mFailureProb に格納される．
  void
  calc_failure_prob();

  /// @brief 回路対の状態遷移確率行列をダンプする．
  void
  dump_trans(ostream& s);

  /// @brief 回路対の状態遷移確率行列を読み込む．
  void
  restore_trans(istream& s);
  

private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // BDD を使うかどうかを制御するフラグ
  bool mUseBdd;

  // FF 数
  ymuint32 mFFNum;
  
  // 正常回路の到達可能状態の集合
  vector<State> mReachableStates1;
  
  // 正常回路の遷移確率を保持するハッシュ表
  // キーは '現状態番号' * 状態数 + '次状態番号'
  //hash_map<ymuint, double> mTransProb1;
  vector<list<TransProb> > mTransProb1;

  // 正常回路の各状態の定常状態における存在確率
  vector<double> mSteadyProb;
  
  // 回路対の一時状態の集合
  vector<State> mReachableStates2;
  
  // 回路対の遷移確率を保持するハッシュ表
  // キーは '現状態番号' * 状態数 + '次状態番号'
  //hash_map<ymuint, double> mTransProb2;
  vector<list<TransProb> > mTransProb2;

  // 回路対の一時状態から failure 状態への遷移確率
  vector<double> mFailureProb0;
  
  // 回路対の各状態の初期存在確率
  hash_map<State, double> mInitialProb;
  
  // 回路対の各状態の failure 吸収確率
  vector<double> mFailureProb;
  
};

END_NAMESPACE_YM_SEAL

#endif // YM_SEAL_MCANALYSIS_H
