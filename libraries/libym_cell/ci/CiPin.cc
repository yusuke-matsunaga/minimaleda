
/// @file libym_cell/CiPin.cc
/// @brief CiPin の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "CiPin.h"


BEGIN_NAMESPACE_YM_CELL

//////////////////////////////////////////////////////////////////////
// クラス CiPin
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] name ピン名
CiPin::CiPin(const ShString& name) :
  mName(name)
{
}

// @brief デストラクタ
CiPin::~CiPin()
{
}

// @brief ピン番号を返す．
ymuint
CiPin::id() const
{
  return mId;
}

// @brief ピン名を返す．
string
CiPin::name() const
{
  return mName;
}

// @brief 負荷容量を返す．
CellCapacitance
CiPin::capacitance() const
{
  return CellCapacitance(0.0);
}

// @brief 立ち上がり時の負荷容量を返す．
CellCapacitance
CiPin::rise_capacitance() const
{
  return CellCapacitance(0.0);
}

// @brief 立ち下がり時の負荷容量を返す．
CellCapacitance
CiPin::fall_capacitance() const
{
  return CellCapacitance(0.0);
}

// @brief 論理式を持っているときに true を返す．
bool
CiPin::has_function() const
{
  return false;
}

// @brief 機能を表す論理式を返す．
LogExpr
CiPin::function() const
{
  return LogExpr::make_zero();
}

// @brief 最大ファンアウト容量を返す．
CellCapacitance
CiPin::max_fanout() const
{
  return CellCapacitance(0.0);
}

// @brief 最小ファンアウト容量を返す．
CellCapacitance
CiPin::min_fanout() const
{
  return CellCapacitance(0.0);
}

// @brief 最大負荷容量を返す．
CellCapacitance
CiPin::max_capacitance() const
{
  return CellCapacitance(0.0);
}

// @brief 最小負荷容量を返す．
CellCapacitance
CiPin::min_capacitance() const
{
  return CellCapacitance(0.0);
}

// @brief 最大遷移時間を返す．
CellTime
CiPin::max_transition() const
{
  return CellTime(0.0);
}

// @brief 最小遷移時間を返す．
CellTime
CiPin::min_transition() const
{
  return CellTime(0.0);
}

// @brief タイミング情報の取得
// @param[in] ipos 開始ピン番号
// @param[in] timing_sense タイミング情報の摘要条件
// @return 条件に合致するタイミング情報を返す．
// @note なければ NULL を返す．
const CellTiming*
CiPin::timing(ymuint ipos,
	      tCellTimingSense sense) const
{
  return NULL;
}

// @brief 出力ピン(入出力ピン)の関数を設定する．
// @param[in] function 関数を表す論理式
void
CiPin::set_function(const LogExpr& function)
{
}

// @brief 出力ピン(入出力ピン)のタイミング情報を設定する．
// @param[in] pin_id 入力ピンのピン番号
// @param[in] sense タイミング情報の適用条件
// @param[in] timing 設定するタイミング情報
void
CiPin::set_timing(ymuint pin_id,
		  tCellTimingSense sense,
		  const CellTiming* timing)
{
}


//////////////////////////////////////////////////////////////////////
// クラス CiInputPin
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] name ピン名
// @param[in] capacitance 負荷容量
// @param[in] rise_capacitance 立ち上がり時の負荷容量
// @param[in] fall_capacitance 立ち下がり時の負荷容量
CiInputPin::CiInputPin(const ShString& name,
		       CellCapacitance capacitance,
		       CellCapacitance rise_capacitance,
		       CellCapacitance fall_capacitance) :
  CiPin(name),
  mCapacitance(capacitance),
  mRiseCapacitance(rise_capacitance),
  mFallCapacitance(fall_capacitance)
{
}

// @brief デストラクタ
CiInputPin::~CiInputPin()
{
}

// @brief 方向を返す．
tCellDirection
CiInputPin::direction() const
{
  return kDirInput;
}

// @brief 負荷容量を返す．
CellCapacitance
CiInputPin::capacitance() const
{
  return mCapacitance;
}

// @brief 立ち上がり時の負荷容量を返す．
CellCapacitance
CiInputPin::rise_capacitance() const
{
  return mRiseCapacitance;
}

// @brief 立ち下がり時の負荷容量を返す．
CellCapacitance
CiInputPin::fall_capacitance() const
{
  return mFallCapacitance;
}


//////////////////////////////////////////////////////////////////////
// クラス CiOutputPin
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] name ピン名
// @param[in] max_fanout 最大ファンアウト容量
// @param[in] min_fanout 最小ファンアウト容量
// @param[in] max_capacitance 最大負荷容量
// @param[in] min_capacitance 最小負荷容量
// @param[in] max_transition 最大遷移時間
// @param[in] min_transition 最小遷移時間
CiOutputPin::CiOutputPin(const ShString& name,
			 CellCapacitance max_fanout,
			 CellCapacitance min_fanout,
			 CellCapacitance max_capacitance,
			 CellCapacitance min_capacitance,
			 CellTime max_transition,
			 CellTime min_transition) :
  CiPin(name),
  mHasFunction(false),
  mMaxFanout(max_fanout),
  mMinFanout(min_fanout),
  mMaxCapacitance(max_capacitance),
  mMinCapacitance(min_capacitance),
  mMaxTransition(max_transition),
  mMinTransition(min_transition)
{
}

