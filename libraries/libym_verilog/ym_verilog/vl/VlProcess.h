#ifndef YM_VERILOG_VL_VLPROCESS_H
#define YM_VERILOG_VL_VLPROCESS_H

/// @file ym_verilog/vl/VlProcess.h
/// @brief VlProcess のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: VlProcess.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_verilog/vl/VlObj.h"
#include "ym_verilog/vl/VlFwd.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class VlProcess VlProcess.h <ym_verilog/vl/VlProcess.h>
/// @brief initial/always 文を表すクラス
/// IEEE Std 1364-2001 26.6.27 Process, block, statement, event statement
//////////////////////////////////////////////////////////////////////
class VlProcess :
  public VlObj
{
protected:

  /// @brief デストラクタ
  virtual
  ~VlProcess() {}


public:
  //////////////////////////////////////////////////////////////////////
  // VlProcess の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 親のスコープを返す．
  virtual
  const VlNamedObj*
  parent() const = 0;
  
  /// @brief 本体のステートメントの取得
  virtual
  const VlStmt*
  stmt() const = 0;
  
};


END_NAMESPACE_YM_VERILOG

#endif // YM_VERILOG_VL_VLPROCESS_H
