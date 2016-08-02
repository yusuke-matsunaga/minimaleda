#ifndef LIBYM_VERILOG_PARSER_MAIN_PARSER_ITEM_H
#define LIBYM_VERILOG_PARSER_MAIN_PARSER_ITEM_H

/// @file libym_verilog/parser/main/Parser_item.h
/// @brief Parser の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: Parser_item.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "Parser.h"

#include "PtiFactory.h"
#include "ym_verilog/pt/PtItem.h"
#include "ym_verilog/pt/PtStmt.h"
#include "ym_verilog/pt/PtExpr.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// defparam の生成
//////////////////////////////////////////////////////////////////////

// @brief defparam 文のヘッダの生成
// @param[in] fr ファイル位置の情報
inline
void
Parser::new_DefParamH(const FileRegion& fr,
		      PtrList<PtAttrInst>* ai_list)
{
  PtItem* head = mFactory.new_DefParamH(fr, get_defparam_array());
  reg_attrinst(head, ai_list);
  add_item(head);
}

// @brief defparam 文の要素の生成
// @param[in] fr ファイル位置の情報
// @param[in] name 名前
// @param[in] value 値を表す式
inline
void
Parser::new_DefParam(const FileRegion& fr,
		     const char* name,
		     PtExpr* value)
{
  add_defparam( mFactory.new_DefParam(fr, name, value) );
}

// @brief defparam 文の要素の生成 (階層つき識別子)
// @param[in] fr ファイル位置の情報
// @param[in] hname 階層名
// @param[in] value 値を表す式
inline
void
Parser::new_DefParam(const FileRegion& fr,
		     PuHierName* hname,
		     PtExpr* value)
{
  PtNameBranchArray nb_array;
  const char* tail_name = extract_HierName(hname, nb_array);
  add_defparam( mFactory.new_DefParam(fr, nb_array, tail_name, value) );
}

// @brief defparam リストに要素を追加する．
inline
void
Parser::add_defparam(PtDefParam* defparam)
{
  mDefParamList.push_back(defparam);
}

// @brief defparam リストを配列に変換する．
inline
PtDefParamArray
Parser::get_defparam_array()
{
  return mDefParamList.to_array(mAlloc);
}


//////////////////////////////////////////////////////////////////////
// continuous assign の生成
//////////////////////////////////////////////////////////////////////

// @brief continuous assign 文のヘッダの生成
// @param[in] fr ファイル位置の情報
inline
void
Parser::new_ContAssignH(const FileRegion& fr,
			PtrList<PtAttrInst>* ai_list)
{
  PtItem* head = mFactory.new_ContAssignH(fr, get_contassign_array());
  reg_attrinst(head, ai_list);
  add_item(head);
}

// @brief continuous assign 文のヘッダの生成 (strengthつき)
// @param[in] fr ファイル位置の情報
// @param[in] strength 信号強度
inline
void
Parser::new_ContAssignH(const FileRegion& fr,
			PtStrength* strength,
			PtrList<PtAttrInst>* ai_list)
{
  PtItem* head = mFactory.new_ContAssignH(fr, strength,
					  get_contassign_array());
  reg_attrinst(head, ai_list);
  add_item(head);
}

// @brief continuous assign 文のヘッダの生成 (遅延付き)
// @param[in] fr ファイル位置の情報
// @param[in] delay 遅延値
inline
void
Parser::new_ContAssignH(const FileRegion& fr,
			PtDelay* delay,
			PtrList<PtAttrInst>* ai_list)
{
  PtItem* head = mFactory.new_ContAssignH(fr, delay,
					  get_contassign_array());
  reg_attrinst(head, ai_list);
  add_item(head);
}

// @brief continuous assign 文のヘッダの生成 (strength, 遅延付き)
// @param[in] fr ファイル位置の情報
// @param[in] strength 信号強度
// @param[in] delay 遅延値
// @param[in] elem_array 要素のリスト
inline
void
Parser::new_ContAssignH(const FileRegion& fr,
			PtStrength* strength,
			PtDelay* delay,
			PtrList<PtAttrInst>* ai_list)
{
  PtItem* head = mFactory.new_ContAssignH(fr, strength, delay,
					  get_contassign_array());
  reg_attrinst(head, ai_list);
  add_item(head);
}

