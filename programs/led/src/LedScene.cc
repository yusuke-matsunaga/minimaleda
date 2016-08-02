
/// @file led/LedScene.cc
/// @brief ゲート描画用の drawing area
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: LedScene.cc 347 2007-01-16 02:37:45Z matsunaga $
///
/// Copyright (C) 2005-2006 Yusuke Matsunaga
/// All rights reserved.

#if HAVE_CONFIG_H
#include "config.h"
#endif

#include "LedScene.h"
#include "GateTemplate.h"
#include "GateObj.h"


BEGIN_NONAMESPACE

// 三角関数関係の定数
const double pi     = 3.14159265358979323846;
const double sqrt3  = 1.73205080756887729353;

// 基本ユニット
const double kUnit  = 8.0;

// ドットの大きさ
const double kDotH  = 1.5 * kUnit;
const double kDotW  = 1.5 * kUnit;

// バッファ(正三角形)のサイズ
const double kBufH  = 7.0 * kUnit;
const double kBufW  = (kBufH / 2.0) * sqrt3;

// 一般のゲート(AND/OR/XOR)のサイズ
const double kGateH =  8.0 * kUnit;
const double kGateW = 10.0 * kUnit;

// ゲートの上端の Y 座標
const double kGateUY =   (kGateH / 2.0);

// ゲートの下端の Y 座標
const double kGateLY = - (kGateH / 2.0);

// 2,3,4,5入力の時の入力のオフセット
// 上下対称なので半分だけ書いてある．
const double kOff[][2] = {
  { 2.0 * kUnit, 0.0 * kUnit },
  { 2.5 * kUnit, 0.0 * kUnit },
  { 3.0 * kUnit, 1.0 * kUnit },
  { 3.0 * kUnit, 1.5 * kUnit }
};

// XORの入力側の円弧のオフセット
const double kXorM  = 1.0 * kUnit;

// ANDの円弧の半径
const double kAndR  = kGateH / 2.0;

// ANDの短辺の長さ
const double kAndL  = kGateW - kAndR;

// ORの円弧の半径
const double kOrR   = kGateH;

// ORの短辺の長さ
const double kOrL   = kGateW - (kOrR / 2.0) * sqrt3;

// XORの円弧の半径
const double kXorR  = kGateH;

// XORの短辺の長さ
const double kXorL  = kGateW - (kXorR / 2.0) * sqrt3;

// 配線の長さ
const double kWireW = 2.0 * kUnit;

// 配線の上下の幅
const double kWireH = 1.0 * kUnit;

// 配線のX軸方向のマージン
const double kWireXmargin = 1.0 * kUnit;

// 配線のY軸方向のマージン
const double kWireYmargin = 1.0 * kUnit;

// 入力側のマージン
const double kImargin = 2.0 * kUnit;

// 出力側のマージン
const double kOmargin = 2.0 * kUnit;

// 縦方向のマージン
const double kHmargin = 1.0 * kUnit;

// ゲート線の幅
const double kGateLineWidth = 2.0;

// ゲート線の赤
const GLclampf kGateLineRed = 1.0;

// ゲート線の緑
const GLclampf kGateLineGreen = 0.0;

// ゲート線の青
const GLclampf kGateLineBlue = 1.0;

// ゲート線のアルファ
const GLclampf kGateLineAlpha = 1.0;

// 配線の幅
const double kWireLineWidth = 2.0;

// 配線の赤
const GLclampf kWireLineRed = 0.0;

// 配線の緑
const GLclampf kWireLineGreen = 1.0;

// 配線の青
const GLclampf kWireLineBlue = 1.0;

// 配線のアルファ
const GLclampf kWireLineAlpha = 1.0;

// 枠線の幅
const double kBBoxLineWidth = 1.0;

// 枠線の赤
const GLclampf kBBoxLineRed = 1.0;

// 枠線の緑
const GLclampf kBBoxLineGreen = 1.0;

// 枠線の青
const GLclampf kBBoxLineBlue = 1.0;

// 枠線のアルファ
const GLclampf kBBoxLineAlpha = 1.0;

// 入力シンボルの長さ
const double kInputW = 8.0 * kUnit;

// 入力シンボルの上下の幅
const double kInputH = 2.0 * kUnit;

// 出力シンボルの長さ
const double kOutputW = 8.0 * kUnit;

// 出力シンボルの上下の幅
const double kOutputH = 2.0 * kUnit;

END_NONAMESPACE


BEGIN_NAMESPACE_YM_GLV

//////////////////////////////////////////////////////////////////////
// このファイル内でのみ使われる関数
//////////////////////////////////////////////////////////////////////

// AND/OR/XOR ゲートの入力辺のサイズを返す．
size_t
calc_points(size_t ni,
	    double& uy,
	    double& ly)
{
  switch (ni) {
  case 6:
  case 7:
  case 8:
    uy = kGateUY + kGateH;
    ly = kGateLY;
    return 2;
  case 9:
  case 10:
    uy = kGateUY + kGateH;
    ly = kGateLY - kGateH;
    return 3;
  }
  size_t nseg = (ni + 4) / 5;
  size_t nseg_u = nseg / 2;
  size_t nseg_l = (nseg - 1) / 2;
  uy = kGateUY + (kGateH * nseg_u);
  ly = kGateLY - (kGateH * nseg_l);
  return nseg;
}

// 入力ピン位置を決めるためのサブルーティン
static
void
distribute_pins(size_t ni,
		vector<size_t>& npins)
{
  if ( ni >= 6 && ni <= 10 ) {
    switch (ni) {
    case 6:
      npins[0] = 3;
      npins[1] = 3;
      break;
    case 7:
      npins[0] = 4;
      npins[1] = 3;
      break;
    case 8:
      npins[0] = 4;
      npins[1] = 4;
      break;
    case 9:
      npins[0] = 3;
      npins[1] = 3;
      npins[2] = 3;
      break;
    case 10:
      npins[0] = 3;
      npins[1] = 4;
      npins[2] = 3;
      break;
    }
  } else {
    size_t nseg = npins.size();
    size_t c = nseg / 2;  // 真ん中のセグメント
    bool up = false;
    size_t u = c - 1; // c が 0 の時はやばいけど, そのときは使われない．
    size_t l = c;
    while ( ni > 0 ) {
      if ( up ) {
	if ( ni >= 9 ) {
	  npins[u] = 5;
	} else if ( ni == 8 || ni == 7 ) {
	  npins[u] = 4;
	} else if ( ni == 6 ) {
	  npins[u] = 3;
	} else {
	  npins[u] = ni;
	}
	ni -= npins[u];
	u --;
	up = false;
      } else {
	if ( ni >= 9 ) {
	  npins[l] = 5;
	} else if ( ni == 8 || ni == 7 ) {
	  npins[l] = 4;
	} else if ( ni == 6 ) {
	  npins[l] = 3;
	} else {
	  npins[l] = ni;
	}
	ni -= npins[l];
	l ++;
	up = true;
      }
    }
  }
}

// AND/OR/XOR の入力辺上のピンの座標を計算する関数
// x_off : X 軸のオフセット
// y_off : Y 軸のオフセット
// pos   : 入力ピン位置(上が0)
// n     : 入力ピン数
// or_xor: OR/XOR の時 true 
static
Point
ipos(double x_off,
     double y_off,
     size_t pos,
     size_t n,
     bool or_xor)
{
  double y1;
  size_t dpos = pos * 2 + 1;
  if ( dpos < n ) {
    y1 = -kOff[n - 2][pos];
  } else if ( dpos == n ) {
    y1 = 0.0;
  } else {
    y1 = kOff[n - 2][n - pos - 1];
  }
  double x1 = 0.0;
  if ( or_xor ) {
    double y2 = kGateH * 0.5;
    if ( y1 > 0.0 ) {
      y2 -= y1;
    } else {
      y2 += y1;
    }
    double tmp1 = sqrt(3.0 * kOrR * kOrR / 4.0 + kOrR * y2 - y2 * y2);
    x1 = tmp1 - sqrt3 * kOrR / 2.0;
  }
  return Point(x_off + x1, y_off + y1);
}

