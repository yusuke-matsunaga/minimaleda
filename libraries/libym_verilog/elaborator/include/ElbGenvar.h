#ifndef LIBYM_VERILOG_ELB_ELBGENVAR_H
#define LIBYM_VERILOG_ELB_ELBGENVAR_H

/// @file libym_verilog/elabotor/include/ElbGenvar.h
/// @brief EiGenvar のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: ElbGenvar.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2008 Yusuke Matsunaga
/// All rights reserved.


#include "ElbDecl.h"
#include "ym_verilog/pt/PtP.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class ElbGenvar ElbGenvar.h "ElbGenvar.h"
/// @brief genvar を表すクラス
//////////////////////////////////////////////////////////////////////
class ElbGenvar :
  public VlNamedObj
{
protected:

  /// @brief コンストラクタ
  ElbGenvar() { }

  /// @brief デストラクタ
  virtual
  ~ElbGenvar() { }


public:
  //////////////////////////////////////////////////////////////////////
  // ElbGenvar に固有の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 使用中の時 true を返す．
  virtual
  bool
  is_inuse() const = 0;

  /// @brief 使用中にする．
  virtual
  void
  set_inuse() = 0;

  /// @brief 使用を終える．
  virtual
  void
  reset_inuse() = 0;

  /// @brief 現在の値を返す．
  virtual
  int
  value() const = 0;

  /// @brief 値を設定する．
  virtual
  void
  set_value(int value) = 0;

  /// @brief 元となったパース木の定義要素を返す．
  virtual
  const PtDeclItem*
  pt_item() const = 0;

};

END_NAMESPACE_YM_VERILOG

#endif // LIBYM_VERILOG_ELB_ELBGENVAR_H
