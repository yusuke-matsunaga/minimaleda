#ifndef YM_VERILOG_VPI_VPIREGS_H
#define YM_VERILOG_VPI_VPIREGS_H

/// @file ym_verilog/vpi/VpiRegs.h
/// @brief Reg 宣言要素のクラスのヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: VpiRegs.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2006 Yusuke Matsunaga
/// All rights reserved.

// Regs 宣言要素のクラス
// IEEE Std 1364-2001 26.6.7 Regs and reg arrays


#include <ym_verilog/vpi/VpiNamedObj.h>
#include <ym_verilog/vpi/VpiIterator.h>


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class VpiRegArray VpiRegs.h <ym_verilog/vpi/VpiRegs.h>
/// @brief REGの配列を表すクラス
/// IEEE Std 1364-2001 26.6.7 Regs and reg arrays
//////////////////////////////////////////////////////////////////////
class VpiRegArray :
  public VpiNamedObj
{
public:
  //////////////////////////////////////////////////////////////////////
  // VpiRegArray に固有の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 配列要素を生成する．
  virtual
  void
  instantiate_elem(VpiFactory* factory) = 0;

  /// @brief 要素数を返す．
  virtual
  int
  size() const = 0;

  /// @brief スカラーのとき true を返す．
  virtual
  bool
  is_scalar() const = 0;

  /// @brief ベクターのとき true を返す．
  virtual
  bool
  is_vector() const = 0;

  /// @brief 配列の次元数を返す．
  virtual
  ymuint32
  array_dim() const = 0;

  /// @brief automatic に定義されたとき true を返す．
  virtual
  bool
  is_automatic() const = 0;
  
  /// @brief valid の時 true を返す．
  virtual
  bool
  is_valid() const = 0;
  
  /// @brief この要素を含む frame を返す．
  virtual
  VpiFrame*
  frame() const = 0;

  /// @brief 範囲のリストの反復子を返す．
  virtual
  VpiRangeIteratorImpl*
  range_iterator_obj() = 0;

  /// @brief 範囲のリストの反復子を返す．
  virtual
  VpiRangeIterator
  range_iterator() = 0;

  /// @brief reg のリストの反復子を返す．
  virtual
  VpiRegIteratorImpl*
  reg_iterator_obj() = 0;

  /// @brief reg のリストの反復子を返す．
  virtual
  VpiRegIterator
  reg_iterator() = 0;

};


