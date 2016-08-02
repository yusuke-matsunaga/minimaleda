#ifndef YM_SAT_BOOL3_H
#define YM_SAT_BOOL3_H

/// @file ym_sat/Bool3.h
/// @brief Bool3 の定義ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: Bool3.h 2203 2009-04-16 05:04:40Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_sat/sat_nsdef.h"


BEGIN_NAMESPACE_YM_SAT

/// @brief 不定値を含む論理値
/// @ingroup SatGroup
enum Bool3 {
  /// @brief false
  kB3False = -1,
  /// @brief true
  kB3True  = 1,
  /// @brief unknown
  kB3X     = 0
};

/// @brief Bool3 の否定演算
/// @ingroup SatGroup
inline
Bool3
operator~(Bool3 val)
{
  // 条件分岐を用いない dirty hack
  return static_cast<Bool3>(-static_cast<int>(val));
}

/// @brief bool からのキャスト演算子
/// @ingroup SatGroup
inline
Bool3
b2B3(bool b)
{
  // 条件分岐を用いない dirty hack
  // true:1 false:0 と仮定している．
  return static_cast<Bool3>((static_cast<int>(b) << 1) - 1);
}

/// @brief Bool3 の内容を出力するストリーム演算子
/// @ingroup SatGroup
inline
ostream&
operator<<(ostream& s,
	   Bool3 val)
{
  switch ( val ) {
  case kB3False: s << "false"; break;
  case kB3True:  s << "true"; break;
  case kB3X:     s << "unknown"; break;
  }
  return s;
}

END_NAMESPACE_YM_SAT

BEGIN_NAMESPACE_YM

using nsSat::Bool3;
using nsSat::kB3True;
using nsSat::kB3False;
using nsSat::kB3X;

END_NAMESPACE_YM

#endif // YM_SAT_SATSOLVER_H
