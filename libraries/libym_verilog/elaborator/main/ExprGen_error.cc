
/// @file libym_verilog/elaborator/ExprGen_error.cc
/// @brief ElbMgr の実装ファイル(エラーメッセージ出力)
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: ExprGen_primary.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ExprGen.h"

#include "ym_verilog/pt/PtExpr.h"
#include "ym_verilog/pt/PtMisc.h"


BEGIN_NAMESPACE_YM_VERILOG

// @brief constant function 中にシステム関数呼び出し
// @param[in] pt_expr 式を表すパース木
void
ExprGen::error_illegal_sysfunccall_in_cf(const PtExpr* pt_expr)
{
  error_common(pt_expr,
	       "ELABXXX",
	       "System-function call shall not be used in constant function.");
}

// @brief constant expression 中にシステム関数呼び出し
// @param[in] pt_expr 式を表すパース木
void
ExprGen::error_illegal_sysfunccall_in_ce(const PtExpr* pt_expr)
{
  error_common(pt_expr,
	       "ELABXXX",
	       "System-function call shall not be used in constant expression.");
}

// @brief イベント式の根元に定数
// @param[in] pt_expr 式を表すパース木
void
ExprGen::error_illegal_constant_in_event_expression(const PtExpr* pt_expr)
{
  error_common(pt_expr,
	       "ELABXXX",
	       "Constant shall not be used in event description.");
}

// @brief イベント式の根元に関数呼び出し
// @param[in] pt_expr 式を表すパース木
void
ExprGen::error_illegal_funccall_in_event_expression(const PtExpr* pt_expr)
{
  error_common(pt_expr,
	       "ELABXXX",
	       "Function call shall not be used in event description.");
}

// @brief イベント式の根元にシステム関数呼び出し
// @param[in] pt_expr 式を表すパース木
void
ExprGen::error_illegal_sysfunccall_in_event_expression(const PtExpr* pt_expr)
{
  error_common(pt_expr,
	       "ELABXXX",
	       "System-function call shall not be used in event description.");
}

// @brief 左辺式で用いることのできない演算子
// @param[in] pt_expr 式を表すパース木
void
ExprGen::error_illegal_operator_in_lhs(const PtExpr* pt_expr)
{
  error_common(pt_expr,
	       "ELABXXX",
	       "Illegal operator in LHS.");
}

// @brief 左辺式に定数
// @param[in] pt_expr 式を表すパース木
void
ExprGen::error_illegal_constant_in_lhs(const PtExpr* pt_expr)
{
  error_common(pt_expr,
	       "ELABXXX",
	       "Constant shall not be used in LHS.");
}

// @brief 左辺式に関数呼び出し
// @param[in] pt_expr 式を表すパース木
void
ExprGen::error_illegal_funccall_in_lhs(const PtExpr* pt_expr)
{
  error_common(pt_expr,
	       "ELABXXX",
	       "Function call shall not be used in LHS.");
}

// @brief 左辺式にシステム関数呼び出し
// @param[in] pt_expr 式を表すパース木
void
ExprGen::error_illegal_sysfunccall_in_lhs(const PtExpr* pt_expr)
{
  error_common(pt_expr,
	       "ELABXXX",
	       "System-function call shall not be used in LHS.");
}

// @brief 通常の式中に edge descriptor
// @param[in] pt_expr 式を表すパース木
void
ExprGen::error_illegal_edge_descriptor(const PtExpr* pt_expr)
{
  error_common(pt_expr,
	       "ELABXXX",
	       "Edge descriptor in an expression");
}

// @brief real 型のオペランドをとれない
// @param[in] pt_expr 式を表すパース木
void
ExprGen::error_illegal_real_type(const PtExpr* pt_expr)
{
  error_common(pt_expr,
	       "ELABXXX",
	       "Shall not have a real-type operand.");
}

// @brief 該当する関数が存在しない．
// @param[in] pt_expr 式を表すパース木
void
ExprGen::error_no_such_function(const PtExpr* pt_expr)
{
  error_with_hname(pt_expr,
		   "ELABXXX",
		   "No such function.");
}

// @brief 該当するシステム関数が存在しない．
// @param[in] pt_expr 式を表すパース木
void
ExprGen::error_no_such_sysfunction(const PtExpr* pt_expr)
{
  error_with_hname(pt_expr,
		   "ELABXXX",
		   "No such system function.");
}

// @brief 定数関数は自己再帰できない．
// @param[in] pt_expr 式を表すパース木
void
ExprGen::error_uses_itself(const PtExpr* pt_expr)
{
  error_with_hname(pt_expr,
		   "ELABXXX",
		   "Uses itself.");
}

// @brief 定数関数ではない．
// @param[in] pt_expr 式を表すパース木
void
ExprGen::error_not_a_constant_function(const PtExpr* pt_expr)
{
  error_with_hname(pt_expr,
		   "ELABXXX",
		   "Not a constant function.");
}

// @brief 関数ではない．
// @param[in] pt_expr 式を表すパース木
void
ExprGen::error_not_a_function(const PtExpr* pt_expr)
{
  error_with_hname(pt_expr,
		   "ELABXXX",
		   "Not a function.");
}

// @brief 引数の数が合わない．
// @param[in] pt_expr 式を表すパース木
void
ExprGen::error_n_of_arguments_mismatch(const PtExpr* pt_expr)
{
  error_common(pt_expr,
	       "ELABXXX",
	       "# of arguments does not match.");
}

