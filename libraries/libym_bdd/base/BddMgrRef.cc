
/// @file libym_bdd/base/BddMgrRef.cc
/// @brief BddMgrRef の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: BddMgrRef.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2006 Yusuke Matsunaga
/// All rights reserved.


#if HAVE_CONFIG_H
#include <ymconfig.h>
#endif

#include <ym_bdd/Bdd.h>

#include "BddMgr.h"


BEGIN_NAMESPACE_YM_BDD

//////////////////////////////////////////////////////////////////////
// BddMgrRef
//////////////////////////////////////////////////////////////////////

// デフォルトのマネージャを取り出すコンストラクタ
BddMgrRef::BddMgrRef() :
  mPtr(BddMgr::default_mgr())
{
  assert_cond(mPtr, __FILE__, __LINE__);
  mPtr->inc_ref();
}

// コンストラクタ
BddMgrRef::BddMgrRef(const BddMgrFactory& factory) :
  mPtr(factory())
{
  assert_cond(mPtr, __FILE__, __LINE__);
  mPtr->inc_ref();
}

// BddMgr を指定したコンストラクタ
BddMgrRef::BddMgrRef(BddMgr* ptr) :
  mPtr(ptr)
{
  assert_cond(mPtr, __FILE__, __LINE__);
  mPtr->inc_ref();
}

// コピーコンストラクタ
// 実際には同じ BddMgr を指すだけで BddMgr はコピーしない．
BddMgrRef::BddMgrRef(const BddMgrRef& src) :
  mPtr(src.mPtr)
{
  assert_cond(mPtr, __FILE__, __LINE__);
  mPtr->inc_ref();
}

// 代入演算子
// コピーコンストラクタと同様
const BddMgrRef&
BddMgrRef::operator=(const BddMgrRef& src)
{
  if ( mPtr != src.mPtr ) {
    assert_cond(mPtr, __FILE__, __LINE__);
    mPtr->dec_ref();
    mPtr = src.mPtr;
    assert_cond(mPtr, __FILE__, __LINE__);
    mPtr->inc_ref();
  }
  return *this;
}

// デストラクタ
BddMgrRef::~BddMgrRef()
{
  assert_cond(mPtr, __FILE__, __LINE__);
  mPtr->dec_ref();
}

// 2つのマネージャが等価なとき true を返す．
bool
BddMgrRef::operator==(const BddMgrRef& src2) const
{
  return mPtr == src2.mPtr;
}

// 2つのマネージャが等価でないとき true を返す．
bool
BddMgrRef::operator!=(const BddMgrRef& src2) const
{
  return !operator==(src2);
}

// 定数0を表すBDDを作る．
Bdd
BddMgrRef::make_zero()
{
  return Bdd(mPtr, kEdge0);
}

// 定数1を表すBDDを作る．
Bdd
BddMgrRef::make_one()
{
  return Bdd(mPtr, kEdge1);
}

// エラーを表すBDDを作る．
Bdd
BddMgrRef::make_error()
{
  return Bdd(mPtr, kEdgeError);
}

// オーバーフローを表すBDDを作る．
Bdd
BddMgrRef::make_overflow()
{
  return Bdd(mPtr, kEdgeOverflow);
}

// リテラル関数を表すBDDを作る．
Bdd
BddMgrRef::make_literal(tVarId index, tPol pol)
{
  tBddEdge ans = mPtr->make_literal(index, pol);
  return Bdd(mPtr, ans);
}

// リテラル関数を表すBDDを作る
Bdd
BddMgrRef::make_literal(const Literal& lit)
{
  tBddEdge ans = mPtr->make_literal(lit);
  return Bdd(mPtr,ans);
}

// 肯定のリテラル関数を作る
Bdd
BddMgrRef::make_posiliteral(tVarId varid)
{
  tBddEdge ans = mPtr->make_posiliteral(varid);
  return Bdd(mPtr, ans);
}

// 否定のリテラル関数を作る．
Bdd
BddMgrRef::make_negaliteral(tVarId varid)
{
  tBddEdge ans = mPtr->make_negaliteral(varid);
  return Bdd(mPtr, ans);
}

