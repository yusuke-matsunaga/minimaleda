
/// @file led/led.cc
/// @brief LedScene を使ったデモプログラム
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: led.cc 347 2007-01-16 02:37:45Z matsunaga $
///
/// Copyright (C) 2005-2006 Yusuke Matsunaga
/// All rights reserved.


#if HAVE_CONFIG_H
#include "config.h"
#endif

#include "LedScene.h"


BEGIN_NAMESPACE_YM_GLV


///////////////////////////////////////////////////////////////////////////////
// The application class.
///////////////////////////////////////////////////////////////////////////////
class Led :
  public Gtk::Window
{
public:

  Led();

  virtual
  ~Led();


protected:
  // signal handlers:
  void on_button_quit_clicked();

  virtual
  bool on_key_press_event(GdkEventKey* event);


protected:
  // member widgets:
  Gtk::VBox m_VBox;
  LedScene m_LedScene;
  Gtk::Button m_ButtonQuit;
};


Led::Led() :
  m_VBox(false, 0),
  m_LedScene(),
  m_ButtonQuit("Quit")
{
  // Top-level window.
  set_title("Led");

  // Get automatically redrawn if any of their children changed allocation.
  set_reallocate_redraws(true);

  add(m_VBox);


  m_VBox.pack_start(m_LedScene);

  // Simple quit button.
  m_ButtonQuit.signal_clicked().connect(
    sigc::mem_fun(*this, &Led::on_button_quit_clicked));

  m_VBox.pack_start(m_ButtonQuit, Gtk::PACK_SHRINK, 0);

  // Show window.
  set_size_request(3000, 2000);

  set_size_request(300, 200);
  
  show_all();
}

Led::~Led()
{
}

void
Led::on_button_quit_clicked()
{
  Gtk::Main::quit();
}

bool
Led::on_key_press_event(GdkEventKey* event)
{
  switch (event->keyval) {
  case GDK_z:
    m_LedScene.set_zoom(m_LedScene.zoom() / 2.0);
    break;
  case GDK_Z:
    m_LedScene.set_zoom(m_LedScene.zoom() * 2.0);
    break;
  case GDK_i:
    {
      Point p = m_LedScene.corner();
      p.mY += 10.0;
      m_LedScene.set_corner(p);
    }
    break;
  case GDK_k:
    {
      Point p = m_LedScene.corner();
      p.mY -= 10.0;
      m_LedScene.set_corner(p);
    }
    break;
  case GDK_l:
    {
      Point p = m_LedScene.corner();
      p.mX += 10.0;
      m_LedScene.set_corner(p);
    }
    break;
  case GDK_j:
    {
      Point p = m_LedScene.corner();
      p.mX -= 10.0;
      m_LedScene.set_corner(p);
    }
    break;
  case GDK_t:
    if ( m_LedScene.bbox_mode() ) {
      m_LedScene.disable_bbox();
    }
    else {
      m_LedScene.enable_bbox();
    }
    break;
  case GDK_q:
  case GDK_Q:
  case GDK_Escape:
    Gtk::Main::quit();
    break;
  default:
    return true;
  }
  
  return true;
}

END_NAMESPACE_YM_GLV


///////////////////////////////////////////////////////////////////////////////
// Main.
///////////////////////////////////////////////////////////////////////////////

int
main(int argc,
     char** argv)
{
  Gtk::Main kit(argc, argv);

  // Init gtkglextmm.
  Gtk::GL::init(argc, argv);

  // Parse arguments.
#if 0
  bool is_sync = true;
  for (int i = 1; i < argc; ++i) {
    if (std::strcmp(argv[i], "--async") == 0)
      is_sync = false;
  }
#endif
  
  // Instantiate and run the application.
  nsYm::nsGlv::Led led;

  kit.run(led);

  return 0;
}
