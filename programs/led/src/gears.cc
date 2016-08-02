
/// @file led/gears.cc
/// @brief GlvScene を使ったデモプログラム
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: gears.cc 347 2007-01-16 02:37:45Z matsunaga $
///
/// Copyright (C) 2005-2006 Yusuke Matsunaga
/// All rights reserved.


#if HAVE_CONFIG_H
#include "config.h"
#endif

#include "GlvScene.h"


BEGIN_NAMESPACE_YM_GLV

///////////////////////////////////////////////////////////////////////////////
//
// Gears scene.
//
///////////////////////////////////////////////////////////////////////////////

class GearsScene :
  public GlvScene
{
public:
  explicit
  GearsScene(bool is_sync = true);

  virtual
  ~GearsScene();


protected:

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

  /// @brief "realize" イベントハンドラ
  virtual
  void on_realize();

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

  /// @brief "idle" イベントハンドラ
  virtual
  bool on_idle();

  void gear(GLfloat inner_radius,
            GLfloat outer_radius,
            GLfloat width,
            GLint   teeth,
            GLfloat tooth_depth);

public:

  // get & set view rotation values.
  void get_view_rot(GLfloat& x,
		    GLfloat& y,
		    GLfloat& z) {
    x = m_ViewRotX;
    y = m_ViewRotY;
    z = m_ViewRotZ;
  }

  void set_view_rot(GLfloat x,
		    GLfloat y,
		    GLfloat z) {
    m_ViewRotX = x;
    m_ViewRotY = y;
    m_ViewRotZ = z;
  }


protected:

  bool m_IsSync;

  // OpenGL scene related variables:
  GLint m_Gear1;
  GLint m_Gear2;
  GLint m_Gear3;

  GLfloat m_ViewRotX;
  GLfloat m_ViewRotY;
  GLfloat m_ViewRotZ;

  GLfloat m_Angle;

protected:

  // idle signal connection:
  sigc::connection m_ConnectionIdle;

  // frame rate evaluation stuff:
  Glib::Timer m_Timer;

  int m_Frames;

};


GearsScene::GearsScene(bool is_sync) :
  GlvScene(false),
  m_IsSync(is_sync),
  m_Gear1(0),
  m_Gear2(0),
  m_Gear3(0),
  m_ViewRotX(20.0),
  m_ViewRotY(30.0),
  m_ViewRotZ(0.0),
  m_Angle(0.0),
  m_Frames(0)
{
}

GearsScene::~GearsScene()
{
}

/*
 * Draw a gear wheel.  You'll probably want to call this function when
 * building a display list since we do a lot of trig here.
 *
 * Input:  inner_radius - radius of hole at center
 * outer_radius - radius at center of teeth
 * width - width of gear
 * teeth - number of teeth
 * tooth_depth - depth of tooth
 */