// 指定されたインデックスを持つ中間ノードを一つ持つBDDを作る
// その左右の子のデフォルトはそれぞれ0と1
// メモリ不足によってエラーが起きる可能性がある．
Bdd
BddMgrRef::make_bdd(tVarId index, const Bdd& chd0, const Bdd& chd1)
{
  tBddEdge ans = mPtr->make_bdd(index, chd0.root(), chd1.root());
  return Bdd(mPtr, ans);
}

// ベクタを真理値表と見なしてBDDを作る．
// 変数は 0 〜 (ni - 1) とみなす．
// ベクタの値は非ゼロを true とみなす．
// v の大きさは 2^ni に等しくなければならない．
Bdd
BddMgrRef::tvec_to_bdd(const vector<int>& v,
		       tVarSize ni)
{
  VarVector vv(ni);
  for (ymuint i = 0; i < ni; ++ i) {
    vv[i] = i;
  }
  return tvec_to_bdd(v, vv);
}

// ベクタを真理値表と見なしてBDDを作る．
// こちらは個々の変数を vars で指定したもの．
// ベクタの値は非ゼロを true とみなす．
// v の大きさは 2^(vars.size()) に等しくなければならない．
Bdd
BddMgrRef::tvec_to_bdd(const vector<int>& v,
		       const VarVector& vars)
{
  tBddEdge ans = mPtr->tvec_to_bdd(v, vars);
  return Bdd(mPtr, ans);
#if DEBUG_MAKE_BDD
  size_t ni = vars.size();
  size_t nv = (1 << ni);
  Bdd ans0(ans, mPtr);
  BddVector varbdds(ni);
  for (size_t i = 0; i < ni; ++ i) {
    tVarId vid = vars[i];
    varbdds[i] = make_posiliteral(vid);
  }
  
  Bdd ans1 = make_zero();
  for (size_t p = 0; p < nv; ++ p) {
    if ( v[p] ) {
      Bdd minterm = make_one();
      for (size_t i = 0; i < ni; ++ i) {
	if ( p & (1 << (ni - i - 1)) ) {
	  minterm &= varbdds[i];
	}
	else {
	  minterm &= ~varbdds[i];
	}
      }
      ans1 |= minterm;
    }
  }
  assert_cond(ans0 == ans1, __FILE__, __LINE__);
  return ans0;
#endif
}

// Expr から対応するBDDを作り出す．
// 論理式中に現れるリテラルを置き換えるBDDを varmap に入れる．
// varmap に登録されていないリテラルはそのまま使う．
Bdd
BddMgrRef::expr_to_bdd(const LogExpr& expr, const VarBddMap& varmap)
{
  hash_map<tVarId, tBddEdge> vemap;
  for (VarBddMap::const_iterator p = varmap.begin();
       p != varmap.end(); ++ p) {
    tVarId id = p->first;
    Bdd bdd = p->second;
    if ( mPtr != bdd.mMgr || bdd.is_error() ) {
      return make_error();
    }
    if ( bdd.is_overflow() ) {
      return make_overflow();
    }
    vemap.insert(make_pair(id, bdd.root()));
  }
  tBddEdge ans = mPtr->expr_to_bdd(expr, vemap);
  return Bdd(mPtr, ans);
}

// LogExpr から対応するBDDを作り出す．
// 論理式中に現れるリテラルを置き換えるリテラル番号を varmap に入れる．
// varmap に登録されていないリテラルはそのまま使う．
Bdd
BddMgrRef::expr_to_bdd(const LogExpr& expr, const VarVarMap& varmap)
{
  hash_map<tVarId, tBddEdge> vemap;
  for (VarVarMap::const_iterator p = varmap.begin();
       p != varmap.end(); ++ p) {
    tVarId id = p->first;
    tVarId id2 = p->second;
    tBddEdge tmp = mPtr->make_posiliteral(id2);
    vemap.insert(make_pair(id, tmp));
  }
  tBddEdge ans = mPtr->expr_to_bdd(expr, vemap);
  return Bdd(mPtr, ans);
}

