
/// @file libym_verilog/tests/ptview/PtNode_root.cc
/// @brief PtNode の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: VlPtNode_root.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2009 Yusuke Matsunaga
/// All rights reserved.


#include "PtNode_root.h"
#include "PtNode_udp.h"
#include "PtNode_module.h"
#include "ym_verilog/VlMgr.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス RootNode
//////////////////////////////////////////////////////////////////////
  
// @brief コンストラクタ
// @param[in] vl_mgr VlMgr
RootNode::RootNode(const VlMgr& vl_mgr) :
  mVlMgr(vl_mgr)
{
}

// @brief デストラクタ
RootNode::~RootNode()
{
}

// @brief データを返す．
// @param[in] column コラム番号
// @param[in] role 
QVariant
RootNode::data(int column,
	       int role) const
{
  if ( role == Qt::DisplayRole ) {
    if ( column == 0 ) {
      return "Root";
    }
    else if ( column == 1 ) {
      return "";
    }
  }
  return QVariant();
}

// @brief 対象のファイル上での位置を返す．
FileRegion
RootNode::loc() const
{
  return FileRegion();
}

// @brief 子供の配列を作る．
void
RootNode::expand() const
{
  mChildren.reserve(2);
  if ( mVlMgr.pt_udp_list().size() > 0 ) {
    mChildren.push_back( new UdpListNode(mVlMgr) );
  }
  if ( mVlMgr.pt_module_list().size() > 0 ) {
    mChildren.push_back( new ModuleListNode(mVlMgr) );
  }
}


//////////////////////////////////////////////////////////////////////
// クラス UdpListNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] vl_mgr VlMgr
UdpListNode::UdpListNode(const VlMgr& vl_mgr) :
  mVlMgr(vl_mgr)
{
}

// @brief デストラクタ
UdpListNode::~UdpListNode()
{
}

// @brief データを返す．
// @param[in] column コラム番号
// @param[in] role 
QVariant
UdpListNode::data(int column,
		  int role) const
{
  if ( role == Qt::DisplayRole ) {
    if ( column == 0 ) {
      return "UDP list";
    }
    else if ( column == 1 ) {
      return "";
    }
  }
  return QVariant();
}

// @brief 対象のファイル上での位置を返す．
FileRegion
UdpListNode::loc() const
{
  return FileRegion();
}

// @brief 子供の配列を作る．
void
UdpListNode::expand() const
{
  const list<const PtUdp*>& udp_list = mVlMgr.pt_udp_list();
  mChildren.resize(udp_list.size());
  ymuint32 i = 0;
  for (list<const PtUdp*>::const_iterator p = udp_list.begin();
       p != udp_list.end(); ++ p, ++ i) {
    const PtUdp* udp = *p;
    mChildren[i] = new UdpNode(udp);
  }
}


//////////////////////////////////////////////////////////////////////
// クラス ModuleListNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] vl_mgr VlMgr
ModuleListNode::ModuleListNode(const VlMgr& vl_mgr) :
  mVlMgr(vl_mgr)
{
}

// @brief デストラクタ
ModuleListNode::~ModuleListNode()
{
}

// @brief データを返す．
// @param[in] column コラム番号
// @param[in] role 
QVariant
ModuleListNode::data(int column,
		     int role) const
{
  if ( role == Qt::DisplayRole ) {
    if ( column == 0 ) {
      return "Module list";
    }
    else if ( column == 1 ) {
      return "";
    }
  }
  return QVariant();
}

// @brief 対象のファイル上での位置を返す．
FileRegion
ModuleListNode::loc() const
{
  return FileRegion();
}

// @brief 子供の配列を作る．
void
ModuleListNode::expand() const
{
  const list<const PtModule*>& module_list = mVlMgr.pt_module_list();
  mChildren.resize(module_list.size());
  ymuint32 i = 0;
  for (list<const PtModule*>::const_iterator p = module_list.begin();
       p != module_list.end(); ++ p, ++ i) {
    const PtModule* module = *p;
    mChildren[i] = new ModuleNode(module);
  }
}

END_NAMESPACE_YM_VERILOG
