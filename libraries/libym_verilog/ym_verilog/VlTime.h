#ifndef YM_VERILOG_VLTIME_H
#define YM_VERILOG_VLTIME_H

/// @file ym_verilog/VlTime.h
/// @brief VlTime のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: VlTime.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_verilog/verilog.h"


BEGIN_NAMESPACE_YM_VERILOG


//////////////////////////////////////////////////////////////////////
/// @class VlTime VlTime.h <ym_verilog/VlTime.h>
/// @brief シミュレーション時刻を表すクラス
//////////////////////////////////////////////////////////////////////
class VlTime
{
public:

  /// @brief 空のコンストラクタ
  /// @note 値は不定
  VlTime();
  
  /// @brief 2つの値を指定するコンストラクタ
  /// @param[in] l 下位32ビットの値
  /// @param[in] h 上位32ビットの値
  VlTime(PLI_UINT32 l,
	 PLI_UINT32 h);

  /// @brief unsigned int からの変換コンストラクタ
  /// @param[in] val 値
  explicit
  VlTime(unsigned int val);

  /// @brief double からの変換コンストラクタ
  /// @param[in] val 値
  explicit
  VlTime(double val);

  /// @brief コピーコンストラクタ
  VlTime(const VlTime& src);

  /// @brief 代入演算子
  const VlTime&
  operator=(const VlTime& src);

  /// @brief 加算つき代入演算子
  const VlTime&
  operator+=(const VlTime& src);

  /// @brief デストラクタ
  ~VlTime();


public:
  //////////////////////////////////////////////////////////////////////
  // 値を設定する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 2つの値を指定する．
  /// @param[in] l 下位32ビットの値
  /// @param[in] h 上位32ビットの値
  void
  set(PLI_UINT32 l,
      PLI_UINT32 h);

  /// @brief unsigned int の値を設定する．
  void
  set(unsigned int val);

  /// @brief double の値を設定する．
  void
  set(double val);


public:
  //////////////////////////////////////////////////////////////////////
  // 値を取り出す関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 下位32ビットの値を取り出す．
  PLI_UINT32
  low() const;

  /// @brief 上位32ビットの値を取り出す．
  PLI_UINT32
  high() const;

  /// @brief unsigned int への変換
  /// @note unsigned int に入りきらない値の時はおかしな値になる．
  unsigned int
  to_uint() const;

  /// @brief 論理値への変換
  /// @retval kVpiScalar0 値が 0
  /// @retval kVpiScalar1 値が 0 以外
  tVpiScalarVal
  to_logic() const;

  /// @brief double への変換
  double
  to_real() const;


public:
  //////////////////////////////////////////////////////////////////////
  // その他の関数
  //////////////////////////////////////////////////////////////////////

  // ハッシュ関数
  ymuint32
  hash() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 下位32ビットの値
  PLI_UINT32 mLow;

  // 上位32ビットの値
  PLI_UINT32 mHigh;

};


//////////////////////////////////////////////////////////////////////
/// @name VlTime に関連する関数
/// @{

/// @brief 加算
/// @param[in] op1, op2 オペランド
/// @return op1 + op2
VlTime
operator+(const VlTime& op1,
	  const VlTime& op2);

/// @brief 等価比較
/// @param[in] op1, op2 オペランド
/// @return op1 と op2 が等しいとき true を返す．
bool
operator==(const VlTime& op1,
	   const VlTime& op2);

/// @brief 小なり
/// @param[in] op1, op2 オペランド
/// @return op1 < op2 のとき true を返す．
bool
operator<(const VlTime& op1,
	  const VlTime& op2);

/// @brief 小なりまたは等しい
/// @param[in] op1, op2 オペランド
/// @return op1 <= op2 のとき true を返す．
bool
operator<=(const VlTime& op1,
	   const VlTime& op2);

/// @brief 大なり
/// @param[in] op1, op2 オペランド
/// @return op1 > op2 のとき true を返す．
bool
operator>(const VlTime& op1,
	  const VlTime& op2);

/// @brief 大なりまたは等しい
/// @param[in] op1, op2 オペランド
/// @return op1 >= op2 のとき true を返す．
bool
operator>=(const VlTime& op1,
	   const VlTime& op2);

/// @}
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 空のコンストラクタ
// @note 値は不定
inline
VlTime::VlTime() :
  mLow(0),
  mHigh(0)
{
}
  
// @brief 2つの値を指定するコンストラクタ
// @param[in] l 下位32ビットの値
// @param[in] h 上位32ビットの値
inline
VlTime::VlTime(PLI_UINT32 l,
	       PLI_UINT32 h) :
  mLow(l),
  mHigh(h)
{
}

