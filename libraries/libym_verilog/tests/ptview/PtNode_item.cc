
/// @file libym_verilog/tests/vlview/PtNode_item.cc
/// @brief PtNode の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: PtNode_item.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2009 Yusuke Matsunaga
/// All rights reserved.


#include "PtNode_item.h"
#include "PtNode_decl.h"
#include "PtNode_stmt.h"
#include "PtNode_expr.h"
#include "PtNode_misc.h"
#include "ym_verilog/pt/PtItem.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス ItemListNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] label ラベル
// @param[in] item_array item の配列
ItemListNode::ItemListNode(const QString& label,
			   const PtItemArray& item_array) :
  mLabel(label),
  mItemArray(item_array)
{
}

// @brief デストラクタ
ItemListNode::~ItemListNode()
{
}

// @brief データを返す．
// @param[in] column コラム番号
// @param[in] role 
QVariant
ItemListNode::data(int column,
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
ItemListNode::loc() const
{
  return FileRegion();
}

// @brief 子供の配列を作る．
void
ItemListNode::expand() const
{
   ymuint32 n = mItemArray.size();
   mChildren.resize(n);
  for (ymuint32 i = 0; i < n; ++ i) {
    mChildren[i] = new ItemNode(mItemArray[i]);
  }
}


//////////////////////////////////////////////////////////////////////
// クラス ItemNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] label ラベル
// @param[in] item item
ItemNode::ItemNode(const PtItem* item) :
  mItem(item)
{
}

// @brief デストラクタ
ItemNode::~ItemNode()
{
}

// @brief データを返す．
// @param[in] column コラム番号
// @param[in] role 
QVariant
ItemNode::data(int column,
	       int role) const
{
  if ( role == Qt::DisplayRole ) {
    if ( column == 0 ) {
      switch ( mItem->type() ) {
      case kPtItem_DefParam:         return "DefparamHeader";
      case kPtItem_ContAssign:       return "ContAssignHeader";
      case kPtItem_Initial:          return "Initial";
      case kPtItem_Always:           return "Always";
      case kPtItem_Task:             return "Task";
      case kPtItem_Func:
	switch ( mItem->data_type() ) {
	case kVpiVarNone:            return "Function";
	case kVpiVarInteger:         return "Function(integer)";
	case kVpiVarReal:            return "Function(real)";
	case kVpiVarTime:            return "Function(time)";
	case kVpiVarRealtime:        return "Function(realtime)";
	}
	break;
      case kPtItem_GateInst:         return "GateHeader";
      case kPtItem_MuInst:           return "MuHeader";
      case kPtItem_SpecItem:
	switch ( mItem->specitem_type() ) {
	case kVpiPulsestyleOnEvent:  return "Pulse_onevent";
	case kVpiPulsestyleOnDetect: return "Pulse_ondetect";
	case kVpiShowcancelled:      return "Showcancelled";
	case kVpiNoshowcancelled:    return "Noshowcancelld";
	}
      case kPtItem_SpecPath:         return "SpecPath";
      case kPtItem_Generate:         return "Generate";
      case kPtItem_GenBlock:         return "GenBlock";
      case kPtItem_GenIf:            return "GenIf";
      case kPtItem_GenCase:          return "GenCase";
      case kPtItem_GenFor:           return "GenFor";
      }
    }
    else if ( column == 1 ) {
      switch ( mItem->type() ) {
      case kPtItem_Task:
      case kPtItem_Func:
      case kPtItem_GenFor:
	return mItem->name();
	
      case kPtItem_Generate:
      case kPtItem_GenBlock:
	if ( mItem->name() ) {
	  return mItem->name();
	}
	break;
	
      default:
	break;
      }
    }
  }
  return QVariant();
}
    
// @brief 対象のファイル上での位置を返す．
FileRegion
ItemNode::loc() const
{
  return mItem->file_region();
}

// @brief 子供の配列を作る．
void
ItemNode::expand() const
{
  switch ( mItem->type() ) {
  case kPtItem_DefParam:
    mChildren.resize(mItem->size());
    for (ymuint32 i = 0; i < mItem->size(); ++ i) {
      mChildren[i] = new DefParamNode(mItem->defparam(i));
    }
    break;
    
  case kPtItem_ContAssign:
    {
      ymuint32 n = mItem->size();
      if ( mItem->strength() ) {
	++ n;
      }
      if ( mItem->delay() ) {
	++ n;
      }
      mChildren.resize(n);
      for (ymuint32 i = 0; i < mItem->size(); ++ i) {
	mChildren[i] = new ContAssignNode(mItem->contassign(i));
      }
    }
    break;
    
  case kPtItem_Initial:
  case kPtItem_Always:
    mChildren.resize(1);
    mChildren[0] = new StmtNode("Body", mItem->body());
    break;
    
  case kPtItem_Task:
  case kPtItem_Func:
    {
      ymuint32 n = 2;
      if ( mItem->left_range() ) {
	assert_cond(mItem->right_range(), __FILE__, __LINE__);
	n += 3;
      }
      if ( mItem->iohead_array().size() > 0 ) {
	++ n;
      }
      if ( mItem->paramhead_array().size() > 0 ) {
	++ n;
      }
      if ( mItem->localparamhead_array().size() > 0 ) {
	++ n;
      }
      if ( mItem->declhead_array().size() > 0 ) {
	++ n;
      }
      mChildren.reserve(n);
      mChildren.push_back( new BoolNode("Automatic", mItem->automatic()) );
      if ( mItem->left_range() ) {
	mChildren.push_back( new BoolNode("Signed", mItem->is_signed()) );
	mChildren.push_back( new ExprNode("Left Range",
					  mItem->left_range()) );
	mChildren.push_back( new ExprNode("Right Range",
					  mItem->right_range()) );
      }
      if ( mItem->iohead_array().size() > 0 ) {
	mChildren.push_back( new IOHeadListNode(mItem->iohead_array()) );
      }
      if ( mItem->paramhead_array().size() > 0 ) {
	mChildren.push_back( new DeclHeadListNode("Parameter List",
						  mItem->paramhead_array()) );
      }
      if ( mItem->localparamhead_array().size() > 0 ) {
	mChildren.push_back( new DeclHeadListNode("Localparam List",
						  mItem->localparamhead_array()) );
      }
      if ( mItem->declhead_array().size() > 0 ) {
	mChildren.push_back( new DeclHeadListNode("Decl List",
						  mItem->declhead_array()) );
      }
      mChildren.push_back( new StmtNode("Body", mItem->body()) );
    }
    break;
    
  case kPtItem_GateInst:
    {
      ymuint32 n = 1;
      if ( mItem->strength() ) {
	++ n;
      }
      if ( mItem->delay() ) {
	++ n;
      }
      mChildren.reserve(n);
      mChildren.push_back( new PrimTypeNode(mItem->prim_type()) );
      if ( mItem->strength() ) {
	mChildren.push_back( new StrengthNode(mItem->strength()) );
      }
      if ( mItem->delay() ) {
	mChildren.push_back( new DelayNode(mItem->delay()) );
      }
      mChildren.push_back( new InstListNode(mItem) );
    }
    break;
    
  case kPtItem_MuInst:
    {
      ymuint32 n = 1;
      if ( mItem->paramassign_array().size() > 0 ) {
	++ n;
      }
      if ( mItem->strength() ) {
	++ n;
      }
      if ( mItem->delay() ) {
	++ n;
      }
      mChildren.reserve(n);
      mChildren.push_back( new StrNode("DefName", mItem->name()) );
      if ( mItem->paramassign_array().size() > 0 ) {
	mChildren.push_back( new ConnectionListNode(mItem->paramassign_array()) );
      }
      if ( mItem->strength() ) {
	mChildren.push_back( new StrengthNode(mItem->strength()) );
      }
      if ( mItem->delay() ) {
	mChildren.push_back( new DelayNode(mItem->delay()) );
      }
      mChildren.push_back( new InstListNode(mItem) );
    }
    break;
    
  case kPtItem_SpecItem:
    switch ( mItem->specitem_type() ) {
    case kVpiPulsestyleOnEvent:
      break;
      
    case kVpiPulsestyleOnDetect:
      break;
      
    case kVpiShowcancelled:
      break;
      
    case kVpiNoshowcancelled:
      break;
    }
    break;
    
  case kPtItem_SpecPath:
    break;
    
  case kPtItem_Generate:
  case kPtItem_GenBlock:
    {
      ymuint32 n = 0;
      if ( mItem->declhead_array().size() > 0 ) {
	++ n;
      }
      if ( mItem->item_array().size() > 0 ) {
	++ n;
      }
      mChildren.reserve(n);
      if ( mItem->declhead_array().size() > 0 ) {
	mChildren.push_back( new DeclHeadListNode("Decl List",
						  mItem->declhead_array()) );
      }
      if ( mItem->item_array().size() > 0 ) {
	mChildren.push_back( new ItemListNode("Item List",
					      mItem->item_array()) );
      }
    }
    break;
    
  case kPtItem_GenIf:
    {
      ymuint32 n = 1;
      if ( mItem->then_declhead_array().size() > 0 ) {
	++ n;
      }
      if ( mItem->then_item_array().size() > 0 ) {
	++ n;
      }
      if ( mItem->else_declhead_array().size() > 0 ) {
	++ n;
      }
      if ( mItem->else_item_array().size() > 0 ) {
	++ n;
      }
      mChildren.reserve(n);
      mChildren.push_back( new ExprNode("Condition", mItem->expr()) );
      if ( mItem->then_declhead_array().size() > 0 ) {
	mChildren.push_back( new DeclHeadListNode("Then Decl",
						  mItem->then_declhead_array()) );
      }
      if ( mItem->then_item_array().size() > 0 ) {
	mChildren.push_back( new ItemListNode("Then Item",
					      mItem->then_item_array()) );
      }
      if ( mItem->else_declhead_array().size() > 0 ) {
	mChildren.push_back( new DeclHeadListNode("Else Decl",
						  mItem->else_declhead_array()) );
      }
      if ( mItem->else_item_array().size() > 0 ) {
	mChildren.push_back( new ItemListNode("Else Item",
					      mItem->else_item_array()) );
      }
    }
    break;
    
  case kPtItem_GenCase:
    mChildren.resize(2);
    mChildren[0] = new ExprNode("Condition", mItem->expr());
    mChildren[1] = new GenCaseItemListNode(mItem);
    break;
    
  case kPtItem_GenFor:
    {
      ymuint32 n = 4;
      if ( mItem->declhead_array().size() > 0 ) {
	++ n;
      }
      if ( mItem->item_array().size() > 0 ) {
	++ n;
      }
      mChildren.reserve(n);
      mChildren.push_back( new StrNode("Loop Var", mItem->loop_var()) );
      mChildren.push_back( new ExprNode("Initial Expr", mItem->init_expr()) );
      mChildren.push_back( new ExprNode("Condition", mItem->expr()) );
      mChildren.push_back( new ExprNode("Increment Expr", mItem->next_expr()) );
      if ( mItem->declhead_array().size() > 0 ) {
	mChildren.push_back( new DeclHeadListNode("Body Decl List",
						  mItem->declhead_array()) );
      }
      if ( mItem->item_array().size() > 0 ) {
	mChildren.push_back( new ItemListNode("Body Item List",
					      mItem->item_array()) );
      }
    }
    break;
  }
}


//////////////////////////////////////////////////////////////////////
// クラス DefParamNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] defparam
DefParamNode::DefParamNode(const PtDefParam* defparam) :
  mDefParam(defparam)
{
}

// @brief デストラクタ
DefParamNode::~DefParamNode()
{
}

// @brief データを返す．
// @param[in] column コラム番号
// @param[in] role 
QVariant
DefParamNode::data(int column,
		   int role) const
{
  if ( role == Qt::DisplayRole ) {
    if ( column == 0 ) {
      return "DefParam";
    }
    else if ( column == 1 ) {
      return "";
    }
  }
  return QVariant();
}
    
// @brief 対象のファイル上での位置を返す．
FileRegion
DefParamNode::loc() const
{
  return mDefParam->file_region();
}

// @brief 子供の配列を作る．
void
DefParamNode::expand() const
{
  ymuint32 n = 2;
  if ( mDefParam->namebranch_array().size() > 0 ) {
    ++ n;
  }
  mChildren.reserve(n);
  if ( mDefParam->namebranch_array().size() > 0 ) {
    mChildren.push_back( new NameBranchListNode(mDefParam->namebranch_array()) );
  }
  mChildren.push_back( new StrNode("Name", mDefParam->name()) );
  mChildren.push_back( new ExprNode("Value", mDefParam->expr()) );
}


//////////////////////////////////////////////////////////////////////
// クラス ContAssignNdoe
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] cont_assign
ContAssignNode::ContAssignNode(const PtContAssign* cont_assign) :
  mContAssign(cont_assign)
{
}

