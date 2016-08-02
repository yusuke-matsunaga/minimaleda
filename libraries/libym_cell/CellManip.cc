
/// @file libym_cell/CellManip.cc
/// @brief CellManip の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: CellManip.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "CellManip.h"
#include "ym_cell/CellLibrary.h"
#include "ym_cell/Cell.h"
#include "ym_cell/CellBus.h"
#include "ym_cell/CellBundle.h"
#include "CellLutImpl.h"
#include "CellTimingImpl.h"


BEGIN_NAMESPACE_YM_CELL

// @brief コンストラクタ
// @param[in] library 設定対象のライブラリ
CellManip::CellManip(CellLibrary& library) :
  mLibrary(library)
{
}

// @brief デストラクタ
CellManip::~CellManip()
{
}

// @brief 内容をクリアする．
void
CellManip::clear()
{
  mLibrary.mCellList.clear();
  mLibrary.mCellDict.clear();
  mLibrary.mLutTemplateDict.clear();
  mLibrary.mBusTypeDict.clear();
  mLibrary.mAlloc.destroy();
}

// @brief 名前を設定する．
void
CellManip::set_name(string name)
{
  mLibrary.mName = name;
}

// @brief テクノロジを設定する．
void
CellManip::set_technology(tCellTechnology technology)
{
  mLibrary.mTechnology = technology;
}
  
// @brief 遅延モデルを設定する．
void
CellManip::set_delay_model(tCellDelayModel delay_model)
{
  mLibrary.mDelayModel = delay_model;
}

// @brief バス命名規則を設定する．
void
CellManip::set_bus_naming_style(const string& bus_naming_style)
{
  mLibrary.mBusNamingStyle = bus_naming_style;
}
  
// @brief 日付情報を設定する．
void
CellManip::set_date(const string& date)
{
  mLibrary.mDate = date;
}

// @brief リビジョン情報を設定する．
void
CellManip::set_revision(const string& revision)
{
  mLibrary.mRevision = revision;
}

// @brief コメント情報を設定する．
void
CellManip::set_comment(const string& comment)
{
  mLibrary.mComment = comment;
}

// @brief 時間単位を設定する．
void
CellManip::set_time_unit(tCellTimeUnit time_unit)
{
  mLibrary.mTimeUnit = time_unit;
}

// @brief 電圧単位を設定する．
void
CellManip::set_voltage_unit(tCellVoltageUnit voltage_unit)
{
  mLibrary.mVoltageUnit = voltage_unit;
}

// @brief 電流単位を設定する．
void
CellManip::set_current_unit(tCellCurrentUnit current_unit)
{
  mLibrary.mCurrentUnit = current_unit;
}

// @brief 抵抗単位を設定する．
void
CellManip::set_pulling_resistance_unit(tCellResistanceUnit resistance_unit)
{
  mLibrary.mResistanceUnit = resistance_unit;
}

// @brief 容量単位を設定する．
void
CellManip::set_capacitive_load_unit(tCellCapacitanceUnit load_unit)
{
  mLibrary.mCapacitanceUnit = load_unit;
}
  
// @brief 電力単位を設定する．
void
CellManip::set_leakage_power_unit(tCellPowerUnit power_unit)
{
  mLibrary.mPowerUnit = power_unit;
}
  
// @brief ルックアップテーブルのテンプレートを追加する．(1次元)
// @param[in] name テンプレートの名前
// @param[in] var1 1番目の変数の型
// @param[in] index_array1 1番目の変数のインデックス値の配列
// @return 生成されたテンプレートを返す．
CellLutTemplate*
CellManip::new_lut_template(ShString name,
			    tCellVarType var1,
			    const vector<double>& index_array1)
{
  if ( mLibrary.mLutTemplateDict.count(name) > 0 ) {
    // 名前が重複していた．
    cerr << "CellManip::new_lut_template(" << name
	 << "): duplicated." << endl;
    return NULL;
  }
  
  void* p = get_memory(sizeof(CellLutTemplate1D));
  CellLutTemplate* lut_templ = new (p) CellLutTemplate1D(name,
							 var1, index_array1);
  mLibrary.mLutTemplateDict.insert(make_pair(name, lut_templ));
  return lut_templ;
}
  
