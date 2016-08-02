
/// @file libym_verilog/elb_impl/EiMiscStmt.cc
/// @brief EiMiscStmt の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: EiMiscStmt.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "EiFactory.h"
#include "EiMiscStmt.h"

#include "ElbDecl.h"
#include "ElbTaskFunc.h"
#include "ElbUserSystf.h"
#include "ElbControl.h"
#include "ElbExpr.h"

#include "ym_verilog/pt/PtStmt.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// EiFactory の生成関数
//////////////////////////////////////////////////////////////////////

// @brief イベント文を生成する．
// @param[in] parent 親のスコープ
// @param[in] process 親のプロセス (or NULL)
// @param[in] pt_stmt パース木のステートメント定義
// @param[in] named_event 対象のイベント
ElbStmt*
EiFactory::new_EventStmt(const VlNamedObj* parent,
			 ElbProcess* process,
			 const PtStmt* pt_stmt,
			 ElbDecl* named_event)
{
  void* p = mAlloc.get_memory(sizeof(EiEventStmt));
  ElbStmt* stmt = new (p) EiEventStmt(parent, process, pt_stmt,
				      named_event);

  return stmt;
}

// @brief NULL ステートメントを生成する．
// @param[in] parent 親のスコープ
// @param[in] process 親のプロセス (or NULL)
// @param[in] pt_stmt パース木のステートメント定義
ElbStmt*
EiFactory::new_NullStmt(const VlNamedObj* parent,
			ElbProcess* process,
			const PtStmt* pt_stmt)
{
  void* p = mAlloc.get_memory(sizeof(EiNullStmt));
  ElbStmt* stmt = new (p) EiNullStmt(parent, process, pt_stmt);

  return stmt;
}

// @brief タスクコール文を生成する．
// @param[in] parent 親のスコープ
// @param[in] process 親のプロセス (or NULL)
// @param[in] pt_stmt パース木のステートメント定義
// @param[in] task 対象のタスク
// @param[in] arg_array 引数の配列
ElbStmt*
EiFactory::new_TaskCall(const VlNamedObj* parent,
			ElbProcess* process,
			const PtStmt* pt_stmt,
			ElbTaskFunc* task,
			ElbExpr** arg_array)
{
  ymuint32 n = pt_stmt->arg_num();
  void* p = mAlloc.get_memory(sizeof(EiTaskCall));
  EiTaskCall* stmt = new (p) EiTaskCall(parent, process, pt_stmt,
					task, n, arg_array);

  return stmt;
}

// @brief システムタスクコール文を生成する．
// @param[in] parent 親のスコープ
// @param[in] process 親のプロセス (or NULL)
// @param[in] pt_stmt パース木のステートメント定義
// @param[in] user_systf システムタスク
// @param[in] arg_array 引数の配列
ElbStmt*
EiFactory::new_SysTaskCall(const VlNamedObj* parent,
			   ElbProcess* process,
			   const PtStmt* pt_stmt,
			   const ElbUserSystf* user_systf,
			   ElbExpr** arg_array)
{
  ymuint32 n = pt_stmt->arg_num();
  void* p = mAlloc.get_memory(sizeof(EiSysTaskCall));
  EiSysTaskCall* stmt = new (p) EiSysTaskCall(parent, process, pt_stmt,
					      user_systf, n, arg_array);

  return stmt;
}

// @brief disable 文を生成する．
// @param[in] parent 親のスコープ
// @param[in] process 親のプロセス (or NULL)
// @param[in] pt_stmt パース木のステートメント定義
// @param[in] target 対象のスコープ
ElbStmt*
EiFactory::new_DisableStmt(const VlNamedObj* parent,
			   ElbProcess* process,
			   const PtStmt* pt_stmt,
			   const VlNamedObj* target)
{
  void* p = mAlloc.get_memory(sizeof(EiDisableStmt));
  ElbStmt* stmt = new (p) EiDisableStmt(parent, process, pt_stmt,
					target);

  return stmt;
}

// @brief コントロール文を生成する．
// @param[in] parent 親のスコープ
// @param[in] process 親のプロセス (or NULL)
// @param[in] pt_stmt パース木のステートメント定義
// @param[in] control コントロール
// @param[in] stmt 本体のステートメント
ElbStmt*
EiFactory::new_CtrlStmt(const VlNamedObj* parent,
			ElbProcess* process,
			const PtStmt* pt_stmt,
			ElbControl* control,
			ElbStmt* stmt)
{
  void* p = mAlloc.get_memory(sizeof(EiCtrlStmt));
  ElbStmt* stmt1 = new (p) EiCtrlStmt(parent, process, pt_stmt,
				      control, stmt);

  return stmt1;
}