//////////////////////////////////////////////////////////////////////
/// @class VpiRegs VpiRegs.h <ym_verilog/vpi/VpiRegs.h>
/// @brief Reg/RegBit に共通な基底クラス
/// IEEE Std 1364-2001 26.6.7 Regs and reg arrays
//////////////////////////////////////////////////////////////////////
class VpiRegs :
  public VpiNamedObj
{
public:
  //////////////////////////////////////////////////////////////////////
  // VpiRegs に固有の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ビット幅を返す．
  virtual
  int
  size() const = 0;
  
  /// @brief 配列要素の時 true を返す．
  virtual
  bool
  is_array() const = 0;
  
  /// @brief 多次元配列の要素の時 true を返す．
  virtual
  bool
  is_multi_array() const = 0;

  /// @brief 符号付の時 true を返す．
  virtual
  bool
  is_signed() const = 0;

  /// @brief スカラーのとき true を返す．
  virtual
  bool
  is_scalar() const = 0;

  /// @brief ベクターのとき true を返す．
  virtual
  bool
  is_vector() const = 0;

  /// @brief automatic に定義されたとき true を返す．
  virtual
  bool
  is_automatic() const = 0;
  
  /// @brief valid の時 true を返す．
  virtual
  bool
  is_valid() const = 0;
  
  /// @brief この要素を含む frame を返す．
  virtual
  VpiFrame*
  frame() const = 0;

  /// @brief 範囲の MSB を返す．
  virtual
  VpiExpr*
  left_range() const = 0;

  /// @brief 範囲の LSB を返す．
  virtual
  VpiExpr*
  right_range() const = 0;
  
  /// @brief 初期値を得る．
  virtual
  VpiExpr*
  expr() const = 0;
  
  /// @brief path term を返す．
  /// @todo これ Iterator の間違いじゃない？ 要確認
  virtual
  VpiPathTerm*
  path_term() const = 0;

  /// @brief tchk term を返す．
  /// @todo これ Iterator の間違いじゃない？ 要確認
  virtual
  VpiTchkTerm*
  tchk_term() const = 0;

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

  /// @brief ドライバのリストの反復子を返す．
  virtual
  VpiDriverIteratorImpl*
  driver_iterator_obj() = 0;

  /// @brief ドライバのリストの反復子を返す．
  virtual
  VpiDriverIterator
  driver_iterator() = 0;
  
  /// @brief 負荷のリストの反復子を返す．
  virtual
  VpiLoadIteratorImpl*
  load_iterator_obj() = 0;
  
  /// @brief 負荷のリストの反復子を返す．
  virtual
  VpiLoadIterator
  load_iterator() = 0;
  
  /// @brief continuous assign のリストの反復子を返す．
  virtual
  VpiCaCommonIteratorImpl*
  cont_assign_iterator_obj() = 0;
  
  /// @brief continuous assign のリストの反復子を返す．
  virtual
  VpiCaCommonIterator
  cont_assign_iterator() = 0;
  
  /// @brief prim term のリストの反復子を返す．
  virtual
  VpiPrimTermIteratorImpl*
  prim_term_iterator_obj() = 0;
  
  /// @brief prim term のリストの反復子を返す．
  virtual
  VpiPrimTermIterator
  prim_term_iterator() = 0;
  
  /// @brief path term のリストの反復子を返す．
  virtual
  VpiPathTermIteratorImpl*
  path_term_iterator_obj() = 0;
  
  /// @brief path term のリストの反復子を返す．
  virtual
  VpiPathTermIterator
  path_term_iterator() = 0;

  /// @brief tchk term のリストの反復子を返す．
  virtual
  VpiTchkTermIteratorImpl*
  tchk_term_iterator_obj() = 0;

  /// @brief tchk term のリストの反復子を返す．
  virtual
  VpiTchkTermIterator
  tchk_term_iterator() = 0;

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
/// @class VpiReg VpiRegs.h <ym_verilog/vpi/VpiRegs.h>
/// @brief REG のベクタを表す基底クラス
/// IEEE Std 1364-2001 26.6.7 Regs and reg arrays
//////////////////////////////////////////////////////////////////////
class VpiReg :
  public VpiRegs
{
public:
  //////////////////////////////////////////////////////////////////////
  // VpiReg に固有の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ビット要素を生成する．
  virtual
  void
  instantiate_elem(VpiFactory* factory) = 0;

  /// @brief 初期値を設定する．
  virtual
  void
  set_expr(VpiExpr* init) = 0;

  /// @brief オフセットからインデックスを得る．
  virtual
  int
  offset2index(int offset) const = 0;

  /// @brief 配列の要素の場合に親の配列を返す．
  virtual
  VpiRegArray*
  parent() const = 0;

  /// @brief 配列要素の場合にインデックスリストの反復子を返す．
  virtual
  VpiIndexIteratorImpl*
  index_iterator_obj() = 0;

  /// @brief 配列要素の場合にインデックスリストの反復子を返す．
  virtual
  VpiIndexIterator
  index_iterator() = 0;
  
  /// @brief 子供の RegBit のリストの反復子を得る．
  virtual
  VpiRegBitIteratorImpl*
  bit_iterator_obj() = 0;
  
  /// @brief 子供の RegBit のリストの反復子を得る．
  virtual
  VpiRegBitIterator
  bit_iterator() = 0;

};


//////////////////////////////////////////////////////////////////////
/// @class VpiRegBit VpiRegs.h <ym_verilog/vpi/VpiRegs.h>
/// @brief REGの1ビット分を表すタイプ
/// IEEE Std 1364-2001 26.6.7 Regs and reg arrays
//////////////////////////////////////////////////////////////////////
class VpiRegBit :
  public VpiRegs
{
public:
  //////////////////////////////////////////////////////////////////////
  // VpiRegBit に固有の仮想関数
  //////////////////////////////////////////////////////////////////////
  
  /// @brief ビット選択が定数の時 true を返す．
  virtual
  bool
  is_constant_select() const = 0;

  /// @brief 初期値を設定する．
  virtual
  void
  set_init(VpiExpr* init) = 0;

  /// @brief 親のRegを返す．
  virtual
  VpiReg*
  parent() const = 0;

  /// @brief ビット選択を表す式を返す．
  virtual
  VpiExpr*
  index() const = 0;
  
  /// @brief 値を返す．
  virtual
  tVpiScalarVal
  scalar_value() const = 0;

  /// @brief ビット選択と配列の要素インデックスをマージしたリストの反復子を返す．
  virtual
  VpiIndexIteratorImpl*
  index_iterator_obj() = 0;

  /// @brief ビット選択と配列の要素インデックスをマージしたリストの反復子を返す．
  virtual
  VpiIndexIterator
  index_iterator() = 0;

};

END_NAMESPACE_YM_VERILOG

#endif // YM_VERILOG_VPI_VPIREGS_H
