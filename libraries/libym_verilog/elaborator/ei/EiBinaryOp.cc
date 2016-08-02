
/// @file libym_verilog/elb_impl/EiBinaryOp.cc
/// @brief EiBinaryOp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: EiBinaryOp.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "EiFactory.h"
#include "EiBinaryOp.h"

#include "ym_verilog/BitVector.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// EiFactory の生成関数
//////////////////////////////////////////////////////////////////////

// @brief 2項演算子を生成する．
// @param[in] pt_expr パース木の定義要素
// @param[in] op_type 演算子のタイプ
// @param[in] opr0 オペランド
// @param[in] opr1 オペランド
ElbExpr*
EiFactory::new_BinaryOp(const PtBase* pt_expr,
			tVpiOpType op_type,
			ElbExpr* opr0,
			ElbExpr* opr1)
{
  ElbExpr* expr = NULL;
  void* p;
  switch( op_type ) {
  case kVpiBitAndOp:
    p = mAlloc.get_memory(sizeof(EiBitAndOp));
    expr = new (p) EiBitAndOp(pt_expr, opr0, opr1);
    break;

  case kVpiBitOrOp:
    p = mAlloc.get_memory(sizeof(EiBitOrOp));
    expr = new (p) EiBitOrOp(pt_expr, opr0, opr1);
    break;

  case kVpiBitXNorOp:
    p = mAlloc.get_memory(sizeof(EiBitXnorOp));
    expr = new (p) EiBitXnorOp(pt_expr, opr0, opr1);
    break;

  case kVpiBitXorOp:
    p = mAlloc.get_memory(sizeof(EiBitXorOp));
    expr = new (p) EiBitXorOp(pt_expr, opr0, opr1);
    break;

  case kVpiAddOp:
    p = mAlloc.get_memory(sizeof(EiAddOp));
    expr = new (p) EiAddOp(pt_expr, opr0, opr1);
    break;

  case kVpiSubOp:
    p = mAlloc.get_memory(sizeof(EiSubOp));
    expr = new (p) EiSubOp(pt_expr, opr0, opr1);
    break;

  case kVpiMultOp:
    p = mAlloc.get_memory(sizeof(EiMultOp));
    expr = new (p) EiMultOp(pt_expr, opr0, opr1);
    break;

  case kVpiDivOp:
    p = mAlloc.get_memory(sizeof(EiDivOp));
    expr = new (p) EiDivOp(pt_expr, opr0, opr1);
    break;

  case kVpiModOp:
    p = mAlloc.get_memory(sizeof(EiModOp));
    expr = new (p) EiModOp(pt_expr, opr0, opr1);
    break;

  case kVpiPowerOp:
    p = mAlloc.get_memory(sizeof(EiPowerOp));
    expr = new (p) EiPowerOp(pt_expr, opr0, opr1);
    break;

  case kVpiLShiftOp:
    p = mAlloc.get_memory(sizeof(EiLShiftOp));
    expr = new (p) EiLShiftOp(pt_expr, opr0, opr1);
    break;

  case kVpiRShiftOp:
    p = mAlloc.get_memory(sizeof(EiRShiftOp));
    expr = new (p) EiRShiftOp(pt_expr, opr0, opr1);
    break;

  case kVpiArithLShiftOp:
    p = mAlloc.get_memory(sizeof(EiArithLShiftOp));
    expr = new (p) EiArithLShiftOp(pt_expr, opr0, opr1);
    break;

  case kVpiArithRShiftOp:
    p = mAlloc.get_memory(sizeof(EiArithRShiftOp));
    expr = new (p) EiArithRShiftOp(pt_expr, opr0, opr1);
    break;

  case kVpiLogAndOp:
    p = mAlloc.get_memory(sizeof(EiLogAndOp));
    expr = new (p) EiLogAndOp(pt_expr, opr0, opr1);
    break;

  case kVpiLogOrOp:
    p = mAlloc.get_memory(sizeof(EiLogOrOp));
    expr = new (p) EiLogOrOp(pt_expr, opr0, opr1);
    break;

  case kVpiCaseEqOp:
    p = mAlloc.get_memory(sizeof(EiCaseEqOp));
    expr = new (p) EiCaseEqOp(pt_expr, opr0, opr1);
    break;

  case kVpiCaseNeqOp:
    p = mAlloc.get_memory(sizeof(EiCaseNeqOp));
    expr = new (p) EiCaseNeqOp(pt_expr, opr0, opr1);
    break;

  case kVpiEqOp:
    p = mAlloc.get_memory(sizeof(EiEqOp));
    expr = new (p) EiEqOp(pt_expr, opr0, opr1);
    break;

  case kVpiNeqOp:
    p = mAlloc.get_memory(sizeof(EiNeqOp));
    expr = new (p) EiNeqOp(pt_expr, opr0, opr1);
    break;

  case kVpiGeOp:
    p = mAlloc.get_memory(sizeof(EiGeOp));
    expr = new (p) EiGeOp(pt_expr, opr0, opr1);
    break;

  case kVpiGtOp:
    p = mAlloc.get_memory(sizeof(EiGtOp));
    expr = new (p) EiGtOp(pt_expr, opr0, opr1);
    break;

  case kVpiLeOp:
    p = mAlloc.get_memory(sizeof(EiLeOp));
    expr = new (p) EiLeOp(pt_expr, opr0, opr1);
    break;

  case kVpiLtOp:
    p = mAlloc.get_memory(sizeof(EiLtOp));
    expr = new (p) EiLtOp(pt_expr, opr0, opr1);
    break;

  default:
    assert_not_reached(__FILE__, __LINE__);
    break;
  }
  return expr;
}