// @brief デストラクタ
ContAssignNode::~ContAssignNode()
{
}

// @brief データを返す．
// @param[in] column コラム番号
// @param[in] role 
QVariant
ContAssignNode::data(int column,
		     int role) const
{
  if ( role == Qt::DisplayRole ) {
    if ( column == 0 ) {
      return "Continuous Assign";
    }
    else if ( column == 1 ) {
      return "";
    }
  }
  return QVariant();
}
    
// @brief 対象のファイル上での位置を返す．
FileRegion
ContAssignNode::loc() const
{
  return mContAssign->file_region();
}

// @brief 子供の配列を作る．
void
ContAssignNode::expand() const
{
  mChildren.resize(2);
  mChildren[0] = new ExprNode("Lhs", mContAssign->lhs());
  mChildren[1] = new ExprNode("Rhs", mContAssign->rhs());
}


//////////////////////////////////////////////////////////////////////
// クラス InstListNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] item 親の item
InstListNode::InstListNode(const PtItem* item) :
  mItem(item)
{
}

// @brief デストラクタ
InstListNode::~InstListNode()
{
}

// @brief データを返す．
// @param[in] column コラム番号
// @param[in] role 
QVariant
InstListNode::data(int column,
		   int role) const
{
  if ( role == Qt::DisplayRole ) {
    if ( column == 0 ) {
      return "Instance List";
    }
    else if ( column == 1 ) {
      return "";
    }
  }
  return QVariant();
}
    
