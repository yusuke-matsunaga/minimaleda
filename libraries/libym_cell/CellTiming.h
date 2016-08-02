#ifndef YM_CELL_CELLTIMING_H
#define YM_CELL_CELLTIMING_H

/// @file　ym_cell/CellTiming.h
/// @brief Cell のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: CellTiming.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_cell/cell_nsdef.h"
#include "ym_cell/cell_type.h"


BEGIN_NAMESPACE_YM_CELL

//////////////////////////////////////////////////////////////////////
/// @class CellTiming CellTiming.h <ym_cell/CellTiming.h>
/// @brief タイミング情報を表すクラス
//////////////////////////////////////////////////////////////////////
class CellTiming
{
protected:

  /// @brief コンストラクタ
  /// @param[in] ipin_list 開始ピンのリスト
  /// @param[in] opin 終点ピン
  /// @param[in] timing_sense タイミング条件
  /// @param[in] timing_type タイミングの型
  CellTiming(const vector<const CellPin*>& ipin_list,
	     CellPin* opin,
	     tCellTimingSense timing_sense,
	     tCellTimingType timing_type);

  /// @brief デストラクタ
  ~CellTiming();

  
public:
  //////////////////////////////////////////////////////////////////////
  // 共通の属性
  //////////////////////////////////////////////////////////////////////
  
  /// @brief 関連する開始ピン数の取得
  ymuint32
  n_ipins() const;

  /// @brief 関連する開始ピンの取得
  /// @param[in] pos 位置番号 ( 0 <= pos < n_ipins() )
  const CellPin*
  ipin(ymuint32 pos) const;
  
  /// @brief 関連する終点ピンの取得
  const CellPin*
  opin() const;
  
  /// @brief 条件の取得
  tCellTimingSense
  timing_sense() const;

  /// @brief 型の取得
  tCellTimingType
  timing_type() const;


public:
  //////////////////////////////////////////////////////////////////////
  // CMOSジェネリック/CMOS折れ線近似遅延モデルの属性
  //////////////////////////////////////////////////////////////////////

  /// @brief 立ち上がり固有遅延の取得
  virtual
  CellTime
  intrinsic_rise() const;

  /// @brief 立ち下がり固有遅延の取得
  virtual
  CellTime
  intrinsic_fall() const;

  /// @brief 立ち上がりスロープ遅延の取得
  virtual
  CellTime
  slope_rise() const;

  /// @brief 立ち下がりスロープ遅延の取得
  virtual
  CellTime
  slope_fall() const;


public:
  //////////////////////////////////////////////////////////////////////
  // CMOSジェネリック遅延モデルの属性
  //////////////////////////////////////////////////////////////////////
  
  /// @brief 立ち上がり遷移遅延の取得
  virtual
  CellResistance
  rise_resistance() const;

  /// @brief 立ち下がり遷移遅延の取得
  virtual
  CellResistance
  fall_resistance() const;


public:
  //////////////////////////////////////////////////////////////////////
  // CMOS折れ線近似遅延モデルの属性
  //////////////////////////////////////////////////////////////////////
  
  /// @brief 立ち上がり遷移遅延の取得
  virtual
  CellResistance
  rise_pin_resistance() const;

  /// @brief 立ち下がり遷移遅延の取得
  virtual
  CellResistance
  fall_pin_resistance() const;

#if 0
  /// @brief 立ち上がり？？？
  virtual
  CellTime
  rise_delay_intercept() const;

  /// @brief 立ち下がり？？？
  virtual
  CellTime
  fall_delay_intercept() const;
#endif

  
public:
  //////////////////////////////////////////////////////////////////////
  // CMOS非線形遅延モデルの属性
  //////////////////////////////////////////////////////////////////////

  /// @brief 立ち上がり遷移遅延テーブルの取得
  virtual
  const CellLut*
  rise_transition() const;

  /// @brief 立ち下がり遷移遅延テーブルの取得
  virtual
  const CellLut*
  fall_transition() const;

  /// @brief 立ち上がり伝搬遅延テーブルの取得
  virtual
  const CellLut*
  rise_propagation() const;

  /// @brief 立ち下がり伝搬遅延テーブルの取得
  virtual
  const CellLut*
  fall_propagation() const;
  
  /// @brief 立ち上がりセル遅延テーブルの取得
  virtual
  const CellLut*
  cell_rise() const;

  /// @brief 立ち下がりセル遅延テーブルの取得
  virtual
  const CellLut*
  cell_fall() const;
  

private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////
  
  // 関連する開始ピンのリスト
  vector<const CellPin*> mIpinList;

  // 関連する終点ピン
  const CellPin* mOpin;

  // 条件
  tCellTimingSense mTimingSense;

  // 型
  tCellTimingType mTimingType;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////
  
// @brief 関連する開始ピン数の取得
inline
ymuint32
CellTiming::n_ipins() const
{
  return mIpinList.size();
}

// @brief 関連する開始ピンの取得
// @param[in] pos 位置番号 ( 0 <= pos < n_ipins() )
inline
const CellPin*
CellTiming::ipin(ymuint32 pos) const
{
  return mIpinList[pos];
}

// @brief 関連する終点ピンの取得
inline
const CellPin*
CellTiming::opin() const
{
  return mOpin;
}
  
// @brief 条件の取得
inline
tCellTimingSense
CellTiming::timing_sense() const
{
  return mTimingSense;
}

// @brief 型の取得
inline
tCellTimingType
CellTiming::timing_type() const
{
  return mTimingType;
}

END_NAMESPACE_YM_CELL

#endif // YM_CELL_CELLTIMING_H
