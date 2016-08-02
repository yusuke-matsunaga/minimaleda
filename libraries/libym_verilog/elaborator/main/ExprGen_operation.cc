
/// @file libym_verilog/elaborator/ExprGen_opration.cc
/// @brief ElbMgr の実装ファイル(式の実体化)
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: ExprGen_operation.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ExprGen.h"
#include "ElbEnv.h"

#include "ym_verilog/pt/PtExpr.h"
#include "ym_verilog/pt/PtMisc.h"

#include "ElbExpr.h"


BEGIN_NAMESPACE_YM_VERILOG

// @brief PtOpr から ElbExpr を生成する．
// @param[in] parent 親のスコープ
// @param[in] pt_expr 式を表すパース木
// @param[in] env 生成時の環境
ElbExpr*
ExprGen::instantiate_opr(const VlNamedObj* parent,
			 const ElbEnv& env,
			 const PtExpr* pt_expr)
{
  tVpiOpType op_type = pt_expr->opr_type();
  ymuint opr_size = pt_expr->operand_num();

  ElbExpr* opr0 = NULL;
  ElbExpr* opr1 = NULL;
  ElbExpr* opr2 = NULL;
  ElbExpr* expr = NULL;
  bool real_check = false;
  switch ( op_type ) {
  case kVpiPosedgeOp:
  case kVpiNegedgeOp:
    assert_cond(opr_size == 1, __FILE__, __LINE__);
    error_illegal_edge_descriptor(pt_expr);
    return NULL;

  case kVpiBitNegOp:
  case kVpiUnaryAndOp:
  case kVpiUnaryNandOp:
  case kVpiUnaryOrOp:
  case kVpiUnaryNorOp:
  case kVpiUnaryXorOp:
  case kVpiUnaryXNorOp: real_check = true;
  case kVpiPlusOp:
  case kVpiMinusOp:
  case kVpiNotOp:
    assert_cond(opr_size == 1, __FILE__, __LINE__);
    opr0 = instantiate_expr(parent, env, pt_expr->operand(0));
    if ( !opr0 ) {
      return NULL;
    }
    if ( real_check && opr0->value_type() == kVpiValueReal ) {
      error_illegal_real_type(pt_expr->operand(0));
      return NULL;
    }
    return factory().new_UnaryOp(pt_expr, op_type, opr0);

  case kVpiBitAndOp:
  case kVpiBitOrOp:
  case kVpiBitXNorOp:
  case kVpiBitXorOp:
  case kVpiLShiftOp:
  case kVpiRShiftOp:
  case kVpiArithLShiftOp:
  case kVpiArithRShiftOp: real_check = true;
  case kVpiAddOp:
  case kVpiSubOp:
  case kVpiMultOp:
  case kVpiDivOp:
  case kVpiModOp:
  case kVpiPowerOp:
  case kVpiLogAndOp:
  case kVpiLogOrOp:
  case kVpiCaseEqOp:
  case kVpiCaseNeqOp:
  case kVpiEqOp:
  case kVpiNeqOp:
  case kVpiGeOp:
  case kVpiGtOp:
  case kVpiLeOp:
  case kVpiLtOp:
    assert_cond(opr_size == 2, __FILE__, __LINE__);
    opr0 = instantiate_expr(parent, env, pt_expr->operand(0));
    opr1 = instantiate_expr(parent, env, pt_expr->operand(1));
    if ( !opr0 || !opr1 ) {
      return NULL;
    }
    if ( real_check ) {
      if ( opr0->value_type() == kVpiValueReal ) {
	error_illegal_real_type(pt_expr->operand(0));
	return NULL;
      }
      if ( opr1->value_type() == kVpiValueReal ) {
	error_illegal_real_type(pt_expr->operand(1));
	return NULL;
      }
    }
    expr = factory().new_BinaryOp(pt_expr, op_type, opr0, opr1);
    break;

  case kVpiConditionOp:
  case kVpiMinTypMaxOp:
    assert_cond(opr_size == 3, __FILE__, __LINE__);
    opr0 = instantiate_expr(parent, env, pt_expr->operand(0));
    opr1 = instantiate_expr(parent, env, pt_expr->operand(1));
    opr2 = instantiate_expr(parent, env, pt_expr->operand(2));
    if ( !opr0 || !opr1 || !opr2 ) {
      return NULL;
    }
    expr = factory().new_TernaryOp(pt_expr, op_type, opr0, opr1, opr2);
    break;

  case kVpiConcatOp:
    {
      ElbExpr** opr_list = factory().new_ExprList(opr_size);
      for (ymuint i = 0; i < opr_size; ++ i) {
	const PtExpr* pt_expr1 = pt_expr->operand(i);
	ElbExpr* expr1 = instantiate_expr(parent, env, pt_expr1);
	if ( !expr1 ) {
	  return NULL;
	}
	if ( expr1->value_type() == kVpiValueReal ) {
	  error_illegal_real_type(pt_expr1);
	  return NULL;
	}
	opr_list[i] = expr1;
      }

      expr = factory().new_ConcatOp(pt_expr, opr_size, opr_list);
      expr->set_selfsize();
    }
    break;

  case kVpiMultiConcatOp:
    {
      const PtExpr* pt_expr0 = pt_expr->operand(0);
      ElbExpr* rep = instantiate_constant_expr(parent, pt_expr0);
      if ( rep == NULL ) {
	return NULL;
      }
      int rep_num;
      if ( !expr_to_int(rep, rep_num) ) {
	return NULL;
      }
      ElbExpr** opr_list = factory().new_ExprList(opr_size - 1);
      for (ymuint i = 1; i < opr_size; ++ i) {
	const PtExpr* pt_expr1 = pt_expr->operand(i);
	ElbExpr* expr1 = instantiate_expr(parent, env, pt_expr1);
	if ( !expr1 ) {
	  return NULL;
	}
	if ( expr1->value_type() == kVpiValueReal ) {
	  error_illegal_real_type(pt_expr1);
	  return NULL;
	}
	opr_list[i - 1] = expr1;
      }
      expr = factory().new_MultiConcatOp(pt_expr, rep, rep_num,
					 opr_size - 1, opr_list);
      expr->set_selfsize();
    }
    break;

  default:
    assert_not_reached(__FILE__, __LINE__);
    return NULL;
  }

  // attribute instance の生成
  //instantiate_attribute(pt_expr->attr_top(), false, expr);

  return expr;
}

