#ifndef LED_LEDSCENE_H
#define LED_LEDSCENE_H

/// @file led/LedScene.h
/// @brief LedScene のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
//
/// $Id: LedScene.h 347 2007-01-16 02:37:45Z matsunaga $
///
/// Copyright(C) 2002 by Yusuke Matsunaga


#include <ym_utils/DlList.h>
#include <ym_utils/ItvlMgr.h>
#include "GlvScene.h"
#include "GateObj.h"


BEGIN_NAMESPACE_YM_GLV

class GateTemplate;
class GateObj;

//////////////////////////////////////////////////////////////////////
/// @class LedScene LedScene.h "LedScene.h"
/// @brief ゲート描画用の drawing area
//////////////////////////////////////////////////////////////////////
class LedScene :
  public GlvScene
{
public:

  /// @brief コンストラクタ
  LedScene();

  /// @brief デストラクタ
  ~LedScene();


public:
  //////////////////////////////////////////////////////////////////////
  // 具体的なゲートのテンプレートを生成する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 入力を表すテンプレートを返す．
  const GateTemplate* input();

  /// @brief 出力を表すテンプレートを返す．
  const GateTemplate* output();

  /// @brief バッファを表すテンプレートを返す．
  const GateTemplate* buf_gate();

  /// @brief NOTゲートを表すテンプレートを返す．
  const GateTemplate* not_gate();

  /// @brie ANDゲートを表すテンプレートを返す．
  /// @param[in] ni 入力数
  const GateTemplate* and_gate(size_t ni);

  /// @brief NANDゲートを表すテンプレートを返す．
  /// @param[in] ni 入力数
  const GateTemplate* nand_gate(size_t ni);

  /// @brief ORゲートを表すテンプレートを返す．
  /// @param[in] ni 入力数
  const GateTemplate* or_gate(size_t ni);

  /// @brief NORゲートを表すテンプレートを返す．
  /// @param[in] ni 入力数
  const GateTemplate* nor_gate(size_t ni);

  /// @brief XORゲートを表すテンプレートを返す．
  /// @param[in] ni 入力数
  const GateTemplate* xor_gate(size_t ni);

  /// @brief XNORゲートを表すテンプレートを返す．
  /// @param[in] ni 入力数
  const GateTemplate* xnor_gate(size_t ni);


public:
  //////////////////////////////////////////////////////////////////////
  // 描画オブジェクトを管理する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 描画オブジェクトを追加する．
  /// @param[in] gt ゲートテンプレート
  /// @param[in] point 位置
  /// @return オブジェクトを返す．
  GateObj* new_obj(const GateTemplate* gt,
		   const Point& point);

  /// @brief 描画オブジェクトを追加する．
  /// @param[in] gt ゲートテンプレート
  /// @param[in] x 位置のX座標
  /// @param[in] y 位置のY座標
  /// @return オブジェクトを返す．
  GateObj* new_obj(const GateTemplate* gt,
		   GLdouble x,
		   GLdouble y);

  /// @brief 描画オブジェクトを削除する．
  /// @param[in] id オブジェクトID
  /// @note 該当のオブジェクトがなければなにもしない．
  void delete_obj(size_t id);


public:
  //////////////////////////////////////////////////////////////////////
  // 描画範囲に関する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 拡大率を設定する．
  void set_zoom(GLdouble zoom);

  /// @brief 拡大率を得る．
  GLdouble zoom() const;

  /// @brief 表示領域の左下の座標を設定する．
  void set_corner(const Point& point);

  /// @brief 表示領域の左下の座標を設定する．
  void set_corner(GLdouble x,
		  GLdouble y);

  /// @brief 表示領域の左下の座標を得る．
  Point corner() const;

  /// @brief 枠線の表示を行う．
  void enable_bbox();

  /// @brief 枠線の表示を行わない．
  void disable_bbox();

  /// @brief 枠線の表示モードを得る．
  /// @retval true 枠線の表示を行う．
  /// @retval false 枠線の表示を行わない．
  bool bbox_mode() const;


public:
  //////////////////////////////////////////////////////////////////////
  // オブジェクトの描画用属性に関する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ゲート描画用の線幅を得る．
  GLfloat gate_line_width();

  /// @brief ゲート描画用の色を得る．
  /// @return RGBA の4つの値を持つ配列の先頭アドレスと返す．
  const GLclampf* gate_line_color();
  
  /// @brief 配線描画用の線幅を得る．
  GLfloat wire_line_width();

  /// @brief 配線描画用の色を得る．
  /// @return RGBA の4つの値を持つ配列の先頭アドレスと返す．
  const GLclampf* wire_line_color();

  /// @brief 枠線描画用の線幅を得る．
  GLfloat bbox_line_width();

  /// @brief 枠線描画用の色を得る．
  /// @return RGBA の4つの値を持つ配列の先頭アドレスと返す．
  const GLclampf* bbox_line_color();

  /// @brief 配線の X 軸方向のマージンを得る．
  GLdouble wire_x_margin();

  /// @brief 配線の Y 軸方向のマージンを得る．
  GLdouble wire_y_margin();
  
  
protected:
  //////////////////////////////////////////////////////////////////////
  // GlvScene の継承クラスが実装しなければならない仮想関数
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


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  // GateObj を登録する．
  void reg_obj(GateObj* obj);

  // GateObj を削除する．
  void del_obj(GateObj* obj);

  // GateObj を描画する．
  void draw_obj(GateObj* obj);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 入力を表すテンプレート
  GateTemplate* mInput;

  // 出力を表すテンプレート
  GateTemplate* mOutput;

  // バッファを表すテンプレート
  GateTemplate* mBuf;

  // NOT ゲートを表すテンプレート
  GateTemplate* mNot;

  // AND ゲートを表すテンプレート
  // 入力数 - 2 の位置に格納する．
  vector<GateTemplate*> mAndArray;

  // NAND ゲートを表すテンプレート
  // 入力数 - 2 の位置に格納する．
  vector<GateTemplate*> mNandArray;

  // OR ゲートを表すテンプレート
  // 入力数 - 2 の位置に格納する．
  vector<GateTemplate*> mOrArray;

  // NOR ゲートを表すテンプレート
  // 入力数 - 2 の位置に格納する．
  vector<GateTemplate*> mNorArray;

  // XOR ゲートを表すテンプレート
  // 入力数 - 2 の位置に格納する．
  vector<GateTemplate*> mXorArray;

  // XNOR ゲートを表すテンプレート
  // 入力数 - 2 の位置に格納する．
  vector<GateTemplate*> mXnorArray;

  // ゲート描画用の線幅
  GLfloat mGateLineWidth;

  // ゲート描画用の色
  GLclampf mGateLineColor[4];
  
  // 配線描画用の線幅
  GLfloat mWireLineWidth;

  // 配線描画用の色
  GLclampf mWireLineColor[4];

  // 枠線描画用の線幅
  GLfloat mBBoxLineWidth;

  // 枠線描画用の色
  GLclampf mBBoxLineColor[4];

  // 配線の X軸方向のマージン
  GLdouble mWireXMargin;

  // 配線の Y軸方向のマージン
  GLdouble mWireYMargin;

  // 拡大率
  GLdouble mZoom;

  // 表示領域の左下の座標
  Point mCorner;

  // 枠線を表示するとき true
  bool mDrawBBox;
  
  // オブジェクトの配列
  // ID をキーにしている
  vector<GateObj*> mObjArray;

  // mObjAray の使用区間を管理するオブジェクト
  ItvlMgr mObjItvl;
  
  // オブジェクトのリスト
  GateObjList mObjList;
  
};

END_NAMESPACE_YM_GLV

#endif // LED_LEDSCENE_H
