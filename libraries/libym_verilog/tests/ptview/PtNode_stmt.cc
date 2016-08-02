
/// @file libym_verilog/tests/ptview/PtNode_stmt.cc
/// @brief PtNode の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: PtNode_stmt.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2009 Yusuke Matsunaga
/// All rights reserved.


#include "PtNode_stmt.h"
#include "PtNode_decl.h"
#include "PtNode_expr.h"
#include "PtNode_misc.h"
#include "ym_verilog/pt/PtStmt.h"
#include "ym_verilog/pt/PtArray.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス StmtListNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] label ラベル
// @param[in] stmt_array ステートメントの配列
StmtListNode::StmtListNode(const QString& label,
			   const PtStmtArray& stmt_array) :
  mLabel(label),
  mStmtArray(stmt_array)
{
}

// @brief デストラクタ
StmtListNode::~StmtListNode()
{
}

// @brief データを返す．
// @param[in] column コラム番号
// @param[in] role 
QVariant
StmtListNode::data(int column,
		   int role) const
{
  if ( role == Qt::DisplayRole ) {
    if ( column == 0 ) {
      return mLabel;
    }
    else if ( column == 1 ) {
      return "";
    }
  }
  return QVariant();
}
    
// @brief 対象のファイル上での位置を返す．
FileRegion
StmtListNode::loc() const
{
  return FileRegion();
}

// @brief 子供の配列を作る．
void
StmtListNode::expand() const
{
  ymuint32 n = mStmtArray.size();
  mChildren.resize(n);
  for (ymuint32 i = 0; i < n; ++ i) {
    mChildren[i] = new StmtNode("Stmt", mStmtArray[i]);
  }
}


//////////////////////////////////////////////////////////////////////
// クラス StmtNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] label
// @param[in] stmt ステートの配列
StmtNode::StmtNode(const QString& label,
		   const PtStmt* stmt) :
  mLabel(label),
  mStmt(stmt)
{
}

// @brief デストラクタ
StmtNode::~StmtNode()
{
}

// @brief データを返す．
// @param[in] column コラム番号
// @param[in] role 
QVariant
StmtNode::data(int column,
	       int role) const
{
  if ( role == Qt::DisplayRole ) {
    if ( column == 0 ) {
      return mLabel;
    }
    else if ( column == 1 ) {
      switch ( mStmt->type() ) {
      case kPtDisableStmt:       return "Disable";
      case kPtEnableStmt:        return "Enable";
      case kPtSysEnableStmt:     return "SysEnable";
      case kPtDcStmt:            return "ControlStmt";
      case kPtEcStmt:            return "ControlStmt";
      case kPtAssignStmt:        return "Assign";
      case kPtNbAssignStmt:      return "NonBlockingAssign";
      case kPtEventStmt:         return "EventTrigger";
      case kPtNullStmt:          return "NullStmt";
      case kPtIfStmt:            return "If";
      case kPtCaseStmt:          return "Case";
      case kPtCaseXStmt:         return "Casex";
      case kPtCaseZStmt:         return "Casez";
      case kPtWaitStmt:          return "Wait";
      case kPtForeverStmt:       return "Forever";
      case kPtRepeatStmt:        return "Repeat";
      case kPtWhileStmt:         return "While";
      case kPtForStmt:           return "For";
      case kPtPcAssignStmt:      return "PcAssign";
      case kPtDeassignStmt:      return "Deassign";
      case kPtForceStmt:         return "Force";
      case kPtReleaseStmt:       return "Release";
      case kPtParBlockStmt:      return "Fork";
      case kPtSeqBlockStmt:      return "Begin";
      case kPtNamedParBlockStmt: return "Fork";
      case kPtNamedSeqBlockStmt: return "Begin";
      }
    }
  }
  return QVariant();
}
    
// @brief 対象のファイル上での位置を返す．
FileRegion
StmtNode::loc() const
{
  return mStmt->file_region();
}