// @brief ルックアップテーブルのテンプレートを追加する．(2次元)
// @param[in] name テンプレートの名前
// @param[in] var1 1番目の変数の型
// @param[in] index_array1 1番目の変数のインデックス値の配列
// @param[in] var2 2番目の変数の型
// @param[in] index_array2 2番目の変数のインデックス値の配列
// @return 生成されたテンプレートを返す．
CellLutTemplate*
CellManip::new_lut_template(ShString name,
			    tCellVarType var1,
			    const vector<double>& index_array1,
			    tCellVarType var2,
			    const vector<double>& index_array2)
{
  if ( mLibrary.mLutTemplateDict.count(name) > 0 ) {
    // 名前が重複していた．
    cerr << "CellManip::new_lut_template(" << name
	 << "): duplicated." << endl;
    return NULL;
  }
  
  void* p = get_memory(sizeof(CellLutTemplate2D));
  CellLutTemplate* lut_templ = new (p) CellLutTemplate2D(name,
							 var1, index_array1,
							 var2, index_array2);
  mLibrary.mLutTemplateDict.insert(make_pair(name, lut_templ));
  return lut_templ;
}
  
// @brief ルックアップテーブルのテンプレートを追加する．(3次元)
// @param[in] name テンプレートの名前
// @param[in] var1 1番目の変数の型
// @param[in] index_array1 1番目の変数のインデックス値の配列
// @param[in] var2 2番目の変数の型
// @param[in] index_array2 2番目の変数のインデックス値の配列
// @param[in] var3 3番目の変数の型
// @param[in] index_array3 3番目の変数のインデックス値の配列
// @return 生成されたテンプレートを返す．
CellLutTemplate*
CellManip::new_lut_template(ShString name,
			    tCellVarType var1,
			    const vector<double>& index_array1,
			    tCellVarType var2,
			    const vector<double>& index_array2,
			    tCellVarType var3,
			    const vector<double>& index_array3)
{
  if ( mLibrary.mLutTemplateDict.count(name) > 0 ) {
    // 名前が重複していた．
    cerr << "CellManip::new_lut_template(" << name
	 << "): duplicated." << endl;
    return NULL;
  }

  void* p = get_memory(sizeof(CellLutTemplate3D));
  CellLutTemplate* lut_templ = new (p) CellLutTemplate3D(name,
							 var1, index_array1,
							 var2, index_array2,
							 var3, index_array3);
  mLibrary.mLutTemplateDict.insert(make_pair(name, lut_templ));
  return lut_templ;
}

// @brief バスタイプを追加する．
// @param[in] name バスタイプの名前
// @param[in] base_type base_type (今は kArrayType のみが使用可能)
// @param[in] data_type data_type (今は kBitType のみが使用可能)
// @param[in] bit_from ビットの開始位置
// @param[in] bit_to ビットの終了位置
// @return 生成されたバスタイプを返す．
// @note bit_width と downto の情報は bit_from と bit_to から生成する．
CellBusType*
CellManip::new_bus_type(ShString name,
			CellBusType::tBaseType base_type,
			CellBusType::tDataType data_type,
			ymint32 bit_from,
			ymint32 bit_to)
{
  if ( mLibrary.mBusTypeDict.count(name) > 0 ) {
    // 名前が重複していた．
    cerr << "CellManip::new_bus_type(" << name
	 << "): duplicated." << endl;
    return NULL;
  }
  
  void* p = get_memory(sizeof(CellBusType));
  CellBusType* bus_type = new (p) CellBusType(name, base_type, data_type,
					      bit_from, bit_to);
  mLibrary.mBusTypeDict.insert(make_pair(name, bus_type));
  return bus_type;
}