// 円弧を表す点列を作る．
static
void
create_arc(double cx,
	   double cy,
	   double r,
	   size_t ndiv,
	   double start_rad,
	   double end_rad)
{
  double rad = pi * start_rad;
  double diff = end_rad - start_rad;
  double inc = (pi * diff) / ndiv;
  for (size_t i = 1; i < ndiv; ++ i) {
    rad += inc;
    double dx = cx + cos(rad) * r;
    double dy = cy + sin(rad) * r;
    glVertex2d(dx, dy);
  }
}

// 否定を表すドットを描く
static
void
draw_dot(double x,
	 double y)
{
  const double r = kDotW / 2.0;
  glBegin(GL_LINE_LOOP);
  create_arc(x + r, 0.0, r, 36, 0.0, 2.0);
  glEnd();
}

// AND ゲートのシンボルを描く
static
void
draw_and(double uy,
	 double ly)
{
  glBegin(GL_LINE_LOOP);
  glVertex2d(0.0,   kGateLY);
  glVertex2d(kAndL, kGateLY);
  create_arc(kAndL, 0.0, kAndR, 18, 1.5, 2.5);
  glVertex2d(kAndL, kGateUY);
  glVertex2d(0.0,   kGateUY);
  glEnd();
    
  // 上側に延ばす辺
  if ( uy != kGateUY ) {
    glBegin(GL_LINE_STRIP);
    glVertex2d(0.0, uy);
    glVertex2d(0.0, kGateUY);
    glEnd();
  }
    
  // 下側に延ばす辺
  if ( ly != kGateLY ) {
    glBegin(GL_LINE_STRIP);
    glVertex2d(0.0, kGateLY);
    glVertex2d(0.0, ly);
    glEnd();
  }
}

// OR/XOR ゲートのシンボルを描く
static
void
draw_or(double uy,
	double ly,
	bool xor_gate)
{
  size_t ndiv = 8;
  double cx = - (kOrR / 2.0) * sqrt3;

  glBegin(GL_LINE_LOOP);
  glVertex2d(0.0,    kGateLY);
  glVertex2d(kOrL,   kGateLY);
  create_arc(kOrL,   kGateUY, kOrR, ndiv, 9.0 / 6.0, 11.0 / 6.0);
  glVertex2d(kGateW, 0.0);
  create_arc(kOrL,   kGateLY, kOrR, ndiv, 1.0 / 6.0, 3.0 / 6.0);
  glVertex2d(kOrL,   kGateUY);
  glVertex2d(0.0,    kGateUY);
  create_arc(cx,     0.0, kOrR, ndiv, 1.0 / 6.0, -1.0 / 6.0);
  glEnd();

  if ( xor_gate ) {
    glBegin(GL_LINE_STRIP);
    glVertex2d(- kXorM, kGateUY);
    create_arc(cx - kXorM, 0.0, kXorR, ndiv, 1.0 / 6.0, - 1.0 / 6.0);
    glVertex2d(- kXorM, kGateLY);
    glEnd();
  }

  double x0 = 0.0;
  size_t n = (xor_gate) ? 2 : 1;
  for (size_t i = 0; i < n; ++ i, x0 -= kXorM) {
    // 上側に延ばす辺
    if ( uy != kGateUY ) {
      glBegin(GL_LINE_STRIP);
      for (double sy = kGateUY; sy < uy; sy += kGateH ) {
	glVertex2d(x0, sy);
	double cy = sy + (kGateH / 2.0);
	create_arc(x0 + cx, cy, kXorR, ndiv, - 1.0 / 6.0, 1.0 / 6.0);
      }
      glVertex2d(x0, uy);
      glEnd();
    }
    
    // 下側に延ばす辺
    if ( ly != kGateLY ) {
      glBegin(GL_LINE_STRIP);
      for (double sy = ly; sy < kGateLY; sy += kGateH ) {
	glVertex2d(x0, sy);
	double cy = sy + (kGateH / 2.0);
	create_arc(x0 + cx, cy, kXorR, ndiv, - 1.0 / 6.0, 1.0 / 6.0);
      }
      glVertex2d(x0, kGateLY);
      glEnd();
    }
  }
}

// 配列のサイズ合わせをする．
static
void
resize_array(vector<GateTemplate*>& array,
	     size_t n)
{
  size_t s = array.size();
  if ( s <= n ) {
    for (size_t i = s; i <= n; ++ i) {
      array.push_back(NULL);
    }
  }
}


//////////////////////////////////////////////////////////////////////
// メンバ関数
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
LedScene::LedScene() :
  GlvScene(true)
{
  mInput = NULL;
  mOutput = NULL;
  mBuf = NULL;
  mNot = NULL;
  mGateLineWidth = kGateLineWidth;
  mGateLineColor[0] = kGateLineRed;
  mGateLineColor[1] = kGateLineGreen;
  mGateLineColor[2] = kGateLineBlue;
  mGateLineColor[3] = kGateLineAlpha;

  mWireLineWidth = kWireLineWidth;
  mWireLineColor[0] = kWireLineRed;
  mWireLineColor[1] = kWireLineGreen;
  mWireLineColor[2] = kWireLineBlue;
  mWireLineColor[3] = kWireLineAlpha;

  mBBoxLineWidth = kBBoxLineWidth;
  mBBoxLineColor[0] = kBBoxLineRed;
  mBBoxLineColor[1] = kBBoxLineGreen;
  mBBoxLineColor[2] = kBBoxLineBlue;
  mBBoxLineColor[3] = kBBoxLineAlpha;

  mZoom = 1.0;
  mCorner = Point(0.0, 0.0);
  mDrawBBox = true;
}

// @brief デストラクタ
LedScene::~LedScene()
{
  delete mInput;
  delete mOutput;
  delete mBuf;
  delete mNot;
  for (vector<GateTemplate*>::iterator p = mAndArray.begin();
       p != mAndArray.end(); ++ p) {
    delete *p;
  }
  for (vector<GateTemplate*>::iterator p = mNandArray.begin();
       p != mNandArray.end(); ++ p) {
    delete *p;
  }
  for (vector<GateTemplate*>::iterator p = mOrArray.begin();
       p != mOrArray.end(); ++ p) {
    delete *p;
  }
  for (vector<GateTemplate*>::iterator p = mNorArray.begin();
       p != mNorArray.end(); ++ p) {
    delete *p;
  }
  for (vector<GateTemplate*>::iterator p = mXorArray.begin();
       p != mXorArray.end(); ++ p) {
    delete *p;
  }
  for (vector<GateTemplate*>::iterator p = mXnorArray.begin();
       p != mXnorArray.end(); ++ p) {
    delete *p;
  }
}

// @brief 初期化
// @note on_realize() 中で呼ばれる．
// @note gl_begin() と gl_end() で挟まれている．
void
LedScene::initialize()
{
  glEnable(GL_LINE_STIPPLE);
#if 0
  // 基本的なゲートテンプレートを作っておく．
  input();
  output();
  buf_gate();
  not_gate();
  for (size_t i = 2; i <= 4; ++ i) {
    and_gate(i);
    nand_gate(i);
    or_gate(i);
    nor_gate(i);
    xor_gate(i);
    xnor_gate(i);
  }
#endif
  
#if 0
  new_obj(input(), 50.0, 20.0);
  new_obj(output(), 50.0, 100.0);
  new_obj(buf_gate(), 50.0, 200.0);
  new_obj(not_gate(), 50.0, 300.0);
#endif
#if 1
  for (size_t i = 2; i <= 10; ++ i) {
    new_obj(and_gate(i), 50.0 + (i - 2) * 120.0, 200.0);
    new_obj(or_gate(i), 50.0 + (i - 2) * 120.0, 400.0);
    new_obj(xor_gate(i), 50.0 + (i - 2) * 120.0, 600.0);
  }
#endif
#if 0
  new_obj(nand_gate(2), 100.0, 150.0);
  new_obj(or_gate(2), 220.0, 50.0);
  new_obj(nor_gate(2), 220.0, 200.0);
  new_obj(xor_gate(2), 350.0, 50.0);
  new_obj(xnor_gate(2), 350.0, 150.0);
#endif
}

