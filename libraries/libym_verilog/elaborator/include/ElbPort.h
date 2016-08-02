#ifndef LIBYM_VERILOG_ELB_ELBPORT_H
#define LIBYM_VERILOG_ELB_ELBPORT_H

/// @file libym_verilog/elaborator/include/ElbPort.h
/// @brief ElbPort のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: ElbPort.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_verilog/vl/VlPort.h"
#include "ElbFwd.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class ElbPort ElbPort.h "ElbPort.h"
/// @brief ポートを表すクラス
/// IEEE Std 1364-2001 26.6.5 Ports
//////////////////////////////////////////////////////////////////////
class ElbPort :
  public VlPort
{
protected:

  /// @brief コンストラクタ
  ElbPort() { }
  
  /// @brief デストラクタ
  virtual
  ~ElbPort() { }

};

END_NAMESPACE_YM_VERILOG

#endif // LIBYM_VERILOG_ELB_ELBPORT_H
