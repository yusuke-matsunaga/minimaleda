
/// @file libym_verilog/elb_impl/EiTernaryOp.cc
/// @brief EiTernaryOp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: EiTernaryOp.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "EiFactory.h"
#include "EiTernaryOp.h"

#include "ym_verilog/BitVector.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// EiFactory の生成関数
//////////////////////////////////////////////////////////////////////

// @brief 3項演算子を生成する．
// @param[in] pt_expr パース木の定義要素
// @param[in] op_type 演算子のタイプ
// @param[in] opr0 オペランド
// @param[in] opr1 オペランド
// @param[in] opr2 オペランド
ElbExpr*
EiFactory::new_TernaryOp(const PtBase* pt_expr,
			 tVpiOpType op_type,
			 ElbExpr* opr0,
			 ElbExpr* opr1,
			 ElbExpr* opr2)
{
  ElbExpr* expr = NULL;
  void* p;
  switch ( op_type ) {
  case kVpiConditionOp:
    p = mAlloc.get_memory(sizeof(EiConditionOp));
    expr = new (p) EiConditionOp(pt_expr, opr0, opr1, opr2);
    break;

  case kVpiMinTypMaxOp:
    p = mAlloc.get_memory(sizeof(EiMinTypMaxOp));
    expr = new (p) EiMinTypMaxOp(pt_expr, opr0, opr1, opr2);
    break;

  default:
    assert_not_reached(__FILE__, __LINE__);
    break;
  }
  return expr;
}


//////////////////////////////////////////////////////////////////////
// クラス EiTernaryOp
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] pt_obj パース木の定義要素
// @param[in] opr1 オペランド1
// @param[in] opr2 オペランド2
// @param[in] opr3 オペランド3
EiTernaryOp::EiTernaryOp(const PtBase* pt_obj,
			 ElbExpr* opr1,
			 ElbExpr* opr2,
			 ElbExpr* opr3) :
  EiOperation(pt_obj)
{
  mOpr[0] = opr1;
  mOpr[1] = opr2;
  mOpr[2] = opr3;
}

// @brief デストラクタ
EiTernaryOp::~EiTernaryOp()
{
}

// @brief 定数の時 true を返す．
// @note オペランドが定数ならこの式も定数となる．
bool
EiTernaryOp::is_const() const
{
  return operand1()->is_const() &&
    operand2()->is_const() && operand3()->is_const();

}

// @brief オペランド数を返す．
ymuint32
EiTernaryOp::operand_num() const
{
  return 3;
}

// @brief オペランドを返す．
// @param[in] pos 位置番号
ElbExpr*
EiTernaryOp::_operand(ymuint32 pos) const
{
  return mOpr[pos];
}


//////////////////////////////////////////////////////////////////////
// クラス EiConditionOp
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] pt_obj パース木の定義要素
// @param[in] opr1 オペランド1
// @param[in] opr2 オペランド2
// @param[in] opr3 オペランド3
EiConditionOp::EiConditionOp(const PtBase* pt_obj,
			     ElbExpr* opr1,
			     ElbExpr* opr2,
			     ElbExpr* opr3) :
  EiTernaryOp(pt_obj, opr1, opr2, opr3)
{
  // 三項演算子の場合は第1オペランドが self determined で
  // 結果は第2オペランドと第3オペランドから決まる．

  operand1()->set_selfsize();

  tVpiValueType type2 = operand2()->value_type();
  tVpiValueType type3 = operand3()->value_type();
  mType = calc_type(type2, type3);
}

// @brief デストラクタ
EiConditionOp::~EiConditionOp()
{
}

// @brief 式のタイプを返す．
tVpiValueType
EiConditionOp::value_type() const
{
  return mType;
}

// @brief スカラー値を返す．
tVpiScalarVal
EiConditionOp::eval_scalar() const
{
  switch ( operand1()->eval_logic() ) {
  case kVpiScalar0:
    if ( mType == kVpiValueReal ) {
      return conv_to_scalar(operand3()->eval_real());
    }
    else {
      return operand3()->eval_scalar();
    }

  case kVpiScalar1:
    if ( mType == kVpiValueReal ) {
      return conv_to_scalar(operand2()->eval_real());
    }
    else {
      return operand2()->eval_scalar();
    }

  case kVpiScalarX:
    if ( mType == kVpiValueReal ) {
      return kVpiScalarX;
    }
    else {
      BitVector bitvector;
      BitVector tmp;
      operand2()->eval_bitvector(bitvector, mType);
      operand3()->eval_bitvector(tmp, mType);
      bitvector.merge(tmp);
      return bitvector.to_scalar();
    }

  default:
    assert_not_reached(__FILE__, __LINE__);
  }
  // ダミー
  return kVpiScalarX;
}

// @brief 論理値を返す．
tVpiScalarVal
EiConditionOp::eval_logic() const
{
  switch ( operand1()->eval_logic() ) {
  case kVpiScalar0:
    if ( mType == kVpiValueReal ) {
      return conv_to_scalar(operand3()->eval_real());
    }
    else {
      return operand3()->eval_logic();
    }

  case kVpiScalar1:
    if ( mType == kVpiValueReal ) {
      return conv_to_scalar(operand2()->eval_real());
    }
    else {
      return operand2()->eval_logic();
    }

  case kVpiScalarX:
    if ( mType == kVpiValueReal ) {
      return kVpiScalarX;
    }
    else {
      BitVector bitvector;
      BitVector tmp;
      operand2()->eval_bitvector(bitvector, mType);
      operand3()->eval_bitvector(tmp, mType);
      bitvector.merge(tmp);
      return bitvector.to_logic();
    }

  default:
    assert_not_reached(__FILE__, __LINE__);
  }
  // ダミー
  return kVpiScalarX;
}