void
GearsScene::gear(GLfloat inner_radius,
		 GLfloat outer_radius,
		 GLfloat width,
		 GLint   teeth,
		 GLfloat tooth_depth)
{
  GLint i;
  GLfloat r0, r1, r2;
  GLfloat angle, da;
  GLfloat u, v, len;

  r0 = inner_radius;
  r1 = outer_radius - tooth_depth / 2.0;
  r2 = outer_radius + tooth_depth / 2.0;

  da = 2.0 * G_PI / teeth / 4.0;

  glShadeModel(GL_FLAT);

  glNormal3f(0.0, 0.0, 1.0);

  /* draw front face */
  glBegin(GL_QUAD_STRIP);
  for (i = 0; i <= teeth; i++) {
    angle = i * 2.0 * G_PI / teeth;
    glVertex3f(r0 * cos(angle), r0 * sin(angle), width * 0.5);
    glVertex3f(r1 * cos(angle), r1 * sin(angle), width * 0.5);
    if (i < teeth) {
      glVertex3f(r0 * cos(angle), r0 * sin(angle), width * 0.5);
      glVertex3f(r1 * cos(angle + 3 * da), r1 * sin(angle + 3 * da), width * 0.5);
    }
  }
  glEnd();

  /* draw front sides of teeth */
  glBegin(GL_QUADS);
  da = 2.0 * G_PI / teeth / 4.0;
  for (i = 0; i < teeth; i++) {
    angle = i * 2.0 * G_PI / teeth;

    glVertex3f(r1 * cos(angle), r1 * sin(angle), width * 0.5);
    glVertex3f(r2 * cos(angle + da), r2 * sin(angle + da), width * 0.5);
    glVertex3f(r2 * cos(angle + 2 * da), r2 * sin(angle + 2 * da), width * 0.5);
    glVertex3f(r1 * cos(angle + 3 * da), r1 * sin(angle + 3 * da), width * 0.5);
  }
  glEnd();

  glNormal3f(0.0, 0.0, -1.0);

  /* draw back face */
  glBegin(GL_QUAD_STRIP);
  for (i = 0; i <= teeth; i++) {
    angle = i * 2.0 * G_PI / teeth;
    glVertex3f(r1 * cos(angle), r1 * sin(angle), -width * 0.5);
    glVertex3f(r0 * cos(angle), r0 * sin(angle), -width * 0.5);
    if (i < teeth) {
      glVertex3f(r1 * cos(angle + 3 * da), r1 * sin(angle + 3 * da), -width * 0.5);
      glVertex3f(r0 * cos(angle), r0 * sin(angle), -width * 0.5);
    }
  }
  glEnd();

  /* draw back sides of teeth */
  glBegin(GL_QUADS);
  da = 2.0 * G_PI / teeth / 4.0;
  for (i = 0; i < teeth; i++) {
    angle = i * 2.0 * G_PI / teeth;

    glVertex3f(r1 * cos(angle + 3 * da), r1 * sin(angle + 3 * da), -width * 0.5);
    glVertex3f(r2 * cos(angle + 2 * da), r2 * sin(angle + 2 * da), -width * 0.5);
    glVertex3f(r2 * cos(angle + da), r2 * sin(angle + da), -width * 0.5);
    glVertex3f(r1 * cos(angle), r1 * sin(angle), -width * 0.5);
  }
  glEnd();

  /* draw outward faces of teeth */
  glBegin(GL_QUAD_STRIP);
  for (i = 0; i < teeth; i++) {
    angle = i * 2.0 * G_PI / teeth;

    glVertex3f(r1 * cos(angle), r1 * sin(angle), width * 0.5);
    glVertex3f(r1 * cos(angle), r1 * sin(angle), -width * 0.5);
    u = r2 * cos(angle + da) - r1 * cos(angle);
    v = r2 * sin(angle + da) - r1 * sin(angle);
    len = sqrt(u * u + v * v);
    u /= len;
    v /= len;
    glNormal3f(v, -u, 0.0);
    glVertex3f(r2 * cos(angle + da), r2 * sin(angle + da), width * 0.5);
    glVertex3f(r2 * cos(angle + da), r2 * sin(angle + da), -width * 0.5);
    glNormal3f(cos(angle), sin(angle), 0.0);
    glVertex3f(r2 * cos(angle + 2 * da), r2 * sin(angle + 2 * da), width * 0.5);
    glVertex3f(r2 * cos(angle + 2 * da), r2 * sin(angle + 2 * da), -width * 0.5);
    u = r1 * cos(angle + 3 * da) - r2 * cos(angle + 2 * da);
    v = r1 * sin(angle + 3 * da) - r2 * sin(angle + 2 * da);
    glNormal3f(v, -u, 0.0);
    glVertex3f(r1 * cos(angle + 3 * da), r1 * sin(angle + 3 * da), width * 0.5);
    glVertex3f(r1 * cos(angle + 3 * da), r1 * sin(angle + 3 * da), -width * 0.5);
    glNormal3f(cos(angle), sin(angle), 0.0);
  }

  glVertex3f(r1 * cos(0), r1 * sin(0), width * 0.5);
  glVertex3f(r1 * cos(0), r1 * sin(0), -width * 0.5);

  glEnd();

  glShadeModel(GL_SMOOTH);

  /* draw inside radius cylinder */
  glBegin(GL_QUAD_STRIP);
  for (i = 0; i <= teeth; i++) {
    angle = i * 2.0 * G_PI / teeth;
    glNormal3f(-cos(angle), -sin(angle), 0.0);
    glVertex3f(r0 * cos(angle), r0 * sin(angle), -width * 0.5);
    glVertex3f(r0 * cos(angle), r0 * sin(angle), width * 0.5);
  }
  glEnd();

}

