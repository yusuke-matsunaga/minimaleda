#ifndef LIBYM_CELL_CELLMANIP_H
#define LIBYM_CELL_CELLMANIP_H

/// @file　libym_cell/CellManip.h
/// @brief CellManip のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: CellManip.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_cell/cell_nsdef.h"
#include "ym_cell/cell_type.h"
#include "CellPinImpl.h"
#include "ym_cell/CellLut.h"
#include "ym_cell/CellBus.h"


BEGIN_NAMESPACE_YM_CELL

//////////////////////////////////////////////////////////////////////
/// @class CellManip CellManip.h "CellManip.h"
/// @brief セルライブラリの内容を設定するクラス
//////////////////////////////////////////////////////////////////////
class CellManip
{
public:

  /// @brief コンストラクタ
  /// @param[in] library 設定対象のライブラリ
  CellManip(CellLibrary& library);

  /// @brief デストラクタ
  ~CellManip();
  

public:
  //////////////////////////////////////////////////////////////////////
  // 内容をクリアする．
  //////////////////////////////////////////////////////////////////////
  
  /// @brief ライブラリの内容をクリアする．
  void
  clear();

  
public:
  //////////////////////////////////////////////////////////////////////
  // 属性の設定
  //////////////////////////////////////////////////////////////////////

  /// @brief 名前を設定する．
  void
  set_name(string name);

  /// @brief テクノロジを設定する．
  void
  set_technology(tCellTechnology technology);
  
  /// @brief 遅延モデルを設定する．
  void
  set_delay_model(tCellDelayModel delay_model);

  /// @brief バス命名規則を設定する．
  void
  set_bus_naming_style(const string& bus_naming_style);
  
  /// @brief 日付情報を設定する．
  void
  set_date(const string& date);

  /// @brief リビジョン情報を設定する．
  void
  set_revision(const string& revision);

  /// @brief コメント情報を設定する．
  void
  set_comment(const string& comment);

  /// @brief 時間単位を設定する．
  void
  set_time_unit(tCellTimeUnit time_unit);

  /// @brief 電圧単位を設定する．
  void
  set_voltage_unit(tCellVoltageUnit voltage_unit);

  /// @brief 電流単位を設定する．
  void
  set_current_unit(tCellCurrentUnit current_unit);

  /// @brief 抵抗単位を設定する．
  void
  set_pulling_resistance_unit(tCellResistanceUnit resistance_unit);

  /// @brief 容量単位を設定する．
  void
  set_capacitive_load_unit(tCellCapacitanceUnit load_unit);
  
  /// @brief 電力単位を設定する．
  void
  set_leakage_power_unit(tCellPowerUnit power_unit);
  

public:
  //////////////////////////////////////////////////////////////////////
  // 要素の生成/追加
  //////////////////////////////////////////////////////////////////////

  /// @brief ルックアップテーブルのテンプレートを追加する．(1次元)
  /// @param[in] name テンプレートの名前
  /// @param[in] var1 1番目の変数の型
  /// @param[in] index_array1 1番目の変数のインデックス値の配列
  /// @return 生成されたテンプレートを返す．
  CellLutTemplate*
  new_lut_template(ShString name,
		   tCellVarType var1,
		   const vector<double>& index_array1);
  
  /// @brief ルックアップテーブルのテンプレートを追加する．(2次元)
  /// @param[in] name テンプレートの名前
  /// @param[in] var1 1番目の変数の型
  /// @param[in] index_array1 1番目の変数のインデックス値の配列
  /// @param[in] var2 2番目の変数の型
  /// @param[in] index_array2 2番目の変数のインデックス値の配列
  /// @return 生成されたテンプレートを返す．
  CellLutTemplate*
  new_lut_template(ShString name,
		   tCellVarType var1,
		   const vector<double>& index_array1,
		   tCellVarType var2,
		   const vector<double>& index_array2);
  
  /// @brief ルックアップテーブルのテンプレートを追加する．(3次元)
  /// @param[in] name テンプレートの名前
  /// @param[in] var1 1番目の変数の型
  /// @param[in] index_array1 1番目の変数のインデックス値の配列
  /// @param[in] var2 2番目の変数の型
  /// @param[in] index_array2 2番目の変数のインデックス値の配列
  /// @param[in] var3 3番目の変数の型
  /// @param[in] index_array3 3番目の変数のインデックス値の配列
  /// @return 生成されたテンプレートを返す．
  CellLutTemplate*
  new_lut_template(ShString name,
		   tCellVarType var1,
		   const vector<double>& index_array1,
		   tCellVarType var2,
		   const vector<double>& index_array2,
		   tCellVarType var3,
		   const vector<double>& index_array3);

  /// @brief バスタイプを追加する．
  /// @param[in] name バスタイプの名前
  /// @param[in] base_type base_type (今は kArrayType のみが使用可能)
  /// @param[in] data_type data_type (今は kBitType のみが使用可能)
  /// @param[in] bit_from ビットの開始位置
  /// @param[in] bit_to ビットの終了位置
  /// @return 生成されたバスタイプを返す．
  /// @note bit_width と downto の情報は bit_from と bit_to から生成する．
  CellBusType*
  new_bus_type(ShString name,
	       CellBusType::tBaseType base_type,
	       CellBusType::tDataType data_type,
	       ymint32 bit_from,
	       ymint32 bit_to);

