
/// @file libym_verilog/tests/vlview/VmStmtNode.cc
/// @brief VmStmtNode の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: VmStmtNode.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2009 Yusuke Matsunaga
/// All rights reserved.


#include "VmStmtNode.h"
#include "ym_verilog/vl/VlModule.h"
#include "ym_verilog/vl/VlDecl.h"
#include "ym_verilog/vl/VlProcess.h"
#include "ym_verilog/vl/VlStmt.h"
#include "ym_verilog/vl/VlTaskFunc.h"
#include "ym_verilog/vl/VlUserSystf.h"
#include "ym_verilog/vl/VlControl.h"
#include "ym_verilog/vl/VlExpr.h"
#include "ym_verilog/vl/VlNamedObj.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス VmNode の関数
//////////////////////////////////////////////////////////////////////

// @brief ProcessListNode を追加する．
// @param[in] process_list プロセスのリスト
void
VmNode1::add_processlist(const vector<const VlProcess*>& process_list) const
{
  add_child( new VmProcessListNode(vl_mgr(), process_list) );
}

// @brief StmtNodeを追加する．
// @param[in] label ラベル
// @param[in] stmt ステートメント
void
VmNode1::add_stmt(const QString& label,
		  const VlStmt* stmt) const
{
  assert_cond( stmt != NULL, __FILE__, __LINE__);
  add_child( new VmStmtNode(vl_mgr(), label, stmt) );
}


//////////////////////////////////////////////////////////////////////
// クラス VmProcessListNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] vl_mgr VlMgr
// @param[in] process_array プロセスの配列
VmProcessListNode::VmProcessListNode(const VlMgr& vl_mgr,
				     const vector<const VlProcess*>& process_array) :
  VmNode1(vl_mgr),
  mProcessArray(process_array)
{
}


// @brief デストラクタ
VmProcessListNode::~VmProcessListNode()
{
}

// @brief データを返す．
// @param[in] column コラム番号
// @param[in] role
QVariant
VmProcessListNode::data(int column,
			int role) const
{
  if ( role == Qt::DisplayRole ) {
    if ( column == 0 ) {
      return "vpiProcess list";
    }
    else if ( column == 1 ) {
      return "";
    }
  }
  return QVariant();
}

// @brief 対象のファイル上での位置を返す．
FileRegion
VmProcessListNode::loc() const
{
  if ( mProcessArray.empty() ) {
    return FileRegion();
  }
  else {
    FileRegion first = mProcessArray.front()->file_region();
    FileRegion last = mProcessArray.back()->file_region();
    return FileRegion(first, last);
  }
}

// @brief 子供の配列を作る．
void
VmProcessListNode::expand() const
{
  for (vector<const VlProcess*>::const_iterator p = mProcessArray.begin();
       p != mProcessArray.end(); ++ p) {
    add_child( new VmProcessNode(vl_mgr(), *p) );
  }
}


//////////////////////////////////////////////////////////////////////
// クラス VmProcessNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] vl_mgr VlMgr
// @param[in] process プロセス
VmProcessNode::VmProcessNode(const VlMgr& vl_mgr,
			     const VlProcess* process) :
  VmNode1(vl_mgr),
  mProcess(process)
{
}

// @brief デストラクタ
VmProcessNode::~VmProcessNode()
{
}

// @brief データを返す．
// @param[in] column コラム番号
// @param[in] role
QVariant
VmProcessNode::data(int column,
		    int role) const
{
  if ( role == Qt::DisplayRole ) {
    if ( column == 0 ) {
      return "vpiProcess";
    }
    else if ( column == 1 ) {
      switch ( mProcess->type() ) {
      case kVpiInitial: return "vpiInitial";
      case kVpiAlways:  return "vpiAlways";
      default: assert_not_reached(__FILE__, __LINE__);
      }
    }
  }
  return QVariant();
}

// @brief 対象のファイル上での位置を返す．
FileRegion
VmProcessNode::loc() const
{
  return mProcess->file_region();
}

// @brief 子供の配列を作る．
void
VmProcessNode::expand() const
{
  add_str("vpiModule", mProcess->parent()->parent_module()->full_name());
  add_stmt("vpiStmt", mProcess->stmt());
}


