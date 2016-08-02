#ifndef YM_CELL_CELL_TYPE_H
#define YM_CELL_CELL_TYPE_H

/// @file　ym_cell/cell_type.h
/// @brief さまざまな型定義
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: cell_type.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_cell/cell_nsdef.h"


BEGIN_NAMESPACE_YM_CELL

//////////////////////////////////////////////////////////////////////
/// @brief テクノロジを表す列挙型
//////////////////////////////////////////////////////////////////////
enum tCellTechnology {
  kTechAsic,
  kTechFpga
};


//////////////////////////////////////////////////////////////////////
/// @brief 遅延モデルを表す列挙型
//////////////////////////////////////////////////////////////////////
enum tCellDelayModel {
  kDelayGeneric,
  kDelayPiecewise,
  kDelayNonlinear
};


//////////////////////////////////////////////////////////////////////
/// @brief ピンの方向を表す列挙型
//////////////////////////////////////////////////////////////////////
enum tCellDirection {
  kDirInput,
  kDirOutput,
  kDirInout
};


//////////////////////////////////////////////////////////////////////
/// @brief タイミングの条件を表す列挙型
//////////////////////////////////////////////////////////////////////
enum tCellTimingSense {
  kSensePosiUnate = 1,
  kSenseNegaUnate = 2,
  kSenseNonUnate  = 3
};


//////////////////////////////////////////////////////////////////////
/// @brief タイミング情報の型を表す列挙型
//////////////////////////////////////////////////////////////////////
enum tCellTimingType {
  kTimingCombinational          = 0,
  kTimingCombinationalRise      = 1,
  kTimingCombinationalFall      = 2,

  kTimingThreeStateEnable       = 3,
  kTimingThreeStateDisable      = 4,
  kTimingThreeStateEnableRise   = 5,
  kTimingThreeStateEnableFall   = 6,
  kTimingThreeStateDisableRise  = 7,
  kTimingThreeStateDisableFall  = 8,

  kTimingRisingEdge             = 9,
  ktimingFallingEdge            = 10,

  kTimingPreset                 = 11,
  kTimingClear                  = 12,

  kTimingHoldRising             = 13,
  kTimingHaldFalling            = 14,

  kTimingSetupRising            = 15,
  kTimingSetupFalling           = 16,

  kTimingRecoveryRising         = 17,
  kTimingRecoveryFalling        = 18,

  kTimingSkewRising             = 19,
  kTimingSkewFalling            = 20,

  kTimingRemovalRising          = 21,
  kTimingRemovalFalling         = 22,

  kTimingNonSeqSetupRising      = 23,
  kTimingNonSeqSetupFalling     = 24,
  kTimingNonSeqHoldRising       = 25,
  kTimingNonSeqHoldFalling      = 26,

  kTimingNochangeHighHigh       = 27,
  kTimingNochangeHighLow        = 28,
  kTimingNochangeLowHigh        = 29,
  kTimingNochangeLowLow         = 30

};


//////////////////////////////////////////////////////////////////////
/// @calss CellArea cell_type.h <ym_cell/cell_type.h>
/// @brief 面積を表すクラス
//////////////////////////////////////////////////////////////////////
class CellArea
{
public:
  //////////////////////////////////////////////////////////////////////
  // コンストラクタ/デストラクタ
  //////////////////////////////////////////////////////////////////////

  /// @brief 空のコンストラクタ
  CellArea();

  /// @brief double からの変換コンストラクタ
  explicit
  CellArea(double v);

  /// @brief デストラクタ
  ~CellArea();


public:
  //////////////////////////////////////////////////////////////////////
  // 値を作るクラスメソッド
  //////////////////////////////////////////////////////////////////////

  /// @brief 無限大の値を作る．
  static
  CellArea
  infty();


public:
  //////////////////////////////////////////////////////////////////////
  // 値を取得する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 値を得る．
  double
  value() const;


public:
  //////////////////////////////////////////////////////////////////////
  // 代入演算子
  //////////////////////////////////////////////////////////////////////

