
/// @file libym_verilog/elb_impl/EiBlockStmt.cc
/// @brief EiBlockStmt の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: EiBlockStmt.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "EiFactory.h"
#include "EiBlockStmt.h"
#include "EiBlockScope.h"

#include "ym_verilog/pt/PtStmt.h"
#include "ym_verilog/pt/PtArray.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// EiFactory の生成関数
//////////////////////////////////////////////////////////////////////
  
// @brief begin ブロックを生成する．
// @param[in] parent 親のスコープ
// @param[in] process 親のプロセス (or NULL)
// @param[in] pt_stmt パース木のステートメント定義
// @param[in] stmt_list 子のステートメントリスト
ElbStmt*
EiFactory::new_Begin(const VlNamedObj* parent,
		     ElbProcess* process,
		     const PtStmt* pt_stmt,
		     ElbStmt** stmt_list)
{
  ymuint32 stmt_num = pt_stmt->stmt_array().size();
  void* p = mAlloc.get_memory(sizeof(EiBegin));
  EiBegin* stmt = new (p) EiBegin(parent, process, pt_stmt,
				  stmt_num, stmt_list);

  return stmt;
}
  
// @brief fork ブロックを生成する．
// @param[in] parent 親のスコープ
// @param[in] process 親のプロセス (or NULL)
// @param[in] pt_stmt パース木のステートメント定義
// @param[in] stmt_list 子のステートメントリスト
ElbStmt*
EiFactory::new_Fork(const VlNamedObj* parent,
		    ElbProcess* process,
		    const PtStmt* pt_stmt,
		    ElbStmt** stmt_list)
{
  ymuint32 stmt_num = pt_stmt->stmt_array().size();
  void* p = mAlloc.get_memory(sizeof(EiFork));
  EiFork* stmt = new (p) EiFork(parent, process, pt_stmt,
				 stmt_num, stmt_list);

  return stmt;
}
  
// @brief 名前付き begin ブロックを生成する．
// @param[in] block 自分自身に対応するスコープ
// @param[in] process 親のプロセス (or NULL)
// @param[in] pt_stmt パース木のステートメント定義
// @param[in] stmt_list 子のステートメントリスト
ElbStmt*
EiFactory::new_NamedBegin(const VlNamedObj* block,
			  ElbProcess* process,
			  const PtStmt* pt_stmt,
			  ElbStmt** stmt_list)
{
  ymuint32 stmt_num = pt_stmt->stmt_array().size();
  void* p = mAlloc.get_memory(sizeof(EiNamedBegin));
  EiNamedBegin* stmt = new (p) EiNamedBegin(block, process,
					    stmt_num, stmt_list);

  return stmt;
}

// @brief 名前付き fork ブロックを生成する．
// @param[in] block 自分自身に対応するスコープ
// @param[in] process 親のプロセス (or NULL)
// @param[in] pt_stmt パース木のステートメント定義
// @param[in] stmt_list 子のステートメントリスト
ElbStmt*
EiFactory::new_NamedFork(const VlNamedObj* block,
			 ElbProcess* process,
			 const PtStmt* pt_stmt,
			 ElbStmt** stmt_list)
{
  ymuint32 stmt_num = pt_stmt->stmt_array().size();
  void* p = mAlloc.get_memory(sizeof(EiNamedFork));
  EiNamedFork* stmt = new (p) EiNamedFork(block, process,
					  stmt_num, stmt_list);

  return stmt;
}


//////////////////////////////////////////////////////////////////////
// クラス EiBlockStmt
//////////////////////////////////////////////////////////////////////
  
// @brief コンストラクタ
// @param[in] parent 親のスコープ
// @param[in] process 親のプロセス (or NULL)
// @param[in] pt_stmt パース木のステートメント定義
// @param[in] stmt_num ステートメントのリストの要素数
// @param[in] array ステートメントのリスト用配列
EiBlockStmt::EiBlockStmt(const VlNamedObj* parent,
			 ElbProcess* process,
			 const PtStmt* pt_stmt,
			 ymuint32 stmt_num,
			 ElbStmt** array) :
  EiStmtBase(parent, process, pt_stmt),
  mStmtNum(stmt_num),
  mStmtList(array)
{
}

// @brief デストラクタ
EiBlockStmt::~EiBlockStmt()
{
}

// @brief 中身のステートメントのリストの要素数を返す．
ymuint32
EiBlockStmt::child_stmt_num() const
{
  return mStmtNum;
}

// @brief 中身のステートメントを返す．
// @param[in] pos 位置番号
ElbStmt*
EiBlockStmt::_child_stmt(ymuint32 pos) const
{
  return mStmtList[pos];
}


//////////////////////////////////////////////////////////////////////
// クラス EiBegin
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parent 親のスコープ
// @param[in] process 親のプロセス (or NULL)
// @param[in] pt_stmt パース木のステートメント定義
// @param[in] stmt_num ステートメントのリストの要素数
// @param[in] array ステートメントのリスト用配列
EiBegin::EiBegin(const VlNamedObj* parent,
		 ElbProcess* process,
		 const PtStmt* pt_stmt,
		 ymuint32 stmt_num,
		 ElbStmt** array) :
  EiBlockStmt(parent, process, pt_stmt, stmt_num, array)
{
}

// @brief デストラクタ
EiBegin::~EiBegin()
{
}

// @brief 型の取得
tVpiObjType
EiBegin::type() const
{
  return kVpiBegin;
}
  
