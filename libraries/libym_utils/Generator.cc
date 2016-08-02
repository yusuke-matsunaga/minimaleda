
/// @file libym_utils/Generator.cc
/// @brief 組み合わせ生成器と順列生成器の実装ファイル
/// @author Yusuke Matsunaga
///
/// $Id: Generator.cc 700 2007-05-31 00:41:30Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_utils/Generator.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
// CombiGen と PermGen に共通な属性を表す基底クラス
//////////////////////////////////////////////////////////////////////

// 空のコンストラクタ
GenBase::iterator::iterator() :
  mElem(0),
  mParent(NULL)
{
}

// コンストラクタ
// 継承クラスが用いる．
GenBase::iterator::iterator(const GenBase* parent) :
  mElem(parent->k()),
  mParent(parent)
{
  for (size_t i = 0; i < mParent->k(); ++ i) {
    mElem[i] = i;
  }
}

// 内容をコピーする関数
void
GenBase::iterator::copy(const iterator& src)
{
  mElem = src.mElem;
  mParent = src.mParent;
}

// コンストラクタ
// 全要素数 n と選択する要素数 k を必ず指定する．
GenBase::GenBase(size_t n,
		 size_t k) :
  mN(n),
  mK(k)
{
}
  
// デストラクタ
GenBase::~GenBase()
{
}


//////////////////////////////////////////////////////////////////////
// 組み合わせ生成器を表すクラス
//////////////////////////////////////////////////////////////////////

// 空のコンストラクタ
CombiGen::iterator::iterator()
{
}

// コンストラクタ
// CombiGen が用いる．
CombiGen::iterator::iterator(const CombiGen* parent) :
  GenBase::iterator(parent)
{
}

// コピーコンストラクタ
CombiGen::iterator::iterator(const iterator& src)
{
  copy(src);
}

// 代入演算子
const CombiGen::iterator&
CombiGen::iterator::operator=(const iterator& src)
{
  copy(src);
  return *this;
}

// 次の要素を求める．
CombiGen::iterator
CombiGen::iterator::operator++()
{
  if ( elem(k() - 1) < n() ) {
    next(k() - 1);
  }
  return *this;
}

// 末尾の時に true を返す．
bool
CombiGen::iterator::is_end() const
{
  // 末尾の時には範囲外の値(= n())を持っている．
  return elem(k() - 1) == n();
}

// operator++() のサブルーティン
void
CombiGen::iterator::next(size_t pos)
{
  ++ elem(pos);
  if ( elem(pos) == n() - k() + pos + 1) {
    if ( pos > 0 ) {
      next(pos - 1);
      elem(pos) = elem(pos - 1) + 1;
    }
  }
}

// コンストラクタ
// 全要素数 n と選択する要素数 k を必ず指定する．
CombiGen::CombiGen(size_t n,
		   size_t k) :
  GenBase(n, k)
{
}
  
// デストラクタ
CombiGen::~CombiGen()
{
}

// 最初の組み合わせを取り出す．
CombiGen::iterator
CombiGen::begin()
{
  return iterator(this);
}


//////////////////////////////////////////////////////////////////////
// 順列生成器を表すクラス
//////////////////////////////////////////////////////////////////////

// 空のコンストラクタ
PermGen::iterator::iterator()
{
}

// コンストラクタ
// PermGen が用いる．
PermGen::iterator::iterator(const PermGen* parent) :
  GenBase::iterator(parent)
{
}

// コピーコンストラクタ
PermGen::iterator::iterator(const iterator& src)
{
  copy(src);
}

// 代入演算子
const PermGen::iterator&
PermGen::iterator::operator=(const iterator& src)
{
  copy(src);
  return *this;
}

// 次の要素を求める．
PermGen::iterator
PermGen::iterator::operator++()
{
  vector<int> bitmap(n());
  for (size_t i = 0; i < n(); ++ i) {
    bitmap[i] = 0;
  }
  for (size_t i = 0; i < k(); ++ i) {
    bitmap[elem(i)] = 1;
  }
  for (size_t i = k(); i -- > 0; ) {
    bool found = false;
    for (size_t j = elem(i); ++ j < n(); ) {
      if ( bitmap[j] == 0 ) {
	bitmap[elem(i)] = 0;
	elem(i) = j;
	bitmap[j] = 1;
	found = true;
	break;
      }
    }
    if ( found ) {
      size_t pos = 0;
      for (size_t j = i + 1; j < k(); ++ j) {
	for ( ; bitmap[pos] == 1; ++ pos) ;
	bitmap[pos] = 1;
	elem(j) = pos;
      }
      break;
    }
    if ( i > 0 ) {
      bitmap[elem(i)] = 0;
    }
    else {
      elem(0) = n();
    }
  }

  return *this;
}

