
/// @file libym_verilog/elaborator/StmtGen_cond.cc
/// @brief ElbMgr の実装ファイル(conditional statement の実体化)
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: StmtGen_cond.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "StmtGen.h"
#include "ElbEnv.h"

#include "ym_verilog/pt/PtStmt.h"
#include "ym_verilog/pt/PtExpr.h"

#include "ElbStmt.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// 条件ステートメント関係の instantiate 関数
//////////////////////////////////////////////////////////////////////

// @brief if 文の生成を行う．
// @param[in] parent 親のスコープ
// @param[in] process 親のプロセス (or NULL)
// @param[in] env 生成時の環境
// @param[in] pt_stmt 対象のステートメント
ElbStmt*
StmtGen::instantiate_if(const VlNamedObj* parent,
			ElbProcess* process,
			const ElbEnv& env,
			const PtStmt* pt_stmt)
{
  ElbExpr* cond = instantiate_expr(parent, env, pt_stmt->expr());
  if ( !cond ) {
    // たぶんエラー
    return NULL;
  }
  
  const PtStmt* pt_then = pt_stmt->body();
  ElbStmt* then_stmt = instantiate_stmt(parent, process, env, pt_then);
  if ( !then_stmt ) {
    // たぶんエラー
    return NULL;
  }

  ElbStmt* else_stmt = NULL;
  const PtStmt* pt_else = pt_stmt->else_body();
  if ( pt_else ) {
    else_stmt = instantiate_stmt(parent, process, env, pt_else);
    if ( !else_stmt ) {
      // たぶんエラー
      return NULL;
    }
  }

  ElbStmt* stmt = factory().new_IfStmt(parent, process, pt_stmt,
				       cond, then_stmt, else_stmt);

  return stmt;
}

// @brief case 文の実体化を行う．
// @param[in] parent 親のスコープ
// @param[in] process 親のプロセス (or NULL)
// @param[in] env 生成時の環境
// @param[in] pt_stmt 対象のステートメント
ElbStmt*
StmtGen::instantiate_case(const VlNamedObj* parent,
			  ElbProcess* process,
			  const ElbEnv& env,
			  const PtStmt* pt_stmt)
{
  ElbExpr* cond = instantiate_expr(parent, env, pt_stmt->expr());
  
  if ( !cond ) {
    // たぶんエラー
    return NULL;
  }

  ElbStmt* stmt = factory().new_CaseStmt(parent, process, pt_stmt, cond);
  
  // case-item の生成
  for (ymuint i = 0; i < pt_stmt->caseitem_num(); ++ i) {
    const PtCaseItem* pt_item = pt_stmt->caseitem(i);
    ElbStmt* body = NULL;
    if ( pt_item->body() ) {
      body = instantiate_stmt(parent, process, env, pt_item->body());
      if ( !body ) {
	return NULL;
      }
    }
    
    // ラベルの生成と設定
    ymuint n = pt_item->label_num();
    ElbExpr** label_list = factory().new_ExprList(n);
    for (ymuint j = 0; j < n; ++ j) {
      const PtExpr* pt_expr = pt_item->label(j);
      ElbExpr* tmp = instantiate_expr(parent, env, pt_expr);
      if ( !tmp ) {
	// たぶんエラー
	return NULL;
      }
      label_list[j] = tmp;
    }
    
    stmt->set_caseitem(i, pt_item, label_list, body);
  }

  return stmt;
}

// @brief wait 文の実体化を行う．
// @param[in] parent 親のスコープ
// @param[in] process 親のプロセス (or NULL)
// @param[in] env 生成時の環境
// @param[in] pt_stmt 対象のステートメント
ElbStmt*
StmtGen::instantiate_wait(const VlNamedObj* parent,
			  ElbProcess* process,
			  const ElbEnv& env,
			  const PtStmt* pt_stmt)
{
  ElbExpr* cond = instantiate_expr(parent, env, pt_stmt->expr());
  ElbStmt* body = instantiate_stmt(parent, process, env,
				   pt_stmt->body());

  if ( !cond || !body ) {
    return NULL;
  }

  ElbStmt* stmt = factory().new_WaitStmt(parent, process, pt_stmt,
					 cond, body);

  return stmt;
}

// @brief forever 文のインスタンス化を行う．
// @param[in] parent 親のスコープ
// @param[in] process 親のプロセス (or NULL)
// @param[in] env 生成時の環境
// @param[in] pt_stmt 対象のステートメント
ElbStmt*
StmtGen::instantiate_forever(const VlNamedObj* parent,
			     ElbProcess* process,
			     const ElbEnv& env,
			     const PtStmt* pt_stmt)
{
  ElbStmt* body = instantiate_stmt(parent, process, env, pt_stmt->body());

  if ( !body ) {
    return NULL;
  }

  ElbStmt* stmt = factory().new_ForeverStmt(parent, process, pt_stmt,
					    body);

  return stmt;
}

// @brief repeat 文のインスタンス化を行う．
// @param[in] parent 親のスコープ
// @param[in] process 親のプロセス (or NULL)
// @param[in] env 生成時の環境
// @param[in] pt_stmt 対象のステートメント
ElbStmt*
StmtGen::instantiate_repeat(const VlNamedObj* parent,
			    ElbProcess* process,
			    const ElbEnv& env,
			    const PtStmt* pt_stmt)
{
  ElbExpr* expr = instantiate_expr(parent, env, pt_stmt->expr());
  
  ElbStmt* body = instantiate_stmt(parent, process, env, pt_stmt->body());
  
  if ( !expr || !body ) {
    return NULL;
  }
  
  ElbStmt* stmt = factory().new_RepeatStmt(parent, process, pt_stmt,
					   expr, body);
  
  return stmt;
}

// @brief while 文のインスタンス化を行う．
// @param[in] parent 親のスコープ
// @param[in] process 親のプロセス (or NULL)
// @param[in] env 生成時の環境
// @param[in] pt_stmt 対象のステートメント
ElbStmt*
StmtGen::instantiate_while(const VlNamedObj* parent,
			   ElbProcess* process,
			   const ElbEnv& env,
			   const PtStmt* pt_stmt)
{
  ElbExpr* cond = instantiate_expr(parent, env, pt_stmt->expr());
  
  ElbStmt* body = instantiate_stmt(parent, process, env, pt_stmt->body());

  if ( !cond || !body ) {
    return NULL;
  }
  
  ElbStmt* stmt = factory().new_WhileStmt(parent, process, pt_stmt,
					  cond, body);

  return stmt;
}

// @brief for 文のインスタンス化を行う．
// @param[in] parent 親のスコープ
// @param[in] process 親のプロセス (or NULL)
// @param[in] env 生成時の環境
// @param[in] pt_stmt 対象のステートメント
ElbStmt*
StmtGen::instantiate_for(const VlNamedObj* parent,
			 ElbProcess* process,
			 const ElbEnv& env,
			 const PtStmt* pt_stmt)
{
  ElbExpr* cond = instantiate_expr(parent, env, pt_stmt->expr());
  ElbStmt* init = instantiate_stmt(parent, process, env,
				   pt_stmt->init_stmt());
  ElbStmt* next = instantiate_stmt(parent, process, env,
				   pt_stmt->next_stmt());
  ElbStmt* body = instantiate_stmt(parent, process, env,
				   pt_stmt->body());

  if ( !cond || !init || !next || !body ) {
    return NULL;
  }
  
  ElbStmt* stmt = factory().new_ForStmt(parent, process, pt_stmt,
					cond, init, next, body);

  return stmt;
}

END_NAMESPACE_YM_VERILOG
