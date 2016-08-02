
/// @file libym_bdd/dec/DgMgr.cc
/// @brief DgMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: DgMgr.cc 700 2007-05-31 00:41:30Z matsunaga $
///
/// Copyright (C) 2005-2006 Yusuke Matsunaga
/// All rights reserved.


#if HAVE_CONFIG_H
#include <ymconfig.h>
#endif

//#define DG_VERIFY

//#define DG_PROFILE

#include <ym_bdd/Dg.h>

#include "DgNode.h"
#include "base/BddMgr.h"

#if defined(DG_PROFILE)

#include <ym_utils/StopWatch.h>

BEGIN_NAMESPACE_YM_BDD
BEGIN_NONAMESPACE
MStopWatch mtimer(20);
END_NONAMESPACE
END_NAMESPACE_YM_BDD

#endif


BEGIN_NAMESPACE_YM_BDD

/// BDD の GC 時に呼ばれるためのバインダ
class DgBinder :
  public EventBinder
{
 public:
  // コンストラクタ
  // DgMgr を引数として指定する．
  DgBinder(DgMgr* mgr);
  
 private:
  // GCの時に呼ばれる仮想関数
  virtual void event_proc();
  
 private:
  DgMgr* mMgr;
};


bool check_with_assign(const Bdd& F0,
		       const BddLitSet& Asg0,
		       const Bdd& F1,
		       const BddLitSet& Asg1);

bool check_with_assign1(const Bdd& F0,
			const BddLitSet& Asg0,
			const Bdd& F1);

static
void disp_merge_end(tVarId varid,
		    tDgEdge r0,
		    tDgEdge r1,
		    tDgEdge result,
		    const char* case_str);

BEGIN_NONAMESPACE
int debug = 0;
END_NONAMESPACE

// @brief コンストラクタ
// @param mgr DGマネージャ
DgBinder::DgBinder(DgMgr* mgr) :
  mMgr(mgr)
{
}

// @brief GC 時に呼ばれる関数
void
DgBinder::event_proc()
{
  mMgr->sweep();
}

// @brief 関数の縮退チェック (その1)
// @return F0 に Asg0 の割り当てを行った関数と F1 に Asg1 の割り当てを行った
// 関数が一致していたら true を返す．
inline
bool
check_with_assign(const Bdd& F0,
		  const BddLitSet& Asg0,
		  const Bdd& F1,
		  const BddLitSet& Asg1)
{
  Bdd g0 = F0 / Asg0;
  Bdd g1 = F1 / Asg1;
  return g0 == g1;
}

// @brief 関数の縮退チェック (その2)
// F0 に Asg0 の割り当てを行った関数と F1 が一致していたら true を返す．
inline
bool
check_with_assign1(const Bdd& F0,
		   const BddLitSet& Asg0,
		   const Bdd& F1)
{
  Bdd g0 = F0 / Asg0;
  return g0 == F1;
}


//////////////////////////////////////////////////////////////////////
// クラス DgMgr
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] mgr BDD マネージャ
DgMgr::DgMgr(BddMgrRef mgr) :
  mMgr(mgr)
{
  mBidecomp = false;
}

// @brief デストラクタ
DgMgr::~DgMgr()
{
  clear();
}

// @brief 作業領域のサイズを設定する．
// @param[in] ni 扱う関数の入力数
void
DgMgr::resize_buffer(size_t ni)
{
  mNi = ni;
}

// @brief 新たな枝の生成
// @param[in] f 対応する BDD
// @param[in] sup f のサポート
// @param[in] type ノードタイプ
// @param[in] inputs 入力枝のリスト
// @param[in] opol 出力につける極性
// @return 作成された枝
//
// 必要に応じてノードの生成も行う．
tDgEdge
DgMgr::new_edge(const Bdd& f,
		const BddVarSet& sup,
		Dg::tType type,
		const vector<tDgEdge>& inputs,
		tPol opol)
{
  // まずハッシュを探す．
  tBddEdge e = f.root();
  tPol bdd_pol = get_pol(e);
  e = normalize(e);
  hash_map<tBddEdge, Dg>& node_hash = mBidecomp ? mNodeHash2 : mNodeHash1;
  hash_map<tBddEdge, Dg>::const_iterator p = node_hash.find(e);
  if ( p != node_hash.end() ) {
    // ハッシュに登録されていた．
    return inv_pol(p->second.mRoot, bdd_pol * opol);
  }

  size_t ni = inputs.size();
  DgNode* node = new DgNode(f, sup, type, ni);
  node->mAllLit = true;
  for (size_t i = 0; i < ni; ++ i) {
    tDgEdge e = inputs[i];
    node->mInputs[i] = e;
    inc_ref(e);
    if ( edge2node(e)->type() != Dg::kLitP ) {
      node->mAllLit = false;
    }
  }
  ++ mN;

#if defined(DG_VERIFY)
  {
    BddVarSet support;
    size_t ni = node->ni();
    for (size_t i = 0; i < ni; i ++) {
      DgNode* inode = node->input_node(i);
      BddVarSet support1 = inode->support();
      if ( support && support1 ) {
	cerr << "ERROR: not disjoint" << endl;
	display(make_edge(node), cerr);
	abort();
      }
      support += support1;
    }
  }
#endif

  tDgEdge result = make_edge(node, opol);

  // ハッシュに登録しておく
  // 登録用の枝はBDDの極性をつける．
  tDgEdge edge = make_edge(node, bdd_pol);
  node_hash.insert(make_pair(e, Dg(edge, this)));

  return result;
}

// @brief 定数ノード(枝)の生成
// @param[in] val 定数値
// @return 生成された枝
//
// val == 0 の時 0 それ以外の時 1 となる．
inline
tDgEdge
DgMgr::make_const(int val)
{
  Bdd f = mMgr.make_zero();
  tPol pol = val ? kPolNega : kPolPosi;
  tDgEdge result = new_edge(f, BddVarSet(mMgr), Dg::kC0,
			    vector<tDgEdge>(0), pol);
  return result;
}

// @brief リテラルの生成
// @param[in] varid 変数番号
// @param[in] pol 極性
// @return 生成された枝
inline
tDgEdge
DgMgr::make_literal(tVarId varid,
		    tPol pol)
{
  Bdd f = mMgr.make_posiliteral(varid);
  tDgEdge result = new_edge(f, f.support(), Dg::kLitP,
			    vector<tDgEdge>(0), pol);
  return result;
}

// @brief 2つの入力を持つORノードの生成
// @param[in] src1 第1オペランド
// @param[in] src2 第2オペランド
// @param[in] opol 出力極性
// @return 生成された枝
inline
tDgEdge
DgMgr::make_or(tDgEdge src1,
	       tDgEdge src2,
	       tPol opol)
{
  vector<tDgEdge> inputs(2);
  inputs[0] = src1;
  inputs[1] = src2;
  return make_or(inputs, opol);
}

// @brief 2つの入力を持つANDノード(実際にはORノードの否定)の生成
// @param[in] src1 第1オペランド
// @param[in] src2 第2オペランド
// @param[in] opol 出力極性
// @return 生成された枝
inline
tDgEdge
DgMgr::make_and(tDgEdge src1,
		tDgEdge src2,
		tPol opol)
{
  vector<tDgEdge> inputs(2);
  inputs[0] = inv_pol(src1);
  inputs[1] = inv_pol(src2);
  tDgEdge result = make_or(src1, src2, ~opol);
  return result;
}

