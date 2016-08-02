#ifndef LIBYM_CELL_CI_CITIMING_H
#define LIBYM_CELL_CI_CITIMING_H

/// @file　libym_cell/CiTiming.h
/// @brief CellTiming の実装クラスのヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: CiTiming.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_cell/CellTiming.h"


BEGIN_NAMESPACE_YM_CELL

//////////////////////////////////////////////////////////////////////
/// @class CiTiming CiTiming.h "CiTiming.h"
/// @brief 共通の基底クラス
//////////////////////////////////////////////////////////////////////
class CiTiming :
  public CellTiming
{
protected:

  /// @brief コンストラクタ
  /// @param[in] id ID番号
  /// @param[in] type タイミング条件の型
  CiTiming(ymuint id,
	   tCellTimingType type);

  /// @brief デストラクタ
  ~CiTiming();


public:
  //////////////////////////////////////////////////////////////////////
  // 共通の属性
  //////////////////////////////////////////////////////////////////////

  /// @brief ID番号を返す．
  virtual
  ymuint
  id() const;

  /// @brief 型の取得
  virtual
  tCellTimingType
  type() const;


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

  /// @brief 立ち上がり？？？
  virtual
  CellTime
  rise_delay_intercept() const;

  /// @brief 立ち下がり？？？
  virtual
  CellTime
  fall_delay_intercept() const;


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

  // ID
  ymuint32 mId;

  // 型
  tCellTimingType mType;

};


//////////////////////////////////////////////////////////////////////
/// @class CiTiming CiTiming.h "CiTiming.h"
/// @brief CMOSジェネリック/折れ線近似タイプ共通の CellTiming の実装クラス
//////////////////////////////////////////////////////////////////////
class CiTimingGP :
  public CiTiming
{
protected:

  /// @brief コンストラクタ
  /// @param[in] id ID番号
  /// @param[in] timing_type タイミングの型
  /// @param[in] intrinsic_rise 立ち上がり固有遅延
  /// @param[in] intrinsic_fall 立ち下がり固有遅延
  /// @param[in] slope_rise 立ち上がりスロープ遅延
  /// @param[in] slope_fall 立ち下がりスロープ遅延
  CiTimingGP(ymuint id,
	     tCellTimingType timing_type,
	     CellTime intrinsic_rise,
	     CellTime intrinsic_fall,
	     CellTime slope_rise,
	     CellTime slope_fall);

  /// @brief デストラクタ
  virtual
  ~CiTimingGP();


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


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 立ち上がり固有遅延
  CellTime mIntrinsicRise;

  // 立ち下がり固有遅延
  CellTime mIntrinsicFall;

  // 立ち上がりスロープ遅延
  CellTime mSlopeRise;

  // 立ち下がりスロープ遅延
  CellTime mSlopeFall;

};


//////////////////////////////////////////////////////////////////////
/// @class CiTimingGeneric CiTiming.h "CiTiming.h"
/// @brief CMOSジェネリックタイプの CellTiming の実装クラス
//////////////////////////////////////////////////////////////////////
class CiTimingGeneric :
  public CiTimingGP
{
  friend class CiLibrary;

private:

  /// @brief コンストラクタ
  /// @param[in] id ID番号
  /// @param[in] timing_type タイミングの型
  /// @param[in] intrinsic_rise 立ち上がり固有遅延
  /// @param[in] intrinsic_fall 立ち下がり固有遅延
  /// @param[in] slope_rise 立ち上がりスロープ遅延
  /// @param[in] slope_fall 立ち下がりスロープ遅延
  /// @param[in] rise_resistance 立ち上がり遷移遅延パラメータ
  /// @param[in] fall_resistance 立ち下がり遷移遅延パラメータ
  CiTimingGeneric(ymuint id,
		  tCellTimingType timing_type,
		  CellTime intrinsic_rise,
		  CellTime intrinsic_fall,
		  CellTime slope_rise,
		  CellTime slope_fall,
		  CellResistance rise_resistance,
		  CellResistance fall_resistance);

  /// @brief デストラクタ
  virtual
  ~CiTimingGeneric();


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


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 立ち上がり遷移遅延
  CellResistance mRiseResistance;

  // 立ち下がり遷移遅延
  CellResistance mFallResistance;

};