  /// @brief 代入演算子
  const CellArea&
  operator=(const CellArea& src);

  /// @brief 加算付き代入演算子
  const CellArea&
  operator+=(const CellArea& src);

  /// @brief 減算付き代入演算子
  const CellArea&
  operator-=(const CellArea& src);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 値
  double mValue;

};

/// @brief 加算
CellArea
operator+(const CellArea& left,
	  const CellArea& right);

/// @brief 減算
CellArea
operator-(const CellArea& left,
	  const CellArea& right);

/// @brief ストリーム出力
ostream&
operator<<(ostream& s,
	   const CellArea& val);


//////////////////////////////////////////////////////////////////////
/// @class CellTime cell_type.h <ym_cell/cell_type.h>
/// @brief 時間を表すクラス
//////////////////////////////////////////////////////////////////////
class CellTime
{
public:
  //////////////////////////////////////////////////////////////////////
  // コンストラクタ/デストラクタ
  //////////////////////////////////////////////////////////////////////

  /// @brief 空のコンストラクタ
  CellTime();

  /// @brief double からの変換用コンストラクタ
  explicit
  CellTime(double v);

  /// @brief デストラクタ
  ~CellTime();


public:
  //////////////////////////////////////////////////////////////////////
  // 値を作るクラスメソッド
  //////////////////////////////////////////////////////////////////////

  /// @brief 無限大の値を作る．
  static
  CellTime
  infty();


public:
  //////////////////////////////////////////////////////////////////////
  // 値を取得する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 値を得る．
  double
  value() const;


public:
  //////////////////////////////////////////////////////////////////////
  // 代入演算子
  //////////////////////////////////////////////////////////////////////

  /// @brief 代入演算子
  const CellTime&
  operator=(const CellTime& src);

  /// @brief 加算付き代入演算子
  const CellTime&
  operator+=(const CellTime& src);

  /// @brief 減算付き代入演算子
  const CellTime&
  operator-=(const CellTime& src);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 値
  double mValue;

};

/// @brief 加算
CellTime
operator+(const CellTime& left,
	  const CellTime& right);

/// @brief 減算
CellTime
operator-(const CellTime& left,
	  const CellTime& right);

/// @brief ストリーム出力
ostream&
operator<<(ostream& s,
	   const CellTime& val);


//////////////////////////////////////////////////////////////////////
/// @class CellCapacitance cell_type.h <ym_cell/cell_type.h>
/// @brief 容量値を表すクラス
//////////////////////////////////////////////////////////////////////
class CellCapacitance
{
public:
  //////////////////////////////////////////////////////////////////////
  // コンストラクタ/デストラクタ
  //////////////////////////////////////////////////////////////////////

  /// @brief 空のコンストラクタ
  CellCapacitance();

  /// @brief double からの変換コンストラクタ
  explicit
  CellCapacitance(double v);

  /// @brief デストラクタ
  ~CellCapacitance();


public:
  //////////////////////////////////////////////////////////////////////
  // 値を作るクラスメソッド
  //////////////////////////////////////////////////////////////////////

  /// @brief 無限大の値を作る．
  static
  CellCapacitance
  infty();


public:
  //////////////////////////////////////////////////////////////////////
  // 値を取得する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 値を得る．
  double
  value() const;


public:
  //////////////////////////////////////////////////////////////////////
  // 代入演算子
  //////////////////////////////////////////////////////////////////////

  /// @brief 代入演算子
  const CellCapacitance&
  operator=(const CellCapacitance& src);

  /// @brief 加算付き代入演算子
  const CellCapacitance&
  operator+=(const CellCapacitance& src);

  /// @brief 減算付き代入演算子
  const CellCapacitance&
  operator-=(const CellCapacitance& src);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 値
  double mValue;

};

/// @brief 加算
CellCapacitance
operator+(const CellCapacitance& left,
	  const CellCapacitance& right);

/// @brief 減算
CellCapacitance
operator-(const CellCapacitance& left,
	  const CellCapacitance& right);

