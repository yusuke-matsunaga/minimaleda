#ifndef YM_VERILOG_VPI_VPIPARAMETER_H
#define YM_VERILOG_VPI_VPIPARAMETER_H

/// @file ym_verilog/vpi/VpiParameter.h
/// @brief Parameter 宣言要素のクラスのヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: VpiParameter.h 1086 2007-12-11 05:42:36Z matsunaga $
///
/// Copyright (C) 2005-2006 Yusuke Matsunaga
/// All rights reserved.


#include <ym_verilog/vpi/VpiNamedObj.h>
#include <ym_verilog/vpi/VpiIterator.h>


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class VpiParameter VpiParameter.h <ym_verilog/vpi/VpiParameter.h>
/// @brief パラメータの基底クラス
/// IEEE Std 1364-2001 26.6.12 Parameter, specparam
//////////////////////////////////////////////////////////////////////
class VpiParameter :
  public VpiNamedObj
{
public:
  //////////////////////////////////////////////////////////////////////
  // VpiParameter に固有の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 値を設定する．
  virtual
  void
  set_expr(VpiExpr* expr) = 0;

  /// @brief その expression を用いているオブジェクトを登録する．
  virtual
  void
  add_use(VpiObj* obj) = 0;
  
  /// @brief local param の時 true を返す．
  virtual
  bool
  is_local_param() const = 0;

  /// @brief 符号付きの時 true を返す．
  virtual
  bool
  is_signed() const = 0;

  /// @brief 型を返す．
  virtual
  tVpiConstType
  const_type() const = 0;
  
  /// @brief サイズを返す．
  virtual
  int
  size() const = 0;

  /// @brief 範囲のMSBを返す．
  virtual
  VpiExpr*
  left_range() const = 0;
  
  /// @brief 範囲のLSBを返す．
  virtual
  VpiExpr*
  right_range() const = 0;

  /// @brief オフセットからインデックスを得る．
  virtual
  int
  offset2index(int offset) const = 0;

  /// @brief インデックスからオフセットを得る．
  virtual
  int
  index2offset(int index) const = 0;

  /// @brief 値を表す式を返す．
  virtual
  VpiExpr*
  expr() const = 0;
  
  /// @brief 自分を使っている terminal/ports などのリストの反復子を返す．
  virtual
  VpiUseIteratorImpl*
  use_iterator_obj() = 0;
  
  /// @brief 自分を使っている terminal/ports などのリストの反復子を返す．
  virtual
  VpiUseIterator
  use_iterator() = 0;

};


//////////////////////////////////////////////////////////////////////
/// @class VpiSpecParam VpiParameter.h <ym_verilog/vpi/VpiParameter.h>
/// @brief spec param の基底クラス
/// IEEE Std 1364-2001 26.6.12 Parameter, specparam
//////////////////////////////////////////////////////////////////////
class VpiSpecParam :
  public VpiNamedObj
{
public:
  //////////////////////////////////////////////////////////////////////
  // VpiSpecParam に固有の仮想関数
  //////////////////////////////////////////////////////////////////////
  
  /// @brief 値を設定する．
  virtual
  void
  set_expr(VpiExpr* expr) = 0;

  /// @brief その expression を用いているオブジェクトを登録する．
  virtual
  void
  add_use(VpiObj* obj) = 0;

  /// @brief 型を返す．
  virtual
  tVpiConstType
  const_type() const = 0;
  
  /// @brief サイズを返す．
  virtual
  int
  size() const = 0;

  /// @brief 範囲のMSBを返す．
  virtual
  VpiExpr*
  left_range() const = 0;
  
  /// @brief 範囲のLSBを返す．
  virtual
  VpiExpr*
  right_range() const = 0;

  /// @brief オフセットからインデックスを得る．
  virtual
  int
  offset2index(int offset) const = 0;

  /// @brief インデックスからオフセットを得る．
  virtual
  int
  index2offset(int index) const = 0;

  /// @brief 値を表す式を返す．
  virtual
  VpiExpr*
  expr() const = 0;
  
  /// @brief 自分を使っている terminal/ports などのリストの反復子を返す．
  virtual
  VpiUseIteratorImpl*
  use_iterator_obj() = 0;
  
  /// @brief 自分を使っている terminal/ports などのリストの反復子を返す．
  virtual
  VpiUseIterator
  use_iterator() = 0;

};


//////////////////////////////////////////////////////////////////////
/// @class VpiDefParam VpiParameter.h <ym_verilog/vpi/VpiParameter.h>
/// @brief def param 文を表すクラス
/// IEEE Std 1364-2001 26.6.12 Parameter, specparam
//////////////////////////////////////////////////////////////////////
class VpiDefParam :
  public VpiBase
{
public:
  //////////////////////////////////////////////////////////////////////
  // VpiDefParam に固有の仮想関数
  //////////////////////////////////////////////////////////////////////
  
  /// @brief 親のモジュールを取出す
  virtual
  VpiModule*
  module() const = 0;
  
  /// @brief 親のスコープを取出す
  virtual
  VpiScope*
  scope() const = 0;
  
  /// @brief 左辺値を返す．
  virtual
  VpiParameter*
  lhs() const = 0;
  
  /// @brief 右辺値を返す．
  virtual
  VpiExpr*
  rhs() const = 0;
  
};


//////////////////////////////////////////////////////////////////////
/// @class VpiParamAssign VpiParameter.h <ym_verilog/vpi/VpiParameter.h>
/// @brief parameter assignmentを表すクラス
/// IEEE Std 1364-2001 26.6.12 Parameter, specparam
//////////////////////////////////////////////////////////////////////
class VpiParamAssign :
  public VpiBase
{
public:
  //////////////////////////////////////////////////////////////////////
  // VpiParamAssign に固有な仮想関数
  //////////////////////////////////////////////////////////////////////
  
  /// @brief 名前による接続の場合に true を返す．
  virtual
  bool
  is_conn_by_name() const = 0;
  
  /// @brief 親のモジュールを取出す
  virtual
  VpiModule*
  module() const = 0;
  
  /// @brief 親のスコープを取出す
  virtual
  VpiScope*
  scope() const = 0;
  
  /// @brief 左辺値を返す．
  virtual
  VpiParameter*
  lhs() const = 0;
  
  /// @brief 右辺値を返す．
  virtual
  VpiExpr*
  rhs() const = 0;

};

END_NAMESPACE_YM_VERILOG

#endif // YM_VERILOG_VPI_VPIDECL_H
