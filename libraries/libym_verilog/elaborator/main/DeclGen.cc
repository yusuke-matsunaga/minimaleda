
/// @file libym_verilog/elaborator/DeclGen.cc
/// @brief ElbMgr の実装ファイル(宣言要素の elaboration)
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: DeclGen.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "DeclGen.h"
#include "ElbEnv.h"

#include "ym_verilog/pt/PtDecl.h"
#include "ym_verilog/pt/PtExpr.h"
#include "ym_verilog/pt/PtMisc.h"
#include "ym_verilog/pt/PtArray.h"

#include "ym_verilog/vl/VlModule.h"

#include "ElbParamAssign.h"
#include "ElbContAssign.h"
#include "ElbDecl.h"
#include "ElbParameter.h"
#include "ElbGenvar.h"
#include "ElbModule.h"
#include "ElbTaskFunc.h"
#include "ElbRange.h"
#include "ElbExpr.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス DeclGen
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] elab 生成器
// @param[in] elb_mgr Elbオブジェクトを管理するクラス
// @param[in] elb_factory Elbオブジェクトを生成するファクトリクラス
DeclGen::DeclGen(Elaborator& elab,
		 ElbMgr& elb_mgr,
		 ElbFactory& elb_factory) :
  ElbProxy(elab, elb_mgr, elb_factory)
{
}

// @brief デストラクタ
DeclGen::~DeclGen()
{
}