// ストリームを読んで論理式を作り,そこからBDDを作る．
// エラーが起きたら err_msg をセットしてエラーBDDを返す．
Bdd
BddMgrRef::expr_to_bdd(istream& in, string& err_msg)
{
  LogExpr expr = LogExpr::stream_to_expr(in, err_msg);
  if ( err_msg == string() ) {
    return expr_to_bdd(expr);
  }
  return make_error();
}

// 文字列を読んで論理式を作り,そこからBDDを作る．
// エラーが起きたら err_msg をセットしてエラーBDDを返す．
Bdd
BddMgrRef::expr_to_bdd(const char* s, string& err_msg)
{
  istringstream in(s);
  return expr_to_bdd(in, err_msg);
}

// 文字列を読んで論理式を作り,そこからBDDを作る．
// エラーが起きたら err_msg をセットしてエラーBDDを返す．
Bdd
BddMgrRef::expr_to_bdd(const string& s, string& err_msg)
{
  istringstream in(s.c_str());
  return expr_to_bdd(in, err_msg);
}

BEGIN_NONAMESPACE
// make_thfunc() 中で使われるサブルーティン
inline
size_t
elem(size_t i, size_t j, size_t th)
{
  return i * (th + 1) + j;
}
END_NONAMESPACE

// 閾値関数を作る．
Bdd
BddMgrRef::make_thfunc(tVarSize n,
		       tVarSize th)
{
  vector<tBddEdge> table((n + 1) * (th + 1));
  table[elem(n, 0, th)] = kEdge1;
  for (ymuint i = 1; i <= th; i ++) {
    table[elem(n, i, th)] = kEdge0;
  }
  for (ymuint i = n; i -- > 0; ) {
    table[elem(i, 0, th)] = kEdge1;
    for (ymuint j = 1; j <= th; j ++) {
      if ( j > n - i ) {
	table[elem(i, j, th)] = kEdge0;
      }
      else {
	tBddEdge l = table[elem(i + 1, j, th)];
	tBddEdge h = table[elem(i + 1, j - 1, th)];
	tBddEdge tmp = mPtr->make_bdd(i, l, h);
	if ( check_overflow(tmp) ) {
	  return make_overflow();
	}
	table[elem(i, j, th)] = tmp;
      }
    }
  }
  tBddEdge ans = table[elem(0, th, th)];
  return Bdd(mPtr, ans);
}

// 複数のBDDの論理積を求める．
Bdd
BddMgrRef::and_op(const BddVector& bdds)
{
  list<tBddEdge> edge_list;
  for (BddVector::const_iterator p = bdds.begin(); p != bdds.end(); ++p) {
    edge_list.push_back((*p).root());
  }
  tBddEdge ans = mPtr->and_op(edge_list);
  return Bdd(mPtr, ans);
}

Bdd
BddMgrRef::and_op(const BddList& bdds)
{
  list<tBddEdge> edge_list;
  for (BddList::const_iterator p = bdds.begin(); p != bdds.end(); ++p) {
    edge_list.push_back((*p).root());
  }
  tBddEdge ans = mPtr->and_op(edge_list);
  return Bdd(mPtr, ans);
}

// 複数のBDDの論理和を求める．
Bdd
BddMgrRef::or_op(const BddVector& bdds)
{
  list<tBddEdge> edge_list;
  for (BddVector::const_iterator p = bdds.begin(); p != bdds.end(); ++p) {
    edge_list.push_back((*p).root());
  }
  tBddEdge ans = mPtr->or_op(edge_list);
  return Bdd(mPtr, ans);
}

Bdd
BddMgrRef::or_op(const BddList& bdds)
{
  list<tBddEdge> edge_list;
  for (BddList::const_iterator p = bdds.begin(); p != bdds.end(); ++p) {
    edge_list.push_back((*p).root());
  }
  tBddEdge ans = mPtr->or_op(edge_list);
  return Bdd(mPtr, ans);
}

