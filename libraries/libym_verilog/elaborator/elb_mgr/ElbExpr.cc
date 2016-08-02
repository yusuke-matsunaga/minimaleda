
/// @file libym_verilog/elb/ElbExpr.cc
/// @brief ElbExpr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: ElbExpr.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ElbExpr.h"

#include "ym_verilog/BitVector.h"
#include "ym_verilog/VlTime.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス ElbExpr
//////////////////////////////////////////////////////////////////////

// @brief Verilog-HDL の文字列を得る．
string
ElbExpr::decompile() const
{
  return decompile_impl(0);
}

// @brief 式のビット幅を返す．
ymuint32
ElbExpr::bit_size() const
{
  return unpack_size(value_type());
}

// @brief 定数の時 true を返す．
// @note このクラスは false を返す．
bool
ElbExpr::is_const() const
{
  return false;
}

// @brief プライマリ(net/reg/variables/parameter)の時に true を返す．
bool
ElbExpr::is_primary() const
{
  return false;
}

// @brief ビット指定の時に true を返す．
bool
ElbExpr::is_bitselect() const
{
  return false;
}

// @brief 範囲指定の時に true を返す．
bool
ElbExpr::is_partselect() const
{
  return false;
}

// @brief 範囲指定のモードを返す．
tVpiRangeMode
ElbExpr::range_mode() const
{
  return kVpiNoRange;
}

// @brief 演算子の時に true を返す．
bool
ElbExpr::is_operation() const
{
  return false;
}

// @brief 関数呼び出しの時に true を返す．
bool
ElbExpr::is_funccall() const
{
  return false;
}

// @brief システム関数よびあどい時に true を返す．
bool
ElbExpr::is_sysfunccall() const
{
  return false;
}

// @brief 固定選択子の時 true を返す．
// @note ビット選択，部分選択の時，意味を持つ．
// @note このクラスでは false を返す．
bool
ElbExpr::is_constant_select() const
{
  return false;
}

// @brief インデックス式を返す．
// @note ビット選択の時，意味を持つ．
// @note このクラスでは NULL を返す．
const VlExpr*
ElbExpr::index() const
{
  return NULL;
}

// @brief インデックス値を返す．
// @note 式に対するビット選択の時，意味を持つ．
// @note このクラスでは 0 を返す．
int
ElbExpr::index_val() const
{
  return 0;
}

// @brief 範囲の MSB を返す．
// @note 部分選択の時，意味を持つ．
// @note このクラスでは NULL を返す．
const VlExpr*
ElbExpr::left_range() const
{
  return NULL;
}

// @brief 範囲の MSB の値を返す．
// @note 式に対する範囲選択の時，意味を持つ．
// @note このクラスでは 0 を返す．
int
ElbExpr::left_range_val() const
{
  return 0;
}

// @brief 範囲の LSB を返す．
// @note 部分選択の時，意味を持つ．
// @note このクラスでは NULL を返す．
const VlExpr*
ElbExpr::right_range() const
{
  return NULL;
}

// @brief 範囲の LSB の値を返す．
// @note 式に対する範囲選択の時，意味を持つ．
// @note このクラスでは 0 を返す．
int
ElbExpr::right_range_val() const
{
  return 0;
}

// @brief 範囲のビット幅を表す式を返す．
// @note 可変範囲選択の時，意味を持つ．
// @note それ以外では NULL を返す．
const VlExpr*
ElbExpr::range_width() const
{
  return NULL;
}

// @brief 範囲のビット幅を返す．
// @note 可変範囲選択の時，意味を持つ．
// @note それ以外では 0 を返す．
int
ElbExpr::range_width_val() const
{
  return 0;
}

// @brief 親の式を返す．
// @note 式に対するビット選択/範囲選択の時，意味を持つ．
// @note このクラスでは NULL を返す．
const VlExpr*
ElbExpr::parent_expr() const
{
  return NULL;
}

// @brief 演算子のタイプを返す．
// @note 演算子の時，意味を持つ．
// @note このクラスでは kVpiNullOp を返す．
tVpiOpType
ElbExpr::op_type() const
{
  return kVpiNullOp;
}

// @brief オペランド数を返す．
// @note 演算子の時，意味を持つ．
// @note このクラスでは 0 を返す．
ymuint32
ElbExpr::operand_num() const
{
  return 0;
}

// @brief オペランドを返す．
// @param[in] pos 位置番号
// @note 演算子の時，意味を持つ．
const VlExpr*
ElbExpr::operand(ymuint32 pos) const
{
  return _operand(pos);
}

// @brief 定数の型を返す．
// @note 定数の時，意味を持つ．
// @note このクラスでは動作は不定
tVpiConstType
ElbExpr::constant_type() const
{
  // とは言っても何か返さなければならない．
  return kVpiDecConst;
}

// @brief 対象の関数を返す．
// @note function call の時，意味を持つ．
// @note このクラスでは NULL を返す．
const VlTaskFunc*
ElbExpr::function() const
{
  return NULL;
}

// @brief 対象のシステム関数を返す．
// @note system function call の時，意味を持つ．
// @note このクラスでは NULL を返す．
const VlUserSystf*
ElbExpr::user_systf() const
{
  return NULL;
}

