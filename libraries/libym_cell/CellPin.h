#ifndef YM_CELL_CELLPIN_H
#define YM_CELL_CELLPIN_H

/// @file　ym_cell/CellPin.h
/// @brief CellPin のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: CellPin.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_cell/cell_nsdef.h"
#include "ym_cell/cell_type.h"
#include "ym_utils/ShString.h"


BEGIN_NAMESPACE_YM_CELL

//////////////////////////////////////////////////////////////////////
/// @class CellPin CellPin.h <ym_cell/CellPin.h>
/// @brief セルのピンを表すクラス
//////////////////////////////////////////////////////////////////////
class CellPin
{
  friend class CellManip;
  
protected:

  /// @brief コンストラクタ
  /// @param[in] name 名前
  CellPin(const ShString& name);

  /// @brief デストラクタ
  virtual
  ~CellPin();
  
  
public:
  //////////////////////////////////////////////////////////////////////
  // 共通属性
  //////////////////////////////////////////////////////////////////////

  /// @brief ピン名を返す．
  ShString
  name() const;

  /// @brief 方向を返す．
  virtual
  tCellDirection
  direction() const = 0;


public:
  //////////////////////////////////////////////////////////////////////
  // 入力ピンの属性
  //////////////////////////////////////////////////////////////////////

  /// @brief 負荷容量を返す．
  /// @note デフォルトでは 0.0 を返す．
  virtual
  CellCapacitance
  capacitance() const;

  /// @brief 立ち上がり時の負荷容量を返す．
  /// @note デフォルトでは 0.0 を返す．
  virtual
  CellCapacitance
  rise_capacitance() const;

  /// @brief 立ち下がり時の負荷容量を返す．
  /// @note デフォルトでは 0.0 を返す．
  virtual
  CellCapacitance
  fall_capacitance() const;


public:
  //////////////////////////////////////////////////////////////////////
  // 出力ピンの属性
  //////////////////////////////////////////////////////////////////////

  /// @brief 最大ファンアウト容量を返す．
  /// @note デフォルトでは 0.0 を返す．
  virtual
  CellCapacitance
  max_fanout() const;

  /// @brief 最小ファンアウト容量を返す．
  /// @note デフォルトでは 0.0 を返す．
  virtual
  CellCapacitance
  min_fanout() const;
  
  /// @brief 最大負荷容量を返す．
  /// @note デフォルトでは 0.0 を返す．
  virtual
  CellCapacitance
  max_capacitance() const;
  
  /// @brief 最小負荷容量を返す．
  /// @note デフォルトでは 0.0 を返す．
  virtual
  CellCapacitance
  min_capacitance() const;
  
  /// @brief 最大遷移時間を返す．
  /// @note デフォルトでは 0.0 を返す．
  virtual
  CellTime
  max_transition() const;

  /// @brief 最小遷移時間を返す．
  /// @note デフォルトでは 0.0 を返す．
  virtual
  CellTime
  min_transition() const;

  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ピン名
  ShString mName;
  
  // clock_gate_clock_pin : bool
  // clock_gate_enable_pin : bool
  // clock_gate_obs_pin : bool
  // clock_gate_out_pin : bool
  // clock_gate_test_pin : bool
  
  // complementary_pin : string (pin)

  // connection_class : string

  // dont_fault : sa0|sa1|sa01
  // fault_model : string
  
  // driver_type : string

  // inverted_output : bool

  // pin_func_type : string

  // steady_state_resistance_float_max : float
  // steady_state_resistance_float_min : float
  // steady_state_resistance_high_max : float
  // steady_state_resistance_high_min : float
  // steady_state_resistance_low_max : float
  // steady_state_resistance_low_min : float

  // test_output_only : bool
  
  // cell_degradation : complex structure

  // clock : bool
  // min_period : float
  // min_pulse_width_high : float
  // min_pulse_width_low : float
  
  // function : LogExpr
  // three_state : LogExpr
  // x_function : LogExpr
  // state_function : LogExpr
  // internal_node : string

  // prefer_tied : string (論理値?)

  // is_pad : bool
  // multicell_pad_pin : bool
  // input_voltage : string
  // output_voltage : string
  // pulling_resistance : float
  // pulling_current : float
  // hysteresis : bool
  // drive_current : float
  // slew_control : string
  // rise_current_slope_before_threshold : float
  // rise_current_slope_after_threshold : float
  // fall_current_slope_before_threshold : float
  // fall_current_slope_after_threshold : float
  // rise_time_before_threshold : float
  // rise_time_after_thresold : float
  // fall_time_before_threshold : float
  // fall_time_after_thresold : float
};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief ピン名を返す．
inline
ShString
CellPin::name() const
{
  return mName;
}

END_NAMESPACE_YM_CELL

#endif // YM_CELL_CELLPIN_H
