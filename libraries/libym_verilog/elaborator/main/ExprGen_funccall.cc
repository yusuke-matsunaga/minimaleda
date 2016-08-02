
/// @file libym_verilog/elaborator/ExprGen_funccall.cc
/// @brief ElbMgr の実装ファイル(式の実体化)
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: ExprGen_funccall.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ExprGen.h"
#include "ElbEnv.h"

#include "ym_verilog/pt/PtModule.h"
#include "ym_verilog/pt/PtItem.h"
#include "ym_verilog/pt/PtExpr.h"
#include "ym_verilog/pt/PtMisc.h"

#include "ym_verilog/vl/VlModule.h"

#include "ElbIODecl.h"
#include "ElbDecl.h"
#include "ElbExpr.h"
#include "ElbTaskFunc.h"

BEGIN_NONAMESPACE

const int debug = 0;
#define dout cout

END_NONAMESPACE


BEGIN_NAMESPACE_YM_VERILOG

BEGIN_NONAMESPACE

void
put_value_type(ostream& s,
	       tVpiValueType type)
{
  if ( type == kVpiValueInteger ) {
    s << "integer type";
  }
  else if ( type == kVpiValueReal ) {
    s << "real type";
  }
  else if ( type == kVpiValueTime ) {
    s << "time type";
  }
  else {
    s << "bitvector type: ";
    if ( is_signed_type(type) ) {
      s << "signed";
    }
    else {
      s << "unsigned";
    }
    s << " , ";
    if ( is_sized_type(type) ) {
      s << unpack_size(type) << " bits";
    }
    else {
      s << "unsized";
    }
  }
}

END_NONAMESPACE

// @brief PtFuncCall から ElbExpr を生成する．
// @param[in] parent 親のスコープ
// @param[in] env 生成時の環境
// @param[in] pt_expr 式を表すパース木
ElbExpr*
ExprGen::instantiate_funccall(const VlNamedObj* parent,
			      const ElbEnv& env,
			      const PtExpr* pt_expr)
{
  const ElbTaskFunc* child_func = NULL;
  if ( env.is_constant() ) {
    // 定数関数を探し出す．
    PtNameBranchArray nb_array = pt_expr->namebranch_array();
    if ( nb_array.size() > 0 ) {
      error_hname_in_ce(pt_expr);
      return NULL;
    }

    // 関数名
    const char* name = pt_expr->name();

    // 関数本体を探し出す．
    // constant function はモジュール直下にしかあり得ない
    // <- generated scope 内の関数は constant function ではない．
    const VlModule* module = parent->parent_module();
    const PtModule* pt_module = find_moduledef(module->def_name());
    const PtItem* pt_func = pt_module->find_function(name);
    if ( !pt_func ) {
      error_no_such_function(pt_expr);
      return NULL;
    }

    if ( pt_func->is_in_use() ) {
      error_uses_itself(pt_expr);
      return NULL;
    }

    child_func = find_constant_function(module, name);
    if ( child_func == NULL ) {
      pt_func->set_in_use();
      // なかったので作る．
      child_func = instantiate_constant_function(parent, pt_func);
      pt_func->clear_in_use();
    }
    if ( !child_func ) {
      error_not_a_constant_function(pt_expr);
      return NULL;
    }
  }
  else {
    // 関数本体を探し出す．
    PtNameBranchArray nb_array = pt_expr->namebranch_array();
    const char* name = pt_expr->name();
    ElbObjHandle* handle = find_obj_up(parent, nb_array, name, NULL);
    if ( handle == NULL ) {
      error_no_such_function(pt_expr);
      return NULL;
    }
    if ( handle->type() != kVpiFunction ) {
      error_not_a_function(pt_expr);
      return NULL;
    }
    child_func = handle->taskfunc();
    assert_cond(child_func, __FILE__, __LINE__);
  }

  // 引数の生成
  ymuint n = pt_expr->operand_num();
  if ( n != child_func->io_num() ) {
    error_n_of_arguments_mismatch(pt_expr);
    return NULL;
  }

  ElbExpr** arg_list = factory().new_ExprList(n);
  for (ymuint i = 0; i < n; ++ i) {
    const PtExpr* pt_expr1 = pt_expr->operand(i);
    ElbExpr* expr1 = instantiate_expr(parent, env, pt_expr1);
    if ( !expr1 ) {
      // エラーが起った．
      return NULL;
    }
    ElbIODecl* io_decl = child_func->_io(i);
    ElbDecl* decl = io_decl->_decl();
    if ( decl->value_type() != expr1->value_type() ) {
      error_illegal_argument_type(pt_expr);
      if ( debug ) {
	dout << "decl->value_type() = ";
	put_value_type(dout, decl->value_type());
	dout << endl
	     << "expr1->value_type() = ";
	put_value_type(dout, expr1->value_type());
	dout << endl;
      }
    }
    arg_list[i] = expr1;
  }

  // function call の生成
  ElbExpr* expr = factory().new_FuncCall(pt_expr, child_func, n, arg_list);

  // attribute instance の生成
  //instantiate_attribute(pt_expr->attr_top(), false, expr);

  return expr;
}