// @brief 対象のファイル上での位置を返す．
FileRegion
InstListNode::loc() const
{
  return FileRegion();
}

// @brief 子供の配列を作る．
void
InstListNode::expand() const
{
   ymuint32 n = mItem->size();
   mChildren.resize(n);
  for (ymuint32 i = 0; i < n; ++ i) {
    mChildren[i] = new InstNode(mItem->inst(i));
  }
}


//////////////////////////////////////////////////////////////////////
// クラス InstNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] inst
InstNode::InstNode(const PtInst* inst) :
  mInst(inst)
{
}

// @brief デストラクタ
InstNode::~InstNode()
{
}

// @brief データを返す．
// @param[in] column コラム番号
// @param[in] role 
QVariant
InstNode::data(int column,
	       int role) const
{
  if ( role == Qt::DisplayRole ) {
    if ( column == 0 ) {
      return "Instance";
    }
    else if ( column == 1 ) {
      if ( mInst->name() ) {
	return mInst->name();
      }
      else {
	return "";
      }
    }
  }
  return QVariant();
}
    
// @brief 対象のファイル上での位置を返す．
FileRegion
InstNode::loc() const
{
  return mInst->file_region();
}

// @brief 子供の配列を作る．
void
InstNode::expand() const
{
  ymuint32 n = 1;
  if ( mInst->left_range() ) {
    assert_cond(mInst->right_range(), __FILE__, __LINE__);
    n += 2;
  }
  mChildren.reserve(n);
  if ( mInst->left_range() ) {
    mChildren.push_back( new ExprNode("Left Range", mInst->left_range()) );
    mChildren.push_back( new ExprNode("Right Range", mInst->right_range()) );
  }
  mChildren.push_back( new InstPortListNode(mInst) );
}


