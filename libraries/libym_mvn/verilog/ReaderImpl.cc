
/// @file libym_mvn/verilog/ReaderImpl.cc
/// @brief ReaderImpl の実装クラス
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ReaderImpl.h"
#include "DeclMap.h"
#include "Driver.h"
#include "ym_mvn/MvMgr.h"
#include "ym_mvn/MvModule.h"
#include "ym_mvn/MvPort.h"
#include "ym_mvn/MvNode.h"
#include "ym_verilog/BitVector.h"
#include "ym_verilog/vl/VlModule.h"
#include "ym_verilog/vl/VlPrimitive.h"
#include "ym_verilog/vl/VlUdp.h"
#include "ym_verilog/vl/VlIODecl.h"
#include "ym_verilog/vl/VlDecl.h"
#include "ym_verilog/vl/VlPort.h"
#include "ym_verilog/vl/VlContAssign.h"
#include "ym_verilog/vl/VlExpr.h"
#include "ym_verilog/vl/VlRange.h"


BEGIN_NAMESPACE_YM_MVN_VERILOG

using namespace nsYm::nsVerilog;


BEGIN_NONAMESPACE

const
bool debug_driver = false;

END_NONAMESPACE


// @brief コンストラクタ
ReaderImpl::ReaderImpl() :
  mVlMgr(mMsgMgr)
{
  mMvMgr = NULL;
}

// @brief デストラクタ
ReaderImpl::~ReaderImpl()
{
}

// @brief 内部のデータをクリアする．
void
ReaderImpl::clear()
{
  mVlMgr.clear();
  mFFInfoDict.clear();
}

// @brief verilog 形式のファイルを読み込む．
// @param[in] filename ファイル名
// @param[in] searchpath サーチパス
// @param[in] watcher_list 行番号ウォッチャーのリスト
// @retval true 正常に読み込めた．
// @retval false 読込中にエラーが起こった．
bool
ReaderImpl::read(const string& filename,
		 const SearchPathList& searchpath,
		 const list<VlLineWatcher*> watcher_list)
{
  return mVlMgr.read_file(filename, searchpath, watcher_list);
}

// @brief 今まで読み込んだ情報からネットワークを生成する．
// @param[in] mgr ネットワーク生成用のマネージャ
// @retval true 正常に処理を行った．
// @retval false 生成中にエラーが起こった．
bool
ReaderImpl::gen_network(MvMgr& mgr,
			vector<pair<const VlDecl*, ymuint> >& node_map)
{
  mVlMgr.elaborate();

  mMvMgr = &mgr;

  mDeclMap.clear();
  mIODeclMap.clear();
  mNodeMap.clear();
  mDriverList.clear();

  MvModule* module0 = NULL;
  list<const VlModule*> tmp_list(mVlMgr.topmodule_list());
  for (list<const VlModule*>::const_iterator p = tmp_list.begin();
       p != tmp_list.end(); ++ p) {
    const VlModule* vl_module = *p;

    if ( vl_module->is_cell_instance() ) continue;

    // module を実体化
    MvModule* module = gen_module(vl_module);
    if ( module == NULL ) {
      return false;
    }
    if ( module0 == NULL ) {
      module0 = module;
    }
    else {
      cerr << "more than one top modules" << endl;
    }
  }

  // 結線を行う．
  ymuint n = mMvMgr->max_node_id();
  for (ymuint i = 0; i < n; ++ i) {
    MvNode* node = mMvMgr->_node(i);
    if ( node == NULL ) continue;
    const vector<Driver>& dlist = driver_list(node);
    if ( dlist.empty() ) continue;
    ymuint bw = node->input(0)->bit_width();
    vector<Driver> tmp(bw);
    for (vector<Driver>::const_iterator p = dlist.begin();
	 p != dlist.end(); ++ p) {
      const Driver& driver = *p;
      if ( driver.is_simple() ) {
	for (ymuint i = 0; i < bw; ++ i) {
	  if ( tmp[i].rhs_node() != NULL ) {
	    error_drivers(node, tmp[i], driver);
	  }
	  tmp[i] = driver;
	}
      }
      else if ( driver.has_bitselect() ) {
	ymuint index = driver.index();
	if ( tmp[index].rhs_node() != NULL ) {
	  error_drivers(node, tmp[index], driver);
	}
	tmp[index] = driver;
      }
      else {
	ymuint msb = driver.msb();
	ymuint lsb = driver.lsb();
	for (ymuint i = lsb; i <= msb; ++ i) {
	  if ( tmp[i].rhs_node() != NULL ) {
	    error_drivers(node, tmp[i], driver);
	  }
	  tmp[i] = driver;
	}
      }
    }

    // 明示的なドライバがない場合の処理
    for (ymuint i = 0; i < bw; ++ i) {
      if ( tmp[i].rhs_node() == NULL ) {
#warning "TODO: warning メッセージを出すようにする．"
	vector<ymuint32> val(1, 0);
	MvNode* ud_node = mMvMgr->new_const(module0, 1, val);
	if ( bw == 1 ) {
	  tmp[i] = Driver(ud_node);
	}
	else {
	  tmp[i] = Driver(ud_node, i);
	}
      }
    }

    Driver prev;
    vector<Driver> tmp2;
    tmp2.reserve(bw);
    for (ymuint i = 0; i < bw; ++ i) {
      ymuint idx = bw - i - 1;
      const Driver& driver = tmp[idx];
      if ( driver != prev ) {
	tmp2.push_back(driver);
	prev = driver;
	assert_cond( driver.rhs_node() != NULL, __FILE__, __LINE__);
      }
    }

    ymuint nd = tmp2.size();
    if ( nd == 1 ) {
      const Driver& driver = tmp2[0];
      MvNode* src_node = driver.rhs_node();
      mMvMgr->connect(src_node, 0, node, 0);
    }
    else {
      vector<ymuint> bw_array(nd);
      for (ymuint i = 0; i < nd; ++ i) {
	const Driver& driver = tmp2[i];
	assert_cond( driver.rhs_node() != NULL, __FILE__, __LINE__);
	if ( driver.has_bitselect() ) {
	  bw_array[i] = 1;
	}
	else if ( driver.has_partselect() ) {
	  bw_array[i] = driver.msb() - driver.lsb() + 1;
	}
	else {
	  assert_not_reached(__FILE__, __LINE__);
	}
      }
      MvNode* node1 = mMvMgr->new_concat(module0, bw_array);
      mMvMgr->connect(node1, 0, node, 0);
      for (ymuint i = 0; i < nd; ++ i) {
	const Driver& driver = tmp2[i];
	mMvMgr->connect(driver.rhs_node(), 0, node1, i);
      }
    }
  }

  // 冗長な through ノードを取り除く
  {
    vector<MvNode*> node_list;
    node_list.reserve(n);
    for (ymuint i = 0; i < n; ++ i) {
      MvNode* node = mMvMgr->_node(i);
      if ( node == NULL ) continue;
      if ( node->type() != MvNode::kThrough ) continue;
      node_list.push_back(node);
    }
    for (vector<MvNode*>::iterator p = node_list.begin();
	 p != node_list.end(); ++ p) {
      MvNode* node = *p;

      const MvInputPin* ipin = node->input(0);
      const MvOutputPin* src_pin = ipin->src_pin();
      if ( src_pin == NULL ) continue;

      MvNode* src_node = src_pin->node();
      ymuint src_pos = src_pin->pos();
      mMvMgr->disconnect(src_node, src_pos, node, 0);
      mMvMgr->reconnect(node, 0, src_node, src_pos);
      expand_nodemap(src_node->id());
      mNodeMap[src_node->id()] = mNodeMap[node->id()];
      mMvMgr->delete_node(node);
    }
  }

  // 冗長な through ノードを削除する．
  mMvMgr->sweep();

  node_map = mNodeMap;

  return true;
}