// 末尾の時に true を返す．
bool
PermGen::iterator::is_end() const
{
  return elem(0) == n();
}

// コンストラクタ
// 全要素数 n と選択する要素数 k を必ず指定する．
PermGen::PermGen(size_t n,
		 size_t k) :
  GenBase(n, k)
{
}
  
// デストラクタ
PermGen::~PermGen()
{
}

// 最初の組み合わせを取り出す．
PermGen::iterator
PermGen::begin()
{
  return iterator(this);
}


//////////////////////////////////////////////////////////////////////
// 複数の要素のグループの組み合わせ生成器を表すクラス
//////////////////////////////////////////////////////////////////////

// 空のコンストラクタ
MultiGenBase::iterator::iterator() :
  mElemArray(0),
  mParent(NULL)
{
}

// コンストラクタ
// 継承クラスが用いる．
MultiGenBase::iterator::iterator(const MultiGenBase* parent) :
  mElemArray(parent->ngrp()),
  mParent(parent)
{
  size_t ngrp = parent->ngrp();
  for (size_t g = 0; g < ngrp; ++ g) {
    mElemArray[g] = new vector<size_t>(k(g));
    init(g);
  }
}

// コピーする．
void
MultiGenBase::iterator::copy(const iterator& src)
{
  if ( this != &src ) {
    free();
    mParent = src.mParent;
    if ( mParent ) {
      size_t ngrp = mParent->ngrp();
      mElemArray.resize(ngrp);
      for (size_t g = 0; g < ngrp; ++ g) {
	mElemArray[g] = new vector<size_t>(k(g));
	elem(g) = src.elem(g);
      }
    }
    else {
      mElemArray.resize(0);
    }
  }
}

// デストラクタ
MultiGenBase::iterator::~iterator()
{
  free();
}

// grp 番目のグループの要素配列を初期化する．
void
MultiGenBase::iterator::init(size_t grp)
{
  for (size_t i = 0; i < k(grp); ++ i) {
    elem(grp)[i] = i;
  }
}

// grp 番目のグループの要素配列を得る．
vector<size_t>&
MultiGenBase::iterator::elem(size_t g)
{
  assert_cond(mElemArray[g], __FILE__, __LINE__);
  return *mElemArray[g];
}

// grp 番目のグループの要素配列を得る．
// こちらは const 版
const vector<size_t>&
MultiGenBase::iterator::elem(size_t g) const
{
  return *mElemArray[g];
}

// 確保したメモリを解放する
void
MultiGenBase::iterator::free()
{
  size_t n = mElemArray.size();
  for (size_t g = 0; g < n; ++ g) {
    delete mElemArray[g];
    mElemArray[g] = NULL;
  }
}

// コンストラクタ
// 全要素数 n と選択する要素数 k のベクタを指定する．
MultiGenBase::MultiGenBase(const vector<pair<size_t, size_t> >& nk_array) :
  mNkArray(nk_array)
{
}
  
// デストラクタ
MultiGenBase::~MultiGenBase()
{
}


//////////////////////////////////////////////////////////////////////
// 複数の要素のグループの組み合わせ生成器を表すクラス
//////////////////////////////////////////////////////////////////////

// 空のコンストラクタ
MultiCombiGen::iterator::iterator()
{
}

// コンストラクタ
// MultiCombiGen が用いる．
MultiCombiGen::iterator::iterator(const MultiCombiGen* parent) :
  MultiGenBase::iterator(parent)
{
}

// コピーコンストラクタ
MultiCombiGen::iterator::iterator(const iterator& src)
{
  copy(src);
}

// 代入演算子
const MultiCombiGen::iterator&
MultiCombiGen::iterator::operator=(const iterator& src)
{
  copy(src);
  return *this;
}

