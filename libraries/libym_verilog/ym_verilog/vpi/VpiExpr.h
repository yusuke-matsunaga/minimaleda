#ifndef YM_VERILOG_VPI_VPIEXPR_H
#define YM_VERILOG_VPI_VPIEXPR_H

/// @file ym_verilog/vpi/VpiExpr.h
/// @brief 式を表すクラスのヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: VpiExpr.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2006 Yusuke Matsunaga
/// All rights reserved.

// IEEE Std 1364-2001 26.6.25 Simple expressions
// IEEE Std 1364-2001 26.6.26 Expressions


#include <ym_verilog/vpi/VpiBase.h>
#include <ym_verilog/vpi/VpiIterator.h>


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class VpiExpr VpiExpr.h <ym_verilog/vpi/VpiExpr.h>
/// @brief expression を表すクラス
//////////////////////////////////////////////////////////////////////
class VpiExpr :
  public VpiBase
{
public:
  /// @brief 親クラスのエイリアス
  typedef VpiBase Parent;
  

public:
  //////////////////////////////////////////////////////////////////////
  // VpiExpr に固有の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 式のタイプを返す．
  virtual
  tVpiValueType
  expr_type() const = 0;

  /// @brief 要求される式のサイズをセットする．
  /// @param[in] type 要求される式の型
  /// @note 必要であればオペランドに対して再帰的に処理を行なう．
  virtual
  void
  set_reqsize(tVpiValueType type) = 0;

  /// @brief 要求される式のサイズを自分で決めてセットする．
  /// @note 必要であればオペランドに対して再帰的に処理を行なう．
  void
  set_selfsize()
  {
    set_reqsize(expr_type());
  }

  /// @brief 自分の複製をつくる．
  virtual
  VpiExpr*
  duplicate(VpiFactory* factory) const = 0;

  /// @brief 定数の時 true を返す．
  virtual
  bool
  is_const() const = 0;
  
  /// @brief int 型の値を返す．
  virtual
  int
  eval_int() const = 0;

  /// @brief スカラー値を返す．
  virtual
  tVpiScalarVal
  eval_scalar() const = 0;

  /// @brief 論理値を返す．
  virtual
  tVpiScalarVal
  eval_logic() const = 0;

  /// @brief 論理値を返す．
  virtual
  bool
  eval_bool() const = 0;
  
  /// @brief real 型の値を返す．
  virtual
  double
  eval_real() const = 0;

  /// @brief VlTime 型の値を返す．
  virtual
  VlTime
  eval_time() const = 0;

  /// @brief bitvector 型の値を返す．
  virtual
  void
  eval_bitvector(BitVector& bitvector,
		 tVpiValueType req_type = kVpiValueNone) const = 0;
  
  /// @brief Verilog-HDL の文字列を得る．
  string
  decompile() const
  {
    return decompile_impl(0);
  }

  /// @brief decompile() の実装関数
  virtual
  string
  decompile_impl(int ppri) const = 0;

  /// @brief pos ビット目の部分式を生成する．
  virtual
  VpiExpr*
  bit_expr(VpiFactory* factory,
		    ymuint32 pos) = 0;
  
  /// @brief [msb_offset: lsb_offset] の部分式を生成する．
  virtual
  VpiExpr*
  part_expr(VpiFactory* factory,
	    ymuint32 msb_offset,
	    ymuint32 lsb_offset) = 0;

  /// @brief port expression として用いられたときに実際の
  /// @brief net/reg の mPortsListに該当の port を追加する．
  virtual
  void
  add_port(VpiPorts* port) = 0;

  /// @brief port expression として用いられたときに実際の
  /// @brief net/reg の mPortInstList に該当の port を追加する．
  virtual
  void
  add_portinst(VpiPorts* port) = 0;

  /// @brief prim term 用の expression として用いられたときに
  /// @brief net/reg の mPrimTermList に該当の prim_term を追加する．
  virtual
  void
  add_primterm(VpiPrimTerm* prim_term) = 0;

  /// @brief その expression を用いているオブジェクトを登録する．
  virtual
  void
  add_use(VpiObj* obj) = 0;
  
  /// @brief この式が net_lvalue かどうか調べる．
  /// @note 具体的には net 系の primary 自身もしくはその定数範囲指定
  /// @note および定数ビット指定のみからなる式の時のみ true を返す．
  /// @note 上記の concatenation もOK
  virtual
  bool
  check_net_lvalue() const = 0;
  
  /// @brief この式が variable_lvalue かどうか調べる．
  /// @note 具体的には reg/variable 系の primary 自身もしくはその範囲指定
  /// @note およびビット指定のみからなるときのみ true を返す．
  /// @note 上記の concatenation も OK
  virtual
  bool
  check_var_lvalue() const = 0;

  /// @brief この式が procedural continuous assignment の左辺式かどうか調べる．
  /// @note 具体的には variable そのものか variable の concatenation のみ
  /// @note true を返す．
  virtual
  bool
  check_assign_lvalue() const = 0;

  /// @brief この式が force 文の左辺式かどうか調べる．
  /// @note 具体的には variable/net/bit-select of net/part-select of net
  /// @note もしくは上記の concatenation のみ true を返す．
  virtual
  bool
  check_force_lvalue() const = 0;

  /// @brief この式が parameter の初期値かどうか調べる．
  /// @note 具体的には parameter(localparam)/constant/constant function call
  /// @note のみを終端とする式が OK
  virtual
  bool
  check_param_rvalue() const = 0;

  /// @brief 自分を使っている terminal/ports などのリストの反復子を返す．
  virtual
  VpiUseIteratorImpl*
  use_iterator_obj() = 0;

  /// @brief 自分を使っている terminal/ports などのリストの反復子を返す．
  virtual
  VpiUseIterator
  use_iterator() = 0;

  
public:
  //////////////////////////////////////////////////////////////////////
  // VpiExpr に固有の仮想関数(左辺式用)
  //////////////////////////////////////////////////////////////////////

  /// @brief ビットベクタ値を設定する．
  virtual
  void
  set(const BitVector& bitvector) = 0;

  /// @brief 実数値を設定する．
  virtual
  void
  set(double real) = 0;

};

END_NAMESPACE_YM_VERILOG

#endif // YM_VERILOG_VPI_VPIEXPR_H
