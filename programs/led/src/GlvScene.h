#ifndef LED_GLVSCENE_H
#define LED_GLVSCENE_H

/// @file led/GlvScene.h
/// @brief GL用の描画領域クラス
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: GlvScene.h 347 2007-01-16 02:37:45Z matsunaga $
///
/// Copyright (C) 2005-2006 Yusuke Matsunaga
/// All rights reserved.

#include "Glv.h"


BEGIN_NAMESPACE_YM_GLV

//////////////////////////////////////////////////////////////////////
/// @class GlvScene GlvScene.h "GlvScene.h"
/// @brief GLV scene class
//////////////////////////////////////////////////////////////////////
class GlvScene :
  public Gtk::GL::DrawingArea
{
public:
  /// @brief コンストラクタ
  /// @param[in] has_alph アルファ値を持つとき true
  GlvScene(bool has_alpha);

  /// @brief デストラクタ
  virtual
  ~GlvScene();


public:

  /// @brief ウィンドウ全体を無効化する．
  void invalidate();

  /// @brief ウィンドウを更新する．
  void update();


protected:
  //////////////////////////////////////////////////////////////////////
  // 継承クラスが実装しなければならない仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 初期化
  /// @note on_realize() 中で呼ばれる．
  /// @note gl_begin() と gl_end() で挟まれている．
  virtual
  void initialize();

  /// @brief ウィンドウのサイズが変わったときの処理
  /// @note on_configure_event() 中で呼ばれる．
  /// @note gl_begin() と gl_end() で挟まれている．
  virtual
  void resize();

  /// @brief 描画を行う．
  /// @note on_expose_event() 中で呼ばれる．
  /// @note gl_begin() と gl_end() で挟まれている．
  /// @note ただし gl_end() の前に swap_buffers() か glFlush()
  /// を行う．
  virtual
  void draw();


protected:
  //////////////////////////////////////////////////////////////////////
  // シグナルハンドラ
  //////////////////////////////////////////////////////////////////////

  /// @brief "realize" イベントハンドラ
  virtual
  void on_realize();

  /// @brief "configure" イベントハンドラ
  virtual
  bool on_configure_event(GdkEventConfigure* event);

  /// @brief "expose" イベントハンドラ
  virtual
  bool on_expose_event(GdkEventExpose* event);

  /// @brief "map" イベントハンドラ
  virtual
  bool on_map_event(GdkEventAny* event);

  /// @brief "unmap" イベントハンドラ
  virtual
  bool on_unmap_event(GdkEventAny* event);

  /// @brief "visibility notify" イベントハンドラ
  virtual
  bool on_visibility_notify_event(GdkEventVisibility* event);

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief ウィンドウ全体を無効化する．
inline
void
GlvScene::invalidate()
{
  get_window()->invalidate_rect(get_allocation(), false);
}

// @brief ウィンドウを更新する．
inline
void
GlvScene::update()
{
  get_window()->process_updates(false);
}

END_NAMESPACE_YM_GLV

#endif // LED_GLVSCENE_H