// @brief 引数の型が合わない．
// @param[in] pt_expr 式を表すパース木
void
ExprGen::error_illegal_argument_type(const PtExpr* pt_expr)
{
  error_common(pt_expr,
	       "ELABXXX",
	       "Actual argument type does not match"
	       " with formal argument.");
}

// @brief オブジェクトが存在しない
// @param[in] pt_expr 式を表すパース木
void
ExprGen::error_not_found(const PtExpr* pt_expr)
{
  error_with_hname(pt_expr,
		   "ELABXXX",
		   "Not found.");
}

// @brief オブジェクトの型が不適切だった
void
ExprGen::error_illegal_object(const PtExpr* pt_expr)
{
  error_with_hname(pt_expr,
		   "ELABXXX",
		   "Illegal object type.");
}

// @brief オブジェクトの型が constant function 用として不適切だった
// @param[in] pt_expr 式を表すパース木
void
ExprGen::error_illegal_object_cf(const PtExpr* pt_expr)
{
  error_with_hname(pt_expr,
		   "ELABXXX",
		   "Illegal object type inside constant function.");
}

// @brief 階層名が constant expression 中にあった
// @param[in] pt_expr 式を表すパース木
void
ExprGen::error_hname_in_ce(const PtExpr* pt_expr)
{
  error_with_hname(pt_expr,
		   "ELABXXX",
		   "Hierarchical name shall not be used"
		   " in constant expression.");
}

// @brief 階層名が constant function 中にあった
// @param[in] pt_expr 式を表すパース木
void
ExprGen::error_hname_in_cf(const PtExpr* pt_expr)
{
  error_with_hname(pt_expr,
		   "ELABXXX",
		   "Hierarchical name shall not be used"
		   " inside constant function.");
}

// @brief オブジェクトが parameter でなかった
// @param[in] pt_expr 式を表すパース木
void
ExprGen::error_not_a_parameter(const PtExpr* pt_expr)
{
  error_with_hname(pt_expr,
		   "ELABXXX",
		   "Not a parameter.");
}

// @brief オブジェクトが named-event でなかった
// @param[in] pt_expr 式を表すパース木
void
ExprGen::error_not_a_namedevent(const PtExpr* pt_expr)
{
  error_with_hname(pt_expr,
		   "ELABXXX",
		   "Not a named-event.");
}

// @brief named-event に対する範囲指定
void
ExprGen::error_select_for_namedevent(const PtExpr* pt_expr)
{
  error_with_hname(pt_expr,
		   "ELABXXX",
		   "Illegal part-select for a named-event.");
}

// @brief assign/deassign に不適切なビット/範囲指定
// @param[in] pt_expr 式を表すパース木
void
ExprGen::error_select_in_pca(const PtExpr* pt_expr)
{
  error_with_hname(pt_expr,
		   "ELABXXX",
		   "Bit/part-select shall not be used"
		   " in LHS of assign/deassign statement.");
}

// @brief force/release に不適切なビット/範囲指定
// @param[in] pt_expr 式を表すパース木
void
ExprGen::error_select_in_force(const PtExpr* pt_expr)
{
  error_with_hname(pt_expr,
		   "ELABXXX",
		   "Bit/part-select shall not be used"
		   " in LHS of force/release statement.");
}

// @brief assign/deassign に不適切な配列要素
// @param[in] pt_expr 式を表すパース木
void
ExprGen::error_array_in_pca(const PtExpr* pt_expr)
{
  error_with_hname(pt_expr,
		   "ELABXXX",
		   "Array element shall not be used"
		   "in LHS of assign/deassign statement.");
}

// @brief force/release に不適切な配列要素
// @param[in] pt_expr 式を表すパース木
void
ExprGen::error_array_in_force(const PtExpr* pt_expr)
{
  error_with_hname(pt_expr,
		   "ELABXXX",
		   "Array element shall not be used"
		   "in LHS of force/release statement.");
}

// @brief 配列の次元が合わない
void
ExprGen::error_dimension_mismatch(const PtExpr* pt_expr)
{
  error_with_hname(pt_expr,
		   "ELABXXX",
		   "Dimension mismatch.");
}

// @brief real 型に対するビット選択あるいは部分選択があった
// @param[in] pt_expr 式を表すパース木
void
ExprGen::error_select_for_real(const PtExpr* pt_expr)
{
  error_with_hname(pt_expr,
		   "ELABXXX",
		   "Bit-select/Part-select for real.");
}

// @brief 階層名付きのエラーメッセージを生成する共通部分
// @param[in] pt_expr 式を表すパース木
// @param[in] label エラーラベル
// @param[in] msg エラーメッセージ
void
ExprGen::error_with_hname(const PtExpr* pt_expr,
			  const char* label,
			  const char* msg)
{
  ostringstream buf;
  buf << expand_full_name(pt_expr->namebranch_array(), pt_expr->name())
      << " : " << msg;
  error_common(pt_expr, label, buf.str().c_str());
}

// @brief エラーメッセージを生成する共通部分
// @param[in] pt_expr 式を表すパース木
// @param[in] label エラーラベル
// @param[in] msg エラーメッセージ
void
ExprGen::error_common(const PtExpr* pt_expr,
		      const char* label,
		      const char* msg)
{
  put_msg(__FILE__, __LINE__,
	  pt_expr->file_region(),
	  kMsgError,
	  label,
	  msg);
}

END_NAMESPACE_YM_VERILOG
