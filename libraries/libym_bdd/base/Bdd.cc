
/// @file libym_bdd/base/Bdd.cc
/// @brief Bdd の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: Bdd.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2006 Yusuke Matsunaga
/// All rights reserved.


#if HAVE_CONFIG_H
#include <ymconfig.h>
#endif

#include <ym_bdd/Bdd.h>
#include <ym_bdd/BddVarSet.h>
#include <ym_bdd/BddLitSet.h>

#include "BddMgr.h"
#include "Dumper.h"


BEGIN_NAMESPACE_YM_BDD

// @brief mgr, root をセットする時に呼ばれる関数
void
Bdd::set(BddMgr* mgr,
	 tBddEdge root)
{
  mMgr = mgr;
  mRoot = root;
  mMgr->inc_rootref(mRoot);

  // BDD のリストに追加
  Bdd* top = mMgr->mTopBdd;
  mNext = top;
  if ( top ) {
    top->mPrev = this;
  }
  mMgr->mTopBdd = this;
  mPrev = NULL;
}

// @brief (mMgr, mRoot) への参照をなくす時に呼ばれる関数
void
Bdd::reset()
{
  mMgr->dec_rootref(mRoot);

  // BDD のリストから削除
  Bdd* p = mPrev;
  Bdd* n = mNext;
  if ( p ) {
    p->mNext = n;
  }
  else {
    mMgr->mTopBdd = n;
  }
  if ( n ) {
    n->mPrev = p;
  }
}

// @brief 根の枝を new_e に置き換える．
void
Bdd::assign(tBddEdge new_e)
{
  mMgr->inc_rootref(new_e);
  mMgr->dec_rootref(mRoot);
  mRoot = new_e;
}

// @brief default_mgr に養子縁組を行う
void
Bdd::change_mgr(Bdd* top_bdd,
		BddMgr* default_mgr)
{
  Bdd* last = NULL;
  for (Bdd* bdd = top_bdd; bdd; bdd = bdd->mNext) {
    bdd->mRoot = kEdgeError;
    bdd->mMgr = default_mgr;
    last = bdd;
  }
  Bdd* first = default_mgr->mTopBdd;
  default_mgr->mTopBdd = top_bdd;
  top_bdd->mPrev = NULL;
  last->mNext = first;
  if ( first ) {
    first->mPrev = last;
  }
}

// @brief デフォルトのコンストラクタ
Bdd::Bdd()
{
  BddMgrRef defmgr;
  set(defmgr.mPtr, kEdge0);
}

// @brief コピーコンストラクタ
Bdd::Bdd(const Bdd& src)
{
  set(src.mMgr, src.mRoot);
}

// @brief BDD マネージャと根の枝を引数とするコンストラクタ
Bdd::Bdd(BddMgr* pmgr,
	 tBddEdge e)
{
  set(pmgr, e);
}

// @brief デストラクタ
Bdd::~Bdd()
{
  reset();
}

// @brief 代入演算子
const Bdd&
Bdd::operator=(const Bdd& src)
{
  if ( src.mMgr != mMgr ) {
    // マネージャも切り替える場合
    reset();
    set(src.mMgr, src.mRoot);
  }
  else {
    // 同じマネージャで根の枝だけ切り替える場合
    assign(src.mRoot);
  }
  return *this;
}

// @brief 否定を計算する．
Bdd
Bdd::operator~() const
{
  return Bdd(mMgr, negate_ifvalid(mRoot));
}

// @brief 否定の論理を計算して代入する．
const Bdd&
Bdd::negate()
{
  mRoot = negate_ifvalid(mRoot);
  return *this;
}

// @brief AND 付き代入
const Bdd&
Bdd::operator&=(const Bdd& src2)
{
  tBddEdge ans;
  if ( mMgr != src2.mMgr ) {
    // マネージャが異なる．
    ans = kEdgeError;
  }
  else {
    ans = mMgr->and_op(root(), src2.root());
  }
  assign(ans);
  return *this;
}

// @brief OR 付き代入
const Bdd&
Bdd::operator|=(const Bdd& src2)
{
  tBddEdge ans;
  if ( mMgr != src2.mMgr ) {
    // マネージャが異なる．
    ans = kEdgeError;
  }
  else {
    tBddEdge e1 = negate_ifvalid(root());
    tBddEdge e2 = negate_ifvalid(src2.root());
    ans = negate_ifvalid(mMgr->and_op(e1, e2));
  }
  assign(ans);
  return *this;
}

// @brief XOR 付き代入
const Bdd&
Bdd::operator^=(const Bdd& src2)
{
  tBddEdge ans;
  if ( mMgr != src2.mMgr ) {
    // マネージャが異なる．
    ans = kEdgeError;
  }
  else {
    ans = mMgr->xor_op(root(), src2.root());
  }
  assign(ans);
  return *this;
}

