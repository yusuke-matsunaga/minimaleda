#ifndef YM_VERILOG_VPI_VPIPRIMITIVE_H
#define YM_VERILOG_VPI_VPIPRIMITIVE_H

/// @file ym_verilog/vpi/VpiPrimitive.h
/// @brief Primitive を表すクラスのヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: VpiPrimitive.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2006 Yusuke Matsunaga
/// All rights reserved.

// IEEE Std 1364-2001 26.6.13 Primitive, prim term


#include <ym_verilog/vpi/VpiNamedObj.h>
#include <ym_verilog/vpi/VpiIterator.h>


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class VpiPrimArray VpiPrimitive.h <ym_verilog/vpi/VpiPrimitive.h>
/// @brief primitive arrayを表すクラス
/// IEEE Std 1364-2001 26.6.13 Primitive, prim term
//////////////////////////////////////////////////////////////////////
class VpiPrimArray :
  public VpiNamedObj
{
public:
  /// @brief 親クラスのエイリアス
  typedef VpiNamedObj Parent;


public:
  //////////////////////////////////////////////////////////////////////
  // VpiPrimArray に固有の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 要素を生成する．
  virtual
  void
  instantiate_elem(VpiFactory* factory) = 0;

  /// @brief index 番目の要素を返す．
  virtual
  VpiPrimitive*
  elem(int index) const = 0;
  
  /// @brief 要素数を返す．
  virtual
  int
  size() const = 0;

  /// @brief primitive type を返す．
  virtual
  tVpiPrimType
  prim_type() const = 0;
  
  /// @brief 範囲のMSBを返す．
  virtual
  VpiExpr*
  left_range() const = 0;
  
  /// @brief 範囲のLSBを返す．
  virtual
  VpiExpr*
  right_range() const = 0;
  
  /// @brief delay を得る．
  virtual
  VpiExpr*
  delay() const = 0;

  /// @brief 要素のリストを表す VpiExpr を返す．
  virtual
  VpiExpr*
  expr() const = 0;

  /// @brief プリミティブ配列の最初の反復子を返す．
  virtual
  VpiPrimitiveIteratorImpl*
  primitive_iterator_obj() = 0;

  /// @brief プリミティブ配列の最初の反復子を返す．
  virtual
  VpiPrimitiveIterator
  primitive_iterator() = 0;

};


//////////////////////////////////////////////////////////////////////
/// @class VpiPrimitive VpiPrimitive.h <ym_verilog/vpi/VpiPrimitive.h>
/// @brief primitive の基底クラス
/// IEEE Std 1364-2001 26.6.13 Primitive, prim term
//////////////////////////////////////////////////////////////////////
class VpiPrimitive :
  public VpiNamedObj
{
public:
  /// @brief 親クラスのエイリアス
  typedef VpiNamedObj Parent;


public:
  //////////////////////////////////////////////////////////////////////
  // VpiPrimitive に固有の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief pos 番目の端子に port_expr を接続する．
  virtual
  void
  connect(ymuint32 pos,
	  VpiExpr* port_expr) = 0;

  /// @brief 遅延を設定する．
  virtual
  void
  set_delay(VpiExpr* delay) = 0;
  
  /// @brief primitive type を返す．
  virtual
  tVpiPrimType
  prim_type() const = 0;
  
  /// @brief primitive type を表す文字列返す．
  virtual
  string
  s_prim_type() const = 0;
  
  /// @brief 配列要素の時 true を返す．
  virtual
  bool
  is_array() const = 0;
  
  /// @brief 多次元配列の要素の時 true を返す．
  virtual
  bool
  is_multi_array() const = 0;

  /// @brief 入力数を返す．
  virtual
  int
  size() const = 0;
  
  /// @brief 0 の強さを返す．
  virtual
  tVpiStrength
  strength0() const = 0;
  
  /// @brief 1 の強さを返す．
  virtual
  tVpiStrength
  strength1() const= 0;

  /// @brief 定義されたプリミティブ名を返す．
  virtual
  string
  def_name() const = 0;

  /// @brief 配列の要素の場合に親の配列を返す．
  virtual
  VpiPrimArray*
  primitive_array() const = 0;

  /// @brief 配列要素の場合にインデックスを返す．
  virtual
  VpiExpr*
  index() const = 0;
  
  /// @brief 遅延を表す式を得る．
  virtual
  VpiExpr*
  delay() const = 0;

  /// @brief UDP 定義を返す．
  /// UDP タイプの時のみ有効．\n
  /// それ以外では NULL
  virtual
  VpiUdpDefn*
  udp_defn() const = 0;
  
  /// @brief prim term のリストの反復子を返す．
  virtual
  VpiPrimTermIteratorImpl*
  prim_term_iterator_obj() = 0;

  /// @brief prim term のリストの反復子を返す．
  virtual
  VpiPrimTermIterator
  prim_term_iterator() = 0;

};


//////////////////////////////////////////////////////////////////////
/// @class VpiPrimTerm VpiPrimitive.h <ym_verilog/vpi/VpiPrimitive.h>
/// @brief primitive の端子を表すクラス
/// IEEE Std 1364-2001 26.6.13 Primitive, prim term
//////////////////////////////////////////////////////////////////////
class VpiPrimTerm :
  public VpiBase
{
public:
  /// @brief 親クラスのエイリアス
  typedef VpiBase Parent;


public:
  //////////////////////////////////////////////////////////////////////
  // VpiPrimTerm に固有の仮想関数
  //////////////////////////////////////////////////////////////////////
  
  /// @brief 入出力の種類を返す．
  virtual
  tVpiDirection
  direction() const = 0;
  
  /// @brief 端子の番号を返す．
  virtual
  int
  term_index() const = 0;

  /// @brief 接続しているネット(だと思う)を返す．
  virtual
  VpiExpr*
  expr() const = 0;

  /// @brief 親の primitive を返す．
  virtual
  VpiPrimitive*
  primitive() const = 0;
  
};

END_NAMESPACE_YM_VERILOG

#endif // YM_VERILOG_VPI_VPIPRIMITIVE_H