// @brief IO宣言要素をインスタンス化する．
// @param[in] module 親のモジュール
// @param[in] taskfunc 親のタスク/関数
// @param[in] pt_head_array IO宣言ヘッダの配列
// @note module, taskfunc は1つのみが値を持つ．残りは NULL
void
DeclGen::instantiate_iodecl(ElbModule* module,
			    ElbTaskFunc* taskfunc,
			    PtIOHeadArray pt_head_array)
{
  assert_cond( module != NULL || taskfunc != NULL, __FILE__, __LINE__);
  assert_cond( module == NULL || taskfunc == NULL, __FILE__, __LINE__);

  VlNamedObj* namedobj = NULL;
  if ( module ) {
    namedobj = module;
  }
  else if ( taskfunc ) {
    namedobj = taskfunc;
  }
  else {
    // 冗長
    assert_not_reached(__FILE__, __LINE__);
  }

  ymuint index = 0;
  for (ymuint i = 0; i < pt_head_array.size(); ++ i) {
    const PtIOHead* pt_head = pt_head_array[i];
    tVpiAuxType def_aux_type = pt_head->aux_type();
    bool sign = pt_head->is_signed();

    const PtExpr* pt_left = pt_head->left_range();
    const PtExpr* pt_right = pt_head->right_range();

    ElbExpr* left = NULL;
    ElbExpr* right = NULL;
    int left_val = 0;
    int right_val = 0;
    if ( !instantiate_range(namedobj,
			    pt_left, pt_right,
			    left, right,
			    left_val, right_val) ) {
      // エラーが起きたのでスキップする．
      continue;
    }

    ElbIOHead* head = NULL;
    if ( module ) {
      head = factory().new_ModIOHead(module, pt_head);
    }
    else {
      if ( taskfunc->type() == kVpiTask ) {
	head = factory().new_TaskIOHead(taskfunc, pt_head);
      }
      else {
	head = factory().new_FunctionIOHead(taskfunc, pt_head);
      }
    }
    assert_cond( head != NULL, __FILE__, __LINE__);

    for (ymuint j = 0; j < pt_head->item_num(); ++ j) {
      const PtIOItem* pt_item = pt_head->item(j);

      // IO定義と変数/ネット定義が一致しているか調べる．
      ElbObjHandle* handle = find_obj(namedobj, pt_item->name());
      ElbDecl* decl = NULL;
      if ( handle ) {
	// 同名の要素が見つかった．
	if ( def_aux_type != kVpiAuxNone ) {
	  // なのに IO 宣言の aux_type もある．
	  ostringstream buf;
	  buf << pt_item->name() << " : has an aux-type declaration"
	      << ", while it also has another declaration in "
	      << handle->file_region() << ".";
	  put_msg(__FILE__, __LINE__,
		  pt_item->file_region(),
		  kMsgError,
		  "ELAB",
		  buf.str());
	  continue;
	}
	decl = handle->decl();
	if ( decl ) {
	  tVpiObjType type = decl->type();
	  if ( (module == NULL || type != kVpiNet) &&
	       type != kVpiReg &&
	       type != kVpiIntegerVar &&
	       type != kVpiTimeVar) {
	    decl = NULL;
	  }
	}
	if ( !decl ) {
	  ostringstream buf;
	  buf << handle->full_name()
	      << ": Should be a ";
	  if ( module ) {
	    buf << "net, ";
	  }
	  buf << "reg or integer/time variable.";
	  put_msg(__FILE__, __LINE__,
		  pt_item->file_region(),
		  kMsgError,
		  "ELAB",
		  buf.str());
	  continue;
	}

	if ( decl->dimension() > 0 ) {
	  ostringstream buf;
	  buf << pt_item->name()
	      << ": Array object shall not be connected to IO port.";
	  put_msg(__FILE__, __LINE__,
		  decl->file_region(),
		  kMsgError,
		  "ELAB",
		  buf.str());
	  continue;
	}

	// decl と型が一致しているか調べる．
	// IEEE 1364-2001 12.3.3 Port declarations
	ElbExpr* left2 = decl->_left_range();
	ElbExpr* right2 = decl->_right_range();
	if ( left2 && right2 ) {
	  if ( left == NULL && right == NULL ) {
	    // decl は範囲を持っているが IO は持っていない．
	    // これはエラーにしなくてよいのだろうか？
	    // たぶんコンパイルオプションで制御すべき
	    if ( allow_empty_io_range() ) {
	      left = left2;
	      right = right2;
	    }
	    else {
	      ostringstream buf;
	      buf << "Conflictive range declaration of \""
		  << pt_item->name() << "\".";
	      put_msg(__FILE__, __LINE__,
		      pt_item->file_region(),
		      kMsgError,
		      "ELAB",
		      buf.str());
	      continue;
	    }
	  }
	  else {
	    int left2_val;
	    bool stat1 = expr_to_int(left2, left2_val);
	    assert_cond(stat1, __FILE__, __LINE__);
	    int right2_val;
	    bool stat2 = expr_to_int(right2, right2_val);
	    assert_cond(stat2, __FILE__, __LINE__);
	    if ( left_val != left2_val || right_val != right2_val ) {
	      ostringstream buf;
	      buf << "Conflictive range declaration of \""
		  << pt_item->name() << "\".";
	      put_msg(__FILE__, __LINE__,
		      pt_item->file_region(),
		      kMsgError,
		      "ELAB",
		      buf.str());
	      cout << "IO range: [" << left_val << ":" << right_val << "]"
		   << endl
		   << "Decl range: [" << left2_val << ":" << right2_val << "]"
		   << endl;
	      continue;
	    }
	  }
	}
	else {
	  if ( left && right ) {
	    // decl は範囲を持っていないが IO は持っている．
	    // エラーとする．
	    ostringstream buf;
	    buf << "Conflictive range declaration of \""
		<< pt_item->name() << "\".";
	    put_msg(__FILE__, __LINE__,
		    pt_item->file_region(),
		    kMsgError,
		    "ELAB",
		    buf.str());
	    continue;
	  }
	}
	// どちらか一方でも符号付きなら両方符号付きにする．
	// ちょっと ad-hoc な仕様
	if ( !decl->is_signed() && sign ) {
	  decl->set_signed();
	}
      }
      else {
	// 同名の要素が見つからなかったので作る必要がある．
	tVpiAuxType aux_type = def_aux_type;
	if ( aux_type == kVpiAuxNone ) {
	  if ( module ) {
	    // モジュール IO の場合は `default_net_type を参照する．
	    tVpiNetType net_type = module->def_net_type();
	    if ( net_type == kVpiNone ) {
	      ostringstream buf;
	      buf << pt_item->name() << " : Implicit declaration is inhibited "
		  << " because default_nettype = \"none\".";
	      put_msg(__FILE__, __LINE__,
		      pt_item->file_region(),
		      kMsgError,
		      "ELAB",
		      buf.str());
	      continue;
	    }
	    aux_type = kVpiAuxNet;
	  }
	  else {
	    // task/function の場合，型指定が無い時は reg 型となる．
	    aux_type = kVpiAuxReg;
	  }
	}

	ElbDeclHead* head = NULL;
	if ( module ) {
	  if ( left && right ) {
	    head = factory().new_DeclHead(module, pt_head, aux_type,
					  left, right,
					  left_val, right_val);
	  }
	  else {
	    head = factory().new_DeclHead(module, pt_head, aux_type);
	  }
	}
	if ( taskfunc ) {
	  if ( left && right ) {
	    head = factory().new_DeclHead(taskfunc, pt_head, aux_type,
					  left, right,
					  left_val, right_val);
	  }
	  else {
	    head = factory().new_DeclHead(taskfunc, pt_head, aux_type);
	  }
	}
	assert_cond( head != NULL, __FILE__, __LINE__);

	const PtExpr* pt_init = pt_item->init_value();
	ElbExpr* init = NULL;
	if ( module ) {
	  if ( pt_init ) {
	    // 初期値を持つ場合
	    if ( aux_type == kVpiAuxNet ) {
	      // net 型の場合(ここに来るのは暗黙宣言のみ)は初期値を持てない．
	      ostringstream buf;
	      buf << pt_item->name()
		  << " : Implicit net declaration cannot have initial value.";
	      put_msg(__FILE__, __LINE__,
		      pt_item->file_region(),
		      kMsgError,
		      "ELAB",
		      buf.str());
	      continue;
	    }
	    // これは verilog_grammer.yy の list_of_variable_port_identifiers
	    // に対応するので必ず constant_expression である．
	    init = instantiate_constant_expr(module, pt_init);
	    // エラーの場合には init = NULL となるが処理は続ける．
	  }
	}
	else {
	  // task/function の IO 宣言には初期値はない．
	  assert_cond( pt_init == NULL, __FILE__, __LINE__);
	}

	decl = factory().new_Decl(head, pt_item, init);
	int tag = 0;
	switch ( aux_type ) {
	case kVpiAuxNet: tag = vpiNet; break;
	case kVpiAuxReg: tag = vpiReg; break;
	case kVpiAuxVar: tag = vpiVariables; break;
	default:
	  assert_not_reached(__FILE__, __LINE__);
	}
	reg_decl(tag, decl);
      }

      if ( module ) {
	module->init_iodecl(index, head, pt_item, decl);
      }
      else if ( taskfunc ) {
	taskfunc->init_iodecl(index, head, pt_item, decl);
      }
      else {
	// かなりしつこく冗長
	assert_not_reached(__FILE__, __LINE__);
      }
      ++ index;

      ostringstream buf;
      buf << "IODecl(" << pt_item->name() << ")@"
	  << namedobj->full_name() << " created.";
      put_msg(__FILE__, __LINE__,
	      pt_head->file_region(),
	      kMsgInfo,
	      "ELAB",
	      buf.str());
    }
  }
}