// @brief ウィンドウのサイズが変わったときの処理
// @note on_configure_event() 中で呼ばれる．
// @note gl_begin() と gl_end() で挟まれている．
void
LedScene::resize()
{
  GLint w = get_width();
  GLint h = get_height();
  glViewport(0, 0, w, h);

  GLdouble vw = static_cast<GLdouble>(w) / mZoom;
  GLdouble vh = static_cast<GLdouble>(h) / mZoom;
  Point p = corner();
  GLdouble x0 = p.mX;
  GLdouble y0 = p.mY;

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(x0, x0 + vw, y0, y0 + vh);
  glMatrixMode(GL_MODELVIEW);
}

// @brief 描画を行う．
// @note on_expose_event() 中で呼ばれる．
// @note gl_begin() と gl_end() で挟まれている．
// @note ただし gl_end() の前に swap_buffers() か glFlush()
// を行う．
void
LedScene::draw()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  for (GateObjListIter p = mObjList.begin();
       p != mObjList.end(); ++ p) {
    GateObj* obj = *p;
    draw_obj(obj);
  }
}

// @brief 入力を表すテンプレートを返す．
const GateTemplate*
LedScene::input()
{
  if ( mInput == NULL ) {
    mInput = new GateTemplate(0, 1);

    // 右のX座標
    const double rx = kInputW;
    // 左のX座標
    const double lx = 0.0;
    // 上のY座標
    const double uy = (kInputH / 2.0);
    // 下のY座標
    const double ly = -(kInputH / 2.0);
    // 中央のY座標
    const double cy = 0.0;
    // 斜め線のX座標
    const double mx = rx - uy;

    // 右外周の X座標
    const double brx = rx + kOmargin;
    // 左外周の X座標
    const double blx = lx - kImargin;
    // 上外周の Y座標
    const double buy = uy + kHmargin;
    // 下外周の Y座標
    const double bly = ly - kHmargin;
    
    // 周りの矩形の設定
    mInput->mBoundingBox.mLeft  = blx;
    mInput->mBoundingBox.mUpper = buy;
    mInput->mBoundingBox.mRight = brx;
    mInput->mBoundingBox.mLower = bly;

    // 入力ピンはない

    // 出力ピン位置の設定
    mInput->mOpinLocations[0] = Point(brx, cy);

    // ディスプレイリストの生成
    mInput->mId = glGenLists(1);
    glNewList(mInput->mId, GL_COMPILE);
    
    // 配線の描画
    glLineWidth(wire_line_width());
    glColor4fv(wire_line_color());

    glBegin(GL_LINE_STRIP);
    glVertex2d(rx, cy);
    glVertex2d(brx, cy);
    glEnd();

    // ゲートの描画
    glLineWidth(gate_line_width());
    glColor4fv(gate_line_color());

    glBegin(GL_LINE_LOOP);
    glVertex2d(lx, uy);
    glVertex2d(mx, uy);
    glVertex2d(rx, cy);
    glVertex2d(mx, ly);
    glVertex2d(lx, ly);
    glEnd();

    glEndList();
  }
  return mInput;
}

// @brief 出力を表すテンプレートを返す．
const GateTemplate*
LedScene::output()
{
  if ( mOutput == NULL ) {
    mOutput = new GateTemplate(1, 0);

    // 右のX座標
    const double rx = kOutputW;
    // 左のX座標
    const double lx = 0.0;
    // 上のY座標
    const double uy = (kOutputH / 2.0);
    // 下のY座標
    const double ly = -(kOutputH / 2.0);
    // 中央のY座標
    const double cy = 0.0;
    // 斜め線のX座標
    const double mx = rx - uy;

    // 右外周の X座標
    const double brx = rx + kOmargin;
    // 左外周の X座標
    const double blx = lx - kImargin;
    // 上外周の Y座標
    const double buy = uy + kHmargin;
    // 下外周の Y座標
    const double bly = ly - kHmargin;
    
    // 周りの矩形の設定
    mInput->mBoundingBox.mLeft  = blx;
    mInput->mBoundingBox.mUpper = buy;
    mInput->mBoundingBox.mRight = brx;
    mInput->mBoundingBox.mLower = bly;

    // 入力ピンの位置の設定
    mOutput->mIpinLocations[0] = Point(blx, cy);

    // 出力ピンはない

    // ディスプレイリストの生成
    mOutput->mId = glGenLists(1);
    glNewList(mOutput->mId, GL_COMPILE);

    // 配線の描画
    glLineWidth(wire_line_width());
    glColor4fv(wire_line_color());

    glBegin(GL_LINE_STRIP);
    glVertex2d(blx, cy);
    glVertex2d(lx, cy);
    glEnd();

    // ゲートの描画
    glLineWidth(gate_line_width());
    glColor4fv(gate_line_color());

    glBegin(GL_LINE_LOOP);
    glVertex2d(lx, uy);
    glVertex2d(mx, uy);
    glVertex2d(rx, cy);
    glVertex2d(mx, ly);
    glVertex2d(lx, ly);
    glEnd();
    
    glEndList();
  }
  return mOutput;
}

// @brief バッファを表すテンプレートを返す．
const GateTemplate*
LedScene::buf_gate()
{
  if ( mBuf == NULL ) {
    mBuf = new GateTemplate(1, 1);

    // 右のX座標
    const double rx = kBufW;
    // 左のX座標
    const double lx = 0.0;
    // 上のY座標
    const double uy = (kBufH / 2.0);
    // 下のY座標
    const double ly = - (kBufH / 2.0);
    // 中央のY座標
    const double cy = 0.0;

    // 右外周の X座標
    const double brx = rx + kOmargin;
    // 左外周の X座標
    const double blx = lx - kImargin;
    // 上外周の Y座標
    const double buy = uy + kHmargin;
    // 下外周の Y座標
    const double bly = ly - kHmargin;
    
    // 周りの矩形の設定
    mInput->mBoundingBox.mLeft  = blx;
    mInput->mBoundingBox.mUpper = buy;
    mInput->mBoundingBox.mRight = brx;
    mInput->mBoundingBox.mLower = bly;

    // 入力ピン位置の設定
    mBuf->mIpinLocations[0] = Point(blx, cy);

    // 出力ピン位置の設定
    mBuf->mOpinLocations[0] = Point(brx, cy);
    
    // ディスプレイリストの生成
    mBuf->mId = glGenLists(1);
    glNewList(mBuf->mId, GL_COMPILE);

    // 配線の描画
    glLineWidth(wire_line_width());
    glColor4fv(wire_line_color());

    glBegin(GL_LINE_STRIP);
    glVertex2d(blx, cy);
    glVertex2d(lx, cy);
    glEnd();

    glBegin(GL_LINE_STRIP);
    glVertex2d(rx, cy);
    glVertex2d(brx, cy);
    glEnd();

    // ゲートの描画
    glLineWidth(gate_line_width());
    glColor4fv(gate_line_color());

    glBegin(GL_LINE_LOOP);
    glVertex2d(lx, uy);
    glVertex2d(lx, ly);
    glVertex2d(rx, cy);
    glEnd();
    
    glEndList();
  }
  return mBuf;
}

// @brief NOTゲートを表すテンプレートを返す．
const GateTemplate*
LedScene::not_gate()
{
  if ( mNot == NULL ) {
    mNot = new GateTemplate(1, 1);

    // 右のX座標
    double gx = kBufW;
    // 左のX座標
    double lx = 0.0;
    // 上のY座標
    double uy = (kBufH / 2.0);
    // 下のY座標
    double ly = - (kBufH / 2.0);
    // 中央のY座標
    double cy = 0.0;

    // ドットの右端の X座標
    double rx = gx + kDotW;
    
    // 右外周の X座標
    double brx = rx + kOmargin;
    // 左外周の X座標
    double blx = lx - kImargin;
    // 上外周の Y座標
    double buy = uy + kHmargin;
    // 下外周の Y座標
    double bly = ly - kHmargin;
    
    // 周りの矩形の設定
    mInput->mBoundingBox.mLeft  = blx;
    mInput->mBoundingBox.mUpper = buy;
    mInput->mBoundingBox.mRight = brx;
    mInput->mBoundingBox.mLower = bly;
    
    // 入力ピン位置の設定
    mNot->mIpinLocations[0] = Point(blx, cy);
    
    // 出力ピン位置の設定
    mNot->mOpinLocations[0] = Point(brx, cy);
    
    // ディスプレイリストの生成
    mNot->mId = glGenLists(1);
    glNewList(mNot->mId, GL_COMPILE);

    // 配線の描画
    glLineWidth(wire_line_width());
    glColor4fv(wire_line_color());

    glBegin(GL_LINE_STRIP);
    glVertex2d(blx, cy);
    glVertex2d(lx, cy);
    glEnd();

    glBegin(GL_LINE_STRIP);
    glVertex2d(rx, cy);
    glVertex2d(brx, cy);
    glEnd();

    // ゲートの描画
    glLineWidth(gate_line_width());
    glColor4fv(gate_line_color());

    glBegin(GL_LINE_LOOP);
    glVertex2d(0.0, uy);
    glVertex2d(0.0, ly);
    glVertex2d(gx,  cy);
    glEnd();

    draw_dot(gx, 0.0);
    
    glEndList();
  }
  return mNot;
}

