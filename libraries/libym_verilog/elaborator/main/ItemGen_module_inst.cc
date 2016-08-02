
/// @file libym_verilog/elaborator/ItemGen_module_inst.cc
/// @brief ElbMgr の実装ファイル(モジュールインスタンスの実体化)
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: ItemGen_module_inst.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ItemGen.h"
#include "ElbEnv.h"
#include "ElbParamCon.h"

#include "ym_verilog/BitVector.h"

#include "ym_verilog/pt/PtModule.h"
#include "ym_verilog/pt/PtPort.h"
#include "ym_verilog/pt/PtItem.h"
#include "ym_verilog/pt/PtExpr.h"
#include "ym_verilog/pt/PtMisc.h"

#include "ym_verilog/vl/VlPort.h"

#include "ElbModule.h"
#include "ElbExpr.h"


BEGIN_NAMESPACE_YM_VERILOG

BEGIN_NONAMESPACE

// 英語の序数の接尾語を作る関数
const char*
num_suffix(int num)
{
  switch ( num ) {
  case 1: return "st";
  case 2: return "nd";
  case 3: return "rd";
  default: return "th";
  }
}

END_NONAMESPACE


//////////////////////////////////////////////////////////////////////
// インスタンス関係の instantiate 関数 (変な日本語)
//////////////////////////////////////////////////////////////////////

// @brief module instance のスコープ生成を行う．
// @param[in] parent 親のスコープ
// @param[in] pt_head ヘッダ
void
ItemGen::phase1_muheader(const VlNamedObj* parent,
			 const PtItem* pt_head)
{
  const FileRegion& fr = pt_head->file_region();
  const char* defname = pt_head->name();

  const PtModule* pt_module = find_moduledef(defname);
  if ( pt_module ) {
    // モジュール定義が見つかった．
  
    if ( pt_module->is_in_use() ) {
      // 依存関係が循環している．
      ostringstream buf;
      buf << pt_module->name() << " : instantiated within itself.";
      put_msg(__FILE__, __LINE__,
	      pt_module->file_region(),
	      kMsgError,
	      "PARS",
	      buf.str());
      return;
    }

    for (ymuint i = 0; i < pt_head->size(); ++ i) {
      const PtInst* pt_inst = pt_head->inst(i);
      const char* name = pt_inst->name();
      if ( name == NULL ) {
	// 名無しのモジュールインスタンスはない
	put_msg(__FILE__, __LINE__,
		fr,
		kMsgError,
		"ELAB",
		"No module instance name given.");
	return;
      }

      const PtExpr* pt_left = pt_inst->left_range();
      const PtExpr* pt_right = pt_inst->right_range();
      if ( pt_left && pt_right ) {
	// 配列型は今すぐにはインスタンス化できない．
	add_phase1stub(make_stub(this, &ItemGen::phase1_module_array,
				 parent, pt_module, pt_head, pt_inst));
      }
      else {
	// 単一の要素
	ElbModule* module1 = factory().new_Module(parent, 
						  pt_module,
						  pt_head,
						  pt_inst);
	reg_module(module1);
  
	// attribute instance の生成
	//instantiate_attribute(pt_module->attr_top(), true, module1);
	//instantiate_attribute(pt_head->attr_top(), false, module1);
	
	ostringstream buf;
	buf << "\"" << module1->full_name() << "\" has been created.";
	put_msg(__FILE__, __LINE__,
		fr,
		kMsgInfo,
		"ELAB",
		buf.str());
	
	// パラメータ割り当て式の生成
	PtConnectionArray pa_array = pt_head->paramassign_array();
	ymuint n = pa_array.size();
	bool named_con = false;
	if ( n > 0 && pa_array[0]->name() != NULL ) {
	  named_con = true;
	}
	ElbParamCon param_con(pt_head->file_region(), n, named_con);
	for (ymuint i = 0; i < n; ++ i) {
	  const PtConnection* pt_con = pa_array[i];
	  ElbExpr* expr = instantiate_constant_expr(parent, pt_con->expr());
	  param_con.set(i, pt_con, expr);
	}
	
	phase1_module_item(module1, pt_module, &param_con);
	add_phase3stub(make_stub(this, &ItemGen::link_module,
				 module1, pt_module, pt_inst));
      }
    }
    return;
  }

  // 次に udp を探す．
  const ElbUdpDefn* udpdefn = find_udp(defname);
  if ( udpdefn ) {
    // ただしこの場合, mParamList は空でなければならない．
    // 問題は delay が mParamList に見える場合があるということ．
    PtConnectionArray pa_array = pt_head->paramassign_array();
    ymuint param_size = pa_array.size();
    const PtDelay* pt_delay = pt_head->delay();
    if ( param_size > 0 && pa_array[0]->name() != NULL ) {
      put_msg(__FILE__, __LINE__,
	      fr,
	      kMsgError,
	      "ELAB",
	      "UDP instance cannot have named parameter list.");
      return;
    }
    if ( (pt_delay && param_size > 0) || param_size > 1 ) {
      put_msg(__FILE__, __LINE__,
	      fr,
	      kMsgError,
	      "ELAB",
	      "UDP instance cannot have ordered parameter list.");
      return;
    }
    
    // 今すぐには処理できないのでキューに積む．
    add_phase2stub(make_stub(this, &ItemGen::instantiate_udpheader,
			     parent, pt_head, udpdefn));
    return;
  }
  
  // どちらもなければエラー
  ostringstream buf;
  buf << defname << " : No such module or UDP.";
  put_msg(__FILE__, __LINE__,
	  fr,
	  kMsgError,
	  "ELAB",
	  buf.str());
}

