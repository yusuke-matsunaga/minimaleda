#ifndef YM_UTILS_DLLIST_H
#define YM_UTILS_DLLIST_H

/// @file ym_utils/DlList.h 
/// @brief DlList のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: DlList.h 1143 2008-01-04 07:27:17Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


/// @defgroup DlListGroup リンク内蔵型リスト
/// @ingroup YmUtils
///
/// 要素自身にリンクを内蔵したリンクトリストを作るためのクラス群
///
/// 使うときは，リストの要素となるクラスを DlElem から派生させて作る．
/// そのクラスを X とすると DlList<X> というクラスが X のリストを表す
/// クラスとなる．
///
/// ただし，このクラスの派生クラスに仮想関数があると仮想関数テーブル
/// へのポインタ分のオフセットがあとで付加されるため正しく動かなくなる．
///
/// このリストに対する反復子と定数反復子はそれぞれ DlListIter<X>
/// と DlListConstIter<X> になる．


#include "ymtools.h"


BEGIN_NAMESPACE_YM

template<typename T>
class DlList;

template<typename T>
class DlListIter;

template<typename T>
class DlListConstIter;


//////////////////////////////////////////////////////////////////////
/// @class DlElem DlList.h <ym_utils/DlList.h>
/// @brief DlList の要素の基底クラス
/// @ingroup DlListGroup
/// @sa DlList DlListIter
//////////////////////////////////////////////////////////////////////
class DlElem
{
  friend class DlListBase;

public:

  /// @brief コンストラクタ
  DlElem();

  /// @brief 前の要素を得る．
  DlElem*
  prev() const;
  
  /// @brief 次の要素を得る．
  DlElem*
  next() const;

  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 前の要素を指すポインタ
  DlElem* mPrevLink;

  // 次の要素を指すポインタ
  DlElem* mNextLink;

};


//////////////////////////////////////////////////////////////////////
/// @class DlListBase DlList.h <ym_utils/DlList.h>
/// @brief DlElem のリスト
/// @ingroup DlListGroup
/// @sa DlElem
//////////////////////////////////////////////////////////////////////
class DlListBase
{
public:

  /// @brief コンストラクタ
  /// @note 空のリストになる
  DlListBase();

  /// @brief デストラクタ
  /// @note このリストが持っている要素は自動的に削除される
  ~DlListBase();

  /// @brief 内容を空にする．
  void
  clear();
  
  /// @brief 要素数の取得
  size_t
  size() const;

  /// @brief 空のとき true を返す．
  bool
  empty() const;


protected:

  /// @brief 先頭に要素を追加する．
  void
  _push_front(DlElem* t);
  
  /// @brief 末尾に要素を追加する．
  void
  _push_back(DlElem* t);

  /// @brief 要素を挿入する．
  /// @param[in] t 挿入する要素
  /// @param[in] prev 挿入する直前の要素
  /// @param[in] next 挿入する直後の要素
  void
  _insert(DlElem* t,
	  DlElem* prev,
	  DlElem* next);

  /// @brief 先頭の要素を削除する．
  void
  _pop_front();

  /// @brief 末尾の要素を削除する．
  void
  _pop_back();
  
  /// @brief 要素を削除する．
  void
  _erase(DlElem* t);
  
  /// @brief 要素を削除する．
  /// @param[in] t 削除する要素
  /// @param[in] prev t の直前の要素
  /// @param[in] next t の直後の要素
  void
  __erase(DlElem* t,
	  DlElem* prev,
	  DlElem* next);

  /// @brief 先頭の要素を返す．
  DlElem*
  _front() const;

  /// @brief 末尾の要素を返す．
  DlElem*
  _back() const;
  
  /// @brief 先頭の反復子を返す．
  DlElem*
  _begin() const;

  /// @brief 末尾の反復子を返す．
  DlElem*
  _end() const;


private:

  /// @brief コピーコンストラクタ
  /// @note これは実装してはいけない．
  DlListBase(const DlListBase& src);

  /// @brief 代入演算子
  /// @note これは実装してはいけない．
  const DlListBase&
  operator=(const DlListBase& src);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ダミーの要素
  mutable
  DlElem mDummyTop;

  // 要素数
  size_t mNum;

};


//////////////////////////////////////////////////////////////////////
/// @class DlListIter DlList.h <ym_utils/DlList.h>
/// @brief Doublly Linked List の反復子
/// @ingroup DlListGroup
/// @sa DlList DlElem
//////////////////////////////////////////////////////////////////////
template<typename T>
class DlListIter
{
  friend class DlList<T>;

public:

