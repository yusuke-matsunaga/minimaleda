#ifndef YM_VERILOG_VPI_VPIPROCESS_H
#define YM_VERILOG_VPI_VPIPROCESS_H

/// @file ym_verilog/vpi/VpiProcess.h
/// @brief Initial/Always 文を表すクラスのヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: VpiProcess.h 400 2007-03-06 17:58:07Z matsunaga $
///
/// Copyright (C) 2005-2006 Yusuke Matsunaga
/// All rights reserved.

// IEEE Std 1364-2001 26.6.27 Process, block, statement, event statement


#include <ym_verilog/vpi/VpiBase.h>


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class VpiProcess VpiProcess.h <ym_verilog/vpi/VpiProcess.h>
/// @brief intial/always の基底クラス
/// IEEE Std 1364-2001 26.6.27 Process, block, statement, event statement
//////////////////////////////////////////////////////////////////////
class VpiProcess :
  public VpiBase
{
public:
  /// @brief 親クラスのエイリアス
  typedef VpiBase Parent;


public:
  //////////////////////////////////////////////////////////////////////
  // VpiProcess に固有の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 親のモジュールを取出す
  virtual
  VpiModule*
  module() const = 0;

  /// @brief 親のスコープを取出す
  virtual
  VpiScope*
  scope() const = 0;
  
  /// @brief ステートメントを返す．
  virtual
  VpiStmt*
  stmt() const = 0;

};

END_NAMESPACE_YM_VERILOG

#endif // YM_VERILOG_VPI_VPIPROCESS_H