// @brief 2つの入力を持つXORノードの生成
// @param[in] src1 第1オペランド
// @param[in] src2 第2オペランド
// @param[in] opol 出力極性
// @return 生成された枝
inline
tDgEdge
DgMgr::make_xor(tDgEdge src1,
		tDgEdge src2,
		tPol opol)
{
  vector<tDgEdge> inputs(2);
  inputs[0] = src1;
  inputs[1] = src2;
  return make_xor(inputs, opol);
}

// @brief 複数の入力を持つノードの生成
// @param[in] type ノードタイプ(ここでは Dg::kOr か Dg::kXor)
// @param[in] inputs 入力枝のリスト
// @param[in] opol 出力極性
// @return 生成された枝
inline
tDgEdge
DgMgr::make_node(Dg::tType type,
		 const vector<tDgEdge>& inputs,
		 tPol opol)
{
  if ( type == Dg::kOr ) {
    return make_or(inputs, opol);
  }
  else if ( type == Dg::kXor ) {
    return make_xor(inputs, opol);
  }
  else {
    assert_not_reached(__FILE__, __LINE__);
  }
  // ダミー
  return 0;
}

// @brief リテラルとの OR ノードの生成
// @param[in] varid 第1オペランドのリテラルの変数番号
// @param[in] lit_pol 第1オペランドのリテラルの極性
// @param[in] src2 第2オペランド
// @param[in] opol 出力ノード
// @return 生成された枝
inline
tDgEdge
DgMgr::make_litOr(tVarId varid,
		  tPol lit_pol,
		  tDgEdge src2,
		  tPol opol)
{
  tDgEdge lit = make_literal(varid, lit_pol);
  tDgEdge result = make_or(lit, src2, opol);
  return result;
}

// @brief リテラルとの AND ノードの生成
// @param[in] varid 第1オペランドのリテラルの変数番号
// @param[in] lit_pol 第1オペランドのリテラルの極性
// @param[in] src2 第2オペランド
// @param[in] opol 出力ノード
// @return 生成された枝
inline
tDgEdge
DgMgr::make_litAnd(tVarId varid,
		   tPol lit_pol,
		   tDgEdge src2,
		   tPol opol)
{
  tDgEdge lit = make_literal(varid, lit_pol);
  tDgEdge result = make_and(lit, src2, opol);
  return result;
}

// @brief リテラルとの XOR ノードの生成
// @param[in] varid 第1オペランドのリテラルの変数番号
// @param[in] lit_pol 第1オペランドのリテラルの極性
// @param[in] src2 第2オペランド
// @param[in] opol 出力ノード
// @return 生成された枝
inline
tDgEdge
DgMgr::make_litXor(tVarId varid,
		   tPol lit_pol,
		   tDgEdge src2,
		   tPol opol)
{
  tDgEdge lit = make_literal(varid, lit_pol);
  tDgEdge result = make_xor(lit, src2, opol);
  return result;
}

// @brief 単純直交分解
// @param[in] f 論理関数
// @return 単純直交分解を表すグラフ
Dg
DgMgr::decomp(const Bdd& f)
{
#if defined(DG_PROFILE)
  mtimer.change(12);
#endif

  tDgEdge root;
  
  // 特別な場合
  if ( f.is_zero() ) {
    root = make_const(0);
  }
  else if ( f.is_one() ) {
    root = make_const(1);
  }
  else {
    mNi = f.support_size();
    root = decomp_step(f.root());
  }

#if defined(DG_PROFILE)
  mtimer.change(0);
#endif

  return Dg(root, this);
}

// @brief 直交二項分解
// @param[in] f 論理関数
// @return 直交二項分解を表すグラフ
Dg
DgMgr::bidecomp(const Bdd& f)
{
#if defined(DG_PROFILE)
  mtimer.change(12);
#endif

  tDgEdge root;
  
  // 特別な場合
  if ( f.is_zero() ) {
    root = make_const(0);
  }
  else if ( f.is_one() ) {
    root = make_const(1);
  }
  else {
    mBidecomp = true;
    mNi = f.support_size();
    root = decomp_step(f.root());
    mBidecomp = false;
  }
  
#if defined(DG_PROFILE)
  mtimer.change(0);
#endif

  return Dg(root, this);
}

// @brief 関数分解を行なう基本ステップ
// e の表す関数をトップの変数でコファクタリングして
// 2つのコファクタに対する分解グラフを得る．
// それをマージして自分の分解グラフとする．
tDgEdge
DgMgr::decomp_step(tBddEdge e)
{
  // 境界条件
  // 定数関数なら定数枝を返す．
  if ( check_zero(e) ) {
    return kDgEdge0;
  }
  if ( check_one(e) ) {
    return kDgEdge1;
  }

  // 極性を取り除いて処理を行なって最後に付加する．
  tPol pol = get_pol(e);
  e = normalize(e);

  tDgEdge result = 0;
  // ハッシュに登録されているか調べる．
  hash_map<tBddEdge, Dg>& node_hash = mBidecomp ? mNodeHash2 : mNodeHash1;
  hash_map<tBddEdge, Dg>::iterator p = node_hash.find(e);
  if ( p != node_hash.end() ) {
    result = p->second.mRoot;
  }
  else {
    // ハッシュに登録されていないので本当に計算する．
    tBddEdge e0;
    tBddEdge e1;
    tVarId varId = mMgr.mPtr->root_decomp(e, e0, e1);
    tDgEdge r0 = decomp_step(e0);
    tDgEdge r1 = decomp_step(e1);

#if defined(DG_PROFILE)
    mtimer.change(11);
#endif

    // 結果をマージする．
    result = merge(varId, r0, r1);

    // ハッシュに登録する．
    node_hash.insert(make_pair(e, Dg(result, this)));
    
#if defined(DG_PROFILE)
    mtimer.change(12);
#endif

#if defined(DG_VERIFY)
    {
      Bdd f = edge2node(result)->global_func();
      if ( edge2pol(result) ) {
	f.negate();
      }
      Bdd g(combine(vp, kPolPosi));
      if ( g != f ) {
	cerr << "r0" << endl;
	display(r0, cerr);
	cerr << "r1" << endl;
	display(r1, cerr);
	cerr << "result" << endl;
	display(result, cerr);
	f.dump(cerr);
	g.dump(cerr);
	abort();
      }
    }
#endif
  }

  // pol の補正を行う．
  inv_edge(result, pol);

  return result;
}