// @brie ANDゲートを表すテンプレートを返す．
// @param[in] ni 入力数
const GateTemplate*
LedScene::and_gate(size_t ni)
{
  size_t k = ni - 2;
  resize_array(mAndArray, k);
  if ( mAndArray[k] == NULL ) {
    GateTemplate* gate = new GateTemplate(ni, 1);
    mAndArray[k] = gate;

    // 左のX座標
    const double lx = 0.0;
    // 右のX座標
    const double rx = kGateW;
    // 上のY座標
    double uy;
    // 下のY座標
    double ly;
    size_t nseg = calc_points(ni, uy, ly);
    // 中心のY座標
    double oy = 0.0;

    // 右外周の X座標
    double brx = rx + kOmargin;
    // 左外周の X座標
    double blx = lx - kImargin;
    // 上外周の Y座標
    double buy = uy + kHmargin;
    // 下外周の Y座標
    double bly = ly - kHmargin;
    
    // 周りの矩形の設定
    gate->mBoundingBox.mLeft  = blx;
    gate->mBoundingBox.mUpper = buy;
    gate->mBoundingBox.mRight = brx;
    gate->mBoundingBox.mLower = bly;

    // 入力ピンの位置を設定する．
    // まず各セグメント内の入力数を決める．
    vector<size_t> npins(nseg);
    distribute_pins(ni, npins);
    vector<Point> pinloc(ni);
    size_t pos = 0;
    for (size_t i = 0; i < nseg; i ++) {
      size_t n = npins[i];
      double y = ly + kGateH * (i + 0.5);
      // pos 〜 pos + n - 1 までが i 番目のセグメント
      for (size_t j = 0; j < n; j ++) {
	pinloc[pos + j] = ipos(lx, y, j, n, false);
	gate->mIpinLocations[pos + j] = Point(blx, y);
      }
      pos += n;
    }

    // 出力ピンの位置を設定する．
    gate->mOpinLocations[0] = Point(brx, oy);

    // ディスプレイリストの生成
    gate->mId = glGenLists(1);
    glNewList(gate->mId, GL_COMPILE);

    // 配線の描画
    glLineWidth(wire_line_width());
    glColor4fv(wire_line_color());

    for (size_t i = 0; i < ni; ++ i) {
      glBegin(GL_LINE_STRIP);
      Point point = pinloc[i];
      glVertex2d(blx, point.mY);
      glVertex2d(point.mX, point.mY);
      glEnd();
    }

    glBegin(GL_LINE_STRIP);
    glVertex2d(rx, oy);
    glVertex2d(brx, oy);
    glEnd();

    // ゲートの描画
    glLineWidth(gate_line_width());
    glColor4fv(gate_line_color());

    draw_and(uy, ly);

    glEndList();
  }
  return mAndArray[k];
}

// @brief NANDゲートを表すテンプレートを返す．
// @param[in] ni 入力数
const GateTemplate*
LedScene::nand_gate(size_t ni)
{
  size_t k = ni - 2;
  resize_array(mNandArray, k);
  if ( mNandArray[k] == NULL ) {
    GateTemplate* gate = new GateTemplate(ni, 1);
    mNandArray[k] = gate;

    // 左のX座標
    const double lx = 0.0;
    // 右のX座標
    const double gx = kGateW;
    // 上のY座標
    double uy;
    // 下のY座標
    double ly;
    size_t nseg = calc_points(ni, uy, ly);
    // 中心のY座標
    double oy = 0.0;

    // ドットの右端のX座標
    double rx = gx + kDotW;

    // 右外周の X座標
    double brx = rx + kOmargin;
    // 左外周の X座標
    double blx = lx - kImargin;
    // 上外周の Y座標
    double buy = uy + kHmargin;
    // 下外周の Y座標
    double bly = ly - kHmargin;
    
    // 周りの矩形の設定
    gate->mBoundingBox.mLeft  = blx;
    gate->mBoundingBox.mUpper = buy;
    gate->mBoundingBox.mRight = brx;
    gate->mBoundingBox.mLower = bly;
    
    // 入力ピンの位置を設定する．
    // まず各セグメント内の入力数を決める．
    vector<size_t> npins(nseg);
    distribute_pins(ni, npins);
    vector<Point> pinloc(ni);
    size_t pos = 0;
    for (size_t i = 0; i < nseg; i ++) {
      size_t n = npins[i];
      double y = ly + kGateH * (i + 0.5);
      // pos 〜 pos + n - 1 までが i 番目のセグメント
      for (size_t j = 0; j < n; j ++) {
	pinloc[pos + j] = ipos(lx, y, j, n, false);
	gate->mIpinLocations[pos + j] = Point(blx, y);
      }
      pos += n;
    }
    
    // 出力ピンの位置を設定する．
    gate->mOpinLocations[0] = Point(brx, oy);

    // ディスプレイリストの生成
    gate->mId = glGenLists(1);
    glNewList(gate->mId, GL_COMPILE);

    // 配線の描画
    glLineWidth(wire_line_width());
    glColor4fv(wire_line_color());

    for (size_t i = 0; i < ni; ++ i) {
      Point point = pinloc[i];
      glBegin(GL_LINE_STRIP);
      glVertex2d(blx, point.mY);
      glVertex2d(point.mX, point.mY);
      glEnd();
    }

    glBegin(GL_LINE_STRIP);
    glVertex2d(rx, oy);
    glVertex2d(brx, oy);
    glEnd();

    // ゲートの描画
    glLineWidth(gate_line_width());
    glColor4fv(gate_line_color());

    draw_and(uy, ly);
    draw_dot(gx, 0.0);

    glEndList();
  }
  return mNandArray[k];
}

// @brief ORゲートを表すテンプレートを返す．
// @param[in] ni 入力数
const GateTemplate*
LedScene::or_gate(size_t ni)
{
  size_t k = ni - 2;
  resize_array(mOrArray, k);
  if ( mOrArray[k] == NULL ) {
    GateTemplate* gate = new GateTemplate(ni, 1);
    mOrArray[k] = gate;

    // 左のX座標
    const double lx = 0.0;
    // 右のX座標
    const double rx = kGateW;
    // 上のY座標
    double uy;
    // 下のY座標
    double ly;
    size_t nseg = calc_points(ni, uy, ly);
    // 中心のY座標
    double oy = 0.0;

    // 右外周の X座標
    double brx = rx + kOmargin;
    // 左外周の X座標
    double blx = lx - kImargin;
    // 上外周の Y座標
    double buy = uy + kHmargin;
    // 下外周の Y座標
    double bly = ly - kHmargin;
    
    // 周りの矩形の設定
    gate->mBoundingBox.mLeft  = blx;
    gate->mBoundingBox.mUpper = buy;
    gate->mBoundingBox.mRight = brx;
    gate->mBoundingBox.mLower = bly;
    
    // 入力ピンの位置を設定する．
    // まず各セグメント内の入力数を決める．
    vector<size_t> npins(nseg);
    distribute_pins(ni, npins);
    vector<Point> pinloc(ni);
    size_t pos = 0;
    for (size_t i = 0; i < nseg; i ++) {
      size_t n = npins[i];
      double y = ly + kGateH * (i + 0.5);
      // pos 〜 pos + n - 1 までが i 番目のセグメント
      for (size_t j = 0; j < n; j ++) {
	pinloc[pos + j] = ipos(lx, y, j, n, true);
	gate->mIpinLocations[pos + j] = Point(blx, y);
      }
      pos += n;
    }
    
    // 出力ピンの位置を設定する．
    gate->mOpinLocations[0] = Point(rx, oy);
    
    // ディスプレイリストの生成
    gate->mId = glGenLists(1);
    glNewList(gate->mId, GL_COMPILE);

    // 配線の描画
    glLineWidth(wire_line_width());
    glColor4fv(wire_line_color());

    for (size_t i = 0; i < ni; ++ i) {
      Point point = pinloc[i];
      glBegin(GL_LINE_STRIP);
      glVertex2d(blx, point.mY);
      glVertex2d(point.mX, point.mY);
      glEnd();
    }

    glBegin(GL_LINE_STRIP);
    glVertex2d(rx, oy);
    glVertex2d(brx, oy);
    glEnd();

    // ゲートの描画
    glLineWidth(gate_line_width());
    glColor4fv(gate_line_color());

    draw_or(uy, ly, false);

    glEndList();
  }
  return mOrArray[k];
}

