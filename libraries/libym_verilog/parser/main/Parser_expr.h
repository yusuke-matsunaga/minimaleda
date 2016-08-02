#ifndef LIBYM_VERILOG_PARSER_MAIN_PARSER_EXPR_H
#define LIBYM_VERILOG_PARSER_MAIN_PARSER_EXPR_H

/// @file libym_verilog/parser/main/Parser_expr.h
/// @brief Parser の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: Parser_expr.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "Parser.h"

#include "PtiFactory.h"


BEGIN_NAMESPACE_YM_VERILOG

// @brief 単項演算子の生成
// @param[in] fr ファイル位置の情報
// @param[in] type 演算の種類
// @param[in] opr オペランド
// @return 生成された演算子
inline
PtExpr*
Parser::new_Opr(const FileRegion& fr,
		tVpiOpType type,
		PtExpr* opr,
		PtrList<PtAttrInst>* ai_list)
{
  PtExpr* expr = mFactory.new_Opr(fr, type, opr);
  reg_attrinst(expr, ai_list);
  return expr;
}

// @brief 二項演算子の生成
// @param[in] fr ファイル位置の情報
// @param[in] type 演算の種類
// @param[in] opr1 オペランド1
// @param[in] opr2 オペランド2
// @return 生成された演算子
inline
PtExpr*
Parser::new_Opr(const FileRegion& fr,
		tVpiOpType type,
		PtExpr* opr1,
		PtExpr* opr2,
		PtrList<PtAttrInst>* ai_list)
{
  PtExpr* expr = mFactory.new_Opr(fr, type, opr1, opr2);
  reg_attrinst(expr, ai_list);
  return expr;
}

// @brief 三項演算子の生成
// @param[in] fr ファイル位置の情報
// @param[in] type 演算の種類
// @param[in] opr1 オペランド1
// @param[in] opr2 オペランド2
// @param[in] opr3 オペランド3
// @return 生成された演算子
inline
PtExpr*
Parser::new_Opr(const FileRegion& fr,
		tVpiOpType type,
		PtExpr* opr1,
		PtExpr* opr2,
		PtExpr* opr3,
		PtrList<PtAttrInst>* ai_list)
{
  PtExpr* expr = mFactory.new_Opr(fr, type, opr1, opr2, opr3);
  reg_attrinst(expr, ai_list);
  return expr;
}

// @brief concatination 演算子の生成
// @param[in] fr ファイル位置の情報
// @param[in] expr_list オペランドのリスト
// @return 生成された concatination 演算子
inline
PtExpr*
Parser::new_Concat(const FileRegion& fr,
		   PtrList<PtExpr>* expr_list)
{
  return mFactory.new_Concat(fr, to_array(expr_list));
}

// @brief multi-concatination 演算子の生成
// @param[in] fr ファイル位置の情報
// @param[in] rep 繰り返し数
// @param[in] expr_list 結合するオペランドのリスト
// @return 生成された multi-concatination 演算子
inline
PtExpr*
Parser::new_MultiConcat(const FileRegion& fr,
			PtExpr* rep,
			PtrList<PtExpr>* expr_list)
{
  return mFactory.new_MultiConcat(fr, ExprArray(rep, expr_list));
}
  
// @brief min/typ/max delay 演算子の生成
// @param[in] fr ファイル位置の情報
// @param[in] val0 minimum 値
// @param[in] val1 typical 値
// @param[in] val2 maximum 値
// @return 生成された min/typ/max 演算子
inline
PtExpr*
Parser::new_MinTypMax(const FileRegion& fr,
		      PtExpr* val0,
		      PtExpr* val1,
		      PtExpr* val2)
{
  return mFactory.new_MinTypMax(fr, val0, val1, val2);
}

// @brief primary の生成
// @param[in] fr ファイル位置の情報
// @param[in] name 識別子名
// @return 生成された primary
inline
PtExpr*
Parser::new_Primary(const FileRegion& fr,
		    const char* name)
{
  return mFactory.new_Primary(fr, name);
}

// @brief インデックス付き primary の生成
// @param[in] fr ファイル位置の情報
// @param[in] name 識別子名
// @param[in] index_list インデックスのリスト
// @return 生成された primary
inline
PtExpr*
Parser::new_Primary(const FileRegion& fr,
		    const char* name,
		    PtrList<PtExpr>* index_array)
{
  return mFactory.new_Primary(fr, name, to_array(index_array));
}