BEGIN_NONAMESPACE

// val0 と val1 が === で等価とみなされる時 true を返す．
bool
caseeq(ElbValue& val0,
       ElbValue& val1)
{
  if ( val0.is_real() ) {
    val1.to_real();
    return ( val0.real_value() == val1.real_value() );
  }
  else if ( val1.is_real() ) {
    val0.to_real();
    return ( val0.real_value() == val1.real_value() );
  }
  else {
    val0.to_bitvector();
    val1.to_bitvector();
    return eq_with_x(val0.bitvector_value(), val1.bitvector_value());
  }
}

// val0 == val1 の比較結果を返す．
tVpiScalarVal
eq(ElbValue& val0,
   ElbValue& val1)
{
  if ( val0.is_real() ) {
    val1.to_real();
    if ( val0.real_value() == val1.real_value() ) {
      return kVpiScalar1;
    }
    else {
      return kVpiScalar0;
    }
  }
  else if ( val1.is_real() ) {
    val0.to_real();
    if ( val0.real_value() == val1.real_value() ) {
      return kVpiScalar1;
    }
    else {
      return kVpiScalar0;
    }
  }
  else {
    val0.to_bitvector();
    val1.to_bitvector();
    return eq(val0.bitvector_value(), val1.bitvector_value());
  }
}