// @brief NORゲートを表すテンプレートを返す．
// @param[in] ni 入力数
const GateTemplate*
LedScene::nor_gate(size_t ni)
{
  size_t k = ni - 2;
  resize_array(mNorArray, k);
  if ( mNorArray[k] == NULL ) {
    GateTemplate* gate = new GateTemplate(ni, 1);
    mNorArray[k] = gate;

    // 左のX座標
    double lx = 0.0;
    // 右のX座標
    double gx = kGateW;
    // 上のY座標
    double uy;
    // 下のY座標
    double ly;
    size_t nseg = calc_points(ni, uy, ly);
    // 中心のY座標
    double oy = 0.0;

    // ドットの右端のX座標
    double rx = gx + kDotW;

    // 右外周の X座標
    double brx = rx + kOmargin;
    // 左外周の X座標
    double blx = lx - kImargin;
    // 上外周の Y座標
    double buy = uy + kHmargin;
    // 下外周の Y座標
    double bly = ly - kHmargin;
    
    // 周りの矩形の設定
    gate->mBoundingBox.mLeft  = blx;
    gate->mBoundingBox.mUpper = buy;
    gate->mBoundingBox.mRight = brx;
    gate->mBoundingBox.mLower = bly;

    // 入力ピンの位置を設定する．
    // まず各セグメント内の入力数を決める．
    vector<size_t> npins(nseg);
    distribute_pins(ni, npins);
    vector<Point> pinloc(ni);
    size_t pos = 0;
    for (size_t i = 0; i < nseg; i ++) {
      size_t n = npins[i];
      double y = ly + kGateH * (i + 0.5);
      // pos 〜 pos + n - 1 までが i 番目のセグメント
      for (size_t j = 0; j < n; j ++) {
	pinloc[pos + j] = ipos(lx, y, j, n, true);
	gate->mIpinLocations[pos + j] = Point(blx, y);
      }
      pos += n;
    }
    
    // 出力ピンの位置を設定する．
    gate->mOpinLocations[0] = Point(brx, 0.0);

    // ディスプレイリストの生成
    gate->mId = glGenLists(1);
    glNewList(gate->mId, GL_COMPILE);

    // 配線の描画
    glLineWidth(wire_line_width());
    glColor4fv(wire_line_color());

    for (size_t i = 0; i < ni; ++ i) {
      Point point = pinloc[i];
      glBegin(GL_LINE_STRIP);
      glVertex2d(blx, point.mY);
      glVertex2d(point.mX, point.mY);
      glEnd();
    }

    glBegin(GL_LINE_STRIP);
    glVertex2d(rx, oy);
    glVertex2d(brx, oy);
    glEnd();

    // ゲートの描画
    glLineWidth(gate_line_width());
    glColor4fv(gate_line_color());

    draw_or(uy, ly, false);
    draw_dot(gx, 0.0);

    glEndList();
  }
  return mNorArray[k];
}

// @brief XORゲートを表すテンプレートを返す．
// @param[in] ni 入力数
const GateTemplate*
LedScene::xor_gate(size_t ni)
{
  size_t k = ni - 2;
  resize_array(mXorArray, k);
  if ( mXorArray[k] == NULL ) {
    GateTemplate* gate = new GateTemplate(ni, 1);
    mXorArray[k] = gate;

    // 左のX座標
    double lx = 0.0;
    // 右のX座標
    double rx = kGateW;
    // 上のY座標
    double uy;
    // 下のY座標
    double ly;
    size_t nseg = calc_points(ni, uy, ly);
    // 中心のY座標
    double oy = 0.0;

    // 右外周の X座標
    double brx = rx + kOmargin;
    // 左外周の X座標
    double blx = lx - kImargin;
    // 上外周の Y座標
    double buy = uy + kHmargin;
    // 下外周の Y座標
    double bly = ly - kHmargin;
    
    // 周りの矩形の設定
    gate->mBoundingBox.mLeft  = blx;
    gate->mBoundingBox.mUpper = buy;
    gate->mBoundingBox.mRight = brx;
    gate->mBoundingBox.mLower = bly;
    
    // 入力ピンの位置を設定する．
    // まず各セグメント内の入力数を決める．
    vector<size_t> npins(nseg);
    distribute_pins(ni, npins);
    vector<Point> pinloc(ni);
    size_t pos = 0;
    for (size_t i = 0; i < nseg; i ++) {
      size_t n = npins[i];
      double y = ly + kGateH * (i + 0.5);
      // pos 〜 pos + n - 1 までが i 番目のセグメント
      for (size_t j = 0; j < n; j ++) {
	pinloc[pos + j] = ipos(-kXorM, y, j, n, true);
	gate->mIpinLocations[pos + j] = Point(blx, y);
      }
      pos += n;
    }
    
    // 出力ピンの位置を設定する．
    gate->mOpinLocations[0] = Point(brx, oy);
    
    // ディスプレイリストの生成
    gate->mId = glGenLists(1);
    glNewList(gate->mId, GL_COMPILE);

    // 配線の描画
    glLineWidth(wire_line_width());
    glColor4fv(wire_line_color());

    for (size_t i = 0; i < ni; ++ i) {
      Point point = pinloc[i];
      glBegin(GL_LINE_STRIP);
      glVertex2d(blx, point.mY);
      glVertex2d(point.mX, point.mY);
      glEnd();
    }

    glBegin(GL_LINE_STRIP);
    glVertex2d(rx, oy);
    glVertex2d(brx, oy);
    glEnd();

    // ゲートの描画
    glLineWidth(gate_line_width());
    glColor4fv(gate_line_color());

    draw_or(uy, ly, true);

    glEndList();
  }
  return mXorArray[k];
}

