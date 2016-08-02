
/// @file libym_verilog/elb_impl/EiCondStmt.cc
/// @brief EiCondStmt の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: EiCondStmt.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "EiFactory.h"
#include "EiCondStmt.h"
#include "ElbExpr.h"

#include "ym_verilog/BitVector.h"
#include "ym_verilog/pt/PtStmt.h"



BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// EiFactory の生成関数
//////////////////////////////////////////////////////////////////////

// @brief while 文を生成する．
// @param[in] parent 親のスコープ
// @param[in] process 親のプロセス (or NULL)
// @param[in] pt_stmt パース木のステートメント定義
// @param[in] cond 条件式
// @param[in] stmt 本体のステートメント
ElbStmt*
EiFactory::new_WhileStmt(const VlNamedObj* parent,
			 ElbProcess* process,
			 const PtStmt* pt_stmt,
			 ElbExpr* cond,
			 ElbStmt* stmt)
{
  void* p = mAlloc.get_memory(sizeof(EiWhileStmt));
  ElbStmt* stmt1 = new (p) EiWhileStmt(parent, process, pt_stmt,
				       cond, stmt);

  return stmt1;
}

// @brief repeat 文を生成する．
// @param[in] parent 親のスコープ
// @param[in] process 親のプロセス (or NULL)
// @param[in] pt_stmt パース木のステートメント定義
// @param[in] cond 条件式
// @param[in] stmt 本体のステートメント
ElbStmt*
EiFactory::new_RepeatStmt(const VlNamedObj* parent,
			  ElbProcess* process,
			  const PtStmt* pt_stmt,
			  ElbExpr* cond,
			  ElbStmt* stmt)
{
  void* p = mAlloc.get_memory(sizeof(EiRepeatStmt));
  ElbStmt* stmt1 = new (p) EiRepeatStmt(parent, process, pt_stmt,
					cond, stmt);

  return stmt1;
}

// @brief wait 文を生成する．
// @param[in] parent 親のスコープ
// @param[in] process 親のプロセス (or NULL)
// @param[in] pt_stmt パース木のステートメント定義
// @param[in] cond 条件式
// @param[in] stmt 本体のステートメント
ElbStmt*
EiFactory::new_WaitStmt(const VlNamedObj* parent,
			ElbProcess* process,
			const PtStmt* pt_stmt,
			ElbExpr* cond,
			ElbStmt* stmt)
{
  void* p = mAlloc.get_memory(sizeof(EiWaitStmt));
  ElbStmt* stmt1 = new (p) EiWaitStmt(parent, process, pt_stmt,
				      cond, stmt);

  return stmt1;
}

// @brief for 文を生成する．
// @param[in] parent 親のスコープ
// @param[in] process 親のプロセス (or NULL)
// @param[in] pt_stmt パース木のステートメント定義
// @param[in] cond 条件式
// @param[in] init_stmt 初期化式
// @param[in] inc_stmt 増加式
// @param[in] stmt 本体のステートメント
ElbStmt*
EiFactory::new_ForStmt(const VlNamedObj* parent,
		       ElbProcess* process,
		       const PtStmt* pt_stmt,
		       ElbExpr* cond,
		       ElbStmt* init_stmt,
		       ElbStmt* inc_stmt,
		       ElbStmt* stmt)
{
  void* p = mAlloc.get_memory(sizeof(EiForStmt));
  ElbStmt* stmt1 = new (p) EiForStmt(parent, process, pt_stmt,
				     cond, init_stmt, inc_stmt, stmt);

  return stmt1;
}

// @brief forever 文を生成する．
// @param[in] parent 親のスコープ
// @param[in] process 親のプロセス (or NULL)
// @param[in] pt_stmt パース木のステートメント定義
// @param[in] stmt 本体のステートメント
ElbStmt*
EiFactory::new_ForeverStmt(const VlNamedObj* parent,
			   ElbProcess* process,
			   const PtStmt* pt_stmt,
			   ElbStmt* stmt)
{
  void* p = mAlloc.get_memory(sizeof(EiForeverStmt));
  ElbStmt* stmt1 = new (p) EiForeverStmt(parent, process, pt_stmt,
					 stmt);

  return stmt1;
}