//////////////////////////////////////////////////////////////////////
// クラス EIBinaryOp
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] pt_obj パース木の定義要素
// @param[in] opr1 オペランド1
// @param[in] opr2 オペランド2
EiBinaryOp::EiBinaryOp(const PtBase* pt_obj,
		       ElbExpr* opr1,
		       ElbExpr* opr2) :
  EiOperation(pt_obj)
{
  mOpr[0] = opr1;
  mOpr[1] = opr2;
}

// @brief デストラクタ
EiBinaryOp::~EiBinaryOp()
{
}

// @brief 定数の時 true を返す．
// @note オペランドが定数ならこの式も定数となる．
bool
EiBinaryOp::is_const() const
{
  return operand1()->is_const() && operand2()->is_const();
}

// @brief オペランド数を返す．
ymuint32
EiBinaryOp::operand_num() const
{
  return 2;
}

// @brief オペランドを返す．
// @param[in] pos 位置番号
ElbExpr*
EiBinaryOp::_operand(ymuint32 pos) const
{
  return mOpr[pos];
}


//////////////////////////////////////////////////////////////////////
// クラス EiCompareOp
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] pt_obj パース木の定義要素
// @param[in] opr1 オペランド1
// @param[in] opr2 オペランド2
EiCompareOp::EiCompareOp(const PtBase* pt_obj,
			 ElbExpr* opr1,
			 ElbExpr* opr2) :
  EiBinaryOp(pt_obj, opr1, opr2)
{
  // 比較演算は大きい方の型を用いる．
  tVpiValueType type1 = operand1()->value_type();
  tVpiValueType type2 = operand2()->value_type();

  mOprType = calc_type(type1, type2);

  operand1()->set_reqsize(mOprType);
  operand2()->set_reqsize(mOprType);
}

// @brief デストラクタ
EiCompareOp::~EiCompareOp()
{
}

// @brief 式のタイプを返す．
tVpiValueType
EiCompareOp::value_type() const
{
  // 常に1ビット符号なし
  return pack(kVpiValueUS, 1);
}

// @brief スカラー値を返す．
tVpiScalarVal
EiCompareOp::eval_scalar() const
{
  return eval_logic();
}

// @brief real 型の値を返す．
double
EiCompareOp::eval_real() const
{
  return conv_to_real(eval_logic());
}

// @brief bitvector 型の値を返す．
void
EiCompareOp::eval_bitvector(BitVector& bitvector,
			    tVpiValueType req_type) const
{
  bitvector = eval_logic();
  bitvector.coerce(req_type);
}

// @brief 要求される式の型を計算してセットする．
// @param[in] type 要求される式の型
// @note 必要であればオペランドに対して再帰的に処理を行なう．
void
EiCompareOp::set_reqsize(tVpiValueType type)
{
  // なにもしない．
}


//////////////////////////////////////////////////////////////////////
// クラス EiEqOp
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] pt_obj パース木の定義要素
// @param[in] opr1 オペランド1
// @param[in] opr2 オペランド2
EiEqOp::EiEqOp(const PtBase* pt_obj,
	       ElbExpr* opr1,
	       ElbExpr* opr2) :
  EiCompareOp(pt_obj, opr1, opr2)
{
}

// @brief デストラクタ
EiEqOp::~EiEqOp()
{
}

// @brief 論理値を返す．
tVpiScalarVal
EiEqOp::eval_logic() const
{
  if ( opr_type() == kVpiValueReal ) {
    if ( operand1()->eval_real() == operand2()->eval_real() ) {
      return kVpiScalar1;
    }
    else {
      return kVpiScalar0;
    }
  }
  else {
    BitVector bv0;
    BitVector bv1;
    operand1()->eval_bitvector(bv0, opr_type());
    operand2()->eval_bitvector(bv1, opr_type());
    return eq(bv0, bv1);
  }
}

// @brief 演算子のタイプを返す．
tVpiOpType
EiEqOp::op_type() const
{
  return kVpiEqOp;
}


//////////////////////////////////////////////////////////////////////
// クラス EiNeqOp
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] pt_obj パース木の定義要素
// @param[in] opr1 オペランド1
// @param[in] opr2 オペランド2
EiNeqOp::EiNeqOp(const PtBase* pt_obj,
		 ElbExpr* opr1,
		 ElbExpr* opr2) :
  EiCompareOp(pt_obj, opr1, opr2)
{
}

// @brief デストラクタ
EiNeqOp::~EiNeqOp()
{
}

// @brief 論理値を返す．
tVpiScalarVal
EiNeqOp::eval_logic() const
{
  if ( opr_type() == kVpiValueReal ) {
    if ( operand1()->eval_real() != operand2()->eval_real() ) {
      return kVpiScalar1;
    }
    else {
      return kVpiScalar0;
    }
  }
  else {
    BitVector bv0;
    BitVector bv1;
    operand1()->eval_bitvector(bv0, opr_type());
    operand2()->eval_bitvector(bv1, opr_type());
    return ne(bv0, bv1);
  }
}

// @brief 演算子のタイプを返す．
tVpiOpType
EiNeqOp::op_type() const
{
  return kVpiNeqOp;
}


//////////////////////////////////////////////////////////////////////
// クラス EiCaseEqOp
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] pt_obj パース木の定義要素
// @param[in] opr1 オペランド1
// @param[in] opr2 オペランド2
EiCaseEqOp::EiCaseEqOp(const PtBase* pt_obj,
		       ElbExpr* opr1,
		       ElbExpr* opr2) :
  EiCompareOp(pt_obj, opr1, opr2)
{
}

// @brief デストラクタ
EiCaseEqOp::~EiCaseEqOp()
{
}

// @brief 論理値を返す．
tVpiScalarVal
EiCaseEqOp::eval_logic() const
{
  if ( opr_type() == kVpiValueReal ) {
    if ( operand1()->eval_real() == operand2()->eval_real() ) {
      return kVpiScalar1;
    }
    else {
      return kVpiScalar0;
    }
  }
  else {
    BitVector bv0;
    BitVector bv1;
    operand1()->eval_bitvector(bv0, opr_type());
    operand2()->eval_bitvector(bv1, opr_type());
    if ( eq_with_xz(bv0, bv1) ) {
      return kVpiScalar1;
    }
    else {
      return kVpiScalar0;
    }
  }
}