// @brief varid という変数で r0 と r1 のDGをマージする．
tDgEdge
DgMgr::merge(tVarId varid,
	     tDgEdge r0,
	     tDgEdge r1)
{
#if defined(DG_PROFILE)
  mtimer.change(1);
#endif

  if ( debug == 1 ) {
    cerr << "Merge[" << varid << "](" << DispEdge(r0) << ", "
	 << DispEdge(r1) << ")" << endl;
    cerr << "r0(" << DispEdge(r0) << ")" << endl;
    display(r0, cerr);
    cerr << "r1(" << DispEdge(r1) << ")" << endl;
    display(r1, cerr);
  }

  // まずは trivial case
  tDgEdge result = kDgEdge0;
  if ( r0 == kDgEdge0 ) {
    if ( r1 == kDgEdge1 ) {
      result = make_literal(varid, kPolPosi);
    }
    else {
      result = make_litAnd(varid, kPolPosi, r1);
    }
  }
  else if ( r0 == kDgEdge1 ) {
    if ( r1 == kDgEdge0 ) {
      result = make_literal(varid, kPolNega);
    }
    else {
      result = make_litOr(varid, kPolNega, r1);
    }
  }
  else if ( r1 == kDgEdge0 ) {
    result = make_litAnd(varid, kPolNega, r0);
  }
  else if ( r1 == kDgEdge1 ) {
    result = make_litOr(varid, kPolPosi, r0);
  }
  else if ( r0 == inv_pol(r1) ) {
    result = make_litXor(varid, kPolPosi, r0);
  }
  if ( result != kDgEdge0 ) {
    if ( debug == 1 ) {
      disp_merge_end(varid, r0, r1, result, "Trivial case");
    }
    return result;
  }

#if defined(DG_PROFILE)
  mtimer.change(2);
#endif

  tPol pol0 = edge2pol(r0);
  tPol pol1 = edge2pol(r1);
  DgNode* v0 = edge2node(r0);
  DgNode* v1 = edge2node(r1);
  size_t n0 = v0->ni();
  size_t n1 = v1->ni();
  if ( v0->type() == Dg::kOr ) {
    for (size_t i = 0; i < n0; ++ i) {
      if ( inv_pol(v0->input(i), pol0) == r1 ) {
	// v0 が OR で，r1 を入力として持つ．
	result = case2or(varid, v0, pol0, i, r1, kPolNega);
	if ( debug == 1 ) {
	  disp_merge_end(varid, r0, r1, result, "Case2(OR)");
	}
	return result;
      }
    }
  }

  if ( v1->type() == Dg::kOr ) {
    for (size_t i = 0; i < n1; ++ i) {
      if ( inv_pol(v1->input(i), pol1) == r0 ) {
	// v1 が OR で，r0 を入力として持つ．
	result = case2or(varid, v1, pol1, i, r0, kPolPosi);
	if ( debug == 1 ) {
	  disp_merge_end(varid, r0, r1, result, "Case2(OR)");
	}
	return result;
      }
    }
  }

  if ( v0->type() == Dg::kXor ) {
    for (size_t i = 0; i < n0; ++ i) {
      if ( v0->input_node(i) == v1 ) {
	// v0 が XOR で，v1 を入力として持つ．
	// 極性の違いは問わない．
	result = case2xor(varid, v0, pol0, i, v1, pol1, kPolNega);
	if ( debug == 1 ) {
	  disp_merge_end(varid, r0, r1, result, "Case2(XOR)");
	}
	return result;
      }
    }
  }

  if ( v1->type() == Dg::kXor ) {
    for (size_t i = 0; i < n1; ++ i) {
      if ( v1->input_node(i) == v0 ) {
	// v1 が XOR で，v0 を入力として持つ．
	// 極性の違いは問わない．
	result = case2xor(varid, v1, pol1, i, v0, pol0, kPolPosi);
	if ( debug == 1 ) {
	  disp_merge_end(varid, r0, r1, result, "Case2(XOR)");
	}
	return result;
      }
    }
  }

  size_t nc = 0;
  int diff0 = -1, diff1 = -1;
  if ( v0->type() == v1->type() ) {
    // v0, v1 に共通な入力を求める．
    size_t i0 = 0;
    size_t i1 = 0;
    if ( n0 > 0 && n1 > 0 ) {
      tDgEdge ie0 = v0->input(i0);
      DgNode* iv0 = edge2node(ie0);
      tDgEdge ie1 = v1->input(i1);
      DgNode* iv1 = edge2node(ie1);
      for ( ; ; ) {
	int diff = iv0->top_var() - iv1->top_var();
	if ( diff < 0 ) {
	  diff0 = i0;
	  v0->input_flag(i0, 0);
	  ++ i0;
	  if ( i0 == n0 ) break;
	  ie0 = v0->input(i0);
	  iv0 = edge2node(ie0);
	}
	else if ( diff == 0 ) {
	  if ( ie0 == ie1 ) {
	    v0->input_flag(i0, 1);
	    v1->input_flag(i1, 1);
	    ++ i0;
	    ++ i1;
	    ++ nc;
	    if ( i0 == n0 || i1 == n1 ) break;
	    ie0 = v0->input(i0);
	    iv0 = edge2node(ie0);
	    ie1 = v1->input(i1);
	    iv1 = edge2node(ie1);
	  }
	  else {
	    diff0 = i0;
	    diff1 = i1;
	    v0->input_flag(i0, 0);
	    v1->input_flag(i1, 0);
	    ++ i0;
	    ++ i1;
	    if ( i0 == n0 || i1 == n1 ) break;
	    ie0 = v0->input(i0);
	    iv0 = edge2node(ie0);
	    ie1 = v1->input(i1);
	  iv1 = edge2node(ie1);
	  }
	}
	else {
	  diff1 = i1;
	  v1->input_flag(i1, 0);
	  ++ i1;
	  if ( i1 == n1 ) break;
	  ie1 = v1->input(i1);
	  iv1 = edge2node(ie1);
	}
      }
    }
    while ( i0 < n0 ) {
      diff0 = i0;
      v0->input_flag(i0, 0);
      ++ i0;
    }
    while ( i1 < n1 ) {
      diff1 = i1;
      v1->input_flag(i1, 0);
      ++ i1;
    }

#if defined(DG_PROFILE)
    mtimer.change(3);
#endif

    if ( nc >= 1 ) {
      if ( v0->type() == Dg::kXor ) {
	// r0, r1 がともに XOR で共通の入力を持つ．
	// OR と違って pol0 と pol1 が同じでなくても良い．
	result = case1xor(varid, v0, v1, pol0, pol1, nc);
	if ( debug == 1 ) {
	  disp_merge_end(varid, r0, r1, result, "Case1(XOR)");
	}
	return result;
      }
      else if ( pol0 == pol1 && v0->type() == Dg::kOr ) {
	// r0, r1 がともに OR で共通の入力を持つ．
	result = case1or(varid, v0, v1, pol0, nc);
	if ( debug == 1 ) {
	  disp_merge_end(varid, r0, r1, result, "Case1(OR)");
	}
	return result;
      }
    }
  }

#if defined(DG_PROFILE)
  mtimer.change(4);
#endif

  // ここまで来たら，グローバル関数は必ず必要になる．
  Bdd F0 = v0->global_func();
  if ( pol0 ) {
    F0.negate();
  }
  Bdd F1 = v1->global_func();
  if ( pol1 ) {
    F1.negate();
  }
  Bdd F = mMgr.make_bdd(varid, F0, F1);

  if ( mBidecomp ) {
    // 二項分解の時はここで終わり．
    BddVarSet support = v0->support() + v1->support() + BddVarSet(mMgr, varid);
    result = make_cplx(F, support);
    if ( debug == 1 ) {
      disp_merge_end(varid, r0, r1, result, "Case3(CPLX)(bidecomp)");
    }
    return result;
  }

  if ( !(v0->support() && v1->support()) ) {
    // v0 と v1 が disjoint なら話は簡単

    // 単純なので insert3() は用いない．
    vector<DgNode*> inputs;
    inputs.reserve(3);
    inputs.push_back(edge2node(make_literal(varid, kPolPosi)));
    if ( v0->top_var() < v1->top_var() ) {
      inputs.push_back(v0);
      inputs.push_back(v1);
    }
    else {
      inputs.push_back(v1);
      inputs.push_back(v0);
    }
    result = make_cplx(F, inputs);

    if ( debug == 1 ) {
      disp_merge_end(varid, r0, r1, result, "Case3(CPLX)(disjoint)");
    }

    return result;
  }

  if ( v0->type() == Dg::kCplx && v1->type() == Dg::kCplx ) {
    if ( n0 == nc + 1 && n1 == nc + 1 ) {
      // r0, r1 がともにCPLXでただ一つの入力以外が共通の場合

      // 異なっている入力を一致させると等しいか調べる．
      // コファクターを比較すれば良い．
      BddLitSet c0_0 = v0->input_node(diff0)->pat0();
      BddLitSet c0_1 = v0->input_node(diff0)->pat1();
      BddLitSet c1_0 = v1->input_node(diff1)->pat0();
      BddLitSet c1_1 = v1->input_node(diff1)->pat1();
      if ( check_with_assign(F0, c0_0, F1, c1_0) &&
	   check_with_assign(F0, c0_1, F1, c1_1) ) {
	// 一致した．
	result = case1cplx(F, varid, v0, v1, diff0, diff1, kPolPosi);
	if ( debug == 1 ) {
	  disp_merge_end(varid, r0, r1, result, "Case1(CPLX:POSI)");
	}
	return result;
      }
      else if ( check_with_assign(F0, c0_0, F1, c1_1) &&
		  check_with_assign(F0, c0_1, F1, c1_0) ) {
	// 逆に一致することもあり得る．
	result = case1cplx(F, varid, v0, v1, diff0, diff1, kPolNega);
	if ( debug == 1 ) {
	  disp_merge_end(varid, r0, r1, result, "Case1(CPLX::NEGA)");
	}
	return result;
      }
    }

#if defined(DG_PROFILE)
    mtimer.change(5);
#endif

    if ( n0 == nc && n1 == nc ) {
      // 非常にレアなケース
      // どれかの入力を反転すると一致する場合．
      size_t ni0 = v0->ni();
      for (size_t i = 0; i < ni0; ++ i) {
	DgNode* inode = v0->input_node(i);
	if ( inode->type() == Dg::kLitP ) {
	  // 外部入力の場合にはコファクターを見比べればよい．
	  tVarId varid = inode->literal_var();
	  Bdd f0_0 = F0.cofactor(varid, kPolNega);
	  Bdd f0_1 = F0.cofactor(varid, kPolPosi);
	  Bdd f1_0 = F1.cofactor(varid, kPolNega);
	  Bdd f1_1 = F1.cofactor(varid, kPolPosi);
	  if ( f0_0 != f1_1 || f0_1 != f1_0 ) {
	    continue;
	  }
	}
	else {
	  // 外部入力ではない入力に対しては一つづつ反転させてみるしかない．
	  BddLitSet c0 = inode->pat0();
	  BddLitSet c1 = inode->pat1();
	  if ( !check_with_assign(F0, c0, F1, c1) ||
	       !check_with_assign(F0, c1, F1, c0) ) {
	    continue;
	  }
	}
	// i番目の入力を反転させると一致した．
	result = case1cplx2(F, varid, v0, v1, i);
	if ( debug == 1 ) {
	  disp_merge_end(varid, r0, r1, result, "Case1(CPLX2)");
	}
	return result;
      }
    }
  }

#if defined(DG_PROFILE)
  mtimer.change(6);
#endif

  // ここまで来たら新しい CPLX ノードをつくるしかない．
  // まず，共通でない入力が片方のみにある場合のチェックを行う．
  BddVarSet sup0 = v0->support();
  BddVarSet sup1 = v1->support();
  BddVarSet sup0diff = sup0 - sup1;
  BddVarSet sup1diff = sup1 - sup0;
  if ( v0->type() == Dg::kCplx && !sup0diff.empty() && sup1diff.empty() ) {
    for (size_t i = 0; i < n0; ++ i) {
      DgNode* inode = v0->input_node(i);
      BddVarSet isup = inode->support();
      if ( !(sup1 && isup) ) {
	BddLitSet c0 = inode->pat0();
	if ( check_with_assign1(F0, c0, F1) ) {
	  result = case2cplx(F, varid, inode, kPolNega, kPolNega, v0);
	  if ( debug == 1 ) {
	    disp_merge_end(varid, r0, r1, result, "Case2(CPLX)");
	  }
	  return result;
	}
	BddLitSet c1 = inode->pat1();
	if ( check_with_assign1(F0, c1, F1) ) {
	  result = case2cplx(F, varid, inode, kPolPosi, kPolPosi, v0);
	  if ( debug == 1 ) {
	    disp_merge_end(varid, r0, r1, result, "Case2(CPLX)");
	  }
	  return result;
	}
      }
    }
  }
  if ( v1->type() == Dg::kCplx && !sup1diff.empty() && sup0diff.empty() ) {
    for (size_t i = 0; i < n1; ++ i) {
      DgNode* inode = v1->input_node(i);
      BddVarSet isup = inode->support();
      if ( !(sup0 && isup) ) {
	BddLitSet c0 = inode->pat0();
	if ( check_with_assign1(F1, c0, F0) ) {
	  result = case2cplx(F, varid, inode, kPolPosi, kPolNega, v1);
	  if ( debug == 1 ) {
	    disp_merge_end(varid, r0, r1, result, "Case2(CPLX)");
	  }
	  return result;
	}
	BddLitSet c1 = inode->pat1();
	if ( check_with_assign1(F1, c1, F0) ) {
	  result = case2cplx(F, varid, inode, kPolNega, kPolPosi, v1);
	  if ( debug == 1 ) {
	    disp_merge_end(varid, r0, r1, result, "Case2(CPLX)");
	  }
	  return result;
	}
      }
    }
  }

#if defined(DG_PROFILE)
  mtimer.change(7);
#endif

  if ( v0->is_all_literal() && v1->is_all_literal() ) {
    // どちらの入力もリテラルの時はBDDさえあれば良い．
    BddVarSet support = v0->support() + v1->support() + BddVarSet(mMgr, varid);
    result = make_cplx(F, support);
    if ( debug == 1 ) {
      disp_merge_end(varid, r0, r1, result, "Case3(CPLX)(all literals)");
    }
    return result;
  }

#if defined(DG_PROFILE)
  mtimer.change(8);
#endif
  
  // 共通の入力を求める．
  // OR/XOR ノードが部分的に共通な場合に注意．
  vector<DgNode*> inputs;
  inputs.reserve(mNi);
  insert3(inputs, edge2node(make_literal(varid, kPolPosi)));
  if ( v0->type() == Dg::kLitP ) {
    // v0 は無条件に境界となる．
    insert3(inputs, v0);
    // v1 の入力で，v0 と disjoint なものを求める．
    find_disjoint_node(v1, sup0, inputs);
  }
  else if ( v1->type() == Dg::kLitP ) {
    // v1 は無条件に境界となる．
    insert3(inputs, v1);
    // v0 の入力で，v1 と disjoint なものを求める．
    find_disjoint_node(v0, sup1, inputs);
  }
  else {
    v0->mark_recur(1);
    v1->mark_recur(2);
    v0->chdmark_recur();
    v1->chdmark_recur();
    vector<DgNode*> or_list0;
    or_list0.reserve(mNi);
    vector<DgNode*> xor_list0;
    xor_list0.reserve(mNi);
    v0->get_boundary(or_list0, xor_list0);
    vector<DgNode*> or_list1;
    or_list1.reserve(mNi);
    vector<DgNode*> xor_list1;
    xor_list1.reserve(mNi);
    v1->get_boundary(or_list1, xor_list1);
    find_uncommon_inputs(or_list0, 1, inputs);
    find_uncommon_inputs(or_list1, 2, inputs);
    find_uncommon_inputs(xor_list0, 1, inputs);
    find_uncommon_inputs(xor_list1, 2, inputs);
    find_common_inputs(or_list0, or_list1, inputs);
    find_common_inputs(xor_list0, xor_list1, inputs);
    list_boundary(v0, inputs);
    list_boundary(v1, inputs);
    v0->clear_mark();
    v1->clear_mark();
  }

#if defined(DG_PROFILE)
  mtimer.change(9);
#endif

  result = make_cplx(F, inputs);
  if ( debug == 1 ) {
    disp_merge_end(varid, r0, r1, result, "Case3(CPLX)");
  }

  return result;
}

