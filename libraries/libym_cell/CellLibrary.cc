
/// @file libym_cell/CellLibrary.cc
/// @brief CellLibrary の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: CellLibrary.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_cell/CellLibrary.h"
#include "ym_cell/Cell.h"
#include "ym_cell/CellLut.h"
#include "ym_cell/CellBus.h"


BEGIN_NAMESPACE_YM_CELL

#if 0
// @brief コンストラクタ
CellLibrary::CellLibrary() :
  mAlloc(4096)
{
}

// @brief デストラクタ
CellLibrary::~CellLibrary()
{
}

// @brief 名前からのセルの取得
const Cell*
CellLibrary::cell(const char* name) const
{
  ShString tmp(name);
  hash_map<ShString, Cell*>::const_iterator p = mCellDict.find(tmp);
  if ( p == mCellDict.end() ) {
    return NULL;
  }
  return p->second;
}

// @brief ルックアップテーブルのテンプレートの取得
// @param[in] name テンプレート名
// @note なければ NULL を返す．
const CellLutTemplate*
CellLibrary::lu_table_template(const char* name) const
{
  ShString tmp(name);
  hash_map<ShString, CellLutTemplate*>::const_iterator p = mLutTemplateDict.find(tmp);
  if ( p == mLutTemplateDict.end() ) {
    return NULL;
  }
  return p->second;
}

// @brief バスタイプの取得
// @param[in] name バスタイプ名
// @note なければ NULL を返す．
const CellBusType*
CellLibrary::bus_type(const char* name) const
{
  ShString tmp(name);
  hash_map<ShString, CellBusType*>::const_iterator p = mBusTypeDict.find(tmp);
  if ( p == mBusTypeDict.end() ) {
    return NULL;
  }
  return p->second;
}
#endif

END_NAMESPACE_YM_CELL
