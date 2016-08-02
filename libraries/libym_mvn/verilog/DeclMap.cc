
/// @file libym_mvn/verilog/DeclMap.cc
/// @brief DeclMap の実装クラス
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "DeclMap.h"
#include "ym_mvn/MvNode.h"
#include "ym_verilog/vl/VlDecl.h"


BEGIN_NAMESPACE_YM_MVN_VERILOG

BEGIN_NONAMESPACE

const
bool debug_declmap = false;

END_NONAMESPACE


// @brief コンストラクタ
DeclMap::DeclMap() :
  mAlloc(sizeof(Cell), 1024),
  mNum(0)
{
  alloc_table(1024);
}

// @brief デストラクタ
DeclMap::~DeclMap()
{
  delete [] mTable;
}

// @brief 内部のデータをクリアする．
void
DeclMap::clear()
{
  for (ymuint i = 0; i < mSize; ++ i) {
    mTable[i] = NULL;
  }
  mNum = 0;
  mAlloc.destroy();
}

// @brief 登録する(単一要素の場合)
// @param[in] decl 宣言要素
// @param[in] node 対応するノード
void
DeclMap::add(const VlDecl* decl,
	     MvNode* node)
{
  if ( debug_declmap ) {
    cerr << "DeclMap::add(" << decl->full_name()
	 << ", Node#" << node->id() << ")"
	 << endl;
  }
  Cell* cell = find_cell(decl, 0);
  if ( cell ) {
    cell->mNode = node;
  }
  else {
    put_cell(decl, 0, node);
  }
}

// @brief 登録する(配列の場合)
// @param[in] decl 宣言要素
// @param[in] offset
// @param[in] node 対応するノード
void
DeclMap::add(const VlDecl* decl,
	     ymuint offset,
	     MvNode* node)
{
  if ( debug_declmap ) {
    cerr << "DeclMap::add(" << decl->full_name()
	 << "[" << offset << "]"
	 << ", Node#" << node->id() << ")"
	 << endl;
  }
  Cell* cell = find_cell(decl, offset);
  if ( cell ) {
    cell->mNode = node;
  }
  else {
    put_cell(decl, offset, node);
  }
}

// @brief 対応するノードを取り出す．
// @param[in] decl 宣言要素
// @return 対応するノードを返す．
// @note 登録されていない場合と配列型の場合には NULL を返す．
MvNode*
DeclMap::get(const VlDecl* decl) const
{
  Cell* cell = find_cell(decl, 0);
  if ( cell ) {
    return cell->mNode;
  }
  return NULL;
}

// @brief 対応するノードを取り出す(配列型)．
// @param[in] decl 宣言要素
// @param[in] offset オフセット
// @return 対応するノードを返す．
// @note 登録されていない場合と配列型でない場合，
// オフセットが範囲外の場合には NULL を返す．
MvNode*
DeclMap::get(const VlDecl* decl,
	     ymuint offset) const
{
  Cell* cell = find_cell(decl, offset);
  if ( cell ) {
    return cell->mNode;
  }
  return NULL;
}

// @brief Cell を登録する．
void
DeclMap::put_cell(const VlDecl* decl,
		  ymuint offset,
		  MvNode* node)
{
  if ( mNum >= mLimit ) {
    // テーブルを拡大する．
    ymuint old_size = mSize;
    Cell** old_table = mTable;
    alloc_table(old_size << 1);
    for (ymuint i = 0; i < old_size; ++ i) {
      for (Cell* cell = old_table[i]; cell; ) {
	Cell* next = cell->mLink;
	ymuint pos = hash_func(cell->mDecl, cell->mOffset);
	cell->mLink = mTable[pos];
	mTable[pos] = cell;
	cell = next;
      }
    }
    delete [] old_table;
  }
  ymuint pos = hash_func(decl, offset);
  void* p = mAlloc.get_memory(sizeof(Cell));
  Cell* cell = new (p) Cell;
  cell->mDecl = decl;
  cell->mOffset = offset;
  cell->mNode = node;
  cell->mLink = mTable[pos];
  mTable[pos] = cell;
  ++ mNum;
}

// @brief Cell を探す．
DeclMap::Cell*
DeclMap::find_cell(const VlDecl* decl,
		   ymuint offset) const
{
  ymuint pos = hash_func(decl, offset);
  for (Cell* cell = mTable[pos]; cell; cell = cell->mLink) {
    if ( cell->mDecl == decl && cell->mOffset == offset ) {
      return cell;
    }
  }
  return NULL;
}

// @brief テーブルの領域を確保する．
// @param[in] size 必要なサイズ
void
DeclMap::alloc_table(ymuint size)
{
  mSize = size;
  mLimit = static_cast<ymuint32>(mSize * 1.8);
  mTable = new Cell*[mSize];
  for (ymuint i = 0; i < mSize; ++ i) {
    mTable[i] = NULL;
  }
}

// @brief ハッシュ値を計算する．
ymuint
DeclMap::hash_func(const VlDecl* decl,
		   ymuint offset) const
{
  return (reinterpret_cast<ympuint>(decl) * (offset + 1) + offset) % mSize;
}

END_NAMESPACE_YM_MVN_VERILOG