  /// @brief 空の反復子
  DlListIter();

  /// @brief コピーコンストラクタ
  /// @param[in] src コピー元のオブジェクト
  DlListIter(const DlListIter& src);

  /// @brief 代入演算子
  /// @param[in] src コピー元のオブジェクト
  const DlListIter&
  operator=(const DlListIter& src);

  /// @brief デストラクタ
  ~DlListIter();

  /// @brief 反復子の指している内容を得る．
  T*
  operator*() const;

  /// @brief 一つ進める(前置演算子)
  /// @return 進めた後の反復子を返す．
  DlListIter
  operator++();

  /// @brief 一つ進める(後置演算子)
  /// @return 進める前の反復子を返す．
  /// @note int は後置演算子を表すためのダミー
  DlListIter
  operator++(int);

  /// @brief 一つ戻す(前置演算子)
  /// @return 戻した後の反復子を返す．
  DlListIter
  operator--();

  /// @brief 一つ戻す(後置演算子)
  /// @return 戻す前の反復子を返す．
  /// @note int は後置演算子を表すためのダミー
  DlListIter
  operator--(int);

  /// @brief 等価比較演算子
  bool
  operator==(const DlListIter& src) const;

  /// @brief 非等価比較演算子
  bool
  operator!=(const DlListIter& src) const;


private:

  /// @brief 内容を指定するコンストラクタ
  DlListIter(T* t);

  /// @brief ポインタを進める．
  void
  next_ptr();

  /// @brief ポインタを戻す．
  void
  prev_ptr();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 指している内容
  T* mPtr;

};


//////////////////////////////////////////////////////////////////////
/// @class DlListConstIter DlList.h <ym_utils/DlList.h>
/// @brief Doublly Linked List の constant 反復子
/// @ingroup DlListGroup
/// @sa DlList DlElem
//////////////////////////////////////////////////////////////////////
template<typename T>
class DlListConstIter
{
  friend class DlList<T>;

public:

  /// @brief 空の反復子
  DlListConstIter();

  /// @brief コピーコンストラクタ
  /// @param[in] src コピー元のオブジェクト
  DlListConstIter(const DlListConstIter& src);

  /// @brief 代入演算子
  /// @param[in] src コピー元のオブジェクト
  const DlListConstIter&
  operator=(const DlListConstIter& src);

  /// @brief デストラクタ
  ~DlListConstIter();

  /// @brief 反復子の指している内容を得る．
  T*
  operator*() const;

  /// @brief 一つ進める(前置演算子)
  /// @return 進めた後の反復子を返す．
  DlListConstIter
  operator++();

  /// @brief 一つ進める(後置演算子)
  /// @return 進める前の反復子を返す．
  /// @note int は後置演算子を表すためのダミー
  DlListConstIter
  operator++(int);

  /// @brief 一つ戻す(前置演算子)
  /// @return 戻した後の反復子を返す．
  DlListConstIter
  operator--();

  /// @brief 一つ戻す(後置演算子)
  /// @return 戻す前の反復子を返す．
  /// @note int は後置演算子を表すためのダミー
  DlListConstIter
  operator--(int);

  /// @brief 等価比較演算子
  bool
  operator==(const DlListConstIter& src) const;

  /// @brief 非等価比較演算子
  bool
  operator!=(const DlListConstIter& src) const;


private:

  /// @brief 内容を指定するコンストラクタ
  DlListConstIter(T* t);

  /// @brief ポインタを進める．
  void
  next_ptr();

  /// @brief ポインタを戻す．
  void
  prev_ptr();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 指している内容
  T* mPtr;

};


//////////////////////////////////////////////////////////////////////
/// @class DlList DlList.h <ym_utils/DlList.h>
/// @brief Dl のリスト
/// @ingroup DlListGroup
/// @sa DlListIter
//////////////////////////////////////////////////////////////////////
template<typename T>
class DlList :
  public DlListBase
{
public:

  typedef DlListIter<T> iterator;
  typedef DlListConstIter<T> const_iterator;
  
public:

  /// @brief コンストラクタ
  /// @note 空のリストになる
  DlList();

  /// @brief デストラクタ
  /// @note このリストが持っている要素は自動的に削除される
  ~DlList();

  /// @brief 先頭に要素を追加する．
  void
  push_front(T* t);
  
  /// @brief 末尾に要素を追加する．
  void
  push_back(T* t);

  /// @brief 要素を挿入する．
  void
  insert(const DlListIter<T>& p,
	 T* t);

  /// @brief 先頭の要素を削除する．
  void
  pop_front();

  /// @brief 末尾の要素を削除する．
  void
  pop_back();
  
  /// @brief 要素を削除する．
  void
  erase(T* t);

  /// @brief 先頭の要素を返す．
  T*
  front() const;

  /// @brief 末尾の要素を返す．
  T*
  back() const;
  
  /// @brief 先頭の反復子を返す．
  DlListIter<T>
  begin();

  /// @brief 先頭の反復子を返す．
  DlListConstIter<T>
  begin() const;

  /// @brief 末尾の反復子を返す．
  DlListIter<T>
  end();

  /// @brief 末尾の反復子を返す．
  DlListConstIter<T>
  end() const;


private:

  /// @brief コピーコンストラクタ
  /// @note これは実装してはいけない．
  DlList(const DlList& src);

  /// @brief 代入演算子
  /// @note これは実装してはいけない．
  const DlList&
  operator=(const DlList& src);

};