// @brief 共通部分のチェック
// @param[in] src2 オペランド
// @return 自分自身と src2 が共通部分を持つとき true を返す．
bool
Bdd::operator&&(const Bdd& src2) const
{
  if ( mMgr != src2.mMgr ) {
    // マネージャが異なる．
    return false;
  }
  return mMgr->check_intersect(root(), src2.root()) == kEdge1;
}

// @brief 包含関係のチェック
// @param[in] src2
// @return 自分自身が minterms の集合として src2 を含むとき true を返す．
bool
Bdd::operator>=(const Bdd& src2) const
{
  if ( mMgr != src2.mMgr ) {
    // マネージャが異なる．
    return false;
  }
  tBddEdge e1 = negate_ifvalid(root());
  tBddEdge e2 = src2.root();
  return mMgr->check_intersect(e1, e2) == kEdge0;
}

// @brief compose 演算
Bdd
Bdd::compose(tVarId var,
	     const Bdd& g) const
{
  tBddEdge ans;
  if ( mMgr != g.mMgr || g.is_error() ) {
    ans = kEdgeError;
  }
  else if ( g.is_overflow() ) {
    ans = kEdgeOverflow;
  }
  else {
    mMgr->compose_start();
    mMgr->compose_reg(var, g.root());
    ans = mMgr->compose(root());
  }
  return Bdd(mMgr, ans);
}

// @brief 複数変数の compose 演算
Bdd
Bdd::compose(const VarBddMap& comp_map) const
{
  // 始める前にエラーチェックを行う．
  for (VarBddMap::const_iterator p = comp_map.begin();
       p != comp_map.end(); ++ p) {
    Bdd bdd = p->second;
    if ( mMgr != bdd.mMgr || bdd.is_error() ) {
      return Bdd(mMgr, kEdgeError);
    }
    if ( bdd.is_overflow() ) {
      return Bdd(mMgr, kEdgeOverflow);
    }
  }
  
  mMgr->compose_start();
  for (VarBddMap::const_iterator p = comp_map.begin();
       p != comp_map.end(); ++ p) {
    tVarId id = p->first;
    Bdd bdd = p->second;
    mMgr->compose_reg(id, bdd.root());
  }
  tBddEdge ans = mMgr->compose(root());
  return Bdd(mMgr, ans);
}

// @brief 変数インデックスの置き換え
Bdd
Bdd::remap_var(const VarVarMap& var_map) const
{
  mMgr->compose_start();
  for (VarVarMap::const_iterator p = var_map.begin();
       p != var_map.end(); ++ p) {
    tVarId id = p->first;
    tVarId mid = p->second;
    Bdd bdd = mgr().make_posiliteral(mid);
    mMgr->compose_reg(id, bdd.root());
  }
  tBddEdge ans = mMgr->compose(root());
  return Bdd(mMgr, ans);
}

// @brief コファクター演算
Bdd
Bdd::cofactor(tVarId var,
	      tPol pol) const
{
  tBddEdge ans = mMgr->scofactor(root(), var, pol);
  return Bdd(mMgr, ans);
}

// @brief 複数変数のコファクター演算付き代入
const Bdd&
Bdd::operator/=(const BddLitSet& lit_set)
{
  return operator/=(lit_set.function());
}

// @brief generalized cofactor 演算
const Bdd&
Bdd::operator/=(const Bdd& c)
{
  tBddEdge ans;
  if ( mMgr != c.mMgr ) {
    // マネージャが異なる．
    ans = kEdgeError;
  }
  else {
    ans = mMgr->gcofactor(root(), c.root());
  }
  assign(ans);
  return *this;
}

// @brief Davio 展開のモーメント項を求める処理
Bdd
Bdd::xor_moment(tVarId idx) const
{
  tBddEdge ans = mMgr->xor_moment(root(), idx);
  return Bdd(mMgr, ans);
}

// @brief Smallest Cube Containg F を求める．
Bdd
Bdd::SCC() const
{
  tBddEdge ans = mMgr->SCC(root());
  return Bdd(mMgr, ans);
}

// @brief AND 演算
Bdd
operator&(const Bdd& src1,
	  const Bdd& src2)
{
  return Bdd(src1).operator&=(src2);
}

// @brief OR 演算
Bdd
operator|(const Bdd& src1,
	  const Bdd& src2)
{
  return Bdd(src1).operator|=(src2);
}

// @brief XOR 演算
Bdd
operator^(const Bdd& src1,
	  const Bdd& src2)
{
  return Bdd(src1).operator^=(src2);
}