// @brief if 文を生成する．
// @param[in] parent 親のスコープ
// @param[in] process 親のプロセス (or NULL)
// @param[in] pt_stmt パース木のステートメント定義
// @param[in] cond 条件式
// @param[in] stmt 本体のステートメント
// @param[in] else_stmt else節のステートメント
// @note else_stmt は NULL もありうる．
ElbStmt*
EiFactory::new_IfStmt(const VlNamedObj* parent,
		      ElbProcess* process,
		      const PtStmt* pt_stmt,
		      ElbExpr* cond,
		      ElbStmt* stmt,
		      ElbStmt* else_stmt)
{
  ElbStmt* stmt1;
  if ( else_stmt ) {
    void* p = mAlloc.get_memory(sizeof(EiIfElseStmt));
    stmt1 = new (p) EiIfElseStmt(parent, process, pt_stmt,
				 cond, stmt, else_stmt);
  }
  else {
    void* p = mAlloc.get_memory(sizeof(EiIfStmt));
    stmt1 = new (p) EiIfStmt(parent, process, pt_stmt,
			     cond, stmt);
  }
  return stmt1;
}
  
// @brief case 文を生成する．
// @param[in] parent 親のスコープ
// @param[in] process 親のプロセス (or NULL)
// @param[in] pt_stmt パース木のステートメント定義
// @param[in] expr 条件式
ElbStmt*
EiFactory::new_CaseStmt(const VlNamedObj* parent,
			ElbProcess* process,
			const PtStmt* pt_stmt,
			ElbExpr* expr)
{
  ymuint32 caseitem_num = pt_stmt->caseitem_num();
  
  void* q = mAlloc.get_memory(sizeof(EiCaseItem) * caseitem_num);
  EiCaseItem* array = new (q) EiCaseItem[caseitem_num];
  
  void* p = mAlloc.get_memory(sizeof(EiCaseStmt));
  EiCaseStmt* stmt1 = new (p) EiCaseStmt(parent, process, pt_stmt,
					 expr,
					 caseitem_num, array);
  
  return stmt1;
}


//////////////////////////////////////////////////////////////////////
// クラス EiLoopStmt
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parent 親のスコープ
// @param[in] process 親のプロセス (or NULL)
// @param[in] pt_stmt パース木のステートメント定義
// @param[in] cond 条件式
// @param[in] stmt 本体のステートメント
EiLoopStmt::EiLoopStmt(const VlNamedObj* parent,
		       ElbProcess* process,
		       const PtStmt* pt_stmt,
		       ElbExpr* cond,
		       ElbStmt* stmt) :
  EiStmtBase(parent, process, pt_stmt),
  mCondition(cond),
  mBodyStmt(stmt)
{
}

// @brief デストラクタ
EiLoopStmt::~EiLoopStmt()
{
}
  
// @brief 条件式を返す．
const VlExpr*
EiLoopStmt::expr() const
{
  return mCondition;
}
  
// @brief 本体のステートメントを返す．
const VlStmt*
EiLoopStmt::body_stmt() const
{
  return mBodyStmt;
}

ElbStmt*
EiLoopStmt::_body_stmt() const
{
  return mBodyStmt;
}


//////////////////////////////////////////////////////////////////////
// クラス EiWhileStmt
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parent 親のスコープ
// @param[in] process 親のプロセス (or NULL)
// @param[in] pt_stmt パース木のステートメント定義
// @param[in] cond 条件式
// @param[in] stmt 本体のステートメント
EiWhileStmt::EiWhileStmt(const VlNamedObj* parent,
			 ElbProcess* process,
			 const PtStmt* pt_stmt,
			 ElbExpr* cond,
			 ElbStmt* stmt) :
  EiLoopStmt(parent, process, pt_stmt, cond, stmt)
{
}

// @brief デストラクタ
EiWhileStmt::~EiWhileStmt()
{
}

// @brief 型の取得
tVpiObjType
EiWhileStmt::type() const
{
  return kVpiWhile;
}

// @brief function 中の実行を行う．
const VlNamedObj*
EiWhileStmt::func_exec(bool constant_function) const
{
  while ( expr()->eval_logic() == kVpiScalar1 ) {
    const VlNamedObj* scope = _body_stmt()->func_exec(constant_function);
    if ( scope ) {
      return scope;
    }
  }
  return NULL;
}