// @brief XNORゲートを表すテンプレートを返す．
// @param[in] ni 入力数
const GateTemplate*
LedScene::xnor_gate(size_t ni)
{
  size_t k = ni - 2;
  resize_array(mXnorArray, k);
  if ( mXnorArray[k] == NULL ) {
    GateTemplate* gate = new GateTemplate(ni, 1);
    mXnorArray[k] = gate;

    // 左のX座標
    double lx = 0.0;
    // 右のX座標
    double gx = kGateW;
    // 上のY座標
    double uy;
    // 下のY座標
    double ly;
    size_t nseg = calc_points(ni, uy, ly);
    // 中心のY座標
    double oy = 0.0;

    // ドットの右端のX座標
    double rx = gx + kDotW;

    // 右外周の X座標
    double brx = rx + kOmargin;
    // 左外周の X座標
    double blx = lx - kImargin;
    // 上外周の Y座標
    double buy = uy + kHmargin;
    // 下外周の Y座標
    double bly = ly - kHmargin;
    
    // 周りの矩形の設定
    gate->mBoundingBox.mLeft  = blx;
    gate->mBoundingBox.mUpper = buy;
    gate->mBoundingBox.mRight = brx;
    gate->mBoundingBox.mLower = bly;

    // 入力ピンの位置を設定する．
    // まず各セグメント内の入力数を決める．
    vector<size_t> npins(nseg);
    distribute_pins(ni, npins);
    vector<Point> pinloc(ni);
    size_t pos = 0;
    for (size_t i = 0; i < nseg; i ++) {
      size_t n = npins[i];
      double y = ly + kGateH * (i + 0.5);
      // pos 〜 pos + n - 1 までが i 番目のセグメント
      for (size_t j = 0; j < n; j ++) {
	pinloc[pos + j] = ipos(-kXorM, y, j, n, true);
	gate->mIpinLocations[pos + j] = Point(blx, y);
      }
      pos += n;
    }

    // 出力ピンの位置を設定する．
    gate->mOpinLocations[0] = Point(rx, oy);
    
    // ディスプレイリストの生成
    gate->mId = glGenLists(1);
    glNewList(gate->mId, GL_COMPILE);

    // 配線の描画
    glLineWidth(wire_line_width());
    glColor4fv(wire_line_color());

    for (size_t i = 0; i < ni; ++ i) {
      Point point = pinloc[i];
      glBegin(GL_LINE_STRIP);
      glVertex2d(blx, point.mY);
      glVertex2d(point.mX, point.mY);
      glEnd();
    }

    glBegin(GL_LINE_STRIP);
    glVertex2d(rx, oy);
    glVertex2d(brx, oy);
    glEnd();

    // ゲートの描画
    glLineWidth(gate_line_width());
    glColor4fv(gate_line_color());

    draw_or(uy, ly, true);
    draw_dot(gx, 0.0);

    glEndList();
  }
  return mXnorArray[k];
}

// ゲート描画用の線幅を得る．
GLfloat
LedScene::gate_line_width()
{
  return mGateLineWidth;
}

// @brief ゲート描画用の色を得る．
// @return RGBA の4つの値を持つ配列の先頭アドレスと返す．
const GLclampf*
LedScene::gate_line_color()
{
  return mGateLineColor;
}

// 配線描画用の線幅を得る．
GLfloat
LedScene::wire_line_width()
{
  return mWireLineWidth;
}

// @brief 配線描画用の色を得る．
// @return RGBA の4つの値を持つ配列の先頭アドレスと返す．
const GLclampf*
LedScene::wire_line_color()
{
  return mWireLineColor;
}

// 枠線描画用の線幅を得る．
GLfloat
LedScene::bbox_line_width()
{
  return mBBoxLineWidth;
}

// @brief 枠線描画用の色を得る．
// @return RGBA の4つの値を持つ配列の先頭アドレスと返す．
const GLclampf*
LedScene::bbox_line_color()
{
  return mBBoxLineColor;
}

// 配線の X 軸方向のマージンを得る．
double
LedScene::wire_x_margin()
{
  return kWireXmargin;
}

// 配線の Y 軸方向のマージンを得る．
double
LedScene::wire_y_margin()
{
  return kWireYmargin;
}

// @brief 描画オブジェクトを追加する．
// @param[in] gt ゲートテンプレート
// @param[in] point 位置
// @return オブジェクトを返す．
GateObj*
LedScene::new_obj(const GateTemplate* gt,
		  const Point& point)
{
  GateObj* obj = new GateObj(gt, point);
  reg_obj(obj);
  return obj;
}

// @brief 描画オブジェクトを追加する．
// @param[in] gt ゲートテンプレート
// @param[in] x 位置のX座標
// @param[in] y 位置のY座標
// @return オブジェクト を返す．
GateObj*
LedScene::new_obj(const GateTemplate* gt,
		  GLdouble x,
		  GLdouble y)
{
  GateObj* obj = new GateObj(gt, x, y);
  reg_obj(obj);
  return obj;
}

// @brief 描画オブジェクトを削除する．
// @param[in] id オブジェクトID
// @note 該当のオブジェクトがなければなにもしない．
void
LedScene::delete_obj(size_t id)
{
  if ( mObjArray.size() > id ) {
    GateObj* obj = mObjArray[id];
    if ( obj ) {
      del_obj(obj);
    }
  }
}

/// @brief 拡大率を設定する．
void
LedScene::set_zoom(GLdouble zoom)
{
  mZoom = zoom;
  resize();
  invalidate();
}

// @brief 拡大率を得る．
GLdouble
LedScene::zoom() const
{
  return mZoom;
}

// @brief 表示領域の左下の座標を設定する．
void
LedScene::set_corner(const Point& point)
{
  mCorner = point;
  resize();
  invalidate();
}

// @brief 表示領域の左下の座標を設定する．
void
LedScene::set_corner(GLdouble x,
		     GLdouble y)
{
  mCorner.mX = x;
  mCorner.mY = y;
  resize();
  invalidate();
}

// @brief 表示領域の左下の座標を得る．
Point
LedScene::corner() const
{
  return mCorner;
}

// @brief 枠線の表示を行う．
void
LedScene::enable_bbox()
{
  mDrawBBox = true;
  invalidate();
}

// @brief 枠線の表示を行わない．
void
LedScene::disable_bbox()
{
  mDrawBBox = false;
  invalidate();
}

// @brief 枠線の表示モードを得る．
// @retval true 枠線の表示を行う．
// @retval false 枠線の表示を行わない．
bool
LedScene::bbox_mode() const
{
  return mDrawBBox;
}

// GateObj を登録する．
void
LedScene::reg_obj(GateObj* obj)
{
  size_t id = mObjItvl.avail_num();
  mObjItvl.erase(id);
  while ( mObjArray.size() <= id ) {
    mObjArray.push_back(NULL);
  }
  mObjArray[id] = obj;
  mObjList.push_back(obj);
  obj->mObjId = id;
}

// GateObj を削除する．
void
LedScene::del_obj(GateObj* obj)
{
  mObjItvl.add(obj->mObjId);
  mObjArray[obj->mObjId] = NULL;
  mObjList.erase(obj);
}

// GateObj を描画する．
void
LedScene::draw_obj(GateObj* obj)
{
  glPushMatrix();
  Point point = obj->location();
  glTranslatef(point.mX, point.mY, 0.0);
  glCallList(obj->id());

  // 枠線の描画
  if ( mDrawBBox ) {
    DiagPoints dp = obj->mGateTemplate->bounding_box();
    double rx = dp.mRight;
    double uy = dp.mUpper;
    double lx = dp.mLeft;
    double ly = dp.mLower;
  
    glLineWidth(bbox_line_width());
    glColor4fv(bbox_line_color());

    glEnable(GL_LINE_STIPPLE);
    glPushAttrib(GL_LINE_BIT);
    glLineStipple(2, 0xaaaa);
  
    glBegin(GL_LINE_LOOP);
    glVertex2d(rx, uy);
    glVertex2d(lx, uy);
    glVertex2d(lx, ly);
    glVertex2d(rx, ly);
    glEnd();

    glPopAttrib();
    glDisable(GL_LINE_STIPPLE);
  }
  
  glPopMatrix();
}


#if 0
// コンストラクタその1
GateTemplate::GateTemplate(tGateType type, size_t ni)
{
  create(type, vector<tPol>(ni, kPolPosi));
}

// コンストラクタその2
GateTemplate::GateTemplate(tGateType type, const vector<tPol>& ipols)
{
  create(type, ipols);
}