// 複数のBDDの排他的論理和を求める．
Bdd
BddMgrRef::xor_op(const BddVector& bdds)
{
  list<tBddEdge> edge_list;
  for (BddVector::const_iterator p = bdds.begin(); p != bdds.end(); ++p) {
    edge_list.push_back((*p).root());
  }
  tBddEdge ans = mPtr->xor_op(edge_list);
  return Bdd(mPtr, ans);
}

Bdd
BddMgrRef::xor_op(const BddList& bdds)
{
  list<tBddEdge> edge_list;
  for (BddList::const_iterator p = bdds.begin(); p != bdds.end(); ++p) {
    edge_list.push_back((*p).root());
  }
  tBddEdge ans = mPtr->xor_op(edge_list);
  return Bdd(mPtr, ans);
}

// 変数を確保する．
// 確保に失敗したら false を返す．
// 最後の変数の後ろに挿入される．
bool
BddMgrRef::new_var(tVarId varid)
{
  return mPtr->new_var(varid);
}

// 現在登録されている変数をそのレベルの昇順で返す．
tVarSize
BddMgrRef::var_list(list<tVarId>& vlist) const
{
  return mPtr->var_list(vlist);
}

// 変数番号からレベルを得る．
// もしもレベルが割り当てられていない場合にはエラーとなる．
tLevel
BddMgrRef::level(tVarId varid) const
{
  return mPtr->level(varid);
}

// レベルから変数番号を得る．
tVarId
BddMgrRef::varid(tLevel level) const
{
  return mPtr->varid(level);
}
  
// 動的変数順変更を許可する．
void
BddMgrRef::enable_DVO()
{
  mPtr->enable_DVO();
}

// 動的変数順変更を禁止する．
void
BddMgrRef::disable_DVO()
{
  mPtr->disable_DVO();
}

// ガーベージコレクションを行なう．
// 具体的には各ノードの参照回数が0のノードをフリーリストに回収し
// 再利用できるよ うにする．
// その際，それらのノードに関係した演算結果テーブルの内容はクリアされる．
// shrink_nodetable = true の時, 可能なら節点テーブルのサイズを縮小する．
void
BddMgrRef::gc(bool shrink_nodetable)
{
  mPtr->gc(shrink_nodetable);
}

// GC 前の sweep 処理を行うためのバインダーを登録する．
void
BddMgrRef::reg_sweep_binder(EventBinder* binder)
{
  mPtr->reg_sweep_binder(binder);
}
  
// ログ出力用のストリームを設定する．
void
BddMgrRef::set_logstream(ostream& s)
{
  mPtr->set_logstream(s);
}

// ログ出力用のストリームを解除する．
void
BddMgrRef::unset_logstream()
{
  mPtr->unset_logstream();
}
  
// ログ出力用のストリームを得る．
ostream&
BddMgrRef::logstream() const
{
  return mPtr->logstream();
}

// パラメータを設定する．設定したい項目のマスクビットを1とする．
void
BddMgrRef::param(const BddMgrParam& param,
		 ymuint32 mask)
{
  mPtr->param(param, mask);
}
  
// パラメータを取得する．
void
BddMgrRef::param(BddMgrParam& param) const
{
  mPtr->param(param);
}

// 名前を得る．
const string&
BddMgrRef::name() const
{
  return mPtr->name();
}

// 使用メモリ量(in bytes)を得る．
size_t
BddMgrRef::used_mem() const
{
  return mPtr->used_mem();
}
  
// 節点テーブルに登録されているノードの数を得る．
size_t
BddMgrRef::node_num() const
{
  return mPtr->node_num();
}
  
// GC で回収される(フリーになる)ノード数を得る．
size_t
BddMgrRef::garbage_num() const
{
  return mPtr->garbage_num();
}
  
// 利用可能なフリーノード数を得る．
size_t
BddMgrRef::avail_num() const
{
  return mPtr->avail_num();
}
  
// GC の起動された回数を得る．
size_t
BddMgrRef::gc_count() const
{
  return mPtr->gc_count();
}

END_NAMESPACE_YM_BDD