//////////////////////////////////////////////////////////////////////
// クラス EiRepeatStmt
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parent 親のスコープ
// @param[in] process 親のプロセス (or NULL)
// @param[in] pt_stmt パース木のステートメント定義
// @param[in] cond 条件式
// @param[in] stmt 本体のステートメント
EiRepeatStmt::EiRepeatStmt(const VlNamedObj* parent,
			   ElbProcess* process,
			   const PtStmt* pt_stmt,
			   ElbExpr* cond,
			   ElbStmt* stmt) :
  EiLoopStmt(parent, process, pt_stmt, cond, stmt)
{
}

// @brief デストラクタ
EiRepeatStmt::~EiRepeatStmt()
{
}

// @brief 型の取得
tVpiObjType
EiRepeatStmt::type() const
{
  return kVpiRepeat;
}

// @brief function 中の実行を行う．
const VlNamedObj*
EiRepeatStmt::func_exec(bool constant_function) const
{
  int n;
  if ( !expr()->eval_int(n) ) {
    // 整数値でなかったら 0 と見なす．
    n = 0;
  }
  for (int i = 0; i < n; ++ i) {
    const VlNamedObj* scope = _body_stmt()->func_exec(constant_function);
    if ( scope ) {
      return scope;
    }
  }
  return NULL;
}


//////////////////////////////////////////////////////////////////////
// クラス EiWaitStmt
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parent 親のスコープ
// @param[in] process 親のプロセス (or NULL)
// @param[in] pt_stmt パース木のステートメント定義
// @param[in] cond 条件式
// @param[in] stmt 本体のステートメント
EiWaitStmt::EiWaitStmt(const VlNamedObj* parent,
		       ElbProcess* process,
		       const PtStmt* pt_stmt,
		       ElbExpr* cond,
		       ElbStmt* stmt) :
  EiLoopStmt(parent, process, pt_stmt, cond, stmt)
{
}

// @brief デストラクタ
EiWaitStmt::~EiWaitStmt()
{
}

// @brief 型の取得
tVpiObjType
EiWaitStmt::type() const
{
  return kVpiWait;
}
  
// @brief function 中の実行を行う．
// @note このクラスは function 中では使えない．
const VlNamedObj*
EiWaitStmt::func_exec(bool constant_function) const
{
  assert_not_reached(__FILE__, __LINE__);
  return NULL;
}


//////////////////////////////////////////////////////////////////////
// クラス EiForStmt
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parent 親のスコープ
// @param[in] process 親のプロセス (or NULL)
// @param[in] pt_stmt パース木のステートメント定義
// @param[in] cond 条件式
// @param[in] init_stmt 初期化式
// @param[in] inc_stmt 増加式
// @param[in] stmt 本体のステートメント
EiForStmt::EiForStmt(const VlNamedObj* parent,
		     ElbProcess* process,
		     const PtStmt* pt_stmt,
		     ElbExpr* cond,
		     ElbStmt* init_stmt,
		     ElbStmt* inc_stmt,
		     ElbStmt* stmt) :
  EiLoopStmt(parent, process, pt_stmt, cond, stmt),
  mInitStmt(init_stmt),
  mIncStmt(inc_stmt)
{
} 

// @brief デストラクタ
EiForStmt::~EiForStmt()
{
}

// @brief 型の取得
tVpiObjType
EiForStmt::type() const
{
  return kVpiFor;
}
  
// 初期化文を返す．
const VlStmt*
EiForStmt::init_stmt() const
{
  return mInitStmt;
}
  
// 繰り返し文を返す．
const VlStmt*
EiForStmt::inc_stmt() const
{
  return mIncStmt;
}

// @brief function 中の実行を行う．
const VlNamedObj*
EiForStmt::func_exec(bool constant_function) const
{
  const VlNamedObj* scope = mInitStmt->func_exec(constant_function);
  if ( scope ) {
    return scope;
  }
  while ( expr()->eval_logic() == kVpiScalar1 ) {
    scope = _body_stmt()->func_exec(constant_function);
    if ( scope ) {
      return scope;
    }
    scope = mIncStmt->func_exec(constant_function);
    if ( scope ) {
      return scope;
    }
  }
  return NULL;
}


//////////////////////////////////////////////////////////////////////
// クラス EiForeverStmt
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parent 親のスコープ
// @param[in] process 親のプロセス (or NULL)
// @param[in] pt_stmt パース木のステートメント定義
// @param[in] stmt 本体のステートメント
EiForeverStmt::EiForeverStmt(const VlNamedObj* parent,
			     ElbProcess* process,
			     const PtStmt* pt_stmt,
			     ElbStmt* stmt) :
  EiStmtBase(parent, process, pt_stmt),
  mBodyStmt(stmt)
{
}

