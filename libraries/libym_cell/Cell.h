#ifndef YM_CELL_CELL_H
#define YM_CELL_CELL_H

/// @file　ym_cell/Cell.h
/// @brief Cell のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: Cell.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.

#include "ym_cell/cell_nsdef.h"
#include "ym_cell/cell_type.h"
#include "ym_utils/ShString.h"


BEGIN_NAMESPACE_YM_CELL

//////////////////////////////////////////////////////////////////////
/// @class Cell Cell.h <ym_cell/Cell.h>
/// @brief セル本体のクラス
//////////////////////////////////////////////////////////////////////
class Cell
{
  friend class CellManip;
  
private:
  
  /// @brief コンストラクタ．
  Cell(ShString name,
       CellArea area,
       ymuint32 n_pin,
       ymuint32 n_bus,
       ymuint32 n_bundle);

  /// @brief デストラクタ
  virtual
  ~Cell();

  
public:
  //////////////////////////////////////////////////////////////////////
  // セル情報の取得
  //////////////////////////////////////////////////////////////////////

  /// @brief 名前の取得
  ShString
  name() const;
  
  /// @brief 面積の取得
  CellArea
  area() const;

  /// @brief ピン数の取得
  ymuint32
  n_pins() const;
  
  /// @brief ピンの取得
  /// @param[in] pos 位置番号 ( 0 <= pos < n_pins() )
  const CellPin*
  pin(ymuint32 pos) const;
  
  /// @brief 名前からピンの取得
  /// @param[in] name ピン名
  /// @return name という名前をピンを返す．
  /// @note なければ NULL を返す．
  const CellPin*
  pin(const string& name) const;
  
  /// @brief バス数の取得
  ymuint32
  n_buses() const;

  /// @brief バスの取得
  /// @param[in] pos 位置番号 ( 0 <= pos < n_buses() )
  const CellBus*
  bus(ymuint32 pos) const;

  /// @brief 名前からバスの取得
  /// @param[in] name バス名
  /// @return name という名前のバスを返す．
  /// @note なければ NULL を返す．
  const CellBus*
  bus(const string& name) const;

  /// @brief バンドル数の取得
  ymuint32
  n_bundles() const;

  /// @brief バンドルの取得
  /// @param[in] pos 位置番号 ( 0 <= pos < n_bundles() )
  const CellBundle*
  bundle(ymuint32 pos) const;

  /// @brief 名前からバンドルの取得
  const CellBundle*
  bundle(const string& name) const;
  
  /// @brief タイミング情報の取得
  /// @param[in] ipos 開始ピン番号
  /// @param[in] opos 終了ピン番号
  /// @param[out] timing_list タイミング情報を納めるベクタ
  /// @return 条件に合致するタイミング情報の数を返す．
  ymuint32
  timing(ymuint32 ipos,
	 ymuint32 opos,
	 vector<const CellTiming*>& timing_list) const;
  
  /// @brief タイミング情報の取得
  /// @param[in] ipos 開始ピン番号
  /// @param[in] opos 終了ピン番号
  /// @param[in] timing_sense タイミング情報の摘要条件
  /// @param[in] timing_type タイミング情報の種類
  /// @param[out] timing_list タイミング情報を納めるベクタ
  /// @return 条件に合致するタイミング情報の数を返す．
  ymuint32
  timing(ymuint32 ipos,
	 ymuint32 opos,
	 tCellTimingSense timing_sense,
	 tCellTimingType timing_type,
	 vector<const CellTiming*>& timing_list) const;
	 
  /// @brief デバッグ用の内容表示関数
  /// @param[in] s 出力先のストリーム
  void
  dump(ostream& s) const;
  
  
private:
  //////////////////////////////////////////////////////////////////////
  // 使用禁止の関数定義
  //////////////////////////////////////////////////////////////////////

  // コピーコンストラクタと代入演算子は、使用禁止。実体も定義しない。
  Cell(const Cell& src);
  
  const Cell&
  operator=(const Cell& src);
  
  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////
  
  // セル名
  ShString mName;
  
  // 面積
  CellArea mArea;
  
  // ピンの配列
  vector<CellPin*> mPinList;
  
  // バス情報
  vector<CellBus*> mBusList;

  // バンドル情報
  vector<CellBundle*> mBundleList;
  
  // タイミング情報
  vector<CellTiming*> mTimingList;
  
  // cell_footprint : string
  // contention_condition : string
  // dont_fault : sa0|sa1|sa01
  // dont_touch : bool
  // dont_nse : bool
  // handle_negative_constraint : bool
  // is_clock_gating_cell : bool
  // map_only : bool
  // pad_cell : bool
  // auxiliaray_pad_cell : bool
  // pad_type : string
  // pin_equal : string-list
  // pin_opposite : list of string-list
  // preferred : bool
  // scaling_factors : string
  // scan_group : string
  // usr_for_size_only : bool
  // vhdl_name : string
  // mode_definition : complex structure
  // routing_track : complex structure
  // single_bit_degenerate : string
  
};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 名前の取得
inline
ShString
Cell::name() const
{
  return mName;
}
  
// @brief 面積の取得
inline
CellArea
Cell::area() const
{
  return mArea;
}

// @brief ピン数の取得
inline
ymuint32
Cell::n_pins() const
{
  return mPinList.size();
}
  
// @brief ピンの取得
// @param[in] pos 位置番号 ( 0 <= pos < n_pins() )
inline
const CellPin*
Cell::pin(ymuint32 pos) const
{
  return mPinList[pos];
}
  
// @brief バス数の取得
inline
ymuint32
Cell::n_buses() const
{
  return mBusList.size();
}

// @brief バスの取得
// @param[in] pos 位置番号 ( 0 <= pos < n_buses() )
inline
const CellBus*
Cell::bus(ymuint32 pos) const
{
  return mBusList[pos];
}

// @brief バンドル数の取得
inline
ymuint32
Cell::n_bundles() const
{
  return mBundleList.size();
}

// @brief バンドルの取得
// @param[in] pos 位置番号 ( 0 <= pos < n_bundles() )
inline
const CellBundle*
Cell::bundle(ymuint32 pos) const
{
  return mBundleList[pos];
}

END_NAMESPACE_YM_CELL

#endif // YM_CELL_CELL_H
