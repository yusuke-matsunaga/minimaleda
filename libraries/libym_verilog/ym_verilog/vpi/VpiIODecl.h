#ifndef YM_VERILOG_VPI_VPIIODECL_H
#define YM_VERILOG_VPI_VPIIODECL_H

/// @file ym_verilog/vpi/VpiIODecl.h
/// @brief IO宣言要素のクラスのヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: VpiIODecl.h 400 2007-03-06 17:58:07Z matsunaga $
///
/// Copyright (C) 2005-2006 Yusuke Matsunaga
/// All rights reserved.


#include <ym_verilog/vpi/VpiBase.h>


BEGIN_NAMESPACE_YM_VERILOG

class VpiModule;
class VpiUdpDefn;
class VpiTaskFunc;
class VpiExpr;

//////////////////////////////////////////////////////////////////////
/// @class VpiIODecl VpiIODecl.h <ym_verilog/vpi/VpiIODecl.h>
/// @brief IO decl の基底クラス
//////////////////////////////////////////////////////////////////////
class VpiIODecl :
  public VpiBase
{
public:
  //////////////////////////////////////////////////////////////////////
  // VpiIODecl に固有の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 名前を返す．
  virtual
  string
  name() const = 0;

  /// @brief 方向を返す．
  virtual
  tVpiDirection
  direction() const = 0;

  /// @brief サイズを返す．
  virtual
  int
  size() const = 0;

  /// @brief 符号の属性の取得
  /// @return 符号付きのとき true を返す．
  virtual
  bool
  is_signed() const = 0;

  /// @brief スカラーのチェック
  /// @return スカラー (1ビット) なら true を返す．
  virtual
  bool
  is_scalar() const = 0;
  
  /// @brief ベクターのチェック
  /// @return ベクター (多ビット) なら true を返す．
  virtual
  bool
  is_vector() const = 0;

  /// @brief 接続している式を返す．
  virtual
  VpiExpr*
  expr() const = 0;

  /// @brief 範囲の MSB の取得
  /// @return 範囲の MSB を返す．
  virtual
  VpiExpr*
  left_range() const = 0;

  /// @brief 範囲の LSB の取得
  /// @return 範囲の LSB を返す．
  virtual
  VpiExpr*
  right_range() const = 0;

  /// @brief 親のモジュールの取得
  virtual
  VpiModule*
  module() const = 0;

  /// @brief 親の UDP の取得
  virtual
  VpiUdpDefn*
  udp_defn() const = 0;

  /// @brief 親の task/function の取得
  virtual
  VpiTaskFunc*
  task_func() const = 0;

};

END_NAMESPACE_YM_VERILOG

#endif // YM_VERILOG_VPI_VPIIODECL_H