// @brief 範囲指定付き primary の生成
// @param[in] fr ファイル位置の情報
// @param[in] name 識別子名
// @param[in] mode 範囲指定のモード
// @param[in] left 範囲の左側の式
// @param[in] right 範囲の右側の式
// @return 生成された primary
PtExpr*
Parser::new_Primary(const FileRegion& fr,
		    const char* name,
		    tVpiRangeMode mode,
		    PtExpr* left,
		    PtExpr* right)
{
  return mFactory.new_Primary(fr, name, mode, left, right);
}

// @brief インデックスと範囲指定付き primary の生成
// @param[in] fr ファイル位置の情報
// @param[in] name 識別子名
// @param[in] index_list インデックスのリスト
// @param[in] mode 範囲指定のモード
// @param[in] left 範囲の左側の式
// @param[in] right 範囲の右側の式
// @return 生成された primary
PtExpr*
Parser::new_Primary(const FileRegion& fr,
		    const char* name,
		    PtrList<PtExpr>* index_list,
		    tVpiRangeMode mode,
		    PtExpr* left,
		    PtExpr* right)
{
  return mFactory.new_Primary(fr, name, to_array(index_list),
			      mode, left, right);
}

// @brief primary の生成 (階層付き)
// @param[in] fr ファイル位置の情報
// @param[in] hname 階層名
// @return 生成された primary
inline
PtExpr*
Parser::new_Primary(const FileRegion& fr,
		    PuHierName* hname)
{
  PtNameBranchArray nb_array;
  const char* tail_name = extract_HierName(hname, nb_array);
  return mFactory.new_Primary(fr, nb_array, tail_name);
}

// @brief インデックス付き primary の生成 (階層付き)
// @param[in] fr ファイル位置の情報
// @param[in] hname 階層名
// @param[in] index_list インデックスのリスト
// @return 生成された primary
inline
PtExpr*
Parser::new_Primary(const FileRegion& fr,
		    PuHierName* hname,
		    PtrList<PtExpr>* index_list)
{
  PtNameBranchArray nb_array;
  const char* tail_name = extract_HierName(hname, nb_array);
  return mFactory.new_Primary(fr, nb_array, tail_name, to_array(index_list));
}

// @brief 範囲指定付き primary の生成 (階層付き)
// @param[in] fr ファイル位置の情報
// @param[in] hname 階層名
// @param[in] mode 範囲指定のモード
// @param[in] left 範囲の左側の式
// @param[in] right 範囲の右側の式
// @return 生成された primary
inline
PtExpr*
Parser::new_Primary(const FileRegion& fr,
		    PuHierName* hname,
		    tVpiRangeMode mode,
		    PtExpr* left,
		    PtExpr* right)
{
  PtNameBranchArray nb_array;
  const char* tail_name = extract_HierName(hname, nb_array);
  return mFactory.new_Primary(fr, nb_array, tail_name, mode, left, right);
}

// @brief インデックスと範囲指定付き primary の生成 (階層付き)
// @param[in] fr ファイル位置の情報
// @param[in] hname 階層名
// @param[in] index_list インデックスのリスト
// @param[in] mode 範囲指定のモード
// @param[in] left 範囲の左側の式
// @param[in] right 範囲の右側の式
// @return 生成された primary
inline
PtExpr*
Parser::new_Primary(const FileRegion& fr,
		    PuHierName* hname,
		    PtrList<PtExpr>* index_list,
		    tVpiRangeMode mode,
		    PtExpr* left,
		    PtExpr* right)
{
  PtNameBranchArray nb_array;
  const char* tail_name = extract_HierName(hname, nb_array);
  return mFactory.new_Primary(fr, nb_array, tail_name, to_array(index_list),
			      mode, left, right);
}
  
// @brief constant primary の生成
// @param[in] fr ファイル位置の情報
// @param[in] name 識別子名
// @param[in] index インデックス
// @return 生成された const primary
inline
PtExpr*
Parser::new_CPrimary(const FileRegion& fr,
		     const char* name,
		     PtExpr* index)
{
  return mFactory.new_CPrimary(fr, name, index);
}
  
// @brief constant primary の生成
// @param[in] fr ファイル位置の情報
// @param[in] name 識別子名
// @param[in] index_list インデックスのリスト
// @return 生成された const primary
inline
PtExpr*
Parser::new_CPrimary(const FileRegion& fr,
		     const char* name,
		     PtrList<PtExpr>* index_list)
{
  return mFactory.new_CPrimary(fr, name, to_array(index_list));
}

// @brief 範囲指定付き constant primary の生成
// @param[in] fr ファイル位置の情報
// @param[in] name 識別子名
// @param[in] mode 範囲指定のモード
// @param[in] left 範囲の左側の式
// @param[in] right 範囲の右側の式
// @return 生成された constant primary
inline
PtExpr*
Parser::new_CPrimary(const FileRegion& fr,
		     const char* name,
		     tVpiRangeMode mode,
		     PtExpr* left,
		     PtExpr* right)
{
  return mFactory.new_CPrimary(fr, name, mode, left, right);
}
  
