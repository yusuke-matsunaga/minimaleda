
/// @file libym_cell/CellPin.cc
/// @brief CellPin の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: CellPin.cc 1978 2009-02-06 12:29:16Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "CellPinImpl.h"


BEGIN_NAMESPACE_YM_CELL

//////////////////////////////////////////////////////////////////////
// クラス CellPin
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] name 名前
CellPin::CellPin(const ShString& name) :
  mName(name)
{
}

// @brief デストラクタ
CellPin::~CellPin()
{
}

// @brief 負荷容量を返す．
// @note デフォルトでは 0.0 を返す．
CellCapacitance
CellPin::capacitance() const
{
  return CellCapacitance(0.0);
}

// @brief 立ち上がり時の負荷容量を返す．
// @note デフォルトでは 0.0 を返す．
CellCapacitance
CellPin::rise_capacitance() const
{
  return CellCapacitance(0.0);
}

// @brief 立ち下がり時の負荷容量を返す．
// @note デフォルトでは 0.0 を返す．
CellCapacitance
CellPin::fall_capacitance() const
{
  return CellCapacitance(0.0);
}

// @brief 最大ファンアウト容量を返す．
// @note デフォルトでは 0.0 を返す．
CellCapacitance
CellPin::max_fanout() const
{
  return CellCapacitance(0.0);
}

// @brief 最小ファンアウト容量を返す．
// @note デフォルトでは 0.0 を返す．
CellCapacitance
CellPin::min_fanout() const
{
  return CellCapacitance(0.0);
}
  
// @brief 最大負荷容量を返す．
// @note デフォルトでは 0.0 を返す．
CellCapacitance
CellPin::max_capacitance() const
{
  return CellCapacitance(0.0);
}
  
// @brief 最小負荷容量を返す．
// @note デフォルトでは 0.0 を返す．
CellCapacitance
CellPin::min_capacitance() const
{
  return CellCapacitance(0.0);
}
  
// @brief 最大遷移時間を返す．
// @note デフォルトでは 0.0 を返す．
CellTime
CellPin::max_transition() const
{
  return CellTime(0.0);
}

// @brief 最小遷移時間を返す．
// @note デフォルトでは 0.0 を返す．
CellTime
CellPin::min_transition() const
{
  return CellTime(0.0);
}


//////////////////////////////////////////////////////////////////////
// クラス CellInputPin
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] name 名前
CellInputPin::CellInputPin(const ShString& name) :
  CellPin(name)
{
}

/// @brief デストラクタ
CellInputPin::~CellInputPin()
{
}

// @brief 方向を返す．
tCellDirection
CellInputPin::direction() const
{
  return kDirInput;
}

// @brief 負荷容量を返す．
CellCapacitance
CellInputPin::capacitance() const
{
  return mCapacitance;
}

// @brief 立ち上がり時の負荷容量を返す．
CellCapacitance
CellInputPin::rise_capacitance() const
{
  return mRiseCapacitance;
}

// @brief 立ち下がり時の負荷容量を返す．
CellCapacitance
CellInputPin::fall_capacitance() const
{
  return mFallCapacitance;
}


//////////////////////////////////////////////////////////////////////
// クラス CellOutputPin
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] name 名前
CellOutputPin::CellOutputPin(const ShString& name) :
  CellPin(name)
{
}

// @brief デストラクタ
CellOutputPin::~CellOutputPin()
{
}

// @brief 方向を返す．
tCellDirection
CellOutputPin::direction() const
{
  return kDirOutput;
}

// @brief 最大ファンアウト容量を返す．
CellCapacitance
CellOutputPin::max_fanout() const
{
  return mMaxFanout;
}

// @brief 最小ファンアウト容量を返す．
CellCapacitance
CellOutputPin::min_fanout() const
{
  return mMinFanout;
}
  
// @brief 最大負荷容量を返す．
CellCapacitance
CellOutputPin::max_capacitance() const
{
  return mMaxCapacitance;
}
  
// @brief 最小負荷容量を返す．
CellCapacitance
CellOutputPin::min_capacitance() const
{
  return mMinCapacitance;
}
  
// @brief 最大遷移時間を返す．
CellTime
CellOutputPin::max_transition() const
{
  return mMaxTransition;
}

// @brief 最小遷移時間を返す．
CellTime
CellOutputPin::min_transition() const
{
  return mMinTransition;
}


//////////////////////////////////////////////////////////////////////
// クラス CellInoutPin
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] name 名前
CellInoutPin::CellInoutPin(const ShString& name) :
  CellOutputPin(name)
{
}

// @brief デストラクタ
CellInoutPin::~CellInoutPin()
{
}

// @brief 方向を返す．
tCellDirection
CellInoutPin::direction() const
{
  return kDirInout;
}

// @brief 負荷容量を返す．
CellCapacitance
CellInoutPin::capacitance() const
{
  return mCapacitance;
}

// @brief 立ち上がり時の負荷容量を返す．
CellCapacitance
CellInoutPin::rise_capacitance() const
{
  return mRiseCapacitance;
}

// @brief 立ち下がり時の負荷容量を返す．
CellCapacitance
CellInoutPin::fall_capacitance() const
{
  return mFallCapacitance;
}

END_NAMESPACE_YM_CELL
