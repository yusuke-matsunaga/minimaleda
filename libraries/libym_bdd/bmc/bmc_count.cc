
/// @file libym_bdd/bmc/bmc_count.cc
/// @brief 節点数を数える関数の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: bmc_count.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2006 Yusuke Matsunaga
/// All rights reserved.


#if HAVE_CONFIG_H
#include <ymconfig.h>
#endif

#include "BddMgrClassic.h"


BEGIN_NAMESPACE_YM_BDD

BEGIN_NONAMESPACE

mpz_class n_invect;
ymuint n_invect_int;

tLevel w_level;

END_NONAMESPACE

// e を根とするBDDの節点数を数える．
size_t
BddMgrClassic::size(tBddEdge e)
{
  mNum = 0;
  count1(e);
  clear_pnmark(e);
  return mNum;
}

// edge list に登録されたBDDの節点数を数える．
size_t
BddMgrClassic::size(const list<tBddEdge>& edge_list)
{
  mNum = 0;
  for (list<tBddEdge>::const_iterator p = edge_list.begin();
       p != edge_list.end(); ++ p) {
    count1(*p);
  }
  for (list<tBddEdge>::const_iterator p = edge_list.begin();
       p != edge_list.end(); ++ p) {
    clear_pnmark(*p);
  }
  return mNum;
}

// size() の下請関数
void
BddMgrClassic::count1(tBddEdge e)
{
  for ( ; ; ) {
    Node* vp = get_node(e);
    if ( vp == 0 || vp->pmark() ) {
      return;
    }
    ++ mNum;
    vp->pmark(1);
    count1(vp->edge0());
    e = vp->edge1();
  }
}

// BDD の表す論理関数の minterm の数を返す．
// 無限長精度の整数(mpz_class)を用いて計算する．
// 論理関数の変数の数を指定するバージョン
mpz_class
BddMgrClassic::minterm_count(tBddEdge e,
			     tVarSize n)
{
  if ( check_overflow(e) ) {
    return 0;
  }
  if ( check_error(e) ) {
    return 0;
  }

  ymuint bitsize = n + 1;
  if ( bitsize < sizeof(ymuint) * 8 ) {
    // 値の範囲が int に収まるのなら int 版の関数を呼ぶ．

    // 全入力ベクトルの数の計算
    n_invect_int = 1U << n;

    hash_map<tBddEdge, ymuint> mc_map;
    ymuint ans = mterm_step(e, mc_map);

    return mpz_class(ans);
  }
  else {
    // 全入力ベクトルの数の計算
    n_invect = mpz_class(1U) << n;

    hash_map<tBddEdge, mpz_class> mc_map;
    mpz_class ans = mterm_step(e, mc_map);
    return ans;
  }
}

// mpz_class 版の minterm_count の下請関数
mpz_class
BddMgrClassic::mterm_step(tBddEdge e,
			  hash_map<tBddEdge, mpz_class>& mc_map)
{
  if ( check_one(e) ) {
    return n_invect;
  }
  if ( check_zero(e) ) {
    return 0;
  }

  Node* vp = get_node(e);
  tPol pol = get_pol(e);
  ymuint ref = vp->refcount();
  if ( ref != 1 ) {
    hash_map<tBddEdge, mpz_class>::iterator p = mc_map.find(e);
    if ( p != mc_map.end() ) {
      return p->second;
    }
  }

  // 子ノードが表す関数のminterm数を計算する
  mpz_class n0 = mterm_step(vp->edge0(pol), mc_map);
  mpz_class n1 = mterm_step(vp->edge1(pol), mc_map);

  // 子ノードが表す関数の minterm 数を足して半分にしたものが
  // 親ノードが表す関数の minterm 数
  mpz_class ans = (n0 + n1) >> 1;

  if ( ref != 1) {
    mc_map[e] = ans;
  }

  return ans;
}

// int 版の minterm_count の下請関数
ymuint
BddMgrClassic::mterm_step(tBddEdge e,
			  hash_map<tBddEdge, ymuint>& mc_map)
{
  if ( check_one(e) ) {
    return n_invect_int;
  }
  if ( check_zero(e) ) {
    return 0;
  }

  Node* vp = get_node(e);
  tPol pol = get_pol(e);
  ymuint ref = vp->refcount();
  if ( ref != 1 ) {
    hash_map<tBddEdge, ymuint>::iterator p = mc_map.find(e);
    if ( p != mc_map.end() ) {
      return p->second;
    }
  }


  // 子ノードが表す関数のminterm数を計算する
  ymuint n0 = mterm_step(vp->edge0(pol), mc_map);
  ymuint n1 = mterm_step(vp->edge1(pol), mc_map);

  // 子ノードが表す関数の minterm 数を足して半分にしたものが
  // 親ノードが表す関数の minterm 数
  ymuint ans = (n0 + n1) >> 1;

  if ( ref != 1) {
    mc_map[e] = ans;
  }

  return ans;
}