// @brief 複数変数のコファクター演算
Bdd
operator/(const Bdd& src,
	  const BddLitSet& lit_set)
{
  return Bdd(src).operator/=(lit_set.function());
}

// @brief generalized cofactor 演算
Bdd
operator/(const Bdd& src,
	  const Bdd& c)
{
  return Bdd(src).operator/=(c);
}

// @brief 正リテラルのみのキューブの時，真となる．
bool
Bdd::is_posi_cube() const
{
  return mMgr->check_posi_cube(root());
}

// @brief キューブの時，真となる．
bool
Bdd::is_cube() const
{
  return mMgr->check_cube(root());
}

// @brief 対称性のチェック
bool
Bdd::check_symmetry(tVarId x,
		    tVarId y,
		    tPol pol) const
{
  if ( x == y ) {
    return pol == kPolPosi;
  }
#if defined(VERIFY_CHECK_SYMMETRY)
  bool result = mMgr->check_symmetry(root(), x, y, pol);
  Bdd f0 = cofactor(x, kPolNega);
  Bdd f1 = cofactor(x, kPolPosi);
  bool result2;
  if ( pol == kPolPosi ) {
    Bdd f2 = f0.cofactor(y, kPolPosi);
    Bdd f3 = f1.cofactor(y, kPolNega);
    result2 = f2 == f3;
  }
  else {
    Bdd f2 = f0.cofactor(y, kPolNega);
    Bdd f3 = f1.cofactor(y, kPolPosi);
    result2 = f2 == f3;
  }
  if ( result != result2 ) {
    cout << "ERROR @ check_symmetry("
	 << x << ", " << y << ", ";
    if ( pol == kPolPosi ) {
      cout << "positive";
    }
    else {
      cout << "negative";
    }
    cout << ")" << endl;
    //display(cout);
    cout << sop() << endl;
    cout << "expected result = ";
    if ( result2 ) {
      cout << "true";
    }
    else {
      cout << "false";
    }
    cout << endl;
  }
  return result2;
#else
  return mMgr->check_symmetry(root(), x, y, pol);
#endif
}

// @brief existential quntification(smoothing)
Bdd
Bdd::esmooth(const BddVarSet& svars) const
{
  Bdd sbdd = svars.function();
  tBddEdge ans;
  if ( mMgr != sbdd.mMgr ) {
    // マネージャが異なる．
    // 本当は異なってもいいね
    ans = kEdgeError;
  }
  else {
    ans = mMgr->esmooth(root(), sbdd.root());
  }
  return Bdd(mMgr, ans);
}

// @brief universal quantification
Bdd
Bdd::asmooth(const BddVarSet& svars) const
{
  Bdd sbdd = svars.function();
  tBddEdge ans;
  if ( mMgr != sbdd.mMgr ) {
    // マネージャが異なる．
    // 本当は異なってもいいね
    ans = kEdgeError;
  }
  else {
    ans = mMgr->esmooth(negate_ifvalid(root()), sbdd.root());
  }
  return Bdd(mMgr, negate_ifvalid(ans));
}

// @brief x_level の変数を y_level まで「押し込む」
Bdd
Bdd::push_down(tLevel x_level,
	       tLevel y_level,
	       tPol pol) const
{
  if ( x_level >= y_level ) {
    return Bdd(mMgr, kEdgeError);
  }
  tBddEdge ans = mMgr->push_down(root(), x_level, y_level, pol);
  return Bdd(mMgr, ans);
}

// @brief Shannon 展開 (Boole 展開) を行なう．
tVarId
Bdd::root_decomp(Bdd& f0,
		 Bdd& f1) const
{
  tBddEdge e0, e1;
  tVarId ans = mMgr->root_decomp(root(), e0, e1);
  f0 = Bdd(mMgr, e0);
  f1 = Bdd(mMgr, e1);
  return ans;
}

// @brief 根の変数番号を取り出す．
tVarId
Bdd::root_var() const
{
  return mMgr->root_var(root());
}

// @brief 0枝の取得
Bdd
Bdd::edge0() const
{
  tBddEdge ans = mMgr->edge0(root());
  return Bdd(mMgr, ans);
}

// @brief 1枝の取得
Bdd
Bdd::edge1() const
{
  tBddEdge ans = mMgr->edge1(root());
  return Bdd(mMgr, ans);
}

// @brief BDD の内容を積和形論理式に変換する．
LogExpr
Bdd::sop() const
{
  LogExpr ans_expr;
  mMgr->isop(root(), root(), ans_expr);
  return ans_expr;
}

// @brief 1パスの探索
BddLitSet
Bdd::onepath() const
{
  tBddEdge ans = mMgr->onepath(root());
  return BddLitSet(Bdd(mMgr, ans));
}

