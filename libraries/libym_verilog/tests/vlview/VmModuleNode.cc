
/// @file libym_verilog/tests/vlview/VmModuleNode.cc
/// @brief VmModuleNode の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: VmModuleNode.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "VmModuleNode.h"
#include "VmMiscNode.h"
#include "ym_verilog/VlMgr.h"
#include "ym_verilog/vl/VlModule.h"
#include "ym_verilog/vl/VlPort.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス VmNode の関数
//////////////////////////////////////////////////////////////////////

// @brief ModuleArrayListNode を追加する．
// @param[in] ma_list モジュール配列のリスト
void
VmNode1::add_modulearraylist(const vector<const VlModuleArray*>& ma_list) const
{
  add_child( new VmModuleArrayListNode(vl_mgr(), ma_list) );
}

// @brief ModuleInstListNode を追加する．
// @param[in] module_list モジュールのリスト
void
VmNode1::add_moduleinstlist(const vector<const VlModule*>& module_list) const
{
  add_child( new VmModuleInstListNode(vl_mgr(), module_list) );
}

// @brief ModuleListNode を追加する．
void
VmNode1::add_modulelist() const
{
  add_child( new VmModuleListNode(vl_mgr()) );
}


//////////////////////////////////////////////////////////////////////
// クラス VmModuleArrayListNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] vl_mgr VlMgr
// @param[in] ma_list モジュール配列のリスト
VmModuleArrayListNode::VmModuleArrayListNode(const VlMgr& vl_mgr,
					     const vector<const VlModuleArray*>& ma_list) :
  VmNode1(vl_mgr),
  mModuleArrayArray(ma_list)
{
}

// @brief デストラクタ
VmModuleArrayListNode::~VmModuleArrayListNode()
{
}

// @brief データを返す．
// @param[in] column コラム番号
// @param[in] role 
QVariant
VmModuleArrayListNode::data(int column,
		       int role) const
{
  if ( role == Qt::DisplayRole ) {
    if ( column == 0 ) {
      return "vpiModuleArray list";
    }
    else if ( column == 1 ) {
      return "";
    }
  }
  return QVariant();
}

// @brief 対象のファイル上での位置を返す．
FileRegion
VmModuleArrayListNode::loc() const
{
  return FileRegion();
}

// @brief 子供の配列を作る．
void
VmModuleArrayListNode::expand() const
{
  for (vector<const VlModuleArray*>::const_iterator p = mModuleArrayArray.begin();
       p != mModuleArrayArray.end(); ++ p) {
    add_child( new VmModuleArrayNode(vl_mgr(), *p) );
  }
}


//////////////////////////////////////////////////////////////////////
// クラス VmModuleArrayNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] vl_mgr VlMgr
// @param[in] ma_list モジュール配列のリスト
VmModuleArrayNode::VmModuleArrayNode(const VlMgr& vl_mgr,
				     const VlModuleArray* module_array) :
  VmNode1(vl_mgr),
  mModuleArray(module_array)
{
}

// @brief デストラクタ
VmModuleArrayNode::~VmModuleArrayNode()
{
}

// @brief データを返す．
// @param[in] column コラム番号
// @param[in] role 
QVariant
VmModuleArrayNode::data(int column,
			int role) const
{
  if ( role == Qt::DisplayRole ) {
    if ( column == 0 ) {
      return "vpiModuleArray";
    }
    else if ( column == 1 ) {
      return "";
    }
  }
  return QVariant();
}

// @brief 対象のファイル上での位置を返す．
FileRegion
VmModuleArrayNode::loc() const
{
  return FileRegion();
}

// @brief 子供の配列を作る．
void
VmModuleArrayNode::expand() const
{
  ymuint n = mModuleArray->elem_num();
  for (ymuint i = 0; i < n; ++ i) {
    const VlModule* module = mModuleArray->elem_by_offset(i);
    add_child( new VmModuleNode(vl_mgr(), module) );
  }
}


//////////////////////////////////////////////////////////////////////
// クラス VmModuleInstListNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] vl_mgr VlMgr
// @param[in] module_list モジュールのリスト
VmModuleInstListNode::VmModuleInstListNode(const VlMgr& vl_mgr,
					   const vector<const VlModule*>& module_list) :
  VmNode1(vl_mgr),
  mModuleArray(module_list)
{
}

