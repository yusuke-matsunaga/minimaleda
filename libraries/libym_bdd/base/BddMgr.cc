
/// @file libym_bdd/base/BddMgr.cc
/// @brief BddMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: BddMgr.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2006 Yusuke Matsunaga
/// All rights reserved.


#if HAVE_CONFIG_H
#include <ymconfig.h>
#endif

#include <ym_utils/HeapTree.h>
#include <ym_bdd/Bdd.h>
#include <ym_bdd/BmcFactory.h>

#include "BddMgr.h"


BEGIN_NAMESPACE_YM_BDD

BEGIN_NONAMESPACE
// 入力数がこの値よりも小さかったらヒープ木は使わない．
const size_t kNiLimit = 8;
END_NONAMESPACE


//////////////////////////////////////////////////////////////////////
// BddMgrFactory
//////////////////////////////////////////////////////////////////////

BddMgrFactory::BddMgrFactory()
{
}

BddMgrFactory::~BddMgrFactory()
{
}


//////////////////////////////////////////////////////////////////////
// BddMgr
//////////////////////////////////////////////////////////////////////

BddMgr* BddMgr::mDefaultMgr = NULL;

// デフォルトマネージャを返すクラスメソッド
BddMgr*
BddMgr::default_mgr()
{
  if ( !mDefaultMgr ) {
    BmcFactory factory("default manager");
    mDefaultMgr = factory();
    assert_cond(mDefaultMgr, __FILE__, __LINE__);
    // 絶対に解放しない
    mDefaultMgr->inc_ref();
  }
  return mDefaultMgr;
}

// コンストラクタ
BddMgr::BddMgr()
{
  mRefCount = 0;

  // ログ出力用ストリームの初期化
  mNullStream = new ofstream("/dev/null", ios::out);
  if ( !mNullStream->is_open() ) {
    cerr << "Panic! : could not open \"/dev/null\"" << endl;
    abort();
  }
  mLogFp = mNullStream;

  // BDD リストの初期化
  mTopBdd = NULL;
}

// デストラクタ
BddMgr::~BddMgr()
{
  assert_cond(this != mDefaultMgr, __FILE__, __LINE__);

  // BDD の解放
  // と言っても Bdd のオブジェクトを削除するわけには行かないので
  // デフォルトマネージャのエラーBDDにすり替える．
  if ( mTopBdd ) {
    Bdd::change_mgr(mTopBdd, mDefaultMgr);
  }

  // /dev/null ストリームの破壊
  delete mNullStream;
}

// 参照回数を1増やす．
void
BddMgr::inc_ref()
{
  ++ mRefCount;
}

// 参照回数を1減らす．
// 参照回数が0になったら自分自身を破壊する．
void
BddMgr::dec_ref()
{
  -- mRefCount;
  if ( mRefCount == 0 ) {
    delete this;
  }
}

// log用ストリームを設定する．
void
BddMgr::set_logstream(ostream& s)
{
  mLogFp = &s;
}

// log用ストリームを解除する．
void
BddMgr::unset_logstream()
{
  mLogFp = mNullStream;
}

// log用ファイルポインタを読み出す．
ostream&
BddMgr::logstream() const
{
  return *mLogFp;
}

// LogExpr から BDD を作るためのサブルーティン
tBddEdge
BddMgr::expr_to_bdd(const LogExpr& expr,
		    const hash_map<tVarId, tBddEdge>& vemap)
{
  // 定数0の場合
  if ( expr.is_zero() ) {
    return kEdge0;
  }

  // 定数1の場合
  if ( expr.is_one() ) {
    return kEdge1;
  }

  // リテラルの場合
  if ( expr.is_literal() ) {
    tVarId pos = expr.varid();
    hash_map<tVarId, tBddEdge>::const_iterator p = vemap.find(pos);
    tBddEdge ans;
    if ( p == vemap.end() ) {
      ans = make_posiliteral(pos);
    }
    else {
      ans = p->second;
    }
    if ( expr.is_negaliteral() ) {
      return ans = negate(ans);
    }
    return ans;
  }

  // ここまで来たら根はAND/OR/XOR演算子
  size_t n = expr.child_num();
  list<tBddEdge> edge_list;
  for (size_t i = 0; i < n; ++ i) {
    tBddEdge e = expr_to_bdd(expr.child(i), vemap);
    edge_list.push_back(e);
  }
  if ( expr.is_and() ) {
    return and_op(edge_list);
  }
  if ( expr.is_or() ) {
    return or_op(edge_list);
  }
  if ( expr.is_xor() ) {
    return xor_op(edge_list);
  }
  assert_not_reached(__FILE__, __LINE__);
  return kEdgeError;
}

