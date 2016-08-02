
/// @file libym_cell/CellTiming.cc
/// @brief CellTiming の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: CellTiming.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "CellTimingImpl.h"


BEGIN_NAMESPACE_YM_CELL

//////////////////////////////////////////////////////////////////////
// クラス CellTiming
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] ipin_list 開始ピンのリスト
// @param[in] opin 終点ピン
// @param[in] timing_sense タイミング条件
// @param[in] timing_type タイミングの型
CellTiming::CellTiming(const vector<const CellPin*>& ipin_list,
		       CellPin* opin,
		       tCellTimingSense timing_sense,
		       tCellTimingType timing_type) :
  mIpinList(ipin_list),
  mOpin(opin),
  mTimingSense(timing_sense),
  mTimingType(timing_type)
{
}

// @brief デストラクタ
CellTiming::~CellTiming()
{
}

// @brief 立ち上がり固有遅延の取得
CellTime
CellTiming::intrinsic_rise() const
{
  return CellTime(0.0);
}

// @brief 立ち下がり固有遅延の取得
CellTime
CellTiming::intrinsic_fall() const
{
  return CellTime(0.0);
}

// @brief 立ち上がりスロープ遅延の取得
CellTime
CellTiming::slope_rise() const
{
  return CellTime(0.0);
}

// @brief 立ち下がりスロープ遅延の取得
CellTime
CellTiming::slope_fall() const
{
  return CellTime(0.0);
}
  
// @brief 立ち上がり遷移遅延の取得
CellResistance
CellTiming::rise_resistance() const
{
  return CellResistance(0.0);
}

// @brief 立ち下がり遷移遅延の取得
CellResistance
CellTiming::fall_resistance() const
{
  return CellResistance(0.0);
}
  
// @brief 立ち上がり遷移遅延の取得
CellResistance
CellTiming::rise_pin_resistance() const
{
  return CellResistance(0.0);
}

// @brief 立ち下がり遷移遅延の取得
CellResistance
CellTiming::fall_pin_resistance() const
{
  return CellResistance(0.0);
}

#if 0
// @brief 立ち上がり？？？
CellTime
CellTiming::rise_delay_intercept() const
{
  return CellTime(0.0);
}

// @brief 立ち下がり？？？
CellTime
CellTiming::fall_delay_intercept() const
{
  return CellTime(0.0);
}
#endif

// @brief 立ち上がり遷移遅延テーブルの取得
const CellLut*
CellTiming::rise_transition() const
{
  return NULL;
}

// @brief 立ち下がり遷移遅延テーブルの取得
const CellLut*
CellTiming::fall_transition() const
{
  return NULL;
}

// @brief 立ち上がり伝搬遅延テーブルの取得
const CellLut*
CellTiming::rise_propagation() const
{
  return NULL;
}

// @brief 立ち下がり伝搬遅延テーブルの取得
const CellLut*
CellTiming::fall_propagation() const
{
  return NULL;
}
  
// @brief 立ち上がりセル遅延テーブルの取得
const CellLut*
CellTiming::cell_rise() const
{
  return NULL;
}

// @brief 立ち下がりセル遅延テーブルの取得
const CellLut*
CellTiming::cell_fall() const
{
  return NULL;
}


//////////////////////////////////////////////////////////////////////
// クラス CellTimingBase
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] ipin_list 開始ピンのリスト
// @param[in] opin 終点ピン
// @param[in] timing_sense タイミング条件
// @param[in] timing_type タイミングの型
// @param[in] intrisic_rise 立ち上がり固有遅延
// @param[in] intrisic_fall 立ち下がり固有遅延
// @param[in] slope_rise 立ち上がりスロープ遅延
// @param[in] slope_fall 立ち下がりスロープ遅延
CellTimingBase::CellTimingBase(const vector<const CellPin*>& ipin_list,
			       CellPin* opin,
			       tCellTimingSense timing_sense,
			       tCellTimingType timing_type,
			       CellTime intrisic_rise,
			       CellTime intrisic_fall,
			       CellTime slope_rise,
			       CellTime slope_fall) :
  CellTiming(ipin_list, opin, timing_sense, timing_type),
  mIntrisicRise(intrisic_rise),
  mIntrisicFall(intrisic_fall),
  mSlopeRise(slope_rise),
  mSlopeFall(slope_fall)
{
}

// @brief デストラクタ
CellTimingBase::~CellTimingBase()
{
}

// @brief 立ち上がり固有遅延の取得
CellTime
CellTimingBase::intrinsic_rise() const
{
  return mIntrisicRise;
}

// @brief 立ち下がり固有遅延の取得
CellTime
CellTimingBase::intrinsic_fall() const
{
  return mIntrisicFall;
}

// @brief 立ち上がりスロープ遅延の取得
CellTime
CellTimingBase::slope_rise() const
{
  return mSlopeRise;
}

// @brief 立ち下がりスロープ遅延の取得
CellTime
CellTimingBase::slope_fall() const
{
  return mSlopeFall;
}


//////////////////////////////////////////////////////////////////////
// クラス CellTimingGeneric
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
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
CellTimingGeneric::CellTimingGeneric(const vector<const CellPin*>& ipin_list,
				     CellPin* opin,
				     tCellTimingSense timing_sense,
				     tCellTimingType timing_type,
				     CellTime intrisic_rise,
				     CellTime intrisic_fall,
				     CellTime slope_rise,
				     CellTime slope_fall,
				     CellResistance rise_resistance,
				     CellResistance fall_resistance) :
  CellTimingBase(ipin_list, opin, timing_sense, timing_type,
		 intrisic_rise, intrisic_fall,
		 slope_rise, slope_fall),
  mRiseResistance(rise_resistance),
  mFallResistance(fall_resistance)
{
}

