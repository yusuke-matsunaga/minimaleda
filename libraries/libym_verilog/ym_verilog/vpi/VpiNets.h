#ifndef YM_VERILOG_VPI_VPINETS_H
#define YM_VERILOG_VPI_VPINETS_H

/// @file ym_verilog/vpi/VpiNets.h
/// @brief Net 宣言要素のクラスのヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: VpiNets.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2006 Yusuke Matsunaga
/// All rights reserved.


#include <ym_verilog/vpi/VpiNamedObj.h>
#include <ym_verilog/vpi/VpiIterator.h>


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class VpiNetArray VpiNets.h <ym_verilog/vpi/VpiNets.h>
/// @brief ネットの配列を表すクラス
/// IEEE Std 1364-2001 26.6.6 Nets and net arrays
//////////////////////////////////////////////////////////////////////
class VpiNetArray :
  public VpiNamedObj
{
public:
  //////////////////////////////////////////////////////////////////////
  // VpiNetArray に固有の仮想関数
  //////////////////////////////////////////////////////////////////////
	   
  /// @brief 配列要素を生成する．
  virtual
  void
  instantiate_elem(VpiFactory* factory) = 0;

  /// @brief 遅延式を設定する．
  virtual
  void
  set_delay(VpiExpr* delay) = 0;
  
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

  /// @brief 範囲のリストの反復子を返す．
  virtual
  VpiRangeIteratorImpl*
  range_iterator_obj() = 0;

  /// @brief 範囲のリストの反復子を返す．
  virtual
  VpiRangeIterator
  range_iterator() = 0;

  /// @brief net のリストの反復子を返す．
  virtual
  VpiNetIteratorImpl*
  net_iterator_obj() = 0;

  /// @brief net のリストの反復子を返す．
  virtual
  VpiNetIterator
  net_iterator() = 0;
  
};


//////////////////////////////////////////////////////////////////////
/// @class VpiNets VpiNets.h <ym_verilog/vpi/VpiNets.h>
/// @brief Net/NetBit に共通な基底クラス
/// IEEE Std 1364-2001 26.6.6 Nets and net arrays
//////////////////////////////////////////////////////////////////////
class VpiNets :
  public VpiNamedObj
{
public:
  //////////////////////////////////////////////////////////////////////
  // VpiNets に固有の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ビット幅を返す．
  virtual
  int
  size() const = 0;
  
  /// @brief net type を返す．
  virtual
  tVpiNetType
  net_type() const = 0;
  
  /// @brief resolved net type を返す．
  virtual
  tVpiNetType
  resolved_net_type() const = 0;
  
  /// @brief 0 の強さを返す．
  virtual
  tVpiStrength
  strength0() const = 0;
  
  /// @brief 1 の強さを返す．
  virtual
  tVpiStrength
  strength1() const = 0;

  /// @brief チャージの強さを返す．
  virtual
  tVpiStrength
  charge_strength() const = 0;
    
  /// @brief 配列要素の時 true を返す．
  virtual
  bool
  is_array() const = 0;
  
  /// @brief 多次元配列の要素の時 true を返す．
  virtual
  bool
  is_multi_array() const = 0;

  /// @brief スカラーのとき true を返す．
  virtual
  bool
  is_scalar() const = 0;

  /// @brief ベクターのとき true を返す．
  virtual
  bool
  is_vector() const = 0;
  
  /// @brief 暗黙に宣言されていたら true を返す．
  virtual
  bool
  is_implicit_decl() const = 0;
  
  /// @brief 宣言中にアサイン文があれば true を返す．
  virtual
  bool
  has_net_decl_assign() const = 0;
  
  /// @brief scalar 宣言されていたら true を返す．
  virtual
  bool
  is_explicit_scalared() const = 0;
  
  /// @brief vector 宣言されていたら true を返す．
  virtual
  bool
  is_explicit_vectored() const = 0;

  /// @brief 符号つきなら true を返す。
  virtual
  bool
  is_signed() const = 0;

  /// @brief 拡張されていたら？ true を返す．
  virtual
  bool
  is_expanded() const = 0;

  /// @brief 範囲のMSBを返す．
  virtual
  VpiExpr*
  left_range() const = 0;
  
  /// @brief 範囲のLSBを返す．
  virtual
  VpiExpr*
  right_range() const = 0;

  /// @brief 遅延値を返す．
  virtual
  VpiExpr*
  delay() const = 0;

  /// @brief simulated net を返す．
  virtual
  VpiNets*
  sim_net() const = 0;

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

  /// @brief ローカルドライバのリストの反復子を返す．
  virtual
  VpiLocalDriverIteratorImpl*
  local_driver_iterator_obj() = 0;

  /// @brief ローカルドライバのリストの反復子を返す．
  virtual
  VpiLocalDriverIterator
  local_driver_iterator() = 0;
  
  /// @brief ローカルな負荷のリストの反復子を返す．
  virtual
  VpiLocalLoadIteratorImpl*
  local_load_iterator_obj() = 0;
  
  /// @brief ローカルな負荷のリストの反復子を返す．
  virtual
  VpiLocalLoadIterator
  local_load_iterator() = 0;

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
/// @class VpiNet VpiNets.h <ym_verilog/vpi/VpiNets.h>
/// @brief ネットのベクタを表す基底クラス
/// IEEE Std 1364-2001 26.6.6 Nets and net arrays
//////////////////////////////////////////////////////////////////////
class VpiNet :
  public VpiNets
{
public:
  //////////////////////////////////////////////////////////////////////
  // VpiNet に固有の仮想関数
  //////////////////////////////////////////////////////////////////////
  
  /// @brief ビット要素を生成する．
  virtual
  void
  instantiate_elem(VpiFactory* factory) = 0;

  /// @brief 遅延式を設定する．
  virtual
  void
  set_delay(VpiExpr* delay) = 0;
  
  /// @brief オフセットからインデックスを得る．
  virtual
  int
  offset2index(int offset) const = 0;

  /// @brief このネットが配列の要素の場合に親の配列を返す．
  virtual
  VpiNetArray*
  parent() const = 0;

  /// @brief 配列要素の場合にインデックスリストの反復子を返す．
  virtual
  VpiIndexIteratorImpl*
  index_iterator_obj() = 0;

  /// @brief 配列要素の場合にインデックスリストの反復子を返す．
  virtual
  VpiIndexIterator
  index_iterator() = 0;

  /// @brief 子供の NetBit のリストの反復子を得る．
  virtual
  VpiNetBitIteratorImpl*
  bit_iterator_obj() = 0;

  /// @brief 子供の NetBit のリストの反復子を得る．
  virtual
  VpiNetBitIterator
  bit_iterator() = 0;

};


//////////////////////////////////////////////////////////////////////
/// @class VpiNetBit VpiNets.h <ym_verilog/vpi/VpiNets.h>
/// @brief ネットの1ビット分を表すタイプ
/// IEEE Std 1364-2001 26.6.6 Nets and net arrays
//////////////////////////////////////////////////////////////////////
class VpiNetBit :
  public VpiNets
{
public:
  //////////////////////////////////////////////////////////////////////
  // VpiNetBit に固有の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ビット選択が定数の時 true を返す．
  virtual
  bool
  is_constant_select() const = 0;

  /// @brief 親のネットを返す．
  virtual
  VpiNet*
  parent() const = 0;

  /// @brief ビット選択式を返す．
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

#endif // YM_VERILOG_VPI_VPINETS_H
