
/// @file libym_seal/BddFsm.cc
/// @brief BddFsm の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: BddFsm.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "BddFsm.h"
#include "ym_bdd/Bdd.h"
#include "ym_bdd/BddLitSet.h"

#include "ym_utils/StopWatch.h"


BEGIN_NAMESPACE_YM_SEAL

// @brief コンストラクタ
// @param[in] bdd_mgr BDD マネージャ
// @param[in] input_vars 入力変数番号の配列
// @param[in] state_vars 状態変数番号の配列(現状態と次状態のペア)
// @param[in] trans_relation 状態遷移関係
BddFsm::BddFsm(BddMgrRef bdd_mgr,
	       const vector<ymuint>& input_vars,
	       const vector<pair<ymuint, ymuint> >& state_vars,
	       const Bdd& trans_relation) :
  mBddMgr(bdd_mgr),
  mInputVarIds(input_vars.size()),
  mCurVarIds(state_vars.size()),
  mNextVarIds(state_vars.size()),
  mTransRel(trans_relation),
  mInputVars(bdd_mgr),
  mCurStateVars(bdd_mgr)
{
  for (ymuint i = 0; i < input_num(); ++ i) {
    mInputVarIds[i] = input_vars[i];
  }
  
  // 入力変数の集合を表す BDD を作る．
  mInputVars = BddVarSet(bdd_mgr, mInputVarIds);

  // 状態遷移関係から入力変数を消去する．
  mTransRel1 = mTransRel.esmooth(mInputVars);

  for (ymuint i = 0; i < ff_num(); ++ i) {
    ymuint cur_id = state_vars[i].first;
    ymuint next_id = state_vars[i].second;
    mCurVarIds[i] = cur_id;
    mNextVarIds[i] = next_id;
    mNext2CurMap.insert(make_pair(next_id, cur_id));
  }
  
  // 現状態変数の集合を表す BDD を作る．
  mCurStateVars = BddVarSet(bdd_mgr, mCurVarIds);

  // 状態遷移確率計算用の最小項の重みをセットする．
  mWeight = 1.0 / pow(2, input_num());
}

// @brief デストラクタ
BddFsm::~BddFsm()
{
}

// @brief 到達可能状態を求める．
// @param[in] init_states 初期状態集合
// @param[out] reachable_states 到達可能状態
Bdd
BddFsm::enum_reachable_states(const vector<State>& init_states)
{
  StopWatch sw;
  sw.reset();
  sw.start();
  
  // State のベクタを BDD に変換する．
  Bdd cur_bdd = mBddMgr.make_zero();
  for (vector<State>::const_iterator p = init_states.begin();
       p != init_states.end(); ++ p) {
    cur_bdd |= cur_state2bdd(*p);
  }
#if 0
  vector<State> tmp;
  bdd2cur_states(cur_bdd, tmp);
  cout << "Initial states" << endl;
  for (vector<State>::iterator p = tmp.begin(); p != tmp.end(); ++ p) {
    cout << *p << endl;
  }
#endif
  
  Bdd reached_bdd = cur_bdd;
  while ( !cur_bdd.is_zero() ) {
    Bdd next_bdd = (mTransRel1 & cur_bdd).esmooth(mCurStateVars);
    if ( next_bdd.is_invalid() ) {
      // たぶんメモリオーバーフロー
      cout << "BddFsm::enum_reachable_states(): Memory Overflow"
	   << endl;
      abort();
    }
    Bdd new_bdd = next_states2cur_states(next_bdd);
    cur_bdd = new_bdd & ~reached_bdd;
    reached_bdd |= new_bdd;
#if 0
    vector<State> tmp;
    bdd2cur_states(cur_bdd, tmp);
    cout << "New states" << endl;
    for (vector<State>::iterator p = tmp.begin(); p != tmp.end(); ++ p) {
      cout << *p << endl;
    }
#endif
  }

  sw.stop();
  cout << "BddFsm::enum_reachable_states(): "
       << sw.time() << endl;

  return reached_bdd;
}