// @brief 現在持っている Decomposition Graph を開放する．
void
DgMgr::clear()
{
  mNodeHash1.clear();
  mNodeHash2.clear();
}

// @brief 参照回数が0のBDDのエントリを削除する．
void
DgMgr::sweep()
{
  hash_map<tBddEdge, Dg>::iterator p = mNodeHash1.begin();
  while ( p != mNodeHash1.end() ) {
    hash_map<tBddEdge, Dg>::iterator tmp = p;
    ++ p;
    tBddEdge e = tmp->first;
    if ( mMgr.mPtr->check_noref(e) ) {
      mNodeHash1.erase(tmp);
    }
  }

  p = mNodeHash2.begin();
  while ( p != mNodeHash2.end() ) {
    hash_map<tBddEdge, Dg>::iterator tmp = p;
    ++ p;
    tBddEdge e = tmp->first;
    if ( mMgr.mPtr->check_noref(e) ) {
      mNodeHash2.erase(tmp);
    }
  }
}


// @brief r0, r1 がともに OR ノードで共通の入力を持つ場合の処理
tDgEdge
DgMgr::case1or(tVarId varid,
	       DgNode* v0,
	       DgNode* v1,
	       tPol pol0,
	       size_t nc)
{
#if defined(DG_PROFILE)
  ++ mProf.mCase1OrNum;
#endif

  size_t ni0 = v0->ni();
  size_t ni1 = v1->ni();
  size_t n0 = ni0 - nc;
  size_t n1 = ni1 - nc;
  // 新しい入力をつくる．
  tDgEdge new_input;
  if ( n0 == 0 ) {
    tDgEdge tmp = make_or(v1, 0);
    new_input = make_litAnd(varid, kPolPosi, tmp);
  }
  else if ( n1 == 0 ) {
    tDgEdge tmp = make_or(v0, 0);
    new_input = make_litAnd(varid, kPolNega, tmp);
  }
  else {
    tDgEdge tmp0 = make_or(v0, 0);
    tDgEdge tmp1 = make_or(v1, 0);

#if defined(DG_PROFILE)
    int old_id = mtimer.cur_id();
#endif

    // 再帰する．n0, n1 がともに1ならtmp0, tmp1 のタイプは
    // OR ではないので全くはじめからやり直す必要がある．
    new_input = merge(varid, tmp0, tmp1);

#if defined(DG_PROFILE)
    mtimer.change(old_id);
#endif
  }

  // 根本には共通な入力＋新しい入力の OR ノードをつくる．
  vector<tDgEdge> inputs;
  inputs.reserve(ni0 + 1);
  inputs.push_back(new_input);
  for (size_t i = 0; i < ni0; ++ i) {
    if ( v0->input_flag(i) == 1 ) {
      inputs.push_back(v0->input(i));
    }
  }
  tDgEdge result = make_or(inputs, pol0);
  return result;
}

