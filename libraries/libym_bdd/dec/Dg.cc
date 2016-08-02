
/// @file libym_bdd/dec/Dg.cc
/// @brief Dg の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: Dg.cc 700 2007-05-31 00:41:30Z matsunaga $
///
/// Copyright (C) 2005-2006 Yusuke Matsunaga
/// All rights reserved.


#if HAVE_CONFIG_H
#include <ymconfig.h>
#endif

#include <ym_bdd/Dg.h>

#include "DgNode.h"


BEGIN_NAMESPACE_YM_BDD

// @brief 空のコンストラクタ
// 定数0のノードをセットしておく
Dg::Dg()
{
  mRoot = kDgEdge0;
  mMgr = NULL;
}

// @brief 根を指定するコンストラクタ
// @param[in] root 根の枝
// @param[in] mgr DGマネージャ
Dg::Dg(tDgEdge root,
       DgMgr* mgr)
{
  assert_cond(mgr, __FILE__, __LINE__);
  mRoot = root;
  mMgr = mgr;
  mMgr->inc_ref(mRoot);
}

// @brief コピーコンストラクタ
// @param[in] src ソースノード
Dg::Dg(const Dg& src)
{
  mRoot = src.mRoot;
  mMgr = src.mMgr;
  if ( mMgr ) {
    mMgr->inc_ref(mRoot);
  }
}

// @brief 代入演算子
// @param[in] src ソースノード
// @return 自分自身を返す．
const Dg&
Dg::operator=(const Dg& src)
{
  if ( &src != this ) {
    mMgr->dec_ref(mRoot);
    mRoot = src.mRoot;
    mMgr = src.mMgr;
    if ( mMgr ) {
      mMgr->inc_ref(mRoot);
    }
  }
  return *this;
}

// @brief デストラクタ
Dg::~Dg()
{
  if ( mMgr ) {
    mMgr->dec_ref(mRoot);
  }
}

// @brief non-trivial decomposition のチェック
// @return non-trivial decomposition を持つとき true を返す．
bool
Dg::has_decomp() const
{
  DgNode* node = edge2node(mRoot);
  if ( node == 0 ) {
    // 例外: 定数関数は分解を持つと見なす．
    return true;
  }
  if ( node->type() != Dg::kCplx ) {
    return true;
  }
  if ( !node->is_all_literal() ) {
    return true;
  }
  return false;
}

// @brief CBF のチェック
// @return e が CBF の時 true を返す
bool
Dg::is_CBF() const
{
  DgNode* node = edge2node(mRoot);
  if ( node == 0 ) {
    // 例外: 定数関数は分解を持つと見なす．
    return true;
  }
  if ( node->type() == Dg::kCplx ) {
    return false;
  }
  for (size_t i = 0; i < node->ni(); ++ i) {
    if ( !(input(i).is_CBF()) ) {
      return false;
    }
  }
  return true;
}

// @brief 根の分解のタイプを返す．
// @return 根の分解のタイプ
Dg::tType
Dg::type() const
{
  DgNode* node = edge2node(mRoot);
  tPol pol = edge2pol(mRoot);
  tType type = node->type();
  if ( pol == kPolNega && type != kCplx ) {
    type = tType(int(type) | 1);
  }
  return type;
}

// @brief 2項分解のチェック
// @return 根の分解のタイプが2項分解のときに true を返す．
bool
Dg::is_bidecomp() const
{
  DgNode* node = edge2node(mRoot);
  return node->is_bidecomp();
}

// @brief 根の分解の入力数の取得
// @return 根の分解の入力数
size_t
Dg::ni() const
{
  DgNode* node = edge2node(mRoot);
  return node->ni();
}

// @brief 根の分解の入力の取得
// @param[in] pos 取得する入力の位置 (最初の位置は0)
// @param[in] 根の分解の pos 番目の入力の分解を得る．
Dg
Dg::input(size_t pos) const
{
  DgNode* node = edge2node(mRoot);
  tDgEdge input = node->input(pos);
  return Dg(input, mMgr);
}