// @brief メッセージハンドラを付加する．
void
ReaderImpl::add_msg_handler(MsgHandler* msg_handler)
{
  mMsgMgr.reg_handler(msg_handler);
}

// @brief フリップフロップのセル名，ピン名を設定する．
// @param[in] cell_name セル名
// @param[in] data_pin_name データ入力ピン名
// @param[in] clock_pin_name クロック入力ピン名
// @param[in] q_pin_name ノーマル出力ピン名
// @param[in] qn_pin_name 反転出力ピン名
// @param[in] set_pin_name セットピン名
// @param[in] reset_pin_name リセットピン名
// @note 存在しない場合には空文字列を渡す．
void
ReaderImpl::set_ffname(const string& cell_name,
		       const string& data_pin_name,
		       const string& clock_pin_name,
		       const string& q_pin_name,
		       const string& qn_pin_name,
		       const string& set_pin_name,
		       const string& reset_pin_name)
{
  mFFInfoDict.insert(make_pair(cell_name, FFInfo(data_pin_name,
						 clock_pin_name,
						 q_pin_name,
						 qn_pin_name,
						 set_pin_name,
						 reset_pin_name)));
}

// @brief module を生成する．
// @param[in] vl_module 対象のモジュール
MvModule*
ReaderImpl::gen_module(const VlModule* vl_module)
{
  // ポート数，入出力のビット幅を調べる．
  ymuint np = vl_module->port_num();
  ymuint nio_all = vl_module->io_num();
  ymuint ni = 0;
  ymuint no = 0;
  ymuint nio = 0;
  for (ymuint i = 0; i < nio_all; ++ i) {
    const VlIODecl* io = vl_module->io(i);
    switch ( io->direction() ) {
    case kVpiInput:  ++ ni; break;
    case kVpiOutput: ++ no; break;
    case kVpiInout:  ++ nio; break;
    default:
      mMsgMgr.put_msg(__FILE__, __LINE__,
		      io->file_region(),
		      kMsgError,
		      "MVN_VL01",
		      "Only Input/Output/Inout types are supported");
      return NULL;
    }
  }
  vector<ymuint> ibw_array(ni);
  vector<ymuint> obw_array(no);
  vector<ymuint> iobw_array(nio);
  ni = 0;
  no = 0;
  nio = 0;
  for (ymuint i = 0; i < nio_all; ++ i) {
    const VlIODecl* io = vl_module->io(i);
    switch ( io->direction() ) {
    case kVpiInput:  ibw_array[ni] = io->bit_size(); ++ ni; break;
    case kVpiOutput: obw_array[no] = io->bit_size(); ++ no; break;
    case kVpiInout:  iobw_array[nio] = io->bit_size(); ++ nio; break;
    default: break;
    }
  }

  MvModule* module = mMvMgr->new_module(vl_module->name(), np,
					ibw_array, obw_array, iobw_array);

  // 宣言要素を生成する．
  bool stat = gen_decl(module, vl_module);
  if ( !stat ) {
    return NULL;
  }

  // 要素を生成する．
  stat = gen_item(module, vl_module);
  if ( !stat ) {
    return NULL;
  }

  // 入出力ノードの対応表を作る．
  ymuint i1 = 0;
  ymuint i2 = 0;
  ymuint i3 = 0;
  for (ymuint i = 0; i < nio_all; ++ i) {
    const VlIODecl* io = vl_module->io(i);
    const VlDecl* decl = io->decl();
    MvNode* node = mDeclMap.get(decl);
    assert_cond( node != NULL, __FILE__, __LINE__);
    switch ( io->direction() ) {
    case kVpiInput:
      mMvMgr->connect(module->input(i1), 0, node, 0);
      reg_ionode(decl, module->input(i1));
      ++ i1;
      break;

    case kVpiOutput:
      mMvMgr->connect(node, 0, module->output(i2), 0);
      reg_ionode(decl, module->output(i2));
      ++ i2;
      break;

    case kVpiInout:
      mMvMgr->connect(module->inout(i3), 0, node, 0);
      reg_ionode(decl, module->inout(i3));
      ++ i3;
      break;

    default:
      break;
    }
  }

  // ポートの接続を行う．
  for (ymuint i = 0; i < np; ++ i) {
    const VlPort* port = vl_module->port(i);
    const VlExpr* expr = port->low_conn();
    if ( expr->is_operation() ) {
      assert_cond( expr->op_type() == kVpiConcatOp, __FILE__, __LINE__);
      ymuint n = expr->operand_num();
      mMvMgr->init_port(module, i, port->name(), n);
      for (ymuint j = 0; j < n; ++ j) {
	MvNode* node;
	ymuint msb;
	ymuint lsb;
	switch ( gen_portref(expr->operand(j), node, msb, lsb) ) {
	case 0:
	  mMvMgr->set_port_ref(module, i, j, node);
	  break;

	case 1:
	  mMvMgr->set_port_ref(module, i, j, node, msb);
	  break;

	case 2:
	  mMvMgr->set_port_ref(module, i, j, node, msb, lsb);
	  break;

	default:
	  assert_not_reached(__FILE__, __LINE__);
	  break;
	}
      }
    }
    else {
      mMvMgr->init_port(module, i, port->name(), 1);
      MvNode* node;
      ymuint msb;
      ymuint lsb;
      switch ( gen_portref(expr, node, msb, lsb) ) {
      case 0:
	mMvMgr->set_port_ref(module, i, 0, node);
	break;

      case 1:
	mMvMgr->set_port_ref(module, i, 0, node, msb);
	break;

      case 2:
	mMvMgr->set_port_ref(module, i, 0, node, msb, lsb);
	break;

      default:
	assert_not_reached(__FILE__, __LINE__);
	break;
      }
    }
  }

  return module;
}

// @brief 宣言要素を生成する．
// @param[in] module モジュール
// @param[in] vl_scope 対象のスコープ
// @retval true 成功した．
// @retval false エラーが起こった．
bool
ReaderImpl::gen_decl(MvModule* module,
		     const VlNamedObj* vl_scope)
{
  // ネットの生成
  {
    vector<const VlDecl*> net_list;
    if ( mVlMgr.find_decl_list(vl_scope, vpiNet, net_list) ) {
      for (vector<const VlDecl*>::iterator p = net_list.begin();
	   p != net_list.end(); ++ p) {
	const VlDecl* vl_decl = *p;
	// 仮の through ノードに対応させる．
	ymuint bw = vl_decl->bit_size();
	MvNode* node = mMvMgr->new_through(module, bw);
	reg_node(vl_decl, node);
      }
    }
  }

  // ネット配列の生成
  {
    vector<const VlDecl*> netarray_list;
    if ( mVlMgr.find_decl_list(vl_scope, vpiNetArray, netarray_list) ) {
      for (vector<const VlDecl*>::iterator p = netarray_list.begin();
	   p != netarray_list.end(); ++ p) {
	const VlDecl* vl_decl = *p;
	ymuint d = vl_decl->dimension();
	ymuint array_size = 1;
	for (ymuint i = 0; i < d; ++ i) {
	  array_size *= vl_decl->range(i)->size();
	}
	for (ymuint i = 0; i < array_size; ++ i) {
	  // 仮の through ノードに対応させる．
	  ymuint bw = vl_decl->bit_size();
	  MvNode* node = mMvMgr->new_through(module, bw);
	  reg_node(vl_decl, i, node);
	}
      }
    }
  }

  // 内部スコープ要素の生成
  {
    vector<const VlNamedObj*> scope_list;
    if ( mVlMgr.find_genblock_list(vl_scope, scope_list) ) {
      for (vector<const VlNamedObj*>::iterator p = scope_list.begin();
	   p != scope_list.end(); ++ p) {
	const VlNamedObj* vl_scope1 = *p;
	bool stat = gen_decl(module, vl_scope1);
	if ( !stat ) {
	  return false;
	}
      }
    }
  }

  return true;
}