// @brief デストラクタ
EiForeverStmt::~EiForeverStmt()
{
}

// @brief 型の取得
tVpiObjType
EiForeverStmt::type() const
{
  return kVpiForever;
}
  
// @brief 本体のステートメントを返す．
const VlStmt*
EiForeverStmt::body_stmt() const
{
  return mBodyStmt;
}
  
// @brief function 中の実行を行う．
const VlNamedObj*
EiForeverStmt::func_exec(bool constant_function) const
{
  while ( 1 ) {
    const VlNamedObj* scope = mBodyStmt->func_exec(constant_function);
    if ( scope ) {
      return scope;
    }
  }
  return NULL;
}


//////////////////////////////////////////////////////////////////////
// クラス IfStmt
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parent 親のスコープ
// @param[in] process 親のプロセス (or NULL)
// @param[in] pt_stmt パース木のステートメント定義
// @param[in] cond 条件式
// @param[in] stmt 本体のステートメント
EiIfStmt::EiIfStmt(const VlNamedObj* parent,
		   ElbProcess* process,
		   const PtStmt* pt_stmt,
		   ElbExpr* cond,
		   ElbStmt* stmt) :
  EiStmtBase(parent, process, pt_stmt),
  mCondition(cond),
  mBodyStmt(stmt)
{
}

// @brief デストラクタ
EiIfStmt::~EiIfStmt()
{
}

// @brief 型の取得
tVpiObjType
EiIfStmt::type() const
{
  return kVpiIf;
}
  
// @brief 条件式を返す．
const VlExpr*
EiIfStmt::expr() const
{
  return mCondition;
}
  
// @brief 本体のステートメントを返す．
const VlStmt*
EiIfStmt::body_stmt() const
{
  return mBodyStmt;
}
  
// @brief function 中の実行を行う．
const VlNamedObj*
EiIfStmt::func_exec(bool constant_function) const
{
  if ( expr()->eval_scalar() == kVpiScalar1 ) {
    return mBodyStmt->func_exec(constant_function);
  }
  return NULL;
}

ElbStmt*
EiIfStmt::_body_stmt() const
{
  return mBodyStmt;
}


//////////////////////////////////////////////////////////////////////
// クラス EiIfElseStmt
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parent 親のスコープ
// @param[in] process 親のプロセス (or NULL)
// @param[in] pt_stmt パース木のステートメント定義
// @param[in] cond 条件式
// @param[in] stmt 本体のステートメント
// @param[in] else_stmt else節のステートメント
EiIfElseStmt::EiIfElseStmt(const VlNamedObj* parent,
			   ElbProcess* process,
			   const PtStmt* pt_stmt,
			   ElbExpr* cond,
			   ElbStmt* stmt,
			   ElbStmt* else_stmt) :
  EiIfStmt(parent, process, pt_stmt, cond, stmt),
  mElseStmt(else_stmt)
{
}

// @brief デストラクタ
EiIfElseStmt::~EiIfElseStmt()
{
}

// @brief 型の取得
tVpiObjType
EiIfElseStmt::type() const
{
  return kVpiIfElse;
}
  
// @brief else ステートメントを返す．
const VlStmt*
EiIfElseStmt::else_stmt() const
{
  return mElseStmt;
}
  
// @brief function 中の実行を行う．
const VlNamedObj*
EiIfElseStmt::func_exec(bool constant_function) const
{
  if ( expr()->eval_scalar() == kVpiScalar1 ) {
    return _body_stmt()->func_exec(constant_function);
  }
  else {
    return mElseStmt->func_exec(constant_function);
  }
}


//////////////////////////////////////////////////////////////////////
// クラス EiCaseItem
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
EiCaseItem::EiCaseItem() :
  mExprNum(0),
  mExprList(NULL),
  mBodyStmt(NULL)
{
}

// @brief デストラクタ
EiCaseItem::~EiCaseItem()
{
}

// @brief 型の取得
tVpiObjType
EiCaseItem::type() const
{
  return kVpiCaseItem;
}

// @brief ファイル位置を返す．
FileRegion
EiCaseItem::file_region() const
{
  return mPtCaseItem->file_region();
}

