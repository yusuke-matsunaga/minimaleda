#ifndef LIBYM_LUTMAP_ADCOST_H
#define LIBYM_LUTMAP_ADCOST_H

/// @file libym_lutmap/ADCost.h
/// @brief ADCost のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: ADCost.h 2274 2009-06-10 07:45:29Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.

#include "ym_lutmap/lutmap_nsdef.h"


BEGIN_NAMESPACE_YM_LUTMAP

class Cut;

template <typename AreaT>
class ADCostList;

template <typename AreaT>
class ADCostIterator;

template <typename AreaT>
class ADCostMgr;

//////////////////////////////////////////////////////////////////////
/// @class ADCost ADCost.h "ADCost.h"
/// @brief 面積と段数のコストを表すクラス
//////////////////////////////////////////////////////////////////////
template <typename AreaT>
class ADCost
{
  friend class ADCostList<AreaT>;
  friend class ADCostIterator<AreaT>;
  friend class ADCostMgr<AreaT>;

public:

  /// @brief 空のコンストラクタ
  ADCost();

  /// @brief デストラクタ
  ~ADCost();


public:

  /// @brief カットを取り出す．
  const Cut*
  cut() const;

  /// @brief 段数を取り出す．
  int
  depth() const;

  /// @brief 面積を取り出す．
  AreaT
  area() const;
  

private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // カット
  const Cut* mCut;

  // 深さ
  int mDepth;

  // 面積
  AreaT mArea;

  // 次の要素を指すリンク
  ADCost* mLink;

};


//////////////////////////////////////////////////////////////////////
/// @class ADCostIterator ADCost.h "ADCost.h"
/// @brief ADCostList 用の反復子
//////////////////////////////////////////////////////////////////////
template <typename AreaT>
class ADCostIterator
{
  friend class ADCostList<AreaT>;

public:

  /// @brief 空のコンストラクタ
  ADCostIterator();
  
  /// @brief コピーコンストラクタ
  /// @param[in] src コピー元のオブジェクト
  ADCostIterator(const ADCostIterator& src);
  
  /// @brief 要素を取り出す．
  ADCost<AreaT>*
  operator*() const;
  
  /// @brief 次の要素に移動する．
  void
  operator++();
  
  /// @brief 末尾の時に true を返す．
  bool
  is_end() const;
  
  
private:
  
  /// @brief ポインタを指定したコンストラクタ
  /// @param[in] ptr 要素を指すポインタ
  ADCostIterator(ADCost<AreaT>* ptr);
  
  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////
  
  // 実際の要素へのポインタ
  ADCost<AreaT>* mPtr;
  
};


//////////////////////////////////////////////////////////////////////
/// @class ADCostMgr ADCost.h "ADCost.h"
/// @brief ADCost の確保/解放を行うクラス
//////////////////////////////////////////////////////////////////////
template <typename AreaT>
class ADCostMgr
{
public:

  /// @brief コンストラクタ
  ADCostMgr();

  /// @brief デストラクタ
  /// @note このオブジェクトが確保したすべてのメモリを開放する．
  ~ADCostMgr();


public:

  /// @brief ADCost を確保する．
  /// @return 確保したオブジェクトを返す．
  ADCost<AreaT>*
  alloc_cost();

  /// @brief ADCost を再利用リストにもどす
  /// @param[in] cost 戻すオブジェクト
  void
  delete_cost(ADCost<AreaT>* cost);

  /// @brief 確保したページ数を返す．
  ymuint
  alloc_num() const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いる定数とデータ構造
  //////////////////////////////////////////////////////////////////////
  
  // 1ページのサイズ
  static
  const ymuint kChunkSize = 1024;

  // 1ページの構造体
  struct Page
  {
    // コスト本体の配列
    ADCost<AreaT> mChunk[kChunkSize];

    // 次のページを指すリンク
    Page* mLink;
  };
  

private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////
  
  // 先頭のページ
  Page* mTopPage;
  
