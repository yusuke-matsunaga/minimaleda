
/// @file libym_bdd/dec/Bidec.cc
/// @brief Bidec の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: Bidec.cc 700 2007-05-31 00:41:30Z matsunaga $
///
/// Copyright (C) 2005-2006 Yusuke Matsunaga
/// All rights reserved.


#if HAVE_CONFIG_H
#include <ymconfig.h>
#endif

#include <ym_utils/MFSet.h>
#include <ym_bdd/Dg.h>

//#define DEBUG_MERGE_BIDEC
//#define DEBUG_FIND_COMMON_BIDEC
//#define DEBUG_ENUM_COMMON_BIDEC


BEGIN_NAMESPACE_YM_BDD

BEGIN_NONAMESPACE

// support に含まれている変数を同一のグループにマージする．
void
merge_vargroup(MFSet& mfset,
	       const BddVarSet& support)
{
  if ( !support.empty() ) {
    BddVarSet::iterator p = support.begin();
    size_t top_id = *p;
    while ( ++ p != support.end() ) {
      tVarId vid = *p;
      top_id = mfset.merge(top_id, vid);
    }
  }
}

END_NONAMESPACE


// \f$2^|\mbox{idx\_vector}|\f$個の2項分解 bidec_vector の共通部分を求める．
//
// @param[in] idx_vector インデックスのリスト
// @param[in] bidec_vector idex_vector によるコファクターの
// 2項分解の結果のリスト
// @param[in] m_bidec 共通な2項分解を格納する変数
// @return 共通部分をもつとき true を返す．
bool
merge_bidec(const VarVector& idx_vector,
	    const BidecVector& bidec_vector,
	    Bidec& m_bidec)
{
  size_t n = idx_vector.size();
  size_t nexp = 1 << n;

#if defined(DEBUG_MERGE_BIDEC)
  cout << endl << "merge_bidec" << endl;
  for (size_t i = 0; i < nexp; i ++) {
    cout << "#" << i << endl;
    bidec_vector[i].display(cout);
  }
  cout << endl;
#endif

  int com = bidec_vector[0].root_type();
  for (size_t i = 1; i < nexp; ++ i) {
    com &= bidec_vector[i].root_type();
  }
  if ( !com ) {
    // 共通部分はない．
#if defined(DEBUG_MERGE_BIDEC)
    cout << "merge_bidec end -- COULD NOT MERGE" << endl;
#endif
    return false;
  }

  if ( com & Bidec::kAnd ) {
    com = Bidec::kAnd;
  }
  else if ( com & Bidec::kOr ) {
    com = Bidec::kOr;
  }
  else {
    com = Bidec::kXor;
  }
  BddMgrRef mgr = bidec_vector[0].global_func().mgr();
  Bdd f = mgr.make_zero();
  Bdd g0 = mgr.make_zero();
  Bdd g1 = mgr.make_zero();
  for (size_t i = 0; i < nexp; ++ i) {
    Bdd idx = mgr.make_one();
    for (size_t j = 0; j < n; ++ j) {
      if ( i & (1 << j) ) {
	idx &= mgr.make_posiliteral(idx_vector[j]);
      }
      else {
	idx &= mgr.make_negaliteral(idx_vector[j]);
      }
    }
    f |= idx & bidec_vector[i].global_func();
    Bdd gg0 = bidec_vector[i].input_func(0);
    Bdd gg1 = bidec_vector[i].input_func(1);
    int t = bidec_vector[i].root_type();
    if ( t & Bidec::kConst0 ) {
      gg0 = mgr.make_zero();
      gg1 = mgr.make_zero();
    }
    else if ( t & Bidec::kConst1 ) {
      gg0 = mgr.make_one();
      if ( com == Bidec::kXor ) {
	gg1 = mgr.make_zero();
      }
      else {
	gg1 = mgr.make_one();
      }
    }
    else if ( t & Bidec::kLit0 ) {
      if ( com == Bidec::kAnd ) {
	gg1 = mgr.make_one();
      }
      else {
	gg1 = mgr.make_zero();
      }
    }
    else if ( t & Bidec::kLit1 ) {
      if ( com == Bidec::kAnd ) {
	gg0 = mgr.make_one();
      }
      else {
	gg0 = mgr.make_zero();
      }
    }
    g0 |= idx & gg0;
    g1 |= idx & gg1;
  }

  m_bidec.set(f, g0, g1);

#if defined(DEBUG_MERGE_BIDEC)
  cout << "merge_bidec end" << endl;
  m_bidec.display(cout);
#endif

  return true;
}

