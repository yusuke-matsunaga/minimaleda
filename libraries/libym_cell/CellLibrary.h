#ifndef YM_CELL_CELLLIBRARY_H
#define YM_CELL_CELLLIBRARY_H

/// @file　ym_cell/CellLibrary.h
/// @brief CellLibrary のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: CellLibrary.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_cell/cell_nsdef.h"
#include "ym_cell/cell_type.h"
#include "ym_utils/ShString.h"
#include "ym_utils/Alloc.h"


BEGIN_NAMESPACE_YM_CELL

//////////////////////////////////////////////////////////////////////
/// @class CellLibrary CellLibrary.h <ym_cell/CellLibrary.h>
/// @brief セルライブラリを表すクラス
//////////////////////////////////////////////////////////////////////
class CellLibrary
{
public:

  /// @brief コンストラクタ
  CellLibrary();

  /// @brief デストラクタ
  virtual
  ~CellLibrary();


public:
  //////////////////////////////////////////////////////////////////////
  // 属性の取得
  //////////////////////////////////////////////////////////////////////

  /// @brief 名前の取得
  virtual
  string
  name() const = 0;

  /// @brief テクノロジの取得
  virtual
  tCellTechnology
  technology() const = 0;

  /// @brief 遅延モデルの取得
  virtual
  tCellDelayModel
  delay_model() const = 0;

  /// @brief バス命名規則の取得
  virtual
  string
  bus_naming_style() const = 0;

  /// @brief 日付情報の取得
  virtual
  string
  date() const = 0;

  /// @brief リビジョン情報の取得
  virtual
  string
  revision() const = 0;

  /// @brief コメント情報の取得
  virtual
  string
  comment() const = 0;

  /// @brief 時間単位の取得
  virtual
  tCellTimeUnit
  time_unit() const = 0;

  /// @brief 電圧単位の取得
  virtual
  tCellVoltageUnit
  voltage_unit() const = 0;

  /// @brief 電流単位の取得
  virtual
  tCellCurrentUnit
  current_unit() const = 0;

  /// @brief 抵抗単位の取得
  virtual
  tCellResistanceUnit
  pulling_resistance_unit() const = 0;

  /// @brief 容量単位の取得
  virtual
  tCellCapacitanceUnit
  capacitive_load_unit() const = 0;

  /// @brief 電力単位の取得
  virtual
  tCellPowerUnit
  leakage_power_unit() const = 0;

  /// @brief ルックアップテーブルのテンプレートの取得
  /// @param[in] name テンプレート名
  /// @note なければ NULL を返す．
  virtual
  const CellLutTemplate*
  lu_table_template(const char* name) const = 0;

  /// @brief バスタイプの取得
  /// @param[in] name バスタイプ名
  /// @note なければ NULL を返す．
  virtual
  const CellBusType*
  bus_type(const char* name) const = 0;


public:
  //////////////////////////////////////////////////////////////////////
  // セル情報の取得
  //////////////////////////////////////////////////////////////////////

  /// @brief このライブラリの持つセル数の取得
  virtual
  ymuint
  n_cells() const = 0;

  /// @brief セルの取得
  /// @param[in] pos 位置番号( 0 <= pos < n_cells() )
  virtual
  const Cell*
  cell(ymuint pos) const = 0;

  /// @brief 名前からのセルの取得
  virtual
  const Cell*
  cell(const char* name) const = 0;


private:
  //////////////////////////////////////////////////////////////////////
  // CellManip から呼ばれる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容をクリアする．
  void
  clear();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // このセルライブラリに関する全てのメモリを確保するオブジェクト
  SimpleAlloc mAlloc;

  // 名前
  string mName;

  // テクノロジ
  tCellTechnology mTechnology;

  // 遅延モデル
  tCellDelayModel mDelayModel;

  // バス命名規則
  string mBusNamingStyle;

  // 日付
  string mDate;

  // リビジョン
  string mRevision;

  // コメント
  string mComment;

  // 時間単位
  tCellTimeUnit mTimeUnit;

  // 電圧単位
  tCellVoltageUnit mVoltageUnit;

  // 電流単位
  tCellCurrentUnit mCurrentUnit;

  // 抵抗単位
  tCellResistanceUnit mResistanceUnit;

  // 容量単位
  tCellCapacitanceUnit mCapacitanceUnit;

  // 電力単位
  tCellPowerUnit mPowerUnit;

  // セルを納めるリスト
  vector<Cell*> mCellList;

  // セル名をキーにしたハッシュ表
  hash_map<ShString, Cell*> mCellDict;

