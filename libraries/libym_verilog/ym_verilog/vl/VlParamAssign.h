#ifndef YM_VERILOG_VL_VLPARAMASSIGN_H
#define YM_VERILOG_VL_VLPARAMASSIGN_H

/// @file ym_verilog/vl/VlParamAssign.h
/// @brief VlParamAssign のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: VlParamAssign.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_verilog/vl/VlObj.h"
#include "ym_verilog/vl/VlFwd.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class VlParamAssign VlParamAssign.h <ym_verilog/vl/VlParamAssign.h>
/// @brief parameter 代入文を表すクラス
/// IEEE Std 1364-2001 26.6.12 Parameter, specparam
//////////////////////////////////////////////////////////////////////
class VlParamAssign :
  public VlObj
{
protected:

  /// @brief デストラクタ
  virtual
  ~VlParamAssign() {}


public:
  //////////////////////////////////////////////////////////////////////
  // VlParamAssign の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 親のモジュールを返す．
  virtual
  const VlModule*
  parent() const = 0;
  
  /// @brief 左辺値を返す．
  virtual
  const VlDecl*
  lhs() const = 0;
  
  /// @brief 右辺値を返す．
  virtual
  const VlExpr*
  rhs() const = 0;
  
  /// @brief 名前による接続の場合に true を返す．
  virtual
  bool
  is_conn_by_name() const = 0;
  
};


//////////////////////////////////////////////////////////////////////
/// @class VlDefParam VlParamAssign.h <ym_verilog/vl/VlParamAssign.h>
/// @brief defparam 文を表すクラス
/// IEEE Std 1364-2001 26.6.12 Parameter, specparam
//////////////////////////////////////////////////////////////////////
class VlDefParam :
  public VlObj
{
protected:

  /// @brief デストラクタ
  virtual
  ~VlDefParam() {}


public:
  //////////////////////////////////////////////////////////////////////
  // VlParamAssign の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 親のモジュールを返す．
  virtual
  const VlModule*
  parent() const = 0;
  
  /// @brief 左辺値を返す．
  virtual
  const VlDecl*
  lhs() const = 0;
  
  /// @brief 右辺値を返す．
  virtual
  const VlExpr*
  rhs() const = 0;
  
};


END_NAMESPACE_YM_VERILOG

#endif // YM_VERILOG_VL_VLPARAMASSIGN_H