// 2^|idx| 個の関数 cf[] が同種の二項分解を持つとき true を返す．
// その時の 二項分解を m_bidec に入れる．
// ただし，第一項の入力数は limit1 以下, 第二項の入力数は limit2 以下
// とする．
bool
find_common_bidec(const VarVector& idx,
		  const BddVector& cf,
		  size_t limit1,
		  size_t limit2,
		  Bidec& m_bidec)
{
#if defined(DEBUG_FIND_COMMON_BIDEC)
  cout << "find_common_bidec begin" << endl;
#endif

  size_t n = idx.size();

  list<BidecVector> bidec_list;
  enum_common_bidec(cf, limit1 - n, limit2 - n, bidec_list);

  for (list<BidecVector>::iterator p = bidec_list.begin();
       p != bidec_list.end(); ++ p) {
    BidecVector bidec_vect = *p;
    if ( merge_bidec(idx, bidec_vect, m_bidec) ) {
#if defined(DEBUG_FIND_COMMON_BIDEC)
      cout << "find_common_bidec end: status -- FOUND" << endl;
#endif
      return true;
    }
  }
#if defined(DEBUG_FIND_COMMON_BIDEC)
  cout << "find_common_bidec end: status -- NG" << endl;
#endif
  return false;
}

// 関数のベクタ cf[] の両立する二項分解を求める．
// ただし，第一項の入力数は limit1 以下, 第二項の入力数は limit2 以下
// とする．
void
enum_common_bidec(const BddVector& cf,
		  size_t limit1,
		  size_t limit2,
		  list<BidecVector>& bidec_list)
{
  size_t nexp = cf.size();

#if defined(DEBUG_ENUM_COMMON_BIDEC)
  cout << "enum_common_bidec() begin" << endl;
  for (size_t i = 0; i < nexp; ++ i) {
    cout << "cf[" << i << "]: " << cf[i].sop() << endl;
  }
#endif

  // 個々の関数の二項分解グラフを作り, 共通な変数分割を得る．
  //DgMgr& dgmgr = DgMgr::get_obj();
  BddMgrRef bddmgr = cf[0].mgr();
  DgMgr dgmgr(bddmgr);
  BddVarSet all_sup(bddmgr);
  vector<Dg> dg_vector(nexp);
  for (size_t i = 0; i < nexp; ++ i) {
    all_sup += cf[i].support();
    Dg dg = dgmgr.bidecomp(cf[i]);
    dg_vector[i] = dg;
  }

  size_t nsup = all_sup.size();
  size_t max_id = 0;
  for (BddVarSet::iterator p = all_sup.begin(); p != all_sup.end(); ++ p) {
    size_t id = *p;
    if ( max_id < id ) {
      max_id = id;
    }
  }
  ++ max_id;
  MFSet mfset(max_id);
  for (size_t i = 0; i < nexp; ++ i) {
    const Dg& dg = dg_vector[i];
    if ( dg.is_bidecomp() ) {
      size_t ni = dg.ni();
      for (size_t j = 0; j < ni; ++ j) {
	merge_vargroup(mfset, dg.input(j).support());
      }
    }
    else {
      merge_vargroup(mfset, dg.support());
    }
  }

  // 変数グループの数を数える．
  vector<size_t> vargroup(max_id, 0);
  vector<size_t> replist;
  replist.reserve(nsup);
  for (BddVarSet::iterator p = all_sup.begin(); p != all_sup.end(); ++ p) {
    size_t id = *p;
    size_t rep_id = mfset.find(id);
    if ( vargroup[rep_id] == 0 ) {
      replist.push_back(rep_id);
    }
    ++ vargroup[rep_id];
  }

  size_t ng = replist.size();

  if ( ng == 1 ) {
#if defined(DEBUG_ENUM_COMMON_BIDEC)
    cout << "enum_common_bidec() end ( ng == 1 )" << endl;
#endif
    return;
  }

  // ここに来たら ng>= 2
  size_t nend = 1 << ng;
  for (size_t b = 0; b < nend; ++ b) {
    size_t n0 = 0;
    for (size_t i = 0; i < ng; ++ i) {
      if ( b & (1 << i) ) {
	n0 += vargroup[replist[i]];
      }
    }
    if ( n0 <= limit1 && nsup - n0 <= limit2 ) {
      // 制約を満たす分解が見つかった．

      // 最初の変数グループに含まれる代表の変数番号
      hash_set<size_t> rep0;
      for (size_t i = 0; i < ng; ++ i) {
	if ( b & (1 << i) ) {
	  rep0.insert(replist[i]);
	}
      }
      BddVarSet var0(bddmgr);
      for (BddVarSet::iterator p = all_sup.begin(); p != all_sup.end(); ++ p) {
	size_t id = *p;
	size_t rep_id = mfset.find(id);
	if ( rep0.count(rep_id) > 0 ) {
	  var0 += BddVarSet(bddmgr, id);
	}
      }

      bidec_list.push_back(BidecVector(nexp));
      BidecVector& tmp = bidec_list.back();
      for (size_t i = 0; i < nexp; ++ i) {
	const Dg& dg = dg_vector[i];
	if ( dg.is_bidecomp() ) {
	  size_t ni = dg.ni();
	  Bdd g1 = bddmgr.make_zero();
	  Bdd g2 = bddmgr.make_zero();
	  for (size_t j = 0; j < ni; ++ j) {
	    Bdd& dst = ( dg.input(j).support() && var0 ) ? g1 : g2;
	    Bdd src = dg.input(j).global_func();
	    switch ( dg.type() ) {
	    case Dg::kAnd  :
	    case Dg::kOr   : dst |= src; break;
	    case Dg::kXor  :
	    case Dg::kXnor : dst ^= src; break;
	    default: break;
	    }
	  }
	  tmp[i].set(cf[i], g1, g2);
	}
	else {
	  if ( dg.support() && var0 ) {
	    tmp[i].set(cf[i], dg.global_func(), bddmgr.make_zero());
	  }
	  else {
	    tmp[i].set(cf[i], bddmgr.make_zero(), dg.global_func());
	  }
	}
      }

#if defined(DEBUG_ENUM_COMMON_BIDEC)
      cout << "FOUND" << endl;
      for (size_t i = 0; i < nexp; ++ i) {
	cout << "#" << i << endl;
	tmp[i].display(cout);
      }
#endif
    }
  }
#if defined(DEBUG_ENUM_COMMON_BIDEC)
  cout << "enum_common_bidec() end" << endl;
#endif
}

