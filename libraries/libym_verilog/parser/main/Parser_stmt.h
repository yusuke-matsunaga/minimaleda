#ifndef LIBYM_VERILOG_PARSER_MAIN_PARSER_STMT_H
#define LIBYM_VERILOG_PARSER_MAIN_PARSER_STMT_H

/// @file libym_verilog/parser/main/Parser_stmt.h
/// @brief Parser の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: Parser_stmt.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "Parser.h"

#include "PtiFactory.h"
#include "ym_verilog/pt/PtExpr.h"


BEGIN_NAMESPACE_YM_VERILOG

// @brief disable 文の生成
// @param[in] fr ファイル位置の情報
// @param[in] name 停止するスコープ名
// @return 生成された disable 文
inline
PtStmt*
Parser::new_Disable(const FileRegion& fr,
		    const char* name,
		    PtrList<PtAttrInst>* ai_list)
{
  PtStmt* stmt = mFactory.new_Disable(fr, name);
  reg_attrinst(stmt, ai_list);
  return stmt;
}
  
// @brief disable 文の生成 (階層付き識別子)
// @param[in] fr ファイル位置の情報
// @param[in] hname 階層名
// @param[in] name 末尾の名前
// @return 生成された disable 文
inline
PtStmt*
Parser::new_Disable(const FileRegion& fr,
		    PuHierName* hname,
		    PtrList<PtAttrInst>* ai_list)
{
  PtNameBranchArray nb_array;
  const char* tail_name = extract_HierName(hname, nb_array);
  PtStmt* stmt = mFactory.new_Disable(fr, nb_array, tail_name);
  reg_attrinst(stmt, ai_list);
  return stmt;
}

// @brief enable 文の生成
// @param[in] fr ファイル位置の情報
// @param[in] name 起動するタスク名
// @param[in] arg_array 引数のリスト
// @return 生成された enable 文
inline
PtStmt*
Parser::new_Enable(const FileRegion& fr,
		   const char* name,
		   PtrList<PtExpr>* arg_list,
		   PtrList<PtAttrInst>* ai_list)
{
  PtStmt* stmt = mFactory.new_Enable(fr, name, to_array(arg_list));
  reg_attrinst(stmt, ai_list);
  return stmt;
}

// @brief enable 文の生成 (階層付き識別子)
// @param[in] fr ファイル位置の情報
// @param[in] hname 階層名
// @param[in] name 末尾の名前
// @param[in] arg_array 引数のリスト
// @return 生成された enable 文
inline
PtStmt*
Parser::new_Enable(const FileRegion& fr,
		   PuHierName* hname,
		   PtrList<PtExpr>* arg_list,
		   PtrList<PtAttrInst>* ai_list)
{
  PtNameBranchArray nb_array;
  const char* tail_name = extract_HierName(hname, nb_array);
  PtStmt* stmt = mFactory.new_Enable(fr, nb_array, tail_name, to_array(arg_list));
  reg_attrinst(stmt, ai_list);
  return stmt;
}

// @brief system task enable 文の生成
// @param[in] fr ファイル位置の情報
// @param[in] name 起動するシステムタスク名
// @param[in] arg_array 引数のリスト
inline
PtStmt*
Parser::new_SysEnable(const FileRegion& fr,
		      const char* name,
		      PtrList<PtExpr>* arg_list,
		      PtrList<PtAttrInst>* ai_list)
{
  PtStmt* stmt = mFactory.new_SysEnable(fr, name, to_array(arg_list));
  reg_attrinst(stmt, ai_list);
  return stmt;
}

// @brief delay control 文の生成
// @param[in] fr ファイル位置の情報
// @param[in] delay delay control (delay control 文とは別物)
// @param[in] body 本体のステートメント
// @return 生成された delay control 文
inline
PtStmt*
Parser::new_DcStmt(const FileRegion& fr,
		   PtControl* delay,
		   PtStmt* body,
		   PtrList<PtAttrInst>* ai_list)
{
  PtStmt* stmt = mFactory.new_DcStmt(fr, delay, body);
  reg_attrinst(stmt, ai_list);
  return stmt;
}

// @brief event control 文の生成
// @param[in] fr ファイル位置の情報
// @param[in] event event control (event control 文とは別物)
// @param[in] body 本体のステートメント
// @return 生成された event control 文
inline
PtStmt*
Parser::new_EcStmt(const FileRegion& fr,
		   PtControl* event,
		   PtStmt* body,
		   PtrList<PtAttrInst>* ai_list)
{
  PtStmt* stmt = mFactory.new_EcStmt(fr, event, body);
  reg_attrinst(stmt, ai_list);
  return stmt;
}