  /// @brief セルを追加する．
  /// @param[in] name セル名
  /// @param[in] area 面積
  /// @param[in] n_pin ピン数
  /// @param[in] n_bus バス数
  /// @param[in] n_bundle バンドル数
  /// @return 生成されたセルを返す．
  /// @note n_pin, n_bus, n_bundle の値は参考にするだけ．
  /// @note この時点ではピンもバスもバンドルも持たない．
  Cell*
  new_cell(ShString name,
	   CellArea area,
	   ymuint32 n_pin = 0,
	   ymuint32 n_bus = 0,
	   ymuint32 n_bundle = 0);
  
  
public:
  //////////////////////////////////////////////////////////////////////
  // セルに対する要素の追加
  //////////////////////////////////////////////////////////////////////

  /// @brief セルに入力ピンを追加する．
  /// @param[in] cell セル
  /// @param[in] name ピン名
  /// @return 生成されたピンを返す．
  CellInputPin*
  add_ipin(Cell* cell,
	   ShString name);
  
  /// @brief セルに出力ピンを追加する．
  /// @param[in] cell セル
  /// @param[in] name ピン名
  /// @return 生成されたピンを返す．
  CellOutputPin*
  add_opin(Cell* cell,
	   ShString name);

  /// @brief セルに入出力ピンを追加する．
  /// @param[in] cell セル
  /// @param[in] name ピン名
  /// @return 生成されたピンを返す．
  CellInoutPin*
  add_iopin(Cell* cell,
	    ShString name);

  /// @brief セルにバスを追加する．
  /// @param[in] cell セル
  /// @param[in] name バス名
  /// @param[in] bus_type バスの型
  /// @return 生成されたバスを返す．
  CellBus*
  add_bus(Cell* cell,
	  ShString name,
	  CellBusType* bus_type);

  /// @brief セルにバンドルを追加する．
  /// @param[in] cell セル
  /// @param[in] name バンドル名
  /// @param[in] n_pin ピン数
  /// @return 生成されたバンドルを返す．
  /// @note n_pin の値は参考にするだけ．
  CellBundle*
  add_bundle(Cell* cell,
	     ShString name,
	     ymuint32 n_pin = 0);
  

public:
  //////////////////////////////////////////////////////////////////////
  // セルのピンに対する情報を設定する．
  //////////////////////////////////////////////////////////////////////

  /// @brief ピンに負荷容量を設定する．
  /// @param[in] pin ピン
  /// @param[in] load 負荷容量
  void
  set_capacitance(CellInputPin* pin,
		  CellCapacitance load);

  /// @brief ピンに立ち上がり時の負荷容量を設定する．
  /// @param[in] pin ピン
  /// @param[in] load 負荷容量
  void
  set_rise_capacitance(CellInputPin* pin,
		       CellCapacitance load);

  /// @brief ピンに立ち下がり時の負荷容量を設定する．
  /// @param[in] pin ピン
  /// @param[in] load 負荷容量
  void
  set_fall_capacitance(CellInputPin* pin,
		       CellCapacitance load);

  /// @brief ピンに負荷容量を設定する．
  /// @param[in] pin ピン
  /// @param[in] load 負荷容量
  void
  set_capacitance(CellInoutPin* pin,
		  CellCapacitance load);

  /// @brief ピンに立ち上がり時の負荷容量を設定する．
  /// @param[in] pin ピン
  /// @param[in] load 負荷容量
  void
  set_rise_capacitance(CellInoutPin* pin,
		       CellCapacitance load);

  /// @brief ピンに立ち下がり時の負荷容量を設定する．
  /// @param[in] pin ピン
  /// @param[in] load 負荷容量
  void
  set_fall_capacitance(CellInoutPin* pin,
		       CellCapacitance load);
  
  /// @brief ピンに最大ファンアウト容量を設定する．
  /// @param[in] pin ピン
  /// @param[in] load 容量
  void
  set_max_fanout(CellOutputPin* pin,
		 CellCapacitance load);

  /// @brief ピンに最小ファンアウト容量を設定する．
  /// @param[in] pin ピン
  /// @param[in] load 容量
  void
  set_min_fanout(CellOutputPin* pin,
		 CellCapacitance load);

  /// @brief ピンに最大負荷容量を設定する．
  /// @param[in] pin ピン
  /// @param[in] load 容量
  void
  set_max_capacitance(CellOutputPin* pin,
		      CellCapacitance load);

  /// @brief ピンに最小負荷容量を設定する．
  /// @param[in] pin ピン
  /// @param[in] load 容量
  void
  set_min_capacitance(CellOutputPin* pin,
		      CellCapacitance load);
  
  /// @brief ピンに最大遷移時間を設定する．
  /// @param[in] pin ピン
  /// @param[in] time 遷移時間
  void
  set_max_transition(CellOutputPin* pin,
		     CellTime time);
  