// PtSysFuncCall から引数を生成する．
// @param[in] parent 親のスコープ
// @param[in] env 生成時の環境
// @param[in] pt_expr 式を表すパース木
ElbExpr*
ExprGen::instantiate_sysfunccall(const VlNamedObj* parent,
				 const ElbEnv& env,
				 const PtExpr* pt_expr)
{
  const char* name = pt_expr->name();

  // system function を探し出す．
  const ElbUserSystf* user_systf = find_user_systf(name);
  if ( user_systf == NULL ) {
    error_no_such_sysfunction(pt_expr);
    return NULL;
  }

#warning "TODO: 引数の個数と型のチェック"

  // 引数の生成
  ymuint n = pt_expr->operand_num();
  ElbExpr** arg_list = factory().new_ExprList(n);
  for (ymuint i = 0; i < n; ++ i) {
    const PtExpr* pt_expr1 = pt_expr->operand(i);
    ElbExpr* arg = NULL;
    if ( pt_expr ) {
      arg = instantiate_arg(parent, env, pt_expr1);
      if ( !arg ) {
	// エラーが起った．
	return NULL;
      }
    }
    else {
      // 関数呼び出しと異なり，空の引数がありうる．
      ;
    }
    arg_list[i] = arg;
  }

  // system function call の生成
  ElbExpr* expr = factory().new_SysFuncCall(pt_expr, user_systf, n, arg_list);

  return expr;
}

// @brief PtFuncCall から式の値を評価する．
// @param[in] parent 親のスコープ
// @param[in] pt_expr 式を表すパース木
ElbValue
ExprGen::evaluate_funccall(const VlNamedObj* parent,
			   const PtExpr* pt_expr)
{
  // 定数関数を探し出す．

  // 階層名は使えない．
  PtNameBranchArray nb_array = pt_expr->namebranch_array();
  if ( nb_array.size() > 0 ) {
    error_hname_in_ce(pt_expr);
    return ElbValue();
  }

  // 関数名
  const char* name = pt_expr->name();

  // 関数本体を探し出す．
  // constant function はモジュール直下にしかあり得ない
  // <- generated scope 内の関数は constant function ではない．
  const VlModule* module = parent->parent_module();
  const PtModule* pt_module = find_moduledef(module->def_name());
  const PtItem* pt_func = pt_module->find_function(name);
  if ( !pt_func ) {
    error_no_such_function(pt_expr);
    return ElbValue();
  }

  if ( pt_func->is_in_use() ) {
    error_uses_itself(pt_expr);
    return ElbValue();
  }

  const ElbTaskFunc* child_func = find_constant_function(module, name);
  if ( child_func == NULL ) {
    pt_func->set_in_use();
    // なかったので作る．
    child_func = instantiate_constant_function(parent, pt_func);
    pt_func->clear_in_use();
  }
  if ( !child_func ) {
    error_not_a_constant_function(pt_expr);
    return ElbValue();
  }

  // 引数の生成
  ymuint n = pt_expr->operand_num();
  if ( n != child_func->io_num() ) {
    error_n_of_arguments_mismatch(pt_expr);
    return ElbValue();
  }

  vector<ElbValue> arg_list(n);
  for (ymuint i = 0; i < n; ++ i) {
    const PtExpr* pt_expr1 = pt_expr->operand(i);
    ElbValue val1 = evaluate_expr(parent, pt_expr1);
    if ( val1.is_error() ) {
      // エラーが起った．
      return ElbValue();
    }
    ElbIODecl* io_decl = child_func->_io(i);
    ElbDecl* decl = io_decl->_decl();
    tVpiValueType decl_type = decl->value_type();
    if ( decl_type == kVpiValueReal ) {
      val1.to_real();
      if ( val1.is_error() ) {
	// 型が異なる．
	error_illegal_argument_type(pt_expr1);
	return ElbValue();
      }
    }
    else if ( is_bitvector_type(decl_type) ) {
      val1.to_bitvector();
      // 型が異なる．
      error_illegal_argument_type(pt_expr1);
      return ElbValue();
    }
    arg_list[i] = val1;
  }

#if 0
  // function call の生成
  ElbExpr* expr = factory().new_FuncCall(pt_expr, child_func, n, arg_list);
#else
  return ElbValue();
#endif
}

END_NAMESPACE_YM_VERILOG