// 宣言要素をインスタンス化する．
// @param[in] parent 親のスコープ
// @param[in] pt_head_array 宣言ヘッダの配列
void
DeclGen::instantiate_decl(const VlNamedObj* parent,
			  PtDeclHeadArray pt_head_array)
{
  for (ymuint i = 0; i < pt_head_array.size(); ++ i) {
    const PtDeclHead* pt_head = pt_head_array[i];
    switch ( pt_head->type() ) {
    case kPtDecl_Param:
    case kPtDecl_LocalParam:
      assert_not_reached(__FILE__, __LINE__);
      break;

    case kPtDecl_Reg:
      instantiate_reg_head(parent, pt_head);
      break;

    case kPtDecl_Var:
      instantiate_var_head(parent, pt_head);
      break;

    case kPtDecl_Genvar:
      instantiate_genvar_head(parent, pt_head);
      break;

    case kPtDecl_Net:
      instantiate_net_head(parent, pt_head);
      break;

    case kPtDecl_Event:
      instantiate_event_head(parent, pt_head);
      break;

    case kPtDecl_SpecParam:
      // 未対応
      break;

    default:
      break;
    }
  }
}

// @brief パラメータ用の instantiate 関数
// @param[in] parent 親のスコープ
// @param[in] pt_head_array 宣言ヘッダの配列
// @param[in] is_local local_param の時 true
void
DeclGen::instantiate_param(const VlNamedObj* parent,
			   PtDeclHeadArray pt_head_array,
			   bool is_local)
{
  const VlModule* module = parent->parent_module();

  for (ymuint i = 0; i < pt_head_array.size(); ++ i) {
    const PtDeclHead* pt_head = pt_head_array[i];
    ElbParamHead* param_head = NULL;

    const PtExpr* pt_left = pt_head->left_range();
    const PtExpr* pt_right = pt_head->right_range();
    if ( pt_left && pt_right ) {
      ElbExpr* left = NULL;
      ElbExpr* right = NULL;
      int left_val = 0;
      int right_val = 0;
      if ( !instantiate_range(parent, pt_left, pt_right,
			      left, right,
			      left_val, right_val) ) {
	continue;
      }
      param_head = factory().new_ParamHead(module, pt_head,
					   left, right,
					   left_val, right_val);
    }
    else {
      param_head = factory().new_ParamHead(module, pt_head);
    }

    for (ymuint i = 0; i < pt_head->item_num(); ++ i) {
      const PtDeclItem* pt_item = pt_head->item(i);
      const FileRegion& file_region = pt_item->file_region();

      // 右辺の式は constant expression のはずなので今つくる．
      const PtExpr* pt_init_expr = pt_item->init_value();
      ElbExpr* init = instantiate_constant_expr(parent, pt_init_expr);
      if ( !init ) {
	continue;
      }

      ElbDecl* param = factory().new_Parameter(param_head,
					       pt_item,
					       is_local);
      assert_cond(param, __FILE__, __LINE__);
      reg_decl(vpiParameter, param);

      // attribute instance の生成
      //instantiate_attribute(pt_head->attr_top(), false, param);

      ostringstream buf;
      buf << "Parameter(" << param->full_name() << ") created.";
      msg_mgr().put_msg(__FILE__, __LINE__,
			file_region,
			kMsgInfo,
			"ELAB",
			buf.str());

      param->set_expr(init);

      // ダブっている感じがするけど同じことを表す parameter assign 文
      // をつくってモジュールに追加する．
      ElbParamAssign* pa = factory().new_ParamAssign(module, pt_item,
						     param, init,
						     true);
      reg_paramassign(pa);
    }
  }
}

