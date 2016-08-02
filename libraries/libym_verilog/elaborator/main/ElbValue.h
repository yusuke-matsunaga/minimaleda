#ifndef LIBYM_VERILOG_ELABORATOR_MAIN_ELBVALUE_H
#define LIBYM_VERILOG_ELABORATOR_MAIN_ELBVALUE_H

/// @file libym_verilog/elaborator/main/ElbValue.h
/// @brief ElbValue のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_verilog/verilog.h"
#include "ym_verilog/BitVector.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class ElbValue ElbValue.h "ElbValue.h"
/// @brief 値を表すクラス
//////////////////////////////////////////////////////////////////////
class ElbValue
{
public:

  /// @brief 空のコンストラクタ
  /// @note 中身は不定
  ElbValue();

  /// @brief 整数値からの変換コンストラクタ
  /// @param[in] value 値
  ElbValue(int value);

  /// @brief スカラー値からの変換コンストラクタ
  /// @param[in] value 値
  ElbValue(tVpiScalarVal value);

  /// @brief ビットベクタからの変換コンストラクタ
  /// @param[in] value 値
  ElbValue(const BitVector& value);

  /// @brief 実数値からの変換コンストラクタ
  /// @param[in] value 値
  ElbValue(double value);

  /// @brief デストラクタ
  ~ElbValue();


public:

  /// @brief 値を整数に変換する．
  void
  to_int();

  /// @brief 値をスカラー値に変換する．
  void
  to_scalar();

  /// @brief 値を論理値に変換する．
  void
  to_logic();

  /// @brief 値をビットベクタに変換する．
  void
  to_bitvector();

  /// @brief 値を実数に変換する．
  void
  to_real();


public:

  /// @brief どの値も表していないエラーの場合に true を返す．
  bool
  is_error() const;

  /// @brief 値が整数値の場合に true を返す．
  bool
  is_int() const;

  /// @brief 値がスカラー値の場合に true を返す．
  bool
  is_scalar() const;

  /// @brief 値がビットベクタの場合に true を返す．
  bool
  is_bitvector() const;

  /// @brief 値が実数値の場合に true を返す．
  bool
  is_real() const;

  /// @brief 整数値を返す．
  int
  int_value() const;

  /// @brief スカラー値を返す．
  tVpiScalarVal
  scalar_value() const;

  /// @brief ビットベクタ値を返す．
  const BitVector&
  bitvector_value() const;

  /// @brief 実数値を返す．
  double
  real_value() const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief int => bitvector の変換を行う．
  void
  int2bitvector();

  /// @brief int => real の変換を行う．
  void
  int2real();

  /// @brief int => scalar の変換を行う．
  void
  int2scalar();

  /// @brief scalar => int の変換を行う．
  void
  scalar2int();

  /// @brief scalar => bool の変換を行う．
  void
  scalar2bool();

  /// @brief scalar => bitvector の変換を行う．
  void
  scalar2bitvector();

  /// @brief bitvector => int の変換を行う．
  void
  bitvector2int();

  /// @brief bitvector => scalar の変換を行う．
  void
  bitvector2scalar();

  /// @brief bitvector => real の変換を行う．
  void
  bitvector2real();

  /// @brief real => logic の変換を行う．
  void
  real2logic();


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いる定数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内部状態を表す列挙型
  enum tState {
    /// @brief エラー状態
    kError,
    /// @brief 整数値
    kInt,
    /// @brief スカラー値
    kScalar,
    /// @brief ビットベクタ
    kBitVector,
    /// @brief 実数値
    kReal
  };


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 内部状態
  tState mState;

  // 整数の場合の値
  int mIntValue;

  // スカラーの場合の値
  tVpiScalarVal mScalarValue;

  // ビットベクタの場合の値
  BitVector mBitVectorValue;

