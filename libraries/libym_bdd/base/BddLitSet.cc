
/// @file libym_bdd/base/BddLitSet.cc
/// @brief BddLitSet の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: BddLitSet.cc 700 2007-05-31 00:41:30Z matsunaga $
///
/// Copyright (C) 2005-2006 Yusuke Matsunaga
/// All rights reserved.


#if HAVE_CONFIG_H
#include <ymconfig.h>
#endif

#include <ym_bdd/BddLitSet.h>


BEGIN_NAMESPACE_YM_BDD

//////////////////////////////////////////////////////////////////////
// BddLitSet::iterator
//////////////////////////////////////////////////////////////////////

// コンストラクタ
BddLitSet::iterator::iterator()
{
}

// BddLitSet 用のコンストラクタ
BddLitSet::iterator::iterator(const Bdd& bdd) :
  mCur(bdd)
{
}
    
// 現在指している要素を返す．
// 末尾かどうかのチェックは行わない．
Literal
BddLitSet::iterator::operator*() const
{
  Bdd f0, f1;
  tVarId varid = mCur.root_decomp(f0, f1);
  tPol pol1 = f0.is_zero() ? kPolPosi : kPolNega;
  return Literal(varid, pol1);
}

// ひとつ先に進める．
// 自分自身を返す．
BddLitSet::iterator&
BddLitSet::iterator::operator++()
{
  next();
  return *this;
}

// ひとつ先に進める．
// 進める前の値を返す．
BddLitSet::iterator
BddLitSet::iterator::operator++(int)
{
  iterator old = *this;
  next();
  return old;
}

// 等価比較
bool
BddLitSet::iterator::operator==(const iterator& src) const
{
  return mCur == src.mCur;
}

bool
BddLitSet::iterator::operator!=(const iterator& src) const
{
  return ! operator==(src);
}
    
// ひとつ先に進める．
void
BddLitSet::iterator::next()
{
  Bdd f0, f1;
  mCur.root_decomp(f0, f1);
  if ( f0.is_zero() ) {
    mCur = f1;
  }
  else {
    mCur = f0;
  }
}


//////////////////////////////////////////////////////////////////////
// BddLitSet
//////////////////////////////////////////////////////////////////////

// 空のコンストラクタ
BddLitSet::BddLitSet(BddMgrRef mgr) :
  mBody(mgr.make_one()),
  mSize(0U)
{
  set_size(0);
}

// BDD を指定するコンストラクタ
BddLitSet::BddLitSet(const Bdd& bdd) :
  mBody(bdd),
  mSize(0U)
{
}

// コピーコンストラクタ
BddLitSet::BddLitSet(const BddLitSet& src) :
  mBody(src.mBody),
  mSize(src.mSize)
{
}

// 代入演算子
const BddLitSet&
BddLitSet::operator=(const BddLitSet& src)
{
  mBody = src.mBody;
  mSize = src.mSize;
  return *this;
}

// リテラル1つだけを要素とする集合を作るコンストラクタ
BddLitSet::BddLitSet(BddMgrRef mgr,
		     tVarId varid,
		     tPol pol) :
  mBody(mgr.make_literal(varid, pol)),
  mSize(0U)
{
  set_size(1);
}

BddLitSet::BddLitSet(BddMgrRef mgr,
		     const Literal& lit) :
  mBody(mgr.make_literal(lit)),
  mSize(0U)
{
  set_size(1);
}

// 空集合を返すクラスメソッド
BddLitSet
BddLitSet::make_empty(BddMgrRef mgr)
{
  return BddLitSet(mgr.make_one());
}

// vector からの変換用コンストラクタ
BddLitSet::BddLitSet(BddMgrRef mgr,
		     const LiteralVector& src) :
  mSize(0U)
{
  mBody = mgr.make_one();
  for (LiteralVector::const_iterator p = src.begin();
       p != src.end(); ++ p) {
    mBody &= mgr.make_literal(*p);
  }
  set_size(src.size());
}