// @brief デストラクタ
CellTimingGeneric::~CellTimingGeneric()
{
}
  
// @brief 立ち上がり遷移遅延の取得
CellResistance
CellTimingGeneric::rise_resistance() const
{
  return mRiseResistance;
}

// @brief 立ち下がり遷移遅延の取得
CellResistance
CellTimingGeneric::fall_resistance() const
{
  return mFallResistance;
}


//////////////////////////////////////////////////////////////////////
// クラス CellTimingPiecewise
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] ipin_list 開始ピンのリスト
// @param[in] opin 終点ピン
// @param[in] timing_sense タイミング条件
// @param[in] timing_type タイミングの型
// @param[in] intrisic_rise 立ち上がり固有遅延
// @param[in] intrisic_fall 立ち下がり固有遅延
// @param[in] slope_rise 立ち上がりスロープ遅延
// @param[in] slope_fall 立ち下がりスロープ遅延
CellTimingPiecewise::CellTimingPiecewise(const vector<const CellPin*>& ipin_list,
					 CellPin* opin,
					 tCellTimingSense timing_sense,
					 tCellTimingType timing_type,
					 CellTime intrisic_rise,
					 CellTime intrisic_fall,
					 CellTime slope_rise,
					 CellTime slope_fall, 
					 CellResistance rise_pin_resistance,
					 CellResistance fall_pin_resistance) :
  CellTimingBase(ipin_list, opin, timing_sense, timing_type,
		 intrisic_rise, intrisic_fall,
		 slope_rise, slope_fall),
  mRisePinResistance(rise_pin_resistance),
  mFallPinResistance(fall_pin_resistance)
{
}

// @brief デストラクタ
CellTimingPiecewise::~CellTimingPiecewise()
{
}
  
// @brief 立ち上がり遷移遅延の取得
CellResistance
CellTimingPiecewise::rise_pin_resistance() const
{
  return mRisePinResistance;
}

// @brief 立ち下がり遷移遅延の取得
CellResistance
CellTimingPiecewise::fall_pin_resistance() const
{
  return mFallPinResistance;
}

#if 0
// @brief 立ち上がり？？？
CellTime
CellTimingPiecewise::rise_delay_intercept() const
{
}

// @brief 立ち下がり？？？
CellTime
CellTimingPiecewise::fall_delay_intercept() const
{
}
#endif


//////////////////////////////////////////////////////////////////////
// クラス CellTimingNonlinear1
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] ipin_list 開始ピンのリスト
// @param[in] opin 終点ピン
// @param[in] timing_sense タイミング条件
// @param[in] timing_type タイミングの型
// @param[in] cell_rise 立ち上がりセル遅延テーブル
// @param[in] cell_fall 立ち下がりセル遅延テーブル
CellTimingNonlinear1::CellTimingNonlinear1(const vector<const CellPin*>& ipin_list,
					   CellPin* opin,
					   tCellTimingSense timing_sense,
					   tCellTimingType timing_type,
					   CellLut* cell_rise,
					   CellLut* cell_fall) :
  CellTiming(ipin_list, opin, timing_sense, timing_type),
  mCellRise(cell_rise),
  mCellFall(cell_fall)
{
}

// @brief デストラクタ
CellTimingNonlinear1::~CellTimingNonlinear1()
{
}
  
// @brief 立ち上がりセル遅延テーブルの取得
const CellLut*
CellTimingNonlinear1::cell_rise() const
{
  return mCellRise;
}

// @brief 立ち下がりセル遅延テーブルの取得
const CellLut*
CellTimingNonlinear1::cell_fall() const
{
  return mCellFall;
}


//////////////////////////////////////////////////////////////////////
// クラス CellTimingNonlinear2
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] ipin_list 開始ピンのリスト
// @param[in] opin 終点ピン
// @param[in] timing_sense タイミング条件
// @param[in] timing_type タイミングの型
// @param[in] rise_transition 立ち上がり遷移遅延テーブル
// @param[in] fall_transition 立ち下がり遷移遅延テーブル
// @param[in] rise_propagation 立ち上がり伝搬遅延テーブル
// @param[in] fall_propagation 立ち下がり伝搬遅延テーブル
CellTimingNonlinear2::CellTimingNonlinear2(const vector<const CellPin*>& ipin_list,
					   CellPin* opin,
					   tCellTimingSense timing_sense,
					   tCellTimingType timing_type,
					   CellLut* rise_transition,
					   CellLut* fall_transition,
					   CellLut* rise_propagation,
					   CellLut* fall_propagation) :
  CellTiming(ipin_list, opin, timing_sense, timing_type),
  mRiseTransition(rise_transition),
  mFallTransition(fall_transition),
  mRisePropagation(rise_propagation),
  mFallPropagation(fall_propagation)
{
}
	
// @brief デストラクタ
CellTimingNonlinear2::~CellTimingNonlinear2()
{
}

// @brief 立ち上がり遷移遅延テーブルの取得
const CellLut*
CellTimingNonlinear2::rise_transition() const
{
  return mRiseTransition;
}

// @brief 立ち下がり遷移遅延テーブルの取得
const CellLut*
CellTimingNonlinear2::fall_transition() const
{
  return mFallTransition;
}

// @brief 立ち上がり伝搬遅延テーブルの取得
const CellLut*
CellTimingNonlinear2::rise_propagation() const
{
  return mRisePropagation;
}

// @brief 立ち下がり伝搬遅延テーブルの取得
const CellLut*
CellTimingNonlinear2::fall_propagation() const
{
  return mFallPropagation;
}

END_NAMESPACE_YM_CELL
