
/// @file libym_verilog/parser/Parser.cc
/// @brief Parser の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: Parser_module.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "Parser.h"
#include "Parser_port.h"

#include "Lex.h"
#include "LexModuleState.h"
#include "PtMgr.h"
#include "PtiFactory.h"
#include "ym_verilog/pt/PtModule.h"
#include "PtiDecl.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// Module 関係
//////////////////////////////////////////////////////////////////////

// Verilog1995 タイプのモジュールを生成する．
void
Parser::new_Module1995(const FileRegion& file_region,
		       bool is_macro,
		       const char* module_name,
		       PtrList<PtAttrInst>* ai_list)
{
  PtiPortArray port_array = get_port_array();
  PtDeclHeadArray paramport_array = get_paramport_array();
  PtIOHeadArray iohead_array = get_io_array();
  PtDeclHeadArray paramhead_array = get_param_array();
  PtDeclHeadArray localparamhead_array = get_localparam_array();
  PtDeclHeadArray declhead_array = get_decl_array();
  PtItemArray item_array = get_item_array();

  LexModuleState* state = lex().module_state();
  bool is_cell = state->cell_define();
  bool is_protected = false; // これどうやって決めるの？
  int time_u = state->time_unit();
  int time_p = state->time_precision();
  tVpiNetType nettype = state->default_nettype();
  tVpiUnconnDrive unconn = state->unconnected_drive();
  tVpiDefDelayMode delay = state->delay_mode();
  int decay = state->default_decay_time();

#if 0 // VERIFAULT
  bool portfaults = state->portfaults();
  bool suppress_faults = state->suppress_faults();
#else
  bool portfaults = false;
  bool suppress_faults = false;
#endif

  string config;  // ?
  string library; // ?
  string cell;    // ?

  // port_array をスキャンして中で用いられている名前を port_ref_dic
  // に登録する．
  hash_set<string> port_ref_dic;
  for (ymuint32 i = 0; i < port_array.size(); ++ i) {
    PtiPort* port = port_array[i];
    ymuint32 n = port->portref_num();
    for (ymuint32 j = 0; j < n; ++ j) {
      PtiPortRef* portref = port->_portref(j);
      const char* name = portref->name();
      port_ref_dic.insert(name);
    }
  }

  // 入出力ポート宣言に現れる名前を iodecl_names に入れる．
  // ポート宣言が型を持つ場合にはモジュール内部の宣言要素を生成する．
  // 持たない場合にはデフォルトタイプのネットを生成する．
  hash_map<string, tVpiDirection> iodecl_dirs;
  for (ymuint32 i = 0; i < iohead_array.size(); ++ i) {
    const PtIOHead* io_head = iohead_array[i];
    // 名前をキーにして方向を記録しておく
    tVpiDirection dir = kVpiNoDirection;
    switch ( io_head->type() ) {
    case kPtIO_Input:  dir = kVpiInput; break;
    case kPtIO_Output: dir = kVpiOutput; break;
    case kPtIO_Inout:  dir = kVpiInout; break;
    default:
      assert_not_reached(__FILE__, __LINE__);
    }
    for (ymuint32 j = 0; j < io_head->item_num(); ++ j) {
      const PtIOItem* elem = io_head->item(j);
      const char* elem_name = elem->name();

      // まず未定義/多重定義のエラーをチェックする．
      if ( !port_ref_dic.count(elem_name) ) {
	// port expression に現れない信号線名
	// 未定義エラー
	ostringstream buf;
	buf << "\"" << elem_name << "\" is not defined in the port list.";
	put_msg(__FILE__, __LINE__,
		elem->file_region(),
		kMsgError,
		"ELAB",
		buf.str());
      }
      if ( iodecl_dirs.count(elem_name) > 0 ) {
	// 二重登録エラー
	ostringstream buf;
	buf << "\"" << elem_name << "\" is redefined.";
	put_msg(__FILE__, __LINE__,
		elem->file_region(),
		kMsgError,
		"ELAB",
		buf.str());
      }
      else {
	iodecl_dirs.insert(make_pair(elem_name, dir));
      }
    }
  }

  // 今度はポートリストに現れている信号線が入出力ポート宣言されているか
  // 調べる．
  // 同時に名無しのポートがあるかどうかしらべる．
  bool named_port = true;
  for (ymuint32 i = 0; i < port_array.size(); ++ i) {
    PtiPort* port = port_array[i];
    if ( port->ext_name() == NULL ) {
      // 1つでも名前を持たないポートがあったら名前での結合はできない．
      named_port = false;
    }
    ymuint32 n = port->portref_num();
    for (ymuint32 j = 0; j < n; ++ j) {
      PtiPortRef* portref = port->_portref(j);
      const char* name = portref->name();
      hash_map<string, tVpiDirection>::iterator p = iodecl_dirs.find(name);
      if ( p == iodecl_dirs.end() ) {
	// name は IOH リストに存在しない．
	ostringstream buf;
	buf << "\"" << name << "\" is in the port list but not declared.";
	put_msg(__FILE__, __LINE__,
		port->file_region(),
		kMsgError,
		"ELAB",
		buf.str());
      }
      else {
	tVpiDirection dir = p->second;
	portref->set_dir(dir);
      }
    }
  }

  PtModule* module = mFactory.new_Module(file_region,
					 module_name,
					 is_macro,
					 is_cell,
					 is_protected,
					 time_u, time_p,
					 nettype, unconn,
					 delay, decay,
					 named_port,
					 portfaults, suppress_faults,
					 config, library, cell,
					 paramport_array,
					 port_array,
					 iohead_array,
					 paramhead_array,
					 localparamhead_array,
					 declhead_array,
					 item_array);
  mPtMgr.reg_module(module);
  reg_attrinst(module, ai_list);
  end_module();
}