// @brief デストラクタ
VmModuleInstListNode::~VmModuleInstListNode()
{
}

// @brief データを返す．
// @param[in] column コラム番号
// @param[in] role 
QVariant
VmModuleInstListNode::data(int column,
			   int role) const
{
  if ( role == Qt::DisplayRole ) {
    if ( column == 0 ) {
      return "vpiModule list";
    }
    else if ( column == 1 ) {
      return "";
    }
  }
  return QVariant();
}

// @brief 対象のファイル上での位置を返す．
FileRegion
VmModuleInstListNode::loc() const
{
  return FileRegion();
}

// @brief 子供の配列を作る．
void
VmModuleInstListNode::expand() const
{
  for (vector<const VlModule*>::const_iterator p = mModuleArray.begin();
       p != mModuleArray.end(); ++ p) {
    const VlModule* module = *p;
    add_child( new VmModuleNode(vl_mgr(), module) );
  }
}


//////////////////////////////////////////////////////////////////////
// クラス VmModuleListNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] vl_mgr VlMgr
VmModuleListNode::VmModuleListNode(const VlMgr& vl_mgr) :
  VmNode1(vl_mgr)
{
}

// @brief デストラクタ
VmModuleListNode::~VmModuleListNode()
{
}

// @brief データを返す．
// @param[in] column コラム番号
// @param[in] role 
QVariant
VmModuleListNode::data(int column,
		       int role) const
{
  if ( role == Qt::DisplayRole ) {
    if ( column == 0 ) {
      return "vpiModule list";
    }
    else if ( column == 1 ) {
      return "";
    }
  }
  return QVariant();
}

// @brief 対象のファイル上での位置を返す．
FileRegion
VmModuleListNode::loc() const
{
  return FileRegion();
}

// @brief 子供の配列を作る．
void
VmModuleListNode::expand() const
{
  const list<const VlModule*>& module_list = vl_mgr().topmodule_list();
  for (list<const VlModule*>::const_iterator p = module_list.begin();
       p != module_list.end(); ++ p) {
    add_child( new VmModuleNode(vl_mgr(), *p) );
  }
}


//////////////////////////////////////////////////////////////////////
// クラス VmModuleNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] vl_mgr VlMgr
// @param[in] module Module の構造を表すオブジェクト
VmModuleNode::VmModuleNode(const VlMgr& vl_mgr,
			   const VlModule* module) :
  VmNode1(vl_mgr),
  mModule(module)
{
}

// @brief デストラクタ
VmModuleNode::~VmModuleNode()
{
}

// @brief データを返す．
// @param[in] column コラム番号
// @param[in] role 
QVariant
VmModuleNode::data(int column,
		   int role) const
{
  if ( role == Qt::DisplayRole ) {
    if ( column == 0 ) {
      return "vpiModule";
    }
    else if ( column == 1 ) {
      return mModule->name();
    }
  }
  return QVariant();
}

// @brief 対象のファイル上での位置を返す．
FileRegion
VmModuleNode::loc() const
{
  return mModule->file_region();
}

BEGIN_NONAMESPACE

// 時間の単位を表す文字列を得る．
QString
unit2str(int unit)
{
  if ( unit > 2 || unit < -15 ) {
    return "illegal time unit";
  }

  unit += 15;
  QString ans;
  switch ( unit % 3 ) {
  case 0: ans = "1"; break;
  case 1: ans = "10"; break;
  case 2: ans = "100"; break;
  }
  switch ( unit / 3 ) {
  case 5: ans += "s"; break;
  case 4: ans += "ms"; break;
  case 3: ans += "us"; break;
  case 2: ans += "ns"; break;
  case 1: ans += "ps"; break;
  case 0: ans += "fs"; break;
  }
  return ans;
}

END_NONAMESPACE