// @brief デストラクタ
CiOutputPin::~CiOutputPin()
{
}

// @brief 方向を返す．
tCellDirection
CiOutputPin::direction() const
{
  return kDirOutput;
}

// @brief 論理式を持っているときに true を返す．
bool
CiOutputPin::has_function() const
{
  return mHasFunction;
}

// @brief 機能を表す論理式を返す．
LogExpr
CiOutputPin::function() const
{
  return mFunction;
}

// @brief 最大ファンアウト容量を返す．
CellCapacitance
CiOutputPin::max_fanout() const
{
  return mMaxFanout;
}

// @brief 最小ファンアウト容量を返す．
CellCapacitance
CiOutputPin::min_fanout() const
{
  return mMinFanout;
}

// @brief 最大負荷容量を返す．
CellCapacitance
CiOutputPin::max_capacitance() const
{
  return mMaxCapacitance;
}

// @brief 最小負荷容量を返す．
CellCapacitance
CiOutputPin::min_capacitance() const
{
  return mMinCapacitance;
}

// @brief 最大遷移時間を返す．
CellTime
CiOutputPin::max_transition() const
{
  return mMaxTransition;;
}

// @brief 最小遷移時間を返す．
CellTime
CiOutputPin::min_transition() const
{
  return mMinTransition;
}

// @brief タイミング情報の取得
// @param[in] ipos 開始ピン番号
// @param[in] timing_sense タイミング情報の摘要条件
// @return 条件に合致するタイミング情報を返す．
// @note なければ NULL を返す．
const CellTiming*
CiOutputPin::timing(ymuint ipos,
		    tCellTimingSense sense) const
{
  ymuint offset = 0;
  if ( sense == kSenseNegaUnate ) {
    offset = 1;
  }
  return mTimingArray[ipos * 2 + offset];
}

// @brief 出力ピン(入出力ピン)の関数を設定する．
// @param[in] function 関数を表す論理式
void
CiOutputPin::set_function(const LogExpr& function)
{
  mHasFunction = true;
  mFunction = function;
}

// @brief 出力ピン(入出力ピン)のタイミング情報を設定する．
// @param[in] pin_id 入力ピンのピン番号
// @param[in] sense タイミング情報の適用条件
// @param[in] timing 設定するタイミング情報
void
CiOutputPin::set_timing(ymuint pin_id,
			tCellTimingSense sense,
			const CellTiming* timing)
{
  switch ( sense ) {
  case kSensePosiUnate:
    mTimingArray[pin_id * 2 + 0] = timing;
    break;

  case kSenseNegaUnate:
    mTimingArray[pin_id * 2 + 1] = timing;
    break;

  case kSenseNonUnate:
    mTimingArray[pin_id * 2 + 0] = timing;
    mTimingArray[pin_id * 2 + 1] = timing;
    break;

  default:
    assert_not_reached(__FILE__, __LINE__);
  }
}


//////////////////////////////////////////////////////////////////////
// クラス CiInoutPin
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] name ピン名
// @param[in] capacitance 負荷容量
// @param[in] rise_capacitance 立ち上がり時の負荷容量
// @param[in] fall_capacitance 立ち下がり時の負荷容量
// @param[in] max_fanout 最大ファンアウト容量
// @param[in] min_fanout 最小ファンアウト容量
// @param[in] max_capacitance 最大負荷容量
// @param[in] min_capacitance 最小負荷容量
// @param[in] max_transition 最大遷移時間
// @param[in] min_transition 最小遷移時間
CiInoutPin::CiInoutPin(const ShString& name,
		       CellCapacitance capacitance,
		       CellCapacitance rise_capacitance,
		       CellCapacitance fall_capacitance,
		       CellCapacitance max_fanout,
		       CellCapacitance min_fanout,
		       CellCapacitance max_capacitance,
		       CellCapacitance min_capacitance,
		       CellTime max_transition,
		       CellTime min_transition) :
  CiOutputPin(name, max_fanout, min_fanout,
	      max_capacitance, min_capacitance,
	      max_transition, min_transition),
  mCapacitance(capacitance),
  mRiseCapacitance(mRiseCapacitance),
  mFallCapacitance(mFallCapacitance)
{
}

// @brief デストラクタ
CiInoutPin::~CiInoutPin()
{
}

// @brief 方向を返す．
tCellDirection
CiInoutPin::direction() const
{
  return kDirInput;
}

// @brief 負荷容量を返す．
CellCapacitance
CiInoutPin::capacitance() const
{
  return mCapacitance;
}

// @brief 立ち上がり時の負荷容量を返す．
CellCapacitance
CiInoutPin::rise_capacitance() const
{
  return mRiseCapacitance;
}

// @brief 立ち下がり時の負荷容量を返す．
CellCapacitance
CiInoutPin::fall_capacitance() const
{
  return mFallCapacitance;
}

END_NAMESPACE_YM_CELL
