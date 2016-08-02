#ifndef YMMATH_H
#define YMMATH_H

/// @file ymmath.h
/// @brief 数値関係の定義ファイル
/// @author Yusuke Matsunaga (松永 裕介)
/// 
/// $Id: ymmath.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2009 Yusuke Matsunaga
/// All rights reserved.

#include <ymnsdef.h>


BEGIN_NAMESPACE_YM

/// @brief 2つの数値が誤差を考慮して等しいか判断する．
inline
bool
is_similar(float a,
	   float b)
{
  float delta = fabsf(a - b);
  return delta <= (FLT_EPSILON * a);
}

/// @brief 2つの数値が誤差を考慮して等しいか判断する．
inline
bool
is_similar(double a,
	   double b)
{
  double delta = fabs(a - b);
  return delta <= (DBL_EPSILON * a);
}

/// @brief 2つの数値が誤差を考慮して等しいか判断する．
inline
bool
is_similar(long double a,
	   long double b)
{
  long double delta = fabsl(a - b);
  return delta <= (LDBL_EPSILON * a);
}

END_NAMESPACE_YM

#endif // YMMATH_H
