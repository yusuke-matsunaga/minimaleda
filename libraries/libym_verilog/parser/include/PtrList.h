#ifndef LIBYM_VERILOG_PARSER_MAIN_PTRLIST_H
#define LIBYM_VERILOG_PARSER_MAIN_PTRLIST_H

/// @file libym_verilog/parser/main/PtrList.h
/// @brief PtrList のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: PtrList.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "PtiFwd.h"
#include "ym_utils/Alloc.h"
#include "ym_verilog/pt/PtArray.h"


BEGIN_NAMESPACE_YM_VERILOG

template <typename T1,
	  typename T2>
class PtrList;

//////////////////////////////////////////////////////////////////////
/// @class PtrListCell PtrList.h "PtrList.h"
/// @brief PtrList 用のセル
//////////////////////////////////////////////////////////////////////
template <typename T>
struct PtrListCell
{
  // 本体を指すポインタ
  T* mPtr;

  // リンクポインタ
  PtrListCell* mLink;
  
};


//////////////////////////////////////////////////////////////////////
/// @class PtrListIterator PtrList.h "PtrList.h"
/// @brief PtrList 用の反復子
//////////////////////////////////////////////////////////////////////
template <typename T>
class PtrListIterator
{
public:

  /// @brief コンストラクタ
  PtrListIterator();

  /// @brief コピーコンストラクタ
  PtrListIterator(const PtrListIterator& src);

  /// @brief PtrList が用いるコンストラクタ
  PtrListIterator(PtrListCell<T>* cell);

  /// @brief デストラクタ
  ~PtrListIterator();


public:

  /// @brief 内容を取り出す演算子
  T*
  operator*() const;

  /// @brief 次の要素を指す．
  const PtrListIterator&
  operator++();
  
  /// @brief 正しい内容をさしているときに true を返す．
  bool
  is_valid() const;

  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // セル
  PtrListCell<T>* mCell;
  
};


//////////////////////////////////////////////////////////////////////
/// @class PtrList PtrList.h "PtrList.h"
/// @brief リスト構造のテンプレートクラス
/// @note T2 は T1 の親クラス
//////////////////////////////////////////////////////////////////////
template <typename T1,
	  typename T2 = T1>
class PtrList
{
public:

  typedef PtrListIterator<T1> const_iterator;
  typedef PtrListCell<T1> Cell;
  
public:
  
  /// @brief コンストラクタ
  /// @param[in] alloc アロケータ
  /// @note 空のリストを作る
  PtrList(UnitAlloc& alloc);
  
  /// @brief デストラクタ
  ~PtrList();


public:

  /// @brief リストのクリア
  void
  clear();
  
  /// @brief 要素の追加
  /// @param[in] elem 追加する要素
  void
  push_back(T1* elem);

  /// @brief 要素数の取得
  /// @return 要素数
  ymuint32
  size() const;

  /// @brief 空の時に true を返す．
  bool
  empty() const;
  
  /// @brief 先頭の反復子を返す．
  PtrListIterator<T1>
  begin() const;
  
  /// @brief 先頭の要素を返す．
  T1*
  front() const;

  /// @brief 末尾の要素を返す．
  T1*
  back() const;

  
public:
  //////////////////////////////////////////////////////////////////////
  /// 特別な関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容を配列にコピーする．
  /// @param[in] alloc 配列用のメモリアロケータ
  /// @note この処理の後ではリストは空になる．
  PtArray<T2>
  to_array(AllocBase& alloc);

  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // メモリ確保用のアロケータ
  UnitAlloc& mAlloc;
  
  // 先頭の要素
  Cell* mTop;
  
  // 末尾の要素
  Cell* mEnd;

  // 要素数
  ymuint32 mNum;

};


//////////////////////////////////////////////////////////////////////
// PtrListIterator のインライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
template <typename T>
inline
PtrListIterator<T>::PtrListIterator() :
  mCell(NULL)
{
}

// @brief コピーコンストラクタ
template <typename T>
inline
PtrListIterator<T>::PtrListIterator(const PtrListIterator& src) :
  mCell(src.mCell)
{
}