// @brief wait 文の生成
// @param[in] fr ファイル位置の情報
// @param[in] cond 条件式
// @param[in] body 本体のステートメント
// @return 生成された wait 文
inline
PtStmt*
Parser::new_Wait(const FileRegion& fr,
		 PtExpr* cond,
		 PtStmt* body,
		 PtrList<PtAttrInst>* ai_list)
{
  PtStmt* stmt = mFactory.new_Wait(fr, cond, body);
  reg_attrinst(stmt, ai_list);
  return stmt;
}

// @brief assign 文の生成
// @param[in] fr ファイル位置の情報
// @param[in] lhs 左辺式
// @param[in] rhs 右辺式
// @return 生成された assign 文
inline
PtStmt*
Parser::new_Assign(const FileRegion& fr,
		   PtExpr* lhs,
		   PtExpr* rhs,
		   PtrList<PtAttrInst>* ai_list)
{
  PtStmt* stmt = mFactory.new_Assign(fr, lhs, rhs);
  reg_attrinst(stmt, ai_list);
  return stmt;
}

// @brief control 付き assign 文の生成
// @param[in] fr ファイル位置の情報
// @param[in] lhs 左辺式
// @param[in] rhs 右辺式
// @param[in] control 制御式
// @return 生成された assign 文
inline
PtStmt*
Parser::new_Assign(const FileRegion& fr,
		   PtExpr* lhs,
		   PtExpr* rhs,
		   PtControl* control,
		   PtrList<PtAttrInst>* ai_list)
{
  PtStmt* stmt = mFactory.new_Assign(fr, lhs, rhs, control);
  reg_attrinst(stmt, ai_list);
  return stmt;
}

// @brief nonblocking assign 文の生成
// @param[in] fr ファイル位置の情報
// @param[in] lhs 左辺式
// @param[in] rhs 右辺式
// @return 生成された nonblocking assign 文
inline
PtStmt*
Parser::new_NbAssign(const FileRegion& fr,
		     PtExpr* lhs,
		     PtExpr* rhs,
		     PtrList<PtAttrInst>* ai_list)
{
  PtStmt* stmt = mFactory.new_NbAssign(fr, lhs, rhs);
  reg_attrinst(stmt, ai_list);
  return stmt;
}

// @brief control 付き nonblocking assign 文の生成
// @param[in] fr ファイル位置の情報
// @param[in] lhs 左辺式
// @param[in] rhs 右辺式
// @param[in] control 制御式
// @return 生成された nonblocking assign 文
inline
PtStmt*
Parser::new_NbAssign(const FileRegion& fr,
		     PtExpr* lhs,
		     PtExpr* rhs,
		     PtControl* control,
		     PtrList<PtAttrInst>* ai_list)
{
  PtStmt* stmt = mFactory.new_NbAssign(fr, lhs, rhs, control);
  reg_attrinst(stmt, ai_list);
  return stmt;
}

// @brief event 文の生成
// @param[in] fr ファイル位置の情報
// @param[in] event イベント名
// @return 生成された event 文
inline
PtStmt*
Parser::new_EventStmt(const FileRegion& fr,
		      PtExpr* event,
		      PtrList<PtAttrInst>* ai_list)
{
  PtStmt* stmt = mFactory.new_EventStmt(fr, event);
  reg_attrinst(stmt, ai_list);
  return stmt;
}

// @brief null 文の生成
// @param[in] fr ファイル位置の情報
// @return 生成された null 文
inline
PtStmt*
Parser::new_NullStmt(const FileRegion& fr,
		     PtrList<PtAttrInst>* ai_list)
{
  PtStmt* stmt = mFactory.new_NullStmt(fr);
  reg_attrinst(stmt, ai_list);
  return stmt;
}

// @brief if 文の生成
// @param[in] fr ファイル位置の情報
// @param[in] expr 条件式
// @param[in] then_body 成り立ったときに実行されるステートメント
// @return 生成された if 文
inline
PtStmt*
Parser::new_If(const FileRegion& fr,
	       PtExpr* expr,
	       PtStmt* then_body,
	       PtrList<PtAttrInst>* ai_list)
{
  PtStmt* stmt = mFactory.new_If(fr, expr, then_body);
  reg_attrinst(stmt, ai_list);
  return stmt;
}

// @brief if 文の生成
// @param[in] fr ファイル位置の情報
// @param[in] expr 条件式
// @param[in] then_body 成り立ったときに実行されるステートメント
// @param[in] else_body 成り立たなかったときに実行されるステートメント
// @return 生成された if 文
inline
PtStmt*
Parser::new_If(const FileRegion& fr,
	       PtExpr* expr,
	       PtStmt* then_body,
	       PtStmt* else_body,
	       PtrList<PtAttrInst>* ai_list)
{
  PtStmt* stmt = mFactory.new_If(fr, expr, then_body, else_body);
  reg_attrinst(stmt, ai_list);
  return stmt;
}

