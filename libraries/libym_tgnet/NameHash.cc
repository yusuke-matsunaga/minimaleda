
/// @file libym_tgnet/NameHash.cc
/// @brief NameHash の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: NameHash.cc 1920 2008-12-20 15:52:42Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "NameHash.h"
#include "ym_tgnet/TgNode.h"


BEGIN_NAMESPACE_YM_TGNET

//////////////////////////////////////////////////////////////////////
// クラス NameHash
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
NameHash::NameHash() :
  mAlloc(4096),
  mTableSize(0),
  mNum(0)
{
  alloc_table(1024);
}

// @brief デストラクタ
NameHash::~NameHash()
{
  delete [] mHashTable;
}

// @brief 内容をクリアする．
void
NameHash::clear()
{
  for (size_t i = 0; i < mTableSize; ++ i) {
    mHashTable[i] = NULL;
  }
  mAlloc.destroy();
  mNum = 0;
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

// @brief 名前からノードを検索する
// @param[in] name 検索対象の名前
// @return name という名前のノードを返す．
TgNode*
NameHash::find_node(const char* name)
{
  assert_cond(name, __FILE__, __LINE__);
  size_t pos = hash_func(name) % mTableSize;
  for (StrIdCell* cell = mHashTable[pos]; cell; cell = cell->mLink) {
    if ( strcmp(cell->mStr, name) == 0 ) {
      return cell->mNode;
    }
  }
  return NULL;
}
  
// @brief ノードを登録する．
const char*
NameHash::reg_node(const char* name,
		   TgNode* node)
{
  if ( mNum >= mNextLimit ) {
    // テーブルを拡張する．
    StrIdCell** old_table = mHashTable;
    size_t old_size = mTableSize;
    alloc_table(old_size * 2);
    for (size_t i = 0; i < old_size; ++ i) {
      for (StrIdCell* cell = old_table[i]; cell; ) {
	StrIdCell* next = cell->mLink;
	size_t pos1 = hash_func(cell->mStr) % mTableSize;
	cell->mLink = mHashTable[pos1];
	mHashTable[pos1] = cell;
	cell = next;
      }
    }
  }
  
  // 新しいセルを確保する．
  size_t size = sizeof(StrIdCell) + strlen(name);
  void* p = mAlloc.get_memory(size);
  StrIdCell* cell = new (p) StrIdCell(node, name);

  // テーブルに追加する．
  size_t pos0 = hash_func(name);
  size_t pos = pos0 % mTableSize;
  cell->mLink = mHashTable[pos];
  mHashTable[pos] = cell;

  return cell->mStr;
}

// ハッシュ表を拡大する．
void
NameHash::alloc_table(size_t new_size)
{
  mHashTable = new StrIdCell*[new_size];
  mTableSize = new_size;
  mNextLimit = static_cast<size_t>(mTableSize * 1.8);
  for (size_t i = 0; i < new_size; ++ i) {
    mHashTable[i] = NULL;
  }
}

// コンストラクタ
NameHash::StrIdCell::StrIdCell(TgNode* node,
			       const char* name) :
  mNode(node)
{
  char* d = mStr;
  const char* s = name;
  while ( (*d ++ = *s ++) ) ;
}

END_NAMESPACE_YM_TGNET