// @brief v0, v1 がともに XOR ノードで共通の入力を持つ場合の処理
tDgEdge
DgMgr::case1xor(tVarId varid,
		DgNode* v0,
		DgNode* v1,
		tPol pol0,
		tPol pol1,
		size_t nc)
{
#if defined(DG_PROFILE)
  ++ mProf.mCase1XorNum;
#endif

  size_t ni0 = v0->ni();
  size_t ni1 = v1->ni();
  size_t n0 = ni0 - nc;
  size_t n1 = ni1 - nc;
  // 新しい入力をつくる．
  tDgEdge new_input;
  if ( n0 == 0 ) {
    tDgEdge tmp = make_xor(v1, 0, pol1);
    if ( pol0 == kPolPosi ) {
      new_input = make_litAnd(varid, kPolPosi, tmp);
    }
    else {
      new_input = make_litOr(varid, kPolNega, tmp);
    }
  }
  else if ( n1 == 0 ) {
    tDgEdge tmp = make_xor(v0, 0, pol0);
    if ( pol1 == kPolPosi ) {
      new_input = make_litAnd(varid, kPolNega, tmp);
    }
    else {
      new_input = make_litOr(varid, kPolPosi, tmp);
    }
  }
  else {
    tDgEdge tmp0 = make_xor(v0, 0, pol0);
    tDgEdge tmp1 = make_xor(v1, 0, pol1);

#if defined(DG_PROFILE)
    int old_id = mtimer.cur_id();
#endif

    // 再帰する．n0, n1 がともに1ならtmp0, tmp1 のタイプは
    // type と異なるので全くはじめからやり直す必要がある．
    new_input = merge(varid, tmp0, tmp1);

#if defined(DG_PROFILE)
    mtimer.change(old_id);
#endif
  }

  // 根本には共通な入力＋新しい入力の XOR ノードをつくる．
  vector<tDgEdge> inputs;
  inputs.reserve(ni0 + 1);
  inputs.push_back(new_input);
  for (size_t i = 0; i < ni0; ++ i) {
    if ( v0->input_flag(i) == 1 ) {
      inputs.push_back(v0->input(i));
    }
  }
  tDgEdge result = make_xor(inputs);
  return result;
}

// @brief v0 が OR で，v1 を入力として持つ．
tDgEdge
DgMgr::case2or(tVarId varid,
	       DgNode* v0,
	       tPol pol0,
	       size_t pos,
	       tDgEdge v1,
	       tPol ipol)
{
#if defined(DG_PROFILE)
  ++ mProf.mCase2OrNum;
#endif

  // 新しい入力を作る．
  size_t ni = v0->ni();
  tDgEdge tmp;
  if ( ni == 2 ) {
    int alt_pos = pos ^ 1;  // 0 なら 1，1 なら 0
    tmp = v0->input(alt_pos);
  }
  else {
    vector<tDgEdge> inputs;
    inputs.reserve(ni);
    for (size_t i = 0; i < ni; ++ i) {
      if ( i != pos ) {
	inputs.push_back(v0->input(i));
      }
    }
    tmp = make_or(inputs);
  }
  tDgEdge new_input = make_litAnd(varid, ipol, tmp);

  // それと v1 との OR を作る．
  tDgEdge result = make_or(new_input, inv_pol(v1, pol0), pol0);
  return result;
}

