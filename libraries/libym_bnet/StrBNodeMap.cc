
/// @file libym_bnet/StrBNodeMap.cc
/// @brief StrBNodeMap の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: StrBNodeMap.cc 1343 2008-03-25 17:15:35Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "StrBNodeMap.h"


BEGIN_NAMESPACE_YM_BNET

BEGIN_NONAMESPACE

inline
size_t
hash_func(const char* str)
{
  size_t h = 0;
  size_t c;
  for ( ; (c = static_cast<size_t>(*str)); ++ str) {
    h = h * 37 + c;
  }
  return h;
}

END_NONAMESPACE

// @brief コンストラクタ
StrBNodeMap::StrBNodeMap() :
  mAlloc(4096),
  mAvail(NULL),
  mNum(0)
{
  new_table(1024);
}

// @brief デストラクタ
StrBNodeMap::~StrBNodeMap()
{
  delete [] mTable;
}

// @brief 名前を登録する．
bool
StrBNodeMap::reg_name(const char* name,
		      BNode::tId id)
{
  size_t pos0 = hash_func(name);
  size_t pos = pos0 % mTableSize;
  for (Cell* cell = mTable[pos]; cell; cell = cell->mLink) {
    if ( check_name(cell, name) ) {
      // 同名のノードが既に存在する．
      return false;
    }
  }

  if ( mNum >= mNextLimit ) {
    // テーブルを2倍する．
    size_t old_size = mTableSize;
    Cell** old_table = mTable;
    new_table(old_size * 2);
    for (size_t i = 0; i < old_size; ++ i) {
      for (Cell* cell = old_table[i]; cell; ) {
	Cell* cell1 = cell;
	cell = cell->mLink;
	size_t pos = hash_func(cell1->mName) % mTableSize;
	cell1->mLink = mTable[pos];
	mTable[pos] = cell1;
      }
    }
    delete [] old_table;
    // テーブルサイズが変わっているのでハッシュ値を補正する．
    pos = pos0 % mTableSize;
  }

  Cell* cell = new_cell();
  cell->mName = name;
  cell->mId = id;
  cell->mLink = mTable[pos];
  mTable[pos] = cell;
  ++ mNum;

  return true;
}

// @brief 名前を削除する．
void
StrBNodeMap::del_name(const char* name)
{
  size_t pos = hash_func(name) % mTableSize;
  Cell** prev = &mTable[pos];
  while ( *prev ) {
    Cell* cell = *prev;
    if ( check_name(cell, name) ) {
      *prev = cell->mLink;
      free_cell(cell);
      -- mNum;
      return;
    }
    prev = &(cell->mLink);
  }
}

// @brief 内容をクリアする．
void
StrBNodeMap::clear()
{
  for (size_t i = 0; i < mTableSize; ++ i) {
    mTable[i] = NULL;
  }
  mNum = 0;
}

// @brief 名前からノード番号を取り出す．
BNode::tId
StrBNodeMap::find_id(const char* name) const
{
  size_t pos = hash_func(name) % mTableSize;
  for (Cell* cell = mTable[pos]; cell; cell = cell->mLink) {
    if ( check_name(cell, name) ) {
      return cell->mId;
    }
  }
  return 0;
}

// ハッシュ表の領域を確保する．
void
StrBNodeMap::new_table(size_t req_size)
{
  mTableSize = req_size;
  mNextLimit = static_cast<size_t>(mTableSize * 1.8);
  mTable = new Cell*[mTableSize];
  for (size_t i = 0; i < mTableSize; ++ i) {
    mTable[i] = NULL;
  }
}

// Cell を取ってくる．
StrBNodeMap::Cell*
StrBNodeMap::new_cell()
{
  Cell* cell = mAvail;
  if ( cell ) {
    mAvail = cell->mLink;
  }
  else {
    cell = static_cast<Cell*>(mAlloc.get_memory(sizeof(Cell)));
  }
  return cell;
}

// Cell を未使用にする．
void
StrBNodeMap::free_cell(Cell* cell)
{
  cell->mLink = mAvail;
  mAvail = cell;
}

END_NAMESPACE_YM_BNET