// @brief case 文の生成
// @param[in] fr ファイル位置の情報
// @param[in] expr 条件式
// @param[in] caseitem_list case item のリスト
// @return 生成された case 文
inline
PtStmt*
Parser::new_Case(const FileRegion& fr,
		 PtExpr* expr,
		 PtrList<PtCaseItem>* caseitem_list,
		 PtrList<PtAttrInst>* ai_list)
{
  if ( !check_default_label(caseitem_list) ) {
    return NULL;
  }
  PtStmt* stmt = mFactory.new_Case(fr, expr, to_array(caseitem_list));
  reg_attrinst(stmt, ai_list);
  return stmt;
}

// @brief casex 文の生成
// @param[in] fr ファイル位置の情報
// @param[in] expr 条件式
// @param[in] caseitem_list case item のリスト
// @return 生成された case 文
inline
PtStmt*
Parser::new_CaseX(const FileRegion& fr,
		  PtExpr* expr,
		  PtrList<PtCaseItem>* caseitem_list,
		  PtrList<PtAttrInst>* ai_list)
{
  if ( !check_default_label(caseitem_list) ) {
    return NULL;
  }
  PtStmt* stmt = mFactory.new_CaseX(fr, expr, to_array(caseitem_list));
  reg_attrinst(stmt, ai_list);
  return stmt;
}

// @brief casez 文の生成
// @param[in] fr ファイル位置の情報
// @param[in] expr 条件式
// @param[in] caseitem_list case item のリスト
// @return 生成された case 文
inline
PtStmt*
Parser::new_CaseZ(const FileRegion& fr,
		  PtExpr* expr,
		  PtrList<PtCaseItem>* caseitem_list,
		  PtrList<PtAttrInst>* ai_list)
{
  if ( !check_default_label(caseitem_list) ) {
    return NULL;
  }
  PtStmt* stmt = mFactory.new_CaseZ(fr, expr, to_array(caseitem_list));
  reg_attrinst(stmt, ai_list);
  return stmt;
}

// @brief case item の生成
// @param[in] fr ファイル位置の情報
// @param[in] label_list ラベルのリスト
// @param[in] body 本体のステートメント
// @return 生成された case item
inline
PtCaseItem*
Parser::new_CaseItem(const FileRegion& fr,
		     PtrList<PtExpr>* label_list,
		     PtStmt* body)
{
  return mFactory.new_CaseItem(fr, to_array(label_list), body);
}

// @brief forever 文の生成
// @param[in] fr ファイル位置の情報
// @param[in] body 本体のステートメント
// @return 生成された forever 文
inline
PtStmt*
Parser::new_Forever(const FileRegion& fr,
		    PtStmt* body,
		    PtrList<PtAttrInst>* ai_list)
{
  PtStmt* stmt = mFactory.new_Forever(fr, body);
  reg_attrinst(stmt, ai_list);
  return stmt;
}

// @brief repeat 文の生成
// @param[in] fr ファイル位置の情報
// @param[in] expr 繰り返し数を表す式
// @param[in] body 本体のステートメント
// @return 生成された repeat 文
inline
PtStmt*
Parser::new_Repeat(const FileRegion& fr,
		   PtExpr* expr,
		   PtStmt* body,
		   PtrList<PtAttrInst>* ai_list)
{
  PtStmt* stmt = mFactory.new_Repeat(fr, expr, body);
  reg_attrinst(stmt, ai_list);
  return stmt;
}

// @brief while 文の生成
// @param[in] fr ファイル位置の情報
// @param[in] cond 繰り返し条件式
// @param[in] body 本体のステートメント
// @return 生成された while 文
inline
PtStmt*
Parser::new_While(const FileRegion& fr,
		  PtExpr* cond,
		  PtStmt* body,
		  PtrList<PtAttrInst>* ai_list)
{
  PtStmt* stmt = mFactory.new_While(fr, cond, body);
  reg_attrinst(stmt, ai_list);
  return stmt;
}

// @brief for 文の生成
// @param[in] fr ファイル位置の情報
// @param[in] init 初期化文
// @param[in] cond 繰り返し条件式
// @param[in] next 増加文
// @param[in] body 本体のステートメント
// @return 生成された for 文
inline
PtStmt*
Parser::new_For(const FileRegion& fr,
		PtStmt* init,
		PtExpr* cond,
		PtStmt* next,
		PtStmt* body,
		PtrList<PtAttrInst>* ai_list)
{
  PtStmt* stmt = mFactory.new_For(fr, init, cond, next, body);
  reg_attrinst(stmt, ai_list);
  return stmt;
}