// @brief unsigned int からの変換コンストラクタ
inline
VlTime::VlTime(unsigned int val) :
  mLow(val),
  mHigh(0)
{
}

// @brief double からの変換コンストラクタ
inline
VlTime::VlTime(double val)
{
  set(val);
}

// @brief コピーコンストラクタ
inline
VlTime::VlTime(const VlTime& src) :
  mLow(src.mLow),
  mHigh(src.mHigh)
{
} 

// @brief 代入演算子
inline
const VlTime&
VlTime::operator=(const VlTime& src)
{
  mLow = src.mLow;
  mHigh = src.mHigh;
  return *this;
}

// @brief デストラクタ
inline
VlTime::~VlTime()
{
}

// @brief 2つの値を指定する．
// @param[in] l 下位32ビットの値
// @param[in] h 上位32ビットの値
inline
void
VlTime::set(PLI_UINT32 l,
	    PLI_UINT32 h)
{
  mLow = l;
  mHigh = h;
}

// @brief unsigned int の値を設定する．
inline
void
VlTime::set(unsigned int val)
{
  mLow = val;
  mHigh = 0;
}

// @brief double の値を設定する．
inline
void
VlTime::set(double val)
{
  if ( val < 0.0 ) {
    mLow = 0;
    mHigh = 0;
    return;
  }

  double hd = rint(val / 4294967296.0);
  if ( hd >= 4294967296.0 ) {
    // オーバーフロー
    mLow = 0;
    mHigh = 0;
    return;
  }

  mHigh = static_cast<PLI_UINT32>(hd);
  val -= hd;
  mLow = static_cast<PLI_UINT32>(rint(val));
}

// @brief 下位32ビットの値を取り出す．
inline
PLI_UINT32
VlTime::low() const
{
  return mLow;
}

// @brief 上位32ビットの値を取り出す．
inline
PLI_UINT32
VlTime::high() const
{
  return mHigh;
}

// @brief unsigned int への変換
// @note unsigned int に入りきらない値の時はおかしな値になる．
inline
unsigned int
VlTime::to_uint() const
{
  return mLow;
}

// @brief 論理値への変換
// @retval kVpiScalar0 値が 0
// @retval kVpiScalar1 値が 0 以外
inline
tVpiScalarVal
VlTime::to_logic() const
{
  if ( mHigh != 0 || mLow != 0 ) {
    return kVpiScalar1;
  }
  else {
    return kVpiScalar0;
  }
}

// @brief double への変換
inline
double
VlTime::to_real() const
{
  return static_cast<double>(mHigh) * 4294967296.0 +
    static_cast<double>(mLow);
}

// ハッシュ関数
inline
ymuint32
VlTime::hash() const
{
  return (mHigh + 1) * (mLow + 3);
}

// 加算付き代入
inline
const VlTime&
VlTime::operator+=(const VlTime& op2)
{
  // 桁あげを考慮しなければならないのでちょっとめんどくさい
  mLow += op2.mLow;
  ymuint32 carry = 0;
  if ( mLow < op2.mLow ) {
    // 桁あげが起った．
    carry = 1;
  }
  mHigh += op2.mHigh + carry;
  return *this;
}

// 加算
inline
VlTime
operator+(const VlTime& op1,
	  const VlTime& op2)
{
  return VlTime(op1).operator+=(op2);
}

// 等価比較
inline
bool
operator==(const VlTime& op1,
	   const VlTime& op2)
{
  if ( op1.high() == op2.high() &&
       op1.low() == op2.low() ) {
    return true;
  }
  else {
    return false;
  }
}

// 小なり
inline
bool
operator<(const VlTime& op1,
	  const VlTime& op2)
{
  if ( op1.high() < op2.high() ) {
    return true;
  }
  if ( op1.high() > op2.high() ) {
    return false;
  }
  return op1.low() < op2.low();
}

// 小なりまたは等しい
inline
bool
operator<=(const VlTime& op1,
	   const VlTime& op2)
{
  return !(op2 < op1);
}

// 大なり
inline
bool
operator>(const VlTime& op1,
	  const VlTime& op2)
{
  return op2 < op1;
}

// 大なりまたは等しい
inline
bool
operator>=(const VlTime& op1,
	   const VlTime& op2)
{
  return !(op1 < op2);
}

END_NAMESPACE_YM_VERILOG

BEGIN_NAMESPACE_HASH

// VlTimeをキーにしたハッシュ関数クラスの定義
template <>
struct hash<nsYm::nsVerilog::VlTime>
{
  ymuint32
  operator()(const nsYm::nsVerilog::VlTime& time) const
  {
    return time.hash();
  }
};

END_NAMESPACE_HASH

#endif // YM_VERILOG_VLTIME_H
