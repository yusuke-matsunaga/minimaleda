
/// @file libym_verilog/tests/vlview/PtNode_expr.cc
/// @brief PtNode の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: PtNode_expr.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2009 Yusuke Matsunaga
/// All rights reserved.


#include "PtNode_expr.h"
#include "PtNode_misc.h"
#include "ym_verilog/pt/PtExpr.h"
#include "ym_verilog/pt/PtArray.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス ExprListNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] label ラベル
// @param[in] expr_array 式の配列
ExprListNode::ExprListNode(const QString& label,
			   const PtExprArray& expr_array) :
  mLabel(label),
  mExprArray(expr_array)
{
}

// @brief デストラクタ
ExprListNode::~ExprListNode()
{
}

// @brief データを返す．
// @param[in] column コラム番号
// @param[in] role 
QVariant
ExprListNode::data(int column,
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
ExprListNode::loc() const
{
  return FileRegion();
}

// @brief 子供の配列を作る．
void
ExprListNode::expand() const
{
  ymuint32 n = mExprArray.size();
  mChildren.resize(n);
  for (ymuint32 i = 0; i < n; ++ i) {
    mChildren[i] = new ExprNode(mLabel, mExprArray[i]);
  }
}


//////////////////////////////////////////////////////////////////////
// クラス ExprNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] label ラベル
// @param[in] expr 式
ExprNode::ExprNode(const QString& label,
		   const PtExpr* expr) :
  mLabel(label),
  mExpr(expr)
{
}

// @brief デストラクタ
ExprNode::~ExprNode()
{
}

// @brief データを返す．
// @param[in] column コラム番号
// @param[in] role 
QVariant
ExprNode::data(int column,
	       int role) const
{
  if ( role == Qt::DisplayRole ) {
    if ( column == 0 ) {
      return mLabel;
    }
    else if ( column == 1 ) {
      switch ( mExpr->type() ) {
      case kPtOprExpr:         return "Operation";
      case kPtConstExpr:       return "Constant";
      case kPtFuncCallExpr:    return "Function Call";
      case kPtSysFuncCallExpr: return "System Function Call";
      case kPtPrimaryExpr:     return "Primary";
      }
    }
  }
  return QVariant();
}

BEGIN_NONAMESPACE

QString
int2str(ymuint32 val)
{
  ostringstream buf;
  buf << val;
  return buf.str().c_str();
}

QString
double2str(double val)
{
  ostringstream buf;
  buf << val;
  return buf.str().c_str();
}

END_NONAMESPACE

// @brief 対象のファイル上での位置を返す．
FileRegion
ExprNode::loc() const
{
  return mExpr->file_region();
}

// @brief 子供の配列を作る．
void
ExprNode::expand() const
{
  ymuint32 n = 0;
  switch ( mExpr->type() ) {
  case kPtOprExpr:
    n += 1;
    n += mExpr->operand_num();
    break;

  case kPtConstExpr:
    n += 3;
    break;

  case kPtFuncCallExpr:
  case kPtSysFuncCallExpr:
    n += 1;
    if ( mExpr->namebranch_array().size() > 0 ) {
      n += 1;
    }
    n += mExpr->operand_num();
    break;

  case kPtPrimaryExpr:
    n +=1 ;
    if ( mExpr->namebranch_array().size() > 0 ) {
      ++ n;
    }
    n += mExpr->index_num();
    if ( mExpr->left_range() ) {
      n += 3;
    }
    break;
  }

  mChildren.reserve(n);
  
  switch ( mExpr->type() ) {
  case kPtOprExpr:
    mChildren.push_back( new OpTypeNode( mExpr->opr_type() ) );
    for (ymuint32 i = 0; i < mExpr->operand_num(); ++ i) {
      mChildren.push_back( new ExprNode("Operand", mExpr->operand(i)) );
    }
    break;

  case kPtConstExpr:
    mChildren.push_back( new ConstTypeNode( mExpr->const_type() ) );
    mChildren.push_back( new StrNode( "Const Size",
				      int2str(mExpr->const_size()) ) );

    switch ( mExpr->const_type() ) {
    case kVpiIntConst:
      if ( mExpr->const_str() == NULL ) {
	mChildren.push_back( new StrNode( "Const Uint",
					  int2str(mExpr->const_uint()) ) );
      }
      else {
	mChildren.push_back( new StrNode( "Const Str",
					  mExpr->const_str() ) ) ;
      }
      break;

    case kVpiBinaryConst:
    case kVpiOctConst:
    case kVpiDecConst:
    case kVpiHexConst:
    case kVpiSignedBinaryConst:
    case kVpiSignedOctConst:
    case kVpiSignedDecConst:
    case kVpiSignedHexConst:
    case kVpiStringConst:
      mChildren.push_back( new StrNode( "Const Str",
					mExpr->const_str() ) ) ;
      break;

    case kVpiRealConst:
      mChildren.push_back( new StrNode( "Const Readl",
					double2str(mExpr->const_real()) ) );
      break;
    }
    break;

  case kPtFuncCallExpr:
  case kPtSysFuncCallExpr:
    if ( mExpr->namebranch_array().size() > 0 ) {
      mChildren.push_back( new NameBranchListNode( mExpr->namebranch_array() ) );
    }
    mChildren.push_back( new StrNode( "Name", mExpr->name() ) );
    for (ymuint32 i = 0; i < mExpr->operand_num(); ++ i) {
      mChildren.push_back( new ExprNode("Argument", mExpr->operand(i)) );
    }
    break;

  case kPtPrimaryExpr:
    if ( mExpr->namebranch_array().size() > 0 ) {
      mChildren.push_back( new NameBranchListNode( mExpr->namebranch_array() ) );
    }
    mChildren.push_back( new StrNode( "Name", mExpr->name() ) );
    for (ymuint32 i = 0; i < mExpr->index_num(); ++ i) {
      mChildren.push_back ( new ExprNode("Index", mExpr->index(i)) );
    }
    if ( mExpr->left_range() ) {
      assert_cond( mExpr->right_range(), __FILE__, __LINE__);
      mChildren.push_back( new RangeModeNode( mExpr->range_mode() ) );
      mChildren.push_back( new ExprNode( "Left Range", mExpr->left_range() ) );
      mChildren.push_back( new ExprNode( "Right Range", mExpr->right_range() ) );
    }
    break;
  }
}

END_NAMESPACE_YM_VERILOG