// @brief 演算子のタイプを返す．
tVpiOpType
EiCaseEqOp::op_type() const
{
  return kVpiCaseEqOp;
}


//////////////////////////////////////////////////////////////////////
// クラス EiCaseNeqOp
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] pt_obj パース木の定義要素
// @param[in] opr1 オペランド1
// @param[in] opr2 オペランド2
EiCaseNeqOp::EiCaseNeqOp(const PtBase* pt_obj,
			 ElbExpr* opr1,
			 ElbExpr* opr2) :
  EiCompareOp(pt_obj, opr1, opr2)
{
}

// @brief デストラクタ
EiCaseNeqOp::~EiCaseNeqOp()
{
}

// @brief 論理値を返す．
tVpiScalarVal
EiCaseNeqOp::eval_logic() const
{
  if ( opr_type() == kVpiValueReal ) {
    if ( operand1()->eval_real() != operand2()->eval_real() ) {
      return kVpiScalar1;
    }
    else {
      return kVpiScalar0;
    }
  }
  else {
    BitVector bv0;
    BitVector bv1;
    operand1()->eval_bitvector(bv0, opr_type());
    operand2()->eval_bitvector(bv1, opr_type());
    if ( eq_with_xz(bv0, bv1) ) {
      return kVpiScalar0;
    }
    else {
      return kVpiScalar1;
    }
  }
}

// @brief 演算子のタイプを返す．
tVpiOpType
EiCaseNeqOp::op_type() const
{
  return kVpiCaseNeqOp;
}


//////////////////////////////////////////////////////////////////////
// クラス EiGtOp
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] pt_obj パース木の定義要素
// @param[in] opr1 オペランド1
// @param[in] opr2 オペランド2
EiGtOp::EiGtOp(const PtBase* pt_obj,
	       ElbExpr* opr1,
	       ElbExpr* opr2) :
  EiCompareOp(pt_obj, opr1, opr2)
{
}

// @brief デストラクタ
EiGtOp::~EiGtOp()
{
}

// @brief 論理値を返す．
tVpiScalarVal
EiGtOp::eval_logic() const
{
  if ( opr_type() == kVpiValueReal ) {
    if ( operand1()->eval_real() > operand2()->eval_real() ) {
      return kVpiScalar1;
    }
    else {
      return kVpiScalar0;
    }
  }
  else {
    BitVector bv0;
    BitVector bv1;
    operand1()->eval_bitvector(bv0, opr_type());
    operand2()->eval_bitvector(bv1, opr_type());
    return gt(bv0, bv1);
  }
}

// @brief 演算子のタイプを返す．
tVpiOpType
EiGtOp::op_type() const
{
  return kVpiGtOp;
}


//////////////////////////////////////////////////////////////////////
// クラス EiGeOp
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] pt_obj パース木の定義要素
// @param[in] opr1 オペランド1
// @param[in] opr2 オペランド2
EiGeOp::EiGeOp(const PtBase* pt_obj,
	       ElbExpr* opr1,
	       ElbExpr* opr2) :
  EiCompareOp(pt_obj, opr1, opr2)
{
}

// @brief デストラクタ
EiGeOp::~EiGeOp()
{
}

// @brief 論理値を返す．
tVpiScalarVal
EiGeOp::eval_logic() const
{
  if ( opr_type() == kVpiValueReal ) {
    if ( operand1()->eval_real() >= operand2()->eval_real() ) {
      return kVpiScalar1;
    }
    else {
      return kVpiScalar0;
    }
  }
  else {
    BitVector bv0;
    BitVector bv1;
    operand1()->eval_bitvector(bv0, opr_type());
    operand2()->eval_bitvector(bv1, opr_type());
    return ge(bv0, bv1);
  }
}

// @brief 演算子のタイプを返す．
tVpiOpType
EiGeOp::op_type() const
{
  return kVpiGeOp;
}


//////////////////////////////////////////////////////////////////////
// クラス EiLtOp
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] pt_obj パース木の定義要素
// @param[in] opr1 オペランド1
// @param[in] opr2 オペランド2
EiLtOp::EiLtOp(const PtBase* pt_obj,
	       ElbExpr* opr1,
	       ElbExpr* opr2) :
  EiCompareOp(pt_obj, opr1, opr2)
{
}

// @brief デストラクタ
EiLtOp::~EiLtOp()
{
}

// @brief 論理値を返す．
tVpiScalarVal
EiLtOp::eval_logic() const
{
  if ( opr_type() == kVpiValueReal ) {
    if ( operand1()->eval_real() < operand2()->eval_real() ) {
      return kVpiScalar1;
    }
    else {
      return kVpiScalar0;
    }
  }
  else {
    BitVector bv0;
    BitVector bv1;
    operand1()->eval_bitvector(bv0, opr_type());
    operand2()->eval_bitvector(bv1, opr_type());
    return lt(bv0, bv1);
  }
}

// @brief 演算子のタイプを返す．
tVpiOpType
EiLtOp::op_type() const
{
  return kVpiLtOp;
}


//////////////////////////////////////////////////////////////////////
// クラス EiLeOp
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] pt_obj パース木の定義要素
// @param[in] opr1 オペランド1
// @param[in] opr2 オペランド2
EiLeOp::EiLeOp(const PtBase* pt_obj,
	       ElbExpr* opr1,
	       ElbExpr* opr2) :
  EiCompareOp(pt_obj, opr1, opr2)
{
}

// @brief デストラクタ
EiLeOp::~EiLeOp()
{
}