// @brief net をインスタンス化する．
// @param[in] parent 親のスコープ
// @param[in] pt_head 宣言のヘッダ
void
DeclGen::instantiate_net_head(const VlNamedObj* parent,
			      const PtDeclHead* pt_head)
{
  const PtExpr* pt_left = pt_head->left_range();
  const PtExpr* pt_right = pt_head->right_range();
  const PtDelay* pt_delay = pt_head->delay();

  bool has_delay = (pt_delay != NULL);

  ElbDeclHead* net_head = NULL;
  if ( pt_left && pt_right ) {
    ElbExpr* left = NULL;
    ElbExpr* right = NULL;
    int left_val = 0;
    int right_val = 0;
    if ( !instantiate_range(parent, pt_left, pt_right,
			    left, right,
			    left_val, right_val) ) {
      return;
    }
    net_head = factory().new_DeclHead(parent, pt_head,
				      left, right,
				      left_val, right_val,
				      has_delay);
  }
  else {
    net_head = factory().new_DeclHead(parent, pt_head);
  }
  assert_cond( net_head, __FILE__, __LINE__);

  if ( pt_delay ) {
    add_phase3stub(make_stub(this, &DeclGen::link_net_delay,
			     net_head, pt_delay));
  }

  for (ymuint i = 0; i < pt_head->item_num(); ++ i) {
    const PtDeclItem* pt_item = pt_head->item(i);
    // init_value() が 0 でなければ初期割り当てを持つということ．
    const PtExpr* pt_init = pt_item->init_value();

    ymuint dim_size = pt_item->dimension_list_size();
    if ( dim_size > 0 ) {
      // 配列

      // 初期割り当ては構文規則上持てないはず
      assert_cond( !pt_init, __FILE__, __LINE__);

      // 範囲の配列を作る．
      vector<ElbRangeSrc> range_src;
      if ( !instantiate_dimension_list(parent, pt_item, range_src) ) {
	continue;
      }

      ElbDeclArray* net_array = factory().new_DeclArray(net_head,
							pt_item,
							range_src);
      reg_declarray(vpiNetArray, net_array);

      // attribute instance の生成
      //instantiate_attribute(pt_head->attr_top(), false, net_array);

      ostringstream buf;
      buf << "NetArray(" << net_array->full_name() << ") created.";
      msg_mgr().put_msg(__FILE__, __LINE__,
			pt_item->file_region(),
			kMsgInfo,
			"ELAB",
			buf.str());
    }
    else {
      // 単一の要素
      ElbDecl* net = factory().new_Decl(net_head,
					pt_item);
      reg_decl(vpiNet, net);

      if ( pt_init ) {
	// 初期割り当てつき
	add_phase3stub(make_stub(this, &DeclGen::link_net_assign,
				 net, pt_item));
      }

      // attribute instance の生成
      //instantiate_attribute(pt_head->attr_top(), false, net);

      ostringstream buf;
      buf << "Net(" << net->full_name() << ") created.";
      msg_mgr().put_msg(__FILE__, __LINE__,
			pt_item->file_region(),
			kMsgInfo,
			"ELAB",
			buf.str());
    }
  }
}