// @brief 最短の 1パスの探索
BddLitSet
Bdd::shortest_onepath() const
{
  tBddEdge ans = mMgr->shortest_onepath(root());
  return BddLitSet(Bdd(mMgr, ans));
}

// @brief 最短の 1パスの長さの取得
tVarSize
Bdd::shortest_onepath_len() const
{
  return mMgr->shortest_onepath_len(root());
}

// @brief BDD の内容を書き出す．
size_t
Bdd::display(ostream& s) const
{
  Displayer displayer(mMgr, s);
  displayer.display_root(root());
  return displayer.num();
}

// @brief BDD ベクタの内容を書き出す
// @param[in] array BDD ベクタ
// @param[in] s 出力ストリーム
size_t
display(const BddVector& array,
	ostream& s)
{
  if ( array.empty() ) {
    return 0;
  }
  // 今は array の中のBDDのマネージャがすべて同じと仮定している．
  BddMgr* mgr = array.front().mMgr;
  Displayer displayer(mgr, s);
  for (BddVector::const_iterator p = array.begin();
       p != array.end(); ++ p) {
    Bdd bdd = *p;
    displayer.display_root(bdd.root());
  }
  return displayer.num();
}

// @brief BDD リストの内容を書き出す
// @param[in] array BDD リスト
// @param[in] s 出力ストリーム
size_t
display(const BddList& array,
	ostream& s)
{
  if ( array.empty() ) {
    return 0;
  }
  // 今は array の中のBDDのマネージャがすべて同じと仮定している．
  BddMgr* mgr = array.front().mMgr;
  Displayer displayer(mgr, s);
  for (BddList::const_iterator p = array.begin();
       p != array.end(); ++ p) {
    Bdd bdd = *p;
    displayer.display_root(bdd.root());
  }
  return displayer.num();
}

// @brief 内容のダンプ
void
Bdd::dump(ostream& s) const
{
  Dumper dumper(mMgr, s);
  dumper.dump(root());
  dumper.dump_edge(root());
  s << endl;
}

// @brief BDD ベクタの内容をダンプする．
// @param[in] array BDD の配列
// @param[in] s 出力ストリーム
void
dump(const BddVector& array,
     ostream& s)
{
  if ( array.empty() ) {
    return;
  }
  // 今は array の中のBDDのマネージャがすべて同じと仮定している．
  BddMgr* mgr = array.front().mMgr;
  Dumper dumper(mgr, s);
  for (BddVector::const_iterator p = array.begin();
       p != array.end(); ++ p) {
    Bdd bdd = *p;
    dumper.dump(bdd.root());
  }
  for (BddVector::const_iterator p = array.begin();
       p != array.end(); ++ p) {
    Bdd bdd = *p;
    dumper.dump_edge(bdd.root());
    s << endl;
  }
}

// @brief BDD リストの内容をダンプする．
// @param[in] array BDD のリスト
// @param[in] s 出力ストリーム
void
dump(const BddList& array,
     ostream& s)
{
  if ( array.empty() ) {
    return;
  }
  // 今は array の中のBDDのマネージャがすべて同じと仮定している．
  BddMgr* mgr = array.front().mMgr;
  Dumper dumper(mgr, s);
  for (BddList::const_iterator p = array.begin();
       p != array.end(); ++ p) {
    Bdd bdd = *p;
    dumper.dump(bdd.root());
  }
  for (BddList::const_iterator p = array.begin();
       p != array.end(); ++ p) {
    Bdd bdd = *p;
    dumper.dump_edge(bdd.root());
    s << endl;
  }
}

// @brief ダンプされた情報を BDD を読み込む．
// @param[in] s 入力ストリーム
// @return 読み込まれた BDD
Bdd
BddMgrRef::restore(istream& s)
{
  Restorer restorer(mPtr, s);
  size_t n = restorer.read();
  if ( n != 1 ) {
    // エラーもしくは複数の BDD データだった．
    return Bdd(mPtr, kEdgeError);
  }
  else {
    return Bdd(mPtr, restorer.root(0));
  }
}

// @brief ダンプされた情報を BDD ベクタに読み込む．
// @param[in] s 入力ストリーム
// @param[in] array 読み込み先の BDD ベクタ
void
BddMgrRef::restore(istream& s,
		   BddVector& array)
{
  Restorer restorer(mPtr, s);
  size_t n = restorer.read();
  array.resize(n);
  for (size_t i = 0; i < n; ++ i) {
    array[i] = Bdd(mPtr, restorer.root(i));
  }
}

