
/// @file led/GateObj.cc
/// @brief GateObj の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: GateObj.cc 347 2007-01-16 02:37:45Z matsunaga $
///
/// Copyright (C) 2005-2006 Yusuke Matsunaga
/// All rights reserved.

#if HAVE_CONFIG_H
#include "config.h"
#endif

#include "GateObj.h"
#include "GateTemplate.h"


BEGIN_NAMESPACE_YM_GLV

// @brief 入力数を得る．
size_t
GateObj::ni() const
{
  return mGateTemplate->ni();
}

// @brief 出力数を得る．
size_t
GateObj::no() const
{
  return mGateTemplate->no();
}

// @brief このゲートを囲む最小の矩形を表す左上と右下の点を得る．
DiagPoints
GateObj::bounding_box() const
{
  DiagPoints tmp = mGateTemplate->bounding_box();
  return tmp.move(mLocation);
} 

// @brief pos 番目の入力ピン位置を得る．
Point
GateObj::ipin_location(size_t pos) const
{
  Point tmp = mGateTemplate->ipin_location(pos);
  return tmp.move(mLocation);
}

// @brief pos 番目の出力ピン位置を得る．
Point
GateObj::opin_location(size_t pos) const
{
  Point tmp = mGateTemplate->opin_location(pos);
  return tmp.move(mLocation);
}

// @brief ディスプレイリストを取り出す．
GLuint
GateObj::id() const
{
  return mGateTemplate->id();
}

END_NAMESPACE_YM_GLV