// @brief net の遅延値を生成する．
// @param[in] net_head ネットのヘッダ
// @param[in] pt_delay パース木の遅延式定義
void
DeclGen::link_net_delay(ElbDeclHead* net_head,
			const PtDelay* pt_delay)
{
  const VlNamedObj* parent = net_head->parent();
  ElbDelay* delay = instantiate_delay(parent, pt_delay);
  if ( delay ) {
    net_head->set_delay(delay);
  }
}

// @brief net の初期値を生成する．
// @param[in] net ネット
// @param[in] pt_item パース木のネット定義要素
void
DeclGen::link_net_assign(ElbDecl* net,
			 const PtDeclItem* pt_item)
{
  // 実際には対応する continuous assign 文を作る．

  ElbExpr* lhs = factory().new_Primary(pt_item, net);

  const VlNamedObj* parent = net->parent();
  const PtExpr* pt_init = pt_item->init_value();
  ElbExpr* rhs = instantiate_rhs(parent, ElbEnv(), pt_init, lhs);
  if ( !rhs ) {
    return;
  }

  const VlModule* module = parent->parent_module();
  ElbContAssign* ca = factory().new_ContAssign(module, pt_item,
					       lhs, rhs);
  reg_contassign(ca);
}

// @brief reg をインスタンス化する．
// @param[in] parent 親のスコープ
// @param[in] pt_head 宣言のヘッダ
void
DeclGen::instantiate_reg_head(const VlNamedObj* parent,
			      const PtDeclHead* pt_head)
{
  const PtExpr* pt_left = pt_head->left_range();
  const PtExpr* pt_right = pt_head->right_range();

  ElbDeclHead* reg_head = NULL;
  if ( pt_left && pt_right ) {
    ElbExpr* left = NULL;
    ElbExpr* right = NULL;
    int left_val = 0;
    int right_val = 0;
    if ( !instantiate_range(parent, pt_left, pt_right,
			    left, right,
			    left_val, right_val) ) {
      return;
    }
    reg_head = factory().new_DeclHead(parent, pt_head,
				      left, right,
				      left_val, right_val);
  }
  else {
    reg_head = factory().new_DeclHead(parent, pt_head);
  }
  assert_cond( reg_head != NULL, __FILE__, __LINE__);

  for (ymuint i = 0; i < pt_head->item_num(); ++ i) {
    const PtDeclItem* pt_item = pt_head->item(i);
    const PtExpr* pt_init = pt_item->init_value();
    ymuint dim_size = pt_item->dimension_list_size();
    if ( dim_size > 0 ) {
      // 配列の場合

      // 配列は初期値を持たない．
      assert_cond( !pt_init, __FILE__, __LINE__);

      // 範囲の配列を作る．
      vector<ElbRangeSrc> range_src;
      if ( !instantiate_dimension_list(parent, pt_item, range_src) ) {
	continue;
      }

      ElbDeclArray* reg_array = factory().new_DeclArray(reg_head,
							pt_item,
							range_src);
      reg_declarray(vpiRegArray, reg_array);

      // attribute instance の生成
      //instantiate_attribute(pt_head->attr_top(), false, reg_array);

      ostringstream buf;
      buf << "RegArray(" << reg_array->full_name() << ") created.";
      msg_mgr().put_msg(__FILE__, __LINE__,
			pt_item->file_region(),
			kMsgInfo,
			"ELAB",
			buf.str());
    }
    else {
      // 単独の要素
      ElbExpr* init = NULL;
      if ( pt_init ) {
	// 初期値を持つ場合
	// 初期値は constant_expression なので今作る．
	init = instantiate_constant_expr(parent, pt_init);
	// エラーの時には init = NULL となるがそれでも処理は続ける．
	// もちろんエラーは記録されている．
      }

      ElbDecl* reg = factory().new_Decl(reg_head, pt_item, init);
      reg_decl(vpiReg, reg);

      // attribute instance の生成
      //instantiate_attribute(pt_head->attr_top(), false, reg);

      ostringstream buf;
      buf << "Reg(" << reg->full_name() << ") created.";
      msg_mgr().put_msg(__FILE__, __LINE__,
			pt_item->file_region(),
			kMsgInfo,
			"ELAB",
			buf.str());
    }
  }
}

