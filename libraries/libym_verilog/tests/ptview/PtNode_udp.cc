
/// @file libym_verilog/tests/ptview/PtNode_udp.cc
/// @brief PtNode の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: PtNode_udp.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2009 Yusuke Matsunaga
/// All rights reserved.


#include "PtNode_udp.h"
#include "PtNode_port.h"
#include "PtNode_decl.h"
#include "PtNode_expr.h"
#include "PtNode_misc.h"
#include "ym_verilog/pt/PtUdp.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス UdpNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] udp UDP を表すパース木
UdpNode::UdpNode(const PtUdp* udp) :
  mUdp(udp)
{
}

// @brief デストラクタ
UdpNode::~UdpNode()
{
}

// @brief データを返す．
// @param[in] column コラム番号
// @param[in] role 
QVariant
UdpNode::data(int column,
	      int role) const
{
  if ( role == Qt::DisplayRole ) {
    if ( column == 0 ) {
      return "UDP";
    }
    else if ( column == 1 ) {
      return mUdp->name();
    }
  }
  return QVariant();
}

// @brief 対象のファイル上での位置を返す．
FileRegion
UdpNode::loc() const
{
  return mUdp->file_region();
}

// @brief 子供の配列を作る．
void
UdpNode::expand() const
{
  // 要素は
  // primitive type
  // port list
  // iohead list
  // (initial value)
  // table list
  mChildren.clear();
  mChildren.reserve(5);
  
  mChildren.push_back( new PrimTypeNode(mUdp->prim_type()) );
  mChildren.push_back( new UdpPortListNode(mUdp) );
  mChildren.push_back( new IOHeadListNode(mUdp->iohead_array()) );
  if ( mUdp->init_value() ) {
    mChildren.push_back( new ExprNode("initial value", mUdp->init_value()) );
  }
  mChildren.push_back( new UdpEntryListNode(mUdp->table_array()) );
}


//////////////////////////////////////////////////////////////////////
// クラス UdpPortListNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] udp UDP を表すパース木
UdpPortListNode::UdpPortListNode(const PtUdp* udp) :
  mUdp(udp)
{
}

// @brief デストラクタ
UdpPortListNode::~UdpPortListNode()
{
}

// @brief データを返す．
// @param[in] column コラム番号
// @param[in] role 
QVariant
UdpPortListNode::data(int column,
		      int role) const
{
  if ( role == Qt::DisplayRole ) {
    if ( column == 0 ) {
      return "UDP Port List";
    }
    else if ( column == 1 ) {
      return "";
    }
  }
  return QVariant();
}
    
// @brief 対象のファイル上での位置を返す．
FileRegion
UdpPortListNode::loc() const
{
  return FileRegion();
}

// @brief 子供の配列を作る．
void
UdpPortListNode::expand() const
{
  ymuint32 n = mUdp->port_num();
  mChildren.resize(n);
  for (ymuint32 i = 0; i < n; ++ i) {
    mChildren[i] = new PortNode(mUdp->port(i));
  }
}


//////////////////////////////////////////////////////////////////////
// クラス UdpEntryListNode
//////////////////////////////////////////////////////////////////////

/// @brief コンストラクタ
/// @param[in] table_array テーブルの配列
UdpEntryListNode::UdpEntryListNode(const PtUdpEntryArray& table_array) :
  mTableArray(table_array)
{
}

// @brief デストラクタ
UdpEntryListNode::~UdpEntryListNode()
{
}

// @brief データを返す．
// @param[in] column コラム番号
// @param[in] role 
QVariant
UdpEntryListNode::data(int column,
		       int role) const
{
  if ( role == Qt::DisplayRole ) {
    if ( column == 0 ) {
      return "UDP Table Entry List";
    }
    else if ( column == 1 ) {
      return "";
    }
  }
  return QVariant();
}
    
// @brief 対象のファイル上での位置を返す．
FileRegion
UdpEntryListNode::loc() const
{
  return FileRegion();
}

// @brief 子供の配列を作る．
void
UdpEntryListNode::expand() const
{
  mChildren.resize(mTableArray.size());
  for (ymuint32 i = 0; i < mTableArray.size(); ++ i) {
    mChildren[i] = new UdpEntryNode(mTableArray[i]);
  }
}


//////////////////////////////////////////////////////////////////////
// クラス UdpEntryNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] table テーブルの1行分
UdpEntryNode::UdpEntryNode(const PtUdpEntry* table) :
  mTable(table)
{
}

// @brief デストラクタ
UdpEntryNode::~UdpEntryNode()
{
}

// @brief データを返す．
// @param[in] column コラム番号
// @param[in] role 
QVariant
UdpEntryNode::data(int column,
		   int role) const
{
  if ( role == Qt::DisplayRole ) {
    if ( column == 0 ) {
      return "UDP Table Entry";
    }
    else if ( column == 1 ) {
      return "";
    }
  }
  return QVariant();
}
    
// @brief 対象のファイル上での位置を返す．
FileRegion
UdpEntryNode::loc() const
{
  return mTable->file_region();
}

// @brief 子供の配列を作る．
void
UdpEntryNode::expand() const
{
  ymuint32 n = mTable->input_array().size();
  if ( mTable->current() ) {
    ++ n;
  }
  ++ n;
  mChildren.reserve(n);
  for (ymuint32 i = 0; i < mTable->input_array().size(); ++ i) {
    mChildren.push_back( new UdpValNode("Input", mTable->input_array()[i]) );
  }
  if ( mTable->current() ) {
    mChildren.push_back( new UdpValNode("Current", mTable->current()) );
  }
  mChildren.push_back( new UdpValNode("Output", mTable->output()) );
}


//////////////////////////////////////////////////////////////////////
// クラス UdpValNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] label ラベル
// @param[in] val テーブルの値
UdpValNode::UdpValNode(const QString& label,
		       const PtUdpValue* val) :
  mLabel(label),
  mVal(val)
{
}

// @brief デストラクタ
UdpValNode::~UdpValNode()
{
}

// @brief データを返す．
// @param[in] column コラム番号
// @param[in] role 
QVariant
UdpValNode::data(int column,
		 int role) const
{
  if ( role == Qt::DisplayRole ) {
    if ( column == 0 ) {
      return mLabel;
    }
    else if ( column == 1 ) {
      return symbol2string(mVal->symbol()).c_str();
    }
  }
  return QVariant();
}
    
// @brief 対象のファイル上での位置を返す．
FileRegion
UdpValNode::loc() const
{
  return mVal->file_region();
}

// @brief 子供の配列を作る．
void
UdpValNode::expand() const
{
}

END_NAMESPACE_YM_VERILOG
