#ifndef LIBYM_SEAL_BDDFSM_H
#define LIBYM_SEAL_BDDFSM_H

/// @file libym_seal/BddFsm.h
/// @brief BddFsm のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: BddFsm.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_seal/seal_nsdef.h"
#include "ym_bdd/Bdd.h"
#include "ym_bdd/BddVarSet.h"
#include "ym_seal/MCAnalysis.h"


BEGIN_NAMESPACE_YM_SEAL

typedef string State;
typedef string StatePair;

//////////////////////////////////////////////////////////////////////
/// @class BddFsm BddFsm.h "BddFsm.h"
/// @brief BDD で「有限状態機械」を表すクラス
//////////////////////////////////////////////////////////////////////
class BddFsm
{
public:

  /// @brief コンストラクタ
  /// @param[in] bdd_mgr BDD マネージャ
  /// @param[in] input_vars 入力変数番号の配列
  /// @param[in] state_vars 状態変数番号の配列(現状態と次状態のペア)
  /// @param[in] trans_relation 状態遷移関係
  BddFsm(BddMgrRef bdd_mgr,
	 const vector<ymuint>& input_vars,
	 const vector<pair<ymuint, ymuint> >& state_vars,
	 const Bdd& trans_relation);

  /// @brief デストラクタ
  ~BddFsm();


public:

  /// @brief 入力のビット数を返す．
  ymuint
  input_num() const;

  /// @brief 状態のビット数を返す．
  ymuint
  ff_num() const;
  
  /// @brief 入力の変数番号を返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < input_num() )
  ymuint
  input_varid(ymuint pos) const;

  /// @brief 現状態の変数番号を返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < ff_num() )
  ymuint
  cur_varid(ymuint pos) const;
  
  /// @brief 現状態の変数番号から位置番号に変換する．
  /// @retval true 変換成功
  /// @retval false 変数番号が範囲外だった．
  bool
  cur_varid2pos(ymuint varid,
		ymuint& pos) const;
  
  /// @brief 次状態の変数番号を返す．
  /// @param[in] pos 位置番号 ( 0<= pos < ff_num() )
  ymuint
  next_varid(ymuint pos) const;
  
  /// @brief 次状態の変数番号から位置番号に変換する．
  /// @retval true 変換成功
  /// @retval false 変数番号が範囲外だった．
  bool
  next_varid2pos(ymuint varid,
		 ymuint& pos) const;

  /// @brief 次状態遷移関係を返す．
  Bdd
  trans_relation() const;
  
  /// @brief 到達可能状態を求める．
  /// @param[in] init_states 初期状態集合
  /// @return 到達可能状態
  Bdd
  enum_reachable_states(const vector<State>& init_states);
  
  /// @brief 状態遷移確率を求める．
  /// @param[in] reachable_states_bdd 到達可能状態を表す BDD
  /// @param[in] reachable_states 到達可能状態を収めたベクタ
  /// @param[out] trans_prob_map 状態遷移確率を格納するハッシュ表
  void
  calc_trans_prob(const Bdd& reachable_states_bdd,
		  const vector<State>& reachable_states,
		  vector<list<TransProb> >& trans_map);
  
  /// @brief 現状態を BDD に変換する
  Bdd
  cur_state2bdd(State state);
  
  /// @brief 次状態を BDD に変換する
  Bdd
  next_state2bdd(State state);
  
  /// @brief BDD から現状態の集合に変換する．
  void
  bdd2cur_states(Bdd bdd,
		 vector<State>& states) const;
    
  /// @brief BDD から次状態の集合に変換する．
  void
  bdd2next_states(Bdd bdd,
		  vector<State>& states) const;
  

private:

  /// @brief calc_trans_prob の下請け関数
  /// @param[in] rel 状態遷移関係
  /// @param[in] ns 到達可能状態数
  /// @param[in] state_hash 状態文字列から状態番号を得るためのハッシュ表
  /// @param[in] st_vec 現在たどっている状態を入れる作業領域
  /// @param[in] trans_prob_map 確率を収めるハッシュ表
  void
  rs_sub(Bdd rel,
	 const hash_map<State, ymuint>& state_hash,
	 vector<ymuint>& st_vec,
	 vector<list<TransProb> >& trans_map);
  
  /// @brief 次状態集合の BDD を現状態集合の BDD に変換する．
  Bdd
  next_states2cur_states(const Bdd& next_bdd) const;

  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////
  
  // BDD マネージャ
  BddMgrRef mBddMgr;
  
  // 入力変数の配列
  vector<ymuint32> mInputVarIds;

  // 現状態変数の配列
  vector<ymuint32> mCurVarIds;

  // 次状態変数の配列
  vector<ymuint32> mNextVarIds;

  // 次状態遷移関係
  Bdd mTransRel;
  
  // mTransRel から入力変数を消去したもの
  Bdd mTransRel1;
  
  // 外部入力の変数集合
  BddVarSet mInputVars;

  // 現状態の変数集合
  BddVarSet mCurStateVars;
  
  // 次状態から現状態を得るための変換マップ
  VarVarMap mNext2CurMap;
  
  // 最小項の重み
  double mWeight;
  
};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 入力のビット数を返す．
inline
ymuint
BddFsm::input_num() const
{
  return mInputVarIds.size();
}

// @brief 状態のビット数を返す．
inline
ymuint
BddFsm::ff_num() const
{
  return mCurVarIds.size();
}

// @brief 入力の変数番号を返す．
// @param[in] pos 位置番号 ( 0 <= pos < input_num() )
inline
ymuint
BddFsm::input_varid(ymuint pos) const
{
  return mInputVarIds[pos];
}

// @brief 現状態の変数番号を返す．
// @param[in] pos 位置番号 ( 0 <= pos < ff_num() )
inline
ymuint
BddFsm::cur_varid(ymuint pos) const
{
  return mCurVarIds[pos];
}

// @brief 現状態の変数番号から位置番号に変換する．
// @retval true 変換成功
// @retval false 変数番号が範囲外だった．
inline
bool
BddFsm::cur_varid2pos(ymuint varid,
		      ymuint& pos) const
{
  ymuint n = ff_num();
  for (pos = 0; pos < n; ++ pos) {
    if ( varid == mCurVarIds[pos] ) {
      return true;
    }
  }
  return false;
}

// @brief 次状態の変数番号を返す．
// @param[in] pos 位置番号 ( 0<= pos < ff_num() )
inline
ymuint
BddFsm::next_varid(ymuint pos) const
{
  return mNextVarIds[pos];
}
  
// @brief 次状態の変数番号から位置番号に変換する．
// @retval true 変換成功
// @retval false 変数番号が範囲外だった．
inline
bool
BddFsm::next_varid2pos(ymuint varid,
		       ymuint& pos) const
{
  ymuint n = ff_num();
  for (pos = 0; pos < n; ++ pos) {
    if ( varid == mNextVarIds[pos] ) {
      return true;
    }
  }
  return false;
}

// @brief 次状態遷移関係を返す．
inline
Bdd
BddFsm::trans_relation() const
{
  return mTransRel;
}
  
// @brief 次状態集合の BDD を現状態集合の BDD に変換する．
inline
Bdd
BddFsm::next_states2cur_states(const Bdd& next_bdd) const
{
  return next_bdd.remap_var(mNext2CurMap);
}

END_NAMESPACE_YM_SEAL

#endif // LIBYM_SEAL_BDDFSM_H
