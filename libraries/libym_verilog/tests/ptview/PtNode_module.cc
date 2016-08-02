
/// @file libym_verilog/tests/ptview/PtNode_module.cc
/// @brief PtNode の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: PtNode_module.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2009 Yusuke Matsunaga
/// All rights reserved.


#include "PtNode_module.h"
#include "PtNode_port.h"
#include "PtNode_decl.h"
#include "PtNode_item.h"
#include "PtNode_misc.h"
#include "ym_verilog/pt/PtModule.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス ModuleNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] module Module を表すパース木
ModuleNode::ModuleNode(const PtModule* module) :
  mModule(module)
{
}

// @brief デストラクタ
ModuleNode::~ModuleNode()
{
}

// @brief データを返す．
// @param[in] column コラム番号
// @param[in] role 
QVariant
ModuleNode::data(int column,
		 int role) const
{
  if ( role == Qt::DisplayRole ) {
    if ( column == 0 ) {
      return "Module";
    }
    else if ( column == 1 ) {
      return mModule->name();
    }
  }
  return QVariant();
}

// @brief 対象のファイル上での位置を返す．
FileRegion
ModuleNode::loc() const
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
ModuleNode::expand() const
{
  mChildren.push_back( new BoolNode("Cell Define", mModule->is_cell()) );
  mChildren.push_back( new BoolNode("Protected", mModule->is_protected()) );
  mChildren.push_back( new BoolNode("MacroModule", mModule->is_macromodule()) );
  if ( mModule->time_unit() != -16 ) {
    mChildren.push_back( new StrNode("Time Unit", unit2str(mModule->time_unit())) );
    mChildren.push_back( new StrNode("Time Precision", unit2str(mModule->time_precision())) );
  }
  mChildren.push_back( new NetTypeNode("Default Net Type", mModule->nettype()) );
  mChildren.push_back( new UdNode(mModule->unconn_drive()) );
  mChildren.push_back( new DelayModeNode(mModule->delay_mode()) );
  
  mChildren.push_back( new StrNode("Config", mModule->config().c_str()) );
  mChildren.push_back( new StrNode("Library", mModule->library().c_str()) );
  mChildren.push_back( new StrNode("Cell", mModule->cell().c_str()) );
  
  if ( mModule->paramport_array().size() > 0 ) {
    mChildren.push_back( new DeclHeadListNode("Parameter Port",
					      mModule->paramport_array()) );
  }
  if ( mModule->port_num() > 0 ) {
    mChildren.push_back( new ModulePortListNode(mModule) );
  }
  if ( mModule->iohead_array().size() > 0 ) {
    mChildren.push_back( new IOHeadListNode(mModule->iohead_array()) );
  }
  if ( mModule->paramhead_array().size() > 0 ) {
    mChildren.push_back( new DeclHeadListNode("Parameter",
					      mModule->paramhead_array()) );
  }
  if ( mModule->localparamhead_array().size() > 0 ) {
    mChildren.push_back( new DeclHeadListNode("Localparam",
					      mModule->localparamhead_array()) );
  }
  if ( mModule->declhead_array().size() > 0 ) {
    mChildren.push_back( new DeclHeadListNode("Decl",
					      mModule->declhead_array()) );
  }
  if ( mModule->item_array().size() > 0 ) {
    mChildren.push_back( new ItemListNode("Item",
					  mModule->item_array()) );
  }
}


//////////////////////////////////////////////////////////////////////
// クラス ModulePortListNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] module Module を表すパース木
ModulePortListNode::ModulePortListNode(const PtModule* module) :
  mModule(module)
{
}

// @brief デストラクタ
ModulePortListNode::~ModulePortListNode()
{
}

// @brief データを返す．
// @param[in] column コラム番号
// @param[in] role 
QVariant
ModulePortListNode::data(int column,
			 int role) const
{
  if ( role == Qt::DisplayRole ) {
    if ( column == 0 ) {
      return "Module Port List";
    }
    else if ( column == 1 ) {
      return "";
    }
  }
  return QVariant();
}
    
// @brief 対象のファイル上での位置を返す．
FileRegion
ModulePortListNode::loc() const
{
  return FileRegion();
}

// @brief 子供の配列を作る．
void
ModulePortListNode::expand() const
{
  mChildren.resize(mModule->port_num());
  for (ymuint32 i = 0; i < mModule->port_num(); ++ i) {
    mChildren[i] = new PortNode(mModule->port(i));
  }
}

END_NAMESPACE_YM_VERILOG