// Walsh変換を行なう．ただし，0次の係数しか求めない．
// n は全入力数
mpz_class
BddMgrClassic::walsh0(tBddEdge e,
		      tVarSize n)
{
  if ( check_overflow(e) ) {
    return 0;
  }
  if ( check_error(e) ) {
    return 0;
  }

  ymuint bitsize = n + 2;
  if ( bitsize < sizeof(int) * 8 ) {
    // 値の範囲が int に収まるのなら int 版の関数を呼ぶ．
    n_invect_int = 1U << n;

    hash_map<Node*, ymint> result_map;
    ymuint result = wt0_step(e, result_map);

    return mpz_class(result);
  }
  else {
    n_invect = mpz_class(1U) << n;

    hash_map<Node*, mpz_class> result_map;
    mpz_class result = wt0_step(e, result_map);

    return result;
  }
}

// Walsh spectrumの0次の係数を求める処理
mpz_class
BddMgrClassic::wt0_step(tBddEdge e,
			hash_map<Node*, mpz_class>& result_map)
{
  if ( check_zero(e) ) {
    return n_invect;
  }
  if ( check_one(e) ) {
    return -n_invect;
  }

  // まずハッシュ表を探す．
  Node* vp = get_node(e);
  tPol pol = get_pol(e);
  ymuint ref = vp->refcount();
  if ( ref != 1 ) {
    hash_map<Node*, mpz_class>::iterator p = result_map.find(vp);
    if ( p != result_map.end() ) {
      if ( pol == kPolPosi ) {
	return p->second;
      }
      else {
	return -p->second;
      }
    }
  }

  // 未登録の場合．
  // 子供の値を再帰的に計算してそれを足し合わせる．
  mpz_class n0 = wt0_step(vp->edge0(), result_map);
  mpz_class n1 = wt0_step(vp->edge1(), result_map);
  mpz_class result = (n0 + n1) >> 1;

  if ( ref != 1 ) {
    result_map[vp] = result;
  }

  if ( pol == kPolNega ) {
    result = -result;
  }

  return result;
}

// Walsh spectrumの0次の係数を求める処理
// こちらは int 版
ymint
BddMgrClassic::wt0_step(tBddEdge e,
			hash_map<Node*, ymint>& result_map)
{
  if ( check_zero(e) ) {
    return static_cast<ymint>(n_invect_int);
  }
  if ( check_one(e) ) {
    return -static_cast<ymint>(n_invect_int);
  }

  // まずハッシュ表を探す．
  Node* vp = get_node(e);
  tPol pol = get_pol(e);
  ymuint ref = vp->refcount();
  if ( ref != 1 ) {
    hash_map<Node*, ymint>::iterator p = result_map.find(vp);
    if ( p != result_map.end() ) {
      if ( pol == kPolPosi ) {
	return p->second;
      }
      else {
	return -p->second;
      }
    }
  }

  // 未登録の場合．
  // 子供の値を再帰的に計算してそれを足し合わせる．
  ymint n0 = wt0_step(vp->edge0(), result_map);
  ymint n1 = wt0_step(vp->edge1(), result_map);
  ymint result = (n0 + n1) >> 1;

  if ( ref != 1 ) {
    result_map[vp] = result;
  }

  if ( pol == kPolNega ) {
    result = -result;
  }

  return result;
}

// Walsh変換を行なう．ただし，1次の係数しか求めない．
// n は全入力数
mpz_class
BddMgrClassic::walsh1(tBddEdge e,
		      tVarId var,
		      tVarSize n)
{
  if ( check_overflow(e) || check_error(e) ) {
    return 0;
  }

  w_level = level(var);

  // var でコファクターを取るので必要なビット数をひとつ減らす
  ymuint bitsize = n + 1;
  if ( bitsize < sizeof(ymint) * 8 ) {
    // 値の範囲が int に収まるのなら int 版の関数を呼ぶ．
    n_invect_int = 1U << n;

    hash_map<Node*, ymint> result_map;
    ymint result = wt1_step(e, result_map);

    return mpz_class(result);
  }
  else {
    n_invect = mpz_class(1U) << n;

    hash_map<Node*, mpz_class> result_map;
    mpz_class result = wt1_step(e, result_map);

    return result;
  }
}

