
/// @file libym_utils/FragAlloc.cc
/// @brief FragAlloc の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: FragAlloc.cc 1052 2007-10-24 11:08:51Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_utils/Alloc.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
// クラス FragAlloc
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
FragAlloc::FragAlloc(size_t max_size) :
  mMaxSize(max_size),
  mUsedSize(0),
  mMaxUsedSize(0),
  mAllocSize(0),
  mAllocCount(0)
{
  mMinSize = 1;
  mMinLogSize = 0;
  for ( ; mMinSize < ALIGNOF_DOUBLE; mMinSize <<= 1, ++ mMinLogSize) ;
  assert_cond(mMinSize == ALIGNOF_DOUBLE, __FILE__, __LINE__);
  
  assert_cond(max_size > 0, __FILE__, __LINE__);
  mMaxPowerSize = 1;
  mMaxLogSize = 0;
  for ( ; mMaxPowerSize < max_size; mMaxPowerSize <<= 1, ++ mMaxLogSize) ;

  mBlockListArray = new Block*[mMaxLogSize - mMinLogSize + 1];
  for (size_t i = mMinLogSize; i <= mMaxLogSize; ++ i ) {
    mBlockListArray[i - mMinLogSize] = NULL;
  }
}

// デストラクタ
FragAlloc::~FragAlloc()
{
  destroy();
  delete [] mBlockListArray;
}

// @brief n バイトの領域を確保する．
void*
FragAlloc::get_memory(size_t n)
{
  if ( n == 0 ) {
    return NULL;
  }
  
  mUsedSize += n;
  if ( mMaxUsedSize < mUsedSize ) {
    mMaxUsedSize = mUsedSize;
  }

  if ( n > mMaxSize ) {
    mAllocSize += n;
    ++ mAllocCount;
    return static_cast<void*>(new char[n]);
  }
  
  // 2の巾乗のサイズに整える．
  size_t alloc_size = mMinSize;
  size_t pos0 = mMinLogSize;
  while ( alloc_size < n ) {
    alloc_size <<= 1;
    ++ pos0;
  }
  char* ans = alloc_block(pos0);
  return static_cast<void*>(ans);
}

// @brief n バイトの領域を開放する．
void
FragAlloc::put_memory(size_t n,
		      void* block)
{
  if ( n == 0 ) {
    return;
  }

  mUsedSize -= n;

  char* cblock = static_cast<char*>(block);
  
  if ( n > mMaxSize ) {
    delete [] cblock;
  }
  
  // 2の巾乗のサイズに整える．
  size_t alloc_size = mMinSize;
  size_t pos0 = mMinLogSize;
  while ( alloc_size < n ) {
    alloc_size <<= 1;
    ++ pos0;
  }
  put_block(pos0, cblock);
}

// @brief 今までに確保した全ての領域を破棄する．
void
FragAlloc::destroy()
{
  for (size_t i = mMinLogSize; i <= mMaxLogSize; ++ i ) {
    mBlockListArray[i - mMinLogSize] = NULL;
  }
  for (list<char*>::iterator p = mAllocList.begin();
       p != mAllocList.end(); ++ p) {
    delete [] *p;
  }
  mAllocList.clear();
}

// サイズ 2^p のブロックを確保する．
char*
FragAlloc::alloc_block(size_t p)
{
  char* block = get_block(p);
  if ( block ) {
    return block;
  }
  else if ( p == mMaxLogSize ) {
    char* block = new char[mMaxPowerSize];
    mAllocList.push_back(block);
    mAllocSize += mMaxPowerSize;
    ++ mAllocCount;
    return block;
  }
  else {
    char* block = alloc_block(p + 1);
    char* block1 = &block[(1 << p)];
    put_block(p, block1);
    return block;
  }
}

// サイズ 2^p のブロックがあれば返す．
// なければ NULL を返す．
char*
FragAlloc::get_block(size_t p)
{
  Block* b = mBlockListArray[p - mMinLogSize];
  if ( b ) {
    mBlockListArray[p - mMinLogSize] = b->mLink;
    return reinterpret_cast<char*>(b);
  }
  return NULL;
}

// サイズ 2^p のブロックをリストに戻す．
void
FragAlloc::put_block(size_t p,
		     char* block)
{
  Block* b = reinterpret_cast<Block*>(block);
  b->mLink = mBlockListArray[p - mMinLogSize];
  mBlockListArray[p - mMinLogSize] = b;
}

// @brief 使用されているメモリ量を返す．
size_t
FragAlloc::used_size() const
{
  return mUsedSize;
}

// @brief used_size() の最大値を返す．
size_t
FragAlloc::max_used_size() const
{
  return mMaxUsedSize;
}

// @brief 実際に確保したメモリ量を返す．
size_t
FragAlloc::allocated_size() const
{
  return mAllocSize;
}

// @brief 実際に確保した回数を返す．
size_t
FragAlloc::allocated_count() const
{
  return mAllocCount;
}

// @brief 内部状態を出力する．
void
FragAlloc::print_stats(ostream& s) const
{
  s << "maximum used size: " << max_used_size() << endl
    << "current used size: " << used_size() << endl
    << "allocated size:    " << allocated_size() << endl
    << "allocated count:   " << allocated_count() << endl
    << endl;
}

END_NAMESPACE_YM