// @brief global function を得る．
// @return global function
Bdd
Dg::global_func() const
{
  DgNode* node = edge2node(mRoot);
  tPol pol = edge2pol(mRoot);
  return pol == kPolPosi ? node->global_func() : ~node->global_func();
}

// @brief 入力を iset のみに制限した場合の global function を求める．
// @param[in] iset 入力番号のリスト
// @return 入力を iset のみに制限した場合の global function を求める
Bdd
Dg::partial_func(const vector<size_t>& iset) const
{
  DgNode* node = edge2node(mRoot);
  tPol pol = edge2pol(mRoot);
  Bdd ans = node->partial_func(iset);
  return pol == kPolPosi ? ans : ~ans;
}

// @brief サポートを得る
// @return サポート
BddVarSet
Dg::support() const
{
  DgNode* node = edge2node(mRoot);
  return node->support();
}

// @brief サポートのサイズを得る．
// @return サポートのサイズ
size_t
Dg::support_size() const
{
  DgNode* node = edge2node(mRoot);
  return node->support_size();
}

// @brief 直交分解の列挙
// @param[in] llimit 入力数の下限
// @param[in] ulimit 入力数の上限
// @param[out] dec_list 求まった直交分解を入れるリスト
//
// 直交関数分解のうち，bound-set の入力数が llimit 以上 ulimit 以下
// のものを列挙する．
void
Dg::enum_djdec(size_t llimit,
	       size_t ulimit,
	       PrimDecList& dec_list) const
{
  ed_recur(llimit, ulimit, global_func().support(), dec_list);

  // 特例: bound-set が空集合の場合
  BddMgrRef mgr = global_func().mgr();
  BddVarSet fset = support();
  dec_list.push_back(PrimDec(BddVarSet(mgr), support(), mgr.make_zero()));
}

// @brief enum_djdec のための再帰関数
// @param[in] llimit 入力数の下限
// @param[in] ulimit 入力数の上限
// @param[in] all_sup もとの関数のサポート
// @param[out] dec_list 求まった直交分解を入れるリスト
void
Dg::ed_recur(size_t llimit,
	     size_t ulimit,
	     const BddVarSet& all_sup,
	     PrimDecList& dec_list) const
{
  size_t s = support_size();
  if ( llimit <= s && s <= ulimit ) {
    // 自分自身を登録する．
    BddVarSet fset = all_sup - support();
    Bdd bfunc = global_func();
    dec_list.push_back(PrimDec(support(), fset, bfunc));
  }

  if ( llimit < s ) {
    if ( is_bidecomp() ) {
      // 子供の部分集合を登録する．
      size_t nexp = (1 << ni()) - 1;
      // 0 と (1 << ni) - 1 を含めていないのは
      // 空集合と全集合を除外するため．
      BddMgrRef mgr = global_func().mgr();
      for (size_t p = 1; p < nexp; ++ p) {
	size_t n = 0;
	BddVarSet tmp_support(mgr);
	vector<size_t> iset;
	for (size_t i = 0; i < ni(); ++ i) {
	  if (p & (1 << i)) {
	    ++ n;
	    tmp_support += input(i).support();
	    iset.push_back(i);
	  }
	}
	// n == 1 の時は下の for ループで再帰したときに処理されるので
	// ここでは処理しない．
	size_t s = tmp_support.size();
	if ( n > 1 && llimit <= s && s <= ulimit ) {
	  BddVarSet fset = all_sup - tmp_support;
	  Bdd bfunc = partial_func(iset);
	  dec_list.push_back(PrimDec(tmp_support, fset, bfunc));
	}
      }
    }

    // (どのタイプのノードでも)子供に対して再帰する．
    for (size_t i = 0; i < ni(); i ++) {
      input(i).ed_recur(llimit, ulimit, all_sup, dec_list);
    }
  }
}

// @brief 内容を出力する．
// @param[in] s 出力ストリーム
void
Dg::display(ostream& s) const
{
  nsYm::nsBdd::display(mRoot, s);
}

END_NAMESPACE_YM_BDD