// @brief 子供の配列を作る．
void
StmtNode::expand() const
{
  ymuint32 n = 0;
  switch ( mStmt->type() ) {
  case kPtDisableStmt:
    n = 1;
    if ( mStmt->namebranch_array().size() > 0 ) {
      ++ n;
    }
    mChildren.reserve(n);
    if ( mStmt->namebranch_array().size() > 0 ) {
      mChildren.push_back( new NameBranchListNode(mStmt->namebranch_array()) );
    }
    mChildren.push_back( new StrNode("Name", mStmt->name()) );
    break;

  case kPtEnableStmt:
  case kPtSysEnableStmt:
    n = 2;
    if ( mStmt->namebranch_array().size() > 0 ) {
      ++ n;
    }
    mChildren.reserve(n);
    if ( mStmt->namebranch_array().size() > 0 ) {
      mChildren.push_back( new NameBranchListNode(mStmt->namebranch_array()) );
    }
    mChildren.push_back( new StrNode("Name", mStmt->name()) );
    mChildren.push_back( new ArgListNode(mStmt) );
    break;

  case kPtDcStmt:
  case kPtEcStmt:
    mChildren.resize(2);
    mChildren[0] = new ControlNode(mStmt->control());
    mChildren[1] = new StmtNode("Body", mStmt->body());
    break;

  case kPtWaitStmt:
    mChildren.resize(2);
    mChildren[0] = new ExprNode("Condition", mStmt->expr());
    mChildren[1] = new StmtNode("Body", mStmt->body());
    break;

  case kPtAssignStmt:
  case kPtNbAssignStmt:
  case kPtPcAssignStmt:
  case kPtForceStmt:
  case kPtDeassignStmt:
  case kPtReleaseStmt:
    n = 2;
    if ( mStmt->control() ) {
      ++ n;
    }
    mChildren.resize(n);
    mChildren[0] = new ExprNode("Lhs", mStmt->lhs());
    mChildren[1] = new ExprNode("Rhs", mStmt->rhs());
    if ( mStmt->control() ) {
      mChildren[2] = new ControlNode(mStmt->control());
    }
    break;

  case kPtEventStmt:
    mChildren.resize(1);
    mChildren[0] = new ExprNode("Event Name", mStmt->primary());
    break;

  case kPtNullStmt:
    break;

  case kPtIfStmt:
    n = 2;
    if ( mStmt->else_body() ) {
      ++ n;
    }
    mChildren.resize(n);
    mChildren[0] = new ExprNode("Condition", mStmt->expr());
    mChildren[1] = new StmtNode("Then body", mStmt->body());
    if ( mStmt->else_body() ) {
      mChildren[2] = new StmtNode("Else Body", mStmt->else_body());
    }
    break;

  case kPtCaseStmt:
  case kPtCaseXStmt:
  case kPtCaseZStmt:
    mChildren.resize(2);
    mChildren[0] = new ExprNode("Condition", mStmt->expr());
    mChildren[1] = new CaseItemListNode(mStmt);
    break;

  case kPtForeverStmt:
    mChildren.resize(1);
    mChildren[0] = new StmtNode("Body", mStmt->body());
    break;
    
  case kPtRepeatStmt:
  case kPtWhileStmt:
    mChildren.resize(2);
    mChildren[0] = new ExprNode("Condition", mStmt->expr());
    mChildren[1] = new StmtNode("Body", mStmt->body());
    break;
    
  case kPtForStmt:
    mChildren.resize(4);
    mChildren[0] = new StmtNode("Initial Stmt", mStmt->init_stmt());
    mChildren[1] = new ExprNode("Condition", mStmt->expr());
    mChildren[2] = new StmtNode("Increent Stmt", mStmt->next_stmt());
    mChildren[3] = new StmtNode("Body", mStmt->body());
    break;
    
  case kPtNamedParBlockStmt:
  case kPtNamedSeqBlockStmt:
    n = 1;
    if ( mStmt->declhead_array().size() > 0 ) {
      ++ n;
    }
    if ( mStmt->stmt_array().size() > 0 ) {
      ++ n;
    }
    mChildren.reserve(n);
    mChildren.push_back( new StrNode("Name", mStmt->name()) );
    if ( mStmt->declhead_array().size() > 0 ) {
      mChildren.push_back( new DeclHeadListNode("Decl List",
						mStmt->declhead_array()) );
    }
    if ( mStmt->stmt_array().size() > 0 ) {
      mChildren.push_back( new StmtListNode("Stmt List",
					    mStmt->stmt_array()) );
    }
    break;

  case kPtParBlockStmt:
  case kPtSeqBlockStmt:
    n = 0;
    if ( mStmt->declhead_array().size() > 0 ) {
      ++ n;
    }
    if ( mStmt->stmt_array().size() > 0 ) {
      ++ n;
    }
    mChildren.reserve(n);
    if ( mStmt->declhead_array().size() > 0 ) {
      mChildren.push_back( new DeclHeadListNode("Decl List",
						mStmt->declhead_array()) );
    }
    if ( mStmt->stmt_array().size() > 0 ) {
      mChildren.push_back( new StmtListNode("Stmt List",
					    mStmt->stmt_array()) );
    }
    break;
  }
}