// @brief 要素を生成する．
// @param[in] module モジュール
// @param[in] vl_scope 対象のスコープ
// @retval true 成功した．
// @retval false エラーが起こった．
bool
ReaderImpl::gen_item(MvModule* module,
		     const VlNamedObj* vl_scope)
{
  // モジュールインスタンスの生成
  {
    vector<const VlModule*> module_list;
    if ( mVlMgr.find_module_list(vl_scope, module_list) ) {
      for (vector<const VlModule*>::iterator p = module_list.begin();
	   p != module_list.end(); ++ p) {
	const VlModule* vl_module = *p;
	gen_moduleinst(vl_module, module);
      }
    }
  }

  // モジュール配列インスタンスの生成
  {
    vector<const VlModuleArray*> modulearray_list;
    if ( mVlMgr.find_modulearray_list(vl_scope, modulearray_list) ) {
      for (vector<const VlModuleArray*>::iterator p = modulearray_list.begin();
	   p != modulearray_list.end(); ++ p) {
	const VlModuleArray* vl_module_array = *p;
	ymuint n = vl_module_array->elem_num();
	for (ymuint i = 0; i < n; ++ i) {
	  const VlModule* vl_module = vl_module_array->elem_by_offset(i);
	  gen_moduleinst(vl_module, module);
	}
      }
    }
  }

  // プリミティブインスタンスの生成
  {
    vector<const VlPrimitive*> primitive_list;
    if ( mVlMgr.find_primitive_list(vl_scope, primitive_list) ) {
      for (vector<const VlPrimitive*>::iterator p = primitive_list.begin();
	   p != primitive_list.end(); ++ p) {
	const VlPrimitive* vl_prim = *p;
	gen_priminst(vl_prim, module);
      }
    }
  }

  // プリミティブ配列インスタンスの生成
  {
    vector<const VlPrimArray*> primarray_list;
    if ( mVlMgr.find_primarray_list(vl_scope, primarray_list) ) {
      for (vector<const VlPrimArray*>::iterator p = primarray_list.begin();
	   p != primarray_list.end(); ++ p) {
	const VlPrimArray* vl_primarray = *p;
	ymuint n = vl_primarray->elem_num();
	for (ymuint i = 0; i < n; ++ i) {
	  const VlPrimitive* vl_prim = vl_primarray->elem_by_offset(i);
	  gen_priminst(vl_prim, module);
	}
      }
    }
  }

  // 継続的代入文の生成
  {
    vector<const VlContAssign*> contassign_list;
    if ( mVlMgr.find_contassign_list(vl_scope, contassign_list) ) {
      for (vector<const VlContAssign*>::iterator p = contassign_list.begin();
	   p != contassign_list.end(); ++ p) {
	const VlContAssign* vl_contassign = *p;
	const VlExpr* lhs = vl_contassign->lhs();
	const VlExpr* rhs = vl_contassign->rhs();
	MvNode* node = gen_expr1(module, rhs);
	connect_lhs(module, lhs, node);
      }
    }
  }

  // 内部スコープ要素の生成
  {
    vector<const VlNamedObj*> scope_list;
    if ( mVlMgr.find_genblock_list(vl_scope, scope_list) ) {
      for (vector<const VlNamedObj*>::iterator p = scope_list.begin();
	   p != scope_list.end(); ++ p) {
	const VlNamedObj* vl_scope1 = *p;
	bool stat = gen_item(module, vl_scope1);
	if ( !stat ) {
	  return false;
	}
      }
    }
  }

  return true;
}

// @brief portref の実体化を行う．
// @param[in] expr 対象の式
// @param[out] node 対応するノードを格納する変数
// @param[out] msb ビット指定位置か範囲指定の MSB を格納する変数
// @param[out] lsb 範囲指定の LSB を格納する変数
// @retval 0 単純な形式だった．
// @retval 1 ビット指定形式だった．
// @retval 2 範囲指定形式だった．
int
ReaderImpl::gen_portref(const VlExpr* expr,
			MvNode*& node,
			ymuint& msb,
			ymuint& lsb)
{
  const VlDecl* decl = expr->decl_obj();
  assert_cond( decl != NULL, __FILE__, __LINE__);
  node = mIODeclMap.get(decl);
  if ( node == NULL ) {
    cerr << decl->full_name() << " is not found in mIODeclMap" << endl;
  }
  assert_cond( node != NULL, __FILE__, __LINE__);

  if ( expr->is_bitselect() ) {
    assert_cond( node != NULL, __FILE__, __LINE__);
    assert_cond( expr->is_constant_select(), __FILE__, __LINE__);
    assert_cond( expr->declarray_dimension() == 0, __FILE__, __LINE__);
    msb = expr->index_val();
    return 1;
  }
  if ( expr->is_partselect() ) {
    assert_cond( node != NULL, __FILE__, __LINE__);
    assert_cond( expr->is_constant_select(), __FILE__, __LINE__);
    assert_cond( expr->declarray_dimension() == 0, __FILE__, __LINE__);
    msb = expr->left_range_val();
    lsb = expr->right_range_val();
    return 2;
  }
  return 0;
}

// @brief モジュールインスタンスの生成を行う．
// @param[in] vl_module モジュール
// @param[in] parent_module 親のモジュール
// @return 対応するノードを返す．
void
ReaderImpl::gen_moduleinst(const VlModule* vl_module,
			   MvModule* parent_module)
{
  hash_map<string, FFInfo>::const_iterator p
    = mFFInfoDict.find(vl_module->def_name());
  if ( p != mFFInfoDict.end() ) {
    // 特例．FFセルに置き換える．
    const FFInfo& ff_info = p->second;
    MvNode* node = mMvMgr->new_dff1(parent_module, 1);
    ymuint np = vl_module->port_num();
    for (ymuint i = 0; i < np; ++ i) {
      const VlPort* vl_port = vl_module->port(i);
      string port_name = vl_port->name();
      const VlExpr* expr = vl_port->high_conn();
      if ( port_name == ff_info.mDataPinName ) {
	MvNode* node1 = gen_expr1(parent_module, expr);
	mMvMgr->connect(node1, 0, node, 0);
      }
      else if ( port_name == ff_info.mClockPinName ) {
	MvNode* node1 = gen_expr1(parent_module, expr);
	mMvMgr->connect(node1, 0, node, 1);
      }
      else if ( port_name == ff_info.mResetPinName ) {
	if ( expr != NULL ) {
	  MvNode* node1 = gen_expr1(parent_module, expr);
	  mMvMgr->connect(node1, 0, node, 2);
	}
      }
      else if ( port_name == ff_info.mSetPinName ) {
	if ( expr != NULL ) {
	  MvNode* node1 = gen_expr1(parent_module, expr);
	  mMvMgr->connect(node1, 0, node, 3);
	}
      }
      else if ( port_name == ff_info.mQPinName ) {
	if ( expr != NULL ) {
	  connect_lhs(parent_module, expr, node);
	}
      }
      else if ( port_name == ff_info.mQnPinName ) {
	if ( expr != NULL ) {
	  MvNode* node1 = mMvMgr->new_not(parent_module, 1);
	  mMvMgr->connect(node, 0, node1, 0);
	  connect_lhs(parent_module, expr, node1);
	}
      }
      else {
	ostringstream buf;
	buf << port_name
	    << " : No such pin in "
	    << vl_module->def_name()
	    << ".";
	mMsgMgr.put_msg(__FILE__, __LINE__,
			vl_port->file_region(),
			kMsgError,
			"MVNXXX",
			buf.str());
      }
    }
    return;
  }

  // 通常の処理

  // 宣言要素を生成する．
  bool stat = gen_decl(parent_module, vl_module);
  if ( !stat ) {
    return;
  }

  // 要素を生成する．
  stat = gen_item(parent_module, vl_module);
  if ( !stat ) {
    return;
  }

  // ポートの接続を行う．
  ymuint np = vl_module->port_num();
  for (ymuint i = 0; i < np; ++ i) {
    const VlPort* vl_port = vl_module->port(i);
    const VlExpr* hi = vl_port->high_conn();
    if ( hi == NULL ) continue;
    const VlExpr* lo = vl_port->low_conn();
    switch ( vl_port->direction() ) {
    case kVpiInput:
      // hi は右辺式
      // lo は左辺式
      {
	MvNode* node1 = gen_expr1(parent_module, hi);
	connect_lhs(parent_module, lo, node1);
      }
      break;

    case kVpiOutput:
      // hi は左辺式
      // lo は右辺式
      {
	MvNode* node1 = gen_expr1(parent_module, lo);
	connect_lhs(parent_module, hi, node1);
      }
      break;

    case kVpiInout:
      // hi は単純な参照か連結のみ
      break;

    case kVpiMixedIO:
      // hi は単純な参照か連結のみ
      //connect_port2(port, hi);
      // TODO: connect_port2 を作る
      assert_not_reached(__FILE__, __LINE__);
      break;

    default:
      assert_not_reached(__FILE__, __LINE__);
      break;
    }
  }
}