// @brief function 中の実行を行う．
const VlNamedObj*
EiBegin::func_exec(bool constant_function) const
{
  for (ymuint32 i = 0; i < child_stmt_num(); ++ i) {
    ElbStmt* stmt1 = _child_stmt(i);
    const VlNamedObj* scope = stmt1->func_exec(constant_function);
    if ( scope ) {
      return scope;
    }
  }
  return NULL;
}


//////////////////////////////////////////////////////////////////////
// クラス EiFork
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parent 親のスコープ
// @param[in] process 親のプロセス (or NULL)
// @param[in] pt_stmt パース木のステートメント定義
// @param[in] stmt_num ステートメントのリストの要素数
// @param[in] array ステートメントのリスト用配列
EiFork::EiFork(const VlNamedObj* parent,
	       ElbProcess* process,
	       const PtStmt* pt_stmt,
	       ymuint32 stmt_num,
	       ElbStmt** array) :
  EiBlockStmt(parent, process, pt_stmt, stmt_num, array)
{
}

// @brief デストラクタ
EiFork::~EiFork()
{
}

// @brief 型の取得
tVpiObjType
EiFork::type() const
{
  return kVpiFork;
}

// @brief function 中の実行を行う．
// @note このクラスは function 中では使えないのでエラーとなる．
const VlNamedObj*
EiFork::func_exec(bool constant_function) const
{
  assert_not_reached(__FILE__, __LINE__);
  return NULL;
}


//////////////////////////////////////////////////////////////////////
// クラス EiNamedBlockStmt
//////////////////////////////////////////////////////////////////////
  
// @brief コンストラクタ
// @param[in] block 自分自身に対応するスコープ
// @param[in] process 親のプロセス (or NULL)
// @param[in] stmt_num ステートメントのリストの要素数
// @param[in] array ステートメントのリスト用配列
EiNamedBlockStmt::EiNamedBlockStmt(const VlNamedObj* block,
				   ElbProcess* process,
				   ymuint32 stmt_num,
				   ElbStmt** array) :
  mBlockScope(block),
  mProcess(process),
  mStmtNum(stmt_num),
  mStmtList(array)
{
}

// @brief デストラクタ
EiNamedBlockStmt::~EiNamedBlockStmt()
{
}

// @brief ファイル位置の取得
FileRegion
EiNamedBlockStmt::file_region() const
{
  return mBlockScope->file_region();
}

// @brief 親のスコープを返す．
const VlNamedObj*
EiNamedBlockStmt::parent() const
{
  return mBlockScope->parent();
}
  
// @brief 対応するスコープを返す．
const VlNamedObj*
EiNamedBlockStmt::scope() const
{
  return mBlockScope;
}

// @brief 中身のステートメントのリストの要素数を返す．
ymuint32
EiNamedBlockStmt::child_stmt_num() const
{
  return mStmtNum;
}

// @brief 中身のステートメントを返す．
// @param[in] pos 位置番号
ElbStmt*
EiNamedBlockStmt::_child_stmt(ymuint32 pos) const
{
  return mStmtList[pos];
}


//////////////////////////////////////////////////////////////////////
// クラス EiNamedBegin
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] block 自分自身に対応するスコープ
// @param[in] process 親のプロセス (or NULL)
// @param[in] stmt_num ステートメントのリストの要素数
// @param[in] array ステートメントのリスト用配列
EiNamedBegin::EiNamedBegin(const VlNamedObj* block,
			   ElbProcess* process,
			   ymuint32 stmt_num,
			   ElbStmt** array) :
  EiNamedBlockStmt(block, process, stmt_num, array)
{
}

// @brief デストラクタ
EiNamedBegin::~EiNamedBegin()
{
}

// @brief 型の取得
tVpiObjType
EiNamedBegin::type() const
{
  return kVpiNamedBegin;
}
  
// @brief function 中の実行を行う．
const VlNamedObj*
EiNamedBegin::func_exec(bool constant_function) const
{
  for (ymuint32 i = 0; i < child_stmt_num(); ++ i) {
    ElbStmt* stmt1 = _child_stmt(i);
    const VlNamedObj* scope1 = stmt1->func_exec(constant_function);
    if ( scope1 ) {
      if ( scope1 == scope() ) {
	return NULL;
      }
      else {
	return scope1;
      }
    }
  }
  return NULL;
}


//////////////////////////////////////////////////////////////////////
// クラス EiNamedFork
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] block 自分自身に対応するスコープ
// @param[in] process 親のプロセス (or NULL)
// @param[in] stmt_num ステートメントのリストの要素数
// @param[in] array ステートメントのリスト用配列
EiNamedFork::EiNamedFork(const VlNamedObj* block,
			 ElbProcess* process,
			 ymuint32 stmt_num,
			 ElbStmt** array) :
  EiNamedBlockStmt(block, process, stmt_num, array)
{
}

// @brief デストラクタ
EiNamedFork::~EiNamedFork()
{
}

// @brief 型の取得
tVpiObjType
EiNamedFork::type() const
{
  return kVpiNamedFork;
}
  
// @brief function 中の実行を行う．
// @note このクラスは function 中では使えない．
const VlNamedObj*
EiNamedFork::func_exec(bool constant_function) const
{
  assert_not_reached(__FILE__, __LINE__);
  return NULL;
}

END_NAMESPACE_YM_VERILOG