// @brief 論理値を返す．
tVpiScalarVal
EiLeOp::eval_logic() const
{
  if ( opr_type() == kVpiValueReal ) {
    if ( operand1()->eval_real() <= operand2()->eval_real() ) {
      return kVpiScalar1;
    }
    else {
      return kVpiScalar0;
    }
  }
  else {
    BitVector bv0;
    BitVector bv1;
    operand1()->eval_bitvector(bv0, opr_type());
    operand2()->eval_bitvector(bv1, opr_type());
    return le(bv0, bv1);
  }
}

// @brief 演算子のタイプを返す．
tVpiOpType
EiLeOp::op_type() const
{
  return kVpiLeOp;
}


//////////////////////////////////////////////////////////////////////
// クラス EiBinaryLogOp
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] pt_obj パース木の定義要素
// @param[in] opr1 オペランド1
// @param[in] opr2 オペランド2
EiBinaryLogOp::EiBinaryLogOp(const PtBase* pt_obj,
			     ElbExpr* opr1,
			     ElbExpr* opr2) :
  EiBinaryOp(pt_obj, opr1, opr2)
{
  // 論理演算の場合はオペランドも1ビットスカラーのはずだが
  // 仕様書には max(L(i), L(j)) なんて書いてある．
  // これは == などと一緒に表記しているからだと思う．
  // 実際には self determined で計算しておいて
  // 評価時に true/false/X に分類する．
  opr1->set_selfsize();
  opr2->set_selfsize();
}

// @brief デストラクタ
EiBinaryLogOp::~EiBinaryLogOp()
{
}

// @brief 式のタイプを返す．
tVpiValueType
EiBinaryLogOp::value_type() const
{
  // 常に1ビット符号なし
  return pack(kVpiValueUS, 1);
}

// @brief スカラー値を返す．
tVpiScalarVal
EiBinaryLogOp::eval_scalar() const
{
  return eval_logic();
}

// @brief real 型の値を返す．
double
EiBinaryLogOp::eval_real() const
{
  return conv_to_real(eval_logic());
}

// @brief bitvector 型の値を返す．
void
EiBinaryLogOp::eval_bitvector(BitVector& bitvector,
			    tVpiValueType req_type) const
{
  bitvector = eval_logic();
  bitvector.coerce(req_type);
}

// @brief 要求される式の型を計算してセットする．
// @param[in] type 要求される式の型
// @note 必要であればオペランドに対して再帰的に処理を行なう．
void
EiBinaryLogOp::set_reqsize(tVpiValueType type)
{
  // なにもしない．
}


//////////////////////////////////////////////////////////////////////
// クラス EiLogAndOp
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] pt_obj パース木の定義要素
// @param[in] opr1 オペランド1
// @param[in] opr2 オペランド2
EiLogAndOp::EiLogAndOp(const PtBase* pt_obj,
		       ElbExpr* opr1,
		       ElbExpr* opr2) :
  EiBinaryLogOp(pt_obj, opr1, opr2)
{
}

// @brief デストラクタ
EiLogAndOp::~EiLogAndOp()
{
}

// @brief 論理値を返す．
tVpiScalarVal
EiLogAndOp::eval_logic() const
{
  tVpiScalarVal v1 = operand1()->eval_logic();
  tVpiScalarVal v2 = operand2()->eval_logic();
  return v1 && v2;
}

// @brief 演算子のタイプを返す．
tVpiOpType
EiLogAndOp::op_type() const
{
  return kVpiLogAndOp;
}


//////////////////////////////////////////////////////////////////////
// クラス EiLogOrOp
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] pt_obj パース木の定義要素
// @param[in] opr1 オペランド1
// @param[in] opr2 オペランド2
EiLogOrOp::EiLogOrOp(const PtBase* pt_obj,
		     ElbExpr* opr1,
		     ElbExpr* opr2) :
  EiBinaryLogOp(pt_obj, opr1, opr2)
{
}

// @brief デストラクタ
EiLogOrOp::~EiLogOrOp()
{
}

// @brief 論理値を返す．
tVpiScalarVal
EiLogOrOp::eval_logic() const
{
  tVpiScalarVal v1 = operand1()->eval_logic();
  tVpiScalarVal v2 = operand2()->eval_logic();
  return v1 || v2;
}

// @brief 演算子のタイプを返す．
tVpiOpType
EiLogOrOp::op_type() const
{
  return kVpiLogOrOp;
}


//////////////////////////////////////////////////////////////////////
// クラス EiBinaryBitOp
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] pt_obj パース木の定義要素
// @param[in] opr1 オペランド1
// @param[in] opr2 オペランド2
EiBinaryBitOp::EiBinaryBitOp(const PtBase* pt_obj,
			     ElbExpr* opr1,
			     ElbExpr* opr2) :
  EiBinaryOp(pt_obj, opr1, opr2)
{
  // オペランドのサイズの大きい方で決まる．
  tVpiValueType type1 = operand1()->value_type();
  tVpiValueType type2 = operand2()->value_type();

  mType = calc_type(type1, type2);

  assert_cond(mType != kVpiValueReal, __FILE__, __LINE__);
}

// @brief デストラクタ
EiBinaryBitOp::~EiBinaryBitOp()
{
}

// @brief 式のタイプを返す．
tVpiValueType
EiBinaryBitOp::value_type() const
{
  return mType;
}

// @brief スカラー値を返す．
tVpiScalarVal
EiBinaryBitOp::eval_scalar() const
{
  BitVector bv;
  eval_bitvector(bv, value_type());
  return bv.to_scalar();
}

// @brief 論理値を返す．
tVpiScalarVal
EiBinaryBitOp::eval_logic() const
{
  BitVector bv;
  eval_bitvector(bv, value_type());
  return bv.to_logic();
}

// @brief real 型の値を返す．
double
EiBinaryBitOp::eval_real() const
{
  BitVector bv;
  eval_bitvector(bv, value_type());
  return bv.to_real();
}

