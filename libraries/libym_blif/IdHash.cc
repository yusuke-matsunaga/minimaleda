
/// @file libym_blif/IdHash.cc
/// @brief BlibParser の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: IdHash.cc 1921 2008-12-21 13:55:05Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "IdHash.h"


BEGIN_NAMESPACE_YM_BLIF

//////////////////////////////////////////////////////////////////////
// クラス IdCell
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
IdCell::IdCell(ymuint32 id,
	       const char* str) :
  mId(id),
  mFlags(0U),
  mLink(NULL)
{
  const char* s = str;
  char* d = mStr;
  while ( ((*d ++) = (*s ++)) ) ;
}

// @brief デストラクタ
IdCell::~IdCell()
{
}


//////////////////////////////////////////////////////////////////////
// クラス IdHash
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
IdHash::IdHash() :
  mAlloc(4096),
  mTableSize(0)
{
  alloc_table(1024);
}

// @brief デストラクタ
IdHash::~IdHash()
{
  clear();
  delete [] mTable;
}

// @brief 内容をクリアする．
void
IdHash::clear()
{
  for (size_t i = 0; i < mTableSize; ++ i) {
    mTable[i] = NULL;
  }
  mCellArray.clear();
  mAlloc.destroy();
}


BEGIN_NONAMESPACE

inline
ymuint
hash_func(const char* str)
{
  ymuint h = 0;
  for (char c; (c = *str); ++ str) {
    h = h * 33 + static_cast<ymuint>(c);
  }
  return h;
}

END_NONAMESPACE

// @brief 識別子に対応するセルを探す．
// @param[in] str 文字列
// @param[in] create 存在しないときに新規生成するなら true
// @return 対応する IdCell を返す．
IdCell*
IdHash::find(const char* str,
	     bool create)
{
  assert_cond(str, __FILE__, __LINE__);
  size_t pos0 = hash_func(str);
  size_t pos = pos0 % mTableSize;
  for (IdCell* cell = mTable[pos]; cell; cell = cell->mLink) {
    if ( strcmp(cell->mStr, str) == 0 ) {
      return cell;
    }
  }

  if ( !create ) {
    return NULL;
  }
  
  if ( mCellArray.size() >= mNextLimit ) {
    // テーブルを拡張する．
    IdCell** old_table = mTable;
    size_t old_size = mTableSize;
    alloc_table(old_size * 2);
    for (size_t i = 0; i < old_size; ++ i) {
      for (IdCell* cell = old_table[i]; cell; ) {
	IdCell* next = cell->mLink;
	size_t pos1 = hash_func(cell->mStr) % mTableSize;
	cell->mLink = mTable[pos1];
	mTable[pos1] = cell;
	cell = next;
      }
    }
  }
  
  // 新しいセルを確保する．
  size_t l = strlen(str);
  size_t reqsize = sizeof(IdCell) + l;
  void* p = mAlloc.get_memory(reqsize);
  IdCell* cell = new (p) IdCell(mCellArray.size(), str);
  mCellArray.push_back(cell);
  
  // テーブルに追加する．
  pos = pos0 % mTableSize;
  cell->mLink = mTable[pos];
  mTable[pos] = cell;
  return cell;
}

// ハッシュ表を拡大する．
void
IdHash::alloc_table(size_t new_size)
{
  mTable = new IdCell*[new_size];
  mTableSize = new_size;
  mNextLimit = static_cast<size_t>(mTableSize * 1.8);
  for (size_t i = 0; i < new_size; ++ i) {
    mTable[i] = NULL;
  }
}

END_NAMESPACE_YM_BLIF