  // 実数の場合の値
  double mRealValue;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 空のコンストラクタ
// @note 中身は不定
inline
ElbValue::ElbValue() :
  mState(kError)
{
}

// @brief 整数値からの変換コンストラクタ
// @param[in] value 値
inline
ElbValue::ElbValue(int value) :
  mState(kInt),
  mIntValue(value)
{
}

// @brief スカラー値からの変換コンストラクタ
// @param[in] value 値
inline
ElbValue::ElbValue(tVpiScalarVal value) :
  mState(kScalar),
  mScalarValue(value)
{
}

// @brief ビットベクタからの変換コンストラクタ
// @param[in] value 値
inline
ElbValue::ElbValue(const BitVector& value) :
  mState(kBitVector),
  mBitVectorValue(value)
{
}

// @brief 実数値からの変換コンストラクタ
// @param[in] value 値
inline
ElbValue::ElbValue(double value) :
  mState(kReal),
  mRealValue(value)
{
}

// @brief デストラクタ
inline
ElbValue::~ElbValue()
{
}

// @brief 値を整数に変換する．
inline
void
ElbValue::to_int()
{
  if ( is_scalar() ) {
    scalar2int();
  }
  else if ( is_bitvector() ) {
    bitvector2int();
  }
  else if ( !is_int() ) {
    mState = kError;
  }
}

// @brief 値をスカラー値に変換する．
inline
void
ElbValue::to_scalar()
{
  if ( is_int() ) {
    int2scalar();
  }
  else if ( is_bitvector() ) {
    bitvector2scalar();
  }
  else if ( !is_scalar() ) {
    mState = kError;
  }
}

// @brief 値を論理値に変換する．
inline
void
ElbValue::to_logic()
{
  if ( is_int() ) {
    int2scalar();
    scalar2bool();
  }
  else if ( is_scalar() ) {
    scalar2bool();
  }
  else if ( is_bitvector() ) {
    bitvector2scalar();
    scalar2bool();
  }
  else if ( is_real() ) {
    real2logic();
  }
}

// @brief 値をビットベクタに変換する．
inline
void
ElbValue::to_bitvector()
{
  if ( is_int() ) {
    int2bitvector();
  }
  else if ( is_scalar() ) {
    scalar2bitvector();
  }
  else if ( !is_bitvector() ) {
    mState = kError;
  }
}

// @brief 値を実数に変換する．
inline
void
ElbValue::to_real()
{
  if ( is_int() ) {
    int2real();
  }
  else if ( is_scalar() ) {
    scalar2int();
    int2real();
  }
  else if ( is_bitvector() ) {
    bitvector2real();
  }
}

// @brief どの値も表していないエラーの場合に true を返す．
inline
bool
ElbValue::is_error() const
{
  return mState == kError;
}

// @brief 値が整数値の場合に true を返す．
inline
bool
ElbValue::is_int() const
{
  return mState == kInt;
}

// @brief 値がスカラー値の場合に true を返す．
inline
bool
ElbValue::is_scalar() const
{
  return mState == kScalar;
}

// @brief 値がビットベクタの場合に true を返す．
inline
bool
ElbValue::is_bitvector() const
{
  return mState == kBitVector;
}

// @brief 値が実数値の場合に true を返す．
inline
bool
ElbValue::is_real() const
{
  return mState == kReal;
}

// @brief 整数値を返す．
inline
int
ElbValue::int_value() const
{
  return mIntValue;
}

// @brief スカラー値を返す．
inline
tVpiScalarVal
ElbValue::scalar_value() const
{
  return mScalarValue;
}

// @brief ビットベクタ値を返す．
inline
const BitVector&
ElbValue::bitvector_value() const
{
  return mBitVectorValue;
}

// @brief 実数値を返す．
inline
double
ElbValue::real_value() const
{
  return mRealValue;
}

// @brief int => bitvector の変換を行う．
inline
void
ElbValue::int2bitvector()
{
  mBitVectorValue = BitVector(int_value());
  mState = kBitVector;
}

// @brief int => real の変換を行う．
inline
void
ElbValue::int2real()
{
  mRealValue = int_value();
  mState = kReal;
}

// @brief int => scalar の変換を行う．
inline
void
ElbValue::int2scalar()
{
  if ( int_value() & 1 ) {
    mScalarValue = kVpiScalar1;
  }
  else {
    mScalarValue = kVpiScalar0;
  }
  mState = kScalar;
}

// @brief scalar => int の変換を行う．
inline
void
ElbValue::scalar2int()
{
  mIntValue = conv_to_int(scalar_value());
  mState = kInt;
}

// @brief scalar => bool の変換を行う．
inline
void
ElbValue::scalar2bool()
{
  if ( conv_to_bool(scalar_value()) ) {
    mScalarValue = kVpiScalar1;
  }
  else {
    mScalarValue = kVpiScalar0;
  }
}

// @brief scalar => bitvector の変換を行う．
inline
void
ElbValue::scalar2bitvector()
{
  mBitVectorValue = BitVector(scalar_value());
  mState = kBitVector;
}

// @brief bitvector => int の変換を行う．
inline
void
ElbValue::bitvector2int()
{
  mIntValue = bitvector_value().to_int();
  mState = kInt;
}

// @brief bitvector => scalar の変換を行う．
inline
void
ElbValue::bitvector2scalar()
{
  mScalarValue = bitvector_value().bit_select(0);
  mState = kScalar;
}

// @brief bitvector => real の変換を行う．
inline
void
ElbValue::bitvector2real()
{
  mRealValue = bitvector_value().to_real();
}

// @brief real => logic の変換を行う．
inline
void
ElbValue::real2logic()
{
  mScalarValue = conv_to_scalar(real_value());
  mState = kScalar;
}

END_NAMESPACE_YM_VERILOG

#endif // LIBYM_VERILOG_ELABORATOR_MAIN_ELBVALUE_H