// @brief real 型の値を返す．
double
EiConditionOp::eval_real() const
{
  switch ( operand1()->eval_logic() ) {
  case kVpiScalar0:
    if ( mType == kVpiValueReal ) {
      return operand3()->eval_real();
    }
    else {
      BitVector bitvector;
      operand3()->eval_bitvector(bitvector, mType);
      return bitvector.to_real();
    }

  case kVpiScalar1:
    if ( mType == kVpiValueReal ) {
      return operand2()->eval_real();
    }
    else {
      BitVector bitvector;
      operand2()->eval_bitvector(bitvector, mType);
      return bitvector.to_real();
    }

  case kVpiScalarX:
    if ( mType == kVpiValueReal ) {
      return 0.0;
    }
    else {
      BitVector bitvector;
      BitVector tmp;
      operand2()->eval_bitvector(bitvector, mType);
      operand3()->eval_bitvector(tmp, mType);
      bitvector.merge(tmp);
      return bitvector.to_real();
    }

  default:
    assert_not_reached(__FILE__, __LINE__);
  }
  // ダミー
  return 0.0;
}

// @brief bitvector 型の値を返す．
void
EiConditionOp::eval_bitvector(BitVector& bitvector,
			  tVpiValueType req_type) const
{
  switch ( operand1()->eval_logic() ) {
  case kVpiScalar0:
    if ( mType == kVpiValueReal ) {
      bitvector = operand3()->eval_real();
    }
    else {
      operand3()->eval_bitvector(bitvector, mType);
    }
    break;

  case kVpiScalar1:
    if ( mType == kVpiValueReal ) {
      bitvector = operand2()->eval_real();
    }
    else {
      operand2()->eval_bitvector(bitvector, mType);
    }
    break;

  case kVpiScalarX:
    if ( mType == kVpiValueReal ) {
      bitvector = BitVector(kVpiScalar0, unpack_size(mType));
    }
    else {
      BitVector tmp;
      operand2()->eval_bitvector(bitvector, mType);
      operand3()->eval_bitvector(tmp, mType);
      bitvector.merge(tmp);
    }
    break;

  default:
    assert_not_reached(__FILE__, __LINE__);
  }
  bitvector.coerce(req_type);
}

// @brief 要求される式の型を計算してセットする．
// @param[in] type 要求される式の型
// @note 必要であればオペランドに対して再帰的に処理を行なう．
void
EiConditionOp::set_reqsize(tVpiValueType type)
{
  mType = update_size(mType, type);

  // 第1オペランドは不変

  // 第2, 第3オペランドは type をそのまま伝える．
  operand2()->set_reqsize(mType);
  operand3()->set_reqsize(mType);
}

// @brief 演算子のタイプを返す．
tVpiOpType
EiConditionOp::op_type() const
{
  return kVpiConditionOp;
}


//////////////////////////////////////////////////////////////////////
// クラス EiMinTypMaxOp
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] pt_obj パース木の定義要素
// @param[in] opr1 オペランド1
// @param[in] opr2 オペランド2
// @param[in] opr3 オペランド3
EiMinTypMaxOp::EiMinTypMaxOp(const PtBase* pt_obj,
			     ElbExpr* opr1,
			     ElbExpr* opr2,
			     ElbExpr* opr3) :
  EiTernaryOp(pt_obj, opr1, opr2, opr3)
{
  // とりあえず真ん中の式を使う．
  mType = opr2->value_type();
}

// @brief デストラクタ
EiMinTypMaxOp::~EiMinTypMaxOp()
{
}

// @brief 式のタイプを返す．
tVpiValueType
EiMinTypMaxOp::value_type() const
{
  return mType;
}

// @brief スカラー値を返す．
tVpiScalarVal
EiMinTypMaxOp::eval_scalar() const
{
  return operand2()->eval_scalar();
}

// @brief 論理値を返す．
tVpiScalarVal
EiMinTypMaxOp::eval_logic() const
{
  return operand2()->eval_scalar();
}

// @brief real 型の値を返す．
double
EiMinTypMaxOp::eval_real() const
{
  return operand2()->eval_real();
}

// @brief bitvector 型の値を返す．
void
EiMinTypMaxOp::eval_bitvector(BitVector& bitvector,
			      tVpiValueType req_type) const
{
  operand2()->eval_bitvector(bitvector, req_type);
}

// @brief 要求される式の型を計算してセットする．
// @param[in] type 要求される式の型
// @note 必要であればオペランドに対して再帰的に処理を行なう．
void
EiMinTypMaxOp::set_reqsize(tVpiValueType type)
{
  mType = update_size(mType, type);

  // 3オペランドには type をそのまま伝える．
  operand1()->set_reqsize(mType);
  operand2()->set_reqsize(mType);
  operand3()->set_reqsize(mType);
}

// @brief 演算子のタイプを返す．
tVpiOpType
EiMinTypMaxOp::op_type() const
{
  return kVpiMinTypMaxOp;
}

END_NAMESPACE_YM_VERILOG
