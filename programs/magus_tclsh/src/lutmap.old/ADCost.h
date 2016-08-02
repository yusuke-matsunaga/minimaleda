#ifndef MAGUS_LUTMAP_ADCOST_H
#define MAGUS_LUTMAP_ADCOST_H

/// @file magus/lutmap/ADCost.h
/// @brief ADCost のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: ADCost.h 2274 2009-06-10 07:45:29Z matsunaga $
///
/// Copyright (C) 2005-2007 Yusuke Matsunaga
/// All rights reserved.

#include "lutmap.h"


BEGIN_NAMESPACE_MAGUS_LUTMAP

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
  ADCost() :
    mCut(NULL),
    mDepth(0),
    mLink(NULL)
  {
  }

  /// @brief デストラクタ
  ~ADCost()
  {
  }


public:

  /// @brief カットを取り出す．
  const Cut*
  cut() const
  {
    return mCut;
  }

  /// @brief 段数を取り出す．
  int
  depth() const
  {
    return mDepth;
  }

  /// @brief 面積を取り出す．
  AreaT
  area() const
  {
    return mArea;
  }
  

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
  ADCostIterator() :
    mPtr(NULL)
  {
  }
  
  /// @brief コピーコンストラクタ
  ADCostIterator(const ADCostIterator& src) :
    mPtr(src.mPtr)
  {
  }
  
  /// @brief 要素を取り出す．
  ADCost<AreaT>*
  operator*() const
  {
    return mPtr;
  }
  
  /// @brief 次の要素に移動する．
  void
  operator++()
  {
    if ( mPtr ) {
      mPtr = mPtr->mLink;
    }
  }
  
  /// @brief 末尾の時に true を返す．
  bool
  is_end() const
  {
    return mPtr == NULL;
  }
  
  
private:
  
  /// @brief ポインタを指定したコンストラクタ
  ADCostIterator(ADCost<AreaT>* ptr) :
    mPtr(ptr)
  {
  }
  
  
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
  ADCostMgr() :
    mTopPage(NULL),
    mNextPos(0),
    mAvail(NULL)
  {
  }

  /// @brief デストラクタ
  ~ADCostMgr()
  {
    for (Page* page = mTopPage; page; ) {
      Page* p = page;
      page = p->mLink;
      delete p;
    }
  }


public:

  /// @brief ADCost を確保する．
  ADCost<AreaT>*
  alloc_cost()
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

  /// @brief ADCost を再利用リストにもどす
  void
  delete_cost(ADCost<AreaT>* cost)
  {
    cost->mLink = mAvail;
    mAvail = cost;
  }

  /// @brief 確保したページ数を返す．
  ymuint
  alloc_num() const
  {
    ymuint n = 0;
    for (Page* page = mTopPage; page; page = page->mLink) {
      ++ n;
    }
    return n;
  }


private:

  static
  const ymuint kChunkSize = 1024;
  
  struct Page
  {
    ADCost<AreaT> mChunk[kChunkSize];
    Page* mLink;
  };
  

private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  Page* mTopPage;

  ymuint32 mNextPos;

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
  ADCostList()
  {
  }

  /// @brief デストラクタ
  ~ADCostList()
  {
  }

  /// @brief ADCostMgr を設定する．
  void
  set_mgr(ADCostMgr<AreaT>* mgr)
  {
    mMgr = mgr;
  }
  

public:

  /// @brief 先頭の反復子を取り出す．
  ADCostIterator<AreaT>
  begin()
  {
    return ADCostIterator<AreaT>(mTop.mLink);
  }

  /// @brief 要素を追加する．
  void
  insert(const Cut* cut,
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
  

private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////
  
  // 先頭の前のダミー要素
  ADCost<AreaT> mTop;

  ADCostMgr<AreaT>* mMgr;
  
};


END_NAMESPACE_MAGUS_LUTMAP

#endif // MAGUS_LUTMAP_ADCOST_H