//////////////////////////////////////////////////////////////////////
// クラス VmStmtListNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] vl_mgr VlMgr
// @param[in] label ラベル
// @param[in] stmt 親のステートメント
VmStmtListNode::VmStmtListNode(const VlMgr& vl_mgr,
			       const QString& label,
			       const VlStmt* stmt) :
  VmNode1(vl_mgr),
  mLabel(label),
  mStmt(stmt)
{
}

// @brief デストラクタ
VmStmtListNode::~VmStmtListNode()
{
}

// @brief データを返す．
// @param[in] column コラム番号
// @param[in] role
QVariant
VmStmtListNode::data(int column,
		   int role) const
{
  if ( role == Qt::DisplayRole ) {
    if ( column == 0 ) {
      return mLabel + " list";
    }
    else if ( column == 1 ) {
      return "";
    }
  }
  return QVariant();
}

// @brief 対象のファイル上での位置を返す．
FileRegion
VmStmtListNode::loc() const
{
  ymuint n = mStmt->child_stmt_num();
  if ( n == 0 ) {
    return FileRegion();
  }
  else {
    FileRegion first = mStmt->child_stmt(0)->file_region();
    FileRegion last = mStmt->child_stmt(n - 1)->file_region();
    return FileRegion(first, last);
  }
}

// @brief 子供の配列を作る．
void
VmStmtListNode::expand() const
{
  ymuint n = mStmt->child_stmt_num();
  for (ymuint i = 0; i < n; ++ i) {
    add_stmt("vpiStmt", mStmt->child_stmt(i));
  }
}


//////////////////////////////////////////////////////////////////////
// クラス VmStmtNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] vl_mgr VlMgr
// @param[in] label
// @param[in] stmt ステートメント
VmStmtNode::VmStmtNode(const VlMgr& vl_mgr,
		       const QString& label,
		       const VlStmt* stmt) :
  VmNode1(vl_mgr),
  mLabel(label),
  mStmt(stmt)
{
}

// @brief デストラクタ
VmStmtNode::~VmStmtNode()
{
}

// @brief データを返す．
// @param[in] column コラム番号
// @param[in] role
QVariant
VmStmtNode::data(int column,
		 int role) const
{
  if ( role == Qt::DisplayRole ) {
    if ( column == 0 ) {
      return mLabel;
    }
    else if ( column == 1 ) {
      switch ( mStmt->type() ) {
      case kVpiBegin:           return "Begin";
      case kVpiFork:            return "Fork";
      case kVpiNamedBegin:      return "NamedBegin";
      case kVpiNamedFork:       return "NamedFork";
      case kVpiNullStmt:        return "NullStmt";
      case kVpiEventStmt:       return "EventStmt";
      case kVpiAssignment:      return "Assignment";
      case kVpiWhile:           return "While";
      case kVpiRepeat:          return "Repeat";
      case kVpiWait:            return "Wait";
      case kVpiFor:             return "For";
      case kVpiForever:         return "Forever";
      case kVpiIf:              return "If";
      case kVpiIfElse:          return "IfElse";
      case kVpiCase:            return "Case";
      case kVpiForce:           return "Force";
      case kVpiAssignStmt:      return "AssignStmt";
      case kVpiDeassign:        return "Deassign";
      case kVpiRelease:         return "Release";
      case kVpiTaskCall:        return "TaskCall";
      case kVpiSysTaskCall:     return "SysTaskCall";
      case kVpiDisable:         return "Disable";
      case kVpiDelayControl:    return "DelayControl";
      case kVpiEventControl:    return "EventControl";
      default: assert_not_reached( __FILE__, __LINE__);
      }
    }
  }
  return QVariant();
}

// @brief 対象のファイル上での位置を返す．
FileRegion
VmStmtNode::loc() const
{
  return mStmt->file_region();
}