// @brief continuous assign 文の生成
// @param[in] fr ファイル位置の情報
// @param[in] lhs 左辺式
// @param[in] rhs 右辺式
inline
void
Parser::new_ContAssign(const FileRegion& fr,
		       PtExpr* lhs,
		       PtExpr* rhs)
{
  add_contassign( mFactory.new_ContAssign(fr, lhs, rhs) );
}

// @brief contassign リストに要素を追加する．
inline
void
Parser::add_contassign(PtContAssign* contassign)
{
  mContAssignList.push_back(contassign);
}

// @brief contassign リストを配列に変換する．
inline
PtContAssignArray
Parser::get_contassign_array()
{
  return mContAssignList.to_array(mAlloc);
}


//////////////////////////////////////////////////////////////////////
// process の生成
//////////////////////////////////////////////////////////////////////

// @brief initial 文の生成
// @param[in] fr ファイル位置の情報
// @param[in] body 本体のステートメント
inline
void
Parser::new_Initial(const FileRegion& fr,
		    PtStmt* body,
		    PtrList<PtAttrInst>* ai_list)
{
  PtItem* item = mFactory.new_Initial(fr, body);
  reg_attrinst(item, ai_list);
  add_item(item);
}

// @brief always 文の生成
// @param[in] fr ファイル位置の情報
// @param[in] body 本体のステートメント
inline
void
Parser::new_Always(const FileRegion& fr,
		   PtStmt* body,
		   PtrList<PtAttrInst>* ai_list)
{
  PtItem* item = mFactory.new_Always(fr, body);
  reg_attrinst(item, ai_list);
  add_item(item);
}


//////////////////////////////////////////////////////////////////////
// task/function の生成
//////////////////////////////////////////////////////////////////////

// @brief task 文の生成
// @param[in] fr ファイル位置の情報
// @param[in] name task 名
// @param[in] automatic automatic task の時に true となるフラグ
// @param[in] stmt 本体のステートメント
inline
void
Parser::new_Task(const FileRegion& fr,
		 const char* name,
		 bool automatic,
		 PtStmt* stmt,
		 PtrList<PtAttrInst>* ai_list)
{
  PtItem* item = mFactory.new_Task(fr, name, automatic,
				   get_io_array(),
				   get_param_array(),
				   get_localparam_array(),
				   get_decl_array(),
				   stmt);
  reg_attrinst(item, ai_list);
  add_item(item);
  end_tf();
}
  
// @brief 1ビット型 function 文の生成
// @param[in] fr ファイル位置の情報
// @param[in] name function 名
// @param[in] automatic automatic task の時に true となるフラグ
// @param[in] sign signed 属性がついていたら true となるフラグ
// @param[in] stmt 本体のステートメント
inline
void
Parser::new_Function(const FileRegion& fr,
		     const char* name,
		     bool automatic,
		     bool sign,
		     PtStmt* stmt,
		     PtrList<PtAttrInst>* ai_list)
{
  // 関数内で使用できないステートメントが含まれていないかチェック
  if ( check_function_statement(stmt) ) {
    PtItem* item = mFactory.new_Function(fr, name, automatic,
					 sign,
					 get_io_array(),
					 get_param_array(),
					 get_localparam_array(),
					 get_decl_array(),
					 stmt);
    reg_attrinst(item, ai_list);
    add_item(item);
  }
  end_tf();
}
  
// @brief 範囲指定型 function 文の生成
// @param[in] fr ファイル位置の情報
// @param[in] name function 名
// @param[in] automatic automatic task の時に true となるフラグ
// @param[in] sign 符号つきの時に true となるフラグ
// @param[in] left 範囲の左側の式
// @param[in] right 範囲の右側の式
// @param[in] stmt 本体のステートメント
inline
void
Parser::new_SizedFunc(const FileRegion& fr,
		      const char* name,
		      bool automatic,
		      bool sign,
		      PtExpr* left,
		      PtExpr* right,
		      PtStmt* stmt,
		      PtrList<PtAttrInst>* ai_list)
{
  // 関数内で使用できないステートメントが含まれていないかチェック
  if ( check_function_statement(stmt) ) {
    PtItem* item = mFactory.new_SizedFunc(fr, name, automatic,
					  sign, left, right,
					  get_io_array(),
					  get_param_array(),
					  get_localparam_array(),
					  get_decl_array(),
					  stmt);
    reg_attrinst(item, ai_list);
    add_item(item);
  }
  end_tf();
}
  
