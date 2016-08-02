
/// @file libym_verilog/tests/vlview/VmUdpNode.cc
/// @brief VmUdpNode の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: VlPtNode_udp.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "VmUdpNode.h"
#include "VmMiscNode.h"
#include "ym_verilog/VlMgr.h"
#include "ym_verilog/vl/VlUdp.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス VmNode
//////////////////////////////////////////////////////////////////////

// @brief UdpListNode を追加する．
void
VmNode1::add_udplist() const
{
  add_child( new VmUdpListNode(vl_mgr()) );
}


//////////////////////////////////////////////////////////////////////
// クラス VmUdpListNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] vl_mgr VlMgr
VmUdpListNode::VmUdpListNode(const VlMgr& vl_mgr) :
  VmNode1(vl_mgr)
{
}

// @brief デストラクタ
VmUdpListNode::~VmUdpListNode()
{
}

// @brief データを返す．
// @param[in] column コラム番号
// @param[in] role 
QVariant
VmUdpListNode::data(int column,
		    int role) const
{
  if ( role == Qt::DisplayRole ) {
    if ( column == 0 ) {
      return "vpiUdpDefn";
    }
    else if ( column == 1 ) {
      return "";
    }
  }
  return QVariant();
}

// @brief 対象のファイル上での位置を返す．
FileRegion
VmUdpListNode::loc() const
{
  return FileRegion();
}

// @brief 子供の配列を作る．
void
VmUdpListNode::expand() const
{
  const list<const VlUdpDefn*>& udp_list = vl_mgr().udp_list();
  for (list<const VlUdpDefn*>::const_iterator p = udp_list.begin();
       p != udp_list.end(); ++ p) {
    const VlUdpDefn* udp = *p;
    add_child( new VmUdpNode(udp) );
  }
}


//////////////////////////////////////////////////////////////////////
// クラス VmUdpNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] udp UDP を表すオブジェクト
VmUdpNode::VmUdpNode(const VlUdpDefn* udp) :
  mUdp(udp)
{
}

// @brief デストラクタ
VmUdpNode::~VmUdpNode()
{
}

// @brief データを返す．
// @param[in] column コラム番号
// @param[in] role 
QVariant
VmUdpNode::data(int column,
		int role) const
{
  if ( role == Qt::DisplayRole ) {
    if ( column == 0 ) {
      return "vpiUdpDefn";
    }
    else if ( column == 1 ) {
      return mUdp->def_name();
    }
  }
  return QVariant();
}

// @brief 対象のファイル上での位置を返す．
FileRegion
VmUdpNode::loc() const
{
  return mUdp->file_region();
}

// @brief 子供の配列を作る．
void
VmUdpNode::expand() const
{
  add_child( new VmPrimTypeNode(mUdp->prim_type()) );
  add_bool("vpiProtected", mUdp->is_protected());
  ymuint n = mUdp->port_num();
  vector<const VlIODecl*> ioarray(n);
  for (ymuint i = 0; i < n - 1; ++ i) {
    ioarray[i] = mUdp->input(i);
  }
  ioarray[n - 1] = mUdp->output();
  add_iolist(ioarray);
  if ( mUdp->init_expr() ) {
    add_expr("vpiInitial", mUdp->init_expr());
  }
  add_child( new VmUdpEntryListNode(mUdp) );
}


//////////////////////////////////////////////////////////////////////
// クラス VmUdpEntryListNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] udp UDP を表すオブジェクト
VmUdpEntryListNode::VmUdpEntryListNode(const VlUdpDefn* udp) :
  mUdp(udp)
{
}

// @brief デストラクタ
VmUdpEntryListNode::~VmUdpEntryListNode()
{
}

// @brief データを返す．
// @param[in] column コラム番号
// @param[in] role 
QVariant
VmUdpEntryListNode::data(int column,
			 int role) const
{
  if ( role == Qt::DisplayRole ) {
    if ( column == 0 ) {
      return "vpiTableEntry";
    }
    else if ( column == 1 ) {
      return "";
    }
  }
  return QVariant();
}
    
// @brief 対象のファイル上での位置を返す．
FileRegion
VmUdpEntryListNode::loc() const
{
  return FileRegion();
}

// @brief 子供の配列を作る．
void
VmUdpEntryListNode::expand() const
{
  ymuint n = mUdp->table_size();
  for (ymuint i = 0; i < n; ++ i) {
    add_child( new VmUdpEntryNode(mUdp->table_entry(i)) );
  }
}


//////////////////////////////////////////////////////////////////////
// クラス VmUdpEntryNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] table テーブルの1行分
VmUdpEntryNode::VmUdpEntryNode(const VlTableEntry* table) :
  mTable(table)
{
}

// @brief デストラクタ
VmUdpEntryNode::~VmUdpEntryNode()
{
}

// @brief データを返す．
// @param[in] column コラム番号
// @param[in] role 
QVariant
VmUdpEntryNode::data(int column,
		     int role) const
{
  if ( role == Qt::DisplayRole ) {
    if ( column == 0 ) {
      return "vpiTableEntry";
    }
    else if ( column == 1 ) {
      return mTable->str().c_str();
    }
  }
  return QVariant();
}
    
// @brief 対象のファイル上での位置を返す．
FileRegion
VmUdpEntryNode::loc() const
{
  return mTable->file_region();
}

// @brief 子供の配列を作る．
void
VmUdpEntryNode::expand() const
{
  // なにもしない．
}

END_NAMESPACE_YM_VERILOG