// 次の要素を求める．
MultiCombiGen::iterator
MultiCombiGen::iterator::operator++()
{
  for (size_t g = ngrp(); g -- > 0; ) {
    if ( !is_end_sub(g) ) {
      next(g, k(g) - 1);
    }
    if ( !is_end_sub(g) ) {
      break;
    }
    if ( g > 0 ) {
      init(g);
    }
  }

  return *this;
}

// 末尾の時に true を返す．
bool
MultiCombiGen::iterator::is_end() const
{
  return is_end_sub(0);
}

// operator++() のサブルーティン
void
MultiCombiGen::iterator::next(size_t g, size_t pos)
{
  ++ elem(g)[pos];
  if ( elem(g)[pos] == n(g) - k(g) + pos + 1) {
    if ( pos > 0 ) {
      next(g, pos - 1);
      elem(g)[pos] = elem(g)[pos - 1] + 1;
    }
  }
}

// grp 番目のグループが終了状態の時 true を返す．
bool
MultiCombiGen::iterator::is_end_sub(size_t grp) const
{
  return elem(grp)[k(grp) - 1] == n(grp);
}

// コンストラクタ
// 全要素数 n と選択する要素数 k のベクタを指定する．
MultiCombiGen::MultiCombiGen(const vector<pair<size_t, size_t> >& nk_array) :
  MultiGenBase(nk_array)
{
}
  
// デストラクタ
MultiCombiGen::~MultiCombiGen()
{
}

// 最初の組み合わせを取り出す．
MultiCombiGen::iterator
MultiCombiGen::begin()
{
  return iterator(this);
}


//////////////////////////////////////////////////////////////////////
// 複数の要素のグループの組み合わせ生成器を表すクラス
//////////////////////////////////////////////////////////////////////

// 空のコンストラクタ
MultiPermGen::iterator::iterator()
{
}

// コンストラクタ
// MultiPermGen が用いる．
MultiPermGen::iterator::iterator(const MultiPermGen* parent) :
  MultiGenBase::iterator(parent)
{
}

// コピーコンストラクタ
MultiPermGen::iterator::iterator(const iterator& src)
{
  copy(src);
}

// 代入演算子
const MultiPermGen::iterator&
MultiPermGen::iterator::operator=(const iterator& src)
{
  copy(src);
  return *this;
}

// 次の要素を求める．
MultiPermGen::iterator
MultiPermGen::iterator::operator++()
{
  for (size_t g = ngrp(); g -- > 0; ) {
    vector<int> bitmap(n(g));
    for (size_t i = 0; i < n(g); ++ i) {
      bitmap[i] = 0;
    }
    for (size_t i = 0; i < k(g); ++ i) {
      bitmap[elem(g)[i]] = 1;
    }
    for (size_t i = k(g); i -- > 0; ) {
      bool found = false;
      for (size_t j = elem(g)[i]; ++ j < n(g); ) {
	if ( bitmap[j] == 0 ) {
	  bitmap[elem(g)[i]] = 0;
	  elem(g)[i] = j;
	  bitmap[j] = 1;
	  found = true;
	  break;
	}
      }
      if ( found ) {
	size_t pos = 0;
	for (size_t j = i + 1; j < k(g); ++ j) {
	  for ( ; bitmap[pos] == 1; ++ pos) ;
	  bitmap[pos] = 1;
	  elem(g)[j] = pos;
	}
	break;
      }
      if ( i > 0 ) {
	bitmap[elem(g)[i]] = 0;
      }
      else {
	elem(g)[0] = n(g);
      }
    }
    if ( !is_end_sub(g) ) {
      break;
    }
    if ( g > 0 ) {
      init(g);
    }
  }

  return *this;
}

// 末尾の時に true を返す．
bool
MultiPermGen::iterator::is_end() const
{
  return is_end_sub(0);
}

// grp 番目のグループが終了状態の時 true を返す．
bool
MultiPermGen::iterator::is_end_sub(size_t grp) const
{
  return elem(grp)[0] == n(grp);
}

// コンストラクタ
// 全要素数 n と選択する要素数 k のベクタを指定する．
MultiPermGen::MultiPermGen(const vector<pair<size_t, size_t> >& nk_array) :
  MultiGenBase(nk_array)
{
}
  
// デストラクタ
MultiPermGen::~MultiPermGen()
{
}

// 最初の組み合わせを取り出す．
MultiPermGen::iterator
MultiPermGen::begin()
{
  return iterator(this);
}

END_NAMESPACE_YM