// @brief 組み込み型 function 文の生成
// @param[in] fr ファイル位置の情報
// @param[in] name function 名
// @param[in] automatic automatic task の時に true となるフラグ
// @param[in] sign signed 属性がついていたら true となるフラグ
// @param[in] func_type 関数の戻値の型
// @param[in] stmt 本体のステートメント
inline
void
Parser::new_TypedFunc(const FileRegion& fr,
		      const char* name,
		      bool automatic,
		      bool sign,
		      tVpiVarType func_type,
		      PtStmt* stmt,
		      PtrList<PtAttrInst>* ai_list)
{
  // 関数内で使用できないステートメントが含まれていないかチェック
  if ( check_function_statement(stmt) ) {
    PtItem* item = mFactory.new_TypedFunc(fr, name, automatic,
					  sign, func_type,
					  get_io_array(),
					  get_param_array(),
					  get_localparam_array(),
					  get_decl_array(),
					  stmt);
    reg_attrinst(item, ai_list);
    add_item(item);
  }
  end_tf();
}


//////////////////////////////////////////////////////////////////////
// gate/module/UDP instance の生成
//////////////////////////////////////////////////////////////////////

// @brief gate instance 文のヘッダの生成
// @param[in] fr ファイル位置の情報
// @param[in] type primitive の型
inline
void
Parser::new_GateH(const FileRegion& fr,
		  tVpiPrimType type,
		  PtrList<PtAttrInst>* ai_list)
{
  PtItem* item = mFactory.new_GateH(fr, type, get_inst_array());
  reg_attrinst(item, ai_list);
  add_item(item);
}
  
// @brief gate instance 文のヘッダの生成 (strength付き)
// @param[in] fr ファイル位置の情報
// @param[in] type primitive の型
// @param[in] strength 信号強度
inline
void
Parser::new_GateH(const FileRegion& fr,
		  tVpiPrimType type,
		  PtStrength* strength,
		  PtrList<PtAttrInst>* ai_list)
{
  PtItem* item = mFactory.new_GateH(fr, type,
				    strength,
				    get_inst_array());
  reg_attrinst(item, ai_list);
  add_item(item);
}
  
// @brief gate instance 文のヘッダの生成 (遅延付き)
// @param[in] fr ファイル位置の情報
// @param[in] type primitive の型
// @param[in] delay 遅延値
inline
void
Parser::new_GateH(const FileRegion& fr,
		  tVpiPrimType type,
		  PtDelay* delay,
		  PtrList<PtAttrInst>* ai_list)
{
  PtItem* item = mFactory.new_GateH(fr, type,
				    delay,
				    get_inst_array());
  reg_attrinst(item, ai_list);
  add_item(item);
}
  
// @brief gate instance 文のヘッダの生成 (strength, 遅延付き)
// @param[in] fr ファイル位置の情報
// @param[in] type primitive の型
// @param[in] strength 信号強度
// @param[in] delay 遅延値
inline
void
Parser::new_GateH(const FileRegion& fr,
		  tVpiPrimType type,
		  PtStrength* strength,
		  PtDelay* delay,
		  PtrList<PtAttrInst>* ai_list)
{
  PtItem* item = mFactory.new_GateH(fr, type,
				    strength, delay,
				    get_inst_array());
  reg_attrinst(item, ai_list);
  add_item(item);
}

// @brief module instance/UDP instance 文のヘッダの生成
// @param[in] fr ファイル位置の情報
// @param[in] def_name 定義名
inline
void
Parser::new_MuH(const FileRegion& fr,
		const char* def_name,
		PtrList<PtAttrInst>* ai_list)
{
  PtItem* item = mFactory.new_MuH(fr, def_name,
				  get_inst_array());
  reg_defname(def_name);
  reg_attrinst(item, ai_list);
  add_item(item);
}