void
GearsScene::initialize()
{
  static GLfloat pos[4] = {5.0, 5.0, 10.0, 0.0};
  static GLfloat red[4] = {0.8, 0.1, 0.0, 1.0};
  static GLfloat green[4] = {0.0, 0.8, 0.2, 1.0};
  static GLfloat blue[4] = {0.2, 0.2, 1.0, 1.0};

  glLightfv(GL_LIGHT0, GL_POSITION, pos);
  glEnable(GL_CULL_FACE);
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_DEPTH_TEST);

  // Make the gears.
  m_Gear1 = glGenLists(1);
  glNewList(m_Gear1, GL_COMPILE);
  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, red);
  gear(1.0, 4.0, 1.0, 20, 0.7);
  glEndList();

  m_Gear2 = glGenLists(1);
  glNewList(m_Gear2, GL_COMPILE);
  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, green);
  gear(0.5, 2.0, 2.0, 10, 0.7);
  glEndList();

  m_Gear3 = glGenLists(1);
  glNewList(m_Gear3, GL_COMPILE);
  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, blue);
  gear(1.3, 2.0, 0.5, 10, 0.7);
  glEndList();

  glEnable(GL_NORMALIZE);

  std::cout << "GL_RENDERER   = " << glGetString(GL_RENDERER)   << std::endl;
  std::cout << "GL_VERSION    = " << glGetString(GL_VERSION)    << std::endl;
  std::cout << "GL_VENDOR     = " << glGetString(GL_VENDOR)     << std::endl;
  std::cout << "GL_EXTENSIONS = " << glGetString(GL_EXTENSIONS) << std::endl;
  std::cout << std::endl;
}

void
GearsScene::resize()
{
  GLfloat h = (GLfloat)(get_height()) / (GLfloat)(get_width());

  glViewport(0, 0, get_width(), get_height());
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glFrustum(-1.0, 1.0, -h, h, 5.0, 60.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glTranslatef(0.0, 0.0, -40.0);
}

void
GearsScene::draw()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glPushMatrix();
    glRotatef(m_ViewRotX, 1.0, 0.0, 0.0);
    glRotatef(m_ViewRotY, 0.0, 1.0, 0.0);
    glRotatef(m_ViewRotZ, 0.0, 0.0, 1.0);

    glPushMatrix();
      glTranslatef(-3.0, -2.0, 0.0);
      glRotatef(m_Angle, 0.0, 0.0, 1.0);
      glCallList(m_Gear1);
    glPopMatrix();

    glPushMatrix();
      glTranslatef(3.1, -2.0, 0.0);
      glRotatef(-2.0 * m_Angle - 9.0, 0.0, 0.0, 1.0);
      glCallList(m_Gear2);
    glPopMatrix();

    glPushMatrix();
      glTranslatef(-3.1, 4.2, 0.0);
      glRotatef(-2.0 * m_Angle - 25.0, 0.0, 0.0, 1.0);
      glCallList(m_Gear3);
    glPopMatrix();

  glPopMatrix();
}

void
GearsScene::on_realize()
{
  GlvScene::on_realize();

  // Start timer.
  m_Timer.start();
}

bool
GearsScene::on_expose_event(GdkEventExpose* event)
{
  bool ans = GlvScene::on_expose_event(event);

  ++m_Frames;

  double seconds = m_Timer.elapsed();
  if ( seconds >= 5.0 ) {
    // std::cout.setf(std::ios_base::fixed, std::ios_base::floatfield);
    std::cout.setf(std::ios::fixed, std::ios::floatfield);
    std::cout.precision(3);
    std::cout << m_Frames << " frames in "
	      << seconds << " seconds = "
	      << (m_Frames / seconds) << " FPS\n";
    m_Timer.reset();
    m_Frames = 0;
  }

  return ans;
}

// "map" イベントハンドラ
bool
GearsScene::on_map_event(GdkEventAny* event)
{
  if ( !m_ConnectionIdle.connected() ) {
    m_ConnectionIdle =
      Glib::signal_idle().connect(sigc::mem_fun(*this, &GearsScene::on_idle), GDK_PRIORITY_REDRAW);
  }
  return true;
}