//////////////////////////////////////////////////////////////////////
// クラス InstPortListNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] inst
InstPortListNode::InstPortListNode(const PtInst* inst) :
  mInst(inst)
{
}

// @brief デストラクタ
InstPortListNode::~InstPortListNode()
{
}

// @brief データを返す．
// @param[in] column コラム番号
// @param[in] role 
QVariant
InstPortListNode::data(int column,
		       int role) const
{
  if ( role == Qt::DisplayRole ) {
    if ( column == 0 ) {
      return "Instance Port List";
    }
    else if ( column == 1 ) {
      return "";
    }
  }
  return QVariant();
}
    
// @brief 対象のファイル上での位置を返す．
FileRegion
InstPortListNode::loc() const
{
  return FileRegion();
}

// @brief 子供の配列を作る．
void
InstPortListNode::expand() const
{
  ymuint32 n = mInst->port_num();
  mChildren.resize(n);
  for (ymuint32 i = 0; i < n; ++ i) {
    mChildren[i] = new ConnectionNode(mInst->port(i));
  }
}


//////////////////////////////////////////////////////////////////////
// クラス GenCaseItemListNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] item 親の generate case 
GenCaseItemListNode::GenCaseItemListNode(const PtItem* item) :
  mItem(item)
{
}

// @brief デストラクタ
GenCaseItemListNode::~GenCaseItemListNode()
{
}