// @brief module instance/UDP instance 文のヘッダの生成 (strength付き)
// @param[in] fr ファイル位置の情報
// @param[in] def_name 定義名
// @param[in] strength 信号強度
inline
void
Parser::new_MuH(const FileRegion& fr,
		const char* def_name,
		PtStrength* strength,
		PtrList<PtAttrInst>* ai_list)
{
  PtItem* item = mFactory.new_MuH(fr, def_name,
				  strength,
				  get_inst_array());
  reg_defname(def_name);
  reg_attrinst(item, ai_list);
  add_item(item);
}

// @brief module instance/UDP instance 文のヘッダの生成 (遅延付き)
// @param[in] fr ファイル位置の情報
// @param[in] def_name 定義名
// @param[in] delay 遅延値
inline
void
Parser::new_MuH(const FileRegion& fr,
		const char* def_name,
		PtDelay* delay,
		PtrList<PtAttrInst>* ai_list)
{
  PtItem* item = mFactory.new_MuH(fr, def_name,
				  delay,
				  get_inst_array());
  reg_defname(def_name);
  reg_attrinst(item, ai_list);
  add_item(item);
}

// @brief module instance/UDP instance 文のヘッダの生成 (strength, 遅延付き)
// @param[in] fr ファイル位置の情報
// @param[in] def_name 定義名
// @param[in] strength 信号強度
// @param[in] delay 遅延値
inline
void
Parser::new_MuH(const FileRegion& fr,
		const char* def_name,
		PtStrength* strength,
		PtDelay* delay,
		PtrList<PtAttrInst>* ai_list)
{
  PtItem* item = mFactory.new_MuH(fr, def_name,
				  strength, delay,
				  get_inst_array());
  reg_defname(def_name);
  reg_attrinst(item, ai_list);
  add_item(item);
}

// @brief module instance/UDP instance 文のヘッダの生成
// @param[in] fr ファイル位置の情報
// @param[in] def_name 定義名
// @param[in] con_array ポート割り当てリスト
inline
void
Parser::new_MuH(const FileRegion& fr,
		const char* def_name,
		PtrList<PtConnection>* con_list,
		PtrList<PtAttrInst>* ai_list)
{
  PtItem* item = mFactory.new_MuH(fr, def_name,
				  to_array(con_list),
				  get_inst_array());
  reg_defname(def_name);
  reg_attrinst(item, ai_list);
  add_item(item);
}

// @brief module instance/UDP/gate instance の要素の生成
// @param[in] fr ファイル位置の情報
// @param[in] con_list ポート割り当ての配列
inline
void
Parser::new_Inst(const FileRegion& fr,
		 PtrList<PtConnection>* con_list)
{
  add_inst( mFactory.new_Inst(fr, to_array(con_list)) );
}

// @brief module instance/UDP/gate instance の要素の生成
// @param[in] fr ファイル位置の情報
// @param[in] expr1 ポート割り当て
inline
void
Parser::new_Inst(const FileRegion& fr,
		 PtExpr* expr1)
{
  add_inst( mFactory.new_Inst(fr, expr1) );
}

// @brief module instance/UDP/gate instance の要素の生成
// @param[in] fr ファイル位置の情報
// @param[in] expr1, expr2 ポート割り当て
inline
void
Parser::new_Inst(const FileRegion& fr,
		 PtExpr* expr1,
		 PtExpr* expr2)
{
  add_inst( mFactory.new_Inst(fr, expr1, expr2) );
}

// @brief module instance/UDP/gate instance の要素の生成
// @param[in] fr ファイル位置の情報
// @param[in] expr1, expr2, expr3 ポート割り当て
inline
void
Parser::new_Inst(const FileRegion& fr,
		 PtExpr* expr1,
		 PtExpr* expr2,
		 PtExpr* expr3)
{
  add_inst( mFactory.new_Inst(fr, expr1, expr2, expr3) );
}

// @brief module instance/UDP/gate instance の要素の生成
// @param[in] fr ファイル位置の情報
// @param[in] expr1, expr2, expr3, expr4 ポート割り当て
inline
void
Parser::new_Inst(const FileRegion& fr,
		 PtExpr* expr1,
		 PtExpr* expr2,
		 PtExpr* expr3,
		 PtExpr* expr4)
{
  add_inst( mFactory.new_Inst(fr, expr1, expr2, expr3, expr4) );
}