/// @brief ストリーム出力
ostream&
operator<<(ostream& s,
	   const CellCapacitance& val);


//////////////////////////////////////////////////////////////////////
/// @class CellResistance cell_type.h <ym_cell/cell_type.h>
/// @brief 抵抗値を表すクラス
//////////////////////////////////////////////////////////////////////
class CellResistance
{
public:
  //////////////////////////////////////////////////////////////////////
  // コンストラクタ/デストラクタ
  //////////////////////////////////////////////////////////////////////

  /// @brief 空のコンストラクタ
  CellResistance();

  /// @brief double からの変換コンストラクタ
  explicit
  CellResistance(double v);

  /// @brief デストラクタ
  ~CellResistance();


public:
  //////////////////////////////////////////////////////////////////////
  // 値を作るクラスメソッド
  //////////////////////////////////////////////////////////////////////

  /// @brief 無限大の値を作る．
  static
  CellResistance
  infty();


public:
  //////////////////////////////////////////////////////////////////////
  // 値を取得する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 値を得る．
  double
  value() const;


public:
  //////////////////////////////////////////////////////////////////////
  // 代入演算子
  //////////////////////////////////////////////////////////////////////

  /// @brief 代入演算子
  const CellResistance&
  operator=(const CellResistance& src);

  /// @brief 加算付き代入演算子
  const CellResistance&
  operator+=(const CellResistance& src);

  /// @brief 減算付き代入演算子
  const CellResistance&
  operator-=(const CellResistance& src);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 値
  double mValue;

};

/// @brief 加算
CellResistance
operator+(const CellResistance& left,
	  const CellResistance& right);

/// @brief 減算
CellResistance
operator-(const CellResistance& left,
	  const CellResistance& right);

/// @brief ストリーム出力
ostream&
operator<<(ostream& s,
	   const CellResistance& val);


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 空のコンストラクタ
inline
CellArea::CellArea() :
  mValue(0.0)
{
}

// @brief double からの変換コンストラクタ
inline
CellArea::CellArea(double v) :
  mValue(v)
{
}

// @brief デストラクタ
inline
CellArea::~CellArea()
{
}

// @brief 無限大の値を作る．
inline
CellArea
CellArea::infty()
{
  return CellArea(DBL_MAX);
}

// @brief 値を得る．
inline
double
CellArea::value() const
{
  return mValue;
}

// @brief 代入演算子
inline
const CellArea&
CellArea::operator=(const CellArea& src)
{
  mValue = src.mValue;
  return *this;
}

// @brief 加算付き代入演算子
inline
const CellArea&
CellArea::operator+=(const CellArea& src)
{
  mValue += src.mValue;
  return *this;
}

// @brief 減算付き代入演算子
inline
const CellArea&
CellArea::operator-=(const CellArea& src)
{
  mValue -= src.mValue;
  return *this;
}

// @brief 加算
inline
CellArea
operator+(const CellArea& left,
	  const CellArea& right)
{
  return CellArea(left).operator+=(right);
}

// @brief 減算
inline
CellArea
operator-(const CellArea& left,
	  const CellArea& right)
{
  return CellArea(left).operator-=(right);
}

// @brief ストリーム出力
inline
ostream&
operator<<(ostream& s,
	   const CellArea& val)
{
  return s << val.value();
}

// @brief 空のコンストラクタ
inline
CellTime::CellTime() :
  mValue(0.0)
{
}

// @brief double からの変換コンストラクタ
inline
CellTime::CellTime(double v) :
  mValue(v)
{
}

// @brief デストラクタ
inline
CellTime::~CellTime()
{
}

// @brief 無限大の値を作る．
inline
CellTime
CellTime::infty()
{
  return CellTime(DBL_MAX);
}

// @brief 値を得る．
inline
double
CellTime::value() const
{
  return mValue;
}

// @brief 代入演算子
inline
const CellTime&
CellTime::operator=(const CellTime& src)
{
  mValue = src.mValue;
  return *this;
}