// @brief 要求される式の型を計算してセットする．
// @param[in] type 要求される式の型
// @note 必要であればオペランドに対して再帰的に処理を行なう．
void
EiBinaryBitOp::set_reqsize(tVpiValueType type)
{
  mType = update_size(mType, type);
  operand1()->set_reqsize(mType);
  operand2()->set_reqsize(mType);
}


//////////////////////////////////////////////////////////////////////
// クラス EiBitAndOp
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] pt_obj パース木の定義要素
// @param[in] opr1 オペランド1
// @param[in] opr2 オペランド2
EiBitAndOp::EiBitAndOp(const PtBase* pt_obj,
		       ElbExpr* opr1,
		       ElbExpr* opr2) :
  EiBinaryBitOp(pt_obj, opr1, opr2)
{
}

// @brief デストラクタ
EiBitAndOp::~EiBitAndOp()
{
}

// @brief bitvector 型の値を返す．
void
EiBitAndOp::eval_bitvector(BitVector& bitvector,
			   tVpiValueType req_type) const
{
  BitVector bv1;
  operand1()->eval_bitvector(bitvector, value_type());
  operand2()->eval_bitvector(bv1, value_type());
  bitvector &= bv1;
  bitvector.coerce(req_type);
}

// @brief 演算子のタイプを返す．
tVpiOpType
EiBitAndOp::op_type() const
{
  return kVpiBitAndOp;
}


//////////////////////////////////////////////////////////////////////
// クラス EiBitOrOp
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] pt_obj パース木の定義要素
// @param[in] opr1 オペランド1
// @param[in] opr2 オペランド2
EiBitOrOp::EiBitOrOp(const PtBase* pt_obj,
		     ElbExpr* opr1,
		     ElbExpr* opr2) :
  EiBinaryBitOp(pt_obj, opr1, opr2)
{
}

// @brief デストラクタ
EiBitOrOp::~EiBitOrOp()
{
}

// @brief bitvector 型の値を返す．
void
EiBitOrOp::eval_bitvector(BitVector& bitvector,
			   tVpiValueType req_type) const
{
  BitVector bv1;
  operand1()->eval_bitvector(bitvector, value_type());
  operand2()->eval_bitvector(bv1, value_type());
  bitvector |= bv1;
  bitvector.coerce(req_type);
}

// @brief 演算子のタイプを返す．
tVpiOpType
EiBitOrOp::op_type() const
{
  return kVpiBitOrOp;
}


//////////////////////////////////////////////////////////////////////
// クラス EiBitXorOp
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] pt_obj パース木の定義要素
// @param[in] opr1 オペランド1
// @param[in] opr2 オペランド2
EiBitXorOp::EiBitXorOp(const PtBase* pt_obj,
		       ElbExpr* opr1,
		       ElbExpr* opr2) :
  EiBinaryBitOp(pt_obj, opr1, opr2)
{
}

// @brief デストラクタ
EiBitXorOp::~EiBitXorOp()
{
}

// @brief bitvector 型の値を返す．
void
EiBitXorOp::eval_bitvector(BitVector& bitvector,
			   tVpiValueType req_type) const
{
  BitVector bv1;
  operand1()->eval_bitvector(bitvector, value_type());
  operand2()->eval_bitvector(bv1, value_type());
  bitvector ^= bv1;
  bitvector.coerce(req_type);
}

// @brief 演算子のタイプを返す．
tVpiOpType
EiBitXorOp::op_type() const
{
  return kVpiBitXorOp;
}


//////////////////////////////////////////////////////////////////////
// クラス EiBitXnorOp
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] pt_obj パース木の定義要素
// @param[in] opr1 オペランド1
// @param[in] opr2 オペランド2
EiBitXnorOp::EiBitXnorOp(const PtBase* pt_obj,
		       ElbExpr* opr1,
		       ElbExpr* opr2) :
  EiBinaryBitOp(pt_obj, opr1, opr2)
{
}

// @brief デストラクタ
EiBitXnorOp::~EiBitXnorOp()
{
}

// @brief bitvector 型の値を返す．
void
EiBitXnorOp::eval_bitvector(BitVector& bitvector,
			    tVpiValueType req_type) const
{
  BitVector bv1;
  operand1()->eval_bitvector(bitvector, value_type());
  operand2()->eval_bitvector(bv1, value_type());
  bitvector ^= bv1;
  bitvector.negate();
  bitvector.coerce(req_type);
}

// @brief 演算子のタイプを返す．
tVpiOpType
EiBitXnorOp::op_type() const
{
  return kVpiBitXNorOp;
}


//////////////////////////////////////////////////////////////////////
// クラス EiBinaryArithOp
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] pt_obj パース木の定義要素
// @param[in] opr1 オペランド1
// @param[in] opr2 オペランド2
EiBinaryArithOp::EiBinaryArithOp(const PtBase* pt_obj,
				 ElbExpr* opr1,
				 ElbExpr* opr2) :
  EiBinaryOp(pt_obj, opr1, opr2)
{
  // オペランドのサイズの大きい方で決まる．
  tVpiValueType type1 = operand1()->value_type();
  tVpiValueType type2 = operand2()->value_type();

  mType = calc_type(type1, type2);
}

// @brief デストラクタ
EiBinaryArithOp::~EiBinaryArithOp()
{
}

// @brief 式のタイプを返す．
tVpiValueType
EiBinaryArithOp::value_type() const
{
  return mType;
}

// @brief スカラー値を返す．
tVpiScalarVal
EiBinaryArithOp::eval_scalar() const
{
  if ( value_type() == kVpiValueReal ) {
    return conv_to_scalar(eval_real());
  }
  else {
    BitVector bv;
    eval_bitvector(bv, value_type());
    return bv.to_scalar();
  }
}