// @brief 左辺式に接続する．
// @param[in] parent_module 親のモジュール
// @param[in] expr 左辺式
// @param[in] node 右辺に対応するノード
void
ReaderImpl::connect_lhs(MvModule* parent_module,
			const VlExpr* expr,
			MvNode* node)
{
  if ( expr->is_primary() ) {
    if ( node->output(0)->bit_width() != expr->bit_size() ) {
      cerr << "node->output(0)->bit_width() = "
	   << node->output(0)->bit_width()
	   << ", expr->bit_size() = "
	   << expr->bit_size()
	   << endl;
    }
    assert_cond( node->output(0)->bit_width() == expr->bit_size(),
		 __FILE__, __LINE__);
    MvNode* node1 = gen_expr2(expr);
    reg_driver(node1, Driver(node));
  }
  else if ( expr->is_bitselect() ) {
    assert_cond( node->output(0)->bit_width() == 1, __FILE__, __LINE__);
    assert_cond( expr->is_constant_select(), __FILE__, __LINE__);
    ymuint offset;
    MvNode* node1 = gen_expr3(expr, offset);
    reg_driver(node1, Driver(node, offset));
  }
  else if ( expr->is_partselect() ) {
    assert_cond( expr->is_constant_select(), __FILE__, __LINE__);
    ymuint msb;
    ymuint lsb;
    MvNode* node1 = gen_expr4(expr, msb, lsb);
    assert_cond( node->output(0)->bit_width() == msb - lsb + 1,
		 __FILE__, __LINE__);
    reg_driver(node1, Driver(node, msb, lsb));
  }
  else if ( expr->is_operation() ) {
    assert_cond( expr->op_type() == kVpiConcatOp, __FILE__, __LINE__);
    ymuint n = expr->operand_num();
    ymuint offset = expr->bit_size();
    for (ymuint i = 0; i < n; ++ i) {
      const VlExpr* expr1 = expr->operand(i);
      offset -= expr1->bit_size();
      connect_lhs_sub(parent_module, expr1, node, offset);
    }
  }
  else {
    assert_not_reached(__FILE__, __LINE__);
  }
}

// @brief 左辺式に接続する．
// @param[in] parent_module 親のモジュール
// @param[in] expr 左辺式
// @param[in] node 右辺に対応するノード
void
ReaderImpl::connect_lhs_sub(MvModule* parent_module,
			    const VlExpr* expr,
			    MvNode* node,
			    ymuint offset)
{
  ymuint bw = node->output(0)->bit_width();
  if ( expr->is_primary() ) {
    MvNode* node1 = gen_expr2(expr);
    ymuint bw1 = node1->input(0)->bit_width();
    MvNode* node2 = NULL;
    if ( bw1 == 1 ) {
      node2 = mMvMgr->new_constbitselect(parent_module,
					 offset,
					 bw);
    }
    else {
      node2 = mMvMgr->new_constpartselect(parent_module,
					  offset + bw1 - 1,
					  offset,
					  bw);
    }
    mMvMgr->connect(node, 0, node2, 0);
    mMvMgr->connect(node2, 0, node1, 0);
    assert_cond( node, __FILE__, __LINE__);
  }
  else if ( expr->is_bitselect() ) {
    ymuint index;
    MvNode* node1 = gen_expr3(expr, index);
    MvNode* node2 = mMvMgr->new_constbitselect(parent_module,
					       offset,
					       bw);
    assert_cond( node, __FILE__, __LINE__);
    mMvMgr->connect(node, 0, node2, 0);
    reg_driver(node1, Driver(node2, index));
  }
  else if ( expr->is_partselect() ) {
    ymuint msb;
    ymuint lsb;
    MvNode* node1 = gen_expr4(expr, msb, lsb);
    ymuint bw1 = msb - lsb + 1;
    MvNode* node2 = mMvMgr->new_constpartselect(parent_module,
						offset + bw1 - 1,
						offset,
						bw);
    mMvMgr->connect(node, 0, node2, 0);
    reg_driver(node1, Driver(node2, msb, lsb));
  }
  else if ( expr->is_operation() ) {
    assert_cond( expr->op_type() == kVpiConcatOp, __FILE__, __LINE__);
    ymuint n = expr->operand_num();
    ymuint offset1 = expr->bit_size();
    for (ymuint i = 0; i < n; ++ i) {
      const VlExpr* expr1 = expr->operand(i);
      offset1 -= expr1->bit_size();
      connect_lhs_sub(parent_module, expr1, node, offset + offset1);
    }
  }
  else {
    assert_not_reached(__FILE__, __LINE__);
  }
}

// @brief プリミティブインスタンスの生成を行う．
// @param[in] prim プリミティブ
// @param[in] parent_module 親のモジュール
void
ReaderImpl::gen_priminst(const VlPrimitive* prim,
			 MvModule* parent_module)
{
  ymuint nt = prim->port_num();
  ymuint ni = 0;
  ymuint no = 0;
  if ( prim->prim_type() == kVpiBufPrim ) {
    ni = 1;
    no = nt - 1;
  }
  else {
    ni = nt - 1;
    no = 1;
  }

  vector<pair<MvNode*, ymuint> > inputs(ni);
  vector<MvNode*> outputs(no);

  switch ( prim->prim_type() ) {
  case kVpiBufPrim:
    {
      MvNode* node = mMvMgr->new_through(parent_module, 1);
      inputs[0] = make_pair(node, 0);
      for (ymuint i = 0; i < no; ++ i) {
	outputs[i] = node;
      }
    }
    break;

  case kVpiNotPrim:
    {
      MvNode* node = mMvMgr->new_not(parent_module, 1);
      inputs[0] = make_pair(node, 0);
      outputs[0] = node;
    }
    break;

  case kVpiAndPrim:
    {
      MvNode* node = gen_andtree(parent_module, ni, inputs, 0);
      outputs[0] = node;
    }
    break;

  case kVpiNandPrim:
    {
      MvNode* node = gen_andtree(parent_module, ni, inputs, 0);
      MvNode* node1 = mMvMgr->new_not(parent_module, 1);
      mMvMgr->connect(node, 0, node1, 0);
      outputs[0] = node1;
    }
    break;

  case kVpiOrPrim:
    {
      MvNode* node = gen_ortree(parent_module, ni, inputs, 0);
      outputs[0] = node;
    }
    break;

  case kVpiNorPrim:
    {
      MvNode* node = gen_ortree(parent_module, ni, inputs, 0);
      MvNode* node1 = mMvMgr->new_not(parent_module, 1);
      mMvMgr->connect(node, 0, node1, 0);
      outputs[0] = node1;
    }
    break;

  case kVpiXorPrim:
    {
      MvNode* node = gen_xortree(parent_module, ni, inputs, 0);
      outputs[0] = node;
    }
    break;

  case kVpiXnorPrim:
    {
      MvNode* node = gen_xortree(parent_module, ni, inputs, 0);
      MvNode* node1 = mMvMgr->new_not(parent_module, 1);
      mMvMgr->connect(node, 0, node1, 0);
      outputs[0] = node1;
    }
    break;

  case kVpiCombPrim:
    {
      assert_not_reached(__FILE__, __LINE__);
      const VlUdpDefn* udp = prim->udp_defn();
      ymuint ni = udp->port_num() - 1;
      MvNode* node = mMvMgr->new_combudp(parent_module, ni);
      for (ymuint i = 0; i < ni; ++ i) {
	inputs[i] = make_pair(node, i);
      }
      outputs[0] = node;
    }
    break;

  case kVpiSeqPrim:
    {
      assert_not_reached(__FILE__, __LINE__);
      const VlUdpDefn* udp = prim->udp_defn();
      ymuint ni = udp->port_num() - 1;
      MvNode* node = mMvMgr->new_sequdp(parent_module, ni);
      for (ymuint i = 0; i < ni; ++ i) {
	inputs[i] = make_pair(node, i);
      }
      outputs[0] = node;
    }
    break;

  default:
    assert_not_reached(__FILE__, __LINE__);
    break;
  }

  ymuint pos = 0;
  for (ymuint i = 0; i < no; ++ i) {
    const VlPrimTerm* term = prim->prim_term(pos);
    ++ pos;
    const VlExpr* expr = term->expr();
    connect_lhs(parent_module, expr, outputs[i]);
  }
  for (ymuint i = 0; i < ni; ++ i) {
    const VlPrimTerm* term = prim->prim_term(pos);
    ++ pos;
    const VlExpr* expr = term->expr();
    MvNode* node = gen_expr1(parent_module, expr);
    const pair<MvNode*, ymuint>& p = inputs[i];
    mMvMgr->connect(node, 0, p.first, p.second);
  }
}