// @brief module array のインスタンス化を行う．
// @param[in] parent 親のスコープ
// @param[in] pt_module モジュール定義
// @param[in] pt_head ヘッダ
// @param[in] pt_inst インスタンス定義
void
ItemGen::phase1_module_array(const VlNamedObj* parent,
			     const PtModule* pt_module,
			     const PtItem* pt_head,
			     const PtInst* pt_inst)
{
  const FileRegion& fr = pt_head->file_region();
  const char* defname = pt_head->name();

  const char* name = pt_inst->name();
  const PtExpr* pt_left = pt_inst->left_range();
  const PtExpr* pt_right = pt_inst->right_range();

  ElbExpr* left = NULL;
  ElbExpr* right = NULL;
  int left_val = 0;
  int right_val = 0;
  if ( !instantiate_range(parent, pt_left, pt_right,
			  left, right,
			  left_val, right_val) ) {
    return;
  }
  
  ElbModuleArray* module_array = factory().new_ModuleArray(parent,
							   pt_module,
							   pt_head,
							   pt_inst,
							   left, right,
							   left_val, right_val);
  reg_modulearray(module_array);

  ostringstream buf;
  buf << "instantiating module array \"" << name << "\" of \""
      << defname << "\" [" << left_val << " : " << right_val << "].";
  put_msg(__FILE__, __LINE__,
	  fr,
	  kMsgInfo,
	  "ELAB",
	  buf.str());

  add_phase3stub(make_stub(this, &ItemGen::link_module_array,
			   module_array, pt_module, pt_inst));

  // パラメータ割り当て式の生成
  PtConnectionArray pa_array = pt_head->paramassign_array();
  ymuint param_num = pa_array.size();
  bool named_con = false;
  if ( param_num > 0 && pa_array[0]->name() != NULL ) {
    named_con = true;
  }
  ElbParamCon param_con(pt_head->file_region(), param_num, named_con);
  for (ymuint i = 0; i < param_num; ++ i) {
    const PtConnection* pt_con = pa_array[i];
    ElbExpr* expr = instantiate_constant_expr(parent, pt_con->expr());
    param_con.set(i, pt_con, expr);
  }

  ymuint n = module_array->elem_num();
  for (ymuint i = 0; i < n; ++ i) {
    ElbModule* module1 = module_array->_module(i);
  
    // attribute instance の生成
    //instantiate_attribute(pt_module->attr_top(), true, module1);
    //instantiate_attribute(pt_head->attr_top(), false, module1);
    
    ostringstream buf;
    buf << "\"" << module1->full_name() << "\" has been created.";
    put_msg(__FILE__, __LINE__,
	    module_array->file_region(),
	    kMsgInfo,
	    "ELAB",
	    buf.str());

    // モジュール要素を作る．
    phase1_module_item(module1, pt_module, &param_con);
  }
}