// @brief v0 が XOR で，v1 を入力として持つ．
tDgEdge
DgMgr::case2xor(tVarId varid,
		DgNode* v0,
		tPol pol0,
		size_t pos,
		DgNode* v1,
		tPol pol1,
		tPol ipol)
{
#if defined(DG_PROFILE)
  ++ mProf.mCase2XorNum;
#endif

  // 新しい入力を作る．
  size_t ni = v0->ni();
  tDgEdge tmp;
  if ( ni == 2 ) {
    int alt_pos = pos ^ 1;  // 0 なら 1，1 なら 0 
    tmp = v0->input(alt_pos);
    if ( pol0 == 1 ) {
      inv_edge(tmp);
    }
  }
  else {
    vector<tDgEdge> inputs;
    inputs.reserve(ni);
    for (size_t i = 0; i < ni; ++ i) {
      if ( i != pos ) {
	inputs.push_back(v0->input(i));
      }
    }
    tmp = make_xor(inputs, pol0);
  }
  tDgEdge new_input;
  if ( pol1 == kPolPosi ) {
    new_input = make_litAnd(varid, ipol, tmp);
  }
  else {
    new_input = make_litOr(varid, ~ipol, tmp);
  }

  // それと v1 との XOR を作る．
  tDgEdge result = make_xor(new_input, make_edge(v1));
  return result;
}

// @brief v0, v1 がともにCPLXノードでただ1組の入力を除いて
// @brief 共通な入力を持つ場合の処理．
tDgEdge
DgMgr::case1cplx(const Bdd& F,
		 tVarId varid,
		 DgNode* v0,
		 DgNode* v1,
		 size_t d0,
		 size_t d1,
		 tPol xpol)
{
#if defined(DG_PROFILE)
  ++ mProf.mCase1CplxNum;
#endif

  // 新しい入力をつくる．
  tDgEdge new_0 = v0->input(d0);
  tDgEdge new_1 = v1->input(d1);

#if defined(DG_PROFILE)
  int old_id = mtimer.cur_id();
#endif

  tDgEdge new_input = merge(varid, new_0, inv_pol(new_1, xpol));

#if defined(DG_PROFILE)
  mtimer.change(old_id);
#endif

  size_t ni0 = v0->ni();
  vector<DgNode*> inputs;
  inputs.reserve(ni0);
  insert3(inputs, edge2node(new_input));
  for (size_t i = 0; i < ni0; ++ i) {
    if ( v0->input_flag(i) == 1 ) {
      insert3(inputs, v0->input_node(i));
    }
  }
  tDgEdge result = make_cplx(F, inputs);
  return result;
}

// @brief v0, v1 がともにCPLXノードですべての入力が等しい場合の処理．
tDgEdge
DgMgr::case1cplx2(const Bdd& F,
		  tVarId varid,
		  DgNode* v0,
		  DgNode* v1,
		  size_t pos)
{
#if defined(DG_PROFILE)
  ++ mProf.mCase1Cplx2Num;
#endif

  // 新しい入力をつくる．
  tDgEdge iedge = v0->input(pos);
  tDgEdge new_input = make_litXor(varid, kPolPosi, iedge);

  size_t ni = v0->ni();
  vector<DgNode*> inputs;
  inputs.reserve(ni);
  insert3(inputs, edge2node(new_input));
  for (size_t i = 0; i < ni; ++ i) {
    if ( i != pos ) {
      insert3(inputs, v0->input_node(i));
    }
  }
  tDgEdge result = make_cplx(F, inputs);
  return result;
}

// @brief v0 の一部を0/1に固定してv1に一致できる場合の処理
tDgEdge
DgMgr::case2cplx(const Bdd& F,
		 tVarId varid,
		 DgNode* c_node,
		 tPol lit_pol,
		 int or_and,
		 DgNode* v)
{
#if defined(DG_PROFILE)
  ++ mProf.mCase2CplxNum;
#endif

  tDgEdge new_input;
  if ( or_and == 0 ) {
    new_input = make_litOr(varid, lit_pol, make_edge(c_node));
  }
  else {
    new_input = make_litAnd(varid, lit_pol, make_edge(c_node));
  }
  size_t ni = v->ni();
  vector<DgNode*> inputs;
  inputs.reserve(ni);
  insert3(inputs, edge2node(new_input));
  for (size_t i = 0; i < ni; ++ i) {
    DgNode* inode = v->input_node(i);
    if ( inode != c_node ) {
      insert3(inputs, inode);
    }
  }
  tDgEdge result = make_cplx(F, inputs);
  return result;
}

// @brief node もしくはそのTFIのうち，supportとdisjointなものを
// @brief inputs に入れる．
// OR/XORノードの場合には自動的に分割する．
void
DgMgr::find_disjoint_node(DgNode* node,
			  const BddVarSet& support,
			  vector<DgNode*>& inputs)
{
  if ( !(node->support() && support) ) {
    // node 自身が disjoint ならば node を inputs に入れる．
    insert3(inputs, node);
  }
  else if ( !(support >= node->support()) ) {
    size_t ni = node->ni();
    Dg::tType type = node->type();
    if ( type == Dg::kOr || type == Dg::kXor ) {
      vector<tDgEdge> others;
      others.reserve(ni);
      for (size_t i = 0; i < ni; ++ i) {
	DgNode* inode = node->input_node(i);
	if ( !(inode->support() && support) ) {
	  others.push_back(node->input(i));
	}
	else {
	  find_disjoint_node(inode, support, inputs);
	}
      }
      size_t no = others.size();
      if ( no >= 2 ) {
	tDgEdge edge = make_node(type, others);
	DgNode* o_node = edge2node(edge);
	insert3(inputs, o_node);
      }
      else if ( no == 1 ) {
	DgNode* o_node = edge2node(others[0]);
	insert3(inputs, o_node);
      }
    }
    else {
      for (size_t i = 0; i < ni; i ++) {
	DgNode* inode = node->input_node(i);
	find_disjoint_node(inode, support, inputs);
      }
    }
  }
}

void
DgMgr::find_uncommon_inputs(const vector<DgNode*>& list1,
			    int mark,
			    vector<DgNode*>& inputs)
{
  for (vector<DgNode*>::const_iterator it = list1.begin();
       it != list1.end(); ++ it) {
    DgNode* node = *it;
    size_t n = node->ni();
    vector<tDgEdge> tmp_inputs;
    tmp_inputs.reserve(n);
    for (size_t i = 0; i < n; ++ i) {
      tDgEdge iedge = node->input(i);
      DgNode* inode = edge2node(iedge);
      if ( inode->bmark() == 1 ) {
	if ( inode->mark() == mark ) {
	  tmp_inputs.push_back(iedge);
	}
      }
    }
    if ( tmp_inputs.size() > 1 ) {
      // 一方のみに現われる入力が2つ以上あればくくり出す．
      tDgEdge c_edge = make_node(node->type(), tmp_inputs);
      DgNode* c_node = edge2node(c_edge);
      insert3(inputs, c_node);
      size_t nuc = c_node->ni();
      for (size_t i = 0; i < nuc; ++ i) {
	// c_node の入力はもはや境界ノードではない．
	DgNode* inode = c_node->input_node(i);
	inode->bmark(3);
      }
    }
  }
}