//////////////////////////////////////////////////////////////////////
/// @class CiTimingPiecewise CiTiming.h "CiTiming.h"
/// @brief CMOS折れ線近似タイプの CellTiming の実装クラス
//////////////////////////////////////////////////////////////////////
class CiTimingPiecewise :
  public CiTimingGP
{
  friend class CiLibrary;

private:

  /// @brief コンストラクタ
  /// @param[in] id ID番号
  /// @param[in] timing_type タイミングの型
  /// @param[in] intrinsic_rise 立ち上がり固有遅延
  /// @param[in] intrinsic_fall 立ち下がり固有遅延
  /// @param[in] slope_rise 立ち上がりスロープ遅延
  /// @param[in] slope_fall 立ち下がりスロープ遅延
  CiTimingPiecewise(ymuint id,
		    tCellTimingType timing_type,
		    CellTime intrinsic_rise,
		    CellTime intrinsic_fall,
		    CellTime slope_rise,
		    CellTime slope_fall,
		    CellResistance rise_pin_resistance,
		    CellResistance fall_pin_resistance);

  /// @brief デストラクタ
  virtual
  ~CiTimingPiecewise();


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

  /// @brief 立ち上がり？？？
  virtual
  CellTime
  rise_delay_intercept() const;

  /// @brief 立ち下がり？？？
  virtual
  CellTime
  fall_delay_intercept() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 立ち上がり遷移遅延
  CellResistance mRisePinResistance;

  // 立ち下がり遷移遅延
  CellResistance mFallPinResistance;

};


//////////////////////////////////////////////////////////////////////
/// @class CiTimingNonlinear1 CiTiming.h "CiTiming.h"
/// @brief CMOS非線形タイプの CellTiming の実装クラス
//////////////////////////////////////////////////////////////////////
class CiTimingNonlinear1 :
  public CiTiming
{
  friend class CiLibrary;

private:

  /// @brief コンストラクタ
  /// @param[in] id ID番号
  /// @param[in] timing_type タイミングの型
  /// @param[in] cell_rise 立ち上がりセル遅延テーブル
  /// @param[in] cell_fall 立ち下がりセル遅延テーブル
  CiTimingNonlinear1(ymuint id,
		     tCellTimingType timing_type,
		     CellLut* cell_rise,
		     CellLut* cell_fall);


  /// @brief デストラクタ
  virtual
  ~CiTimingNonlinear1();


public:
  //////////////////////////////////////////////////////////////////////
  // CMOS非線形遅延モデルの属性
  //////////////////////////////////////////////////////////////////////

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

  // 立ち上がりセル遅延テーブル
  const CellLut* mCellRise;

  // 立ち下がりセル遅延テーブル
  const CellLut* mCellFall;

};


//////////////////////////////////////////////////////////////////////
/// @class CiTimingNonlinear2 CiTiming.h "CiTiming.h"
/// @brief CMOS非線形タイプの CellTiming の実装クラス
//////////////////////////////////////////////////////////////////////
class CiTimingNonlinear2 :
  public CiTiming
{
  friend class CiLibrary;

private:

  /// @brief コンストラクタ
  /// @param[in] id ID番号
  /// @param[in] timing_type タイミングの型
  /// @param[in] rise_transition 立ち上がり遷移遅延テーブル
  /// @param[in] fall_transition 立ち下がり遷移遅延テーブル
  /// @param[in] rise_propagation 立ち上がり伝搬遅延テーブル
  /// @param[in] fall_propagation 立ち下がり伝搬遅延テーブル
  CiTimingNonlinear2(ymuint id,
		     tCellTimingType timing_type,
		     CellLut* rise_transition,
		     CellLut* fall_transition,
		     CellLut* rise_propagation,
		     CellLut* fall_propagation);


  /// @brief デストラクタ
  virtual
  ~CiTimingNonlinear2();


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


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 立ち上がり遷移遅延テーブル
  const CellLut* mRiseTransition;

  // 立ち下がり遷移遅延テーブル
  const CellLut* mFallTransition;

  // 立ち上がり伝搬遅延テーブル
  const CellLut* mRisePropagation;

  // 立ち下がり伝搬遅延テーブル
  const CellLut* mFallPropagation;

};

END_NAMESPACE_YM_CELL

#endif // LIBYM_CELL_CI_CITIMING_H