  // 次に使用可能な位置
  ymuint32 mNextPos;
  
  // 再利用リスト
  ADCost<AreaT>* mAvail;

};


//////////////////////////////////////////////////////////////////////
/// @class ADCostList ADCost.h "ADCost.h"
/// @brief ADCost のリストを表すクラス
//////////////////////////////////////////////////////////////////////
template <typename AreaT>
class ADCostList
{
public:

  /// @brief コンストラクタ
  /// @note 空のリストで初期化される．
  ADCostList();

  /// @brief デストラクタ
  ~ADCostList();

  /// @brief ADCostMgr を設定する．
  /// @param[in] mgr ADCost のメモリ管理を行うオブジェクト
  void
  set_mgr(ADCostMgr<AreaT>* mgr);
  

public:

  /// @brief 先頭の反復子を取り出す．
  ADCostIterator<AreaT>
  begin();

  /// @brief 要素を追加する．
  /// @param[in] cut カット
  /// @param[in] depth 深さ
  /// @param[in] area 面積コスト
  /// @note 適当な位置に挿入される．
  /// @note ただし，深さと面積の両方で他の要素より悪い場合には追加されない．
  /// @note 逆にこの要素によって無効化される要素があればそれは削除される．
  void
  insert(const Cut* cut,
	 int depth,
	 AreaT area);
  

private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////
  
  // 先頭の前のダミー要素
  ADCost<AreaT> mTop;
  
  // メモリ管理オブジェクト
  ADCostMgr<AreaT>* mMgr;
  
};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// クラス ADCost
//////////////////////////////////////////////////////////////////////

// @brief 空のコンストラクタ
template <typename AreaT>
inline
ADCost<AreaT>::ADCost() :
  mCut(NULL),
  mDepth(0),
  mLink(NULL)
{
}

// @brief デストラクタ
template <typename AreaT>
inline
ADCost<AreaT>::~ADCost()
{
}

// @brief カットを取り出す．
template <typename AreaT>
inline
const Cut*
ADCost<AreaT>::cut() const
{
  return mCut;
}

// @brief 段数を取り出す．
template <typename AreaT>
inline
int
ADCost<AreaT>::depth() const
{
  return mDepth;
}

// @brief 面積を取り出す．
template <typename AreaT>
inline
AreaT
ADCost<AreaT>::area() const
{
  return mArea;
}


//////////////////////////////////////////////////////////////////////
// クラス ADCostIterator
//////////////////////////////////////////////////////////////////////

// @brief 空のコンストラクタ
template <typename AreaT>
inline
ADCostIterator<AreaT>::ADCostIterator() :
  mPtr(NULL)
{
}

// @brief コピーコンストラクタ
template <typename AreaT>
inline
ADCostIterator<AreaT>::ADCostIterator(const ADCostIterator& src) :
  mPtr(src.mPtr)
{
}

// @brief 要素を取り出す．
template <typename AreaT>
inline
ADCost<AreaT>*
ADCostIterator<AreaT>::operator*() const
{
  return mPtr;
}

// @brief 次の要素に移動する．
template <typename AreaT>
inline
void
ADCostIterator<AreaT>::operator++()
{
  if ( mPtr ) {
    mPtr = mPtr->mLink;
  }
}

// @brief 末尾の時に true を返す．
template <typename AreaT>
inline
bool
ADCostIterator<AreaT>::is_end() const
{
  return mPtr == NULL;
}

// @brief ポインタを指定したコンストラクタ
template <typename AreaT>
inline
ADCostIterator<AreaT>::ADCostIterator(ADCost<AreaT>* ptr) :
  mPtr(ptr)
{
}


//////////////////////////////////////////////////////////////////////
// クラス ADCostMgr
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
template <typename AreaT>
inline
ADCostMgr<AreaT>::ADCostMgr() :
  mTopPage(NULL),
  mNextPos(0),
  mAvail(NULL)
{
}