// @brief variable header をインスタンス化する．
// @param[in] parent 親のスコープ
// @param[in] pt_head 宣言のヘッダ
void
DeclGen::instantiate_var_head(const VlNamedObj* parent,
			      const PtDeclHead* pt_head)
{
  assert_cond(pt_head->data_type() != kVpiVarNone, __FILE__, __LINE__);

  ElbDeclHead* var_head = factory().new_DeclHead(parent, pt_head);

  for (ymuint i = 0; i < pt_head->item_num(); ++ i) {
    const PtDeclItem* pt_item = pt_head->item(i);
    const PtExpr* pt_init = pt_item->init_value();
    ymuint dim_size = pt_item->dimension_list_size();
    if ( dim_size > 0 ) {
      // 配列の場合

      // 配列は初期値を持たない．
      assert_cond( !pt_init, __FILE__, __LINE__);

      // 範囲の配列を作る．
      vector<ElbRangeSrc> range_src;
      if ( !instantiate_dimension_list(parent, pt_item, range_src) ) {
	continue;
      }

      ElbDeclArray* var_array = factory().new_DeclArray(var_head,
							pt_item,
							range_src);
      reg_declarray(vpiVariables, var_array);

      // attribute instance の生成
      //instantiate_attribute(pt_head->attr_top(), false, var_array);

      ostringstream buf;
      buf << "VarArray(" << var_array->full_name() << ") created.";
      msg_mgr().put_msg(__FILE__, __LINE__,
			pt_item->file_region(),
			kMsgInfo,
			"ELAB",
			buf.str());
    }
    else {
      // 単独の変数
      ElbExpr* init = NULL;
      if ( pt_init ) {
	// 初期値を持つ場合
	// 初期値は constant_expression なので今作る．
	init = instantiate_constant_expr(parent, pt_init);
	// エラーの時には init = NULL となるがそれでも処理は続ける．
	// もちろんエラーは記録されている．
      }

      ElbDecl* var = factory().new_Decl(var_head, pt_item, init);
      reg_decl(vpiVariables, var);

      // attribute instance の生成
      //instantiate_attribute(pt_head->attr_top(), false, var);

      ostringstream buf;
      buf << "Var(" << var->full_name() << ") created.";
      msg_mgr().put_msg(__FILE__, __LINE__,
			pt_item->file_region(),
			kMsgInfo,
			"ELAB",
			buf.str());
    }
  }
}

