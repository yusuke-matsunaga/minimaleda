
/// @file libym_utils/SimpleAlloc.cc
/// @brief SimpleAlloc の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: SimpleAlloc.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_utils/Alloc.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
// クラス SimpleAlloc
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SimpleAlloc::SimpleAlloc(size_t max_size) :
  mUsedSize(0),
  mMaxUsedSize(0),
  mAllocSize(0),
  mAllocCount(0)
{
  assert_cond(max_size > 0, __FILE__, __LINE__);
  mMaxSize = align(max_size);
  mCurBlock = NULL;
  mNextPos = mMaxSize;
}

// デストラクタ
SimpleAlloc::~SimpleAlloc()
{
  destroy();
}

// @brief n バイトの領域を確保する．
void*
SimpleAlloc::get_memory(size_t n)
{
  if ( n == 0 ) {
    return NULL;
  }
  
  mUsedSize += n;
  if ( mMaxUsedSize < mUsedSize ) {
    mMaxUsedSize = mUsedSize;
  }

  if ( n > mMaxSize ) {
    // mMaxSize を越えるものは普通にアロケートする．
    mAllocSize += n;
    ++ mAllocCount;
    return ::operator new(n);
  }
  
  // ワード境界に乗るようにサイズに整える．
  size_t alloc_size = align(n);
  if ( mNextPos + alloc_size > mMaxSize ) {
    mCurBlock = new char[mMaxSize];
    mNextPos = 0;
    mAllocList.push_back(mCurBlock);
    mAllocSize += mMaxSize;
    ++ mAllocCount;
  }
  char* p = &mCurBlock[mNextPos];
  mNextPos += alloc_size;

  return static_cast<void*>(p);
}

// @brief n バイトの領域を開放する．
void
SimpleAlloc::put_memory(size_t n,
			void* block)
{
  mUsedSize -= n;
  
  if ( n > mMaxSize ) {
    // mMaxSize を越えるものは普通に開放する．
    ::operator delete(block);
  }
  
  // このクラスでは領域の再利用はしない．
  return;
}

// @brief 今までに確保した全ての領域を破棄する．
void
SimpleAlloc::destroy()
{
  for (list<char*>::iterator p = mAllocList.begin();
       p != mAllocList.end(); ++ p) {
    delete [] *p;
  }
  mAllocList.clear();
  mCurBlock = NULL;
  mNextPos = mMaxSize;
}

// @brief 使用されているメモリ量を返す．
size_t
SimpleAlloc::used_size() const
{
  return mUsedSize;
}

// @brief used_size() の最大値を返す．
size_t
SimpleAlloc::max_used_size() const
{
  return mMaxUsedSize;
}

// @brief 実際に確保したメモリ量を返す．
size_t
SimpleAlloc::allocated_size() const
{
  return mAllocSize;
}

// @brief 実際に確保した回数を返す．
size_t
SimpleAlloc::allocated_count() const
{
  return mAllocCount;
}

// @brief 内部状態を出力する．
void
SimpleAlloc::print_stats(ostream& s) const
{
  s << "maximum used size: " << max_used_size() << endl
    << "current used size: " << used_size() << endl
    << "allocated size:    " << allocated_size() << endl
    << "allocated count:   " << allocated_count() << endl
    << endl;
}

// @brief アラインメントを考慮してサイズを調節する．
size_t
SimpleAlloc::align(size_t req_size)
{
  return ((req_size + ALIGNOF_DOUBLE - 1) / ALIGNOF_DOUBLE) * ALIGNOF_DOUBLE;
}

END_NAMESPACE_YM