// @brief 論理値を返す．
tVpiScalarVal
EiBinaryArithOp::eval_logic() const
{
  if ( value_type() == kVpiValueReal ) {
    return conv_to_scalar(eval_real());
  }
  else {
    BitVector bv;
    eval_bitvector(bv, value_type());
    return bv.to_logic();
  }
}

// @brief 要求される式の型を計算してセットする．
// @param[in] type 要求される式の型
// @note 必要であればオペランドに対して再帰的に処理を行なう．
void
EiBinaryArithOp::set_reqsize(tVpiValueType type)
{
  mType = update_size(mType, type);
  operand1()->set_reqsize(mType);
  operand2()->set_reqsize(mType);
}


//////////////////////////////////////////////////////////////////////
// クラス EiAddOp
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] pt_obj パース木の定義要素
// @param[in] opr1 オペランド1
// @param[in] opr2 オペランド2
EiAddOp::EiAddOp(const PtBase* pt_obj,
		 ElbExpr* opr1,
		 ElbExpr* opr2) :
  EiBinaryArithOp(pt_obj, opr1, opr2)
{
}

// @brief デストラクタ
EiAddOp::~EiAddOp()
{
}

// @brief int 型の値を返す．
// @param[out] val 結果を格納する変数
// @return 整数値に変換できたら true を返す．
bool
EiAddOp::eval_int(int& val) const
{
  int v0;
  bool stat1 = operand1()->eval_int(v0);
  int v1;
  bool stat2 = operand2()->eval_int(v1);
  if ( stat1 && stat2 ) {
    val = v0 + v1;
    return true;
  }
  return false;
}

// @brief real 型の値を返す．
double
EiAddOp::eval_real() const
{
  double v0 = operand1()->eval_real();
  double v1 = operand2()->eval_real();
  return v0 + v1;
}

// @brief bitvector 型の値を返す．
void
EiAddOp::eval_bitvector(BitVector& bitvector,
			tVpiValueType req_type) const
{
  BitVector bv1;
  operand1()->eval_bitvector(bitvector, value_type());
  operand2()->eval_bitvector(bv1, value_type());
  bitvector += bv1;
  bitvector.coerce(req_type);
}

// @brief 演算子のタイプを返す．
tVpiOpType
EiAddOp::op_type() const
{
  return kVpiAddOp;
}


//////////////////////////////////////////////////////////////////////
// クラス EiSubOp
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] pt_obj パース木の定義要素
// @param[in] opr1 オペランド1
// @param[in] opr2 オペランド2
EiSubOp::EiSubOp(const PtBase* pt_obj,
		 ElbExpr* opr1,
		 ElbExpr* opr2) :
  EiBinaryArithOp(pt_obj, opr1, opr2)
{
}

// @brief デストラクタ
EiSubOp::~EiSubOp()
{
}

// @brief int 型の値を返す．
// @param[out] val 結果を格納する変数
// @return 整数値に変換できたら true を返す．
bool
EiSubOp::eval_int(int& val) const
{
  int v0;
  bool stat1 = operand1()->eval_int(v0);
  int v1;
  bool stat2 = operand2()->eval_int(v1);
  if ( stat1 && stat2 ) {
    val = v0 - v1;
    return true;
  }
  return false;
}

// @brief real 型の値を返す．
double
EiSubOp::eval_real() const
{
  double v0 = operand1()->eval_real();
  double v1 = operand2()->eval_real();
  return v0 - v1;
}

// @brief bitvector 型の値を返す．
void
EiSubOp::eval_bitvector(BitVector& bitvector,
			tVpiValueType req_type) const
{
  BitVector bv1;
  operand1()->eval_bitvector(bitvector, value_type());
  operand2()->eval_bitvector(bv1, value_type());
  bitvector -= bv1;
  bitvector.coerce(req_type);
}

// @brief 演算子のタイプを返す．
tVpiOpType
EiSubOp::op_type() const
{
  return kVpiSubOp;
}


//////////////////////////////////////////////////////////////////////
// クラス EiMultOp
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] pt_obj パース木の定義要素
// @param[in] opr1 オペランド1
// @param[in] opr2 オペランド2
EiMultOp::EiMultOp(const PtBase* pt_obj,
		   ElbExpr* opr1,
		   ElbExpr* opr2) :
  EiBinaryArithOp(pt_obj, opr1, opr2)
{
}

// @brief デストラクタ
EiMultOp::~EiMultOp()
{
}

// @brief int 型の値を返す．
// @param[out] val 結果を格納する変数
// @return 整数値に変換できたら true を返す．
bool
EiMultOp::eval_int(int& val) const
{
  int v0;
  bool stat1 = operand1()->eval_int(v0);
  int v1;
  bool stat2 = operand2()->eval_int(v1);
  if ( stat1 && stat2 ) {
    val = v0 * v1;
    return true;
  }
  return false;
}

// @brief real 型の値を返す．
double
EiMultOp::eval_real() const
{
  double v0 = operand1()->eval_real();
  double v1 = operand2()->eval_real();
  return v0 * v1;
}

// @brief bitvector 型の値を返す．
void
EiMultOp::eval_bitvector(BitVector& bitvector,
			 tVpiValueType req_type) const
{
  BitVector bv1;
  operand1()->eval_bitvector(bitvector, value_type());
  operand2()->eval_bitvector(bv1, value_type());
  bitvector *= bv1;
  bitvector.coerce(req_type);
}

// @brief 演算子のタイプを返す．
tVpiOpType
EiMultOp::op_type() const
{
  return kVpiMultOp;
}


//////////////////////////////////////////////////////////////////////
// クラス EiDivOp
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] pt_obj パース木の定義要素
// @param[in] opr1 オペランド1
// @param[in] opr2 オペランド2
EiDivOp::EiDivOp(const PtBase* pt_obj,
		 ElbExpr* opr1,
		 ElbExpr* opr2) :
  EiBinaryArithOp(pt_obj, opr1, opr2)
{
}

