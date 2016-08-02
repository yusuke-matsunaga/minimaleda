
/// @file libym_verilog/elaborator/CfDict.cc
/// @brief CfDict の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: CfDict.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "CfDict.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// constant function を格納するハッシュ表
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
CfDict::CfDict() :
  mAlloc(sizeof(Cell), 1024),
  mNum(0)
{
  alloc_table(1024);
}

// @brief デストラクタ
CfDict::~CfDict()
{
  clear();
}

// @brief 要素を追加する．
// @param[in] scope 親のスコープ
// @param[in] name 名前
// @param[in] func 登録する関数
void
CfDict::add(const VlNamedObj* scope,
	    const char* name,
	    ElbTaskFunc* func)
{
  if ( mTable == NULL ) {
    alloc_table(1024);
  }
  if ( mNum >= mLimit ) {
    // テーブルを拡張する．
    ymuint old_size = mSize;
    Cell** old_table = mTable;
    alloc_table(old_size << 1);
    for (ymuint i = 0; i < old_size; ++ i) {
      for (Cell* cell = old_table[i]; cell; ) {
	Cell* next = cell->mLink;
	ymuint pos = hash_func(cell->mScope, cell->mName);
	cell->mLink = mTable[pos];
	mTable[pos] = cell;
	cell = next;
      }
    }
    delete [] old_table;
  }
  ymuint pos = hash_func(scope, name);
  Cell* cell = reinterpret_cast<Cell*>(mAlloc.get_memory(sizeof(Cell)));
  cell->mScope = scope;
  cell->mName = name;
  cell->mFunc = func;
  cell->mLink = mTable[pos];
  mTable[pos] = cell;
  ++ mNum;
}

// @brief 名前から該当する要素を検索する．
// @param[in] scope 親のスコープ
// @param[in] name 名前
ElbTaskFunc*
CfDict::find(const VlNamedObj* scope,
	     const char* name) const
{
  ymuint pos = hash_func(scope, name);
  for (Cell* cell = mTable[pos]; cell; cell = cell->mLink) {
    if ( cell->mScope == scope && strcmp(cell->mName, name) == 0 ) {
      return cell->mFunc;
    }
  }
  return NULL;
}
  
// @brief 内容をクリアする．
void
CfDict::clear()
{
  mAlloc.destroy();
  delete [] mTable;
  mTable = NULL;
}

// @brief このオブジェクトが使用しているメモリ量を返す．
size_t
CfDict::allocated_size() const
{
  return sizeof(CfDict) + sizeof(Cell*) * mSize + mAlloc.allocated_size();
}

// @brief テーブルの領域を確保する．
void
CfDict::alloc_table(ymuint size)
{
  mSize = size;
  mLimit = static_cast<ymuint>(mSize * 1.8);
  mTable = new Cell*[mSize];
  for (ymuint i = 0; i < mSize; ++ i) {
    mTable[i] = NULL;
  }
}

// @brief ハッシュ値を計算する．
ymuint
CfDict::hash_func(const VlNamedObj* scope,
		  const char* name) const
{
  ymuint h = 0;
  ymuint c;
  for ( ; (c = static_cast<ymuint>(*name)); ++ name) {
    h = h * 37 + c;
  }
  return ((reinterpret_cast<ympuint>(scope) * h) >> 8) % mSize;
}


END_NAMESPACE_YM_VERILOG