// @brief 子供の配列を作る．
void
VmStmtNode::expand() const
{
  add_str("vpiScope", mStmt->parent()->full_name());

  switch ( mStmt->type() ) {
  case kVpiNamedBegin:
  case kVpiNamedFork:
    {
      const VlNamedObj* scope = mStmt->scope();
      assert_cond( scope, __FILE__, __LINE__);
      add_str("vpiFullName", scope->full_name());
      add_scope_item(scope);
    }
    // わざと次に続く
  case kVpiBegin:
  case kVpiFork:
    add_child( new VmStmtListNode(vl_mgr(), "vpiChildStmt", mStmt) );
    break;

  case kVpiEventStmt:
    add_str("vpiNamedEvent", mStmt->named_event()->full_name());
    break;

  case kVpiAssignment:
    add_bool("vpiBlocking", mStmt->is_blocking());
    add_expr("vpiLhs", mStmt->lhs());
    add_expr("vpiRhs", mStmt->rhs());
    if ( mStmt->control() ) {
      add_child( new VmControlNode(mStmt->control()) );
    }
    break;

  case kVpiWhile:
  case kVpiRepeat:
  case kVpiWait:
    add_expr("vpiCondition", mStmt->expr());
    add_stmt("vpiStmt", mStmt->body_stmt());
    break;

  case kVpiFor:
    add_stmt("vpiForInitStmt", mStmt->init_stmt());
    add_expr("vpiCondition", mStmt->expr());
    add_stmt("vpiForIncStmt", mStmt->inc_stmt());
    add_stmt("vpiStmt", mStmt->body_stmt());
    break;

  case kVpiForever:
    add_stmt("vpiStmt", mStmt->body_stmt());
    break;

  case kVpiIf:
    add_expr("vpiCondition", mStmt->expr());
    add_stmt("vpiStmt", mStmt->body_stmt());
    break;

  case kVpiIfElse:
    add_expr("vpiCondition", mStmt->expr());
    add_stmt("vpiStmt", mStmt->body_stmt());
    add_stmt("vpiElseStmt", mStmt->else_stmt());
    break;

  case kVpiCase:
    switch ( mStmt->case_type() ) {
    case kVpiCaseExact:
      add_str("vpiCaseType", "vpiCaseExact");
      break;

    case kVpiCaseX:
      add_str("vpiCaseType", "vpiCaseX");
      break;

    case kVpiCaseZ:
      add_str("vpiCaseType", "vpiCaseZ");
      break;
    }
    add_expr("vpiCondition", mStmt->expr());
    add_child( new VmCaseItemListNode(vl_mgr(), mStmt) );
    break;

  case kVpiAssignStmt:
  case kVpiForce:
    add_expr("vpiLhs", mStmt->lhs());
    add_expr("vpiRhs", mStmt->rhs());
    break;

  case kVpiDeassign:
  case kVpiRelease:
    add_expr("vpiLhs", mStmt->lhs());
    break;

  case kVpiTaskCall:
    add_str("vpiTask", mStmt->task()->full_name());
    add_child( new VmArgListNode(mStmt) );
    break;

  case kVpiSysTaskCall:
    add_str("vpiUserSystf", mStmt->user_systf()->name());
    add_child( new VmArgListNode(mStmt) );
    break;

  case kVpiDisable:
    add_str("vpiExpr", mStmt->scope()->full_name());
    break;

  case kVpiDelayControl:
  case kVpiEventControl:
    add_child( new VmControlNode(mStmt->control()) );
    add_stmt("vpiStmt", mStmt->body_stmt());
    break;

  default:
    assert_not_reached(__FILE__, __LINE__);
  }
}


//////////////////////////////////////////////////////////////////////
// クラス VmArgListNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] stmt 親のステートメント
VmArgListNode::VmArgListNode(const VlStmt* stmt) :
  mStmt(stmt)
{
}

// @brief デストラクタ
VmArgListNode::~VmArgListNode()
{
}

// @brief データを返す．
// @param[in] column コラム番号
// @param[in] role
QVariant
VmArgListNode::data(int column,
		    int role) const
{
  if ( role == Qt::DisplayRole ) {
    if ( column == 0 ) {
      return "vpiArgument list";
    }
    else if ( column == 1 ) {
      return "";
    }
  }
  return QVariant();
}

// @brief 対象のファイル上での位置を返す．
FileRegion
VmArgListNode::loc() const
{
  ymuint n = mStmt->arg_num();
  if ( n == 0 ) {
    return FileRegion();
  }
  else {
    FileRegion first = mStmt->arg(0)->file_region();
    FileRegion last = mStmt->arg(n - 1)->file_region();
    return FileRegion(first, last);
  }
}

// @brief 子供の配列を作る．
void
VmArgListNode::expand() const
{
  ymuint n = mStmt->arg_num();
  for (ymuint i = 0; i < n; ++ i) {
    add_expr("vpiArgument", mStmt->arg(i));
  }
}