// @brief 子供の配列を作る．
void
VmModuleNode::expand() const
{
  add_str("vpiDefName", mModule->def_name());
  add_str("vpiFullName", mModule->full_name());
  // vpiDefFile

  const VlModuleArray* module_array = mModule->module_array();
  bool is_array = (module_array != NULL);
  add_bool("vpiArray", is_array);
  if ( is_array ) {
    add_str("vpiModuleArray", module_array->full_name());
    add_int("vpiIndex", mModule->index());
  }
  add_bool("vpiTopModule", mModule->is_top_module());
  add_bool("vpiCellInstance", mModule->is_cell_instance());
  add_bool("vpiProtected", mModule->is_protected());
  if ( mModule->time_unit() != -16 ) {
    add_str("vpiTimeUnit", unit2str(mModule->time_unit()));
    add_str("vpiTimePrecision", unit2str(mModule->time_precision()));
  }
  add_child( new VmNetTypeNode("vpiDefaultNetType", mModule->def_net_type()) );
  add_child( new VmUdNode(mModule->unconn_drive()) );
  add_child( new VmDelayModeNode(mModule->def_delay_mode()) );
  add_int("vpiDefaultDecayTime", mModule->def_decay_time());
  
  add_str("Config", mModule->config());
  add_str("Library", mModule->library());
  add_str("Cell", mModule->cell());

  add_child( new VmPortListNode(mModule) );
  ymuint n = mModule->io_num();
  vector<const VlIODecl*> io_array(n);
  for (ymuint i = 0; i < n; ++ i) {
    io_array[i] = mModule->io(i);
  }
  add_iolist(io_array);

  add_scope_item(mModule);

  vector<const VlProcess*> process_list;
  if ( vl_mgr().find_process_list(mModule, process_list) ) {
    add_processlist(process_list);
  }
}


//////////////////////////////////////////////////////////////////////
// クラス VmPortListNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] module Module を表すパース木
VmPortListNode::VmPortListNode(const VlModule* module) :
  mModule(module)
{
}

// @brief デストラクタ
VmPortListNode::~VmPortListNode()
{
}

// @brief データを返す．
// @param[in] column コラム番号
// @param[in] role 
QVariant
VmPortListNode::data(int column,
		     int role) const
{
  if ( role == Qt::DisplayRole ) {
    if ( column == 0 ) {
      return "vpiPort list";
    }
    else if ( column == 1 ) {
      return "";
    }
  }
  return QVariant();
}

// @brief 対象のファイル上での位置を返す．
FileRegion
VmPortListNode::loc() const
{
  return FileRegion();
}

// @brief 子供の配列を作る．
void
VmPortListNode::expand() const
{
  for (ymuint i = 0; i < mModule->port_num(); ++ i) {
    const VlPort* port = mModule->port(i);
    add_child( new VmPortNode(port) );
  }
}


//////////////////////////////////////////////////////////////////////
// クラス VlPortNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] port ポートを表すオブジェクト
VmPortNode::VmPortNode(const VlPort* port) :
  mPort(port)
{
}

// @brief デストラクタ
VmPortNode::~VmPortNode()
{
}

// @brief データを返す．
// @param[in] column コラム番号
// @param[in] role 
QVariant
VmPortNode::data(int column,
		 int role) const
{
  if ( role == Qt::DisplayRole ) {
    if ( column == 0 ) {
      return "vpiPort";
    }
    else if ( column == 1 ) {
      if ( mPort->name() ) {
	return mPort->name();
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
VmPortNode::loc() const
{
  return mPort->file_region();
}

// @brief 子供の配列を作る．
void
VmPortNode::expand() const
{
  add_str("vpiModule", mPort->module()->full_name());
  add_bool("vpiConnByName", mPort->is_conn_by_name());
  add_bool("vpiExplicitName", mPort->is_explicit_name());
  add_int("vpiPortIndex", mPort->port_index());
#if 0
  add_bool("vpiScalar", mPort->is_scalar());
  add_bool("vpiVector", mPort->is_vector());
#endif
  add_int("vpiSzie", mPort->bit_size());
  if ( mPort->high_conn() ) {
    add_expr("vpiHighConn", mPort->high_conn());
  }
  if ( mPort->low_conn() ) {
    add_expr("vpiLowConn", mPort->low_conn());
  }
}

END_NAMESPACE_YM_VERILOG