// @brief module array instance の入出力端子の接続を行う．
// @param[in] pt_module モジュール定義
// @param[in] pt_inst インスタンス定義
// @param[in] module_array モジュール配列
void
ItemGen::link_module_array(ElbModuleArray* module_array,
			   const PtModule* pt_module,
			   const PtInst* pt_inst)
{
  const VlNamedObj* parent = module_array->parent();
  const FileRegion& fr = pt_inst->file_region();

  ymuint module_size = module_array->elem_num();
  ElbModule* module0 = module_array->_module(0);
  ymuint port_num = module0->port_num();

  ymuint n = pt_inst->port_num();
  // ポートの割り当てを行う．
  // 例外: ポートを一つも取らないモジュールの場合
  // module_name instance_name ()
  // という形になって一つのポートが省略された形と区別がつかない．
  // これは Verilog-HDL の仕様がアホ
  // () を取らない形を用意しておけば良かったのに．
  if ( port_num == 0 && n == 1 ) {
    const PtConnection* con = pt_inst->port(0);
    if ( /* con->attr_top() == NULL &&*/
	 con->name() == NULL &&
	 con->expr() == NULL ) {
      // この要素は無視する．
      return;
    }
  }
  if ( n > port_num ) {
    ostringstream buf;
    buf << "Too many items (" << n << ") in the port list.";
    put_msg(__FILE__, __LINE__,
	    fr,
	    kMsgError,
	    "ELAB",
	    buf.str());
    return;
  }
  // どうやら実際のポート数よりも少ないのはいいらしい

  // YACC の文法から一つでも named_con なら全部そう
  bool conn_by_name = (pt_inst->port(0)->name() != NULL);
  hash_map<string, int> port_index;
  if ( conn_by_name ) {
    // ポート名とインデックスの辞書を作る．
    ymuint n = pt_module->port_num();
    for (ymuint index = 0; index < n; ++ index) {
      const PtPort* pt_port = pt_module->port(index);
      const char* name = pt_port->ext_name();
      if ( name != NULL ) {
	port_index.insert(make_pair(name, index));
      }
    }
  }

  ElbEnv env;
  for (ymuint i = 0; i < n; ++ i) {
    const PtConnection* pt_con = pt_inst->port(i);
    const PtExpr* pt_expr = pt_con->expr();
    if ( !pt_expr ) {
      continue;
    }

    ymuint index = i;
    if ( conn_by_name ) {
      const char* port_name = pt_con->name();
      assert_cond(port_name != NULL, __FILE__, __LINE__);
      hash_map<string, int>::iterator p = port_index.find(port_name);
      if ( p == port_index.end() ) {
	ostringstream buf;
	buf << port_name << " : does not exist in the port list.";
	put_msg(__FILE__, __LINE__,
		pt_con->file_region(),
		kMsgError,
		"ELAB",
		buf.str());
	continue;
      }
      index = p->second;
      assert_cond(index < port_num, __FILE__, __LINE__);
    }
    else {
      assert_cond(pt_con->name() == NULL, __FILE__, __LINE__);
    }

    ElbExpr* tmp = instantiate_expr(parent, env, pt_expr);
    if ( !tmp ) {
      continue;
    }
    
    const VlPort* port = module0->port(index);
    if ( port == NULL ) {
      // このポートはダミー
      continue;
    }

    ymuint port_size = port->bit_size();
    tVpiValueType type = tmp->value_type();
    if ( type == kVpiValueReal ) {
      put_msg(__FILE__, __LINE__,
	      tmp->file_region(),
	      kMsgError,
	      "ELAB",
	      "Real expression cannot connect to module port.");
      continue;
    }
    ymuint expr_size = unpack_size(type);

    // 配列型インスタンスの場合 expr_size に制限がある．
    if ( port_size == expr_size ) {
      // サイズが等しい場合はそのまま接続する．
      for (ymuint i = 0; i < module_size; ++ i) {
	ElbModule* module1 = module_array->_module(i);
	module1->set_port_high_conn(index, tmp, conn_by_name);
      }
    }
    else if ( expr_size == 0 ) {
      // もともとサイズがなければ port_size に合わせる．
      tmp->set_reqsize(pack(kVpiValueUS, port_size));
      for (ymuint i = 0; i < module_size; ++ i) {
	ElbModule* module1 = module_array->_module(i);
	module1->set_port_high_conn(index, tmp, conn_by_name);
      }
    }
    else if ( port_size * module_size == expr_size ) {
      // tmp を 分割する．
      for (ymuint i = 0; i < module_size; ++ i) {
	ElbModule* module1 = module_array->_module(i);
	int lsb = i;
	int msb = lsb + port_size - 1;
	ElbExpr* part = factory().new_PartSelect(pt_expr, tmp, msb, lsb);
	module1->set_port_high_conn(index, part, conn_by_name);
      }
    }
    else {
      ostringstream buf;
      buf << module_array->full_name() << " : "
	  << (index + 1) << num_suffix(index + 1)
	  << " port : port size does not match with the expression.";
      put_msg(__FILE__, __LINE__,
	      pt_expr->file_region(),
	      kMsgError,
	      "ELAB",
	      buf.str());
    }
    
    // attribute の設定を行う．
#if 0
    for (ymuint i = 0; i < module_size; ++ i) {
      ElbModule* module1 = module_array->_module(i);
      const VlPort* port = module1->port(index);

      // attribute instance の生成
      instantiate_attribute(pt_con->attr_top(), false, port);
    }
#endif
  }
}