//////////////////////////////////////////////////////////////////////
// クラス ArgListNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] stmt 親の enable 文
ArgListNode::ArgListNode(const PtStmt* stmt) :
  mStmt(stmt)
{
}

// @brief デストラクタ
ArgListNode::~ArgListNode()
{
}

// @brief データを返す．
// @param[in] column コラム番号
// @param[in] role 
QVariant
ArgListNode::data(int column,
		  int role) const
{
  if ( role == Qt::DisplayRole ) {
    if ( column == 0 ) {
      return "Argument List";
    }
    else if ( column == 1 ) {
      return "";
    }
  }
  return QVariant();
}
    
// @brief 対象のファイル上での位置を返す．
FileRegion
ArgListNode::loc() const
{
  return FileRegion();
}

// @brief 子供の配列を作る．
void
ArgListNode::expand() const
{
  ymuint32 n = mStmt->arg_num();
  mChildren.resize(n);
  for (ymuint32 i = 0; i < n; ++ i) {
    mChildren[i] = new ExprNode("Argument", mStmt->arg(i));
  }
}


//////////////////////////////////////////////////////////////////////
// クラス CaseItemListNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] stmt 親の case 文
CaseItemListNode::CaseItemListNode(const PtStmt* stmt) :
  mStmt(stmt)
{
}

// @brief デストラクタ
CaseItemListNode::~CaseItemListNode()
{
}

// @brief データを返す．
// @param[in] column コラム番号
// @param[in] role 
QVariant
CaseItemListNode::data(int column,
		       int role) const
{
  if ( role == Qt::DisplayRole ) {
    if ( column == 0 ) {
      return "Case Item List";
    }
    else if ( column == 1 ) {
      return "";
    }
  }
  return QVariant();
}
    
// @brief 対象のファイル上での位置を返す．
FileRegion
CaseItemListNode::loc() const
{
  return FileRegion();
}

// @brief 子供の配列を作る．
void
CaseItemListNode::expand() const
{
  ymuint32 n = mStmt->caseitem_num();
  mChildren.resize(n);
  for (ymuint32 i = 0; i < n; ++ i) {
    mChildren[i] = new CaseItemNode(mStmt->caseitem(i));
  }
}


//////////////////////////////////////////////////////////////////////
// クラス CaseItemNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] caseitem caseitem 文
CaseItemNode::CaseItemNode(const PtCaseItem* caseitem) :
  mCaseItem(caseitem)
{
}

// @brief デストラクタ
CaseItemNode::~CaseItemNode()
{
}

// @brief データを返す．
// @param[in] column コラム番号
// @param[in] role 
QVariant
CaseItemNode::data(int column,
		   int role) const
{
  if ( role == Qt::DisplayRole ) {
    if ( column == 0 ) {
      return "Case Item";
    }
    else if ( column == 1 ) {
      return "";
    }
  }
  return QVariant();
}
    
// @brief 対象のファイル上での位置を返す．
FileRegion
CaseItemNode::loc() const
{
  return mCaseItem->file_region();
}

// @brief 子供の配列を作る．
void
CaseItemNode::expand() const
{
  ymuint32 n = mCaseItem->label_num();
  ymuint32 m = n + 1;
  if ( n == 0 ) {
    m = 2;
  }
  mChildren.reserve(m);
  for (ymuint32 i = 0; i < n; ++ i) {
    mChildren.push_back( new ExprNode("Label", mCaseItem->label(i)) );
  }
  if ( n == 0 ) {
    mChildren.push_back( new StrNode("Label", "Default") );
  }
  mChildren.push_back( new StmtNode("Body", mCaseItem->body()) );
}

END_NAMESPACE_YM_VERILOG