//////////////////////////////////////////////////////////////////////
// クラス EiEventStmt
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parent 親のスコープ
// @param[in] process 親のプロセス (or NULL)
// @param[in] pt_stmt パース木のステートメント定義
// @param[in] named_event 対象のイベント
EiEventStmt::EiEventStmt(const VlNamedObj* parent,
			 ElbProcess* process,
			 const PtStmt* pt_stmt,
			 ElbDecl* named_event) :
  EiStmtBase(parent, process, pt_stmt),
  mEvent(named_event)
{
}

// @brief デストラクタ
EiEventStmt::~EiEventStmt()
{
}

// @brief 型の取得
tVpiObjType
EiEventStmt::type() const
{
  return kVpiEventStmt;
}

// @brief named event を返す．
const VlDecl*
EiEventStmt::named_event() const
{
  return mEvent;
}

// @brief function 中の実行を行う．
// @note このクラスは function 中では使えない．
const VlNamedObj*
EiEventStmt::func_exec(bool constant_function) const
{
  assert_not_reached(__FILE__, __LINE__);
  return NULL;
}


//////////////////////////////////////////////////////////////////////
// クラス EiNullStmt
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parent 親のスコープ
// @param[in] process 親のプロセス (or NULL)
// @param[in] pt_stmt パース木のステートメント定義
EiNullStmt::EiNullStmt(const VlNamedObj* parent,
		       ElbProcess* process,
		       const PtStmt* pt_stmt) :
  EiStmtBase(parent, process, pt_stmt)
{
}

// @brief デストラクタ
EiNullStmt::~EiNullStmt()
{
}

// @brief 型の取得
tVpiObjType
EiNullStmt::type() const
{
  return kVpiNullStmt;
}

// @brief function 中の実行を行う．
// @note このクラスでは何もしないで NULL を返す．
const VlNamedObj*
EiNullStmt::func_exec(bool constant_function) const
{
  return NULL;
}


//////////////////////////////////////////////////////////////////////
// クラス EiTcBase
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parent 親のスコープ
// @param[in] process 親のプロセス (or NULL)
// @param[in] pt_stmt パース木のステートメント定義
// @param[in] arg_num 引数の数
// @param[in] arg_array 引数を納める配列
EiTcBase::EiTcBase(const VlNamedObj* parent,
		   ElbProcess* process,
		   const PtStmt* pt_stmt,
		   ymuint32 arg_num,
		   ElbExpr** arg_array) :
  EiStmtBase(parent, process, pt_stmt),
  mArgumentNum(arg_num),
  mArgumentList(arg_array)
{
}

// @brief デストラクタ
EiTcBase::~EiTcBase()
{
}

// @brief 引数の数を返す．
ymuint32
EiTcBase::argument_num() const
{
  return mArgumentNum;
}

// @brief 引数の取得
const VlExpr*
EiTcBase::argument(ymuint32 pos) const
{
  return mArgumentList[pos];
}

// @brief 引数の設定
// @param[in] pos 位置番号
// @param[in] arg 設定する引数
void
EiTcBase::set_argument(ymuint32 pos,
		       ElbExpr* arg)
{
  mArgumentList[pos] = arg;
}


//////////////////////////////////////////////////////////////////////
// クラス EiTaskCall
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parent 親のスコープ
// @param[in] process 親のプロセス (or NULL)
// @param[in] pt_stmt パース木のステートメント定義
// @param[in] task 対象のタスク
// @param[in] arg_num 引数の数
// @param[in] arg_array 引数を納める配列
EiTaskCall::EiTaskCall(const VlNamedObj* parent,
		       ElbProcess* process,
		       const PtStmt* pt_stmt,
		       ElbTaskFunc* task,
		       ymuint32 arg_num,
		       ElbExpr** arg_array) :
  EiTcBase(parent, process, pt_stmt, arg_num, arg_array),
  mTask(task)
{
}

// @brief デストラクタ
EiTaskCall::~EiTaskCall()
{
}

// @brief 型の取得
tVpiObjType
EiTaskCall::type() const
{
  return kVpiTaskCall;
}