// @brief 名前付き module instance/UDP/gate instance の要素の生成
// @param[in] fr ファイル位置の情報
// @param[in] name 名前
// @param[in] con_list ポート割り当ての配列
inline
void
Parser::new_InstN(const FileRegion& fr,
		  const char* name,
		  PtrList<PtConnection>* con_list)
{
  add_inst( mFactory.new_InstN(fr, name, to_array(con_list)) );
}

// @brief 名前付き module instance/UDP/gate instance の要素の生成
// @param[in] fr ファイル位置の情報
// @param[in] name 名前
// @param[in] expr1 ポート割り当て
inline
void
Parser::new_InstN(const FileRegion& fr,
		  const char* name,
		  PtExpr* expr1)
{
  add_inst( mFactory.new_InstN(fr, name, expr1) );
}

// @brief 名前付き module instance/UDP/gate instance の要素の生成
// @param[in] fr ファイル位置の情報
// @param[in] name 名前
// @param[in] expr1, expr2 ポート割り当て
inline
void
Parser::new_InstN(const FileRegion& fr,
		  const char* name,
		  PtExpr* expr1,
		  PtExpr* expr2)
{
  add_inst( mFactory.new_InstN(fr, name, expr1, expr2) );
}

// @brief 名前付き module instance/UDP/gate instance の要素の生成
// @param[in] fr ファイル位置の情報
// @param[in] name 名前
// @param[in] expr1, expr2, expr3 ポート割り当て
inline
void
Parser::new_InstN(const FileRegion& fr,
		  const char* name,
		  PtExpr* expr1,
		  PtExpr* expr2,
		  PtExpr* expr3)
{
  add_inst( mFactory.new_InstN(fr, name, expr1, expr2, expr3) );
}

// @brief 名前付き module instance/UDP/gate instance の要素の生成
// @param[in] fr ファイル位置の情報
// @param[in] name 名前
// @param[in] expr1, expr2, expr3, expr4 ポート割り当て
inline
void
Parser::new_InstN(const FileRegion& fr,
		  const char* name,
		  PtExpr* expr1,
		  PtExpr* expr2,
		  PtExpr* expr3,
		  PtExpr* expr4)
{
  add_inst( mFactory.new_InstN(fr, name, expr1, expr2, expr3, expr4) );
}

// @brief 名前と範囲付き module instance/UDP/gate instance の要素の生成
// @param[in] fr ファイル位置の情報
// @param[in] name 名前
// @param[in] left 範囲の左側の式
// @param[in] right 範囲の右側の式
// @param[in] con_list ポート割り当ての配列
inline
void
Parser::new_InstV(const FileRegion& fr,
		  const char* name,
		  PtExpr* left,
		  PtExpr* right,
		  PtrList<PtConnection>* con_list)
{
  add_inst( mFactory.new_InstV(fr, name, left, right, to_array(con_list)) );
}

// @brief 名前と範囲付き module instance/UDP/gate instance の要素の生成
// @param[in] fr ファイル位置の情報
// @param[in] name 名前
// @param[in] left 範囲の左側の式
// @param[in] right 範囲の右側の式
// @param[in] expr1 ポート割り当て
inline
void
Parser::new_InstV(const FileRegion& fr,
		  const char* name,
		  PtExpr* left,
		  PtExpr* right,
		  PtExpr* expr1)
{
  add_inst( mFactory.new_InstV(fr, name, left, right, expr1) );
}

// @brief 名前と範囲付き module instance/UDP/gate instance の要素の生成
// @param[in] fr ファイル位置の情報
// @param[in] name 名前
// @param[in] left 範囲の左側の式
// @param[in] right 範囲の右側の式
// @param[in] expr1, expr2 ポート割り当て
inline
void
Parser::new_InstV(const FileRegion& fr,
		  const char* name,
		  PtExpr* left,
		  PtExpr* right,
		  PtExpr* expr1,
		  PtExpr* expr2)
{
  add_inst( mFactory.new_InstV(fr, name, left, right, expr1, expr2) );
}