//////////////////////////////////////////////////////////////////////
// DlElem のインライン関数
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
inline
DlElem::DlElem() :
  mPrevLink(this),
  mNextLink(this)
{
}

// @brief 前の要素を得る．
inline
DlElem*
DlElem::prev() const
{
  return mPrevLink;
}
  
// @brief 次の要素を得る．
inline
DlElem*
DlElem::next() const
{
  return mNextLink;
}


//////////////////////////////////////////////////////////////////////
// DlListBase のインライン関数
//////////////////////////////////////////////////////////////////////
  
// @brief 要素数の取得
inline
size_t
DlListBase::size() const
{
  return mNum;
}

// @brief 空のとき true を返す．
inline
bool
DlListBase::empty() const
{
  return mNum == 0;
}

// @brief 先頭に要素を追加する．
inline
void
DlListBase::_push_front(DlElem* t)
{
  DlElem* prev = &mDummyTop;
  DlElem* next = prev->mNextLink;
  assert_cond(next->mPrevLink == prev, __FILE__, __LINE__);
  _insert(t, prev, next);
}
  
// @brief 末尾に要素を追加する．
inline
void
DlListBase::_push_back(DlElem* t)
{
  DlElem* next = &mDummyTop;
  DlElem* prev = next->mPrevLink;
  assert_cond(prev->mNextLink == next, __FILE__, __LINE__);
  _insert(t, prev, next);
}

// @brief 要素を挿入する．
// @param[in] t 挿入する要素
// @param[in] prev 挿入する直前の要素
// @param[in] next 挿入する直後の要素
inline
void
DlListBase::_insert(DlElem* t,
		    DlElem* prev,
		    DlElem* next)
{
  assert_cond(t->mNextLink == t, __FILE__, __LINE__);
  assert_cond(t->mPrevLink == t, __FILE__, __LINE__);
  prev->mNextLink = t;
  t->mPrevLink = prev;
  t->mNextLink = next;
  next->mPrevLink = t;
  ++ mNum;
}

// @brief 先頭の要素を削除する．
inline
void
DlListBase::_pop_front()
{
  DlElem* prev = &mDummyTop;
  DlElem* t = prev->mNextLink;
  DlElem* next = t->mNextLink;
  assert_cond(t->mPrevLink == prev, __FILE__, __LINE__);
  assert_cond(next->mPrevLink == t, __FILE__, __LINE__);
  __erase(t, prev, next);
}

// @brief 末尾の要素を削除する．
inline
void
DlListBase::_pop_back()
{
  DlElem* next = &mDummyTop;
  DlElem* t = next->mPrevLink;
  DlElem* prev = t->mPrevLink;
  assert_cond(t->mNextLink == next, __FILE__, __LINE__);
  assert_cond(prev->mNextLink == t, __FILE__, __LINE__);
  __erase(t, prev, next);
}
  
// @brief 要素を削除する．
inline
void
DlListBase::_erase(DlElem* t)
{
  DlElem* prev = t->mPrevLink;
  DlElem* next = t->mNextLink;
  assert_cond(prev != t, __FILE__, __LINE__);
  assert_cond(next != t, __FILE__, __LINE__);
  assert_cond(prev->mNextLink == t, __FILE__, __LINE__);
  assert_cond(next->mPrevLink == t, __FILE__, __LINE__);
  __erase(t, prev, next);
}
  
// @brief 要素を削除する．
// @param[in] t 削除する要素
// @param[in] prev t の直前の要素
// @param[in] next t の直後の要素
inline
void
DlListBase::__erase(DlElem* t,
		    DlElem* prev,
		    DlElem* next)
{
  prev->mNextLink = next;
  next->mPrevLink = prev;
  t->mNextLink = t;
  t->mPrevLink = t;
  -- mNum;
}