// @brief 条件式のリストの要素数を返す．
ymuint32
EiCaseItem::expr_num() const
{
  return mExprNum;
}

// @brief 条件式を返す．
// @param[in] pos 位置番号
const VlExpr*
EiCaseItem::expr(ymuint32 pos) const
{
  return mExprList[pos];
}
  
// @brief 本体のステートメントを返す．
const VlStmt*
EiCaseItem::body_stmt() const
{
  return mBodyStmt;
}


//////////////////////////////////////////////////////////////////////
// クラス EiCase
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parent 親のスコープ
// @param[in] process 親のプロセス (or NULL)
// @param[in] pt_stmt パース木のステートメント定義
// @param[in] expr 条件式
// @param[in] caseitem_list caseitem のリストの要素数
// @param[in] caseitem_array caseitem の配列
EiCaseStmt::EiCaseStmt(const VlNamedObj* parent,
		       ElbProcess* process,
		       const PtStmt* pt_stmt,
		       ElbExpr* expr,
		       ymuint32 caseitem_num,
		       EiCaseItem* caseitem_array) :
  EiStmtBase(parent, process, pt_stmt),
  mCondition(expr),
  mCaseItemNum(caseitem_num),
  mCaseItemList(caseitem_array)
{
}

// @brief デストラクタ
EiCaseStmt::~EiCaseStmt()
{
}

// @brief 型の取得
tVpiObjType
EiCaseStmt::type() const
{
  return kVpiCase;
}

// @brief case type を返す．
tVpiCaseType
EiCaseStmt::case_type() const
{
  switch ( pt_stmt()->type() ) {
  case kPtCaseStmt:  return kVpiCaseExact;
  case kPtCaseXStmt: return kVpiCaseX;
  case kPtCaseZStmt: return kVpiCaseZ;
  default: break;
  }
  assert_not_reached(__FILE__, __LINE__);
  return kVpiCaseExact;
}
  
// @brief 条件式を返す．
const VlExpr*
EiCaseStmt::expr() const
{
  return mCondition;
}

// @brief case item のリストの要素数を返す．
ymuint32
EiCaseStmt::case_item_num() const
{
  return mCaseItemNum;
}

// @brief case item を返す．
// @param[in] pos 位置番号
const VlCaseItem*
EiCaseStmt::case_item(ymuint32 pos) const
{
  return &mCaseItemList[pos];
}

// @brief caseitem を設定する．
// @param[in] pt_caseitem パース木の caseitem 定義
// @param[in] expr_array ラベルのリスト用配列
// @param[in] stmt 本体のステートメント
void
EiCaseStmt::set_caseitem(ymuint32 pos,
			 const PtCaseItem* pt_caseitem,
			 ElbExpr** expr_array,
			 ElbStmt* stmt)
{
  EiCaseItem& ci = mCaseItemList[pos];
  ci.mPtCaseItem = pt_caseitem;
  ci.mExprNum = pt_caseitem->label_num();
  ci.mExprList = expr_array;
  ci.mBodyStmt = stmt;
}

// @brief function 中の実行を行う．
const VlNamedObj*
EiCaseStmt::func_exec(bool constant_function) const
{
  BitVector val;
  BitVector tmp;
  mCondition->eval_bitvector(val);
  for (ymuint32 i = 0; i < mCaseItemNum; ++ i) {
    EiCaseItem& ci = mCaseItemList[i];
    bool found = false;
    for (ymuint32 j = 0; j < ci.mExprNum; ++ j) {
      ci.mExprList[j]->eval_bitvector(tmp);
      switch ( pt_stmt()->type() ) {
      case kPtCaseStmt:
	if ( eq(val, tmp) ) {
	  found = true;
	}
	break;
	
      case kPtCaseXStmt:
	if ( eq_with_x(val, tmp) ) {
	  found = true;
	}
	break;
	
      case kPtCaseZStmt:
	if ( eq_with_xz(val, tmp) ) {
	  found = true;
	}
	break;
	
      default:
	assert_not_reached(__FILE__, __LINE__);
	break;
      }
      if ( found ) {
	break;
      }
    }
    if ( found || ci.mExprNum == 0 ) {
      const VlNamedObj* scope = ci.mBodyStmt->func_exec(constant_function);
      if ( scope ) {
	return scope;
      }
    }
  }
  return NULL;
}

END_NAMESPACE_YM_VERILOG