// @brief PtrList が用いるコンストラクタ
template <typename T>
inline
PtrListIterator<T>::PtrListIterator(PtrListCell<T>* cell) :
  mCell(cell)
{
}

// @brief デストラクタ
template <typename T>
inline
PtrListIterator<T>::~PtrListIterator()
{
}

// @brief 内容を取り出す演算子
template <typename T>
inline
T*
PtrListIterator<T>::operator*() const
{
  if ( mCell ) {
    return mCell->mPtr;
  }
  else {
    return NULL;
  }
}

// @brief 次の要素を指す．
template <typename T>
inline
const PtrListIterator<T>&
PtrListIterator<T>::operator++()
{
  if ( mCell ) {
    mCell = mCell->mLink;
  }
  return *this;
}
  
// @brief 正しい内容をさしているときに true を返す．
template <typename T>
inline
bool
PtrListIterator<T>::is_valid() const
{
  return mCell != NULL;
}


//////////////////////////////////////////////////////////////////////
// PtrList のインライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] alloc アロケータ
// @note 空のリストを作る
template <typename T1,
	  typename T2>
inline
PtrList<T1, T2>::PtrList(UnitAlloc& alloc) :
  mAlloc(alloc),
  mTop(NULL),
  mEnd(NULL),
  mNum(0)
{
}

// @brief デストラクタ
template <typename T1,
	  typename T2>
inline
PtrList<T1, T2>::~PtrList()
{
  clear();
}

// @brief リストのクリア
template <typename T1,
	  typename T2>
inline
void
PtrList<T1, T2>::clear()
{
  for (Cell* cell = mTop; cell; ) {
    Cell* next = cell->mLink;
    mAlloc.put_memory(sizeof(Cell), static_cast<void*>(cell));
    cell = next;
  }
  mTop = NULL;
  mEnd = NULL;
  mNum = 0;
}
  
// @brief 要素の追加
// @param[in] elem 追加する要素
template <typename T1,
	  typename T2>
inline
void
PtrList<T1, T2>::push_back(T1* elem)
{
  void* p = mAlloc.get_memory(sizeof(Cell));
  Cell* cell = new (p) Cell;
  cell->mPtr = elem;
  cell->mLink = NULL;
  if ( mEnd ) {
    mEnd->mLink = cell;
  }
  else {
    mTop = cell;
  }
  mEnd = cell;
  ++ mNum;
}

// @brief 要素数の取得
// @return 要素数
template <typename T1,
	  typename T2>
inline
ymuint32
PtrList<T1, T2>::size() const
{
  return mNum;
}

// @brief 空の時に true を返す．
template <typename T1,
	  typename T2>
inline
bool
PtrList<T1, T2>::empty() const
{
  return mNum == 0;
}

// @brief 先頭の反復子を返す．
template <typename T1,
	  typename T2>
inline
PtrListIterator<T1>
PtrList<T1, T2>::begin() const
{
  return PtrListIterator<T1>(mTop);
}

// @brief 先頭の要素を返す．
template <typename T1,
	  typename T2>
inline
T1*
PtrList<T1, T2>::front() const
{
  if ( mTop ) {
    return mTop->mPtr;
  }
  else {
    return NULL;
  }
}

// @brief 末尾の要素を返す．
template <typename T1,
	  typename T2>
inline
T1*
PtrList<T1, T2>::back() const
{
  if ( mEnd ) {
    return mEnd->mPtr;
  }
  else {
    return NULL;
  }
}

// @brief 内容を配列にコピーする．
// @param[in] array 対象の配列
// @note この処理の後ではリストは空になる．
template <typename T1,
	  typename T2>
inline
PtArray<T2>
PtrList<T1, T2>::to_array(AllocBase& alloc)
{
  ymuint32 n = mNum;
  void* p = alloc.get_memory(sizeof(T2*) * n);
  T2** array = new (p) T2*[n];
  ymuint32 i = 0;
  for (Cell* cell = mTop; cell; cell = cell->mLink, ++ i) {
    array[i] = cell->mPtr;
  }
  clear();
  return PtArray<T2>(n, array);
}

END_NAMESPACE_YM_VERILOG

#endif // LIBYM_VERILOG_PARSER_MAIN_PTRLIST_H
