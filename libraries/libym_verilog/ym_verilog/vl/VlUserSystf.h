#ifndef YM_VERILOG_VL_VLUSERSYSTF_H
#define YM_VERILOG_VL_VLUSERSYSTF_H

/// @file ym_verilog/vl/VlUserSystf.h
/// @brief VlUserSystf のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: VlUserSystf.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_verilog/vl/VlObj.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class VlUserSystf VlUserSystf.h <ym_verilog/vl/VlUserSystf.h>
/// @brief システムタスク/関数の定義を表すクラス
/// IEEE Std 1364-2001 26.6.14 UDP
//////////////////////////////////////////////////////////////////////
class VlUserSystf :
  public VlObj
{
public:

  /// @brief デストラクタ
  virtual
  ~VlUserSystf() {}


public:
  //////////////////////////////////////////////////////////////////////
  // VlUserSystf に固有の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 名前を返す．
  virtual
  string
  name() const = 0;
  
  /// @brief system task の時 true を返す．
  virtual
  bool
  system_task() const = 0;

  /// @brief system function の時 true を返す．
  virtual
  bool
  system_function() const = 0;

  /// @brief system function の型を返す．
  virtual
  tVpiFuncType
  function_type() const = 0;
  
  /// @brief compile 時のコールバック関数
  virtual
  ymuint32
  on_compile() = 0;

  /// @brief 実行時のコールバック関数
  virtual
  ymuint32
  on_call() = 0;

  /// @brief SizedFunc の場合にサイズを返す．
  virtual
  ymuint32
  size() const = 0;
  
};

END_NAMESPACE_YM_VERILOG

#endif // YM_VERILOG_VL_VLUSERSYSTF_H