  /// @brief ピンに最小遷移時間を設定する．
  /// @param[in] pin ピン
  /// @param[in] time 遷移時間
  void
  set_min_transition(CellOutputPin* pin,
		     CellTime time);
  

public:
  //////////////////////////////////////////////////////////////////////
  // セルにタイミング情報を追加する．
  //////////////////////////////////////////////////////////////////////

  /// @brief セルにタイミング情報を追加する．(CMOSジェネリックタイプ)
  /// @param[in] cell セル
  /// @param[in] ipin_list 開始ピンのリスト
  /// @param[in] opin 終点ピン
  /// @param[in] timing_sense タイミング条件
  /// @param[in] timing_type タイミングの型
  /// @param[in] intrisic_rise 立ち上がり固有遅延
  /// @param[in] intrisic_fall 立ち下がり固有遅延
  /// @param[in] slope_rise 立ち上がりスロープ遅延
  /// @param[in] slope_fall 立ち下がりスロープ遅延
  /// @param[in] rise_resistance 立ち上がり遷移遅延パラメータ
  /// @param[in] fall_resistance 立ち下がり遷移遅延パラメータ
  /// @return 生成されたタイミング情報を返す．
  CellTiming*
  add_timing_generic(Cell* cell,
		     const vector<const CellPin*>& ipin_list,
		     CellPin* opin,
		     tCellTimingSense timing_sense,
		     tCellTimingType timing_type,
		     CellTime intrisic_rise,
		     CellTime intrisic_fall,
		     CellTime slope_rise,
		     CellTime slope_fall,
		     CellResistance rise_resistance,
		     CellResistance fall_resistance);

  /// @brief セルにタイミング情報を追加する．(CMOS折れ線近似タイプ)
  /// @param[in] cell セル
  /// @param[in] ipin_list 開始ピンのリスト
  /// @param[in] opin 終点ピン
  /// @param[in] timing_sense タイミング条件
  /// @param[in] timing_type タイミングの型
  /// @param[in] intrisic_rise 立ち上がり固有遅延
  /// @param[in] intrisic_fall 立ち下がり固有遅延
  /// @param[in] slope_rise 立ち上がりスロープ遅延
  /// @param[in] slope_fall 立ち下がりスロープ遅延
  /// @return 生成されたタイミング情報を返す．
  CellTiming*
  add_timing_piecewise(Cell* cell,
		       const vector<const CellPin*>& ipin_list,
		       CellPin* opin,
		       tCellTimingSense timing_sense,
		       tCellTimingType timing_type,
		       CellTime intrisic_rise,
		       CellTime intrisic_fall,
		       CellTime slope_rise,
		       CellTime slope_fall,
		       CellResistance rise_pin_resistance,
		       CellResistance fall_pin_resistance);

  /// @brief セルにタイミング情報を追加する．(CMOS非線形タイプ)
  /// @param[in] cell セル
  /// @param[in] ipin_list 開始ピンのリスト
  /// @param[in] opin 終点ピン
  /// @param[in] timing_sense タイミング条件
  /// @param[in] timing_type タイミングの型
  /// @param[in] cell_rise 立ち上がりセル遅延テーブル
  /// @param[in] cell_fall 立ち下がりセル遅延テーブル
  /// @return 生成されたタイミング情報を返す．
  CellTiming*
  add_timing_nonlinear(Cell* cell,
		       const vector<const CellPin*>& ipin_list,
		       CellPin* opin,
		       tCellTimingSense timing_sense,
		       tCellTimingType timing_type,
		       CellLut* cell_rise,
		       CellLut* cell_fall);

  /// @brief セルにタイミング情報を追加する．(CMOS非線形タイプ)
  /// @param[in] cell セル
  /// @param[in] ipin_list 開始ピンのリスト
  /// @param[in] opin 終点ピン
  /// @param[in] timing_sense タイミング条件
  /// @param[in] timing_type タイミングの型
  /// @param[in] rise_transition 立ち上がり遷移遅延テーブル
  /// @param[in] fall_transition 立ち下がり遷移遅延テーブル
  /// @param[in] rise_propagation 立ち上がり伝搬遅延テーブル
  /// @param[in] fall_propagation 立ち下がり伝搬遅延テーブル
  /// @return 生成されたタイミング情報を返す．
  CellTiming*
  add_timing_nonlinear(Cell* cell,
		       const vector<const CellPin*>& ipin_list,
		       CellPin* opin,
		       tCellTimingSense timing_sense,
		       tCellTimingType timing_type,
		       CellLut* rise_transition,
		       CellLut* fall_transition,
		       CellLut* rise_propagation,
		       CellLut* fall_propagation);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられるメンバ関数
  //////////////////////////////////////////////////////////////////////

  /// @brief メモリ領域を確保する．
  void*
  get_memory(size_t size);
  
  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////
  
  // 対象のライブラリ
  CellLibrary& mLibrary;
  
};

END_NAMESPACE_YM_CELL

#endif // LIBYM_CELL_CELLMANIP_H