// @brief named_event をインスタンス化する．
// @param[in] parent 親のスコープ
// @param[in] pt_head 宣言のヘッダ
void
DeclGen::instantiate_event_head(const VlNamedObj* parent,
				const PtDeclHead* pt_head)
{
  ElbDeclHead* event_head = factory().new_DeclHead(parent, pt_head);

  for (ymuint i = 0; i < pt_head->item_num(); ++ i) {
    const PtDeclItem* pt_item = pt_head->item(i);
    ymuint dim_size = pt_item->dimension_list_size();
    if ( dim_size > 0 ) {
      // 配列

      // 範囲の配列を作る．
      vector<ElbRangeSrc> range_src;
      if ( !instantiate_dimension_list(parent, pt_item, range_src) ) {
	continue;
      }

      ElbDeclArray* ne_array = factory().new_DeclArray(event_head,
						       pt_item,
						       range_src);
      reg_declarray(vpiNamedEventArray, ne_array);

      // attribute instance の生成
      //instantiate_attribute(pt_head->attr_top(), false, ne_array);

      ostringstream buf;
      buf << "NamedEventArray(" << ne_array->full_name() << ") created.";
      msg_mgr().put_msg(__FILE__, __LINE__,
			pt_item->file_region(),
			kMsgInfo,
			"ELAB",
			buf.str());
    }
    else {
      // 単一の要素
      ElbDecl* named_event = factory().new_Decl(event_head,
						pt_item);
      reg_decl(vpiNamedEvent, named_event);

      // attribute instance の生成
      //instantiate_attribute(pt_head->attr_top(), false, named_event);

      ostringstream buf;
      buf << "NamedEvent(" << named_event->full_name() << ") created.";
      msg_mgr().put_msg(__FILE__, __LINE__,
			pt_item->file_region(),
			kMsgInfo,
			"ELAB",
			buf.str());
    }
  }
}

// @brief genvar をインスタンス化する．
// @param[in] parent 親のスコープ
// @param[in] pt_head 宣言のヘッダ
void
DeclGen::instantiate_genvar_head(const VlNamedObj* parent,
				 const PtDeclHead* pt_head)
{
  for (ymuint i = 0; i < pt_head->item_num(); ++ i) {
    const PtDeclItem* pt_item = pt_head->item(i);
    ElbGenvar* genvar = factory().new_Genvar(parent, pt_item, 0);
    reg_genvar(genvar);

    ostringstream buf;
    buf << "Getnvar(" << genvar->full_name() << ") created.";
    msg_mgr().put_msg(__FILE__, __LINE__,
		      pt_item->file_region(),
		      kMsgInfo,
		      "ELAB",
		      buf.str());
  }
}

// @brief 配列の次元リストを生成する．
// @param[in] parent 親のスコープ
// @param[in] pt_item 要素定義
// @param[in] range_src 範囲の情報を設定する配列
bool
DeclGen::instantiate_dimension_list(const VlNamedObj*  parent,
				    const PtDeclItem* pt_item,
				    vector<ElbRangeSrc>& range_src)
{
  ymuint n = pt_item->dimension_list_size();
  range_src.reserve(n);

  bool ok = true;
  for (ymuint i = 0; i < n; ++ i) {
    const PtRange* pt_range = pt_item->range(i);
    const PtExpr* pt_left = pt_range->left();
    const PtExpr* pt_right = pt_range->right();
    ElbExpr* left = NULL;
    ElbExpr* right = NULL;
    int left_val = 0;
    int right_val = 0;
    if ( !instantiate_range(parent, pt_left, pt_right,
			    left, right,
			    left_val, right_val) ) {
      ok = false;
      break;
    }
    range_src.push_back(ElbRangeSrc(pt_range, left, right,
				    left_val, right_val));
  }

  return ok;
}

END_NAMESPACE_YM_VERILOG