// @brief 加算付き代入演算子
inline
const CellTime&
CellTime::operator+=(const CellTime& src)
{
  mValue += src.mValue;
  return *this;
}

// @brief 減算付き代入演算子
inline
const CellTime&
CellTime::operator-=(const CellTime& src)
{
  mValue -= src.mValue;
  return *this;
}

// @brief 加算
inline
CellTime
operator+(const CellTime& left,
	  const CellTime& right)
{
  return CellTime(left).operator+=(right);
}

// @brief 減算
inline
CellTime
operator-(const CellTime& left,
	  const CellTime& right)
{
  return CellTime(left).operator-=(right);
}

// @brief ストリーム出力
inline
ostream&
operator<<(ostream& s,
	   const CellTime& val)
{
  return s << val.value();
}

// @brief 空のコンストラクタ
inline
CellCapacitance::CellCapacitance() :
  mValue(0.0)
{
}

// @brief double からの変換コンストラクタ
inline
CellCapacitance::CellCapacitance(double v) :
  mValue(v)
{
}

// @brief デストラクタ
inline
CellCapacitance::~CellCapacitance()
{
}

// @brief 無限大の値を作る．
inline
CellCapacitance
CellCapacitance::infty()
{
  return CellCapacitance(DBL_MAX);
}

// @brief 値を得る．
inline
double
CellCapacitance::value() const
{
  return mValue;
}

// @brief 代入演算子
inline
const CellCapacitance&
CellCapacitance::operator=(const CellCapacitance& src)
{
  mValue = src.mValue;
  return *this;
}

// @brief 加算付き代入演算子
inline
const CellCapacitance&
CellCapacitance::operator+=(const CellCapacitance& src)
{
  mValue += src.mValue;
  return *this;
}

// @brief 減算付き代入演算子
inline
const CellCapacitance&
CellCapacitance::operator-=(const CellCapacitance& src)
{
  mValue -= src.mValue;
  return *this;
}

// @brief 加算
inline
CellCapacitance
operator+(const CellCapacitance& left,
	  const CellCapacitance& right)
{
  return CellCapacitance(left).operator+=(right);
}

// @brief 減算
inline
CellCapacitance
operator-(const CellCapacitance& left,
	  const CellCapacitance& right)
{
  return CellCapacitance(left).operator-=(right);
}

// @brief ストリーム出力
inline
ostream&
operator<<(ostream& s,
	   const CellCapacitance& val)
{
  return s << val.value();
}

// @brief 空のコンストラクタ
inline
CellResistance::CellResistance() :
  mValue(0.0)
{
}

// @brief double からの変換コンストラクタ
inline
CellResistance::CellResistance(double v) :
  mValue(v)
{
}

// @brief デストラクタ
inline
CellResistance::~CellResistance()
{
}

// @brief 無限大の値を作る．
inline
CellResistance
CellResistance::infty()
{
  return CellResistance(DBL_MAX);
}

// @brief 値を得る．
inline
double
CellResistance::value() const
{
  return mValue;
}

// @brief 代入演算子
inline
const CellResistance&
CellResistance::operator=(const CellResistance& src)
{
  mValue = src.mValue;
  return *this;
}

// @brief 加算付き代入演算子
inline
const CellResistance&
CellResistance::operator+=(const CellResistance& src)
{
  mValue += src.mValue;
  return *this;
}

// @brief 減算付き代入演算子
inline
const CellResistance&
CellResistance::operator-=(const CellResistance& src)
{
  mValue -= src.mValue;
  return *this;
}

// @brief 加算
inline
CellResistance
operator+(const CellResistance& left,
	  const CellResistance& right)
{
  return CellResistance(left).operator+=(right);
}

// @brief 減算
inline
CellResistance
operator-(const CellResistance& left,
	  const CellResistance& right)
{
  return CellResistance(left).operator-=(right);
}

// @brief ストリーム出力
inline
ostream&
operator<<(ostream& s,
	   const CellResistance& val)
{
  return s << val.value();
}


END_NAMESPACE_YM_CELL

#endif // YM_CELL_CELL_TYPE_H
