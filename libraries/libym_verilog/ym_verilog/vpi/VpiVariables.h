#ifndef YM_VERILOG_VPI_VPIVARIABLES_H
#define YM_VERILOG_VPI_VPIVARIABLES_H

/// @file ym_verilog/vpi/VpiVariables.h
/// @brief Variables 宣言要素のクラスのヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: VpiVariables.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2006 Yusuke Matsunaga
/// All rights reserved.


#include <ym_verilog/vpi/VpiNamedObj.h>
#include <ym_verilog/vpi/VpiIterator.h>


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class VpiVariables VpiVariables.h <ym_verilog/vpi/VpiVariables.h>
/// @brief 変数/変数配列に共通な基底クラス
/// IEEE Std 1364-2001 26.6.8 Variables
//////////////////////////////////////////////////////////////////////
class VpiVariables :
  public VpiNamedObj
{
public:
  //////////////////////////////////////////////////////////////////////
  // VpiVariables に固有の仮想関数
  //////////////////////////////////////////////////////////////////////
  
  /// @brief 配列要素の時 true を返す．
  virtual
  bool
  is_array() const = 0;
  
  /// @brief 多次元配列の要素の時 true を返す．
  virtual
  bool
  is_multi_array() const = 0;
  
  /// @brief 符号付きの時 true を返す．
  virtual
  bool
  is_signed() const = 0;

  /// @brief 範囲のMSBを返す．
  virtual
  VpiExpr*
  left_range() const = 0;
  
  /// @brief 範囲のLSBを返す．
  virtual
  VpiExpr*
  right_range() const = 0;

  /// @brief 接続している内側のポートのリストの反復子を返す．
  virtual
  VpiPortsIteratorImpl*
  ports_iterator_obj() = 0;

  /// @brief 接続している内側のポートのリストの反復子を返す．
  virtual
  VpiPortsIterator
  ports_iterator() = 0;

  /// @brief 接続している外側のポートのリストの反復子を返す．
  virtual
  VpiPortInstIteratorImpl*
  port_inst_iterator_obj() = 0;

  /// @brief 接続している外側のポートのリストの反復子を返す．
  virtual
  VpiPortInstIterator
  port_inst_iterator() = 0;

  /// @brief 範囲のリストの反復子を返す．
  virtual
  VpiRangeIteratorImpl*
  range_iterator_obj() = 0;

  /// @brief 範囲のリストの反復子を返す．
  virtual
  VpiRangeIterator
  range_iterator() = 0;

  /// @brief var-select のリストの反復子を返す．
  virtual
  VpiVarSelectIteratorImpl*
  var_select_iterator_obj() = 0;

  /// @brief var-select のリストの反復子を返す．
  virtual
  VpiVarSelectIterator
  var_select_iterator() = 0;
  
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
/// @class VpiVarArray VpiVariables.h <ym_verilog/vpi/VpiVariables.h>
/// @brief 変数配列を表すクラス
/// IEEE Std 1364-2001 26.6.8 Variables
//////////////////////////////////////////////////////////////////////
class VpiVarArray :
  public VpiVariables
{
public:
  //////////////////////////////////////////////////////////////////////
  // VpiVarArray に固有の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 配列の次元数を返す．
  virtual
  ymuint32
  array_dim() const = 0;
  
  /// @brief 要素数を返す．
  virtual
  int
  elem_size() const = 0;
  
};


//////////////////////////////////////////////////////////////////////
/// @class VpiVariable VpiVariables.h <ym_verilog/vpi/VpiVariables.h>
/// @brief 単一の変数を表すクラス
/// IEEE Std 1364-2001 26.6.8 Variables
//////////////////////////////////////////////////////////////////////
class VpiVariable :
  public VpiVariables
{
public:
  //////////////////////////////////////////////////////////////////////
  // VpiVariable に固有の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 初期値を設定する．
  virtual
  void
  set_expr(VpiExpr* init) = 0;

  /// @brief 初期値を返す．
  virtual
  VpiExpr*
  expr() const = 0;

  /// @brief サイズ(ビット幅)を返す．
  virtual
  int
  size() const = 0;
  
};


//////////////////////////////////////////////////////////////////////
/// @class VpiVarSelect VpiVariables.h <ym_verilog/vpi/VpiVariables.h>
/// @brief 変数配列の要素を表すクラス
/// IEEE Std 1364-2001 26.6.8 Variables
//////////////////////////////////////////////////////////////////////
class VpiVarSelect :
  public VpiNamedObj
{
public:
  //////////////////////////////////////////////////////////////////////
  // VpiVarSelect に固有の仮想関数
  //////////////////////////////////////////////////////////////////////
  
  /// @brief サイズを返す．
  virtual
  int
  size() const = 0;
  
  /// @brief 符号付きの時 true を返す．
  virtual
  bool
  is_signed() const = 0;

  /// @brief 配列要素の時 true を返す．
  virtual
  bool
  is_array() const = 0;
  
  /// @brief 多次元配列の要素の時 true を返す．
  virtual
  bool
  is_multi_array() const = 0;
    
  /// @brief index が定数の時 true を返す．
  virtual
  bool
  is_constant_select() const = 0;
  
  /// @brief 親の変数配列を返す．
  virtual
  VpiVarArray*
  parent() const = 0;

  /// @brief インデックスの次元を返す．
  virtual
  ymuint32
  index_dim() const = 0;

  /// @brief インデックスを返す．
  virtual
  VpiExpr*
  index(ymuint32 pos) const = 0;

  /// @brief インデックスを返す．
  /// @note これは一次元の時のみ有効
  virtual
  VpiExpr*
  index() const = 0;

  /// @brief インデックスリストの反復子を返す．
  virtual
  VpiIndexIteratorImpl*
  index_iterator_obj() = 0;

  /// @brief インデックスリストの反復子を返す．
  virtual
  VpiIndexIterator
  index_iterator() = 0;
  
  /// @brief 自分を使っている terminal/ports などのリストの反復子を返す．
  virtual
  VpiUseIteratorImpl*
  use_iterator_obj() = 0;
  
  /// @brief 自分を使っている terminal/ports などのリストの反復子を返す．
  virtual
  VpiUseIterator
  use_iterator() = 0;

};

END_NAMESPACE_YM_VERILOG

#endif // YM_VERILOG_VPI_VPIVARIABLES_H
