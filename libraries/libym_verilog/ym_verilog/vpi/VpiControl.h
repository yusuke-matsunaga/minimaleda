#ifndef YM_VERILOG_VPI_VPICONTROL_H
#define YM_VERILOG_VPI_VPICONTROL_H

/// @file ym_verilog/vpi/VpiControl.h
/// @brief コントロールを表すクラスのヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: VpiControl.h 400 2007-03-06 17:58:07Z matsunaga $
///
/// Copyright (C) 2005-2006 Yusuke Matsunaga
/// All rights reserved.

// IEEE Std 1364-2001 26.6.29 Delay control
// IEEE Std 1364-2001 26.6.30 Event control
// IEEE Std 1364-2001 26.6.31 Repeat control

#include <ym_verilog/vpi/VpiBase.h>


BEGIN_NAMESPACE_YM_VERILOG

class VpiExpr;

//////////////////////////////////////////////////////////////////////
/// @class VpiDelayControl VpiControl.h <ym_verilog/vpi/VpiControl.h>
/// @brief delay control statement を表すクラス
/// IEEE Std 1364-2001 26.6.29 Delay control
/// IEEE1364-2001 では procedural timing control
/// とこれを混同している．
/// 正確には delay control はステートメントではない．
//////////////////////////////////////////////////////////////////////
class VpiDelayControl :
  public VpiBase
{
public:
  //////////////////////////////////////////////////////////////////////
  // VpiDelayControl に固有の仮想関数
  //////////////////////////////////////////////////////////////////////
  
  /// @brief 遅延式を返す．
  virtual
  VpiExpr*
  delay() const = 0;

};


//////////////////////////////////////////////////////////////////////
/// @class VpiEventControl VpiControl.h <ym_verilog/vpi/VpiControl.h>
/// @brief event control statement を表すクラス
/// IEEE Std 1364-2001 26.6.30 Event control
/// IEEE1364-2001 では procedural timing control
/// とこれを混同している．
/// 正確には event control はステートメントではない．
//////////////////////////////////////////////////////////////////////
class VpiEventControl :
  public VpiBase
{
public:
  //////////////////////////////////////////////////////////////////////
  // VpiEventControl に固有の仮想関数
  //////////////////////////////////////////////////////////////////////
  
  /// @brief 条件式を返す．
  virtual
  VpiExpr*
  condition() const = 0;

};


//////////////////////////////////////////////////////////////////////
/// @class VpiRepeatControl VpiControl.h <ym_verilog/vpi/VpiControl.h>
/// @brief repeat control を表すクラス
/// IEEE Std 1364-2001 26.6.31 Repeat control
//////////////////////////////////////////////////////////////////////
class VpiRepeatControl :
  public VpiBase
{
public:
  //////////////////////////////////////////////////////////////////////
  // VpiRepeatControl に固有の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 繰り返し式を返す．
  virtual
  VpiExpr*
  expr() const = 0;
  
  /// @brief 本体の event control を返す．
  virtual
  VpiEventControl*
  event_control() const = 0;

};

END_NAMESPACE_YM_VERILOG

#endif // YM_VERILOG_VPI_VPICONTROL_H