// @brief 共通な入力を2つ以上持つノードを探して，共通部分を括り出す．
// 共通部分は inputs に追加する．
void
DgMgr::find_common_inputs(const vector<DgNode*>& list0,
			  const vector<DgNode*>& list1,
			  vector<DgNode*>& inputs)
{
  for (vector<DgNode*>::const_iterator it0 = list0.begin();
       it0 != list0.end(); ++ it0) {
    DgNode* node0 = *it0;
    size_t n0 = node0->ni();
    hash_set<tDgEdge> set0;
    for (size_t i = 0; i < n0; ++ i) {
      tDgEdge iedge = node0->input(i);
      DgNode* inode = edge2node(iedge);
      if ( inode->bmark() == 1 ) {
	if ( inode->mark() == 3 ) {
	  set0.insert(iedge);
	}
      }
    }
    for (vector<DgNode*>::const_iterator it1 = list1.begin();
	 it1 != list1.end(); ++ it1) {
      DgNode* node1 = *it1;
      size_t n1 = node1->ni();
      vector<tDgEdge> common;
      common.reserve(n1);
      for (size_t i = 0; i < n1; ++ i) {
	tDgEdge iedge1 = node1->input(i);
	DgNode* inode = edge2node(iedge1);
	if ( inode->bmark() == 1 ) {
	  if ( set0.count(iedge1) > 0 ) {
	    common.push_back(iedge1);
	  }
	}
      }
      size_t nc = common.size();
      if ( nc > 1 ) {
	// 共通部分が2つ以上あったのでくくり出す．
	DgNode* c_node;
	if ( nc == n0 ) {
	  // node0 の入力がすべて共通だった
	  c_node = node0;
	}
	else if ( nc == n1 ) {
	  // node1 の入力がすべて共通だった
	  c_node = node1;
	}
	else {
	  tDgEdge c_edge = make_node(node0->type(), common);
	  c_node = edge2node(c_edge);
	}
	insert3(inputs, c_node);
	for (size_t i = 0; i < nc; ++ i) {
	  // c_node の入力はもはや境界ノードではない．
	  DgNode* inode = c_node->input_node(i);
	  inode->bmark(3);
	}
      }
    }
  }
}

// @brief BMark に 1 の印が付いたノードを inputs に入れる．
// 一回処理したら mark を 3 にしておく．
void
DgMgr::list_boundary(DgNode* node,
		     vector<DgNode*>& inputs)
{
  int val = node->bmark();
  if ( val != 3 ) {
    if ( val == 1 ) {
      insert3(inputs, node);
    }
    else {
      size_t ni = node->ni();
      for (size_t i = 0; i < ni; ++ i) {
	DgNode* inode = node->input_node(i);
	list_boundary(inode, inputs);
      }
    }
    node->bmark(3);
  }
}

// @brief inputs に枝を追加する．
void
DgMgr::insert2(vector<tDgEdge>& inputs,
	       tDgEdge edge)
{
  if ( inputs.empty() ) {
    inputs.push_back(edge);
  }
  else {
    tVarId varid = edge2node(edge)->top_var();
    if ( varid < edge2node(inputs.front())->top_var() ) {
      inputs.insert(inputs.begin(), edge);
    }
    else {
      // 先頭は varid よりも小さいことが保証されている．
      vector<tDgEdge>::iterator p;
      for (p = inputs.end(); varid < edge2node(*-- p)->top_var(); ) ;
      inputs.insert(++ p, edge);
    }
  }
}

// @brief inputs にノードを追加する．
// 最初の要素は最小であることを仮定する．
void
DgMgr::insert3(vector<DgNode*>& inputs,
	       DgNode* node)
{
  if ( inputs.empty() ) {
    inputs.push_back(node);
  }
  else {
    tVarId varid = node->top_var();
    if ( varid < inputs.front()->top_var() ) {
      inputs.insert(inputs.begin(), node);
    }
    else {
      vector<DgNode*>::iterator p;
      for (p = inputs.end(); varid < (*-- p)->top_var(); ) ;
      inputs.insert(++ p, node);
    }
  }
}

// @brief 複数の入力を持つ OR ノードを作る．
// 入力は inputs を参照する．
// inputs[i] も OR ノードの場合には併合する．
tDgEdge
DgMgr::make_or(const vector<tDgEdge>& inputs,
	       tPol opol)
{
  size_t n = inputs.size();

  assert_cond(n > 0, __FILE__, __LINE__);

  if ( n == 1 ) {
    // 1入力ならノードをつくる必要はない．
    return inv_pol(inputs[0], opol);
  }

  // まずグローバル関数とサポートを求める．
  Bdd g = mMgr.make_zero();
  BddVarSet support(mMgr);
  vector<tDgEdge> inputs2;
  inputs2.reserve(mNi);
  for (size_t i = 0; i < n; ++ i) {
    tDgEdge iedge = inputs[i];
    DgNode* inode = edge2node(iedge);
    tPol ipol = edge2pol(iedge);
    if ( ipol == kPolPosi ) {
      g |= inode->global_func();
    }
    else {
      g |= ~inode->global_func();
    }
    support += inode->support();
    if ( ipol == kPolPosi && inode->type() == Dg::kOr ) {
      // inode の子供を inputs2 に追加する．
      size_t ni1 = inode->ni();
      for (size_t j = 0; j < ni1; ++ j) {
	insert2(inputs2, inode->input(j));
      }
    }
    else {
      // iedge を inputs2 に追加する．
      insert2(inputs2, iedge);
    }
  }

  tDgEdge result = new_edge(g, support, Dg::kOr, inputs2, opol);
  return result;
}

// @brief ノード v の部分的な入力を持つ OR ノードを作る．
// InputFlag が flag に一致するもののみを使う．
tDgEdge
DgMgr::make_or(DgNode* v,
	       int flag,
	       tPol opol)
{
  // まずグローバル関数とサポートを求める．
  Bdd g = mMgr.make_zero();
  BddVarSet support(mMgr);
  size_t n = v->ni();
  vector<tDgEdge> inputs;
  inputs.reserve(n);
  for (size_t i = 0; i < n; ++ i) {
    if ( v->input_flag(i) == flag ) {
      tDgEdge iedge = v->input(i);
      DgNode* inode = edge2node(iedge);
      if ( edge2pol(iedge) == kPolPosi ) {
	g |= inode->global_func();
      }
      else {
	g |= ~inode->global_func();
      }
      support += inode->support();
      inputs.push_back(iedge);
    }
  }

  size_t new_n = inputs.size();
  if ( new_n == 1 ) {
    // 1入力ならノードを新たに作る必要はない．
    return inv_pol(inputs[0], opol);
  }

  tDgEdge result = new_edge(g, support, Dg::kOr, inputs, opol);
  return result;
}