// @brief インデックス付き constant primary の生成 (階層付き)
// @param[in] fr ファイル位置の情報
// @param[in] hname 階層名
// @param[in] index_list インデックスのリスト
// @return 生成された constant primary
inline
PtExpr*
Parser::new_CPrimary(const FileRegion& fr,
		     PuHierName* hname,
		     PtrList<PtExpr>* index_list)
{
  PtNameBranchArray nb_array;
  const char* tail_name = extract_HierName(hname, nb_array);
  return mFactory.new_CPrimary(fr, nb_array, tail_name, to_array(index_list));
}
  
// @brief function call の生成
// @param[in] fr ファイル位置の情報
// @param[in] name 関数名
// @param[in] arg_list 引数のリスト
// @return 生成された function call
inline
PtExpr*
Parser::new_FuncCall(const FileRegion& fr,
		     const char* name,
		     PtrList<PtExpr>* arg_list,
		     PtrList<PtAttrInst>* ai_list)
{
  PtExpr* expr = mFactory.new_FuncCall(fr, name, to_array(arg_list));
  reg_attrinst(expr, ai_list);
  return expr;
}

// @brief function call の生成 (階層付き)
// @param[in] fr ファイル位置の情報
// @param[in] hname 階層名
// @param[in] arg_list 引数のリスト
// @return 生成された function call
inline
PtExpr*
Parser::new_FuncCall(const FileRegion& fr,
		     PuHierName* hname,
		     PtrList<PtExpr>* arg_list,
		     PtrList<PtAttrInst>* ai_list)
{
  PtNameBranchArray nb_array;
  const char* tail_name = extract_HierName(hname, nb_array);
  PtExpr* expr = mFactory.new_FuncCall(fr, nb_array, tail_name, to_array(arg_list));
  reg_attrinst(expr, ai_list);
  return expr;
}

// @brief system function call の生成
// @param[in] fr ファイル位置の情報
// @param[in] name 関数名
// @param[in] arg_array 引数のリスト
// @return 生成された function call
inline
PtExpr*
Parser::new_SysFuncCall(const FileRegion& fr,
			const char* name,
			PtrList<PtExpr>* arg_list)
{
  return mFactory.new_SysFuncCall(fr, name, to_array(arg_list));
}

// @brief 整数型の定数の生成
// @param[in] fr ファイル位置の情報
// @param[in] value 定数を表す整数値
// @return 生成された定数
inline
PtExpr*
Parser::new_IntConst(const FileRegion& fr,
		     ymuint32 value)
{
  return mFactory.new_IntConst(fr, value);
}

// @brief 整数型の定数の生成
// @param[in] fr ファイル位置の情報
// @param[in] value 定数の文字列表現
// @return 生成された定数
inline
PtExpr*
Parser::new_IntConst(const FileRegion& fr,
		     const char* value)
{
  return mFactory.new_IntConst(fr, value);
}

// @brief 基底付き整数型の定数の生成
// @param[in] fr ファイル位置の情報
// @param[in] const_type 定数の種類
// @param[in] value 定数の文字列表現
// @return 生成された定数
inline
PtExpr*
Parser::new_IntConst(const FileRegion& fr,
		     tVpiConstType const_type,
		     const char* value)
{
  return mFactory.new_IntConst(fr, const_type, value);
}

// @brief サイズと基底付き定数の生成
// @param[in] fr ファイル位置の情報
// @param[in] size サイズ
// @param[in] const_type 定数の種類
// @param[in] value 定数の文字列表現
// @return 生成された定数
inline
PtExpr*
Parser::new_IntConst(const FileRegion& fr,
		     ymuint32 size,
		     tVpiConstType const_type,
		     const char* value)
{
  return mFactory.new_IntConst(fr, size, const_type, value);
}

// @brief 実数型の定数の生成
// @param[in] fr ファイル位置の情報
// @param[in] value 定数の文字列表現
// @return 生成された定数
inline
PtExpr*
Parser::new_RealConst(const FileRegion& fr,
		      double value)
{
  return mFactory.new_RealConst(fr, value);
}

// @brief 文字列型の定数の生成
// @param[in] fr ファイル位置の情報
// @param[in] value 文字列
// @return 生成された定数
inline
PtExpr*
Parser::new_StringConst(const FileRegion& fr,
			const char* value)
{
  return mFactory.new_StringConst(fr, value);
}

END_NAMESPACE_YM_VERILOG

#endif // LIBYM_VERILOG_PARSER_MAIN_PARSER_EXPR_H