// @brief procedural assign 文の生成
// @param[in] fr ファイル位置の情報
// @param[in] lhs 左辺式
// @param[in] rhs 右辺式
// @return 生成された procedural assign 文
inline
PtStmt*
Parser::new_PcAssign(const FileRegion& fr,
		     PtExpr* lhs,
		     PtExpr* rhs,
		     PtrList<PtAttrInst>* ai_list)
{
  PtStmt* stmt = mFactory.new_PcAssign(fr, lhs, rhs);
  reg_attrinst(stmt, ai_list);
  return stmt;
}
  
// @brief deassign 文の生成
// @param[in] fr ファイル位置の情報
// @param[in] lhs 左辺式
// @return 生成された deassign 文
inline
PtStmt*
Parser::new_Deassign(const FileRegion& fr,
		     PtExpr* lhs,
		     PtrList<PtAttrInst>* ai_list)
{
  PtStmt* stmt = mFactory.new_Deassign(fr, lhs);
  reg_attrinst(stmt, ai_list);
  return stmt;
}

// @brief force 文の生成
// @param[in] fr ファイル位置の情報
// @param[in] lhs 左辺式
// @param[in] rhs 右辺式
// @return 生成された force 文
inline
PtStmt*
Parser::new_Force(const FileRegion& fr,
		  PtExpr* lhs,
		  PtExpr* rhs,
		  PtrList<PtAttrInst>* ai_list)
{
  PtStmt* stmt = mFactory.new_Force(fr, lhs, rhs);
  reg_attrinst(stmt, ai_list);
  return stmt;
}

// @brief release 文の生成
// @param[in] fr ファイル位置の情報
// @param[in] lhs 左辺式
// @return 生成された release 文
inline
PtStmt*
Parser::new_Release(const FileRegion& fr,
		    PtExpr* lhs,
		    PtrList<PtAttrInst>* ai_list)
{
  PtStmt* stmt = mFactory.new_Release(fr, lhs);
  reg_attrinst(stmt, ai_list);
  return stmt;
}

// @brief parallel block の生成
// @param[in] fr ファイル位置の情報
// @param[in] stmt_list 本体のステートメントのリスト
// @return 生成された parallel block
inline
PtStmt*
Parser::new_ParBlock(const FileRegion& fr,
		     PtrList<PtStmt>* stmt_list,
		     PtrList<PtAttrInst>* ai_list)
{
  PtStmt* stmt = mFactory.new_ParBlock(fr, to_array(stmt_list));
  reg_attrinst(stmt, ai_list);
  return stmt;
}

// @brief 名前付き parallel block の生成
// @param[in] fr ファイル位置の情報
// @param[in] name ブロック名
// @param[in] stmt_list 本体のステートメントのリスト
// @return 生成された parallel block
inline
PtStmt*
Parser::new_NamedParBlock(const FileRegion& fr,
			  const char* name,
			  PtrList<PtStmt>* stmt_list,
			  PtrList<PtAttrInst>* ai_list)
{
  PtStmt* stmt = mFactory.new_NamedParBlock(fr, name,
					    get_decl_array(),
					    to_array(stmt_list));
  reg_attrinst(stmt, ai_list);

  return stmt;
}

// @brief sequential block の生成
// @param[in] fr ファイル位置の情報
// @param[in] stmt_list 本体のステートメントのリスト
// @return 生成された sequential block
inline
PtStmt*
Parser::new_SeqBlock(const FileRegion& fr,
		     PtrList<PtStmt>* stmt_list,
		     PtrList<PtAttrInst>* ai_list)
{
  PtStmt* stmt = mFactory.new_SeqBlock(fr, to_array(stmt_list));
  reg_attrinst(stmt, ai_list);
  return stmt;
}

// @brief 名前付き sequential block の生成
// @param[in] fr ファイル位置の情報
// @param[in] name ブロック名
// @param[in] stmt_list 本体のステートメントのリスト
// @return 生成された sequential block
inline
PtStmt*
Parser::new_NamedSeqBlock(const FileRegion& fr,
			  const char* name,
			  PtrList<PtStmt>* stmt_list,
			  PtrList<PtAttrInst>* ai_list)
{
  PtStmt* stmt = mFactory.new_NamedSeqBlock(fr, name,
					    get_decl_array(),
					    to_array(stmt_list));
  reg_attrinst(stmt, ai_list);

  return stmt;
}

END_NAMESPACE_YM_VERILOG

#endif // LIBYM_VERILOG_PARSER_MAIN_PARSER_STMT_H