  // 名前をキーにした LUT テンプレートのハッシュ表
  hash_map<ShString, CellLutTemplate*> mLutTemplateDict;

  // 名前をキーにしたバスタイプのハッシュ表
  hash_map<ShString, CellBusType*> mBusTypeDict;

  // routing_layers : ???
  // piece_type : ???
  // piece_define : ???
  // input_threshold_pct_fall : float (0.0 - 100.0)
  // input_threshold_pct_rise : float (0.0 - 100.0)
  // output_threshold_pct_fall : float (0.0 - 100.0)
  // output_threshold_pct_rise : float (0.0 - 100.0)
  // slew_derate_from_library : float (0.0 - 1.0)
  // slew_lower_threshold_pct_fall : float (0.0 - 100.0)
  // slew_lower_threshold_pct_rise : float (0.0 - 100.0)
  // slew_upper_threshold_pct_fall : float (0.0 - 100.0)
  // slew_upper_threshold_pct_rise : float (0.0 - 100.0)
  // preferred_output_pad_slew_rate_control : high/low/medium/none
  // preferred_output_pad_voltage : <STR>
  // preferred_input_pad_voltage : <STR>
  // in_place_swap_mode : match_footprint/no_swapping
  // library_features : report_delay_calculation
  // simulation : bool
  // define_cell_area : ???
  // base_curves : ???
  // compact_lut_template : ???
  // dc_current_template : ???
  // em_lut_template : ???
  // fall_net_delay : ???
  // fall_transition_degradation : ???
  // faults_lut_template : ???
  // fpga_isd : ???
  // input_voltage : ???
  // iv_lut_template : ???
  // maxcap_lut_template : ???
  // maxtrans_lut_template : ???
  // noise_lut_template : ???
  // operating_conditions : ???
  // output_current_template : ???
  // output_voltage : ???
  // part : ???
  // pg_current_template : ???
  // poly_template : ???
  // power_lut_template : ???
  // power_poly_template : ???
  // power_supply : ???
  // default_power_rail : ???
  // power_rail : ???
  // propagation_lut_template : ???
  // rise_net_delay : ???
  // rise_transition_degradation : ???
  // sensitization : ???
  // scaling_factors : ???
  // timing_range : ???
  // user_parameters : ???
  // wire_load : ???
  // wire_load_selection : ???
  // wire_load_table : ???

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 名前の取得
inline
string
CellLibrary::name() const
{
  return mName;
}

// @brief テクノロジの取得
inline
tCellTechnology
CellLibrary::technology() const
{
  return mTechnology;
}

// @brief 遅延モデルの取得
inline
tCellDelayModel
CellLibrary::delay_model() const
{
  return mDelayModel;
}

// @brief バス命名規則の取得
inline
string
CellLibrary::bus_naming_style() const
{
  return mBusNamingStyle;
}

// @brief 日付情報の取得
inline
string
CellLibrary::date() const
{
  return mDate;
}

// @brief リビジョン情報の取得
inline
string
CellLibrary::revision() const
{
  return mRevision;
}

// @brief コメント情報の取得
inline
string
CellLibrary::comment() const
{
  return mComment;
}

// @brief 時間単位の取得
inline
tCellTimeUnit
CellLibrary::time_unit() const
{
  return mTimeUnit;
}

// @brief 電圧単位の取得
inline
tCellVoltageUnit
CellLibrary::voltage_unit() const
{
  return mVoltageUnit;
}

// @brief 電流単位の取得
inline
tCellCurrentUnit
CellLibrary::current_unit() const
{
  return mCurrentUnit;
}

// @brief 抵抗単位の取得
inline
tCellResistanceUnit
CellLibrary::pulling_resistance_unit() const
{
  return mResistanceUnit;
}

// @brief 容量単位の取得
inline
tCellCapacitanceUnit
CellLibrary::capacitive_load_unit() const
{
  return mCapacitanceUnit;
}

// @brief 電力単位の取得
inline
tCellPowerUnit
CellLibrary::leakage_power_unit() const
{
  return mPowerUnit;
}

// @brief このライブラリの持つセル数の取得
inline
ymuint
CellLibrary::n_cells() const
{
  return mCellList.size();
}

// @brief セルの取得
// @param[in] pos ( 0 <= pos < n_cells() )
inline
const Cell*
CellLibrary::cell(ymuint pos) const
{
  return mCellList[pos];
}

END_NAMESPACE_YM_CELL

#endif // YM_CELL_CELLLIBRARY_H