// 内容をセットする．
// @param f 出力の論理関数
// @param g0 1項目の論理関数
// @param g1 2項目の論理関数
// 根のタイプ(複数の候補があり得る)を求めてセットする．
void
Bidec::set(const Bdd& f,
	   const Bdd& g0,
	   const Bdd& g1)
{
  mGlobalFunc = f;

  if ( f.is_zero() ) {
    mType = kConst0 | kLit0 | kLit1 | kAnd | kOr | kXor;
    mInputFunc[0] = f;
    mInputFunc[1] = f;
  }
  else if ( f.is_one() ) {
    mType = kConst1 | kLit0 | kLit1 | kAnd | kOr | kXor;
    mInputFunc[0] = f;
    mInputFunc[1] = f;
  }
  else if ( g0.is_zero() || g0.is_one() ) {
    mType = kLit1 | kAnd | kOr | kXor;
    mInputFunc[1] = f;
    mInputFunc[0] = f.mgr().make_zero();
  }
  else if ( g1.is_zero() || g1.is_one() ) {
    mType = kLit0 | kAnd | kOr | kXor;
    mInputFunc[0] = f;
    mInputFunc[1] = f.mgr().make_zero();
  }
  else {
    int sig = 0;

    Bdd p00 = ~g0 & ~g1;
    if ( !p00.is_zero() ) {
      Bdd m = f / p00;
      if ( m.is_one() ) {
	sig |= 1;
      }
      else {
	assert_cond(m.is_zero(), __FILE__, __LINE__);
      }
    }

    Bdd p01 = g0 & ~g1;
    if ( !p01.is_zero() ) {
      Bdd m = f / p01;
      if ( m.is_one() ) {
	sig |= 2;
      }
      else {
	assert_cond(m.is_zero(), __FILE__, __LINE__);
      }
    }

    Bdd p10 = ~g0 & g1;
    if ( !p10.is_zero() ) {
      Bdd m = f / p10;
      if ( m.is_one() ) {
	sig |= 4;
      }
      else {
	assert_cond(m.is_zero(), __FILE__, __LINE__);
      }
    }

    Bdd p11 = g0 & g1;
    if ( !p11.is_zero() ) {
      Bdd m = f / p11;
      if ( m.is_one() ) {
	sig |= 8;
      }
      else {
	assert_cond(m.is_zero(), __FILE__, __LINE__);
      }
    }

    switch (sig) {
    case 0: // ないと思うけど定数0
      mType = kConst0;
      break;
    case 1: // ~g0 & ~g1;
      mType = kAnd;
      mInputFunc[0] = ~g0;
      mInputFunc[1] = ~g1;
      break;
    case 2: // g0 & ~g1;
      mType = kAnd;
      mInputFunc[0] = g0;
      mInputFunc[1] = ~g1;
      break;
    case 3: // ないと思うけど ~g1;
      mType = kLit1;
      break;
    case 4: // ~g0 & g1;
      mType = kAnd;
      mInputFunc[0] = ~g0;
      mInputFunc[1] = g1;
      break;
    case 5: // ないと思うけど ~g0;
      mType = kLit0;
      break;
    case 6: // g0 xor g1;
      mType = kXor;
      mInputFunc[0] = g0;
      mInputFunc[1] = g1;
      break;
    case 7: // ~g0 | ~g1
      mType = kOr;
      mInputFunc[0] = ~g0;
      mInputFunc[1] = ~g1;
      break;
    case 8: // g0 & g1;
      mType = kAnd;
      mInputFunc[0] = g0;
      mInputFunc[1] = g1;
      break;
    case 9: // ~g0 xor g1 or g0 xor ~g1
      mType = kXor;
      mInputFunc[0] = ~g0;
      mInputFunc[1] = g1;
      break;
    case 10: // ないと思うけど g0
      mType = kLit0;
      break;
    case 11: // g0 | ~g1
      mType = kOr;
      mInputFunc[0] = g0;
      mInputFunc[1] = ~g1;
      break;
    case 12: // ないと思うけど g1
      mType = kLit1;
      break;
    case 13: // ~g0 | g1
      mType = kOr;
      mInputFunc[0] = ~g0;
      mInputFunc[1] = g1;
      break;
    case 14: // g0 | g1
      mType = kOr;
      mInputFunc[0] = g0;
      mInputFunc[1] = g1;
      break;
    case 15: // ないと思うけど定数1
      mType = kConst1;
      break;
    }
  }
}

// 内容を出力するデバッグ用の関数
// @param s 出力ストリーム
void
Bidec::display(ostream& s) const
{
  s << "global func:\t" << global_func().sop() << endl
    << "input func0:\t" << input_func(0).sop() << endl
    << "input func1:\t" << input_func(1).sop() << endl
    << "root type:\t";
  if ( root_type() & kAnd ) {
    s << " AND";
  }
  if ( root_type() & kOr ) {
    s << " OR";
  }
  if ( root_type() & kXor ) {
    s << " XOR";
  }
  if ( root_type() & kLit0 ) {
    s << " LIT0";
  }
  if ( root_type() & kLit1 ) {
    s << " LIT1";
  }
  if ( root_type() & kConst0 ) {
    s << " CONST0";
  }
  if ( root_type() & kConst1 ) {
    s << " CONST1";
  }
  s << endl;
}

END_NAMESPACE_YM_BDD
