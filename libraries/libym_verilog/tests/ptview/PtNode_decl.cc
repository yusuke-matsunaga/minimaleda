
/// @file libym_verilog/tests/vlview/PtNode_decl.cc
/// @brief PtNode の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: PtNode_decl.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2009 Yusuke Matsunaga
/// All rights reserved.


#include "PtNode_decl.h"
#include "PtNode_expr.h"
#include "PtNode_misc.h"
#include "ym_verilog/pt/PtDecl.h"
#include "ym_verilog/pt/PtArray.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス IOHeadListNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] iohead_array IO宣言ヘッダの配列
IOHeadListNode::IOHeadListNode(const PtIOHeadArray& iohead_array) :
  mIOHeadArray(iohead_array)
{
}

// @brief デストラクタ
IOHeadListNode::~IOHeadListNode()
{
}

// @brief データを返す．
// @param[in] column コラム番号
// @param[in] role 
QVariant
IOHeadListNode::data(int column,
		     int role) const
{
  if ( role == Qt::DisplayRole ) {
    if ( column == 0 ) {
      return "IO Header List";
    }
    else if ( column == 1 ) {
      return "";
    }
  }
  return QVariant();
}
    
// @brief 対象のファイル上での位置を返す．
FileRegion
IOHeadListNode::loc() const
{
  return FileRegion();
}

// @brief 子供の配列を作る．
void
IOHeadListNode::expand() const
{
   ymuint32 n = mIOHeadArray.size();
   mChildren.resize(n);
  for (ymuint32 i = 0; i < n; ++ i) {
    mChildren[i] = new IOHeadNode(mIOHeadArray[i]);
  }
}


//////////////////////////////////////////////////////////////////////
// クラス IOHeadNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] expr 式
IOHeadNode::IOHeadNode(const PtIOHead* iohead) :
  mIOHead(iohead)
{
}

// @brief デストラクタ
IOHeadNode::~IOHeadNode()
{
}

// @brief データを返す．
// @param[in] column コラム番号
// @param[in] role 
QVariant
IOHeadNode::data(int column,
		 int role) const
{
  if ( role == Qt::DisplayRole ) {
    if ( column == 0 ) {
      switch ( mIOHead->type() ) {
      case kPtIO_Input:  return "Input";
      case kPtIO_Output: return "Output";
      case kPtIO_Inout:  return "Inout";
      }
    }
    else if ( column == 1 ) {
      return "";
    }
  }
  return QVariant();
}
    
// @brief 対象のファイル上での位置を返す．
FileRegion
IOHeadNode::loc() const
{
  return mIOHead->file_region();
}

// @brief 子供の配列を作る．
void
IOHeadNode::expand() const
{
  if ( mIOHead->aux_type() != kVpiAuxNone ) {
    mChildren.push_back( new AuxTypeNode(mIOHead->aux_type(),
					 mIOHead->net_type(),
					 mIOHead->var_type()) );
  }
  mChildren.push_back( new BoolNode("Signed", mIOHead->is_signed()) );
  if ( mIOHead->left_range() ) {
    mChildren.push_back( new ExprNode("Left Range", mIOHead->left_range()) );
    mChildren.push_back( new ExprNode("Right Range", mIOHead->right_range()) );
  }
  mChildren.push_back( new IOItemListNode(mIOHead) );
}


//////////////////////////////////////////////////////////////////////
// クラス IOItemListNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] iohead IO宣言ヘッダ
IOItemListNode::IOItemListNode(const PtIOHead* iohead) :
  mIOHead(iohead)
{
}

// @brief デストラクタ
IOItemListNode::~IOItemListNode()
{
}

// @brief データを返す．
// @param[in] column コラム番号
// @param[in] role 
QVariant
IOItemListNode::data(int column,
		     int role) const
{
  if ( role == Qt::DisplayRole ) {
    if ( column == 0 ) {
      return "IO Item List";
    }
    else if ( column == 1 ) {
      return "";
    }
  }
  return QVariant();
}
    
// @brief 対象のファイル上での位置を返す．
FileRegion
IOItemListNode::loc() const
{
  return FileRegion();
}

// @brief 子供の配列を作る．
void
IOItemListNode::expand() const
{
  ymuint32 n = mIOHead->item_num();
  mChildren.resize(n);
  for (ymuint32 i = 0; i < n; ++ i) {
    mChildren[i] = new IOItemNode(mIOHead->item(i));
  }
}


//////////////////////////////////////////////////////////////////////
// クラス IOItemNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] expr 式
IOItemNode::IOItemNode(const PtIOItem* item) :
  mIOItem(item)
{
}

// @brief デストラクタ
IOItemNode::~IOItemNode()
{
}