// @brief 状態遷移確率を求める．
// @param[in] reachable_states_bdd 到達可能状態を表す BDD
// @param[in] reachable_states 到達可能状態を収めたベクタ
// @param[out] trans_prob_map 状態遷移確率を格納するハッシュ表
void
BddFsm::calc_trans_prob(const Bdd& reachable_states_bdd,
			const vector<State>& reachable_states,
			vector<list<TransProb> >& trans_map)
{
  StopWatch sw;
  sw.reset();
  sw.start();
  
  // 状態遷移関係を到達可能状態のみに制約する．
  Bdd trans2 = mTransRel & reachable_states_bdd;

  // 状態文字列から状態番号のハッシュ表を作る．
  ymuint ns = reachable_states.size();
  hash_map<State, ymuint> state_hash;
  for (ymuint i = 0; i < ns; ++ i) {
    state_hash.insert(make_pair(reachable_states[i], i));
  }
  
  vector<ymuint> st_vec(ff_num() * 2, 0);

  trans_map.clear();
  trans_map.resize(ns);
  rs_sub(trans2, state_hash, st_vec, trans_map);
  
  sw.stop();
  
  cout << "BddFsm::calc_trans_prob(): "
       << sw.time() << endl;
}

// @brief calc_trans_prob の下請け関数
// @param[in] rel 状態遷移関係
// @param[in] ns 到達可能状態数
// @param[in] state_hash 状態文字列から状態番号を得るためのハッシュ表
// @param[in] st_vec 現在たどっている状態を入れる作業領域
// @param[in] trans_prob_map 確率を収めるハッシュ表
void
BddFsm::rs_sub(Bdd rel,
	       const hash_map<State, ymuint>& state_hash,
	       vector<ymuint>& st_vec,
	       vector<list<TransProb> >& trans_map)
{
  if ( rel.is_zero() ) {
    return;
  }
  Bdd l;
  Bdd r;
  tVarId root_idx = rel.root_decomp(l, r);
  ymuint pos;
  if ( cur_varid2pos(root_idx, pos) ) {
    st_vec[pos] = 1;
    rs_sub(l, state_hash, st_vec, trans_map);
    st_vec[pos] = 2;
    rs_sub(r, state_hash, st_vec, trans_map);
    st_vec[pos] = 0;
  }
  else if ( next_varid2pos(root_idx, pos) ) {
    pos += ff_num();
    st_vec[pos] = 1;
    rs_sub(l, state_hash, st_vec, trans_map);
    st_vec[pos] = 2;
    rs_sub(r, state_hash, st_vec, trans_map);
    st_vec[pos] = 0;
  }
  else {
    mpz_class mc = rel.minterm_count(input_num());
    assert_cond(mc.fits_uint_p(), __FILE__, __LINE__);
    double prob = static_cast<double>(mc.get_ui()) * mWeight;
    vector<ymuint> dcmap;
    dcmap.reserve(ff_num() * 2);
    State tmp = "";
    for (ymuint i = 0; i < ff_num() * 2; ++ i) {
      switch ( st_vec[i] ) {
      case 0:
	tmp += '-';
	dcmap.push_back(i);
	break;

      case 1:
	tmp += '0';
	break;

      case 2:
	tmp += '1';
	break;
      }
    }
    
    ymuint dcsize = dcmap.size();
    ymuint dexp = 1U << dcsize;
    for (ymuint p = 0U; p < dexp; ++ p) {
      for (ymuint i = 0; i < dcsize; ++ i) {
	ymuint pos = dcmap[i];
	if ( p & (1U << i) ) {
	  tmp[pos] = '1';
	}
	else {
	  tmp[pos] = '0';
	}
      }
      State cur_state = tmp.substr(0, ff_num());
      hash_map<State, ymuint>::const_iterator q = state_hash.find(cur_state);
      assert_cond( q != state_hash.end(), __FILE__, __LINE__);
      ymuint cur_state_id = q->second;
      State next_state = tmp.substr(ff_num(), ff_num());
      hash_map<State, ymuint>::const_iterator r = state_hash.find(next_state);
      assert_cond( r != state_hash.end(), __FILE__, __LINE__);
      ymuint next_state_id = r->second;
      trans_map[cur_state_id].push_back(TransProb(next_state_id, prob));
    }
  }
}

// @brief 現状態を BDD に変換する
Bdd
BddFsm::cur_state2bdd(State state)
{
  Bdd tmp = mBddMgr.make_one();
  for (ymuint i = 0; i < ff_num(); ++ i) {
    if ( state[i] == '1' ) {
      tmp &= mBddMgr.make_posiliteral(cur_varid(i));
    }
    else {
      tmp &= mBddMgr.make_negaliteral(cur_varid(i));
    }
  }
  return tmp;
}