// val0 >= val1 の比較を行う．
tVpiScalarVal
ge(ElbValue& val0,
   ElbValue& val1)
{
  if ( val0.is_real() ) {
    val1.to_real();
    if ( val0.real_value() >= val1.real_value() ) {
      return kVpiScalar1;
    }
    else {
      return kVpiScalar0;
    }
  }
  else if ( val1.is_real() ) {
    val0.to_real();
    if ( val0.real_value() >= val1.real_value() ) {
      return kVpiScalar1;
    }
    else {
      return kVpiScalar0;
    }
  }
  else {
    val0.to_bitvector();
    val1.to_bitvector();
    return ge(val0.bitvector_value(), val1.bitvector_value());
  }
}

END_NONAMESPACE


// @brief 演算子に対して int 型の値を評価する．
// @param[in] parent 親のスコープ
// @param[in] pt_expr 式を表すパース木
ElbValue
ExprGen::evaluate_opr(const VlNamedObj* parent,
		      const PtExpr* pt_expr)
{
  tVpiOpType op_type = pt_expr->opr_type();
  ymuint opr_size = pt_expr->operand_num();

  ElbValue val0;
  ElbValue val1;
  ElbValue val2;

  // 一回目はオペランドの評価のみを行う．
  switch ( op_type ) {
  case kVpiPosedgeOp:
  case kVpiNegedgeOp:
    // この演算は使えない．
    assert_cond(opr_size == 1, __FILE__, __LINE__);
    error_illegal_edge_descriptor(pt_expr);
    return ElbValue();

  case kVpiBitNegOp:
  case kVpiUnaryAndOp:
  case kVpiUnaryNandOp:
  case kVpiUnaryOrOp:
  case kVpiUnaryNorOp:
  case kVpiUnaryXorOp:
  case kVpiUnaryXNorOp:
    // この演算はビットベクタ型に変換できなければならない．
    assert_cond(opr_size == 1, __FILE__, __LINE__);
    val0 = evaluate_opr(parent, pt_expr->operand(0));
    if ( val0.is_error() ) {
      return ElbValue();
    }
    if ( val0.is_real() ) {
      error_illegal_real_type(pt_expr->operand(0));
      return ElbValue();
    }
    // ビットベクタ型に変換しておく
    val0.to_bitvector();
    break;

  case kVpiPlusOp:
  case kVpiMinusOp:
    // この演算はどの型でもOK
    assert_cond(opr_size == 1, __FILE__, __LINE__);
    val0 = evaluate_opr(parent, pt_expr->operand(0));
    if ( val0.is_error() ) {
      return ElbValue();
    }
    break;

  case kVpiNotOp:
    // この演算は論理値に変換しておく．
    val0.to_logic();
    break;

  case kVpiBitAndOp:
  case kVpiBitOrOp:
  case kVpiBitXNorOp:
  case kVpiBitXorOp:
  case kVpiLShiftOp:
  case kVpiRShiftOp:
  case kVpiArithLShiftOp:
  case kVpiArithRShiftOp:
    // この演算はビットベクタ型に変換できなければならない．
    assert_cond(opr_size == 2, __FILE__, __LINE__);
    val0 = evaluate_opr(parent, pt_expr->operand(0));
    val1 = evaluate_opr(parent, pt_expr->operand(1));
    if ( val0.is_error() || val1.is_error() ) {
      return ElbValue();
    }
    if ( val0.is_real() ) {
      error_illegal_real_type(pt_expr->operand(0));
      return ElbValue();
    }
    if ( val1.is_real() ) {
      error_illegal_real_type(pt_expr->operand(1));
      return ElbValue();
    }
    // ビットベクタ型に変換しておく．
    val0.to_bitvector();
    val1.to_bitvector();
    break;

  case kVpiAddOp:
  case kVpiSubOp:
  case kVpiMultOp:
  case kVpiDivOp:
  case kVpiModOp:
  case kVpiPowerOp:
  case kVpiCaseEqOp:
  case kVpiCaseNeqOp:
  case kVpiEqOp:
  case kVpiNeqOp:
  case kVpiGeOp:
  case kVpiGtOp:
  case kVpiLeOp:
  case kVpiLtOp:
    // この演算はどの型でもOK
    assert_cond(opr_size == 2, __FILE__, __LINE__);
    val0 = evaluate_opr(parent, pt_expr->operand(0));
    val1 = evaluate_opr(parent, pt_expr->operand(1));
    if ( val0.is_error() || val1.is_error() ) {
      return ElbValue();
    }
    break;

  case kVpiLogAndOp:
  case kVpiLogOrOp:
    // 論理値型に変換しておく．
    val0.to_logic();
    val1.to_logic();
    break;

  case kVpiConditionOp:
  case kVpiMinTypMaxOp:
    assert_cond(opr_size == 3, __FILE__, __LINE__);
    val0 = evaluate_opr(parent, pt_expr->operand(0));
    val1 = evaluate_opr(parent, pt_expr->operand(1));
    val2 = evaluate_opr(parent, pt_expr->operand(2));
    if ( val0.is_error() || val1.is_error() || val2.is_error() ) {
      return ElbValue();
    }
    break;

  case kVpiConcatOp:
  case kVpiMultiConcatOp:
    break;

  default:
    assert_not_reached(__FILE__, __LINE__);
    return ElbValue();
  }

  // 二回目は値の評価を行う．
  switch ( op_type ) {
  case kVpiBitNegOp:
    return ElbValue(~val0.bitvector_value());

  case kVpiUnaryAndOp:
    return ElbValue(val0.bitvector_value().reduction_and());

  case kVpiUnaryNandOp:
    return ElbValue(val0.bitvector_value().reduction_nand());

  case kVpiUnaryOrOp:
    return ElbValue(val0.bitvector_value().reduction_or());

  case kVpiUnaryNorOp:
    return ElbValue(val0.bitvector_value().reduction_nor());

  case kVpiUnaryXorOp:
    return ElbValue(val0.bitvector_value().reduction_xor());

  case kVpiUnaryXNorOp:
    return ElbValue(val0.bitvector_value().reduction_xnor());

  case kVpiPlusOp:
    return val0;

  case kVpiMinusOp:
    if ( val0.is_int() ) {
      return ElbValue(- val0.int_value());
    }
    if ( val0.is_bitvector() ) {
      return ElbValue(- val0.bitvector_value());
    }
    if ( val0.is_real() ) {
      return ElbValue(- val0.real_value());
    }

  case kVpiNotOp:
    if ( val0.scalar_value() == kVpiScalar0 ) {
      return ElbValue(kVpiScalar1);
    }
    else {
      return ElbValue(kVpiScalar0);
    }
    break;

  case kVpiBitAndOp:
    return ElbValue(val0.bitvector_value() & val1.bitvector_value());

  case kVpiBitOrOp:
    return ElbValue(val0.bitvector_value() | val1.bitvector_value());

  case kVpiBitXNorOp:
    return ElbValue(~(val0.bitvector_value() ^ val1.bitvector_value()));

  case kVpiBitXorOp:
    return ElbValue(val0.bitvector_value() ^ val1.bitvector_value());

  case kVpiLShiftOp:
    return ElbValue(val0.bitvector_value() << val1.bitvector_value());

  case kVpiRShiftOp:
    return ElbValue(val0.bitvector_value() >> val1.bitvector_value());

  case kVpiArithLShiftOp:
    return ElbValue(alshift(val0.bitvector_value(), val1.bitvector_value()));

  case kVpiArithRShiftOp:
    return ElbValue(arshift(val0.bitvector_value(), val1.bitvector_value()));

  case kVpiAddOp:
    return ElbValue(val0.bitvector_value() + val1.bitvector_value());

  case kVpiSubOp:
    return ElbValue(val0.bitvector_value() - val1.bitvector_value());

  case kVpiMultOp:
    return ElbValue(val0.bitvector_value() * val1.bitvector_value());

  case kVpiDivOp:
    return ElbValue(val0.bitvector_value() / val1.bitvector_value());

  case kVpiModOp:
    return ElbValue(val0.bitvector_value() % val1.bitvector_value());

  case kVpiPowerOp:
    return ElbValue(power(val0.bitvector_value(), val1.bitvector_value()));

  case kVpiLogAndOp:
    if ( val0.scalar_value() == kVpiScalar1 &&
	 val1.scalar_value() == kVpiScalar1 ) {
      return ElbValue(kVpiScalar1);
    }
    else {
      return ElbValue(kVpiScalar0);
    }

  case kVpiLogOrOp:
    if ( val0.scalar_value() == kVpiScalar1 ||
	 val1.scalar_value() == kVpiScalar1 ) {
      return ElbValue(kVpiScalar1);
    }
    else {
      return ElbValue(kVpiScalar0);
    }

  case kVpiCaseEqOp:
    if ( caseeq(val0, val1) ) {
      return ElbValue(kVpiScalar1);
    }
    else {
      return ElbValue(kVpiScalar0);
    }

  case kVpiCaseNeqOp:
    if ( caseeq(val0, val1) ) {
      return ElbValue(kVpiScalar0);
    }
    else {
      return ElbValue(kVpiScalar1);
    }

  case kVpiEqOp:
    return ElbValue(eq(val0, val1));

  case kVpiNeqOp:
    return ElbValue(!eq(val0, val1));

  case kVpiGeOp:
    return ElbValue(ge(val0, val1));

  case kVpiGtOp:
    return ElbValue(!ge(val1, val0));

  case kVpiLeOp:
    return ElbValue(ge(val1, val0));

  case kVpiLtOp:
    return ElbValue(!ge(val0, val1));

  case kVpiConditionOp:
    val0.to_logic();
    if ( val0.scalar_value() == kVpiScalar1 ) {
      return ElbValue(val1);
    }
    else {
      return ElbValue(val0);
    }

  case kVpiMinTypMaxOp:
    // 本当はエラー
    assert_not_reached(__FILE__, __LINE__);
    break;

  case kVpiConcatOp:
    {
      vector<BitVector> opr_list(opr_size);
      for (ymuint i = 0; i < opr_size; ++ i) {
	const PtExpr* pt_expr1 = pt_expr->operand(i);
	ElbValue val = evaluate_expr(parent, pt_expr1);
	if ( val.is_error() ) {
	  return ElbValue();
	}
	if ( val.is_real() ) {
	  error_illegal_real_type(pt_expr1);
	  return ElbValue();
	}
	val.to_bitvector();
	opr_list[i] = val.bitvector_value();
      }
      return ElbValue(concat(opr_list));
    }

  case kVpiMultiConcatOp:
    {
      const PtExpr* pt_expr0 = pt_expr->operand(0);
      ElbValue val0 = evaluate_expr(parent, pt_expr0);
      if ( val0.is_error() ) {
	return ElbValue();
      }
      val0.to_bitvector();

      vector<BitVector> opr_list(opr_size - 1);
      for (ymuint i = 1; i < opr_size; ++ i) {
	const PtExpr* pt_expr1 = pt_expr->operand(i);
	ElbValue val = evaluate_expr(parent, pt_expr1);
	if ( val.is_error() ) {
	  return ElbValue();
	}
	if ( val.is_real() ) {
	  error_illegal_real_type(pt_expr1);
	  return ElbValue();
	}
	val.to_bitvector();
	opr_list[i - 1] = val.bitvector_value();
      }
      return ElbValue(multi_concat(val0.bitvector_value(), opr_list));
    }

  default:
    assert_not_reached(__FILE__, __LINE__);
    break;
  }

  // ダミー
  return ElbValue();
}

END_NAMESPACE_YM_VERILOG