// @brief セルを追加する．
// @param[in] name セル名
// @param[in] area 面積
// @param[in] n_pin ピン数
// @param[in] n_bus バス数
// @param[in] n_bundle バンドル数
// @return 生成されたセルを返す．
// @note n_pin, n_bus, n_bundle の値は参考にするだけ．
// @note この時点ではピンもバスもバンドルも持たない．
Cell*
CellManip::new_cell(ShString name,
		    CellArea area,
		    ymuint32 n_pin,
		    ymuint32 n_bus,
		    ymuint32 n_bundle)
{
  if ( mLibrary.mCellDict.count(name) > 0 ) {
    // 名前が重複していた．
    cerr << "CellManip::new_cell(" << name
	 << "): duplicated." << endl;
    return NULL;
  }
  
  void* p = get_memory(sizeof(Cell));
  Cell* cell = new (p) Cell(name, area, n_pin, n_bus, n_bundle);
  mLibrary.mCellList.push_back(cell);
  mLibrary.mCellDict.insert(make_pair(name, cell));
  
  return cell;
}
  
// @brief セルに入力ピンを追加する．
// @param[in] cell セル
// @param[in] name ピン名
// @return 生成されたピンを返す．
CellInputPin*
CellManip::add_ipin(Cell* cell,
		    ShString name)
{
  void* p = get_memory(sizeof(CellInputPin));
  CellInputPin* pin = new (p) CellInputPin(name);
  cell->mPinList.push_back(pin);
  return pin;
}
  
// @brief セルに出力ピンを追加する．
// @param[in] cell セル
// @param[in] name ピン名
// @return 生成されたピンを返す．
CellOutputPin*
CellManip::add_opin(Cell* cell,
		    ShString name)
{
  void* p = get_memory(sizeof(CellOutputPin));
  CellOutputPin* pin = new (p) CellOutputPin(name);
  cell->mPinList.push_back(pin);
  return pin;
}

// @brief セルに入出力ピンを追加する．
// @param[in] cell セル
// @param[in] name ピン名
// @return 生成されたピンを返す．
CellInoutPin*
CellManip::add_iopin(Cell* cell,
		     ShString name)
{
  void* p = get_memory(sizeof(CellInoutPin));
  CellInoutPin* pin = new (p) CellInoutPin(name);
  cell->mPinList.push_back(pin);
  return pin;
}

// @brief セルにバスを追加する．
// @param[in] cell セル
// @param[in] name バス名
// @param[in] bus_type バスの型
// @return 生成されたバスを返す．
CellBus*
CellManip::add_bus(Cell* cell,
		   ShString name,
		   CellBusType* bus_type)
{
  void* p = get_memory(sizeof(CellBus));
  CellBus* bus = new (p) CellBus(name, bus_type);
  cell->mBusList.push_back(bus);
  return bus;
}

// @brief セルにバンドルを追加する．
// @param[in] cell セル
// @param[in] name バンドル名
// @param[in] n_pin ピン数
// @return 生成されたバンドルを返す．
// @note n_pin の値は参考にするだけ．
CellBundle*
CellManip::add_bundle(Cell* cell,
		      ShString name,
		      ymuint32 n_pin)
{
  void* p = get_memory(sizeof(CellBundle));
  CellBundle* bundle = new (p) CellBundle(name, n_pin);
  cell->mBundleList.push_back(bundle);
  return bundle;
}

// @brief ピンに負荷容量を設定する．
// @param[in] pin ピン
// @param[in] load 負荷容量
void
CellManip::set_capacitance(CellInputPin* pin,
			   CellCapacitance load)
{
  pin->mCapacitance = load;
}

// @brief ピンに立ち上がり時の負荷容量を設定する．
// @param[in] pin ピン
// @param[in] load 負荷容量
void
CellManip::set_rise_capacitance(CellInputPin* pin,
				CellCapacitance load)
{
  pin->mRiseCapacitance = load;
}

// @brief ピンに立ち下がり時の負荷容量を設定する．
// @param[in] pin ピン
// @param[in] load 負荷容量
void
CellManip::set_fall_capacitance(CellInputPin* pin,
				CellCapacitance load)
{
  pin->mFallCapacitance = load;
}

