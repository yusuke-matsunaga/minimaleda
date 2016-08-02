#ifndef LIBYM_TECHMAP_FUNCGROUP_H
#define LIBYM_TECHMAP_FUNCGROUP_H

/// @file libym_techmap/FuncGroup.h
/// @brief FuncGroup のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_techmap/techmap_nsdef.h"
#include "ym_cell/cell_nsdef.h"
#include "ym_npn/NpnMap.h"


BEGIN_NAMESPACE_YM_TECHMAP

class PatMgr;

//////////////////////////////////////////////////////////////////////
/// @class FuncGroup FuncGroup.h "FuncGroup.h"
/// @brief 論理的に等価なセルのグループを表すクラス
//////////////////////////////////////////////////////////////////////
class FuncGroup
{
  friend class PatMgr;

private:

  /// @brief コンストラクタ
  FuncGroup();

  /// @brief デストラクタ
  ~FuncGroup();


public:

  /// @brief 代表関数に対する変換マップを返す．
  const NpnMap&
  npn_map() const;

  /// @brief セル数を返す．
  ymuint
  cell_num() const;

  /// @brief セルを返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < cell_num() )
  const Cell*
  cell(ymuint pos) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 代表関数に対する変換マップ
  NpnMap mNpnMap;

  // セル数
  ymuint32 mCellNum;

  // セルのポインタの配列
  const Cell** mCellList;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 代表関数に対する変換マップを返す．
inline
const NpnMap&
FuncGroup::npn_map() const
{
  return mNpnMap;
}

// @brief セル数を返す．
inline
ymuint
FuncGroup::cell_num() const
{
  return mCellNum;
}

// @brief セルを返す．
// @param[in] pos 位置番号 ( 0 <= pos < cell_num() )
inline
const Cell*
FuncGroup::cell(ymuint pos) const
{
  return mCellList[pos];
}

END_NAMESPACE_YM_TECHMAP

#endif // LIBYM_TECHMAP_FUNCGROUP_H