// "unmap" イベントハンドラ
bool
GearsScene::on_unmap_event(GdkEventAny* event)
{
  if ( m_ConnectionIdle.connected() ) {
    m_ConnectionIdle.disconnect();
  }
  return true;
}

// "visibility notify" イベントハンドラ
bool
GearsScene::on_visibility_notify_event(GdkEventVisibility* event)
{
  if ( event->state == GDK_VISIBILITY_FULLY_OBSCURED ) {
    if ( m_ConnectionIdle.connected() ) {
      m_ConnectionIdle.disconnect();
    }
  }
  else {
    if ( !m_ConnectionIdle.connected() ) {
      m_ConnectionIdle =
	Glib::signal_idle().connect(sigc::mem_fun(*this, &GearsScene::on_idle), GDK_PRIORITY_REDRAW);
    }
  }
  return true;
}

// "idle" イベントハンドラ
bool
GearsScene::on_idle()
{
  m_Angle += 2.0;

  // Invalidate the whole window.
  invalidate();

  // Update window synchronously (fast).
  if ( m_IsSync ) {
    update();
  }

  return true;
}


///////////////////////////////////////////////////////////////////////////////
//
// The application class.
//
///////////////////////////////////////////////////////////////////////////////
class Gears :
  public Gtk::Window
{
public:

  explicit
  Gears(bool is_sync = true);

  virtual
  ~Gears();


protected:
  // signal handlers:
  void on_button_quit_clicked();

  virtual
  bool on_key_press_event(GdkEventKey* event);


protected:
  // member widgets:
  Gtk::VBox m_VBox;
  GearsScene m_GearsScene;
  Gtk::Button m_ButtonQuit;
};

Gears::Gears(bool is_sync) :
  m_VBox(false, 0),
  m_GearsScene(is_sync),
  m_ButtonQuit("Quit")
{
  //
  // Top-level window.
  //

  set_title("Gears");

  // Get automatically redrawn if any of their children changed allocation.
  set_reallocate_redraws(true);

  add(m_VBox);

  //
  // Gears scene.
  //

  m_GearsScene.set_size_request(300, 300);

  m_VBox.pack_start(m_GearsScene);

  //
  // Simple quit button.
  //

  m_ButtonQuit.signal_clicked().connect(
    sigc::mem_fun(*this, &Gears::on_button_quit_clicked));

  m_VBox.pack_start(m_ButtonQuit, Gtk::PACK_SHRINK, 0);

  //
  // Show window.
  //

  show_all();
}

Gears::~Gears()
{
}

void
Gears::on_button_quit_clicked()
{
  Gtk::Main::quit();
}

bool
Gears::on_key_press_event(GdkEventKey* event)
{
  GLfloat x, y, z;

  m_GearsScene.get_view_rot(x, y, z);

  switch (event->keyval)
    {
    case GDK_z:
      z += 5.0;
      break;
    case GDK_Z:
      z -= 5.0;
      break;
    case GDK_Up:
      x += 5.0;
      break;
    case GDK_Down:
      x -= 5.0;
      break;
    case GDK_Left:
      y += 5.0;
      break;
    case GDK_Right:
      y -= 5.0;
      break;
    case GDK_Escape:
      Gtk::Main::quit();
      break;
    default:
      return true;
    }

  m_GearsScene.set_view_rot(x, y, z);

  m_GearsScene.invalidate();

  return true;
}

END_NAMESPACE_YM_GLV


///////////////////////////////////////////////////////////////////////////////
//
// Main.
//
///////////////////////////////////////////////////////////////////////////////

int
main(int argc,
     char** argv)
{
  Gtk::Main kit(argc, argv);

  //
  // Init gtkglextmm.
  //

  Gtk::GL::init(argc, argv);

  //
  // Parse arguments.
  //

  bool is_sync = true;

  for (int i = 1; i < argc; ++i) {
    if (std::strcmp(argv[i], "--async") == 0)
      is_sync = false;
  }

  //
  // Instantiate and run the application.
  //

  nsYm::nsGlv::Gears gears(is_sync);

  kit.run(gears);

  return 0;
}