// @brief 名前と範囲付き module instance/UDP/gate instance の要素の生成
// @param[in] fr ファイル位置の情報
// @param[in] name 名前
// @param[in] left 範囲の左側の式
// @param[in] right 範囲の右側の式
// @param[in] expr1, expr2, expr3 ポート割り当て
inline
void
Parser::new_InstV(const FileRegion& fr,
		  const char* name,
		  PtExpr* left,
		  PtExpr* right,
		  PtExpr* expr1,
		  PtExpr* expr2,
		  PtExpr* expr3)
{
  add_inst( mFactory.new_InstV(fr, name, left, right, expr1, expr2, expr3) );
}

// @brief 名前と範囲付き module instance/UDP/gate instance の要素の生成
// @param[in] fr ファイル位置の情報
// @param[in] name 名前
// @param[in] left 範囲の左側の式
// @param[in] right 範囲の右側の式
// @param[in] expr1, expr2, expr3, expr4 ポート割り当て
inline
void
Parser::new_InstV(const FileRegion& fr,
		  const char* name,
		  PtExpr* left,
		  PtExpr* right,
		  PtExpr* expr1,
		  PtExpr* expr2,
		  PtExpr* expr3,
		  PtExpr* expr4)
{
  add_inst( mFactory.new_InstV(fr, name, left, right, expr1, expr2, expr3, expr4) );
}

// @brief instance リストに要素を追加する．
inline
void
Parser::add_inst(PtInst* inst)
{
  mInstList.push_back(inst);
}

// @brief instance リストを配列に変換する．
inline
PtInstArray
Parser::get_inst_array()
{
  return mInstList.to_array(mAlloc);
}


//////////////////////////////////////////////////////////////////////
// generate 文の生成
//////////////////////////////////////////////////////////////////////

// @brief generate 文の生成
// @param[in] fr ファイル位置の情報
inline
void
Parser::new_Generate(const FileRegion& fr,
		     PtrList<PtAttrInst>* ai_list)
{
  PtItem* item = mFactory.new_Generate(fr, get_decl_array(), get_item_array());
  reg_attrinst(item, ai_list);
  add_item(item);
  end_generate();
}
  
// @brief generate block 文の生成
// @param[in] fr ファイル位置の情報
inline
void
Parser::new_GenBlock(const FileRegion& fr)
{
  PtItem* item = mFactory.new_GenBlock(fr, get_decl_array(), get_item_array());
  add_item(item);
  end_generate();
}

// @brief 名前付き generate block 文の生成
// @param[in] fr ファイル位置の情報
// @param[in] name 名前
inline
void
Parser::new_GenBlock(const FileRegion& fr,
		     const char* name)
{
  PtItem* item = mFactory.new_GenBlock(fr, name,
				       get_decl_array(), get_item_array());
  add_item(item);
  end_generate();
}

// @brief generate if 文の生成
// @param[in] fr ファイル位置の情報
// @param[in] cond 条件を表す式
inline
void
Parser::new_GenIf(const FileRegion& fr,
		  PtExpr* cond)
{
  PtItem* item = mFactory.new_GenIf(fr, cond,
				    get_then_decl_array(),
				    get_then_item_array(),
				    PtDeclHeadArray(),
				    PtItemArray());
  add_item(item);
  end_genif();
}

// @brief generate if 文の生成
// @param[in] fr ファイル位置の情報
// @param[in] cond 条件を表す式
inline
void
Parser::new_GenIfElse(const FileRegion& fr,
		      PtExpr* cond)
{
  PtItem* item = mFactory.new_GenIf(fr, cond,
				    get_then_decl_array(),
				    get_then_item_array(),
				    get_else_decl_array(),
				    get_else_item_array());
  add_item(item);
  end_genif();
}

// @brief generate-if 文の then 節の宣言リストを配列に変換する．
inline
PtDeclHeadArray
Parser::get_then_decl_array()
{
  flush_declitem_list(*mCurDeclHeadList);
  return mCurThenDeclHeadList->to_array(mAlloc);
}

// @brief generate-if 文の else 節の宣言リストを配列に変換する．
inline
PtDeclHeadArray
Parser::get_else_decl_array()
{
  flush_declitem_list(*mCurDeclHeadList);
  return mCurElseDeclHeadList->to_array(mAlloc);
}

