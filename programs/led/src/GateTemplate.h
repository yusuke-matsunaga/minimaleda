#ifndef LED_GATETEMPLATE_H
#define LED_GATETEMPLATE_H

/// @file led/GateTemplate.h
/// @brief ゲートを描画するためのテンプレート
/// @author Yusuke Matsunaga (松永 裕介)
//
/// $Id: GateTemplate.h 347 2007-01-16 02:37:45Z matsunaga $
///
/// Copyright(C) 2002 by Yusuke Matsunaga


#include "Glv.h"


BEGIN_NAMESPACE_YM_GLV

//////////////////////////////////////////////////////////////////////
/// @class GateTemplate GateTemplate.h "GateTemplate.h"
/// @brief ゲートを描画するためのテンプレート
//////////////////////////////////////////////////////////////////////
class GateTemplate
{
  friend class LedScene;
  
private:

  /// @brief コンストラクタ
  /// @param[in] ni 入力数
  /// @param[in] no 出力数
  GateTemplate(size_t ni,
	       size_t no);

  /// @brief デストラクタ
  ~GateTemplate();


public:

  /// @brief 入力数を得る．
  size_t ni() const;

  /// @brief 出力数を得る．
  size_t no() const;

  /// @brief このゲートを囲む最小の矩形を表す左上と右下の点を得る．
  DiagPoints bounding_box() const;

  /// @brief すべての入力ピン位置を得る．
  const vector<Point>& ipin_locations() const;

  /// @brief pos 番目の入力ピン位置を得る．
  Point ipin_location(size_t pos) const;

  /// @brief すべての出力ピン位置を得る．
  const vector<Point>& opin_locations() const;

  /// @brief pos 番目の出力ピン位置を得る．
  Point opin_location(size_t pos) const;

  /// @brief 描画用の id を得る．
  GLuint id() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // GL の display list
  GLuint mId;

  // このゲートを囲む最小の矩形の対角線上の点
  DiagPoints mBoundingBox;

  // 入力ピン位置の座標のベクタ
  vector<Point> mIpinLocations;

  // 出力ピン位置の座標のベクタ
  vector<Point> mOpinLocations;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] ni 入力数
// @param[in] no 出力数
inline
GateTemplate::GateTemplate(size_t ni,
			   size_t no) :
  mIpinLocations(ni),
  mOpinLocations(no)
{
}

// @brief デストラクタ
inline
GateTemplate::~GateTemplate()
{
}

// @brief 入力数を得る．
inline
size_t
GateTemplate::ni() const
{
  return mIpinLocations.size();
}

// @brief 出力数を得る．
inline
size_t
GateTemplate::no() const
{
  return mOpinLocations.size();
}

// @brief このゲートを囲む最小の矩形を表す左上と右下の点を得る．
inline
DiagPoints
GateTemplate::bounding_box() const
{
  return mBoundingBox;
}

// @brief すべての入力ピン位置を得る．
inline
const vector<Point>&
GateTemplate::ipin_locations() const
{
  return mIpinLocations;
}

// @brief pos 番目の入力ピン位置を得る．
inline
Point
GateTemplate::ipin_location(size_t pos) const
{
  return mIpinLocations[pos];
}

// @brief すべての出力ピン位置を得る．
inline
const vector<Point>&
GateTemplate::opin_locations() const
{
  return mOpinLocations;
}

// @brief pos 番目の出力ピン位置を得る．
inline
Point
GateTemplate::opin_location(size_t pos) const
{
  return mOpinLocations[pos];
}

// @brief 描画用の id を得る．
inline
GLuint
GateTemplate::id() const
{
  return mId;
}

END_NAMESPACE_YM_GLV

#endif // LED_GATETEMPLATE_H