// @brief ピンに負荷容量を設定する．
// @param[in] pin ピン
// @param[in] load 負荷容量
void
CellManip::set_capacitance(CellInoutPin* pin,
			   CellCapacitance load)
{
  pin->mCapacitance = load;
}

// @brief ピンに立ち上がり時の負荷容量を設定する．
// @param[in] pin ピン
// @param[in] load 負荷容量
void
CellManip::set_rise_capacitance(CellInoutPin* pin,
				CellCapacitance load)
{
  pin->mRiseCapacitance = load;
}

// @brief ピンに立ち下がり時の負荷容量を設定する．
// @param[in] pin ピン
// @param[in] load 負荷容量
void
CellManip::set_fall_capacitance(CellInoutPin* pin,
				CellCapacitance load)
{
  pin->mFallCapacitance = load;
}
  
// @brief ピンに最大ファンアウト容量を設定する．
// @param[in] pin ピン
// @param[in] load 容量
void
CellManip::set_max_fanout(CellOutputPin* pin,
			  CellCapacitance load)
{
  pin->mMaxFanout = load;
}

// @brief ピンに最小ファンアウト容量を設定する．
// @param[in] pin ピン
// @param[in] load 容量
void
CellManip::set_min_fanout(CellOutputPin* pin,
			  CellCapacitance load)
{
  pin->mMinFanout = load;
}

// @brief ピンに最大負荷容量を設定する．
// @param[in] pin ピン
// @param[in] load 容量
void
CellManip::set_max_capacitance(CellOutputPin* pin,
			       CellCapacitance load)
{
  pin->mMaxCapacitance = load;
}

// @brief ピンに最小負荷容量を設定する．
// @param[in] pin ピン
// @param[in] load 容量
void
CellManip::set_min_capacitance(CellOutputPin* pin,
			       CellCapacitance load)
{
  pin->mMinCapacitance = load;
}
  
// @brief ピンに最大遷移時間を設定する．
// @param[in] pin ピン
// @param[in] time 遷移時間
void
CellManip::set_max_transition(CellOutputPin* pin,
			      CellTime time)
{
  pin->mMaxTransition = time;
}
  
// @brief ピンに最小遷移時間を設定する．
// @param[in] pin ピン
// @param[in] time 遷移時間
void
CellManip::set_min_transition(CellOutputPin* pin,
			      CellTime time)
{
  pin->mMinTransition = time;
}

// @brief セルにタイミング情報を追加する．(CMOSジェネリックタイプ)
// @param[in] cell セル
// @param[in] ipin_list 開始ピンのリスト
// @param[in] opin 終点ピン
// @param[in] timing_sense タイミング条件
// @param[in] timing_type タイミングの型
// @param[in] intrisic_rise 立ち上がり固有遅延
// @param[in] intrisic_fall 立ち下がり固有遅延
// @param[in] slope_rise 立ち上がりスロープ遅延
// @param[in] slope_fall 立ち下がりスロープ遅延
// @param[in] rise_resistance 立ち上がり遷移遅延パラメータ
// @param[in] fall_resistance 立ち下がり遷移遅延パラメータ
// @return 生成されたタイミング情報を返す．
CellTiming*
CellManip::add_timing_generic(Cell* cell,
			      const vector<const CellPin*>& ipin_list,
			      CellPin* opin,
			      tCellTimingSense timing_sense,
			      tCellTimingType timing_type,
			      CellTime intrisic_rise,
			      CellTime intrisic_fall,
			      CellTime slope_rise,
			      CellTime slope_fall,
			      CellResistance rise_resistance,
			      CellResistance fall_resistance)
{
  void* p = get_memory(sizeof(CellTimingGeneric));
  CellTiming* timing = new (p) CellTimingGeneric(ipin_list, opin,
						 timing_sense,
						 timing_type,
						 intrisic_rise,
						 intrisic_fall,
						 slope_rise,
						 slope_fall,
						 rise_resistance,
						 fall_resistance);
  cell->mTimingList.push_back(timing);
  return timing;
}