// Walsh spectrumの1次の係数を求める処理
mpz_class
BddMgrClassic::wt1_step(tBddEdge e,
			hash_map<Node*, mpz_class>& result_map)
{
  if ( check_zero(e) || check_one(e) ) {
    return 0;
  }

  Node* vp = get_node(e);
  tLevel level = vp->level();
  if ( level > w_level ) {
    return 0;
  }

  // まずハッシュ表を探す．
  tPol pol = get_pol(e);
  ymuint ref = vp->refcount();
  if ( ref != 1 ) {
    hash_map<Node*, mpz_class>::iterator p = result_map.find(vp);
    if ( p != result_map.end() ) {
      if ( pol == kPolPosi ) {
	return p->second;
      }
      else {
	return -p->second;
      }
    }
  }

  // 未登録の場合．
  mpz_class result;
  if ( level < w_level ) {
    // 子供の値を再帰的に計算してそれを足し合わせる．
    mpz_class n0 = wt1_step(vp->edge0(), result_map);
    mpz_class n1 = wt1_step(vp->edge1(), result_map);
    result = (n0 + n1) >> 1;
  }
  else {
    // 正のコファクターの結果から負のコファクターの結果を引く
    mpz_class n0 = wt0_step(vp->edge0(), result_map);
    mpz_class n1 = wt0_step(vp->edge1(), result_map);
    result = n0 - n1;
  }
  if ( ref != 1 ) {
    result_map[vp] = result;
  }

  if ( pol == kPolNega ) {
    result = -result;
  }

  return result;
}

// Walsh spectrumの1次の係数を求める処理
// int 版
ymint
BddMgrClassic::wt1_step(tBddEdge e,
			hash_map<Node*, ymint>& result_map)
{
  if ( check_zero(e) || check_one(e) ) {
    return 0;
  }

  Node* vp = get_node(e);
  tLevel level = vp->level();
  if ( level > w_level ) {
    return 0;
  }

  // まずハッシュ表を探す．
  tPol pol = get_pol(e);
  ymuint ref = vp->refcount();
  if ( ref != 1 ) {
    hash_map<Node*, int>::iterator p = result_map.find(vp);
    if ( p != result_map.end() ) {
      if ( pol == kPolPosi ) {
	return p->second;
      }
      else {
	return -p->second;
      }
    }
  }

  // 未登録の場合．
  ymint result;
  if ( level < w_level ) {
    // 子供の値を再帰的に計算してそれを足し合わせる．
    ymint n0 = wt1_step(vp->edge0(), result_map);
    ymint n1 = wt1_step(vp->edge1(), result_map);
    result = (n0 + n1) >> 1;
  }
  else {
    // 正のコファクターの結果から負のコファクターの結果を引く
    ymint n0 = wt0_step(vp->edge0(), result_map);
    ymint n1 = wt0_step(vp->edge1(), result_map);
    result = n0 - n1;
  }
  if ( ref != 1 ) {
    result_map[vp] = result;
  }

  if ( pol == kPolNega ) {
    result = -result;
  }

  return result;
}

// e を根とするBDDの節点に n-mark を付け，各変数ごとのノード数を数える．
void
BddMgrClassic::scan(tBddEdge e,
		    hash_map<tVarId, size_t>& node_counts)
{
  for (Var* var = mVarTop; var; var = var->mNext) {
    var->mMark = 0;
  }
  scan_step(e);
  clear_pmark(e);
  for (Var* var = mVarTop; var; var = var->mNext) {
    node_counts.insert(make_pair(var->varid(), var->mMark));
    var->mMark = 0;
  }
}

// scan の下請関数
void
BddMgrClassic::scan_step(tBddEdge e)
{
  for ( ; ; ) {
    Node* vp = get_node(e);
    if ( !vp || vp->pmark() ) {
      break;
    }
    vp->pmark(1);
    vp->nmark(1);
    Var* var = vp->var();
    ++ var->mMark;
    scan_step(vp->edge0());
    e = vp->edge1();
  }
}

// e を根とするBDDのレベル level のノード数を数える．
// ただし，n-mark の付いていないノードがあったら UINT_MAX を返す．
size_t
BddMgrClassic::count_at(tBddEdge e,
			tLevel level)
{
  ymuint ans = count_at_step(e, level);
  clear_pmark(e);
  return ans;
}

// bdd のレベル level の *** n_mark のついた *** 節点の数を数える．
// ただし，n_mark の付いていない節点があった場合には ULONG_MAX を返す．
size_t
BddMgrClassic::count_at_step(tBddEdge e,
			     tLevel level)
{
  // tail recursion elimination を行なっているのでコードが
  // みづらい．
  size_t n = 0;
  for ( ; ; ) {
    Node* vp = get_node(e);
    if ( vp == 0 || vp->pmark() ) {
      break;
    }
    int diff = level - vp->level();
    vp->pmark(1);
    if ( diff <=  0 ) {
      if ( diff == 0 ) {
	if ( vp->nmark() ) {
	  ++ n;
	}
	else {
	  n = ULONG_MAX;
	}
      }
      break;
    }
    else {
      // if ( diff > 0 ) {
      size_t n1 = count_at_step(vp->edge0(), level);
      if ( n1 == ULONG_MAX ) {
	n = ULONG_MAX;
	break;
      }
      n += n1;
      e = vp->edge1();
    }
  }
  return n;
}

// scan で付けた n-mark を消す．
void
BddMgrClassic::clear_scanmark(tBddEdge e)
{
  clear_nmark(e);
}

END_NAMESPACE_YM_BDD