// @brief ダンプされた情報を BDD リストに読み込む．
// @param[in] s 入力ストリーム
// @param[in] array 読み込み先の BDD リスト
void
BddMgrRef::restore(istream& s,
		   BddList& array)
{
  Restorer restorer(mPtr, s);
  size_t n = restorer.read();
  for (size_t i = 0; i < n; ++ i) {
    array.push_back(Bdd(mPtr, restorer.root(i)));
  }
}

// @brief BDD が使っているノード数を数える．
size_t
Bdd::size() const
{
  return mMgr->size(root());
}

// @brief BDD ベクタが使っているノード数を数える．
// @param[in] array BDD ベクタ
// @return BDD ベクタが使っているノード数
size_t
size(const BddVector& array)
{
  if ( array.empty() ) {
    return 0;
  }
  list<tBddEdge> edge_list;
  for (BddVector::const_iterator p = array.begin();
       p != array.end(); ++ p) {
    Bdd bdd = *p;
    edge_list.push_back(bdd.root());
  }
  // 今は array の中のBDDのマネージャがすべて同じと仮定している．
  BddMgr* mgr = array.front().mMgr;
  return mgr->size(edge_list);
}

// @brief BDD リストが使っているノード数を数える．
// @param[in] array BDD リスト
// @return BDD リストが使っているノード数
size_t
size(const BddList& array)
{
  if ( array.empty() ) {
    return 0;
  }
  list<tBddEdge> edge_list;
  for (BddList::const_iterator p = array.begin();
       p != array.end(); ++ p) {
    Bdd bdd = *p;
    edge_list.push_back(bdd.root());
  }
  // 今は array の中のBDDのマネージャがすべて同じと仮定している．
  BddMgr* mgr = array.front().mMgr;
  return mgr->size(edge_list);
}
  
// @brief 真理値表密度の計算
// @param[in] n 入力数
double
Bdd::density(tVarSize n) const
{
  mpz_class mc = mMgr->minterm_count(root(), n);
  mpf_class mc_f(mc);
  mpz_class w = mpz_class(1) << n;
  mpf_class d = mc_f / w;
  return d.get_d();
}

// @brief 最小項の数の計算
mpz_class
Bdd::minterm_count(tVarSize n) const
{
  return mMgr->minterm_count(root(), n);
}

// @brief Walsh変換の 0次係数の計算
mpz_class
Bdd::walsh0(tVarSize n) const
{
  return mMgr->walsh0(root(), n);
}

// @brief Walsh変換の 1次係数の計算
mpz_class
Bdd::walsh1(tVarId var,
	    tVarSize n) const
{
  return mMgr->walsh1(root(), var, n);
}

// @brief サポート変数集合の計算 (VarVector)
tVarSize
Bdd::support(VarVector& vars) const
{
  mMgr->mark_support(root());
  return mMgr->mark_to_vector(vars);
}

// @brief サポート変数集合の計算 (VarList)
tVarSize
Bdd::support(VarList& vars) const
{
  mMgr->mark_support(root());
  return mMgr->mark_to_list(vars);
}

// @brief サポート変数集合の計算 (BddVarSet)
BddVarSet
Bdd::support() const
{
  mMgr->mark_support(root());
  tBddEdge ans = mMgr->mark_to_bdd();
  return BddVarSet(Bdd(mMgr, ans));
}

// @brief サポート変数集合の要素数の計算
tVarSize
Bdd::support_size() const
{
  return mMgr->mark_support(root());
}

// @brief BDD ベクタのサポート変数集合の計算 (VarVector)
// @param[in] bdd_array BDD のベクタ
// @param[in] sup サポート変数集合を格納するベクタ
// @return サポート変数集合の要素数
tVarSize
support(const BddVector& bdd_array,
	VarVector& sup)
{
  if ( bdd_array.empty() ) {
    sup.clear();
    return 0;
  }
  list<tBddEdge> edge_list;
  for (BddVector::const_iterator p = bdd_array.begin();
       p != bdd_array.end(); ++ p) {
    edge_list.push_back((*p).root());
  }
  // 今は手抜きで bdd_array 中の BDD のマネージャは全部同じと仮定している．
  BddMgr* mgr = bdd_array.front().mMgr;
  mgr->mark_support(edge_list);
  return mgr->mark_to_vector(sup);
}

// @brief BDD ベクタのサポート変数集合の計算 (VarList)
// @param[in] bdd_array BDD のベクタ
// @param[in] sup サポート変数集合を格納するリスト
// @return サポート変数集合の要素数
tVarSize
support(const BddVector& bdd_array,
	VarList& sup)
{
  if ( bdd_array.empty() ) {
    sup.clear();
    return 0;
  }
  list<tBddEdge> edge_list;
  for (BddVector::const_iterator p = bdd_array.begin();
       p != bdd_array.end(); ++ p) {
    edge_list.push_back((*p).root());
  }
  // 今は手抜きで bdd_array 中の BDD のマネージャは全部同じと仮定している．
  BddMgr* mgr = bdd_array.front().mMgr;
  mgr->mark_support(edge_list);
  return mgr->mark_to_list(sup);
}