// @brief セルにタイミング情報を追加する．(CMOS折れ線近似タイプ)
// @param[in] cell セル
// @param[in] ipin_list 開始ピンのリスト
// @param[in] opin 終点ピン
// @param[in] timing_sense タイミング条件
// @param[in] timing_type タイミングの型
// @param[in] intrisic_rise 立ち上がり固有遅延
// @param[in] intrisic_fall 立ち下がり固有遅延
// @param[in] slope_rise 立ち上がりスロープ遅延
// @param[in] slope_fall 立ち下がりスロープ遅延
// @return 生成されたタイミング情報を返す．
CellTiming*
CellManip::add_timing_piecewise(Cell* cell,
				const vector<const CellPin*>& ipin_list,
				CellPin* opin,
				tCellTimingSense timing_sense,
				tCellTimingType timing_type,
				CellTime intrisic_rise,
				CellTime intrisic_fall,
				CellTime slope_rise,
				CellTime slope_fall, 
				CellResistance rise_pin_resistance,
				CellResistance fall_pin_resistance)
{
  void* p = get_memory(sizeof(CellTimingPiecewise));
  CellTiming* timing = new (p) CellTimingPiecewise(ipin_list, opin,
						   timing_sense,
						   timing_type,
						   intrisic_rise,
						   intrisic_fall,
						   slope_rise,
						   slope_fall,
						   rise_pin_resistance,
						   fall_pin_resistance);
  cell->mTimingList.push_back(timing);
  return timing;
}

// @brief セルにタイミング情報を追加する．(CMOS非線形タイプ)
// @param[in] cell セル
// @param[in] ipin_list 開始ピンのリスト
// @param[in] opin 終点ピン
// @param[in] timing_sense タイミング条件
// @param[in] timing_type タイミングの型
// @return 生成されたタイミング情報を返す．
CellTiming*
CellManip::add_timing_nonlinear(Cell* cell,
				const vector<const CellPin*>& ipin_list,
				CellPin* opin,
				tCellTimingSense timing_sense,
				tCellTimingType timing_type,
				CellLut* cell_rise,
				CellLut* cell_fall)
{
  void* p = get_memory(sizeof(CellTimingNonlinear1));
  CellTiming* timing = new (p) CellTimingNonlinear1(ipin_list, opin,
						    timing_sense,
						    timing_type,
						    cell_rise,
						    cell_fall);
  cell->mTimingList.push_back(timing);
  return timing;
}

// @brief セルにタイミング情報を追加する．(CMOS非線形タイプ)
// @param[in] cell セル
// @param[in] ipin_list 開始ピンのリスト
// @param[in] opin 終点ピン
// @param[in] timing_sense タイミング条件
// @param[in] timing_type タイミングの型
// @param[in] intrisic_rise 立ち上がり固有遅延
// @param[in] intrisic_fall 立ち下がり固有遅延
// @param[in] slope_rise 立ち上がりスロープ遅延
// @param[in] slope_fall 立ち下がりスロープ遅延
// @param[in] rise_resistance 立ち上がり遷移遅延パラメータ
// @param[in] fall_resistance 立ち下がり遷移遅延パラメータ
// @return 生成されたタイミング情報を返す．
CellTiming*
CellManip::add_timing_nonlinear(Cell* cell,
				const vector<const CellPin*>& ipin_list,
				CellPin* opin,
				tCellTimingSense timing_sense,
				tCellTimingType timing_type,
				CellLut* rise_transition,
				CellLut* fall_transition,
				CellLut* rise_propagation,
				CellLut* fall_propagation)
{
  void* p = get_memory(sizeof(CellTimingNonlinear2));
  CellTiming* timing = new (p) CellTimingNonlinear2(ipin_list, opin,
						    timing_sense,
						    timing_type,
						    rise_transition,
						    fall_transition,
						    rise_propagation,
						    fall_propagation);
  cell->mTimingList.push_back(timing);
  return timing;
}

// @brief メモリ領域を確保する．
void*
CellManip::get_memory(size_t size)
{
  return mLibrary.mAlloc.get_memory(size);
}

END_NAMESPACE_YM_CELL