// @brief AND のバランス木を作る．
// @param[in] parent_module 親のモジュール
// @param[in] ni 入力数
// @param[in] inputs 入力ピンを格納する配列
// @param[in] offset inputs のオフセット
MvNode*
ReaderImpl::gen_andtree(MvModule* parent_module,
			ymuint ni,
			vector<pair<MvNode*, ymuint> >& inputs,
			ymuint offset)
{
  assert_cond( ni > 1, __FILE__, __LINE__);

  if ( ni == 2 ) {
    MvNode* node = mMvMgr->new_and(parent_module, 1);
    inputs[offset + 0] = make_pair(node, 0);
    inputs[offset + 1] = make_pair(node, 1);
    return node;
  }

  if ( ni == 3 ) {
    MvNode* node0 = gen_andtree(parent_module, 2, inputs, offset);
    MvNode* node = mMvMgr->new_and(parent_module, 1);
    mMvMgr->connect(node0, 0, node, 0);
    inputs[offset + 2] = make_pair(node, 1);
    return node;
  }

  ymuint nr = ni / 2;
  ymuint nl = ni - nr;

  MvNode* node0 = gen_andtree(parent_module, nl, inputs, offset);
  MvNode* node1 = gen_andtree(parent_module, nr, inputs, offset + nl);

  MvNode* node = mMvMgr->new_and(parent_module, 1);
  mMvMgr->connect(node0, 0, node, 0);
  mMvMgr->connect(node1, 0, node, 1);

  return node;
}

// @brief OR のバランス木を作る．
// @param[in] parent_module 親のモジュール
// @param[in] ni 入力数
// @param[in] inputs 入力ピンを格納する配列
// @param[in] offset inputs のオフセット
MvNode*
ReaderImpl::gen_ortree(MvModule* parent_module,
		       ymuint ni,
		       vector<pair<MvNode*, ymuint> >& inputs,
		       ymuint offset)
{
  assert_cond( ni > 1, __FILE__, __LINE__);

  if ( ni == 2 ) {
    MvNode* node = mMvMgr->new_or(parent_module, 1);
    inputs[offset + 0] = make_pair(node, 0);
    inputs[offset + 1] = make_pair(node, 1);
    return node;
  }

  if ( ni == 3 ) {
    MvNode* node0 = gen_ortree(parent_module, 2, inputs, offset);
    MvNode* node = mMvMgr->new_or(parent_module, 1);
    mMvMgr->connect(node0, 0, node, 0);
    inputs[offset + 2] = make_pair(node, 1);
    return node;
  }

  ymuint nr = ni / 2;
  ymuint nl = ni - nr;

  MvNode* node0 = gen_ortree(parent_module, nl, inputs, offset);
  MvNode* node1 = gen_ortree(parent_module, nr, inputs, offset + nl);

  MvNode* node = mMvMgr->new_or(parent_module, 1);
  mMvMgr->connect(node0, 0, node, 0);
  mMvMgr->connect(node1, 0, node, 1);

  return node;
}

// @brief XOR のバランス木を作る．
// @param[in] parent_module 親のモジュール
// @param[in] ni 入力数
// @param[in] inputs 入力ピンを格納する配列
// @param[in] offset inputs のオフセット
MvNode*
ReaderImpl::gen_xortree(MvModule* parent_module,
			ymuint ni,
			vector<pair<MvNode*, ymuint> >& inputs,
			ymuint offset)
{
  assert_cond( ni > 1, __FILE__, __LINE__);

  if ( ni == 2 ) {
    MvNode* node = mMvMgr->new_xor(parent_module, 1);
    inputs[offset + 0] = make_pair(node, 0);
    inputs[offset + 1] = make_pair(node, 1);
    return node;
  }

  if ( ni == 3 ) {
    MvNode* node0 = gen_xortree(parent_module, 2, inputs, offset);
    MvNode* node = mMvMgr->new_xor(parent_module, 1);
    mMvMgr->connect(node0, 0, node, 0);
    inputs[offset + 2] = make_pair(node, 1);
    return node;
  }

  ymuint nr = ni / 2;
  ymuint nl = ni - nr;

  MvNode* node0 = gen_xortree(parent_module, nl, inputs, offset);
  MvNode* node1 = gen_xortree(parent_module, nr, inputs, offset + nl);

  MvNode* node = mMvMgr->new_xor(parent_module, 1);
  mMvMgr->connect(node0, 0, node, 0);
  mMvMgr->connect(node1, 0, node, 1);

  return node;
}