// @brief BDD ベクタのサポート変数集合の計算 (BddVarSet)
// @param[in] bdd_array BDD のベクタ
// @return サポート変数集合
BddVarSet
support(const BddVector& bdd_array)
{
  if ( bdd_array.empty() ) {
    BddMgrRef defmgr;
    return BddVarSet(defmgr);
  }
  list<tBddEdge> edge_list;
  for (BddVector::const_iterator p = bdd_array.begin();
       p != bdd_array.end(); ++ p) {
    edge_list.push_back((*p).root());
  }
  // 今は手抜きで bdd_array 中の BDD のマネージャは全部同じと仮定している．
  BddMgr* mgr = bdd_array.front().mMgr;
  mgr->mark_support(edge_list);
  tBddEdge ans = mgr->mark_to_bdd();
  return BddVarSet(Bdd(mgr, ans));
}

// @brief BDD ベクタのサポート変数集合の要素数の計算
// @param[in] bdd_array BDD のベクタ
// @return サポート変数集合の要素数
tVarSize
support_size(const BddVector& bdd_array)
{
  if ( bdd_array.empty() ) {
    return 0;
  }
  list<tBddEdge> edge_list;
  for (BddVector::const_iterator p = bdd_array.begin();
       p != bdd_array.end(); ++ p) {
    edge_list.push_back((*p).root());
  }
  // 今は手抜きで bdd_array 中の BDD のマネージャは全部同じと仮定している．
  BddMgr* mgr = bdd_array.front().mMgr;
  return mgr->mark_support(edge_list);
}

// @brief BDD リストのサポート変数集合の計算 (VarVector)
// @param[in] bdd_array BDD のリスト
// @param[in] sup サポート変数集合を格納するベクタ
// @return サポート変数集合の要素数
tVarSize
support(const BddList& bdd_array,
	VarVector& sup)
{
  if ( bdd_array.empty() ) {
    sup.clear();
    return 0;
  }
  list<tBddEdge> edge_list;
  for (BddList::const_iterator p = bdd_array.begin();
       p != bdd_array.end(); ++ p) {
    edge_list.push_back((*p).root());
  }
  // 今は手抜きで bdd_array 中の BDD のマネージャは全部同じと仮定している．
  BddMgr* mgr = bdd_array.front().mMgr;
  mgr->mark_support(edge_list);
  return mgr->mark_to_vector(sup);
}

// @brief BDD リストのサポート変数集合の計算 (VarList)
// @param[in] bdd_array BDD のリスト
// @param[in] sup サポート変数集合を格納するリスト
// @return サポート変数集合の要素数
tVarSize
support(const BddList& bdd_array,
	VarList& sup)
{
  if ( bdd_array.empty() ) {
    sup.clear();
    return 0;
  }
  list<tBddEdge> edge_list;
  for (BddList::const_iterator p = bdd_array.begin();
       p != bdd_array.end(); ++ p) {
    edge_list.push_back((*p).root());
  }
  // 今は手抜きで bdd_array 中の BDD のマネージャは全部同じと仮定している．
  BddMgr* mgr = bdd_array.front().mMgr;
  mgr->mark_support(edge_list);
  return mgr->mark_to_list(sup);
}

// @brief BDD リストのサポート変数集合の計算 (BddVarSet)
// @param[in] bdd_array BDD のリスト
// @return[in] サポート変数集合
BddVarSet
support(const BddList& bdd_array)
{
  if ( bdd_array.empty() ) {
    BddMgrRef defmgr;
    return BddVarSet(defmgr);
  }
  list<tBddEdge> edge_list;
  for (BddList::const_iterator p = bdd_array.begin();
       p != bdd_array.end(); ++ p) {
    edge_list.push_back((*p).root());
  }
  // 今は手抜きで bdd_array 中の BDD のマネージャは全部同じと仮定している．
  BddMgr* mgr = bdd_array.front().mMgr;
  mgr->mark_support(edge_list);
  tBddEdge ans = mgr->mark_to_bdd();
  return BddVarSet(Bdd(mgr, ans));
}

// @brief BDD リストのサポート変数集合の要素数の計算
// @param[in] bdd_array BDD のリスト
// @return サポート変数集合の要素数
tVarSize
support_size(const BddList& bdd_array)
{
  if ( bdd_array.empty() ) {
    return 0;
  }
  list<tBddEdge> edge_list;
  for (BddList::const_iterator p = bdd_array.begin();
       p != bdd_array.end(); ++ p) {
    edge_list.push_back((*p).root());
  }
  // 今は手抜きで bdd_array 中の BDD のマネージャは全部同じと仮定している．
  BddMgr* mgr = bdd_array.front().mMgr;
  return mgr->mark_support(edge_list);
}