// @brief データを返す．
// @param[in] column コラム番号
// @param[in] role 
QVariant
IOItemNode::data(int column,
		 int role) const
{
  if ( role == Qt::DisplayRole ) {
    if ( column == 0 ) {
      return "IO Item";
    }
    else if ( column == 1 ) {
      return mIOItem->name();
    }
  }
  return QVariant();
}
    
// @brief 対象のファイル上での位置を返す．
FileRegion
IOItemNode::loc() const
{
  return mIOItem->file_region();
}

// @brief 子供の配列を作る．
void
IOItemNode::expand() const
{
}


//////////////////////////////////////////////////////////////////////
// クラス DeclHeadListNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] label ラベル
// @param[in] declhead_array 宣言ヘッダの配列
DeclHeadListNode::DeclHeadListNode(const QString& label,
				   const PtDeclHeadArray& declhead_array) :
  mLabel(label),
  mDeclHeadArray(declhead_array)
{
}

// @brief デストラクタ
DeclHeadListNode::~DeclHeadListNode()
{
}

// @brief データを返す．
// @param[in] column コラム番号
// @param[in] role 
QVariant
DeclHeadListNode::data(int column,
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
DeclHeadListNode::loc() const
{
  return FileRegion();
}

// @brief 子供の配列を作る．
void
DeclHeadListNode::expand() const
{
   ymuint32 n = mDeclHeadArray.size();
   mChildren.resize(n);
  for (ymuint32 i = 0; i < n; ++ i) {
    mChildren[i] = new DeclHeadNode(mDeclHeadArray[i]);
  }
}


//////////////////////////////////////////////////////////////////////
// クラス DeclHeadNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] declhead 宣言ヘッダ
DeclHeadNode::DeclHeadNode(const PtDeclHead* declhead) :
  mDeclHead(declhead)
{
}

// @brief デストラクタ
DeclHeadNode::~DeclHeadNode()
{
}

// @brief データを返す．
// @param[in] column コラム番号
// @param[in] role 
QVariant
DeclHeadNode::data(int column,
		   int role) const
{
  if ( role == Qt::DisplayRole ) {
    if ( column == 0 ) {
      switch ( mDeclHead->type() ) {
      case kPtDecl_Param:
	switch ( mDeclHead->data_type() ) {
	case kVpiVarNone:        return "Parameter";
	case kVpiVarInteger:     return "Parameter(integer)"; 
	case kVpiVarReal:        return "Parameter(real)";
	case kVpiVarTime:        return "Parameter(time)";
	case kVpiVarRealtime:    return "Parameter(realtime)";
	}
	break;
	
      case kPtDecl_LocalParam:
	switch ( mDeclHead->data_type() ) {
	case kVpiVarNone:        return "Localparam";
	case kVpiVarInteger:     return "Localparam(integer)"; 
	case kVpiVarReal:        return "Localparam(real)";
	case kVpiVarTime:        return "Localparam(time)";
	case kVpiVarRealtime:    return "Localparam(realtime)";
	}
	break;
	
      case kPtDecl_Reg:          return "Reg";
      case kPtDecl_Var:
	switch ( mDeclHead->data_type() ) {
	case kVpiVarInteger:     return "Integer";
	case kVpiVarReal:        return "Real";   
	case kVpiVarTime:        return "Time";
	case kVpiVarRealtime:    return "Realtime";
	case kVpiVarNone:
	  assert_not_reached(__FILE__, __LINE__);
	}
	break;
	
      case kPtDecl_Genvar:       return "Genvar";    
      case kPtDecl_Event:        return "Event";     
      case kPtDecl_SpecParam:    return "Specparam"; 
      case kPtDecl_Net:
	switch ( mDeclHead->net_type() ) {
	case kVpiSupply0:        return "Supply0";
	case kVpiSupply1:        return "Supply1";
	case kVpiTri:            return "Tri";
	case kVpiTriAnd:         return "Triand";
	case kVpiTriOr:          return "Trior";
	case kVpiTriReg:         return "Trireg";
	case kVpiTri0:           return "Tri0";
	case kVpiTri1:           return "Tri1";
	case kVpiWire:           return "Wire";
	case kVpiWand:           return "Wand";
	case kVpiWor:            return "Wor";
	case kVpiNone:           return "None";
	}
	break;
      }
    }
    else if ( column == 1 ) {
      return "";
    }
  }
  return QVariant();
}
    
// @brief 対象のファイル上での位置を返す．
FileRegion
DeclHeadNode::loc() const
{
  return mDeclHead->file_region();
}

// @brief 子供の配列を作る．
void
DeclHeadNode::expand() const
{
  mChildren.push_back( new BoolNode("Signed", mDeclHead->is_signed()) );
  if ( mDeclHead->left_range() ) {
    mChildren.push_back( new ExprNode("Left Range", mDeclHead->left_range()) );
    mChildren.push_back( new ExprNode("Right Range", mDeclHead->right_range()) );
  }
  if ( mDeclHead->vs_type() != kVpiVsNone ) {
    mChildren.push_back( new VsNode(mDeclHead->vs_type()) );
  }
  if ( mDeclHead->strength() ) {
    mChildren.push_back( new StrengthNode(mDeclHead->strength()) );
  }
  if ( mDeclHead->delay() ) {
    mChildren.push_back( new DelayNode( mDeclHead->delay() ) );
  }
  mChildren.push_back( new DeclItemListNode(mDeclHead) );
}


//////////////////////////////////////////////////////////////////////
// クラス DeclItemListNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] declhead IO宣言ヘッダ
DeclItemListNode::DeclItemListNode(const PtDeclHead* declhead) :
  mDeclHead(declhead)
{
}

// @brief デストラクタ
DeclItemListNode::~DeclItemListNode()
{
}

// @brief データを返す．
// @param[in] column コラム番号
// @param[in] role 
QVariant
DeclItemListNode::data(int column,
		       int role) const
{
  if ( role == Qt::DisplayRole ) {
    if ( column == 0 ) {
      return "Decl Item List";
    }
    else if ( column == 1 ) {
      return "";
    }
  }
  return QVariant();
}
    
// @brief 対象のファイル上での位置を返す．
FileRegion
DeclItemListNode::loc() const
{
  return FileRegion();
}

// @brief 子供の配列を作る．
void
DeclItemListNode::expand() const
{
  ymuint32 n = mDeclHead->item_num();
  mChildren.resize(n);
  for (ymuint32 i = 0; i < n; ++ i) {
    mChildren[i] = new DeclItemNode(mDeclHead->item(i));
  }
}


//////////////////////////////////////////////////////////////////////
// クラス DeclItemNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] expr 式
DeclItemNode::DeclItemNode(const PtDeclItem* item) :
  mDeclItem(item)
{
}

// @brief デストラクタ
DeclItemNode::~DeclItemNode()
{
}

// @brief データを返す．
// @param[in] column コラム番号
// @param[in] role 
QVariant
DeclItemNode::data(int column,
		 int role) const
{
  if ( role == Qt::DisplayRole ) {
    if ( column == 0 ) {
      return "Decl Item";
    }
    else if ( column == 1 ) {
      return mDeclItem->name();
    }
  }
  return QVariant();
}
    
// @brief 対象のファイル上での位置を返す．
FileRegion
DeclItemNode::loc() const
{
  return mDeclItem->file_region();
}

// @brief 子供の配列を作る．
void
DeclItemNode::expand() const
{
  if ( mDeclItem->dimension_list_size() > 0 ) {
    mChildren.push_back( new RangeListNode(mDeclItem) );
  }
  if ( mDeclItem->init_value() ) {
    mChildren.push_back( new ExprNode("Initial Value", mDeclItem->init_value()) );
  }
}


//////////////////////////////////////////////////////////////////////
// クラス RangeListNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] declitem 親の宣言要素
RangeListNode::RangeListNode(const PtDeclItem* declitem) :
  mDeclItem(declitem)
{
}

// @brief デストラクタ
RangeListNode::~RangeListNode()
{
}

// @brief データを返す．
// @param[in] column コラム番号
// @param[in] role 
QVariant
RangeListNode::data(int column,
		    int role) const
{
  if ( role == Qt::DisplayRole ) {
    if ( column == 0 ) {
      return "Range List";
    }
    else if ( column == 1 ) {
      return "";
    }
  }
  return QVariant();
}
    
// @brief 対象のファイル上での位置を返す．
FileRegion
RangeListNode::loc() const
{
  return FileRegion();
}

// @brief 子供の配列を作る．
void
RangeListNode::expand() const
{
  ymuint32 n = mDeclItem->dimension_list_size();
  mChildren.resize(n);
  for (ymuint32 i = 0; i < n; ++ i) {
    mChildren[i] = new RangeNode(mDeclItem->range(i));
  }
}


//////////////////////////////////////////////////////////////////////
// クラス RangeNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] range 範囲
RangeNode::RangeNode(const PtRange* range) :
  mRange(range)
{
}

// @brief デストラクタ
RangeNode::~RangeNode()
{
}

// @brief データを返す．
// @param[in] column コラム番号
// @param[in] role 
QVariant
RangeNode::data(int column,
		int role) const
{
  if ( role == Qt::DisplayRole ) {
    if ( column == 0 ) {
      return "Range";
    }
    else if ( column == 1 ) {
      return "";
    }
  }
  return QVariant();
}
    
// @brief 対象のファイル上での位置を返す．
FileRegion
RangeNode::loc() const
{
  return mRange->file_region();
}

// @brief 子供の配列を作る．
void
RangeNode::expand() const
{
  mChildren.resize(2);
  mChildren[0] = new ExprNode("Left Range", mRange->left());
  mChildren[1] = new ExprNode("Right Range", mRange->left());
}



END_NAMESPACE_YM_VERILOG