// ベクタを真理値表と見なしてBDDを作る．
// 個々の変数を vars で指定する．
// ベクタの値は非ゼロを true とみなす．
// v の大きさは 2^(vars.size()) に等しくなければならない．
tBddEdge
BddMgr::tvec_to_bdd(const vector<int>& v,
		    const VarVector& vars)
{
  size_t ni = vars.size();
  size_t nv = (1 << ni);
  if ( nv != v.size() ) {
    return kEdgeError;
  }

  // 変数の生成
  vector<tBddEdge> var_vector;
  for (VarVector::const_iterator p = vars.begin(); p != vars.end(); ++ p) {
    tVarId varid = *p;
    var_vector.push_back(make_posiliteral(varid));
  }
  size_t size = v.size();
  return tvec_sub(v, 0, size, var_vector, 0);
}

// 真理値表からBDDを作るためのサブルーティン
tBddEdge
BddMgr::tvec_sub(const vector<int>& v,
		 ymuint32 top,
		 ymuint32 size,
		 const vector<tBddEdge>& var_vector,
		 tVarId var_idx)
{
  assert_cond(size > 0, __FILE__, __LINE__);
  if ( size == 1 ) {
    if ( v[top] ) {
      return kEdge1;
    }
    else {
      return kEdge0;
    }
  }
  else {
    size_t size1 = size >> 1;
    tBddEdge l = tvec_sub(v, top, size1, var_vector, var_idx + 1);
    tBddEdge h = tvec_sub(v, top + size1, size1, var_vector, var_idx + 1);
    return ite_op(var_vector[var_idx], h, l);
  }
}

// edge_list に登録されたBDDのANDを計算する．
tBddEdge
BddMgr::and_op(const list<tBddEdge>& edge_list)
{
  size_t n = edge_list.size();
  tBddEdge ans;
  if ( n == 0 ) {
    ans = kEdge1;
  }
  else if ( n == 1 ) {
    ans = edge_list.front();
  }
  else if ( n < kNiLimit ) {
    list<tBddEdge>::const_iterator p = edge_list.begin();
    ans = *p;
    for (++ p; p != edge_list.end(); ++ p) {
      ans = and_op(ans, *p);
    }
  }
  else {
    SimpleHeapTree<tBddEdge> work;
    for (list<tBddEdge>::const_iterator p = edge_list.begin();
	 p != edge_list.end(); ++p) {
      tBddEdge e = *p;
      work.put(e, size(e));
    }
    ans = work.getmin();
    work.popmin();
    while ( !work.empty() ) {
      ans = and_op(ans, work.getmin());
      ans = work.xchgmin(ans, size(ans));
    }
  }
  return ans;
}

// edge_list に登録されたBDDのORを計算する．
tBddEdge
BddMgr::or_op(const list<tBddEdge>& edge_list)
{
  size_t n = edge_list.size();
  tBddEdge ans;
  if ( n == 0 ) {
    ans = kEdge0;
  }
  else if ( n == 1 ) {
    ans = edge_list.front();
  }
  else if ( n < kNiLimit ) {
    list<tBddEdge>::const_iterator p = edge_list.begin();
    ans = *p;
    for (++ p; p != edge_list.end(); ++ p) {
      ans = or_op(ans, *p);
    }
  }
  else {
    SimpleHeapTree<tBddEdge> work;
    for (list<tBddEdge>::const_iterator p = edge_list.begin();
	 p != edge_list.end(); ++p) {
      tBddEdge e = *p;
      work.put(e, size(e));
    }
    ans = work.getmin();
    work.popmin();
    while ( !work.empty() ) {
      ans = or_op(ans, work.getmin());
      ans = work.xchgmin(ans, size(ans));
    }
  }
  return ans;
}

// edge_list に登録されたBDDのANDを計算する．
tBddEdge
BddMgr::xor_op(const list<tBddEdge>& edge_list)
{
  size_t n = edge_list.size();
  tBddEdge ans;
  if ( n == 0 ) {
    ans = kEdge0;
  }
  else if ( n == 1 ) {
    ans = edge_list.front();
  }
  else if ( n < kNiLimit ) {
    list<tBddEdge>::const_iterator p = edge_list.begin();
    ans = *p;
    for (++ p; p != edge_list.end(); ++ p) {
      ans = xor_op(ans, *p);
    }
  }
  else {
    SimpleHeapTree<tBddEdge> work;
    for (list<tBddEdge>::const_iterator p = edge_list.begin();
	 p != edge_list.end(); ++p) {
      tBddEdge e = *p;
      work.put(e, size(e));
    }
    ans = work.getmin();
    work.popmin();
    while ( !work.empty() ) {
      ans = xor_op(ans, work.getmin());
      ans = work.xchgmin(ans, size(ans));
    }
  }
  return ans;
}

END_NAMESPACE_YM_BDD