// @brief module instance の入出力端子の接続を行う．
// @param[in] pt_module モジュール定義
// @param[in] pt_inst インスタンス定義
// @param[in] module モジュール
void
ItemGen::link_module(ElbModule* module,
		     const PtModule* pt_module,
		     const PtInst* pt_inst)
{
  const VlNamedObj* parent = module->parent();
  
  const FileRegion& fr = pt_inst->file_region();

  ymuint port_num = module->port_num();

  ymuint n = pt_inst->port_num();
  // ポートの割り当てを行う．
  // 例外: ポートを一つも取らないモジュールの場合
  // module_name instance_name ()
  // という形になって一つのポートが省略された形と区別がつかない．
  // これは Verilog-HDL の仕様がアホ
  // () を取らない形を用意しておけば良かったのに．
  if ( port_num == 0 && n == 1 ) {
    const PtConnection* pt_con = pt_inst->port(0);
    if ( /* pt_con->attr_top() == NULL &&*/
	 pt_con->name() == NULL &&
	 pt_con->expr() == NULL ) {
      // この要素は無視する．
      return;
    }
  }
  if ( n > port_num ) {
    ostringstream buf;
    buf << "Too many items (" << n << ") in the port list.";
    put_msg(__FILE__, __LINE__,
	    fr,
	    kMsgError,
	    "ELAB",
	    buf.str());
    return;
  }
  // どうやら実際のポート数よりも少ないのはいいらしい

  // YACC の文法から一つでも named_con なら全部そう
  bool conn_by_name = (pt_inst->port(0)->name() != NULL);
  hash_map<string, int> port_index;
  if ( conn_by_name ) {
    // ポート名とインデックスの辞書を作る．
    ymuint n = pt_module->port_num();
    for (ymuint index = 0; index < n; ++ index) {
      const PtPort* pt_port = pt_module->port(index);
      const char* name = pt_port->ext_name();
      if ( name != NULL ) {
	port_index.insert(make_pair(name, index));
      }
    }
  }
  
  ElbEnv env;
  for (ymuint i = 0; i < n; ++ i) {
    const PtConnection* pt_con = pt_inst->port(i);
    const PtExpr* pt_expr = pt_con->expr();
    if ( !pt_expr ) {
      continue;
    }

    ymuint index = i;
    if ( conn_by_name ) {
      const char* port_name = pt_con->name();
      assert_cond(port_name != NULL, __FILE__, __LINE__);
      hash_map<string, int>::iterator p = port_index.find(port_name);
      if ( p == port_index.end() ) {
	ostringstream buf;
	buf << port_name << " : does not exist in the port list.";
	put_msg(__FILE__, __LINE__,
		pt_con->file_region(),
		kMsgError,
		"ELAB",
		buf.str());
	continue;
      }
      index = p->second;
      assert_cond(index < port_num, __FILE__, __LINE__);
    }
    else {
      assert_cond(pt_con->name() == NULL, __FILE__, __LINE__);
    }
    
    ElbExpr* tmp = instantiate_expr(parent, env, pt_expr);
    if ( !tmp ) {
      continue;
    }

    const VlPort* port = module->port(index);
    if ( !port ) {
      // このポートはダミー
      continue;
    }

    ymuint port_size = port->bit_size();
    tVpiValueType type = tmp->value_type();
    ymuint expr_size = unpack_size(type);

    // 単独のインスタンスの場合 expr のサイズは補正される．
    // ... でいいんだよね．
    if ( port_size != expr_size ) {
      if ( expr_size != 0 ) {
	ostringstream buf;
	buf << module->full_name() << " : "
	    << (index + 1) << num_suffix(index + 1)
	    << " port : port size does not match with the expression. "
	    << "expression size is coereced.";
	put_msg(__FILE__, __LINE__,
		pt_expr->file_region(),
		kMsgWarning,
		"ELAB",
		buf.str());
	ostringstream buf2;
	buf2 << "port_size: " << port_size << ", expr_size: " << expr_size;
	put_msg(__FILE__, __LINE__,
		pt_expr->file_region(),
		kMsgDebug,
		"ELAB",
		buf2.str());
      }
      tmp->set_reqsize(pack(kVpiValueUS, port_size));
    }
    module->set_port_high_conn(index, tmp, conn_by_name);

    // attribute instance の生成
    //instantiate_attribute(pt_con->attr_top(), false, port);
  }
}

END_NAMESPACE_YM_VERILOG