// @brief デストラクタ
EiDivOp::~EiDivOp()
{
}

// @brief int 型の値を返す．
// @param[out] val 結果を格納する変数
// @return 整数値に変換できたら true を返す．
bool
EiDivOp::eval_int(int& val) const
{
  int v0;
  bool stat1 = operand1()->eval_int(v0);
  int v1;
  bool stat2 = operand2()->eval_int(v1);
  if ( stat1 && stat2 ) {
    val = v0 / v1;
    return true;
  }
  return false;
}

// @brief real 型の値を返す．
double
EiDivOp::eval_real() const
{
  double v0 = operand1()->eval_real();
  double v1 = operand2()->eval_real();
  return v0 / v1;
}

// @brief bitvector 型の値を返す．
void
EiDivOp::eval_bitvector(BitVector& bitvector,
			tVpiValueType req_type) const
{
  BitVector bv1;
  operand1()->eval_bitvector(bitvector, value_type());
  operand2()->eval_bitvector(bv1, value_type());
  bitvector /= bv1;
  bitvector.coerce(req_type);
}

// @brief 演算子のタイプを返す．
tVpiOpType
EiDivOp::op_type() const
{
  return kVpiDivOp;
}


//////////////////////////////////////////////////////////////////////
// クラス EiModOp
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] pt_obj パース木の定義要素
// @param[in] opr1 オペランド1
// @param[in] opr2 オペランド2
EiModOp::EiModOp(const PtBase* pt_obj,
		 ElbExpr* opr1,
		 ElbExpr* opr2) :
  EiBinaryArithOp(pt_obj, opr1, opr2)
{
}

// @brief デストラクタ
EiModOp::~EiModOp()
{
}

// @brief int 型の値を返す．
// @param[out] val 結果を格納する変数
// @return 整数値に変換できたら true を返す．
bool
EiModOp::eval_int(int& val) const
{
  int v0;
  bool stat1 = operand1()->eval_int(v0);
  int v1;
  bool stat2 = operand2()->eval_int(v1);
  if ( stat1 && stat2 ) {
    val = v0 % v1;
    return true;
  }
  return false;
}

// @brief real 型の値を返す．
double
EiModOp::eval_real() const
{
  // ビットベクタで計算してからあとで double に直すだけ
  BitVector bv;
  eval_bitvector(bv, value_type());
  return bv.to_real();
}

// @brief bitvector 型の値を返す．
void
EiModOp::eval_bitvector(BitVector& bitvector,
			tVpiValueType req_type) const
{
  BitVector bv1;
  operand1()->eval_bitvector(bitvector, value_type());
  operand2()->eval_bitvector(bv1, value_type());
  bitvector %= bv1;
  bitvector.coerce(req_type);
}

// @brief 演算子のタイプを返す．
tVpiOpType
EiModOp::op_type() const
{
  return kVpiModOp;
}


//////////////////////////////////////////////////////////////////////
// クラス EiPowerOp
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] pt_obj パース木の定義要素
// @param[in] opr1 オペランド1
// @param[in] opr2 オペランド2
EiPowerOp::EiPowerOp(const PtBase* pt_obj,
		     ElbExpr* opr1,
		     ElbExpr* opr2) :
  EiBinaryOp(pt_obj, opr1, opr2)
{
  // 巾乗演算の場合, どちらかのオペランドが real, integer, signed
  // なら結果は real, どちらも unsigned の時のみ unsigned となる．
  tVpiValueType type1 = operand1()->value_type();
  tVpiValueType type2 = operand2()->value_type();
  mType = calc_type2(type1, type2);

  // ただし opr2 は self-determined
  operand2()->set_reqsize(type2);
}

// @brief デストラクタ
EiPowerOp::~EiPowerOp()
{
}

// @brief 式のタイプを返す．
tVpiValueType
EiPowerOp::value_type() const
{
  return mType;
}

// @brief スカラー値を返す．
tVpiScalarVal
EiPowerOp::eval_scalar() const
{
  BitVector bv;
  eval_bitvector(bv, value_type());
  return bv.to_scalar();
}

// @brief 論理値を返す．
tVpiScalarVal
EiPowerOp::eval_logic() const
{
  if ( value_type() == kVpiValueReal ) {
    return conv_to_scalar(eval_real());
  }
  else {
    BitVector bv;
    eval_bitvector(bv, value_type());
    return bv.to_logic();
  }
}

// @brief real 型の値を返す．
double
EiPowerOp::eval_real() const
{
  double v0 = operand1()->eval_real();
  double v1 = operand2()->eval_real();
  return pow(v0, v1);
}

// @brief bitvector 型の値を返す．
void
EiPowerOp::eval_bitvector(BitVector& bitvector,
			  tVpiValueType req_type) const
{
  BitVector bv1;
  operand1()->eval_bitvector(bitvector, value_type());
  operand2()->eval_bitvector(bv1, value_type());
  bitvector.power(bv1);
  bitvector.coerce(req_type);
}

// @brief 要求される式の型を計算してセットする．
// @param[in] type 要求される式の型
// @note 必要であればオペランドに対して再帰的に処理を行なう．
void
EiPowerOp::set_reqsize(tVpiValueType type)
{
  mType = update_size(mType, type);

  // 第1オペランドは type をそのまま伝える．
  operand1()->set_reqsize(mType);

  // 第2オペランドは不変
}

// @brief 演算子のタイプを返す．
tVpiOpType
EiPowerOp::op_type() const
{
  return kVpiPowerOp;
}