// Verilog2001 タイプのモジュールを生成する．
void
Parser::new_Module2001(const FileRegion& file_region,
		       bool is_macro,
		       const char* module_name,
		       PtrList<PtAttrInst>* ai_list)
{
  PtDeclHeadArray paramport_array = get_paramport_array();
  PtIOHeadArray iohead_array = get_io_array();
  PtDeclHeadArray paramhead_array = get_param_array();
  PtDeclHeadArray localparamhead_array = get_localparam_array();
  PtDeclHeadArray declhead_array = get_decl_array();
  PtItemArray item_array = get_item_array();

  LexModuleState* state = lex().module_state();
  bool is_cell = state->cell_define();
  bool is_protected = false; // これどうやって決めるの？
  int time_u = state->time_unit();
  int time_p = state->time_precision();
  tVpiNetType nettype = state->default_nettype();
  tVpiUnconnDrive unconn = state->unconnected_drive();
  tVpiDefDelayMode delay = state->delay_mode();
  int decay = state->default_decay_time();

#if 0 // VERIFAULT
  bool portfaults = state->portfaults();
  bool suppress_faults = state->suppress_faults();
#else
  bool portfaults = false;
  bool suppress_faults = false;
#endif

  string config;  // ?
  string library; // ?
  string cell;    // ?

  // iohead_array からポートの配列を作る．
  PtiPortArray port_array = new_PortArray(iohead_array);

  PtModule* module = mFactory.new_Module(file_region,
					 module_name,
					 is_macro, is_cell, is_protected,
					 time_u, time_p, nettype,
					 unconn, delay, decay,
					 true,
					 portfaults, suppress_faults,
					 config, library, cell,
					 paramport_array,
					 port_array,
					 iohead_array,
					 paramhead_array,
					 localparamhead_array,
					 declhead_array,
					 item_array);
  mPtMgr.reg_module(module);
  reg_attrinst(module, ai_list);
  end_module();
}

// @brief 入出力宣言からポートを作る．
PtiPortArray
Parser::new_PortArray(PtIOHeadArray iohead_array)
{
  ymuint32 n = 0;
  for (ymuint32 i = 0; i < iohead_array.size(); ++ i) {
    n += iohead_array[i]->item_num();
  }
  // port_array を確保する．
  void* p = mAlloc.get_memory(sizeof(PtiPort*) * n);
  PtiPort** array = new (p) PtiPort*[n];

  // ポートを生成し arary に格納する．
  n = 0;
  for (ymuint32 i = 0; i < iohead_array.size(); ++ i) {
    const PtIOHead* head = iohead_array[i];
    for (ymuint32 j = 0; j < head->item_num(); ++ j) {
      const PtIOItem* elem = head->item(j);
      const char* name = elem->name();
      init_portref_list();
      new_PortRef(elem->file_region(), name);
      PtiPortRefArray port_expression = get_portref_array();
      array[n] = mFactory.new_Port(elem->file_region(), port_expression, name);
      ++ n;
    }
  }
  return PtiPortArray(n, array);
}

END_NAMESPACE_YM_VERILOG