// @brief 式に対応したノードの木を作る．
// @param[in] parent_module 親のモジュール
// @param[in] expr 式
MvNode*
ReaderImpl::gen_expr1(MvModule* parent_module,
		      const VlExpr* expr)
{
  if ( expr->is_const() ) {
    assert_cond( is_bitvector_type(expr->value_type()), __FILE__, __LINE__);
    BitVector bv;
    expr->eval_bitvector(bv);
    ymuint bit_size = bv.size();
    ymuint n = (bit_size + 31) / 32;
    vector<ymuint32> tmp(n);
    for (ymuint i = 0; i < bit_size; ++ i) {
      ymuint blk = i / 32;
      ymuint pos = i % 32;
      switch ( bv.value(i) ) {
      case kVpiScalar1: tmp[blk] |= (1 << pos); break;
      case kVpiScalar0: break;
      case kVpiScalarX:
      case kVpiScalarZ:
	mMsgMgr.put_msg(__FILE__, __LINE__,
			expr->file_region(),
			kMsgError,
			"MVN_VLXXX",
			"'X' or 'Z' in constant expression");
	break;
      }
    }
    return mMvMgr->new_const(parent_module, bit_size, tmp);
  }
  if ( expr->is_primary() ) {
    return gen_expr2(expr);
  }
  if ( expr->is_bitselect() ) {
    if ( expr->is_constant_select() ) {
      ymuint bitpos;
      MvNode* node = gen_expr3(expr, bitpos);
      const MvOutputPin* pin = node->output(0);
      MvNode* node1 = mMvMgr->new_constbitselect(parent_module,
						 bitpos,
						 pin->bit_width());
      assert_cond( node, __FILE__, __LINE__);
      mMvMgr->connect(node, 0, node1, 0);
      return node1;
    }
    else {
#warning "TODO: [msb:lsb] のオフセット変換をしていない"
      MvNode* node = gen_expr2(expr);
      MvNode* node1 = gen_expr1(parent_module, expr->index());
      const MvOutputPin* pin0 = node->output(0);
      const MvOutputPin* pin1 = node1->output(0);
      MvNode* node2 = mMvMgr->new_bitselect(parent_module,
					    pin0->bit_width(),
					    pin1->bit_width());
      mMvMgr->connect(node, 0, node2, 0);
      mMvMgr->connect(node1, 0, node2, 1);
      return node2;
    }
  }
  if ( expr->is_partselect() ) {
    if ( expr->is_constant_select() ) {
      ymuint msb;
      ymuint lsb;
      MvNode* node = gen_expr4(expr, msb, lsb);
      const MvOutputPin* pin = node->output(0);
      MvNode* node1 = mMvMgr->new_constpartselect(parent_module,
						  msb, lsb,
						  pin->bit_width());
      mMvMgr->connect(node, 0, node1, 0);
      return node1;
    }
    else {
#warning "TODO: [msb:lsb] のオフセット変換をしていない"
      // まだできてない．
      // というか可変 part_select は VPI がおかしいと思う．
      assert_not_reached(__FILE__, __LINE__);
      return NULL;
    }
  }
  if ( expr->is_operation() ) {
    ymuint n = expr->operand_num();
    vector<MvNode*> inputs(n);
    for (ymuint i = 0; i < n; ++ i) {
      inputs[i] = gen_expr1(parent_module, expr->operand(i));
    }

    switch ( expr->op_type() ) {
    case kVpiNullOp:
      return inputs[0];

    case kVpiMinusOp:
      {
	MvNode* node = mMvMgr->new_cmpl(parent_module, expr->bit_size());
	mMvMgr->connect(inputs[0], 0, node, 0);
	return node;
      }

    case kVpiNotOp:
      {
	MvNode* node = mMvMgr->new_not(parent_module, 1);
	mMvMgr->connect(inputs[0], 0, node, 0);
	return node;
      }

    case kVpiBitNegOp:
      {
	MvNode* node = mMvMgr->new_not(parent_module, expr->bit_size());
	mMvMgr->connect(inputs[0], 0, node, 0);
	return node;
      }

    case kVpiPlusOp:
      return inputs[0];

    case kVpiUnaryAndOp:
      {
	ymuint bw = inputs[0]->output(0)->bit_width();
	MvNode* node = mMvMgr->new_rand(parent_module, bw);
	mMvMgr->connect(inputs[0], 0, node, 0);
	return node;
      }

    case kVpiUnaryNandOp:
      {
	ymuint bw = inputs[0]->output(0)->bit_width();
	MvNode* node = mMvMgr->new_rand(parent_module, bw);
	mMvMgr->connect(inputs[0], 0, node, 0);
	MvNode* node1 = mMvMgr->new_not(parent_module, 1);
	mMvMgr->connect(node, 0, node1, 0);
	return node1;
      }

    case kVpiUnaryOrOp:
      {
	ymuint bw = inputs[0]->output(0)->bit_width();
	MvNode* node = mMvMgr->new_ror(parent_module, bw);
	mMvMgr->connect(inputs[0], 0, node, 0);
	return node;
      }

    case kVpiUnaryNorOp:
      {
	ymuint bw = inputs[0]->output(0)->bit_width();
	MvNode* node = mMvMgr->new_ror(parent_module, bw);
	mMvMgr->connect(inputs[0], 0, node, 0);
	MvNode* node1 = mMvMgr->new_not(parent_module, 1);
	mMvMgr->connect(node, 0, node1, 0);
	return node1;
      }

    case kVpiUnaryXorOp:
      {
	ymuint bw = inputs[0]->output(0)->bit_width();
	MvNode* node = mMvMgr->new_rxor(parent_module, bw);
	mMvMgr->connect(inputs[0], 0, node, 0);
	return node;
      }

    case kVpiUnaryXNorOp:
      {
	ymuint bw = inputs[0]->output(0)->bit_width();
	MvNode* node = mMvMgr->new_rxor(parent_module, bw);
	mMvMgr->connect(inputs[0], 0, node, 0);
	MvNode* node1 = mMvMgr->new_not(parent_module, 1);
	mMvMgr->connect(node, 0, node1, 0);
	return node1;
      }

    case kVpiPosedgeOp:
    case kVpiNegedgeOp:
      assert_not_reached(__FILE__, __LINE__);
      break;

    case kVpiAddOp:
      {
	ymuint bw1 = inputs[0]->output(0)->bit_width();
	ymuint bw2 = inputs[1]->output(0)->bit_width();
	ymuint bw3 = expr->bit_size();
	MvNode* node = mMvMgr->new_add(parent_module, bw1, bw2, bw3);
	mMvMgr->connect(inputs[0], 0, node, 0);
	mMvMgr->connect(inputs[1], 0, node, 1);
	return node;
      }

    case kVpiSubOp:
      {
	ymuint bw1 = inputs[0]->output(0)->bit_width();
	ymuint bw2 = inputs[1]->output(0)->bit_width();
	ymuint bw3 = expr->bit_size();
	MvNode* node = mMvMgr->new_sub(parent_module, bw1, bw2, bw3);
	mMvMgr->connect(inputs[0], 0, node, 0);
	mMvMgr->connect(inputs[1], 0, node, 1);
	return node;
      }

    case kVpiMultOp:
      {
	ymuint bw1 = inputs[0]->output(0)->bit_width();
	ymuint bw2 = inputs[1]->output(0)->bit_width();
	ymuint bw3 = expr->bit_size();
	MvNode* node = mMvMgr->new_mult(parent_module, bw1, bw2, bw3);
	mMvMgr->connect(inputs[0], 0, node, 0);
	mMvMgr->connect(inputs[1], 0, node, 1);
	return node;
      }

    case kVpiDivOp:
      {
	ymuint bw1 = inputs[0]->output(0)->bit_width();
	ymuint bw2 = inputs[1]->output(0)->bit_width();
	ymuint bw3 = expr->bit_size();
	MvNode* node = mMvMgr->new_div(parent_module, bw1, bw2, bw3);
	mMvMgr->connect(inputs[0], 0, node, 0);
	mMvMgr->connect(inputs[1], 0, node, 1);
	return node;
      }

    case kVpiModOp:
      {
	ymuint bw1 = inputs[0]->output(0)->bit_width();
	ymuint bw2 = inputs[1]->output(0)->bit_width();
	ymuint bw3 = expr->bit_size();
	MvNode* node = mMvMgr->new_mod(parent_module, bw1, bw2, bw3);
	mMvMgr->connect(inputs[0], 0, node, 0);
	mMvMgr->connect(inputs[1], 0, node, 1);
	return node;
      }

    case kVpiPowerOp:
      {
	ymuint bw1 = inputs[0]->output(0)->bit_width();
	ymuint bw2 = inputs[1]->output(0)->bit_width();
	ymuint bw3 = expr->bit_size();
	MvNode* node = mMvMgr->new_pow(parent_module, bw1, bw2, bw3);
	mMvMgr->connect(inputs[0], 0, node, 0);
	mMvMgr->connect(inputs[1], 0, node, 1);
	return node;
      }

    case kVpiLShiftOp:
      {
	ymuint bw1 = inputs[0]->output(0)->bit_width();
	ymuint bw2 = inputs[1]->output(0)->bit_width();
	ymuint bw3 = expr->bit_size();
	MvNode* node = mMvMgr->new_sll(parent_module, bw1, bw2, bw3);
	mMvMgr->connect(inputs[0], 0, node, 0);
	mMvMgr->connect(inputs[1], 0, node, 1);
	return node;
      }

    case kVpiRShiftOp:
      {
	ymuint bw1 = inputs[0]->output(0)->bit_width();
	ymuint bw2 = inputs[1]->output(0)->bit_width();
	ymuint bw3 = expr->bit_size();
	MvNode* node = mMvMgr->new_srl(parent_module, bw1, bw2, bw3);
	mMvMgr->connect(inputs[0], 0, node, 0);
	mMvMgr->connect(inputs[1], 0, node, 1);
	return node;
      }

    case kVpiArithLShiftOp:
      {
	ymuint bw1 = inputs[0]->output(0)->bit_width();
	ymuint bw2 = inputs[1]->output(0)->bit_width();
	ymuint bw3 = expr->bit_size();
	MvNode* node = mMvMgr->new_sla(parent_module, bw1, bw2, bw3);
	mMvMgr->connect(inputs[0], 0, node, 0);
	mMvMgr->connect(inputs[1], 0, node, 1);
	return node;
      }

    case kVpiArithRShiftOp:
      {
	ymuint bw1 = inputs[0]->output(0)->bit_width();
	ymuint bw2 = inputs[1]->output(0)->bit_width();
	ymuint bw3 = expr->bit_size();
	MvNode* node = mMvMgr->new_sra(parent_module, bw1, bw2, bw3);
	mMvMgr->connect(inputs[0], 0, node, 0);
	mMvMgr->connect(inputs[1], 0, node, 1);
	return node;
      }

    case kVpiBitAndOp:
      {
	ymuint bw = expr->bit_size();
	MvNode* node = mMvMgr->new_and(parent_module, bw);
	mMvMgr->connect(inputs[0], 0, node, 0);
	mMvMgr->connect(inputs[1], 0, node, 1);
	return node;
      }

    case kVpiBitOrOp:
      {
	ymuint bw = expr->bit_size();
	MvNode* node = mMvMgr->new_or(parent_module, bw);
	mMvMgr->connect(inputs[0], 0, node, 0);
	mMvMgr->connect(inputs[1], 0, node, 1);
	return node;
      }

    case kVpiBitXNorOp:
      {
	ymuint bw = expr->bit_size();
	MvNode* node = mMvMgr->new_xor(parent_module, bw);
	mMvMgr->connect(inputs[0], 0, node, 0);
	mMvMgr->connect(inputs[1], 0, node, 1);
	MvNode* node1 = mMvMgr->new_not(parent_module, bw);
	mMvMgr->connect(node, 0, node1, 0);
	return node1;
      }

    case kVpiBitXorOp:
      {
	ymuint bw = expr->bit_size();
	MvNode* node = mMvMgr->new_xor(parent_module, bw);
	mMvMgr->connect(inputs[0], 0, node, 0);
	mMvMgr->connect(inputs[1], 0, node, 1);
	return node;
      }

    case kVpiLogAndOp:
      {
	MvNode* node = mMvMgr->new_and(parent_module, 1);
	mMvMgr->connect(inputs[0], 0, node, 0);
	mMvMgr->connect(inputs[1], 0, node, 1);
	return node;
      }

    case kVpiLogOrOp:
      {
	MvNode* node = mMvMgr->new_or(parent_module, 1);
	mMvMgr->connect(inputs[0], 0, node, 0);
	mMvMgr->connect(inputs[1], 0, node, 1);
	return node;
      }

    case kVpiCaseEqOp:
    case kVpiCaseNeqOp:
      assert_not_reached(__FILE__, __LINE__);
      break;

    case kVpiEqOp:
      {
	ymuint bw = inputs[0]->output(0)->bit_width();
	MvNode* node = mMvMgr->new_equal(parent_module, bw);
	mMvMgr->connect(inputs[0], 0, node, 0);
	mMvMgr->connect(inputs[1], 0, node, 1);
	return node;
      }

    case kVpiNeqOp:
      {
	ymuint bw = inputs[0]->output(0)->bit_width();
	MvNode* node = mMvMgr->new_equal(parent_module, bw);
	mMvMgr->connect(inputs[0], 0, node, 0);
	mMvMgr->connect(inputs[1], 0, node, 1);
	MvNode* node1 = mMvMgr->new_not(parent_module, 1);
	mMvMgr->connect(node, 0, node1, 0);
	return node1;
      }

    case kVpiLtOp:
      {
	ymuint bw = inputs[0]->output(0)->bit_width();
	MvNode* node = mMvMgr->new_lt(parent_module, bw);
	mMvMgr->connect(inputs[0], 0, node, 0);
	mMvMgr->connect(inputs[1], 0, node, 1);
	return node;
      }

    case kVpiGeOp:
      {
	ymuint bw = inputs[0]->output(0)->bit_width();
	MvNode* node = mMvMgr->new_lt(parent_module, bw);
	mMvMgr->connect(inputs[0], 0, node, 0);
	mMvMgr->connect(inputs[1], 0, node, 1);
	MvNode* node1 = mMvMgr->new_not(parent_module, 1);
	mMvMgr->connect(node, 0, node1, 0);
	return node1;
      }

    case kVpiGtOp:
      {
	ymuint bw = inputs[0]->output(0)->bit_width();
	MvNode* node = mMvMgr->new_lt(parent_module, bw);
	mMvMgr->connect(inputs[1], 0, node, 0);
	mMvMgr->connect(inputs[0], 0, node, 1);
	return node;
      }

    case kVpiLeOp:
      {
	ymuint bw = inputs[0]->output(0)->bit_width();
	MvNode* node = mMvMgr->new_lt(parent_module, bw);
	mMvMgr->connect(inputs[1], 0, node, 0);
	mMvMgr->connect(inputs[0], 0, node, 1);
	MvNode* node1 = mMvMgr->new_not(parent_module, 1);
	mMvMgr->connect(node, 0, node1, 0);
	return node1;
      }

    case kVpiConditionOp:
      {
	ymuint bw = expr->bit_size();
	MvNode* node = mMvMgr->new_ite(parent_module, bw);
	mMvMgr->connect(inputs[0], 0, node, 0);
	mMvMgr->connect(inputs[1], 0, node, 1);
	mMvMgr->connect(inputs[2], 0, node, 2);
	return node;
      }

    case kVpiMinTypMaxOp:
      assert_not_reached(__FILE__, __LINE__);
      break;

    case kVpiConcatOp:
      {
	ymuint n = expr->operand_num();
	vector<ymuint> bw_array(n);
	for (ymuint i = 0; i < n; ++ i) {
	  bw_array[i] = inputs[i]->output(0)->bit_width();
	}
	MvNode* node = mMvMgr->new_concat(parent_module, bw_array);
	for (ymuint i = 0; i < n; ++ i) {
	  mMvMgr->connect(inputs[i], 0, node, i);
	}
	return node;
      }

    case kVpiMultiConcatOp:
      {
	ymint r;
	bool stat = expr->operand(0)->eval_int(r);
	assert_cond( stat , __FILE__, __LINE__);
	ymuint n = expr->operand_num() - 1;
	vector<ymuint> bw_array(n * r);
	for (ymuint i = 0; i < n; ++ i) {
	  for (ymint j = 0; j < r; ++ j) {
	    bw_array[j * r + i] = inputs[i + 1]->output(0)->bit_width();
	  }
	}
	MvNode* node = mMvMgr->new_concat(parent_module, bw_array);
	for (ymuint i = 0; i < n; ++ i) {
	  for (ymint j = 0; j < r; ++ j) {
	    mMvMgr->connect(inputs[i + 1], 0, node, j * r + i);
	  }
	}
	return node;
      }

    default:
      assert_not_reached(__FILE__, __LINE__);
      break;
    }
  }
  assert_not_reached(__FILE__, __LINE__);
  return NULL;
}