// @brief 引数の数を返す．
// @note kVpiFuncCall/kVpiSysFuncCall の時，意味を持つ．
// @note このクラスでは 0 を返す．
ymuint32
ElbExpr::argument_num() const
{
  return 0;
}

// @brief 引数を返す．
// @param[in] pos 位置番号 ( 0 <= pos < argument_num() )
// @note kVpiFuncCall/kVpiSysFuncCall の時，意味を持つ．
// @note このクラスでは NULL を返す．
const VlExpr*
ElbExpr::argument(ymuint32 pos) const
{
  return NULL;
}

// @brief 対象のオブジェクトを返す．
// @note プライマリ，ビット選択，部分選択の時，意味を持つ．
// @note このクラスでは NULL を返す．
const VlDecl*
ElbExpr::decl_obj() const
{
  return NULL;
}

// @brief 配列型宣言要素への参照の場合，配列の次元を返す．
// @note このクラスでは 0 を返す．
ymuint32
ElbExpr::declarray_dimension() const
{
  return 0;
}

// @brief 配列型宣言要素への参照の場合，配列のインデックスを返す．
// @param[in] pos 位置番号 ( 0 <= pos < declarray_dimension() )
// @note このクラスでは NULL を返す．
const VlExpr*
ElbExpr::declarray_index(ymuint32 pos) const
{
  return NULL;
}

// @brief 対象のオブジェクトを返す．
// @note 対象がスコープの時，意味を持つ．
// @note このクラスでは NULL を返す．
const VlNamedObj*
ElbExpr::scope_obj() const
{
  return NULL;
}

// @brief 対象のオブジェクトを返す．
// @note 対象が ElbPrimitive の時，意味を持つ．
// @note このクラスでは NULL を返す．
const VlPrimitive*
ElbExpr::primitive_obj() const
{
  return NULL;
}

// @brief int 型の値を返す．
bool
ElbExpr::eval_int(int& val) const
{
  BitVector bv;
  eval_bitvector(bv);
  if ( bv.is_int() ) {
    val = bv.to_int();
    return true;
  }
  return false;
}

// @brief 論理値を返す．
bool
ElbExpr::eval_bool() const
{
  return ( eval_logic() == kVpiScalar1 );
}

// @brief VlTime 型の値を返す．
bool
ElbExpr::eval_time(VlTime& val) const
{
  BitVector bv;
  eval_bitvector(bv);
  if ( bv.is_time() ) {
    val = bv.to_time();
    return true;
  }
  return false;
}

// @brief オペランドを返す．
// @param[in] pos 位置番号
// @note 演算子の時，意味を持つ．
// @note このクラスでは NULL を返す．
ElbExpr*
ElbExpr::_operand(ymuint32 pos) const
{
  return NULL;
}

// 二項演算のタイプとサイズを決める．
tVpiValueType
ElbExpr::calc_type(tVpiValueType type0,
		   tVpiValueType type1)
{
  // どちらか一方でも real なら答は real
  if ( type0 == kVpiValueReal || type1 == kVpiValueReal ) {
    return kVpiValueReal;
  }

  bool is_sized = false;
  bool is_signed = false;

  // 両方が signed の場合にのみ signed
  if ( is_signed_type(type0) && is_signed_type(type1) ) {
    is_signed = true;
  }

  // どちらか一方が sized なら結果も sized
  if ( is_sized_type(type0) || is_sized_type(type1) ) {
    is_sized = true;
  }

  if ( is_sized ) {
    ymuint32 size0 = unpack_size(type0);
    ymuint32 size1 = unpack_size(type1);
    ymuint32 ans_size = ( size0 > size1 ) ? size0 : size1;
    if ( is_signed ) {
      return pack(kVpiValueSS, ans_size);
    }
    else {
      return pack(kVpiValueUS, ans_size);
    }
  }
  else {
    if ( is_signed ) {
      return pack(kVpiValueSU, 0);
    }
    else {
      return pack(kVpiValueUU, 0);
    }
  }
}

// 巾乗演算のタイプとサイズを決める．
tVpiValueType
ElbExpr::calc_type2(tVpiValueType type0,
		    tVpiValueType type1)
{
  // 両方が unsigned でなければ結果は real
  if ( is_signed_type(type0) || is_signed_type(type1) ) {
    return kVpiValueReal;
  }

  // どちらか一方が sized なら結果も sized
  if ( is_sized_type(type0) || is_sized_type(type1) ) {
    ymuint32 size0 = unpack_size(type0);
    ymuint32 size1 = unpack_size(type1);
    ymuint32 ans_size = ( size0 > size1 ) ? size0 : size1;
    return pack(kVpiValueUS, ans_size);
  }
  else {
    return pack(kVpiValueUU, 0);
  }
}

// 出力に要求されているサイズから自分のサイズを決める．
tVpiValueType
ElbExpr::update_size(tVpiValueType type,
		     tVpiValueType req_type)
{
  if ( req_type == kVpiValueReal ) {
    // real は伝播しない．
    return type;
  }
  if ( type == kVpiValueReal ) {
    // 自分が real でも伝播しない．
    return type;
  }
  ymuint32 req_size = unpack_size(req_type);
  ymuint32 size = unpack_size(type);
  if ( req_size <= size ) {
    return type;
  }
  return pack(unpack_type(type), req_size);
}

END_NAMESPACE_YM_VERILOG
