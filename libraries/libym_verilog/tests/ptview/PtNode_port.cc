
/// @file libym_verilog/tests/ptview/PtNode_port.cc
/// @brief PtNode の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: PtNode_port.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2009 Yusuke Matsunaga
/// All rights reserved.


#include "PtNode_port.h"
#include "PtNode_expr.h"
#include "PtNode_misc.h"
#include "ym_verilog/pt/PtPort.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス PortNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] port ポートを表すパース木
PortNode::PortNode(const PtPort* port) :
  mPort(port)
{
}

// @brief デストラクタ
PortNode::~PortNode()
{
}

// @brief データを返す．
// @param[in] column コラム番号
// @param[in] role 
QVariant
PortNode::data(int column,
	       int role) const
{
  if ( role == Qt::DisplayRole ) {
    if ( column == 0 ) {
      return "Port";
    }
    else if ( column == 1 ) {
      if ( mPort->ext_name() ) {
	return mPort->ext_name();
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
PortNode::loc() const
{
  return mPort->file_region();
}

// @brief 子供の配列を作る．
void
PortNode::expand() const
{
  ymuint32 n = mPort->portref_num();
  mChildren.resize(n);
  for (ymuint32 i = 0; i < n; ++ i) {
    mChildren[i] = new PortRefNode(mPort->portref(i));
  }
}


//////////////////////////////////////////////////////////////////////
// クラス PortRefNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] portref ポート参照式を表すパース木
PortRefNode::PortRefNode(const PtPortRef* portref) :
  mPortRef(portref)
{
}

// @brief デストラクタ
PortRefNode::~PortRefNode()
{
}

// @brief データを返す．
// @param[in] column コラム番号
// @param[in] role 
QVariant
PortRefNode::data(int column,
		  int role) const
{
  if ( role == Qt::DisplayRole ) {
    if ( column == 0 ) {
      return "PortRef";
    }
    else if ( column == 1 ) {
      return mPortRef->name();
    }
  }
  return QVariant();
}
    
// @brief 対象のファイル上での位置を返す．
FileRegion
PortRefNode::loc() const
{
  return mPortRef->file_region();
}

// @brief 子供の配列を作る．
void
PortRefNode::expand() const
{
  if ( mPortRef->index() ) {
    mChildren.resize(2);
    mChildren[1] = new ExprNode("Index", mPortRef->index());
  }
  else if ( mPortRef->left_range() ) {
    mChildren.resize(4);
    mChildren[1] = new RangeModeNode(mPortRef->range_mode());
    mChildren[2] = new ExprNode("Left Range", mPortRef->left_range());
    mChildren[3] = new ExprNode("Right Range", mPortRef->right_range());
  }
  else {
    mChildren.resize(1);
  }
  mChildren[0] = new DirNode(mPortRef->dir());
}

END_NAMESPACE_YM_VERILOG
