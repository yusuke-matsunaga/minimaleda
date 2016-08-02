
/// @file led/GlvScene.cc
/// @brief GL用の描画領域クラス
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: GlvScene.cc 347 2007-01-16 02:37:45Z matsunaga $
///
/// Copyright (C) 2005-2006 Yusuke Matsunaga
/// All rights reserved.


#if HAVE_CONFIG_H
#include "config.h"
#endif

#include "GlvScene.h"


BEGIN_NAMESPACE_YM_GLV

// コンストラクタ
GlvScene::GlvScene(bool has_alpha)
{
  // Configure OpenGL-capable visual.
  Glib::RefPtr<Gdk::GL::Config> glconfig;

  Gdk::GL::ConfigMode rgb_mode = Gdk::GL::MODE_RGB;
  if ( has_alpha ) {
    rgb_mode = Gdk::GL::MODE_RGBA;
  }

  // Try double-buffered visual
  glconfig = Gdk::GL::Config::create(rgb_mode             |
                                     Gdk::GL::MODE_DEPTH  |
                                     Gdk::GL::MODE_DOUBLE);
  if ( !glconfig ) {
    std::cerr << "*** Cannot find the double-buffered visual." << std::endl
	      << "*** Trying single-buffered visual." << std::endl;

    // Try single-buffered visual
    glconfig = Gdk::GL::Config::create(rgb_mode            |
				       Gdk::GL::MODE_DEPTH);
    if ( !glconfig ) {
      std::cerr << "*** Cannot find any OpenGL-capable visual." << std::endl;
      std::exit(1);
    }
  }

  // print frame buffer attributes.
  //GLConfigUtil::examine_gl_attrib(glconfig);

  // Set OpenGL-capability to the widget.
  set_gl_capability(glconfig);

  // Add events.
  add_events(Gdk::VISIBILITY_NOTIFY_MASK);
}

// デストラクタ
GlvScene::~GlvScene()
{
}

// @brief 初期化
// @note on_realize() 中で呼ばれる．
// @note gl_begin() と gl_end() で挟まれている．
void
GlvScene::initialize()
{
}

// @brief ウィンドウのサイズが変わったときの処理
// @note on_configure_event() 中で呼ばれる．
// @note gl_begin() と gl_end() で挟まれている．
void
GlvScene::resize()
{
}

// @brief 描画を行う．
// @note on_expose_event() 中で呼ばれる．
// @note gl_begin() と gl_end() で挟まれている．
// @note ただし gl_end() の前に swap_buffers() か glFlush()
// を行う．
void
GlvScene::draw()
{
}

// "realize" イベントハンドラ
void
GlvScene::on_realize()
{
  // We need to call the base on_realize()
  Gtk::DrawingArea::on_realize();

  // Get GL::Drawable.
  Glib::RefPtr<Gdk::GL::Drawable> gldrawable = get_gl_drawable();

  // GL calls.
  // *** OpenGL BEGIN ***
  if ( !gldrawable->gl_begin(get_gl_context()) ) {
    return;
  }

  initialize();

  gldrawable->gl_end();
  // *** OpenGL END ***
}

// "configure" イベントハンドラ
bool
GlvScene::on_configure_event(GdkEventConfigure* event)
{
  // Get GL::Drawable.
  Glib::RefPtr<Gdk::GL::Drawable> gldrawable = get_gl_drawable();

  // GL calls.
  // *** OpenGL BEGIN ***
  if ( !gldrawable->gl_begin(get_gl_context()) ) {
    return false;
  }

  resize();

  gldrawable->gl_end();
  // *** OpenGL END ***

  return true;
}

// "expose" イベントハンドラ
bool
GlvScene::on_expose_event(GdkEventExpose* event)
{
  // Get GL::Drawable.
  Glib::RefPtr<Gdk::GL::Drawable> gldrawable = get_gl_drawable();

  // GL calls.
  // *** OpenGL BEGIN ***
  if ( !gldrawable->gl_begin(get_gl_context()) ) {
    return false;
  }

  draw();

  // Swap buffers.
  if ( gldrawable->is_double_buffered() ) {
    gldrawable->swap_buffers();
  }
  else {
    glFlush();
  }

  gldrawable->gl_end();
  // *** OpenGL END ***

  return true;
}

/// @brief "map" イベントハンドラ
bool
GlvScene::on_map_event(GdkEventAny* event)
{
  return true;
}

/// @brief "unmap" イベントハンドラ
bool
GlvScene::on_unmap_event(GdkEventAny* event)
{
  return true;
}

/// @brief "visibility notify" イベントハンドラ
bool
GlvScene::on_visibility_notify_event(GdkEventVisibility* event)
{
  return true;
}

END_NAMESPACE_YM_GLV
