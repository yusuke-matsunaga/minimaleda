#ifndef YM_VERILOG_VL_VLEXPR_H
#define YM_VERILOG_VL_VLEXPR_H

/// @file ym_verilog/vl/VlExpr.h
/// @brief VlExpr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: VlExpr.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_verilog/vl/VlObj.h"
#include "ym_verilog/vl/VlFwd.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class VlExpr VlExpr.h <ym_verilog/vl/VlExpr.h>
/// @brief エラボレーション中の expression を表す基底クラス
//////////////////////////////////////////////////////////////////////
class VlExpr :
  public VlObj
{
protected:

  /// @brief デストラクタ
  virtual
  ~VlExpr() { }


public:
  //////////////////////////////////////////////////////////////////////
  // VlExpr の派生クラスに共通な仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 式の値のタイプを返す．
  virtual
  tVpiValueType
  value_type() const = 0;

  /// @brief Verilog-HDL の文字列を得る．
  virtual
  string
  decompile() const = 0;

  /// @brief 式のビット幅を返す．
  virtual
  ymuint32
  bit_size() const = 0;

  /// @brief 定数式の時 true を返す．
  virtual
  bool
  is_const() const = 0;

  /// @brief 部分/ビット指定が定数の時 true を返す．
  /// @note kVpiPartSelect/kVpiBitSelect の時，意味を持つ．
  /// @note それ以外では常に false を返す．
  virtual
  bool
  is_constant_select() const = 0;

  /// @brief プライマリ(net/reg/variables/parameter)の時に true を返す．
  virtual
  bool
  is_primary() const = 0;

  /// @brief ビット指定の時に true を返す．
  virtual
  bool
  is_bitselect() const = 0;

  /// @brief 範囲指定の時に true を返す．
  virtual
  bool
  is_partselect() const = 0;

  /// @brief 範囲指定のモードを返す．
  /// @note is_partselect() == true の時のみ意味を持つ．
  virtual
  tVpiRangeMode
  range_mode() const = 0;

  /// @brief 演算子の時に true を返す．
  virtual
  bool
  is_operation() const = 0;

  /// @brief 関数呼び出しの時に true を返す．
  virtual
  bool
  is_funccall() const = 0;

  /// @brief システム関数呼び出しの時に true を返す．
  virtual
  bool
  is_sysfunccall() const = 0;

  /// @brief 宣言要素への参照の場合，対象のオブジェクトを返す．
  /// @note 宣言要素に対するビット選択，部分選択の場合にも意味を持つ．
  /// @note それ以外では NULL を返す．
  virtual
  const VlDecl*
  decl_obj() const = 0;

  /// @brief 配列型宣言要素への参照の場合，配列の次元を返す．
  /// @note それ以外では 0 を返す．
  virtual
  ymuint32
  declarray_dimension() const = 0;

  /// @brief 配列型宣言要素への参照の場合，配列のインデックスを返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < declarray_dimension() )
  /// @note それ以外では NULL を返す．
  virtual
  const VlExpr*
  declarray_index(ymuint32 pos) const = 0;

  /// @brief スコープへの参照の場合，対象のオブジェクトを返す．
  /// @note それ以外では NULL を返す．
  virtual
  const VlNamedObj*
  scope_obj() const = 0;

  /// @brief primitive への参照の場合，対象のオブジェクトを返す．
  virtual
  const VlPrimitive*
  primitive_obj() const = 0;

  /// @brief 親の式を返す．
  /// @note 式に対するビット選択/範囲選択の時，意味を持つ．
  /// @note それ以外では NULL を返す．
  virtual
  const VlExpr*
  parent_expr() const = 0;

  /// @brief インデックス式を返す．
  /// @note 通常のビット選択の時，意味を持つ．
  /// @note それ以外では NULL を返す．
  virtual
  const VlExpr*
  index() const = 0;

  /// @brief インデックス値を返す．
  /// @note 式に対するビット選択の時，意味を持つ．
  /// @note それ以外では 0 を返す．
  virtual
  int
  index_val() const = 0;

  /// @brief 範囲の MSB の式を返す．
  /// @note 通常の範囲選択の時，意味を持つ．
  /// @note それ以外では NULL を返す．
  virtual
  const VlExpr*
  left_range() const = 0;

  /// @brief 範囲の MSB の値を返す．
  /// @note 式に対する範囲選択の時，意味を持つ．
  /// @note それ以外では 0 を返す．
  virtual
  int
  left_range_val() const = 0;

  /// @brief 範囲の LSB の式を返す．
  /// @note 通常の範囲選択の時，意味を持つ．
  /// @note それ以外では NULL を返す．
  virtual
  const VlExpr*
  right_range() const = 0;

  /// @brief 範囲の LSB の値を返す．
  /// @note 式に対する範囲選択の時，意味を持つ．
  /// @note それ以外では 0 を返す．
  virtual
  int
  right_range_val() const = 0;

  /// @brief 範囲のビット幅を表す式を返す．
  /// @note 可変範囲選択の時，意味を持つ．
  /// @note それ以外では NULL を返す．
  virtual
  const VlExpr*
  range_width() const = 0;

  /// @brief 範囲のビット幅を返す．
  /// @note 可変範囲選択の時，意味を持つ．
  /// @note それ以外では 0 を返す．
  virtual
  int
  range_width_val() const = 0;

  /// @brief 演算子の型を返す．
  /// @note kVpiOperation の時，意味を持つ．
  /// @note それ以外では動作は不定
  virtual
  tVpiOpType
  op_type() const = 0;

  /// @brief オペランド数を返す．
  /// @note kVpiOperation の時，意味を持つ．
  /// @note それ以外では 0 を返す．
  virtual
  ymuint32
  operand_num() const = 0;

  /// @brief オペランドを返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < operand_num() )
  /// @note kVpiOperation の時，意味を持つ．
  /// @note それ以外では NULL を返す．
  virtual
  const VlExpr*
  operand(ymuint32 pos) const = 0;

  /// @brief 定数型を返す．
  /// @note kVpiConstant の時，意味を持つ．
  /// @note それ以外では動作は不定
  virtual
  tVpiConstType
  constant_type() const = 0;

  /// @brief 対象の関数を返す．
  /// @note kVpiFuncCall の時，意味を持つ．
  /// @note それ以外では NULL を返す．
  virtual
  const VlTaskFunc*
  function() const = 0;

  /// @brief 対象のシステム関数を返す．
  /// @note kVpiSysFuncCall の時，意味を持つ．
  /// @note それ以外では NULL を返す．
  virtual
  const VlUserSystf*
  user_systf() const = 0;

  /// @brief 引数の数を返す．
  /// @note kVpiFuncCall/kVpiSysFuncCall の時，意味を持つ．
  virtual
  ymuint32
  argument_num() const = 0;

  /// @brief 引数を返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < argument_num() )
  /// @note kVpiFuncCall/kVpiSysFuncCall の時，意味を持つ．
  /// @note それ以外では NULL を返す．
  virtual
  const VlExpr*
  argument(ymuint32 pos) const = 0;

  /// @brief int 型の値を返す．
  /// @param[out] val 結果を格納する変数
  /// @return 整数値に変換できたら true を返す．
  virtual
  bool
  eval_int(int& val) const = 0;

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
  /// @param[out] val 結果を格納する変数
  /// @return VlTime 値に変換できたら true を返す．
  /// @note eval_bitvector() の結果から変換する．
  virtual
  bool
  eval_time(VlTime& val) const = 0;

  /// @brief bitvector 型の値を返す．
  virtual
  void
  eval_bitvector(BitVector& bitvector,
		 tVpiValueType req_type = kVpiValueNone) const = 0;

};

END_NAMESPACE_YM_VERILOG

#endif // YM_VERILOG_VL_VLEXPR_H
