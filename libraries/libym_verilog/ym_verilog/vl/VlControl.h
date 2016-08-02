#ifndef YM_VERILOG_VL_VLCONTROL_H
#define YM_VERILOG_VL_VLCONTROL_H

/// @file ym_verilog/vl/VlControl.h
/// @brief コントロールを表すクラスのヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: VlControl.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.

// IEEE Std 1364-2001 26.6.29 Delay control
// IEEE Std 1364-2001 26.6.30 Event control
// IEEE Std 1364-2001 26.6.31 Repeat control


#include "ym_verilog/vl/VlObj.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class VlControl VlControl.h <ym_verilog/vl/VlControl.h>
/// @brief delay/event/repeat control statement を表すクラス
/// IEEE Std 1364-2001 26.6.29 Delay control
/// IEEE Std 1364-2001 26.6.30 Event control
/// IEEE Std 1364-2001 26.6.31 Repeat control
/// IEEE1364-2001 では procedural timing control
/// とこれを混同している．
/// 正確には delay control はステートメントではない．
//////////////////////////////////////////////////////////////////////
class VlControl :
  public VlObj
{
protected:

  /// @brief デストラクタ
  virtual
  ~VlControl() { }
  
  
public:
  //////////////////////////////////////////////////////////////////////
  // VlControl の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 遅延式を返す．
  virtual
  const VlExpr*
  delay() const = 0;

  /// @brief 繰り返し式を返す．
  virtual
  const VlExpr*
  expr() const = 0;
  
  /// @brief イベント条件式の数を返す．
  virtual
  ymuint32
  event_num() const = 0;
  
  /// @brief イベント条件式を返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < event_num() )
  virtual
  const VlExpr*
  event(ymuint32 pos) const = 0;

};

END_NAMESPACE_YM_VERILOG

#endif // YM_VERILOG_VL_VLCONTROL_H