// @brief 次状態を BDD に変換する
Bdd
BddFsm::next_state2bdd(State state)
{
  Bdd tmp = mBddMgr.make_one();
  for (ymuint i = 0; i < ff_num(); ++ i) {
    if ( state[i] == '1' ) {
      tmp &= mBddMgr.make_posiliteral(next_varid(i));
    }
    else {
      tmp &= mBddMgr.make_negaliteral(next_varid(i));
    }
  }
  return tmp;
}

// @brief BDD から現状態の集合に変換する．
void
BddFsm::bdd2cur_states(Bdd bdd_states,
		       vector<State>& states) const
{
  while ( !bdd_states.is_zero() ) {
    BddLitSet tmp = bdd_states.onepath();
    bdd_states &= ~(tmp.function());
    LiteralVector litvec;
    tmp.to_vector(litvec);
    vector<ymuint> work(ff_num(), 0);
    for (ymuint i = 0; i < litvec.size(); ++ i) {
      Literal l = litvec[i];
      int pat = (l.pol() == kPolNega) ? 1 : 3;
      tVarId id = l.varid();
      ymuint pos;
      bool stat = cur_varid2pos(id, pos);
      if ( stat && pos < ff_num() ) {
	work[pos] = pat;
      }
      else {
	cout << " id = " << id
	     << ", cur_base = " << cur_varid(0)
	     << ", ff_num = " << ff_num() << endl;
	assert_not_reached(__FILE__, __LINE__);
      }
    }
    ymuint dcsize = 0;
    for (ymuint i = 0; i < ff_num(); ++ i) {
      if ( work[i] == 0 ) {
	work[i] = dcsize * 2;
	++ dcsize;
      }
    }
    ymuint n = 1U << dcsize;
    for (ymuint i = 0; i < n; ++ i) {
      State st;
      for (ymuint j = 0; j < ff_num(); ++ j) {
	if ( work[j] == 1 ) {
	  st += '0';
	}
	else if ( work[j] == 3 ) {
	  st += '1';
	}
	else {
	  ymuint pos = (work[j] >> 1);
	  if ( i & (1U << pos) ) {
	    st += '1';
	  }
	  else {
	    st += '0';
	  }
	}
      }
      states.push_back(st);
    }
  }
}

// @brief BDD から次状態の集合に変換する．
void
BddFsm::bdd2next_states(Bdd bdd_states,
			vector<State>& states) const
{
  while ( !bdd_states.is_zero() ) {
    BddLitSet tmp = bdd_states.onepath();
    bdd_states &= ~(tmp.function());
    LiteralVector litvec;
    tmp.to_vector(litvec);
    vector<ymuint> work(ff_num(), 0);
    for (ymuint i = 0; i < litvec.size(); ++ i) {
      Literal l = litvec[i];
      int pat = (l.pol() == kPolNega) ? 1 : 3;
      tVarId id = l.varid();
      ymuint pos;
      bool stat = next_varid2pos(id, pos);
      if ( stat && pos < ff_num() ) {
	work[pos] = pat;
      }
      else {
	cout << " id = " << id
	     << ", next_base = " << next_varid(0)
	     << ", ff_num = " << ff_num() << endl;
	assert_not_reached(__FILE__, __LINE__);
      }
    }
    ymuint dcsize = 0;
    for (ymuint i = 0; i < ff_num(); ++ i) {
      if ( work[i] == 0 ) {
	work[i] = dcsize * 2;
	++ dcsize;
      }
    }
    ymuint n = 1U << dcsize;
    for (ymuint i = 0; i < n; ++ i) {
      State st;
      for (ymuint j = 0; j < ff_num(); ++ j) {
	if ( work[j] == 1 ) {
	  st += '0';
	}
	else if ( work[j] == 3 ) {
	  st += '1';
	}
	else {
	  ymuint pos = (work[j] >> 1);
	  if ( i & (1U << pos) ) {
	    st += '1';
	  }
	  else {
	    st += '0';
	  }
	}
      }
      states.push_back(st);
    }
  }
}

END_NAMESPACE_YM_SEAL