// コンストラクタその1とその2の下請関数
void
GateTemplate::create(tGateType type, const vector<tPol>& ipols)
{
  mNi = ipols.size();
  mHasOpin = true;

  // ゲートの形を作る．
  bool need_odot = false;
  switch (type) {
  case kWIRE:
    mNi = 1;
    create_wire();
    break;
  case kINPUT:
    mNi = 0;
    create_input();
    break;
  case kOUTPUT:
    mNi = 1;
    mHasOpin = false;
    create_output();
    break;
  case kNOT:
    need_odot = true;
    // break はわざと書かない．
  case kBUF:
    mNi = 1;
    create_buf();
    break;
  case kNAND:
    need_odot = true;
    // break はわざと書かない．
  case kAND:
    create_and();
    break;
  case kNOR:
    need_odot = true;
    // break はわざと書かない．
  case kOR:
    create_or(false);
    break;
  case kXNOR:
    need_odot = true;
    // break はわざと書かない．
  case kXOR:
    create_or(true);
    break;
  }

  // 出力が負極性の場合にはドットを作る．
  if ( need_odot ) {
    double ox0 = mOpinLocation.get_x();
    double oy0 = mOpinLocation.get_y();
    double ox1 = ox0 + kDotW;
    double oy1 = oy0 - (kDotH / 2.0);
    double oy2 = oy0 + (kDotH / 2.0);
    mEllipseTemplates.push_back(DiagPoints(ox0, oy1, ox1, oy2));
    // 出力ピンの位置を直しておく
    mOpinLocation.set_x(ox1);
  }

  // 入力が負極性の場合にはドットを作る．
  for (size_t i = 0; i < mNi; i ++) {
    if ( ipols[i] == kPolNega ) {
      const Point& point = mIpinLocations[i];
      double ix0 = point.get_x();
      double iy0 = point.get_y();
      double ix1 = ix0 - kDotW;
      double iy1 = iy0 - (kDotH / 2.0);
      double iy2 = iy0 + (kDotH / 2.0);
      mEllipseTemplates.push_back(DiagPoints(ix1, iy1, ix0, iy2));
      // 入力ピンの位置を直しておく
      mIpinLocations[i].set_x(ix1);
    }
  }

  common_afterproc();
}

// コンストラクタその3
GateTemplate::GateTemplate(const vector<GateTemplate*>& nodes)
{
  // GateTemplate の木を作る．
  vector<GateTemplate*>::const_iterator p = nodes.begin();
  GtTree* root = GtTree::vector2tree(p);

  double w = root->place(0.0);
  mUy = - w / 2.0;
  mLy =   w / 2.0;
  root->move(Point(0.0, mUy));

  mOpinLocation = root->opin_location();
  mRx = root->gate_template()->mRx + root->pos0().get_x();
  copy_from_tree(root);
  mNi = mIpinLocations.size();

  delete root;

  common_afterproc();
}

// デストラクタ
GateTemplate::~GateTemplate()
{
}

// 配線用のダミーゲートを作る．
void
GateTemplate::create_wire()
{
  mRx = kWireW;
  mUy = - (kWireH / 2.0);
  mLy =   (kWireH / 2.0);

  mOpinLocation = Point(mRx, 0.0);
  mIpinLocations = vector<Point>(1, Point(0.0, 0.0));

  Points points;
  points.push_back(Point(0.0, 0.0));
  points.push_back(Point(mRx, 0.0));
  mWireTemplates.push_back(points);
}

// 入力ピンを作る．
void
GateTemplate::create_input()
{
  mRx = kInputW;
  mUy = - (kInputH / 2.0);
  mLy =   (kInputH / 2.0);

  mOpinLocation = Point(mRx, 0.0);
  mIpinLocations = vector<Point>(0);

  double mid_x = mRx - (kInputH / 2.0);
  Points points;
  points.push_back(Point(0.0, mUy));
  points.push_back(Point(mid_x, mUy));
  points.push_back(Point(mRx, 0.0));
  points.push_back(Point(mid_x, mLy));
  points.push_back(Point(0.0, mLy));
  points.push_back(Point(0.0, mUy));
  mPolygonTemplates.push_back(points);
}

// 出力ピンを作る．
void
GateTemplate::create_output()
{
  mRx = kOutputW;
  mUy = - (kOutputH / 2.0);
  mLy =   (kOutputH / 2.0);

  mOpinLocation = Point(mRx, 0.0);
  mIpinLocations = vector<Point>(1);

  double mid_x = mRx - (kInputH / 2.0);
  Points points;
  points.push_back(Point(0.0, mUy));
  points.push_back(Point(mid_x, mUy));
  points.push_back(Point(mRx, 0.0));
  points.push_back(Point(mid_x, mLy));
  points.push_back(Point(0.0, mLy));
  points.push_back(Point(0.0, mUy));
  mPolygonTemplates.push_back(points);
}

// バッファゲートを作る．
void
GateTemplate::create_buf()
{
  mRx = kBufW;
  mUy =  - (kBufH / 2.0);
  mLy =    (kBufH / 2.0);

  Points points;
  points.push_back(Point(0.0, mUy));
  points.push_back(Point(0.0, mLy));
  points.push_back(Point(mRx, 0.0));
  points.push_back(Point(0.0, mUy));
  mPolygonTemplates.push_back(points);

  mOpinLocation = Point(mRx, 0.0);
  mIpinLocations = vector<Point>(1, Point(0.0, 0.0));
}

// AND/OR/XOR ゲートの時の座標を計算する．
// 入力辺のサイズを返す．
size_t
GateTemplate::calc_points()
{
  size_t nseg = (mNi + 4) / 5;
  if ( nseg == 2 ) {
    nseg = 3;
  }
  size_t nseg_u = nseg / 2;
  size_t nseg_l = (nseg - 1) / 2;
  mUy    = kGateUY - (kGateH * nseg_u);
  mLy    = kGateLY + (kGateH * nseg_l);
  mRx    = kGateW;
  mOpinLocation = Point(mRx, 0.0);
  return nseg;
}

// ANDゲートを作る．
void
GateTemplate::create_and()
{
  size_t nseg = calc_points();

  Points points;
  points.push_back(Point(0.0, kGateUY));
  points.push_back(Point(kAndL, kGateUY));
  create_arc(kAndL, 0.0, kAndR, 12, 1.5, 2.5, points);
  points.push_back(Point(kAndL, kGateLY));
  points.push_back(Point(0.0, kGateLY));
  points.push_back(Point(0.0, kGateUY));
  mPolygonTemplates.push_back(points);

  // 上側に延ばす辺
  if ( mUy != kGateUY ) {
    Points points;
    points.push_back(Point(0.0, mUy));
    points.push_back(Point(0.0, kGateUY));
    mLineTemplates.push_back(points);
  }

  // 下側に延ばす辺
  if ( mLy != kGateLY ) {
    Points points;
    points.push_back(Point(0.0, kGateLY));
    points.push_back(Point(0.0, mLy));
    mLineTemplates.push_back(points);
  }

  // 入力ピンの位置を設定する．
  mIpinLocations = vector<Point>(mNi);

  // まず各セグメント内の入力数を決める．
  vector<size_t> npins(nseg);
  distribute_pins(npins, mNi);
  size_t pos = 0;
  for (size_t i = 0; i < nseg; i ++) {
    size_t n = npins[i];
    double cy = mUy + kGateH * (i + 0.5);
    // pos 〜 pos + n - 1 までが i 番目のセグメント
    for (size_t j = 0; j < n; j ++) {
      mIpinLocations[pos + j] = ipos(0.0, cy, j, n, false);
    }
    pos += n;
  }
}

// OR/XOR ゲートの入力側の弧を描く関数
// 入力数が3より大きいときに対応している．
void
GateTemplate::make_arc(double x0)
{
  const size_t ndiv = 8;
  double cx = x0 - (kOrR / 2.0) * sqrt3;

  // 上側に延ばす辺
  if ( mUy != kGateUY ) {
    Points points;
    for (double sy = mUy; sy != kGateUY; sy += kGateH ) {
      points.push_back(Point(x0, sy));
      double cy = sy + (kGateH / 2.0);
      create_arc(cx, cy, kXorR, ndiv, - 1.0 / 6.0, 1.0 / 6.0, points);
    }
    points.push_back(Point(x0, kGateUY));
    mLineTemplates.push_back(points);
  }

  // 下側に延ばす辺
  if ( mLy != kGateLY ) {
    Points points;
    for (double sy = kGateLY; sy != mLy; sy += kGateH ) {
      points.push_back(Point(x0, sy));
      double cy = sy + (kGateH / 2.0);
      create_arc(cx, cy, kXorR, ndiv, - 1.0 / 6.0, 1.0 / 6.0, points);
    }
    points.push_back(Point(x0, mLy));
    mLineTemplates.push_back(points);
  }
}

