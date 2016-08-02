
/// @file libym_verilog/elaborator/AttrDict.cc
/// @brief AttrDict の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: AttrDict.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "AttrDict.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// constant function を格納するハッシュ表
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
AttrDict::AttrDict() :
  mAlloc(sizeof(Cell), 1024),
  mNum(0)
{
  alloc_table(1024);
}

// @brief デストラクタ
AttrDict::~AttrDict()
{
  clear();
}

// @brief 要素を追加する．
// @param[in] pt_attr パース木の属性定義
// @param[in] attr_list 登録する属性のリスト
void
AttrDict::add(const PtAttrInst* pt_attr,
	      ElbAttrList* attr_list)
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
	ymuint pos = hash_func(cell->mPtAttr);
	cell->mLink = mTable[pos];
	mTable[pos] = cell;
	cell = next;
      }
    }
    delete [] old_table;
  }
  ymuint pos = hash_func(pt_attr);
  Cell* cell = reinterpret_cast<Cell*>(mAlloc.get_memory(sizeof(Cell)));
  cell->mPtAttr = pt_attr;
  cell->mAttrList = attr_list;
  cell->mLink = mTable[pos];
  mTable[pos] = cell;
  ++ mNum;
}

// @brief 属性リストを取り出す．
// @param[in] pt_attr パース木の属性定義
ElbAttrList*
AttrDict::find(const PtAttrInst* pt_attr) const
{
  ymuint pos = hash_func(pt_attr);
  for (Cell* cell = mTable[pos]; cell; cell = cell->mLink) {
    if ( cell->mPtAttr == pt_attr ) {
      return cell->mAttrList;
    }
  }
  return NULL;
}
  
// @brief 内容をクリアする．
void
AttrDict::clear()
{
  mAlloc.destroy();
  delete [] mTable;
  mTable = NULL;
}

// @brief このオブジェクトが使用しているメモリ量を返す．
size_t
AttrDict::allocated_size() const
{
  return sizeof(AttrDict) + sizeof(Cell*) * mSize + mAlloc.allocated_size();
}

// @brief テーブルの領域を確保する．
void
AttrDict::alloc_table(ymuint size)
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
AttrDict::hash_func(const PtAttrInst* pt_attr) const
{
  ympuint tmp = reinterpret_cast<ympuint>(pt_attr);
  return ((tmp * tmp) >> 12) % mSize;
}

END_NAMESPACE_YM_VERILOG