// @brief デストラクタ
template <typename AreaT>
inline
ADCostMgr<AreaT>::~ADCostMgr()
{
  for (Page* page = mTopPage; page; ) {
    Page* p = page;
    page = p->mLink;
    delete p;
  }
}

// @brief ADCost を確保する．
template <typename AreaT>
inline
ADCost<AreaT>*
ADCostMgr<AreaT>::alloc_cost()
{
  if ( mAvail ) {
    ADCost<AreaT>* cost = mAvail;
    mAvail = cost->mLink;
    cost->mLink = NULL;
    return cost;
  }
  if ( mTopPage == NULL || mNextPos >= kChunkSize ) {
    Page* page = new Page;
    page->mLink = mTopPage;
    mTopPage = page;
    mNextPos = 0;
  }
  ADCost<AreaT>* cost = &mTopPage->mChunk[mNextPos];
  ++ mNextPos;
  return cost;
}

// @brief ADCost を再利用リストにもどす
template <typename AreaT>
inline
void
ADCostMgr<AreaT>::delete_cost(ADCost<AreaT>* cost)
{
  cost->mLink = mAvail;
  mAvail = cost;
}

// @brief 確保したページ数を返す．
template <typename AreaT>
inline
ymuint
ADCostMgr<AreaT>::alloc_num() const
{
  ymuint n = 0;
  for (Page* page = mTopPage; page; page = page->mLink) {
    ++ n;
  }
  return n;
}


//////////////////////////////////////////////////////////////////////
// クラス ADCostList
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
template <typename AreaT>
inline
ADCostList<AreaT>::ADCostList()
{
}

// @brief デストラクタ
template <typename AreaT>
inline
ADCostList<AreaT>::~ADCostList()
{
}

// @brief ADCostMgr を設定する．
template <typename AreaT>
inline
void
ADCostList<AreaT>::set_mgr(ADCostMgr<AreaT>* mgr)
{
  mMgr = mgr;
}

// @brief 先頭の反復子を取り出す．
template <typename AreaT>
inline
ADCostIterator<AreaT>
ADCostList<AreaT>::begin()
{
  return ADCostIterator<AreaT>(mTop.mLink);
}

// @brief 要素を追加する．
template <typename AreaT>
inline
void
ADCostList<AreaT>::insert(const Cut* cut,
			  int depth,
			  AreaT area)
{
  ADCost<AreaT>* prev = &mTop;
  ADCost<AreaT>* cur;
  while ( (cur = prev->mLink) ) {
    if ( cur->mDepth > depth ) {
      if ( cur->mArea >= area ) {
	// cur は不要
	ADCost<AreaT>* next = cur->mLink;
	cur->mLink = NULL;
	prev->mLink = next;
	mMgr->delete_cost(cur);
      }
      else {
	prev = cur;
      }
    }
    else if ( cur->mDepth == depth ) {
      if ( cur->mArea <= area ) {
	// (depth, area) は不要
	break;
      }
      else {
	// cur は不要 (というか上書きする)
	cur->mCut = cut;
	cur->mArea = area;
	break;
      }
    }
    else { // cur->mDepth < depth
      if ( cur->mArea > area ) {
	// (depth, area) をここに追加
	ADCost<AreaT>* new_cost = mMgr->alloc_cost();
	new_cost->mCut = cut;
	new_cost->mDepth = depth;
	new_cost->mArea = area;
	prev->mLink = new_cost;
	new_cost->mLink = cur;
	break;
      }
      else { // cur->mArea <= area
	// (depth, area) は不要
	break;
      }
    }
  }
  if ( cur == NULL ) {
    cur = mMgr->alloc_cost();
    cur->mCut = cut;
    cur->mDepth = depth;
    cur->mArea = area;
    prev->mLink = cur;
  }
}

END_NAMESPACE_YM_LUTMAP

#endif // LIBYM_LUTMAP_ADCOST_H