// @brief 複数の入力を持つ XOR ノードを作る．
// 入力は inputs を参照する．
// inputs[i] も XOR ノードの場合には併合する．
tDgEdge
DgMgr::make_xor(const vector<tDgEdge>& inputs,
		tPol opol)
{
  size_t n = inputs.size();

  assert_cond(n > 0, __FILE__, __LINE__);

  if ( n == 1 ) {
    // 1入力ならノードをつくる必要はない．
    return inv_pol(inputs[0], opol);
  }

  // まず，グローバル関数とサポートを求める．
  // ただし g は入力の否定枝を取り去って正規化したときの関数
  Bdd g = mMgr.make_zero();
  BddVarSet support(mMgr);
  vector<tDgEdge> inputs2;
  inputs2.reserve(mNi);
  // xpol は入力の枝の極性を集めたもの
  tPol xpol = kPolPosi;
  for (size_t i = 0; i < n; ++ i) {
    tDgEdge iedge = inputs[i];
    DgNode* inode = edge2node(iedge);
    tPol ipol = edge2pol(iedge);
    xpol = xpol * ipol;
    g ^= inode->global_func();
    support += inode->support();
    if ( inode->type() == Dg::kXor ) {
      // inode の子供を inputs2 に追加する．
      size_t ni1 = inode->ni();
      for (size_t j = 0; j < ni1; ++ j) {
	// XOR の子供なので極性は付いていないはず
	insert2(inputs2, inode->input(j));
      }
    }
    else {
      // iedge から極性を抜いたものを inputs2 に追加する．
      insert2(inputs2, make_edge(inode));
    }
  }

  tDgEdge result = new_edge(g, support, Dg::kXor, inputs2, xpol * opol);
  return result;
}

// @brief ノード v の部分的な入力を持つ XOR ノードを作る．
// InputFlag() が flag に一致するものを使う．
tDgEdge
DgMgr::make_xor(DgNode* v,
		int flag,
		tPol opol)
{
  // まず，グローバル関数とサポートを求める．
  Bdd g = mMgr.make_zero();
  BddVarSet support(mMgr);
  size_t n = v->ni();
  vector<tDgEdge> inputs;
  inputs.reserve(n);
  for (size_t i = 0; i < n; ++ i) {
    if ( v->input_flag(i) == flag ) {
      tDgEdge iedge = v->input(i);
      DgNode* inode = edge2node(iedge);
      g ^= inode->global_func();
      support += inode->support();
      inputs.push_back(iedge);
    }
  }

  size_t new_n = inputs.size();
  if ( new_n == 1 ) {
    return inv_pol(inputs[0], opol);
  }

  tDgEdge result = new_edge(g, support, Dg::kXor, inputs, opol);
  return result;
}

// @brief CPLX ノードを作る．
// 入力は inputs を参照する．
tDgEdge
DgMgr::make_cplx(const Bdd& f_orig,
		 const vector<DgNode*>& inputs,
		 tPol opol)
{
  Bdd f = f_orig;

  // BDDの根本に否定が付かないように正規化する．
  if ( get_pol(f.root()) == kPolNega ) {
    opol = ~opol;
    f.negate();
  }

  size_t ni = inputs.size();
  vector<tDgEdge> inputs2(ni);
  BddVarSet support(mMgr);
  for (size_t i = 0; i < ni; ++ i) {
    DgNode* inode = inputs[i];
    inputs2[i] = make_edge(inode);
    support += inode->support();
  }
  
  tDgEdge result = new_edge(f, support, Dg::kCplx, inputs2, opol);
  return result;
}

// @brief CPLX ノードを作る．
// こちらは入力がすべてリテラルの場合なので入力ノードの配列は要らない．
tDgEdge
DgMgr::make_cplx(const Bdd& f_orig,
		 const BddVarSet& support,
		 tPol opol)
{
  Bdd f = f_orig;

  // BDDの根本に否定が付かないように正規化する．
  if ( get_pol(f.root()) == kPolNega ) {
    opol = ~opol;
    f.negate();
  }

  vector<tDgEdge> inputs2;
  inputs2.reserve(support.size());
  for (BddVarSet::iterator p = support.begin(); p != support.end(); ++ p) {
    tVarId varid = *p;
    tDgEdge iedge = make_literal(varid, kPolPosi);
    inputs2.push_back(iedge);
  }

  tDgEdge result = new_edge(f, support, Dg::kCplx, inputs2, opol);
  return result;
}

// @brief 枝の先のノードの参照回数を一つ増やす
// ただし定数枝の場合にはノードは空なのでチェックする
void
DgMgr::inc_ref(tDgEdge root)
{
  DgNode* node = edge2node(root);
  if ( node ) {
    ++ node->mRefCount;
  }
}

// @brief 枝の先のノードの参照回数を一つ減らす
// ただし定数枝の場合にはノードは空なのでチェックする
void
DgMgr::dec_ref(tDgEdge root)
{
  DgNode* node = edge2node(root);
  if ( node ) {
    -- node->mRefCount;
    if ( node->mRefCount == 0 ) {
      for (size_t i = 0; i < node->mNi; ++ i) {
	dec_ref(node->mInputs[i]);
      }
      delete node;
    }
  }
}

// @brief デバッグ用の表示関数
void
disp_merge_end(tVarId varid,
	       tDgEdge r0,
	       tDgEdge r1,
	       tDgEdge result,
	       const char* case_str)
{
  cerr << "Merge[" << varid << "](" << DispEdge(r0)
       << ", " << DispEdge(r1) << ") end." << endl
       << case_str << " has been applied" << endl;
  display(result, cerr);
}

// @brief コンストラクタ
DgMgr::Profile::Profile()
{
  mCase1OrNum = 0;
  mCase1XorNum = 0;
  mCase1CplxNum = 0;
  mCase1Cplx2Num = 0;
  mCase2OrNum = 0;
  mCase2XorNum = 0;
  mCase2CplxNum = 0;
}

// @brief プロファイル情報を出力する．
void
DgMgr::Profile::display(ostream& s) const
{
  s << "Case1Or:    " << mCase1OrNum << " times" << endl
    << "Case1Xor:   " << mCase1XorNum << " times" << endl
    << "Case1Cplx:  " << mCase1CplxNum << " times" << endl
    << "Case1Cplx2: " << mCase1Cplx2Num << " times" << endl
    << "Case2Or:    " << mCase2OrNum << " times" << endl
    << "Case2Xor:   " << mCase2XorNum << " times" << endl
    << "Case2Cplx:  " << mCase2CplxNum << " times" << endl;

#if defined(DG_PROFILE)
  s << " others:      " << mtimer.time(0) << endl
    << " merge(1):    " << mtimer.time(1) << endl
    << " merge(2):    " << mtimer.time(2) << endl
    << " merge(3):    " << mtimer.time(3) << endl
    << " merge(4):    " << mtimer.time(4) << endl
    << " merge(5):    " << mtimer.time(5) << endl
    << " merge(6):    " << mtimer.time(6) << endl
    << " merge(7):    " << mtimer.time(7) << endl
    << " merge(8):    " << mtimer.time(8) << endl
    << " merge(9):    " << mtimer.time(9) << endl
    << " ???     :    " << mtimer.time(10) << endl
    << " merge(?):    " << mtimer.time(11) << endl
    << " decomp_step: " << mtimer.time(12) << endl;
#endif
}

END_NAMESPACE_YM_BDD
