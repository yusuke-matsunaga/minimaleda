#ifndef LED_GATEOBJ_H
#define LED_GATEOBJ_H

/// @file led/GateObj.h
/// @brief GateObj のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
//
/// $Id: GateObj.h 347 2007-01-16 02:37:45Z matsunaga $
///
/// Copyright(C) 2002 by Yusuke Matsunaga


#include <ym_utils/DlList.h>
#include "Glv.h"


BEGIN_NAMESPACE_YM_GLV

class GateTemplate;

//////////////////////////////////////////////////////////////////////
/// @class GateObj GateObj.h "GateObj.h"
/// @brief ゲートのインスタンスを表すクラス
//////////////////////////////////////////////////////////////////////
class GateObj :
  public DlElem
{
  friend class LedScene;
public:

  /// @brief コンストラクタ
  /// @param[in] gt ゲートテンプレート
  /// @param[in] loc 位置
  GateObj(const GateTemplate* gt,
	  const Point& loc = Point(0.0, 0.0));

  /// @brief コンストラクタ
  /// @param[in] gt ゲートテンプレート
  /// @param[in] x 位置のX座標
  /// @param[in] y 位置のY座標
  GateObj(const GateTemplate* gt,
	  GLdouble x,
	  GLdouble y);

  /// @brief デストラクタ
  ~GateObj();


public:
  //////////////////////////////////////////////////////////////////////
  // 位置に関する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 位置を設定する．
  /// @param[in] loc 設定する位置
  void set_location(const Point& loc);

  /// @brief 位置を設定する．
  /// @param[in] x 位置のX座標
  /// @param[in] y 位置のY座標
  void set_location(GLdouble x,
		    GLdouble y);
  
  /// @brief 位置を取得する．
  Point location() const;


public:

  /// @brief 入力数を得る．
  size_t ni() const;

  /// @brief 出力数を得る．
  size_t no() const;

  /// @brief このゲートを囲む最小の矩形を表す左上と右下の点を得る．
  DiagPoints bounding_box() const;

  /// @brief pos 番目の入力ピン位置を得る．
  Point ipin_location(size_t pos) const;

  /// @brief pos 番目の出力ピン位置を得る．
  Point opin_location(size_t pos) const;

  // @brief ディスプレイリストを取り出す．
  GLuint id() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ゲートテンプレート
  const GateTemplate* mGateTemplate;

  // 位置
  Point mLocation;

  // オブジェクト管理用の番号
  size_t mObjId;

};

/// @brief GateObj のリスト
typedef DlList<GateObj> GateObjList;
/// @brief GateObjList の反復子
typedef DlListIter<GateObj> GateObjListIter;
/// @brief GateObjList の定数反復子
typedef DlListConstIter<GateObj> GateObjListConstIter;


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] gt ゲートテンプレート
// @param[in] loc 位置
inline
GateObj::GateObj(const GateTemplate* gt,
		 const Point& loc) :
  mGateTemplate(gt),
  mLocation(loc)
{
}

// @brief コンストラクタ
// @param[in] gt ゲートテンプレート
// @param[in] x 位置のX座標
// @param[in] y 位置のY座標
inline
GateObj::GateObj(const GateTemplate* gt,
		 GLdouble x,
		 GLdouble y) :
  mGateTemplate(gt),
  mLocation(x, y)
{
}

// @brief デストラクタ
inline
GateObj::~GateObj()
{
}

// @brief 位置を設定する．
// @param[in] loc 設定する位置
inline
void
GateObj::set_location(const Point& loc)
{
  mLocation = loc;
}

// @brief 位置を設定する．
// @param[in] x 位置のX座標
// @param[in] y 位置のY座標
inline
void
GateObj::set_location(GLdouble x,
		      GLdouble y)
{
  mLocation.mX = x;
  mLocation.mY = y;
}

// @brief 位置を取得する．
inline
Point
GateObj::location() const
{
  return mLocation;
}

END_NAMESPACE_YM_GLV

#endif // LED_GATEOBJ_H