// @brief 先頭の要素を返す．
inline
DlElem*
DlListBase::_front() const
{
  return mDummyTop.mNextLink;
}

// @brief 末尾の要素を返す．
inline
DlElem*
DlListBase::_back() const
{
  return mDummyTop.mPrevLink;
}
  
// @brief 先頭の反復子を返す．
inline
DlElem*
DlListBase::_begin() const
{
  return mDummyTop.mNextLink;
}

// @brief 末尾の反復子を返す．
inline
DlElem*
DlListBase::_end() const
{
  return &mDummyTop;
}


//////////////////////////////////////////////////////////////////////
// DlListIter<T> のインライン関数
//////////////////////////////////////////////////////////////////////

// @brief 空の反復子
template<typename T>
inline
DlListIter<T>::DlListIter() :
  mPtr(NULL)
{
}

// @brief コピーコンストラクタ
// @param[in] src コピー元のオブジェクト
template<typename T>
inline
DlListIter<T>::DlListIter(const DlListIter& src) :
  mPtr(src.mPtr)
{
}

// @brief 代入演算子
// @param[in] src コピー元のオブジェクト
template<typename T>
inline
const DlListIter<T>&
DlListIter<T>::operator=(const DlListIter& src)
{
  mPtr = src.mPtr;
  return *this;
}

// @brief デストラクタ
template<typename T>
inline
DlListIter<T>::~DlListIter()
{
}

// @brief 反復子の指している内容を得る．
template<typename T>
inline
T*
DlListIter<T>::operator*() const
{
  return mPtr;
}

// @brief 一つ進める(前置演算子)
// @return 進めた後の反復子を返す．
template<typename T>
inline
DlListIter<T>
DlListIter<T>::operator++()
{
  next_ptr();
  return *this;
}

// @brief 一つ進める(後置演算子)
// @return 進める前の反復子を返す．
// @note int は後置演算子を表すためのダミー
template<typename T>
inline
DlListIter<T>
DlListIter<T>::operator++(int)
{
  DlListIter ans(*this);
  next_ptr();
  return ans;
}

// @brief 一つ戻す(前置演算子)
// @return 戻した後の反復子を返す．
template<typename T>
inline
DlListIter<T>
DlListIter<T>::operator--()
{
  prev_ptr();
  return *this;
}

// @brief 一つ戻す(後置演算子)
// @return 戻す前の反復子を返す．
// @note int は後置演算子を表すためのダミー
template<typename T>
inline
DlListIter<T>
DlListIter<T>::operator--(int)
{
  DlListIter ans(*this);
  prev_ptr();
  return ans;
}

// @brief 等価比較演算子
template<typename T>
inline
bool
DlListIter<T>::operator==(const DlListIter& src) const
{
  return mPtr == src.mPtr;
}

// @brief 非等価比較演算子
template<typename T>
inline
bool
DlListIter<T>::operator!=(const DlListIter& src) const
{
  return mPtr != src.mPtr;
}

// @brief 内容を指定するコンストラクタ
template<typename T>
inline
DlListIter<T>::DlListIter(T* t) :
  mPtr(t)
{
}

// @brief ポインタを進める．
template<typename T>
inline
void
DlListIter<T>::next_ptr()
{
  mPtr = reinterpret_cast<T*>(mPtr->next());
}

// @brief ポインタを戻す．
template<typename T>
inline
void
DlListIter<T>::prev_ptr()
{
  mPtr = reinterpret_cast<T*>(mPtr->prev());
}


//////////////////////////////////////////////////////////////////////
// DlListConstIter<T> のインライン関数
//////////////////////////////////////////////////////////////////////

// @brief 空の反復子
template<typename T>
inline
DlListConstIter<T>::DlListConstIter() :
  mPtr(NULL)
{
}

// @brief コピーコンストラクタ
// @param[in] src コピー元のオブジェクト
template<typename T>
inline
DlListConstIter<T>::DlListConstIter(const DlListConstIter& src) :
  mPtr(src.mPtr)
{
}

// @brief 代入演算子
// @param[in] src コピー元のオブジェクト
template<typename T>
inline
const DlListConstIter<T>&
DlListConstIter<T>::operator=(const DlListConstIter& src)
{
  mPtr = src.mPtr;
  return *this;
}

// @brief デストラクタ
template<typename T>
inline
DlListConstIter<T>::~DlListConstIter()
{
}

// @brief 反復子の指している内容を得る．
template<typename T>
inline
T*
DlListConstIter<T>::operator*() const
{
  return mPtr;
}

