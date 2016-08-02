
/// @file libym_cell/Cell.cc
/// @brief Cell の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: Cell.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_cell/Cell.h"
#include "ym_cell/CellBus.h"
#include "ym_cell/CellBundle.h"
#include "ym_cell/CellTiming.h"


BEGIN_NAMESPACE_YM_CELL

#if 0

//////////////////////////////////////////////////////////////////////
// クラス Cell
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ．
Cell::Cell(ShString name,
	   CellArea area,
	   ymuint32 n_pin,
	   ymuint32 n_bus,
	   ymuint32 n_bundle) :
  mName(name),
  mArea(area)
{
  mPinList.reserve(n_pin);
  mBusList.reserve(n_bus);
  mBundleList.reserve(n_bundle);
}

// @brief デストラクタ
Cell::~Cell()
{
}

// @brief 名前からバスの取得
// @param[in] name バス名
// @return name という名前のバスを返す．
// @note なければ NULL を返す．
const CellBus*
Cell::bus(const string& name) const
{
  // 単純な線形探索
  ShString sh_name(name);
  for (vector<CellBus*>::const_iterator p = mBusList.begin();
       p != mBusList.end(); ++ p) {
    const CellBus* bus = *p;
    if ( bus->name() == sh_name ) {
      return bus;
    }
  }
  return NULL;
}

// @brief 名前からバンドルの取得
const CellBundle*
Cell::bundle(const string& name) const
{
  // 単純な線形探索
  ShString sh_name(name);
  for (vector<CellBundle*>::const_iterator p = mBundleList.begin();
       p != mBundleList.end(); ++ p) {
    const CellBundle* bundle = *p;
    if ( bundle->name() == sh_name ) {
      return bundle;
    }
  }
  return NULL;
}

// @brief タイミング情報の取得
// @param[in] ipos 開始ピン番号
// @param[in] opos 終了ピン番号
// @param[out] timing_list タイミング情報を納めるベクタ
// @return 条件に合致するタイミング情報の数を返す．
ymuint32
Cell::timing(ymuint32 ipos,
	     ymuint32 opos,
	     vector<const CellTiming*>& timing_list) const
{
  // 単純な線形探索
  const CellPin* ipin = pin(ipos);
  const CellPin* opin = pin(opos);
  timing_list.clear();
  for (vector<CellTiming*>::const_iterator p = mTimingList.begin();
       p != mTimingList.end(); ++ p) {
    const CellTiming* timing = *p;
    if ( timing->opin() != opin ) continue;
    ymuint32 ni = timing->n_ipins();
    for (ymuint32 i = 0; i < ni; ++ i) {
      if ( timing->ipin(i) == ipin ) {
	timing_list.push_back(timing);
	break;
      }
    }
  }
  return timing_list.size();
}

// @brief タイミング情報の取得
// @param[in] ipos 開始ピン番号
// @param[in] opos 終了ピン番号
// @param[in] timing_sense タイミング情報の摘要条件
// @param[in] timing_type タイミング情報の種類
// @param[out] timing_list タイミング情報を納めるベクタ
// @return 条件に合致するタイミング情報の数を返す．
ymuint32
Cell::timing(ymuint32 ipos,
	     ymuint32 opos,
	     tCellTimingSense timing_sense,
	     tCellTimingType timing_type,
	     vector<const CellTiming*>& timing_list) const
{
  // 単純な線形探索
  const CellPin* ipin = pin(ipos);
  const CellPin* opin = pin(opos);
  timing_list.clear();
  for (vector<CellTiming*>::const_iterator p = mTimingList.begin();
       p != mTimingList.end(); ++ p) {
    const CellTiming* timing = *p;
    if ( timing->opin() != opin ) continue;
#warning "TODO: timing_sense|timing_type のマッチのルールを考える．"
    if ( timing->timing_sense() != timing_sense ) continue;
    if ( timing->timing_type() != timing_type ) continue;
    ymuint32 ni = timing->n_ipins();
    for (ymuint32 i = 0; i < ni; ++ i) {
      if ( timing->ipin(i) == ipin ) {
	timing_list.push_back(timing);
	break;
      }
    }
  }
  return timing_list.size();
}

// @brief デバッグ用の内容表示関数
// @param[in] s 出力先のストリーム
void
Cell::dump(ostream& s) const
{
}


//////////////////////////////////////////////////////////////////////
// クラス CellBusType
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
CellBusType::CellBusType(ShString name,
			 tBaseType base_type,
			 tDataType data_type,
			 ymint32 bit_from,
			 ymint32 bit_to) :
  mName(name),
  mBaseType(base_type),
  mDataType(data_type),
  mBitFrom(bit_from),
  mBitTo(bit_to)
{
  if ( bit_from <= bit_to ) {
    mBitWidth = bit_to - bit_from + 1;
    mDownTo = false;
  }
  else {
    mBitWidth = bit_from - bit_to + 1;
    mDownTo = true;
  }
}

// @brief デストラクタ
CellBusType::~CellBusType()
{
}


//////////////////////////////////////////////////////////////////////
// クラス CellBus
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] name バス名
// @param[in] bus_type バス型
CellBus::CellBus(ShString name,
		 const CellBusType* bus_type) :
  mName(name),
  mBusType(bus_type)
{
  mPinList.reserve(bus_type->bit_width());
}

// @brief デストラクタ
CellBus::~CellBus()
{
}


//////////////////////////////////////////////////////////////////////
// クラス CellBundle
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
CellBundle::CellBundle(ShString name,
		       ymuint32 n_pin) :
  mName(name)
{
  mPinList.reserve(n_pin);
}

// @brief デストラクタ
CellBundle::~CellBundle()
{
}
#endif

END_NAMESPACE_YM_CELL