// @brief 宣言要素への参照に対応するノードを作る．
// @param[in] expr 式
MvNode*
ReaderImpl::gen_expr2(const VlExpr* expr)
{
  const VlDecl* decl = expr->decl_obj();
  assert_cond( decl != NULL, __FILE__, __LINE__);
  ymuint dim = expr->declarray_dimension();
  assert_cond( decl->dimension() == dim, __FILE__, __LINE__);
  if ( dim > 1 ) {
    // 配列型
    ymuint offset = 0;
    ymuint mlt = 1;
    for (ymuint i = 0; i < dim; ++ i) {
      const VlExpr* index = expr->declarray_index(i);
      int index_val;
      bool stat = index->eval_int(index_val);
      assert_cond( stat, __FILE__, __LINE__);
      offset += index_val * mlt;
      mlt *= decl->range(i)->size();
    }
    MvNode* node = mDeclMap.get(decl, offset);
    if ( node == NULL ) {
      cerr << decl->name() << " is not found in mDeclMap" << endl;
    }
    assert_cond( node != NULL, __FILE__, __LINE__);
    return node;
  }
  else {
    MvNode* node = mDeclMap.get(decl);
    if ( node == NULL ) {
      cerr << decl->name() << " is not found in mDeclMap" << endl;
    }
    assert_cond( node != NULL, __FILE__, __LINE__);
    return node;
  }
}