// @brief generate-if 文の then 節の item リストを配列に変換する．
inline
PtItemArray
Parser::get_then_item_array()
{
  return mCurThenItemList->to_array(mAlloc);
}

// @brief generate-if 文の else 節の item リストを配列に変換する．
inline
PtItemArray
Parser::get_else_item_array()
{
  return mCurElseItemList->to_array(mAlloc);
}

// @brief generate case 文の生成
// @param[in] fr ファイル位置の情報
// @param[in] expr 選択式
// @param[in] item_list generate case item のリスト
inline
void
Parser::new_GenCase(const FileRegion& fr,
		    PtExpr* expr,
		    PtrList<PtGenCaseItem>* item_list)
{
  add_item( mFactory.new_GenCase(fr, expr, to_array(item_list)) );
}

// @brief generate case の要素の生成
// @param[in] fr ファイル位置の情報
// @param[in] label_list 比較式のリスト
// @return 生成された generate case item
inline
PtGenCaseItem*
Parser::new_GenCaseItem(const FileRegion& fr,
			PtrList<PtExpr>* label_list)
{
  PtGenCaseItem* item = mFactory.new_GenCaseItem(fr,
						 to_array(label_list),
						 get_decl_array(),
						 get_item_array());
  end_generate();
  return item;
}

// @brief generate for 文の生成
// @param[in] fr ファイル位置の情報
// @param[in] loop_var ループ変数
// @param[in] init_expr 初期化式
// @param[in] cond ループ条件式
// @param[in] inc_var 増加式の左辺の変数
// @param[in] inc_expr 増加式
// @param[in] block_name ブロック名
// @param[in] decl_array 宣言のリスト
// @param[in] item_array 要素のリスト
inline
void
Parser::new_GenFor(const FileRegion& fr,
		   const char* loop_var,
		   PtExpr* init_expr,
		   PtExpr* cond,
		   const char* inc_var,
		   PtExpr* inc_expr,
		   const char* block_name)
{
  if ( strcmp(loop_var, inc_var) != 0 ) {
    end_generate();
    ostringstream buf;
    buf << "Lhs of the increment statement ("
	<< inc_var
	<< ") does not match with Lhs of the initial statement ("
	<< loop_var
	<< ")";
    put_msg(__FILE__, __LINE__,
	    fr,
	    kMsgError,
	    "PARSER",
	    buf.str());
  }

  PtItem* item = mFactory.new_GenFor(fr, loop_var,
				     init_expr, cond, inc_expr,
				     block_name,
				     get_decl_array(),
				     get_item_array());
  add_item(item);
  end_generate();
}


//////////////////////////////////////////////////////////////////////
// spec item の生成
//////////////////////////////////////////////////////////////////////

// @brief specify block item の生成
// @param[in] fr ファイル位置の情報
// @param[in] id specify block item の種類
// @param[in] terminal_list 端子のリスト
inline
void
Parser::new_SpecItem(const FileRegion& fr,
		     tVpiSpecItemType id,
		     PtrList<PtExpr>* terminal_list)
{
  add_item( mFactory.new_SpecItem(fr, id, to_array(terminal_list)) );
}

// @brief path 仕様を生成する．
// @param[in] fr ファイル位置の情報
// @param[in] id spec path の種類
// @param[in] expr 条件式
// @param[in] path_decl パス記述
inline
void
Parser::new_SpecPath(const FileRegion& fr,
		     tVpiSpecPathType id,
		     PtExpr* expr,
		     PtPathDecl* path_decl)
{
  add_item( mFactory.new_SpecPath(fr, id, expr, path_decl) );
}

// @brief パス記述の生成
// @param[in] fr ファイル位置の情報
// @param[in] edge
// @param[in] input_list
// @param[in] input_pol
// @param[in] op
// @param[in] output_list
// @param[in] output_pol
// @param[in] expr
// @param[in] path_delay
inline
PtPathDecl*
Parser::new_PathDecl(const FileRegion& fr,
		     int edge,
		     PtrList<PtExpr>* input_list,
		     int input_pol,
		     int op,
		     PtrList<PtExpr>* output_list,
		     int output_pol,
		     PtExpr* expr,
		     PtPathDelay* path_delay)
{
  return mFactory.new_PathDecl(fr, edge,
			       to_array(input_list), input_pol,
			       op,
			       to_array(output_list), output_pol,
			       expr, path_delay);
}

