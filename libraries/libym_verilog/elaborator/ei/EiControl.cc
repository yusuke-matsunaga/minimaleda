
/// @file libym_verilog/elb_impl/EiControl.cc
/// @brief EiControl の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: EiControl.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "EiFactory.h"
#include "EiControl.h"
#include "ElbExpr.h"

#include "ym_verilog/pt/PtMisc.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// EiFactory の生成関数
//////////////////////////////////////////////////////////////////////

// @brief 遅延コントロールを生成する．
// @param[in] pt_control パース木の定義要素
// @param[in] delay 遅延式
ElbControl*
EiFactory::new_DelayControl(const PtControl* pt_control,
			    ElbExpr* delay)
{
  void* p = mAlloc.get_memory(sizeof(EiDelayControl));
  EiDelayControl* control = new (p) EiDelayControl(pt_control, delay);

  return control;
}

// @brief イベントコントロールを生成する．
// @param[in] pt_control パース木の定義要素
// @param[in] event_num イベントリストの要素数
// @param[in] event_list イベントリストを表す配列
ElbControl*
EiFactory::new_EventControl(const PtControl* pt_control,
			    ymuint32 event_num,
			    ElbExpr** event_list)
{
  void* p = mAlloc.get_memory(sizeof(EiEventControl));
  EiEventControl* control = new (p) EiEventControl(pt_control,
						   event_num, event_list);

  return control;
}

// @brief リピートコントロールを生成する．
// @param[in] pt_control パース木の定義要素
// @param[in] rep 繰り返し式
// @param[in] event_num イベントリストの要素数
// @param[in] event_list イベントリストを表す配列
ElbControl*
EiFactory::new_RepeatControl(const PtControl* pt_control,
			     ElbExpr* rep,
			     ymuint32 event_num,
			     ElbExpr** event_list)
{
  void* p = mAlloc.get_memory(sizeof(EiRepeatControl));
  EiRepeatControl* control = new (p) EiRepeatControl(pt_control, rep,
						     event_num, event_list);

  return control;
}


//////////////////////////////////////////////////////////////////////
// クラス EiControl
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] pt_control パース木の定義要素
EiControl::EiControl(const PtControl* pt_control) :
  mPtControl(pt_control)
{
}

// デストラクタ
EiControl::~EiControl()
{
}

// @brief ファイル位置を返す．
FileRegion
EiControl::file_region() const
{
  return mPtControl->file_region();
}

// @brief 遅延式を返す．
// @note このクラスでは NULL を返す．
const VlExpr*
EiControl::delay() const
{
  return NULL;
}

// @brief 繰り返し式を返す．
// @note このクラスでは NULL を返す．
const VlExpr*
EiControl::expr() const
{
  return NULL;
}

// @brief イベント条件式の数を返す．
// @note このクラスでは 0 を返す．
ymuint32
EiControl::event_num() const
{
  return 0;
}
  
// @brief イベント条件式を返す．
// @param[in] pos 位置番号 ( 0 <= pos < event_num() )
// @note このクラスでは NULL を返す．
const VlExpr*
EiControl::event(ymuint32 pos) const
{
  return NULL;
}


//////////////////////////////////////////////////////////////////////
/// クラス EiDelayControl
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] pt_control パース木の定義要素
// @param[in] delay 遅延式
EiDelayControl::EiDelayControl(const PtControl* pt_control,
			       ElbExpr* delay) :
  EiControl(pt_control),
  mDelay(delay)
{
}
  
// デストラクタ
EiDelayControl::~EiDelayControl()
{
}

// @brief 型の取得
tVpiObjType
EiDelayControl::type() const
{
  return kVpiDelayControl;
}
  
// 遅延式を返す．
const VlExpr*
EiDelayControl::delay() const
{
  return mDelay;
}


//////////////////////////////////////////////////////////////////////
/// クラス EiEventControl EiControl.h "EiControl.h"
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] pt_control パース木の定義要素
// @param[in] event_num イベントリストの要素数
// @param[in] event_list イベントリストを表す配列
EiEventControl::EiEventControl(const PtControl* pt_control,
			       ymuint32 event_num,
			       ElbExpr** event_list) :
  EiControl(pt_control),
  mEventNum(event_num),
  mEventList(event_list)
{
}
  
// @brief デストラクタ
EiEventControl::~EiEventControl()
{
}

// @brief 型の取得
tVpiObjType
EiEventControl::type() const
{
  return kVpiEventControl;
}

// @brief イベント条件式の数を返す．
ymuint32
EiEventControl::event_num() const
{
  return mEventNum;
}
  
// @brief イベント条件式を返す．
// @param[in] pos 位置番号 ( 0 <= pos < event_num() )
const VlExpr*
EiEventControl::event(ymuint32 pos) const
{
  return mEventList[pos];
}


//////////////////////////////////////////////////////////////////////
/// クラス EiRepeatControl ElbControl.h "ElbControl.h"
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] pt_control パース木の定義要素
// @param[in] rep 繰り返し式
// @param[in] event_num イベントリストの要素数
// @param[in] event_list イベントリストを表す配列
EiRepeatControl::EiRepeatControl(const PtControl* pt_control,
				 ElbExpr* rep,
				 ymuint32 event_num,
				 ElbExpr** event_list) :
  EiEventControl(pt_control, event_num, event_list),
  mExpr(rep)
{
}
  
// デストラクタ
EiRepeatControl::~EiRepeatControl()
{
}

// @brief 型の取得
tVpiObjType
EiRepeatControl::type() const
{
  return kVpiRepeatControl;
}

// 繰り返し式を返す．
const VlExpr*
EiRepeatControl::expr() const
{
  return mExpr;
}

END_NAMESPACE_YM_VERILOG