// @brief データを返す．
// @param[in] column コラム番号
// @param[in] role 
QVariant
GenCaseItemListNode::data(int column,
			  int role) const
{
  if ( role == Qt::DisplayRole ) {
    if ( column == 0 ) {
      return "Generate Case Item List";
    }
    else if ( column == 1 ) {
      return "";
    }
  }
  return QVariant();
}
    
// @brief 対象のファイル上での位置を返す．
FileRegion
GenCaseItemListNode::loc() const
{
  return FileRegion();
}

// @brief 子供の配列を作る．
void
GenCaseItemListNode::expand() const
{
  ymuint32 n = mItem->size();
  mChildren.resize(n);
  for (ymuint32 i = 0; i < n; ++ i) {
    mChildren[i] = new GenCaseItemNode(mItem->caseitem(i));
  }
}


//////////////////////////////////////////////////////////////////////
// クラス GenCaseItemNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] item generate case item
GenCaseItemNode::GenCaseItemNode(const PtGenCaseItem* item) :
  mItem(item)
{
}

// @brief デストラクタ
GenCaseItemNode::~GenCaseItemNode()
{
}

// @brief データを返す．
// @param[in] column コラム番号
// @param[in] role 
QVariant
GenCaseItemNode::data(int column,
		      int role) const
{
  if ( role == Qt::DisplayRole ) {
    if ( column == 0 ) {
      return "Generate Case Item";
    }
    else if ( column == 1 ) {
      return "";
    }
  }
  return QVariant();
}
    
// @brief 対象のファイル上での位置を返す．
FileRegion
GenCaseItemNode::loc() const
{
  return mItem->file_region();
}

// @brief 子供の配列を作る．
void
GenCaseItemNode::expand() const
{
  ymuint32 n = mItem->label_num();
  if ( n == 0 ) {
    n = 1;
  }
  if ( mItem->declhead_array().size() > 0 ) {
    ++ n;
  }
  if ( mItem->item_array().size() > 0 ) {
    ++ n;
  }
  mChildren.reserve(n);
  n = mItem->label_num();
  for (ymuint32 i = 0; i < n; ++ i) {
    mChildren.push_back( new ExprNode("Label", mItem->label(i)) );
  }
  if ( n == 0 ) {
    mChildren.push_back( new StrNode("label", "Default") );
  }
  if ( mItem->declhead_array().size() > 0 ) {
    mChildren.push_back( new DeclHeadListNode("Decl Head List",
					      mItem->declhead_array()) );
  }
  if ( mItem->item_array().size() > 0 ) {
    mChildren.push_back( new ItemListNode("Item List",
					  mItem->item_array()) );
  }
}

END_NAMESPACE_YM_VERILOG