// list からの変換用コンストラクタ
BddLitSet::BddLitSet(BddMgrRef mgr,
		     const LiteralList& src) :
  mSize(0U)
{
  mBody = mgr.make_one();
  for (LiteralList::const_iterator p = src.begin();
       p != src.end(); ++ p) {
    mBody &= mgr.make_literal(*p);
  }
  set_size(src.size());
}

// 空の時, 真となる．
bool
BddLitSet::empty() const
{
  return mBody.is_one();
}

// 要素数を返す．
size_t
BddLitSet::size() const
{
  if ( !static_cast<bool>(mSize & 1U) ) {
    set_size(mBody.size());
  }
  return (mSize >> 1);
}

// 先頭の反復子を返す．
BddLitSet::iterator
BddLitSet::begin() const
{
  return iterator(mBody);
}

// 末尾の反復子を返す．
BddLitSet::iterator
BddLitSet::end() const
{
  return iterator(mBody.mgr().make_one());
}

// 集合和を計算する．
BddLitSet
BddLitSet::operator+(const BddLitSet& set2) const
{
  // 実は BDD の論理積演算
  Bdd tmp = mBody & set2.mBody;
  if ( tmp.is_zero() ) {
    // ただし空集合は0ではなく1である．
    tmp.negate();
  }
  return BddLitSet(tmp);
}

// 集合和を計算して代入する．
const BddLitSet&
BddLitSet::operator+=(const BddLitSet& set2)
{
  // 実は BDD の論理積演算
  mBody &= set2.mBody;
  if ( mBody.is_zero() ) {
    // ただし空集合は0ではなく1である．
    mBody = ~mBody;
  }
  invalidate_size();
  return *this;
}

// 集合積を計算する．
BddLitSet
BddLitSet::operator*(const BddLitSet& set2) const
{
  return BddLitSet(lscap(mBody, set2.mBody));
}

// 集合積を計算して代入する．
const BddLitSet&
BddLitSet::operator*=(const BddLitSet& set2)
{
  mBody = lscap(mBody, set2.mBody);
  invalidate_size();
  return *this;
}

// 集合差を計算する．
BddLitSet
BddLitSet::operator-(const BddLitSet& set2) const
{
  return BddLitSet(lsdiff(mBody, set2.mBody));
}

// 集合差を計算して代入する．
const BddLitSet&
BddLitSet::operator-=(const BddLitSet& set2)
{
  mBody = lsdiff(mBody, set2.mBody);
  invalidate_size();
  return *this;
}

// リテラルの vector に変換する．
size_t
BddLitSet::to_vector(LiteralVector& dst) const
{
  return mBody.to_literalvector(dst);
}

// 変数番号の list に変換する．
size_t
BddLitSet::to_list(LiteralList& dst) const
{
  return mBody.to_literallist(dst);
}

// サイズを設定する．
void
BddLitSet::set_size(size_t size) const
{
  mSize = (size << 1) | static_cast<size_t>(true);
}

// サイズを無効化する．
void
BddLitSet::invalidate_size()
{
  mSize = static_cast<size_t>(false);
}

// ostream に対する出力
ostream&
operator<<(ostream& s,
	   const BddLitSet& litset)
{
  Bdd bdd = litset.function();

  s << "BddLitSet{";

  if ( bdd.is_error() ) {
    s << "Error";
  }
  else if ( bdd.is_overflow() ) {
    s << "Overflow";
  }
  else {
    list<Literal> litlist;
    litset.to_list(litlist);
    bool first = true;
    for (list<Literal>::iterator p = litlist.begin();
	 p != litlist.end(); ++ p) {
      if ( first ) {
	first = false;
      }
      else {
	s << ", ";
      }
      Literal literal = *p;
      s << literal;
    }
  }

  s << "}";

  return s;
}

END_NAMESPACE_YM_BDD
