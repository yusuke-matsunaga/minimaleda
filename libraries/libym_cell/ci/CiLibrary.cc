
/// @file libym_cell/CiLibrary.cc
/// @brief CiLibrary の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "CiLibrary.h"
#include "CiCell.h"
#include "CiPin.h"
#include "CiTiming.h"


BEGIN_NAMESPACE_YM_CELL

//////////////////////////////////////////////////////////////////////
// クラス CiLibrary
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] name 名前
CiLibrary::CiLibrary(const string& name) :
  mAlloc(4096),
  mName(name)
{
}

// @brief デストラクタ
CiLibrary::~CiLibrary()
{
}

// @brief 名前の取得
string
CiLibrary::name() const
{
  return mName;
}

// @brief テクノロジの取得
// このクラスは常に kTechAsic を返す．
tCellTechnology
CiLibrary::technology() const
{
  return kTechAsic;
}

// @brief 遅延モデルの取得
// 返り値は
// - kDelayGeneric
// - kDelayPiecewise
// - kDelayNonlinear
// のいずれか
tCellDelayModel
CiLibrary::delay_model() const
{
  return kDelayGeneric;
}

// @brief バス命名規則の取得
string
CiLibrary::bus_naming_style() const
{
  return mBusNamingStyle;
}

// @brief 日付情報の取得
string
CiLibrary::date() const
{
  return mDate;
}

/// @brief リビジョン情報の取得
string
CiLibrary::revision() const
{
  return mRevision;
}

// @brief コメント情報の取得
string
CiLibrary::comment() const
{
  return mComment;
}

// @brief 時間単位の取得
string
CiLibrary::time_unit() const
{
  return mTimeUnit;
}

// @brief 電圧単位の取得
string
CiLibrary::voltage_unit() const
{
  return mVoltageUnit;
}

// @brief 電流単位の取得
string
CiLibrary::current_unit() const
{
  return mCurrentUnit;
}

// @brief 抵抗単位の取得
string
CiLibrary::pulling_resistance_unit() const
{
  return mPullingResistanceUnit;
}

// @brief 容量単位の取得
string
CiLibrary::capacitive_load_unit() const
{
  return mCapacitiveLoadUnit;
}

// @brief 電力単位の取得
string
CiLibrary::leakage_power_unit() const
{
  return mLeakagePowerUnit;
}

// @brief ルックアップテーブルのテンプレートの取得
// @param[in] name テンプレート名
// @note なければ NULL を返す．
const CellLutTemplate*
CiLibrary::lu_table_template(const char* name) const
{
  // 未完
  return NULL;
}

// @brief バスタイプの取得
// @param[in] name バスタイプ名
// @note なければ NULL を返す．
const CellBusType*
CiLibrary::bus_type(const char* name) const
{
  // 未完
  return NULL;
}

// @brief このライブラリの持つセル数の取得
ymuint
CiLibrary::cell_num() const
{
  return mCellNum;
}

// @brief セルの取得
// @param[in] pos 位置番号( 0 <= pos < cell_num() )
const Cell*
CiLibrary::cell(ymuint pos) const
{
  return &mCellArray[pos];
}

// @brief 名前からのセルの取得
const Cell*
CiLibrary::cell(const char* name) const
{
  // 未完
  return NULL;
}

// @brief セル数を設定する．
// @param[in] num 設定する値
void
CiLibrary::set_cell_num(ymuint num)
{
  mCellNum = num;
  void* p = mAlloc.get_memory(sizeof(CiCell) * num);
  mCellArray = new (p) CiCell[num];
}

// @brief セルの内容を設定する．
// @param[in] cell_id セル番号 ( 0 <= cell_id < cell_num() )
// @param[in] name 名前
// @param[in] area 面積
// @param[in] np ピン数
// @param[in] nb バス数
// @param[in] nc バンドル数
// @return セルへのポインタを返す．
CiCell*
CiLibrary::new_cell(ymuint cell_id,
		    ShString name,
		    CellArea area,
		    ymuint np,
		    ymuint nb,
		    ymuint nc)
{
  CiCell* cell = &mCellArray[cell_id];
  cell->mId = cell_id;
  cell->mName = name;
  cell->mArea = area;

  cell->mPinNum = np;
  void* p = mAlloc.get_memory(sizeof(CiPin*) * np);
  cell->mPinArray = new (p) CiPin*[np];

  // バス，バンドル関係は未完

  return cell;
}

// @brief セルの入力ピンの内容を設定する．
// @param[in] cell セル
// @param[in] pin_id ピン番号 ( 0 <= pin_id < cell->pin_num() )
// @param[in] name 入力ピン名
// @param[in] capacitance 入力ピンの負荷容量
// @param[in] rise_capacitance 入力ピンの立ち上がり負荷容量
// @param[in] fall_capacitance 入力ピンの立ち下がり負荷容量
// @return 入力ピンへのポインタを返す．
CiInputPin*
CiLibrary::new_cell_input(CiCell* cell,
			  ymuint pin_id,
			  ShString name,
			  CellCapacitance capacitance,
			  CellCapacitance rise_capacitance,
			  CellCapacitance fall_capacitance)
{
  void* p = mAlloc.get_memory(sizeof(CiInputPin));
  CiInputPin* pin = new (p) CiInputPin(name, capacitance,
				       rise_capacitance, fall_capacitance);
  cell->mPinArray[pin_id] = pin;
  pin->mId = pin_id;

  return pin;
}

