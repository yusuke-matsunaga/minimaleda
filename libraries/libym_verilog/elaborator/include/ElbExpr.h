#ifndef LIBYM_VERILOG_ELB_ELBEXPR_H
#define LIBYM_VERILOG_ELB_ELBEXPR_H

/// @file libym_verilog/elaborator/include/ElbExpr.h
/// @brief ElbExpr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: ElbExpr.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_verilog/vl/VlExpr.h"
#include "ElbFwd.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class ElbExpr ElbExpr.h <ym_verilog/vl/VlExpr.h>
/// @brief エラボレーション中の expression を表す基底クラス
//////////////////////////////////////////////////////////////////////
class ElbExpr :
  public VlExpr
{
protected:

  /// @brief コンストラクタ
  ElbExpr() { }

  /// @brief デストラクタ
  virtual
  ~ElbExpr() { }


public:
  //////////////////////////////////////////////////////////////////////
  // VlExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief Verilog-HDL の文字列を得る．
  virtual
  string
  decompile() const;

  /// @brief 式のビット幅を返す．
  virtual
  ymuint32
  bit_size() const;

  /// @brief 定数の時 true を返す．
  /// @note このクラスは false を返す．
  virtual
  bool
  is_const() const;

  /// @brief 固定選択子の時 true を返す．
  /// @note ビット選択，部分選択の時，意味を持つ．
  /// @note このクラスでは false を返す．
  virtual
  bool
  is_constant_select() const;

  /// @brief プライマリ(net/reg/variables/parameter)の時に true を返す．
  virtual
  bool
  is_primary() const;

  /// @brief ビット指定の時に true を返す．
  virtual
  bool
  is_bitselect() const;

  /// @brief 範囲指定の時に true を返す．
  virtual
  bool
  is_partselect() const;

  /// @brief 範囲指定のモードを返す．
  virtual
  tVpiRangeMode
  range_mode() const;

  /// @brief 演算子の時に true を返す．
  virtual
  bool
  is_operation() const;

  /// @brief 関数呼び出しの時に true を返す．
  virtual
  bool
  is_funccall() const;

  /// @brief システム関数よびあどい時に true を返す．
  virtual
  bool
  is_sysfunccall() const;

  /// @brief 宣言要素への参照の場合，対象のオブジェクトを返す．
  /// @note このクラスでは NULL を返す．
  virtual
  const VlDecl*
  decl_obj() const;

  /// @brief 配列型宣言要素への参照の場合，配列の次元を返す．
  /// @note このクラスでは 0 を返す．
  virtual
  ymuint32
  declarray_dimension() const;

  /// @brief 配列型宣言要素への参照の場合，配列のインデックスを返す．
 /// @param[in] pos 位置番号 ( 0 <= pos < declarray_dimension() )
  /// @note このクラスでは NULL を返す．
  virtual
  const VlExpr*
  declarray_index(ymuint32 pos) const;

  /// @brief スコープへの参照の場合，対象のオブジェクトを返す．
  /// @note このクラスでは NULL を返す．
  virtual
  const VlNamedObj*
  scope_obj() const;

  /// @brief primitive への参照の場合，対象のオブジェクトを返す．
  virtual
  const VlPrimitive*
  primitive_obj() const;

  /// @brief 親の式を返す．
  /// @note 式に対するビット選択/範囲選択の時，意味を持つ．
  /// @note このクラスでは NULL を返す．
  virtual
  const VlExpr*
  parent_expr() const;

  /// @brief インデックス式を返す．
  /// @note ビット選択の時，意味を持つ．
  /// @note このクラスでは NULL を返す．
  virtual
  const VlExpr*
  index() const;

  /// @brief インデックス値を返す．
  /// @note 式に対するビット選択の時，意味を持つ．
  /// @note このクラスでは 0 を返す．
  virtual
  int
  index_val() const;

  /// @brief 範囲の MSB を返す．
  /// @note 部分選択の時，意味を持つ．
  /// @note このクラスでは NULL を返す．
  virtual
  const VlExpr*
  left_range() const;

  /// @brief 範囲の MSB の値を返す．
  /// @note 式に対する範囲選択の時，意味を持つ．
  /// @note このクラスでは 0 を返す．
  virtual
  int
  left_range_val() const;

  /// @brief 範囲の LSB を返す．
  /// @note 部分選択の時，意味を持つ．
  /// @note このクラスでは NULL を返す．
  virtual
  const VlExpr*
  right_range() const;

  /// @brief 範囲の LSB の値を返す．
  /// @note 式に対する範囲選択の時，意味を持つ．
  /// @note このクラスでは 0 を返す．
  virtual
  int
  right_range_val() const;

  /// @brief 範囲のビット幅を表す式を返す．
  /// @note 可変範囲選択の時，意味を持つ．
  /// @note それ以外では NULL を返す．
  virtual
  const VlExpr*
  range_width() const;

  /// @brief 範囲のビット幅を返す．
  /// @note 可変範囲選択の時，意味を持つ．
  /// @note それ以外では 0 を返す．
  virtual
  int
  range_width_val() const;

  /// @brief 演算子のタイプを返す．
  /// @note 演算子の時，意味を持つ．
  /// @note このクラスでは kVpiNullOp を返す．
  virtual
  tVpiOpType
  op_type() const;

  /// @brief オペランド数を返す．
  /// @note 演算子の時，意味を持つ．
  /// @note このクラスでは 0 を返す．
  virtual
  ymuint32
  operand_num() const;

  /// @brief オペランドを返す．
  /// @param[in] pos 位置番号
  /// @note 演算子の時，意味を持つ．
  /// @note このクラスでは NULL を返す．
  virtual
  const VlExpr*
  operand(ymuint32 pos) const;

  /// @brief 定数の型を返す．
  /// @note 定数の時，意味を持つ．
  /// @note このクラスでは動作は不定
  virtual
  tVpiConstType
  constant_type() const;

  /// @brief 対象の関数を返す．
  /// @note kVpiFuncCall の時，意味を持つ．
  /// @note このクラスでは NULL を返す．
  virtual
  const VlTaskFunc*
  function() const;

  /// @brief 対象のシステム関数を返す．
  /// @note kVpiSysFuncCall の時，意味を持つ．
  /// @note このクラスでは NULL を返す．
  virtual
  const VlUserSystf*
  user_systf() const;

  /// @brief 引数の数を返す．
  /// @note kVpiFuncCall/kVpiSysFuncCall の時，意味を持つ．
  /// @note このクラスでは 0 を返す．
  virtual
  ymuint32
  argument_num() const;

  /// @brief 引数を返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < argument_num() )
  /// @note kVpiFuncCall/kVpiSysFuncCall の時，意味を持つ．
  /// @note このクラスでは NULL を返す．
  virtual
  const VlExpr*
  argument(ymuint32 pos) const;

  /// @brief int 型の値を返す．
  /// @param[out] val 結果を格納する変数
  /// @return 整数値に変換できたら true を返す．
  /// @note eval_bitvector() の結果から変換する．
  virtual
  bool
  eval_int(int& val) const;

  /// @brief 論理値を返す．
  /// @note eval_scalar() の結果から変換する．
  virtual
  bool
  eval_bool() const;

  /// @brief VlTime 型の値を返す．
  /// @param[out] val 結果を格納する変数
  /// @return VlTime 値に変換できたら true を返す．
  /// @note eval_bitvector() の結果から変換する．
  virtual
  bool
  eval_time(VlTime& val) const;


public:
  //////////////////////////////////////////////////////////////////////
  // ElbExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 要求される式の型を計算してセットする．
  /// @param[in] type 要求される式の型
  /// @note 必要であればオペランドに対して再帰的に処理を行なう．
  virtual
  void
  set_reqsize(tVpiValueType type) = 0;

  /// @brief 要求される式のサイズを自分で決めてセットする．
  /// @note 必要であればオペランドに対して再帰的に処理を行なう．
  void
  set_selfsize();

  /// @brief decompile() の実装関数
  /// @param[in] pprim 親の演算子の優先順位
  virtual
  string
  decompile_impl(int ppri) const = 0;

  /// @brief オペランドを返す．
  /// @param[in] pos 位置番号
  /// @note 演算子の時，意味を持つ．
  /// @note このクラスでは NULL を返す．
  virtual
  ElbExpr*
  _operand(ymuint32 pos) const;

  /// @brief スカラー値を書き込む．
  /// @param[in] v 書き込む値
  /// @note 左辺式の時のみ意味を持つ．
  virtual
  void
  set_scalar(tVpiScalarVal v) = 0;

  /// @brief 実数値を書き込む．
  /// @param[in] v 書き込む値
  /// @note 左辺式の時のみ意味を持つ．
  virtual
  void
  set_real(double v) = 0;

  /// @brief ビットベクタを書き込む．
  /// @param[in] v 書き込む値
  /// @note 左辺式の時のみ意味を持つ．
  virtual
  void
  set_bitvector(const BitVector& v) = 0;


public:
  //////////////////////////////////////////////////////////////////////
  // 便利関数
  //////////////////////////////////////////////////////////////////////

  // 二項演算のタイプとサイズを決める．
  static
  tVpiValueType
  calc_type(tVpiValueType type0,
	    tVpiValueType type1);


  // 巾乗演算のタイプとサイズを決める．
  static
  tVpiValueType
  calc_type2(tVpiValueType type0,
	     tVpiValueType type1);

  // 出力に要求されているサイズから自分のサイズを決める．
  static
  tVpiValueType
  update_size(tVpiValueType type,
	      tVpiValueType req_type);

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 要求される式のサイズを自分で決めてセットする．
// @note 必要であればオペランドに対して再帰的に処理を行なう．
inline
void
ElbExpr::set_selfsize()
{
  set_reqsize(value_type());
}

END_NAMESPACE_YM_VERILOG

#endif // LIBYM_VERILOG_ELB_ELBEXPR_H