//////////////////////////////////////////////////////////////////////
// クラス VmCaseItemListNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] vl_mgr VlMgr
// @param[in] stmt 親の case 文
VmCaseItemListNode::VmCaseItemListNode(const VlMgr& vl_mgr,
				       const VlStmt* stmt) :
  VmNode1(vl_mgr),
  mStmt(stmt)
{
}

// @brief デストラクタ
VmCaseItemListNode::~VmCaseItemListNode()
{
}

// @brief データを返す．
// @param[in] column コラム番号
// @param[in] role
QVariant
VmCaseItemListNode::data(int column,
			 int role) const
{
  if ( role == Qt::DisplayRole ) {
    if ( column == 0 ) {
      return "vpiCaseItem list";
    }
    else if ( column == 1 ) {
      return "";
    }
  }
  return QVariant();
}

// @brief 対象のファイル上での位置を返す．
FileRegion
VmCaseItemListNode::loc() const
{
  ymuint n = mStmt->caseitem_num();
  if ( n == 0 ) {
    return FileRegion();
  }
  else {
    FileRegion first = mStmt->caseitem(0)->file_region();
    FileRegion last = mStmt->caseitem(n - 1)->file_region();
    return FileRegion(first, last);
  }
}

// @brief 子供の配列を作る．
void
VmCaseItemListNode::expand() const
{
  ymuint n = mStmt->caseitem_num();
  for (ymuint i = 0; i < n; ++ i) {
    add_child( new VmCaseItemNode(vl_mgr(), mStmt->caseitem(i)) );
  }
}


//////////////////////////////////////////////////////////////////////
// クラス VmCaseItemNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] vl_mgr VlMgr
// @param[in] caseitem caseitem 文
VmCaseItemNode::VmCaseItemNode(const VlMgr& vl_mgr,
			       const VlCaseItem* caseitem) :
  VmNode1(vl_mgr),
  mCaseItem(caseitem)
{
}

// @brief デストラクタ
VmCaseItemNode::~VmCaseItemNode()
{
}

// @brief データを返す．
// @param[in] column コラム番号
// @param[in] role
QVariant
VmCaseItemNode::data(int column,
		     int role) const
{
  if ( role == Qt::DisplayRole ) {
    if ( column == 0 ) {
      return "vpiCaseItem";
    }
    else if ( column == 1 ) {
      return "";
    }
  }
  return QVariant();
}

// @brief 対象のファイル上での位置を返す．
FileRegion
VmCaseItemNode::loc() const
{
  return mCaseItem->file_region();
}

// @brief 子供の配列を作る．
void
VmCaseItemNode::expand() const
{
  ymuint n = mCaseItem->expr_num();
  if ( n == 0 ) {
    add_str("vpiLabel", "Default");
  }
  else {
    for (ymuint i = 0; i < n; ++ i) {
      add_expr("vpiExpr", mCaseItem->expr(i));
    }
  }
  add_stmt("vpiBody", mCaseItem->body_stmt());
}


//////////////////////////////////////////////////////////////////////
// クラス VmControlNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] control コントロールを表すオブジェクト
VmControlNode::VmControlNode(const VlControl* control) :
  mControl(control)
{
}

// @brief デストラクタ
VmControlNode::~VmControlNode()
{
}

// @brief データを返す．
// @param[in] column コラム番号
// @param[in] role
QVariant
VmControlNode::data(int column,
		    int role) const
{
  if ( role == Qt::DisplayRole ) {
    if ( column == 0 ) {
      return "vpiControl";
    }
    else if ( column == 1 ) {
      if ( mControl->delay() ) {
	return "vpiDelayControl";
      }
      else if ( mControl->expr() ) {
	return "vpiRepeatControl";
      }
      else {
	return "vpiEventControl";
      }
    }
  }
  return QVariant();
}

// @brief 対象のファイル上での位置を返す．
FileRegion
VmControlNode::loc() const
{
  return mControl->file_region();
}

// @brief 子供の配列を作る．
void
VmControlNode::expand() const
{
  if ( mControl->delay() ) {
    add_expr("vpiDelay", mControl->delay());
  }
  else {
    if ( mControl->expr() ) {
      add_expr("vpiExpr", mControl->expr());
    }
    ymuint n = mControl->event_num();
    vector<const VlExpr*> event_list(n);
    for (ymuint i = 0; i < n; ++ i) {
      event_list[i] = mControl->event(i);
    }
    add_exprlist("vpiCondition", event_list);
  }
}

END_NAMESPACE_YM_VERILOG