// @brief task の実体を返す．
const VlTaskFunc*
EiTaskCall::task() const
{
  return mTask;
}

// @brief function 中の実行を行う．
// @note このクラスは function 中では使えない．
const VlNamedObj*
EiTaskCall::func_exec(bool constant_function) const
{
  assert_not_reached(__FILE__, __LINE__);
  return NULL;
}


//////////////////////////////////////////////////////////////////////
// クラス SysTaskCall
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parent 親のスコープ
// @param[in] process 親のプロセス (or NULL)
// @param[in] pt_stmt パース木のステートメント定義
// @param[in] user_systf システムタスク
// @param[in] arg_num 引数の数
// @param[in] arg_array 引数を納める配列
EiSysTaskCall::EiSysTaskCall(const VlNamedObj* parent,
			     ElbProcess* process,
			     const PtStmt* pt_stmt,
			     const ElbUserSystf* user_systf,
			     ymuint32 arg_num,
			     ElbExpr** arg_array) :
  EiTcBase(parent, process, pt_stmt, arg_num, arg_array),
  mUserSystf(user_systf)
{
}

// @brief デストラクタ
EiSysTaskCall::~EiSysTaskCall()
{
}

// @brief 型の取得
tVpiObjType
EiSysTaskCall::type() const
{
  return kVpiSysTaskCall;
}

// @brief user systf クラスへのポインタを返す．
const VlUserSystf*
EiSysTaskCall::user_systf() const
{
  return mUserSystf;
}

// @brief function 中の実行を行う．
// @note system task は function 中では無視される．
const VlNamedObj*
EiSysTaskCall::func_exec(bool constant_function) const
{
  if ( !constant_function ) {
#warning "TODO: 未完"
  }
  return NULL;
}


//////////////////////////////////////////////////////////////////////
// クラス EiDisableStmt
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parent 親のスコープ
// @param[in] process 親のプロセス (or NULL)
// @param[in] pt_stmt パース木のステートメント定義
// @param[in] target 対象のスコープ
EiDisableStmt::EiDisableStmt(const VlNamedObj* parent,
			     ElbProcess* process,
			     const PtStmt* pt_stmt,
			     const VlNamedObj* target) :
  EiStmtBase(parent, process, pt_stmt),
  mExpr(target)
{
}

// @brief デストラクタ
EiDisableStmt::~EiDisableStmt()
{
}

// @brief 型の取得
tVpiObjType
EiDisableStmt::type() const
{
  return kVpiDisable;
}

// @brief expr を返す．
//
// 型が VlNamedObj になっているが実際には
// ElbFunction/ElbTask/EiNamedFork/EiNamedBegin
// のいずれか
const VlNamedObj*
EiDisableStmt::scope() const
{
  return mExpr;
}

// @brief function 中の実行を行う．
const VlNamedObj*
EiDisableStmt::func_exec(bool constant_function) const
{
  return mExpr;
}


//////////////////////////////////////////////////////////////////////
// クラス EiCtrlStmt
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parent 親のスコープ
// @param[in] process 親のプロセス (or NULL)
// @param[in] pt_stmt パース木のステートメント定義
// @param[in] control コントロール
// @param[in] stmt 本体のステートメント
EiCtrlStmt::EiCtrlStmt(const VlNamedObj* parent,
		       ElbProcess* process,
		       const PtStmt* pt_stmt,
		       ElbControl* control,
		       ElbStmt* stmt) :
  EiStmtBase(parent, process, pt_stmt),
  mControl(control),
  mBodyStmt(stmt)
{
}

// @brief デストラクタ
EiCtrlStmt::~EiCtrlStmt()
{
}

// @brief 型の取得
tVpiObjType
EiCtrlStmt::type() const
{
  return mControl->type();
}

// @brief コントロールを返す．
const VlControl*
EiCtrlStmt::control() const
{
  return mControl;
}

// @brief 本文のステートメントを返す．
const VlStmt*
EiCtrlStmt::body_stmt() const
{
  return mBodyStmt;
}

// @brief function 中の実行を行う．
// @note このクラスは function 中では使えない．
const VlNamedObj*
EiCtrlStmt::func_exec(bool constant_function) const
{
  assert_not_reached(__FILE__, __LINE__);
  return NULL;
}

END_NAMESPACE_YM_VERILOG
