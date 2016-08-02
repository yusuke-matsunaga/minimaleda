#ifndef LIBYM_VERILOG_ELB_ELBCONTROL_H
#define LIBYM_VERILOG_ELB_ELBCONTROL_H

/// @file libym_verilog/elaborator/include/ElbControl.h
/// @brief コントロールを表すクラスのヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: ElbControl.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.

// IEEE Std 1364-2001 26.6.29 Delay control
// IEEE Std 1364-2001 26.6.30 Event control
// IEEE Std 1364-2001 26.6.31 Repeat control


#include "ym_verilog/vl/VlControl.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class ElbControl ElbControl.h "ElbControl.h"
/// @brief delay/event/repeat control statement を表すクラス
/// IEEE Std 1364-2001 26.6.29 Delay control
/// IEEE Std 1364-2001 26.6.30 Event control
/// IEEE Std 1364-2001 26.6.31 Repeat control
/// IEEE1364-2001 では procedural timing control
/// とこれを混同している．
/// 正確には delay control はステートメントではない．
//////////////////////////////////////////////////////////////////////
class ElbControl :
  public VlControl
{
protected:
  
  /// @brief コンストラクタ
  ElbControl() { }
  
  /// @brief デストラクタ
  virtual
  ~ElbControl() { }

};

END_NAMESPACE_YM_VERILOG

#endif // LIBYM_VERILOG_ELB_ELBCONTROL_H