//////////////////////////////////////////////////////////////////////
// クラス EiShiftOp
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] pt_obj パース木の定義要素
// @param[in] opr1 オペランド1
// @param[in] opr2 オペランド2
EiShiftOp::EiShiftOp(const PtBase* pt_obj,
		     ElbExpr* opr1,
		     ElbExpr* opr2) :
  EiBinaryOp(pt_obj, opr1, opr2)
{
  // シフト演算子は第1オペランドの型とサイズをそのまま引き継ぐ
  mType = opr1->value_type();
  assert_cond(mType != kVpiValueReal, __FILE__, __LINE__);

  tVpiValueType type2 = opr2->value_type();
  assert_cond(type2 != kVpiValueReal, __FILE__, __LINE__);

  // 第2オペランドのサイズは self determined
  opr2->set_selfsize();
}

// @brief デストラクタ
EiShiftOp::~EiShiftOp()
{
}

// @brief 式のタイプを返す．
tVpiValueType
EiShiftOp::value_type() const
{
  return mType;
}

// @brief スカラー値を返す．
tVpiScalarVal
EiShiftOp::eval_scalar() const
{
  BitVector bv;
  eval_bitvector(bv, value_type());
  return bv.to_scalar();
}

// @brief 論理値を返す．
tVpiScalarVal
EiShiftOp::eval_logic() const
{
  BitVector bv;
  eval_bitvector(bv, value_type());
  return bv.to_logic();
}

// @brief real 型の値を返す．
double
EiShiftOp::eval_real() const
{
  BitVector bv;
  eval_bitvector(bv, value_type());
  return bv.to_real();
}

// @brief 要求される式の型を計算してセットする．
// @param[in] type 要求される式の型
// @note 必要であればオペランドに対して再帰的に処理を行なう．
void
EiShiftOp::set_reqsize(tVpiValueType type)
{
  mType = update_size(mType, type);

  // 第1オペランドは type をそのまま伝える．
  operand1()->set_reqsize(mType);

  // 第2オペランドは不変
}


//////////////////////////////////////////////////////////////////////
// クラス EiLShiftOp
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] pt_obj パース木の定義要素
// @param[in] opr1 オペランド1
// @param[in] opr2 オペランド2
EiLShiftOp::EiLShiftOp(const PtBase* pt_obj,
		       ElbExpr* opr1,
		       ElbExpr* opr2) :
  EiShiftOp(pt_obj, opr1, opr2)
{
}

// @brief デストラクタ
EiLShiftOp::~EiLShiftOp()
{
}

// @brief bitvector 型の値を返す．
void
EiLShiftOp::eval_bitvector(BitVector& bitvector,
			tVpiValueType req_type) const
{
  BitVector bv1;
  operand1()->eval_bitvector(bitvector, value_type());
  operand2()->eval_bitvector(bv1, value_type());
  bitvector <<= bv1;
  bitvector.coerce(req_type);
}

// @brief 演算子のタイプを返す．
tVpiOpType
EiLShiftOp::op_type() const
{
  return kVpiLShiftOp;
}


//////////////////////////////////////////////////////////////////////
// クラス EiRShiftOp
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] pt_obj パース木の定義要素
// @param[in] opr1 オペランド1
// @param[in] opr2 オペランド2
EiRShiftOp::EiRShiftOp(const PtBase* pt_obj,
		       ElbExpr* opr1,
		       ElbExpr* opr2) :
  EiShiftOp(pt_obj, opr1, opr2)
{
}

// @brief デストラクタ
EiRShiftOp::~EiRShiftOp()
{
}

// @brief bitvector 型の値を返す．
void
EiRShiftOp::eval_bitvector(BitVector& bitvector,
			tVpiValueType req_type) const
{
  BitVector bv1;
  operand1()->eval_bitvector(bitvector, value_type());
  operand2()->eval_bitvector(bv1, value_type());
  bitvector >>= bv1;
  bitvector.coerce(req_type);
}

// @brief 演算子のタイプを返す．
tVpiOpType
EiRShiftOp::op_type() const
{
  return kVpiRShiftOp;
}


//////////////////////////////////////////////////////////////////////
// クラス EiArithLShiftOp
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] pt_obj パース木の定義要素
// @param[in] opr1 オペランド1
// @param[in] opr2 オペランド2
EiArithLShiftOp::EiArithLShiftOp(const PtBase* pt_obj,
				 ElbExpr* opr1,
				 ElbExpr* opr2) :
  EiShiftOp(pt_obj, opr1, opr2)
{
}

// @brief デストラクタ
EiArithLShiftOp::~EiArithLShiftOp()
{
}

// @brief bitvector 型の値を返す．
void
EiArithLShiftOp::eval_bitvector(BitVector& bitvector,
				tVpiValueType req_type) const
{
  BitVector bv1;
  operand1()->eval_bitvector(bitvector, value_type());
  operand2()->eval_bitvector(bv1, value_type());
  bitvector.alshift(bv1);
  bitvector.coerce(req_type);
}

// @brief 演算子のタイプを返す．
tVpiOpType
EiArithLShiftOp::op_type() const
{
  return kVpiArithLShiftOp;
}


//////////////////////////////////////////////////////////////////////
// クラス EiArithRShiftOp
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] pt_obj パース木の定義要素
// @param[in] opr1 オペランド1
// @param[in] opr2 オペランド2
EiArithRShiftOp::EiArithRShiftOp(const PtBase* pt_obj,
				 ElbExpr* opr1,
				 ElbExpr* opr2) :
  EiShiftOp(pt_obj, opr1, opr2)
{
}

// @brief デストラクタ
EiArithRShiftOp::~EiArithRShiftOp()
{
}

// @brief bitvector 型の値を返す．
void
EiArithRShiftOp::eval_bitvector(BitVector& bitvector,
				tVpiValueType req_type) const
{
  BitVector bv1;
  operand1()->eval_bitvector(bitvector, value_type());
  operand2()->eval_bitvector(bv1, value_type());
  bitvector.arshift(bv1);
  bitvector.coerce(req_type);
}

// @brief 演算子のタイプを返す．
tVpiOpType
EiArithRShiftOp::op_type() const
{
  return kVpiArithRShiftOp;
}

END_NAMESPACE_YM_VERILOG