// @brief パス記述の生成
// @param[in] fr ファイル位置の情報
// @param[in] edge
// @param[in] input_list
// @param[in] input_pol
// @param[in] op
// @param[in] output
// @param[in] output_pol
// @param[in] expr
// @param[in] path_delay
inline
PtPathDecl*
Parser::new_PathDecl(const FileRegion& fr,
		     int edge,
		     PtrList<PtExpr>* input_list,
		     int input_pol,
		     int op,
		     PtExpr* output,
		     int output_pol,
		     PtExpr* expr,
		     PtPathDelay* path_delay)
{
  return mFactory.new_PathDecl(fr, edge,
			       to_array(input_list), input_pol,
			       op,
			       output, output_pol,
			       expr, path_delay);
}
  
// @brief path delay value の生成 (値が1個)
// @param[in] fr ファイル位置の情報
// @param[in] value 値
// @return 生成された path delay value
inline
PtPathDelay*
Parser::new_PathDelay(const FileRegion& fr,
		      PtExpr* value)
{
  return mFactory.new_PathDelay(fr, value);
}

// @brief path delay value の生成 (値が2個)
// @param[in] fr ファイル位置の情報
// @param[in] value1 値1
// @param[in] value2 値2
// @return 生成された path delay value
inline
PtPathDelay*
Parser::new_PathDelay(const FileRegion& fr,
		      PtExpr* value1,
		      PtExpr* value2)
{
  return mFactory.new_PathDelay(fr, value1, value2);
}

// @brief path delay value の生成 (値が3個)
// @param[in] fr ファイル位置の情報
// @param[in] value1 値1
// @param[in] value2 値2
// @param[in] value3 値3
// @return 生成された path delay value
PtPathDelay*
Parser::new_PathDelay(const FileRegion& fr,
		      PtExpr* value1,
		      PtExpr* value2,
		      PtExpr* value3)
{
  return mFactory.new_PathDelay(fr, value1, value2, value3);
}

// @brief path delay value の生成 (値が6個)
// @param[in] fr ファイル位置の情報
// @param[in] value1 値1
// @param[in] value2 値2
// @param[in] value3 値3
// @param[in] value4 値4
// @param[in] value5 値5
// @param[in] value6 値6
// @return 生成された path delay value
PtPathDelay*
Parser::new_PathDelay(const FileRegion& fr,
		      PtExpr* value1,
		      PtExpr* value2,
		      PtExpr* value3,
		      PtExpr* value4,
		      PtExpr* value5,
		      PtExpr* value6)
{
  return mFactory.new_PathDelay(fr,
				value1, value2, value3,
				value4, value5, value6);
}

// @brief path delay value の生成 (値が12個)
// @param[in] fr ファイル位置の情報
// @param[in] value1 値1
// @param[in] value2 値2
// @param[in] value3 値3
// @param[in] value4 値4
// @param[in] value5 値5
// @param[in] value6 値6
// @param[in] value7 値7
// @param[in] value8 値8
// @param[in] value9 値9
// @param[in] value10 値10
// @param[in] value11 値11
// @param[in] value12 値12
// @return 生成された path delay value
PtPathDelay*
Parser::new_PathDelay(const FileRegion& fr,
		      PtExpr* value1,
		      PtExpr* value2,
		      PtExpr* value3,
		      PtExpr* value4,
		      PtExpr* value5,
		      PtExpr* value6,
		      PtExpr* value7,
		      PtExpr* value8,
		      PtExpr* value9,
		      PtExpr* value10,
		      PtExpr* value11,
		      PtExpr* value12)
{
  return mFactory.new_PathDelay(fr,
				value1, value2, value3,
				value4, value5, value6,
				value7, value8, value9,
				value10, value11, value12);
}
  
// @brief item リストに要素を追加する．
inline
void
Parser::add_item(PtItem* item)
{
  mCurItemList->push_back(item);
}

END_NAMESPACE_YM_VERILOG

#endif // LIBYM_VERILOG_PARSER_MAIN_PARSER_ITEM_H