// @brief if-then-else 演算．
// @param[in] cond 条件を表す BDD
// @param[in] s 条件が成り立ったときに選ばれる関数
// @param[in] t 条件が成り立たなかったときに選ばれる関数
// @return if-then-else 演算の結果
Bdd
ite_op(const Bdd& cond,
       const Bdd& s,
       const Bdd& t)
{
  tBddEdge ans;
  if ( cond.mMgr != s.mMgr || cond.mMgr != t.mMgr ) {
    // マネージャが異なる．
    ans = kEdgeError;
  }
  else {
    ans = cond.mMgr->ite_op(cond.root(), s.root(), t.root());
  }
  return Bdd(cond.mMgr, ans);
}

// @brief AND-EXIST 演算
// @param[in] src1 第1オペランド
// @param[in] src2 第2オペランド
// @param[in] svars スムージング対象の変数の集合
// @return src1 と src2 の論理積を計算してムージングを行なった結果
Bdd
and_exist(const Bdd& src1,
	  const Bdd& src2,
	  const BddVarSet& svars)
{
  Bdd sbdd = svars.function();
  tBddEdge ans;
  if ( src1.mMgr != src2.mMgr || src1.mMgr != sbdd.mMgr ) {
    // マネージャが異なる．
    ans = kEdgeError;
  }
  else {
    ans = src1.mMgr->and_exist(src1.root(), src2.root(), sbdd.root());
  }
  return Bdd(src1.mMgr, ans);
}

// @brief 非冗長積和形表現の計算
// @param[in] lower 不完全指定論理関数の下限
// @param[in] upper 不完全指定論理関数の上限
// @param[in] cover 計算された非冗長積和形表現を格納する論理式
// @return 非冗長積和形の表している論理関数
Bdd
isop(const Bdd& lower,
     const Bdd& upper,
     LogExpr& cover)
{
  tBddEdge ans;
  if ( lower.mMgr != upper.mMgr ) {
    // マネージャが異なる．
    ans = kEdgeError;
  }
  else {
    ans = lower.mMgr->isop(lower.root(), upper.root(), cover);
  }
  return Bdd(lower.mMgr, ans);
}

// @brief prime cover の計算
// @param[in] lower 不完全指定論理関数の下限
// @param[in] upper 不完全指定論理関数の上限
// @return 計算された prime cover
LogExpr
prime_cover(const Bdd& lower,
	    const Bdd& upper)
{
  if ( lower.mMgr != upper.mMgr ) {
    // マネージャが異なる．
    return LogExpr();
  }
  return lower.mMgr->prime_cover(lower.root(), upper.root());
}

// @brief 極小サポート集合の計算
// @param[in] lower 不完全指定論理関数の下限
// @param[in] upper 不完全指定論理関数の上限
// @return 求まった極小サポートを表す BDD
Bdd
minimal_support(const Bdd& lower,
		const Bdd& upper)
{
  tBddEdge ans;
  if ( lower.mMgr != upper.mMgr ) {
    // マネージャが異なる．
    ans = kEdgeError;
  }
  else {
    ans = lower.mMgr->minimal_support(lower.root(), upper.root());
  }
  return Bdd(lower.mMgr, ans);
}

// @brief 変数集合としての共通部分の計算
// @param[in] src1 第オペランド
// @param[in] src2 第オペランド
// @return 変数集合としての共通部分を表す BDD
Bdd
vscap(const Bdd& src1,
      const Bdd& src2)
{
  tBddEdge ans;
  if ( src1.mMgr != src2.mMgr ) {
    // マネージャが異なる．
    ans = kEdgeError;
  }
  else {
    ans = src1.mMgr->vscap(src1.root(), src2.root());
  }
  return Bdd(src1.mMgr, ans);
}

// @brief 変数集合としての差集合の計算
// @param[in] src1 第オペランド
// @param[in] src2 第オペランド
// @return 変数集合としての差集合 (src1 - src2) を表す BDD
Bdd
vsdiff(const Bdd& src1,
       const Bdd& src2)
{
  tBddEdge ans;
  if ( src1.mMgr != src2.mMgr ) {
    // マネージャが異なる．
    ans = kEdgeError;
  }
  else {
    ans = src1.mMgr->vsdiff(src1.root(), src2.root());
  }
  return Bdd(src1.mMgr, ans);
}

