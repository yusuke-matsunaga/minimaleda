
/// @file libym_bdd/base/BddVarSet.cc
/// @brief BddVarSet の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: BddVarSet.cc 700 2007-05-31 00:41:30Z matsunaga $
///
/// Copyright (C) 2005-2006 Yusuke Matsunaga
/// All rights reserved.


#if HAVE_CONFIG_H
#include <ymconfig.h>
#endif

#include <ym_bdd/BddVarSet.h>


BEGIN_NAMESPACE_YM_BDD

//////////////////////////////////////////////////////////////////////
// BddVarSet::iterator
//////////////////////////////////////////////////////////////////////

// コンストラクタ
BddVarSet::iterator::iterator()
{
}

// BddVarSet 用のコンストラクタ
BddVarSet::iterator::iterator(const Bdd& bdd) :
  mCur(bdd)
{
}
    
// 現在指している要素を返す．
// 末尾かどうかのチェックは行わない．
tVarId
BddVarSet::iterator::operator*() const
{
  return mCur.root_var();
}

// ひとつ先に進める．
// 自分自身を返す．
BddVarSet::iterator&
BddVarSet::iterator::operator++()
{
  mCur = mCur.edge1();
  return *this;
}

// ひとつ先に進める．
// 進める前の値を返す．
BddVarSet::iterator
BddVarSet::iterator::operator++(int)
{
  iterator old = *this;
  mCur = mCur.edge1();
  return old;
}

// 等価比較
bool
BddVarSet::iterator::operator==(const iterator& src) const
{
  return mCur == src.mCur;
}
bool
BddVarSet::iterator::operator!=(const iterator& src) const
{
  return !operator==(src);
}


//////////////////////////////////////////////////////////////////////
// BddVarSet
//////////////////////////////////////////////////////////////////////

// 空のコンストラクタ
// 空集合となるが，明示的に使う場合には下の make_empty() を使うべき
BddVarSet::BddVarSet(BddMgrRef mgr) :
  mBody(mgr.make_one()),
  mSize(0U)
{
  set_size(0);
}

// BDD を指定するコンストラクタ
BddVarSet::BddVarSet(const Bdd& bdd) :
  mBody(bdd),
  mSize(0U)
{
}

// コピーコンストラクタ
BddVarSet::BddVarSet(const BddVarSet& src) :
  mBody(src.mBody),
  mSize(src.mSize)
{
}

// 代入演算子
const BddVarSet&
BddVarSet::operator=(const BddVarSet& src)
{
  mBody = src.mBody;
  mSize = src.mSize;
  return *this;
}

// 変数一つだけを要素とする集合を作るコンストラクタ
BddVarSet::BddVarSet(BddMgrRef mgr,
		     tVarId varid) :
  mBody(mgr.make_posiliteral(varid)),
  mSize(0U)
{
  set_size(1);
}

// 空集合を返すクラスメソッド
BddVarSet
BddVarSet::make_empty(BddMgrRef mgr)
{
  return BddVarSet(mgr.make_one());
}

// vector からの変換用コンストラクタ
BddVarSet::BddVarSet(BddMgrRef mgr,
		     const VarVector& src) :
  mSize(0U)
{
  mBody = mgr.make_one();
  for (VarVector::const_iterator p = src.begin();
       p != src.end(); ++ p) {
    mBody &= mgr.make_posiliteral(*p);
  }
  set_size(src.size());
}

// list からの変換用コンストラクタ
BddVarSet::BddVarSet(BddMgrRef mgr,
		     const VarList& src) :
  mSize(0U)
{
  mBody = mgr.make_one();
  for (VarList::const_iterator p = src.begin();
       p != src.end(); ++ p) {
    mBody &= mgr.make_posiliteral(*p);
  }
  set_size(src.size());
}

// 空の時, 真となる
bool
BddVarSet::empty() const
{
  return mBody.is_one();
}

// 要素数を返す．
size_t
BddVarSet::size() const
{
  if ( !static_cast<bool>(mSize & 1U) ) {
    set_size(mBody.size());
  }
  return (mSize >> 1);
}

// 先頭の反復子を返す．
BddVarSet::iterator
BddVarSet::begin() const
{
  return iterator(mBody);
}

// 末尾の反復子を返す．
BddVarSet::iterator
BddVarSet::end() const
{
  return iterator(mBody.mgr().make_one());
}

// 集合和を計算する．
BddVarSet
BddVarSet::operator+(const BddVarSet& set2) const
{
  // 実は BDD の論理積演算
  Bdd tmp = mBody & set2.mBody;
  return BddVarSet(tmp);
}

// 集合和を計算して代入する．
const BddVarSet&
BddVarSet::operator+=(const BddVarSet& set2)
{
  // 実は BDD の論理積演算
  mBody &= set2.mBody;
  invalidate_size();
  return *this;
}

// 集合積を計算する．
BddVarSet
BddVarSet::operator*(const BddVarSet& set2) const
{
  Bdd tmp = vscap(mBody, set2.mBody);
  return BddVarSet(tmp);
}

// 集合積を計算して代入する．
const BddVarSet&
BddVarSet::operator*=(const BddVarSet& set2)
{
  mBody = vscap(mBody, set2.mBody);
  invalidate_size();
  return *this;
}

// 集合差を計算する．
BddVarSet
BddVarSet::operator-(const BddVarSet& set2) const
{
  Bdd tmp = vsdiff(mBody, set2.mBody);
  return BddVarSet(tmp);
}

// 集合差を計算して代入する．
const BddVarSet&
BddVarSet::operator-=(const BddVarSet& set2)
{
  mBody = vsdiff(mBody, set2.mBody);
  invalidate_size();
  return *this;
}

// 変数番号の vector に変換する．
size_t
BddVarSet::to_vector(VarVector& dst) const
{
  // 手抜き
  return mBody.support(dst);
}

// 変数番号の list に変換する．
size_t
BddVarSet::to_list(VarList& dst) const
{
  // 手抜き
  return mBody.support(dst);
}

// サイズを設定する．
void
BddVarSet::set_size(size_t size) const
{
  mSize = (size << 1) | static_cast<size_t>(true);
}

// サイズを無効化する．
void
BddVarSet::invalidate_size()
{
  mSize = static_cast<size_t>(false);
}

// ostream に対する出力
ostream&
operator<<(ostream& s,
	   const BddVarSet& varset)
{
  Bdd bdd = varset.function();

  s << "BddVarSet{";

  if ( bdd.is_error() ) {
    s << "Error";
  }
  else if ( bdd.is_overflow() ) {
    s << "Overflow";
  }
  else {
    list<tVarId> varlist;
    varset.to_list(varlist);
    bool first = true;
    for (list<tVarId>::iterator p = varlist.begin();
	 p != varlist.end(); ++ p) {
      if ( first ) {
	first = false;
      }
      else {
	s << ", ";
      }
      tVarId varid = *p;
      s << varid;
    }
  }

  s << "}";

  return s;
}

END_NAMESPACE_YM_BDD
