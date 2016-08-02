#ifndef LIBYM_VERILOG_ELABORATOR_EI_EICONTROL_H
#define LIBYM_VERILOG_ELABORATOR_EI_EICONTROL_H

/// @file libym_verilog/elaborator/ei/EiControl.h
/// @brief ElbControl の実装クラスのヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: EiControl.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.

// IEEE Std 1364-2001 26.6.29 Delay control
// IEEE Std 1364-2001 26.6.30 Event control
// IEEE Std 1364-2001 26.6.31 Repeat control


#include "ElbControl.h"
#include "ym_verilog/pt/PtP.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class EiControl EiControl.h "EiControl.h"
/// @brief ElbControl の実装クラス
//////////////////////////////////////////////////////////////////////
class EiControl :
  public ElbControl
{
protected:

  /// @brief コンストラクタ
  /// @param[in] pt_control パース木の定義要素
  EiControl(const PtControl* pt_control);

  /// デストラクタ
  virtual
  ~EiControl();


public:
  //////////////////////////////////////////////////////////////////////
  // VlObj の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイル位置を返す．
  virtual
  FileRegion
  file_region() const;


public:
  //////////////////////////////////////////////////////////////////////
  // VlControl に固有の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 遅延式を返す．
  /// @note このクラスでは NULL を返す．
  virtual
  const VlExpr*
  delay() const;

  /// @brief 繰り返し式を返す．
  /// @note このクラスでは NULL を返す．
  virtual
  const VlExpr*
  expr() const;

  /// @brief イベント条件式の数を返す．
  /// @note このクラスでは 0 を返す．
  virtual
  ymuint32
  event_num() const;

  /// @brief イベント条件式を返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < event_num() )
  /// @note このクラスでは NULL を返す．
  virtual
  const VlExpr*
  event(ymuint32 pos) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // パース木の定義要素
  const PtControl* mPtControl;

};


//////////////////////////////////////////////////////////////////////
/// @class EiDelayControl EiControl.h "EiControl.h"
/// @brief delay control statement を表すクラス
/// IEEE1364-2001 では procedural timing control
/// とこれを混同している．
/// 正確には delay control はステートメントではない．
//////////////////////////////////////////////////////////////////////
class EiDelayControl :
  public EiControl
{
  friend class EiFactory;

private:

  /// @brief コンストラクタ
  /// @param[in] pt_control パース木の定義要素
  /// @param[in] delay 遅延式
  EiDelayControl(const PtControl* pt_control,
		 ElbExpr* delay);

  /// デストラクタ
  virtual
  ~EiDelayControl();


public:
  //////////////////////////////////////////////////////////////////////
  // ElbObj の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型の取得
  virtual
  tVpiObjType
  type() const;


public:
  //////////////////////////////////////////////////////////////////////
  // ElbControl に固有の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// 遅延式を返す．
  virtual
  const VlExpr*
  delay() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 遅延式
  ElbExpr* mDelay;

};


//////////////////////////////////////////////////////////////////////
/// @class EiEventControl EiControl.h "EiControl.h"
/// @brief event control statement を表すクラス
/// IEEE1364-2001 では procedural timing control
/// とこれを混同している．
/// 正確には event control はステートメントではない．
//////////////////////////////////////////////////////////////////////
class EiEventControl :
  public EiControl
{
  friend class EiFactory;

protected:

  /// @brief コンストラクタ
  /// @param[in] pt_control パース木の定義要素
  /// @param[in] event_num イベントリストの要素数
  /// @param[in] event_list イベントリストを表す配列
  EiEventControl(const PtControl* pt_control,
		 ymuint32 event_num,
		 ElbExpr** event_list);

  /// @brief デストラクタ
  virtual
  ~EiEventControl();


public:
  //////////////////////////////////////////////////////////////////////
  // VlObj の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型の取得
  virtual
  tVpiObjType
  type() const;


public:
  //////////////////////////////////////////////////////////////////////
  // VlControl に固有の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief イベント条件式の数を返す．
  virtual
  ymuint32
  event_num() const;

  /// @brief イベント条件式を返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < event_num() )
  virtual
  const VlExpr*
  event(ymuint32 pos) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // イベントリストの要素数
  ymuint32 mEventNum;

  // イベントリスト
  ElbExpr** mEventList;

};


//////////////////////////////////////////////////////////////////////
/// @class EiRepeatControl ElbControl.h "ElbControl.h"
/// @brief repeat control を表すクラス
/// IEEE Std 1364-2001 26.6.31 Repeat control
//////////////////////////////////////////////////////////////////////
class EiRepeatControl :
  public EiEventControl
{
  friend class EiFactory;

private:

  /// @brief コンストラクタ
  /// @param[in] pt_control パース木の定義要素
  /// @param[in] rep 繰り返し式
  /// @param[in] event_num イベントリストの要素数
  /// @param[in] event_list イベントリストを表す配列
  EiRepeatControl(const PtControl* pt_control,
		  ElbExpr* rep,
		  ymuint32 event_num,
		  ElbExpr** event_list);

  /// デストラクタ
  virtual
  ~EiRepeatControl();


public:
  //////////////////////////////////////////////////////////////////////
  // VlObj の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型の取得
  virtual
  tVpiObjType
  type() const;


public:
  //////////////////////////////////////////////////////////////////////
  // VlControl に固有の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// 繰り返し式を返す．
  virtual
  const VlExpr*
  expr() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 繰り返し式
  ElbExpr* mExpr;

};

END_NAMESPACE_YM_VERILOG

#endif // LIBYM_VERILOG_ELABORATOR_EI_EICONTROL_H
