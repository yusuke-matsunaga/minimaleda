#ifndef LED_GLV_H
#define LED_GLV_H

/// @file led/Glv.h
/// @brief GLを用いた描画用プログラムの共通ヘッダ
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: Glv.h 346 2007-01-12 13:56:03Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ymtools.h"

#include <gtkmm.h>
#include <gtkglmm.h>

#ifdef G_OS_WIN32
#define WIN32_LEAN_AND_MEAN 1
#include <windows.h>
#endif

#include <GL/gl.h>
#include <GL/glu.h>


/// @brief glv 用の名前空間の開始
#define BEGIN_NAMESPACE_YM_GLV \
BEGIN_NAMESPACE_YM \
BEGIN_NAMESPACE(nsGlv)

/// @brief lutmap 用の名前空間の終了
#define END_NAMESPACE_YM_GLV \
END_NAMESPACE(nsGlv) \
END_NAMESPACE_YM


BEGIN_NAMESPACE_YM_GLV


//////////////////////////////////////////////////////////////////////
/// @class Point Glv.h "Glv.h"
/// @brief 「点」を表すクラス
//////////////////////////////////////////////////////////////////////
struct Point
{
  /// @brief デフォルトコンストラクタ
  /// @note (0.0, 0.0) にセットされる．
  Point() :
    mX(0.0),
    mY(0.0)
  {
  }
  
  /// @brief X座標とY座標を指定するコンストラクタ
  /// @param[in] x X座標
  /// @param[in] y Y座標
  Point(GLdouble x,
	GLdouble y) :
    mX(x),
    mY(y)
  {
  }

  /// @brief 移動する．
  /// @param[in] vec 移動量を表すベクタ
  /// @return 移動後の自分自身への定数参照を返す．
  const Point& move(const Point& vec)
  {
    mX += vec.mX;
    mY += vec.mY;
    return *this;
  }
  
  /// @brief X座標
  GLdouble mX;
  /// @brief Y座標
  GLdouble mY;
};


//////////////////////////////////////////////////////////////////////
/// @class DiagPoints Glv.h "Glv.h"
/// @brief 矩形を表す対角線上の2点を表すクラス
//////////////////////////////////////////////////////////////////////
struct DiagPoints
{
  /// @brief デフォルトコンストラクタ
  /// @note (0.0, 0.0) - (0.0, 0.0) にセットされる．
  DiagPoints() :
    mLeft(0.0),
    mUpper(0.0),
    mRight(0.0),
    mLower(0.0)
  {
  }

  /// @brief 対角線上の2点を指定したコンストラクタ
  /// @param[in] left 左端のX座標
  /// @param[in] upper 上端のY座標
  /// @param[in] right 右端のX座標
  /// @param[in] lower 下端のY座標
  DiagPoints(GLdouble left,
	     GLdouble upper,
	     GLdouble right,
	     GLdouble lower) :
    mLeft(left),
    mUpper(upper),
    mRight(right),
    mLower(lower)
  {
  }

  /// @brief 対角線上の2点を指定したコンストラクタ
  /// @param[in] 左上の点の座標
  /// @param[in] 右下の点の座標
  DiagPoints(const Point& upper_left,
	     const Point& lower_right) :
    mLeft(upper_left.mX),
    mUpper(upper_left.mY),
    mRight(lower_right.mX),
    mLower(lower_right.mY)
  {
  }

  /// @brief 平行移動する．
  /// @param[in] vec 移動量を表すベクタ
  /// @return 移動後の自分自身への定数参照を返す．
  const DiagPoints& move(const Point& vec)
  {
    mLeft += vec.mX;
    mUpper += vec.mY;
    mRight += vec.mX;
    mLower += vec.mY;
    return *this;
  }
  
  /// @brief 左端のX座標
  GLdouble mLeft;
  /// @brief 上端のY座標
  GLdouble mUpper;
  /// @brief 右端のX座標
  GLdouble mRight;
  /// @brief 下端のY座標
  GLdouble mLower;
};

END_NAMESPACE_YM_GLV

#endif // LED_GLV_H
