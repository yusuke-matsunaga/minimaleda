
/// @file libym_verilog/elaborator/StmtGen_block.cc
/// @brief ElbMgr の実装ファイル(block statement の実体化)
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: StmtGen_block.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "StmtGen.h"
#include "ElbEnv.h"

#include "ym_verilog/pt/PtStmt.h"


BEGIN_NAMESPACE_YM_VERILOG

BEGIN_NONAMESPACE

const int debug = 0;

END_NONAMESPACE

//////////////////////////////////////////////////////////////////////
// ブロックステートメント関係の instantiate 関数
//////////////////////////////////////////////////////////////////////
  
// @brief ブロックスコープ内の宣言要素の生成を行う．
void
StmtGen::phase2_namedblock(const VlNamedObj* parent,
			   PtDeclHeadArray pt_head_array)
{
  if ( debug ) {
    cout << endl
	 << "phase2_namedblock( "
	 << parent->full_name()
	 << " ["
	 << hex << reinterpret_cast<ympuint>(parent) << dec
	 << "] )" << endl;
  }
  
  instantiate_decl(parent, pt_head_array);
  
  if ( debug ) {
    cout << "phase2_namedblock end" << endl
	 << endl;
  }
}

// @brief parallel block のインスタンス化を行う．
// @param[in] parent 親のスコープ
// @param[in] process 親のプロセス (or NULL)
// @param[in] env 生成時の環境
// @param[in] pt_stmt 対象のステートメント
ElbStmt*
StmtGen::instantiate_parblock(const VlNamedObj* parent,
			      ElbProcess* process,
			      const ElbEnv& env,
			      const PtStmt* pt_stmt)
{
  PtStmtArray pt_stmt_array = pt_stmt->stmt_array();
  ymuint stmt_num = pt_stmt_array.size();
  ElbStmt** stmt_list = factory().new_StmtList(stmt_num);
  for (ymuint i = 0; i < stmt_num; ++ i) {
    const PtStmt* pt_stmt1 = pt_stmt_array[i];
    ElbStmt* stmt1 = instantiate_stmt(parent, process, env, pt_stmt1);
    if ( !stmt1 ) {
      return NULL;
    }
    stmt_list[i] = stmt1;
  }
  
  ElbStmt* stmt = factory().new_Fork(parent, process, pt_stmt, stmt_list);

  return stmt;
}

// @brief sequential block のインスタンス化を行う．
// @param[in] parent 親のスコープ
// @param[in] process 親のプロセス (or NULL)
// @param[in] env 生成時の環境
// @param[in] pt_stmt 対象のステートメント
ElbStmt*
StmtGen::instantiate_seqblock(const VlNamedObj* parent,
			      ElbProcess* process,
			      const ElbEnv& env,
			      const PtStmt* pt_stmt)
{
  PtStmtArray pt_stmt_array = pt_stmt->stmt_array();
  ymuint stmt_num = pt_stmt_array.size();
  ElbStmt** stmt_list = factory().new_StmtList(stmt_num);
  for (ymuint i = 0; i < stmt_num; ++ i) {
    const PtStmt* pt_stmt1 = pt_stmt_array[i];
    ElbStmt* stmt1 = instantiate_stmt(parent, process, env, pt_stmt1);
    if ( !stmt1 ) {
      return NULL;
    }
    stmt_list[i] = stmt1;
  }
  
  ElbStmt* stmt = factory().new_Begin(parent, process, pt_stmt, stmt_list);

  return stmt;
}

// @brief 名前つき parallel block のインスタンス化を行う．
// @param[in] parent 親のスコープ
// @param[in] process 親のプロセス (or NULL)
// @param[in] env 生成時の環境
// @param[in] pt_stmt 対象のステートメント
ElbStmt*
StmtGen::instantiate_namedparblock(const VlNamedObj* parent,
				   ElbProcess* process,
				   const ElbEnv& env,
				   const PtStmt* pt_stmt)
{
  ElbObjHandle* handle = find_obj(parent, pt_stmt->name());
  assert_cond(handle, __FILE__, __LINE__);
  const VlNamedObj* block = handle->obj();
  
  PtStmtArray pt_stmt_array = pt_stmt->stmt_array();
  ymuint stmt_num = pt_stmt_array.size();
  ElbStmt** stmt_list = factory().new_StmtList(stmt_num);
  for (ymuint i = 0; i < stmt_num; ++ i) {
    const PtStmt* pt_stmt1 = pt_stmt_array[i];
    ElbStmt* stmt1 = instantiate_stmt(block, process, env,
				      pt_stmt1);
    if ( !stmt1 ) {
      return NULL;
    }
    stmt_list[i] = stmt1;
  }
  
  ElbStmt* stmt = factory().new_NamedFork(block, process,
					  pt_stmt, stmt_list);

  return stmt;
}

// @brief 名前つき sequential block のインスタンス化を行う．
// @param[in] parent 親のスコープ
// @param[in] process 親のプロセス (or NULL)
// @param[in] env 生成時の環境
// @param[in] pt_stmt 対象のステートメント
ElbStmt*
StmtGen::instantiate_namedseqblock(const VlNamedObj* parent,
				   ElbProcess* process,
				   const ElbEnv& env,
				   const PtStmt* pt_stmt)
{
  ElbObjHandle* handle = find_obj(parent, pt_stmt->name());
  assert_cond(handle, __FILE__, __LINE__);
  const VlNamedObj* block = handle->obj();
  
  PtStmtArray pt_stmt_array = pt_stmt->stmt_array();
  ymuint stmt_num = pt_stmt_array.size();
  ElbStmt** stmt_list = factory().new_StmtList(stmt_num);
  for (ymuint i = 0; i < stmt_num; ++ i) {
    const PtStmt* pt_stmt1 = pt_stmt_array[i];
    ElbStmt* stmt1 = instantiate_stmt(block, process, env, pt_stmt1);
    if ( !stmt1 ) {
      return NULL;
    }
    stmt_list[i] = stmt1;
  }
  
  ElbStmt* stmt = factory().new_NamedBegin(block, process,
					   pt_stmt, stmt_list);

  return stmt;
}

END_NAMESPACE_YM_VERILOG