// @brief セルの出力ピンの内容を設定する．
// @param[in] cell セル
// @param[in] pin_id ピン番号 ( 0 <= pin_id < cell->pin_num() )
// @param[in] name 出力ピン名
// @param[in] max_fanout 最大ファンアウト容量
// @param[in] min_fanout 最小ファンアウト容量
// @param[in] max_capacitance 最大負荷容量
// @param[in] min_capacitance 最小負荷容量
// @param[in] max_transition 最大遷移時間
// @param[in] min_transition 最小遷移時間
// @return 出力ピンへのポインタを返す．
CiOutputPin*
CiLibrary::new_cell_output(CiCell* cell,
			   ymuint pin_id,
			   ShString name,
			   CellCapacitance max_fanout,
			   CellCapacitance min_fanout,
			   CellCapacitance max_capacitance,
			   CellCapacitance min_capacitance,
			   CellTime max_transition,
			   CellTime min_transition)
{
  void* p = mAlloc.get_memory(sizeof(CiOutputPin));
  CiOutputPin* pin = new (p) CiOutputPin(name, max_fanout, min_fanout,
					 max_capacitance, min_capacitance,
					 max_transition, min_transition);
  cell->mPinArray[pin_id] = pin;
  pin->mId = pin_id;
  set_timing_array(pin, cell->pin_num());

  return pin;
}

// @brief セルの入出力ピンの内容を設定する．
// @param[in] cell セル
// @param[in] pin_id ピン番号 ( 0 <= pin_id < cell->pin_num() )
// @param[in] name 入出力ピン名
// @param[in] capacitance 入力ピンの負荷容量
// @param[in] rise_capacitance 入力ピンの立ち上がり負荷容量
// @param[in] fall_capacitance 入力ピンの立ち下がり負荷容量
// @param[in] max_fanout 最大ファンアウト容量
// @param[in] min_fanout 最小ファンアウト容量
// @param[in] max_capacitance 最大負荷容量
// @param[in] min_capacitance 最小負荷容量
// @param[in] max_transition 最大遷移時間
// @param[in] min_transition 最小遷移時間
// @return 入出力ピンへのポインタを返す．
CiInoutPin*
CiLibrary::new_cell_inout(CiCell* cell,
			  ymuint pin_id,
			  ShString name,
			  CellCapacitance capacitance,
			  CellCapacitance rise_capacitance,
			  CellCapacitance fall_capacitance,
			  CellCapacitance max_fanout,
			  CellCapacitance min_fanout,
			  CellCapacitance max_capacitance,
			  CellCapacitance min_capacitance,
			  CellTime max_transition,
			  CellTime min_transition)
{
  void* p = mAlloc.get_memory(sizeof(CiInoutPin));
  CiInoutPin* pin =  new (p) CiInoutPin(name, capacitance,
					rise_capacitance, fall_capacitance,
					max_fanout, min_fanout,
					max_capacitance, min_capacitance,
					max_transition, min_transition);
  cell->mPinArray[pin_id] = pin;
  pin->mId = pin_id;
  set_timing_array(pin, cell->pin_num());

  return pin;
}

// @brief タイミング情報を格納する配列を確保する．
void
CiLibrary::set_timing_array(CiOutputPin* pin,
			    ymuint np)
{
  ymuint n = np * 2;
  void* p = mAlloc.get_memory(sizeof(const CellTiming*) * n);
  pin->mTimingArray = new (p) const CellTiming*[n];
  for (ymuint i = 0; i < n; ++ i) {
    pin->mTimingArray[i] = NULL;
  }
}

// @brief タイミング情報を作る．
// @param[in] id ID番号
// @param[in] type タイミングの型
// @param[in] intrinsic_rise 立ち上がり固有遅延
// @param[in] intrinsic_fall 立ち下がり固有遅延
// @param[in] slope_rise 立ち上がりスロープ遅延
// @param[in] slope_fall 立ち下がりスロープ遅延
// @param[in] rise_resistance 立ち上がり負荷依存係数
// @param[in] fall_resistance 立ち下がり負荷依存係数
CellTiming*
CiLibrary::new_timing(ymuint id,
		      tCellTimingType type,
		      CellTime intrinsic_rise,
		      CellTime intrinsic_fall,
		      CellTime slope_rise,
		      CellTime slope_fall,
		      CellResistance rise_resistance,
		      CellResistance fall_resistance)
{
  void* p = mAlloc.get_memory(sizeof(CiTimingGeneric));
  CellTiming* timing = new (p) CiTimingGeneric(id, type,
					       intrinsic_rise,
					       intrinsic_fall,
					       slope_rise,
					       slope_fall,
					       rise_resistance,
					       fall_resistance);
  return timing;
}

// @brief セルのタイミング情報を設定する．
// @param[in] pin 出力(入出力)ピン
// @param[in] ipin_id 関連する入力(入出力)ピン番号
// @param[in] sense タイミング条件
// @param[in] timing 設定するタイミング情報
void
CiLibrary::set_opin_timing(CiOutputPin* pin,
			   ymuint ipin_id,
			   tCellTimingSense sense,
			   const CellTiming* timing)
{
  pin->set_timing(ipin_id, sense, timing);
}

// @brief 出力ピンの機能を設定する．
// @param[in] pin 出力(入出力)ピン
// @param[in] function 機能を表す論理式
void
CiLibrary::set_opin_function(CiOutputPin* pin,
			     const LogExpr& function)
{
  pin->set_function(function);
}

END_NAMESPACE_YM_CELL