// @brief 宣言要素への参照に対応するノードを作る．
// @param[in] expr 式
MvNode*
ReaderImpl::gen_expr3(const VlExpr* expr,
		      ymuint& bitpos)
{
  const VlDecl* decl = expr->decl_obj();
  assert_cond( decl != NULL, __FILE__, __LINE__);
  bitpos = decl->bit_offset(expr->index_val());
  ymuint dim = expr->declarray_dimension();
  assert_cond( decl->dimension() == dim, __FILE__, __LINE__);
  if ( dim > 1 ) {
    // 配列型
    ymuint offset = 0;
    ymuint mlt = 1;
    for (ymuint i = 0; i < dim; ++ i) {
      const VlExpr* index = expr->declarray_index(i);
      int index_val;
      bool stat = index->eval_int(index_val);
      assert_cond( stat, __FILE__, __LINE__);
      offset += index_val * mlt;
      mlt *= decl->range(i)->size();
    }
    MvNode* node = mDeclMap.get(decl, offset);
    if ( node == NULL ) {
      cerr << decl->name() << " is not found in mDeclMap" << endl;
    }
    assert_cond( node != NULL, __FILE__, __LINE__);
    return node;
  }
  else {
    MvNode* node = mDeclMap.get(decl);
    if ( node == NULL ) {
      cerr << decl->name() << " is not found in mDeclMap" << endl;
    }
    assert_cond( node != NULL, __FILE__, __LINE__);
    return node;
  }
}

// @brief 宣言要素への参照に対応するノードを作る．
// @param[in] expr 式
MvNode*
ReaderImpl::gen_expr4(const VlExpr* expr,
		      ymuint& msb,
		      ymuint& lsb)
{
  const VlDecl* decl = expr->decl_obj();
  assert_cond( decl != NULL, __FILE__, __LINE__);
  msb = decl->bit_offset(expr->left_range_val());
  lsb = decl->bit_offset(expr->right_range_val());
  ymuint dim = expr->declarray_dimension();
  assert_cond( decl->dimension() == dim, __FILE__, __LINE__);
  if ( dim > 1 ) {
    // 配列型
    ymuint offset = 0;
    ymuint mlt = 1;
    for (ymuint i = 0; i < dim; ++ i) {
      const VlExpr* index = expr->declarray_index(i);
      int index_val;
      bool stat = index->eval_int(index_val);
      assert_cond( stat, __FILE__, __LINE__);
      offset += index_val * mlt;
      mlt *= decl->range(i)->size();
    }
    MvNode* node = mDeclMap.get(decl, offset);
    if ( node == NULL ) {
      cerr << decl->name() << " is not found in mDeclMap" << endl;
    }
    assert_cond( node != NULL, __FILE__, __LINE__);
    return node;
  }
  else {
    MvNode* node = mDeclMap.get(decl);
    if ( node == NULL ) {
      cerr << decl->name() << " is not found in mDeclMap" << endl;
    }
    assert_cond( node != NULL, __FILE__, __LINE__);
    return node;
  }
}

// @brief 宣言要素に対応するノードを登録する．
// @param[in] decl 宣言要素
// @param[in] node 登録するノード
void
ReaderImpl::reg_node(const VlDecl* decl,
		     MvNode* node)
{
  mDeclMap.add(decl, node);
  expand_nodemap(node->id());
  mNodeMap[node->id()] = make_pair(decl, 0);
}

// @brief 入出力宣言要素に対応するノードを登録する．
// @param[in] decl 宣言要素
// @param[in] node 登録するノード
void
ReaderImpl::reg_ionode(const VlDecl* decl,
		       MvNode* node)
{
  mIODeclMap.add(decl, node);
  expand_nodemap(node->id());
  mNodeMap[node->id()] = make_pair(decl, 0);
}

// @brief 宣言要素に対応するノードを登録する．
// @param[in] decl 宣言要素(配列型)
// @param[in] offset オフセット
// @param[in] node 登録するノード
void
ReaderImpl::reg_node(const VlDecl* decl,
		     ymuint offset,
		     MvNode* node)
{
  mDeclMap.add(decl, offset, node);
  expand_nodemap(node->id());
  mNodeMap[node->id()] = make_pair(decl, offset);
}

// @brief mNodeMap を拡張する．
void
ReaderImpl::expand_nodemap(ymuint id)
{
  while ( mNodeMap.size() <= id ) {
    mNodeMap.push_back(make_pair(static_cast<const VlDecl*>(NULL), 0));
  }
}

// @brief ドライバーを登録する．
// @param[in] node 左辺に対応するノード
// @param[in] driver ドライバー
void
ReaderImpl::reg_driver(MvNode* node,
		       const Driver& driver)
{
  if ( debug_driver ) {
    cerr << "reg_driver(" << node->id()
	 << ", " << driver.rhs_node()->id();
    if ( driver.has_bitselect() ) {
      cerr << "[" << driver.index() << "]";
    }
    else if ( driver.has_partselect() ) {
      cerr << "[" << driver.msb() << ":" << driver.lsb() << "]";
    }
    cerr << ")" << endl;
  }
  driver_list(node).push_back(driver);
}

// @brief ドライバーリストを取り出す．
// @param[in] node 対応するノード
// @note なければ空のリストを作る．
vector<Driver>&
ReaderImpl::driver_list(MvNode* node)
{
  assert_cond( node != NULL, __FILE__, __LINE__);
  ymuint id = node->id();
  while ( mDriverList.size() <= id ) {
    mDriverList.push_back(vector<Driver>());
  }
  return mDriverList[id];
}

// @brief 複数のドライバがある時にエラーメッセージを出力する．
void
ReaderImpl::error_drivers(MvNode* node,
			  const Driver& driver1,
			  const Driver& driver2)
{
  cerr << "more than one drivers for "
       << node->id()
       << endl;
  cerr << "previous driver: "
       << driver1.rhs_node()->id();
  if ( driver1.has_bitselect() ) {
    cerr << "[" << driver1.index() << "]";
  }
  else if ( driver1.has_partselect() ) {
    cerr << "[" << driver1.msb() << ":" << driver1.lsb()
	 << "]";
  }
  cerr << endl;
  cerr << "current driver: "
       << driver2.rhs_node()->id();
  if ( driver2.has_bitselect() ) {
    cerr << "[" << driver2.index() << "]";
  }
  else if ( driver2.has_partselect() ) {
    cerr << "[" << driver2.msb() << ":" << driver2.lsb()
	 << "]";
  }
  cerr << endl;
  abort();
}

END_NAMESPACE_YM_MVN_VERILOG
