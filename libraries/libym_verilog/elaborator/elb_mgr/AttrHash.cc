
/// @file libym_verilog/elb/AttrHash.cc
/// @brief AttrHash の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: AttrHash.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "AttrHash.h"
#include "ym_verilog/vl/VlObj.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス AttrHash
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] alloc メモリ確保用のアロケータ
AttrHash::AttrHash(AllocBase& alloc) :
  mAlloc(alloc),
  mNum(0)
{
  alloc_table(1024);
}

// @brief デストラクタ
AttrHash::~AttrHash()
{
  delete [] mTable;
}
  
// @brief 内容をクリアする．
void
AttrHash::clear()
{
  for (ymuint32 i = 0; i < mSize; ++ i ) {
    mTable[i] = NULL;
  }
  mNum = 0;
}

// @brief 属性を追加する．
// @param[in] obj 対象のオブジェクト
// @param[in] def 定義側の属性の時 true とするフラグ
// @param[in] attr_list 付加する属性リスト
void
AttrHash::add(const VlObj* obj,
	      bool def,
	      ElbAttrList* attr_list)
{
  // 該当の Cell が存在するか調べる．
  Cell* cell = find_cell(obj);
  if ( !cell ) {
    cell = new_cell(obj);
  }
  ymuint32 pos = (def) ? 1 : 0;
  assert_cond(cell->mAttrList[pos] == NULL, __FILE__, __LINE__);
  cell->mAttrList[pos] = attr_list;
}
      
// @brief 属性を取り出す．
// @param[in] obj 対象のオブジェクト
// @param[in] def 定義側の属性の時 true とするフラグ
ElbAttrList*
AttrHash::find(const VlObj* obj,
	       bool def) const
{
  // 該当の Cell が存在するか調べる．
  Cell* cell = find_cell(obj);
  if ( cell ) {
    ymuint32 pos = (def) ? 1 : 0;
    return cell->mAttrList[pos];
  }
  return NULL;
}

// @brief 新しい Cell を生成する．
// @param[in] obj 対象のオブジェクト
AttrHash::Cell*
AttrHash::new_cell(const VlObj* obj)
{
  if ( mNum >= mLimit ) {
    // テーブルを拡張する．
    ymuint32 old_size = mSize;
    Cell** old_table = mTable;
    alloc_table(old_size << 1);
    for (ymuint32 i = 0; i < old_size; ++ i) {
      for (Cell* cell = old_table[i]; cell; ) {
	Cell* next = cell->mLink;
	ymuint32 pos = hash_func(cell->mObj);
	cell->mLink = mTable[pos];
	mTable[pos] = cell;
	cell = next;
      }
    }
    delete [] old_table;
  }
  ymuint32 pos = hash_func(obj);
  void* p = mAlloc.get_memory(sizeof(Cell));
  Cell* cell = new (p) Cell;
  cell->mObj = obj;
  cell->mAttrList[0] = NULL;
  cell->mAttrList[1] = NULL;
  cell->mLink = mTable[pos];
  mTable[pos] = cell;
  ++ mNum;
  return cell;
}
  
// @brief オブジェクトから対応する Cell を取り出す．
// @param[in] obj 対象のオブジェクト
AttrHash::Cell*
AttrHash::find_cell(const VlObj* obj) const
{
  ymuint32 pos = hash_func(obj);
  for (Cell* cell = mTable[pos]; cell; cell = cell->mLink) {
    if ( cell->mObj == obj ) {
      return cell;
    }
  }
  return NULL;
}

// @brief このオブジェクトが使用しているメモリ量を返す．
size_t
AttrHash::allocated_size() const
{
  return sizeof(Cell*) * mSize;
}

// @brief テーブルの領域を確保する．
void
AttrHash::alloc_table(ymuint32 size)
{
  mSize = size;
  mLimit = static_cast<ymuint32>(mSize * 1.8);
  mTable = new Cell*[mSize];
  for (ymuint32 i = 0; i < mSize; ++ i) {
    mTable[i] = NULL;
  }
}

// @brief ハッシュ値を計算する．
ymuint32
AttrHash::hash_func(const VlObj* obj) const
{
  ympuint tmp = reinterpret_cast<ympuint>(obj);
  return ((tmp * tmp) >> 12) % mSize;
}


END_NAMESPACE_YM_VERILOG
