
/// @file libym_bdd/dec/PrimDec.cc
/// @brief PrimDec の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: PrimDec.cc 700 2007-05-31 00:41:30Z matsunaga $
///
/// Copyright (C) 2005-2006 Yusuke Matsunaga
/// All rights reserved.


#if HAVE_CONFIG_H
#include <ymconfig.h>
#endif

#include <ym_bdd/Dg.h>


BEGIN_NAMESPACE_YM_BDD

// @brief 通常のコンストラクタ
// @param[in] bset 束縛変数を表す変数集合
// @param[in] fset 自由変数を表す変数集合
// @param[in] bfunc 束縛関数
PrimDec::PrimDec(const BddVarSet& bset,
		 const BddVarSet& fset,
		 const Bdd& bfunc) :
  mBoundSet(bset),
  mFreeSet(fset),
  mBoundFunc(1, bfunc)
{
  ;
}

// @brief bfunc をベクタで指定するコンストラクタ
// @param[in] bset 束縛変数を表す変数集合
// @param[in] fset 自由変数を表す変数集合
// @param[in] bfunc 束縛関数のベクタ
PrimDec::PrimDec(const BddVarSet& bset,
		 const BddVarSet& fset,
		 const BddVector& bfunc) :
  mBoundSet(bset),
  mFreeSet(fset),
  mBoundFunc(bfunc)
{
  ;
}

// @brief 内容を出力する．
// @param[in] s 出力ストリーム
void
PrimDec::dump(ostream& s) const
{
  s << "B-set:" << mBoundSet << endl;
  s << "F-set:" << mFreeSet << endl;
  s << "B-func:" << endl;
  for (BddVector::const_iterator it = mBoundFunc.begin();
       it != mBoundFunc.end(); ++ it) {
    s << it->sop() << endl;
  }
}

// @brief 直交関数分解の列挙
//
// 直交関数分解のうち，bound-set の入力数が llimit 以上 ulimit 以下
// のものを列挙する．
// ただし，free-set が空のものや bound-set が空のもの, 入力数が 1 の
// ものも含む．
// @param[in] f 対象の論理関数
// @param[in] llimit 束縛変数集合の要素数の下限
// @param[in] ulimit 束縛変数集合の要素数の上限
// @param[out] dec_list 関数分解を格納するリスト
void
enum_djdec(const Bdd& f,
	   size_t llimit,
	   size_t ulimit,
	   PrimDecList& dec_list)
{
  BddMgrRef mgr = f.mgr();
  if ( f.is_zero() || f.is_one() ) {
    // 定数関数の場合は特別．
    dec_list.push_back(PrimDec(BddVarSet(mgr), BddVarSet(mgr), f));
  }
  else {
    // disjoint support decomposition を行う．
    DgMgr dg_mgr(mgr);
    Dg dg = dg_mgr.decomp(f);
    dg.enum_djdec(llimit, ulimit, dec_list);
  }
}

// @brief CapDec() のための再帰関数
// @param[in] list_set 関数分解リストのベクタ
// @param[in] limit 入力数の上限
// @param[in] i 現在処理中のインデックス
// @param[in] n インデックスの要素数
// @param[in] bset 束縛変数集合
// @param[in] fset 自由変数集合
// @param[in,out] cur_bfunc 現在の束縛関数ベクタ
// @param[out] cap_list 求まった関数分解を納めるリスト
void
cd_recur(const vector<PrimDecList >& list_set,
	 size_t limit,
	 size_t i,
	 size_t n,
	 const BddVarSet& bset,
	 const BddVarSet& fset,
	 BddVector& cur_bfunc,
	 PrimDecList& cap_list)
{
  if ( i == n ) {
    // 見つかった．
    cap_list.push_back(PrimDec(bset, fset, cur_bfunc));
  }
  else {
    const PrimDecList& list1 = list_set[i];
    for (PrimDecList::const_iterator it = list1.begin();
	 it != list1.end(); ++ it) {
      const PrimDec& dec1 = *it;
      if ( !(bset && dec1.free_set()) && !(fset && dec1.bound_set()) ) {
	// ここまでのチェックは通ったので次のレベルに進む．
	BddVarSet new_bset = bset + dec1.bound_set();
	BddVarSet new_fset = fset + dec1.free_set();
	cur_bfunc[i] = dec1.bound_func(0);
	if ( new_bset.size() <= limit ) {
	  cd_recur(list_set, limit, i + 1, n, new_bset, new_fset,
		   cur_bfunc, cap_list);
	}
      }
    }
  }
}

// @brief cf[] に共通な直交分解を求める．
//
// @param[in] cf コファクターごとの論理関数を納めた BDD ベクタ
// @param[in] limit 求める関数分解の入力数の上限
// @param[out] cd_list 求まった関数分解を納めるリスト
void
common_djdec(const BddVector& cf,
	     size_t limit,
	     PrimDecList& cd_list)
{
  size_t nf = cf.size();
  // disjoint decomposition を求める．
  vector<PrimDecList >dec_list(nf);
  for (size_t i = 0; i < nf; i ++) {
    enum_djdec(cf[i], 0, limit, dec_list[i]);
  }

  // 分割の共通部分を探す．
  // 今は幼稚な n乗のアルゴリズムを使う．
  size_t n = dec_list.size();
  cd_list.clear();
  const PrimDecList& list1 = dec_list[0];
  for (PrimDecList::const_iterator it = list1.begin();
       it != list1.end(); ++ it) {
    const PrimDec& dec1 = *it;
    BddVarSet bset = dec1.bound_set();
    BddVarSet fset = dec1.free_set();
    BddVector cur_bfunc(n);
    cur_bfunc[0] = dec1.bound_func(0);
    cd_recur(dec_list, limit, 1, n, bset, fset, cur_bfunc, cd_list);
  }
}

// @brief 元の関数 f と束縛関数 bound_func から出力の関数を得る．
// @param[in] f 元の関数
// @param[in] bound_func 束縛関数
// @param[in] hvar 束縛関数の出力に対応する変数番号
// @return 出力の関数
Bdd
root_func(const Bdd& f,
	  const Bdd& bound_func,
	  tVarId hvar)
{
  if ( bound_func.is_zero() || bound_func.is_one() ) {
    return f;
  }
  BddLitSet path1 = bound_func.onepath();
  BddLitSet path0 = (~bound_func).onepath();
  Bdd g1 = f / path1;
  Bdd g0 = f / path0;
  Bdd h = f.mgr().make_posiliteral(hvar);
  return ite_op(h, g1, g0);
}

// @brief dec_list をダンプする．
// @param[in] dec_list 基本分解のリスト
// @param[in] s 出力ストリーム
void
dump_primdec_list(const PrimDecList& dec_list,
		  ostream& s)
{
  for (PrimDecList::const_iterator it = dec_list.begin();
       it != dec_list.end(); ++ it) {
    const PrimDec& dec = *it;
    if ( dec.bound_set().size() <= 1 || dec.free_set().size() == 0 ) {
      // trivial なものは除く
      continue;
    }
    dec.dump(s);
    s << "========================" << endl;
  }
}

END_NAMESPACE_YM_BDD