// ORゲートを作る．
void
GateTemplate::create_or(bool xor)
{
  size_t nseg = calc_points();

  size_t ndiv = 8;

  Points points;
  points.push_back(Point(0.0, kGateUY));
  points.push_back(Point(kOrL, kGateUY));
  create_arc(kOrL, kGateLY, kOrR, ndiv, 9.0 / 6.0, 11.0 / 6.0, points);
  points.push_back(Point(mRx, 0.0));
  create_arc(kOrL, kGateUY, kOrR, ndiv, 1.0 / 6.0, 3.0 / 6.0, points);
  points.push_back(Point(kOrL, kGateLY));
  points.push_back(Point(0.0, kGateLY));
  double cx = - (kOrR / 2.0) * sqrt3;
  create_arc(cx, 0.0, kOrR, ndiv, 1.0 / 6.0, -1.0 / 6.0, points);
  points.push_back(Point(0.0, kGateUY));
  mPolygonTemplates.push_back(points);
  
  make_arc(0.0);

  if ( xor ) {
    Points points;
    points.push_back(Point(- kXorM, kGateUY));
    create_arc(cx - kXorM, 0.0, kXorR, ndiv, -1.0 / 6.0, 1.0 / 6.0, points);
    points.push_back(Point(- kXorM, kGateLY));
    mLineTemplates.push_back(points);

    make_arc(-kXorM);
  }

  // 入力ピンの位置を設定する．
  mIpinLocations = vector<Point>(mNi);

  // まず各セグメント内の入力数を決める．
  vector<size_t> npins(nseg);
  distribute_pins(npins, mNi);
  size_t pos = 0;
  double x_offset = xor ? -kXorM : 0.0;
  for (size_t i = 0; i < nseg; i ++) {
    size_t n = npins[i];
    double cy = mUy + kGateH * (i + 0.5);
    // pos 〜 pos + n - 1 までが i 番目のセグメント
    for (size_t j = 0; j < n; j ++) {
      mIpinLocations[pos + j] = ipos(x_offset, cy, j, n, true);
    }
    pos += n;
  }
}

// すべてのコンストラクタで共通な後処理
void
GateTemplate::common_afterproc()
{
  // 出力側の配線を作る．
  if ( mHasOpin ) {
    double ox0 = mOpinLocation.get_x();
    double oy0 = mOpinLocation.get_y();
    double ox1 = mRx + kOmargin;
    Points points;
    points.push_back(Point(ox0, oy0));
    points.push_back(Point(ox1, oy0));
    mWireTemplates.push_back(points);
    // 以降は配線の開いた端を出力ピンとみなす．
    mOpinLocation.set_x(ox1);
  }
  // 入力側の配線を作る．
  for (size_t i = 0; i < mNi; i ++) {
    const Point& point = mIpinLocations[i];
    double ix0 = point.get_x();
    double iy0 = point.get_y();
    double ix1 = -kImargin;
    Points points;
    points.push_back(Point(ix1, iy0));
    points.push_back(Point(ix0, iy0));
    mWireTemplates.push_back(points);
    // 以降は配線の開いた端を入力ピンとみなす．
    mIpinLocations[i].set_x(ix1);
  }

  mGateLineWidth = kGateLineWidth;
  mWireLineWidth = kWireLineWidth;
  mBboxLineWidth = kBboxLineWidth;
}

// GtTree の各要素の GateTemplate の内容をコピーする．
void
GateTemplate::copy_from_tree(GtTree* node)
{
  GateTemplate* templ = node->gate_template();
  size_t ni = templ->ni();
  if ( node->is_leaf_node() ) {
    for (size_t i = 0; i < ni; i ++) {
      mIpinLocations.push_back(node->ipin_location(i));
    }
  } else {
    vector<double> src_ypos(ni);
    vector<double> dst_ypos(ni);
    for (size_t i = 0; i < ni; i ++) {
      GtTree* chd = node->child(i);
      // 子供に対して再帰的に copy_from_tree を呼ぶ．
      copy_from_tree(chd);
      // 子供の出力ピンと自分の入力ピンの間の配線を作る．
      Point src_point = chd->opin_location();
      Point dst_point = node->ipin_location(i);
      size_t t = node->track_assign(i);
      double tx = dst_point.get_x() - (t * wire_x_margin());
      Points points;
      points.push_back(src_point);
      points.push_back(Point(tx, src_point.get_y()));
      points.push_back(Point(tx, dst_point.get_y()));
      points.push_back(dst_point);
      mWireTemplates.push_back(points);
    }
  }
  Point offset = node->pos0();
  copy_with_offset(templ->mRectTemplates, mRectTemplates, offset);
  copy_with_offset(templ->mEllipseTemplates, mEllipseTemplates, offset);
  copy_with_offset(templ->mPolygonTemplates, mPolygonTemplates, offset);
  copy_with_offset(templ->mLineTemplates, mLineTemplates, offset);
  copy_with_offset(templ->mWireTemplates, mWireTemplates, offset);
}

// DiagPoints のリストをオフセット付きでコピーする
void
GateTemplate::copy_with_offset(const list<DiagPoints>& src,
				     list<DiagPoints>& dst,
				     const Point& offset)
{
  double x_offset = offset.get_x();
  double y_offset = offset.get_y();
  for (list<DiagPoints>::const_iterator p = src.begin();
       p != src.end(); ++ p) {
    const DiagPoints& src_point = *p;
    double x0 = src_point.mX0 + x_offset;
    double y0 = src_point.mY0 + y_offset;
    double x1 = src_point.mX1 + x_offset;
    double y1 = src_point.mY1 + y_offset;
    dst.push_back(DiagPoints(x0, y0, x1, y1));
  }
}

// Points のリストをオフセット付きでコピーする
void
GateTemplate::copy_with_offset(const list<Points>& src,
				     list<Points>& dst,
				     const Point& offset)
{
  for (list<Points>::const_iterator p = src.begin();
       p != src.end(); ++ p) {
    const Points& src_points = *p;
    Points dst_points;
    for (Points::const_iterator q = src_points.begin();
	 q != src_points.end(); ++ q) {
      Point dst_point(*q);
      dst_point += offset;
      dst_points.push_back(dst_point);
    }
    dst.push_back(dst_points);
  }
}

// 入力数を得る．
size_t
GateTemplate::ni() const
{
  return mNi;
}

// 矩形を生成するための元データのリストを得る．
const list<DiagPoints>&
GateTemplate::rect_srcs() const
{
  return mRectTemplates;
}

// 楕円を生成するための元データのリストを得る．
const list<DiagPoints>&
GateTemplate::ellipse_srcs() const
{
  return mEllipseTemplates;
}

// 多角形を生成するための元データのリストを得る．
const list<Points>&
GateTemplate::polygon_srcs() const
{
  return mPolygonTemplates;
}

// 線分を生成するための元データのリストを得る．
const list<Points>&
GateTemplate::line_srcs() const
{
  return mLineTemplates;
}

// 配線用線分を生成するための元データのリストを得る．
const list<Points>&
GateTemplate::wire_srcs() const
{
  return mWireTemplates;
}

// このゲートを囲む最小の矩形を表す左上と右下の点を得る．
DiagPoints
GateTemplate::bounding_box() const
{
  return DiagPoints(-kImargin, mUy - kHmargin, mRx + kOmargin, mLy + kHmargin);
}

// すべての入力ピン位置を得る．
const vector<Point>&
GateTemplate::ipin_locations() const
{
  return mIpinLocations;
}

// pos 番目の入力ピン位置を得る．
Point
GateTemplate::ipin_location(size_t pos) const
{
  return mIpinLocations[pos];
}

// 出力ピン位置を得る．
Point
GateTemplate::opin_location() const
{
  return mOpinLocation;
}

// ゲート描画用の線幅を得る．
double
GateTemplate::gate_line_width() const
{
  return mGateLineWidth;
}

// 配線描画用の線幅を得る．
double
GateTemplate::wire_line_width() const
{
  return mWireLineWidth;
}

// 枠線描画用の線幅を得る．
double
GateTemplate::bbox_line_width() const
{
  return mBboxLineWidth;
}

// 配線の X 軸方向のマージンを得る．
double
GateTemplate::wire_x_margin()
{
  return kWireXmargin;
}

// 配線の Y 軸方向のマージンを得る．
double
GateTemplate::wire_y_margin()
{
  return kWireYmargin;
}
#endif

END_NAMESPACE_YM_GLV