// @brief 一つ進める(前置演算子)
// @return 進めた後の反復子を返す．
template<typename T>
inline
DlListConstIter<T>
DlListConstIter<T>::operator++()
{
  next_ptr();
  return *this;
}

// @brief 一つ進める(後置演算子)
// @return 進める前の反復子を返す．
// @note int は後置演算子を表すためのダミー
template<typename T>
inline
DlListConstIter<T>
DlListConstIter<T>::operator++(int)
{
  DlListConstIter ans(*this);
  next_ptr();
  return ans;
}

// @brief 一つ戻す(前置演算子)
// @return 戻した後の反復子を返す．
template<typename T>
inline
DlListConstIter<T>
DlListConstIter<T>::operator--()
{
  prev_ptr();
  return *this;
}

// @brief 一つ戻す(後置演算子)
// @return 戻す前の反復子を返す．
// @note int は後置演算子を表すためのダミー
template<typename T>
inline
DlListConstIter<T>
DlListConstIter<T>::operator--(int)
{
  DlListConstIter ans(*this);
  prev_ptr();
  return ans;
}

// @brief 等価比較演算子
template<typename T>
inline
bool
DlListConstIter<T>::operator==(const DlListConstIter& src) const
{
  return mPtr == src.mPtr;
}

// @brief 非等価比較演算子
template<typename T>
inline
bool
DlListConstIter<T>::operator!=(const DlListConstIter& src) const
{
  return mPtr != src.mPtr;
}

// @brief 内容を指定するコンストラクタ
template<typename T>
inline
DlListConstIter<T>::DlListConstIter(T* t) :
  mPtr(t)
{
}

// @brief ポインタを進める．
template<typename T>
inline
void
DlListConstIter<T>::next_ptr()
{
  mPtr = reinterpret_cast<T*>(mPtr->next());
}

// @brief ポインタを戻す．
template<typename T>
inline
void
DlListConstIter<T>::prev_ptr()
{
  mPtr = reinterpret_cast<T*>(mPtr->prev());
}


//////////////////////////////////////////////////////////////////////
// DlList<T> のインライン関数
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @note 空のリストになる
template<typename T>
inline
DlList<T>::DlList()
{
}

// @brief デストラクタ
// @note このリストが持っている要素は自動的に削除される
template<typename T>
inline
DlList<T>::~DlList()
{
}

// @brief 先頭に要素を追加する．
template<typename T>
inline
void
DlList<T>::push_front(T* t)
{
  _push_front(t);
}
  
// @brief 末尾に要素を追加する．
template<typename T>
inline
void
DlList<T>::push_back(T* t)
{
  _push_back(t);
}

// @brief 要素を挿入する．
template<typename T>
inline
void
DlList<T>::insert(const DlListIter<T>& p,
		  T* t)
{
  DlElem* prev = p.mPtr;
  DlElem* next = prev->next();
  assert_cond(next->prev() == prev, __FILE__, __LINE__);
  _insert(t, prev, next);
}

// @brief 先頭の要素を削除する．
template<typename T>
inline
void
DlList<T>::pop_front()
{
  _pop_front();
}

// @brief 末尾の要素を削除する．
template<typename T>
inline
void
DlList<T>::pop_back()
{
  _pop_back();
}
  
// @brief 要素を削除する．
template<typename T>
inline
void
DlList<T>::erase(T* t)
{
  _erase(t);
}

// @brief 先頭の要素を返す．
template<typename T>
inline
T*
DlList<T>::front() const
{
  return reinterpret_cast<T*>(_front());
}

// @brief 末尾の要素を返す．
template<typename T>
inline
T*
DlList<T>::back() const
{
  return reinterpret_cast<T*>(_back());
}
  
// @brief 先頭の反復子を返す．
template<typename T>
inline
DlListIter<T>
DlList<T>::begin()
{
  return DlListIter<T>(reinterpret_cast<T*>(_begin()));
}

// @brief 先頭の反復子を返す．
template<typename T>
inline
DlListConstIter<T>
DlList<T>::begin() const
{
  return DlListConstIter<T>(reinterpret_cast<T*>(_begin()));
}

// @brief 末尾の反復子を返す．
template<typename T>
inline
DlListIter<T>
DlList<T>::end()
{
  return DlListIter<T>(reinterpret_cast<T*>(_end()));
}

// @brief 末尾の反復子を返す．
template<typename T>
inline
DlListConstIter<T>
DlList<T>::end() const
{
  return DlListConstIter<T>(reinterpret_cast<T*>(_end()));
}

END_NAMESPACE_YM

#endif // YM_UTILS_DLLIST_H