// @brief 変数集合としての共通部分チェック
// @param[in] src1 第オペランド
// @param[in] src2 第オペランド
// @return 変数集合として共通部分をもつとき true を返す．
bool
vsintersect(const Bdd& src1,
	    const Bdd& src2)
{
  if ( src1.mMgr != src2.mMgr ) {
    // マネージャが異なる．
    return false;
  }
  return src1.mMgr->vsintersect(src1.root(), src2.root());
}

// @brief リテラル集合としての共通部分の計算
// @param[in] src1 第オペランド
// @param[in] src2 第オペランド
// @return リテラル集合としての共通部分を表す BDD
Bdd
lscap(const Bdd& src1,
      const Bdd& src2)
{
  tBddEdge ans;
  if ( src1.mMgr != src2.mMgr ) {
    // マネージャが異なる．
    ans = kEdgeError;
  }
  else {
    ans = src1.mMgr->lscap(src1.root(), src2.root());
  }
  return Bdd(src1.mMgr, ans);
}

// @brief リテラル集合としての差集合の計算
// @param[in] src1 第オペランド
// @param[in] src2 第オペランド
// @return リテラル集合としての差集合 (src1 - src2) を表す BDD
Bdd
lsdiff(const Bdd& src1,
       const Bdd& src2)
{
  tBddEdge ans;
  if ( src1.mMgr != src2.mMgr ) {
    // マネージャが異なる．
    ans = kEdgeError;
  }
  else {
    ans = src1.mMgr->lsdiff(src1.root(), src2.root());
  }
  return Bdd(src1.mMgr, ans);
}

// @brief リテラル集合としての共通部分チェック
// @param[in] src1 第オペランド
// @param[in] src2 第オペランド
// @return リテラル集合として共通部分をもつとき true を返す．
bool
lsintersect(const Bdd& src1,
	    const Bdd& src2)
{
  if ( src1.mMgr != src2.mMgr ) {
    // マネージャが異なる．
    return false;
  }
  return src1.mMgr->lsintersect(src1.root(), src2.root());
}

// @brief BddLitSet を表しているときに内容をベクタに変換する．
// @param[in] dst 結果を格納するベクタ
// @return 要素数
tVarSize
Bdd::to_literalvector(LiteralVector& dst) const
{
  return mMgr->to_literalvector(root(), dst);
}

// @brief BddLitSet を表しているときに内容をリストに変換する．
// @param[in] dst 結果を格納するリスト
// @return 要素数
tVarSize
Bdd::to_literallist(LiteralList& dst) const
{
  return mMgr->to_literallist(root(), dst);
}

// @brief 節点に n-mark を付け，各変数ごとにノード数を数える．
// @param[out] node_counts 変数番号をキーとして，
// その変数のノード数を格納する連想配列
void
Bdd::scan(hash_map<tVarId, size_t>& node_counts) const
{
  return mMgr->scan(root(), node_counts);
}

// @brief レベル level のノード数を数える．
// @param[in] level 変数のレベル
// @return level のノード数
// ただし n-mark が付いていないノードがあったら UINT_MAX を返す．
size_t
Bdd::count_at(tLevel level) const
{
  return mMgr->count_at(root(), level);
}

// @brief scan で付けた n-mark を消す．
void
Bdd::clear_scanmark() const
{
  mMgr->clear_scanmark(root());
}

// @brief カルノー図を描く
// @param[in] s 出力ストリーム
// 4入力以下した受け付けない
void
Bdd::display_map(ostream& s) const
{
  char val[16];
  bool flag = true;
  for (ymuint i = 0; i < 16; i ++) {
    tBddEdge e = root();
    for (ymuint j = 0; j < 4; j ++) {
      tBddEdge e0;
      tBddEdge e1;
      tVarId vid = mMgr->root_decomp(e, e0, e1);
      if ( vid == kVarIdMax ) break;
      if ( vid > j ) continue;
      if ( i & (1 << j) ) {
	e = e1;
      }
      else {
	e = e0;
      }
    }
    if ( check_zero(e) ) {
      val[i] = '0';
    }
    else if ( check_one(e) ) {
      val[i] = '1';
    }
    else {
      flag = false;
      break;
    }
  }
  if ( !flag ) {
    // 4変数ではなかった．
    s << "DumpMap: can't draw a map" << endl;
  }
  else {
    s << "   00 01 11 10" << endl;
    s << "00  " << val[0] << "  " << val[2] << "  "
      << val[3] << "  " << val[1] << endl;
    s << "01  " << val[8] << "  " << val[10] << "  "
      << val[11] << "  " << val[9] << endl;
    s << "11  " << val[12] << "  " << val[14] << "  "
      << val[15] << "  " << val[15] << endl;
    s << "10  " << val[4] << "  " << val[6] << "  "
      << val[7] << "  " << val[5] << endl;
  }
}

END_NAMESPACE_YM_BDD
