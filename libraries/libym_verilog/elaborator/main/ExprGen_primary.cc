
/// @file libym_verilog/elaborator/ExprGen_primary.cc
/// @brief ElbMgr の実装ファイル(式の実体化)
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: ExprGen_primary.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ExprGen.h"
#include "ElbEnv.h"

#include "ym_verilog/pt/PtModule.h"
#include "ym_verilog/pt/PtExpr.h"
#include "ym_verilog/pt/PtMisc.h"

#include "ym_verilog/vl/VlModule.h"

#include "ElbDecl.h"
#include "ElbPrimitive.h"
#include "ElbExpr.h"
#include "ElbGenvar.h"


BEGIN_NAMESPACE_YM_VERILOG

// @brief PtPrimary から ElbExpr を生成する．
// @param[in] parent 親のスコープ
// @param[in] env 生成時の環境
// @param[in] pt_expr 式を表すパース木
ElbExpr*
ExprGen::instantiate_primary(const VlNamedObj* parent,
			     const ElbEnv& env,
			     const PtExpr* pt_expr)
{
  // 識別子の階層
  PtNameBranchArray nb_array = pt_expr->namebranch_array();
  bool has_hname = (nb_array.size() > 0);
  if ( has_hname ) {
    if ( env.is_constant() ) {
      // 階層つき識別子はだめ
      error_hname_in_ce(pt_expr);
      return NULL;
    }
    if ( env.inside_constant_function() ) {
      // 階層つき識別子はだめ
      error_hname_in_cf(pt_expr);
    }
  }

  // 識別子の名前
  const char* name = pt_expr->name();

  // 識別子の添字の次元
  int isize = pt_expr->index_num();

  // 名前に対応したオブジェクトのハンドル
  ElbObjHandle* handle = NULL;

  if ( env.is_constant() ) {
    handle = find_const_handle(parent, pt_expr);
    if ( handle == NULL ) {
      // エラーが起こった．
      // メッセージは find_const_handle() 内で出力されている．
      return NULL;
    }
  }
  else if ( env.inside_constant_function() ) {
    // まず関数内の識別子を探索する．
    handle = find_obj_up(parent, nb_array, name, env.constant_function());
    if ( handle == NULL && !env.is_lhs() ) {
      // 右辺ならモジュール内の定数識別子を探索する．
      handle = find_const_handle(parent, pt_expr);
    }
    if ( handle == NULL ) {
      // エラーが起こった．
      // メッセージは find_const_handle() 内で出力されている．
      return NULL;
    }
  }
  else {
    // 通常のスコープで探索する．
    // たぶんモジュール内でいいはず．
    handle = find_obj_up(parent, nb_array, name,
			 parent->parent_module());
    if ( handle == NULL ) {
      // 見つからなくてもデフォルトネットタイプが kVpiNone でないかぎり
      // 暗黙の1ビットネット宣言を行う．
      // ただし識別子に添字がついていたらだめ
      const VlModule* parent_module = parent->parent_module();
      tVpiNetType def_nettype = parent_module->def_net_type();
      if ( pt_expr->is_simple() &&
	   !has_hname &&
	   isize == 0 &&
	   def_nettype != kVpiNone ) {
	ElbDecl* decl = factory().new_ImpNet(parent, pt_expr, def_nettype);
	reg_decl(vpiNet, decl);

	handle = find_obj(parent, name);
	assert_cond(handle, __FILE__, __LINE__);
      }
    }
    if ( handle == NULL ) {
      error_not_found(pt_expr);
      return NULL;
    }
  }

  if ( env.is_system_tf_arg() ) {
    // システム関数/タスクの引数の場合
    if ( isize == 0 ) {
      const VlNamedObj* scope = handle->obj();
      if ( scope ) {
	return factory().new_ArgHandle(pt_expr, scope);
      }
      ElbPrimitive* primitive = handle->primitive();
      if ( primitive ) {
	return factory().new_ArgHandle(pt_expr, primitive);
      }
      ElbDecl* decl = handle->decl();
      if ( decl ) {
	return factory().new_Primary(pt_expr, decl);
      }
      ElbDeclArray* decl_array = handle->decl_array();
      if ( decl_array ) {
	return factory().new_ArgHandle(pt_expr, decl_array);
      }
    }
    else if ( isize == 1 ) {
      const PtExpr* pt_expr1 = pt_expr->index(0);
      int index;
      bool stat = evaluate_expr_int(parent, pt_expr1, index);
      if ( !stat ) {
	return NULL;
      }
      const VlNamedObj* scope = handle->array_elem(index);
      if ( scope ) {
	return factory().new_ArgHandle(pt_expr, scope);
      }
      ElbPrimArray* prim_array = handle->prim_array();
      if ( prim_array ) {
	ElbPrimitive* primitive = prim_array->_primitive_by_index(index);
	if ( primitive ) {
	  return factory().new_ArgHandle(pt_expr, primitive);
	}
      }
    }
    error_illegal_object(pt_expr);
    return NULL;
  }

  if ( !env.is_lhs() ) {
    // 対象のオブジェクトが genvar の場合
    ElbGenvar* genvar = handle->genvar();
    if ( genvar ) {
      return instantiate_genvar(parent, pt_expr, genvar->value());
    }
  }

  // 対象のオブジェクトが宣言要素だった場合
  bool has_range_select;
  bool has_bit_select;
  ElbExpr* index1;
  ElbExpr* index2;
  ElbDecl* decl = instantiate_decl(handle, parent, env, pt_expr,
				   has_range_select,
				   has_bit_select,
				   index1, index2);
  if ( decl == NULL ) {
    // エラー
    // メッセージは instantiate_decl() 内で出力されている．
    return NULL;
  }

  if ( !check_decl(env, pt_expr, decl, has_range_select | has_bit_select) ) {
    // エラー
    // メッセージは instantiate_decl() 内で出力されている．
    return NULL;
  }

  if ( has_bit_select ) {
    return factory().new_BitSelect(pt_expr, decl, index1);
  }
  if ( has_range_select ) {
    switch ( pt_expr->range_mode() ) {
    case kVpiConstRange:
      {
	int index1_val;
	if ( !expr_to_int(index1, index1_val) ) {
	  return NULL;
	}
	int index2_val;
	if ( !expr_to_int(index2, index2_val) ) {
	  return NULL;
	}
	return factory().new_PartSelect(pt_expr, decl,
					index1, index2,
					index1_val, index2_val);
      }

    case kVpiPlusRange:
      {
	int range_val;
	if ( !expr_to_int(index2, range_val) ) {
	  return NULL;
	}
	return factory().new_PlusPartSelect(pt_expr, decl,
					    index1, index2, range_val);
      }

    case kVpiMinusRange:
      {
	int range_val;
	if ( !expr_to_int(index2, range_val) ) {
	  return NULL;
	}
	return factory().new_MinusPartSelect(pt_expr, decl,
					     index1, index2, range_val);
      }

    case kVpiNoRange:
      assert_not_reached(__FILE__, __LINE__);
      break;
    }
  }
  return factory().new_Primary(pt_expr, decl);
}

// @brief PtExpr(primary) から named_event を生成する．
// @param[in] parent 親のスコープ
// @param[in] pt_expr 式を表すパース木
ElbDecl*
ExprGen::instantiate_namedevent(const VlNamedObj* parent,
				const PtExpr* pt_expr)
{
  assert_cond(pt_expr->type() == kPtPrimaryExpr, __FILE__, __LINE__);
  assert_cond(pt_expr->left_range() == NULL, __FILE__, __LINE__);
  assert_cond(pt_expr->right_range() == NULL, __FILE__, __LINE__);

  // 識別子の階層
  PtNameBranchArray nb_array = pt_expr->namebranch_array();

  // 識別子の名前
  const char* name = pt_expr->name();

  // 名前に対応したオブジェクトのハンドルを求める．
  ElbObjHandle* handle = find_obj_up(parent, nb_array, name, NULL);
  if ( handle == NULL ) {
    // 見つからなかった．
    error_not_found(pt_expr);
    return NULL;
  }

  // 配列要素などの処理を行う．
  ElbEnv env0;
  bool has_range_select;
  bool has_bit_select;
  ElbExpr* index1;
  ElbExpr* index2;
  ElbDecl* decl = instantiate_decl(handle, parent, env0, pt_expr,
				   has_range_select,
				   has_bit_select,
				   index1, index2);
  if ( decl == NULL ) {
    // エラー
    // メッセージは instantiate_decl() 内で出力されている．
    return NULL;
  }
  if ( decl->type() != kVpiNamedEvent ) {
    // 型が違う
    error_not_a_namedevent(pt_expr);
    return NULL;
  }

  return decl;
}

// @brief 定数識別子を探す．
// @param[in] parent 親のスコープ
// @param[in] pt_expr 式を表すパース木
ElbObjHandle*
ExprGen::find_const_handle(const VlNamedObj* parent,
			   const PtExpr* pt_expr)
{
  // 識別子の階層
  PtNameBranchArray nb_array = pt_expr->namebranch_array();

  // 識別子の名前
  const char* name = pt_expr->name();

  // モジュール内の識別子を探索する．
  ElbObjHandle* handle = find_obj_up(parent, nb_array, name,
				     parent->parent_module());
  if ( handle == NULL ) {
    // 見つからなかった．
    error_not_found(pt_expr);
    return NULL;
  }
  // handle が持つオブジェクトは genvar か parameter でなければならない．
  if ( handle->genvar() == NULL ) {
    ElbDecl* decl = handle->decl();
    if ( decl == NULL || decl->type() == kVpiParameter ) {
      error_not_a_parameter(pt_expr);
      return NULL;
    }
  }

  return handle;
}

// @brief genvar に対応した定数を生成する．
// @param[in] pt_expr 式を表すパース木
// @param[in] val 値
// @note pt_expr に添字が付いていたらエラーとなる．
ElbExpr*
ExprGen::instantiate_genvar(const VlNamedObj* parent,
			    const PtExpr* pt_expr,
			    int val)
{
  bool has_range_select = (pt_expr->left_range() && pt_expr->right_range());

  ymuint isize = pt_expr->index_num();
  if (  isize > 1 || (isize == 1 && has_range_select) ) {
    // 配列型ではない．
    error_dimension_mismatch(pt_expr);
    return NULL;
  }
  bool has_bit_select = (isize == 1);

  if ( has_bit_select ) {
    int index1 = 0;
    if ( !evaluate_expr_int(parent, pt_expr->index(0), index1) ) {
      return NULL;
    }
    val >>= index1;
    val &= 1;
  }
  else if ( has_range_select ) {
    int index1 = 0;
    int index2 = 0;
    bool stat1 = evaluate_expr_int(parent, pt_expr->left_range(), index1);
    bool stat2 = evaluate_expr_int(parent, pt_expr->right_range(), index2);
    if ( !stat1 || !stat2 ) {
      return NULL;
    }
    val >>= index2;
    val &= ((1 << (index1 - index2 + 1)) - 1);
  }

  // genvar の値に対応した定数式を生成
  return factory().new_GenvarConstant(pt_expr, val);
}

// @brief 宣言要素のインスタンス化を行う．
// @param[in] handle オブジェクトハンドル
// @param[in] parent 親のスコープ
// @param[in] pt_expr 式を表すパース木
// @param[out] has_range_select 範囲指定を持っていたら true を返す．
// @param[out] has_bit_select ビット指定を持っていたら true を返す．
// @param[out] index1, index2 範囲指定/ビット指定の式を返す．
ElbDecl*
ExprGen::instantiate_decl(ElbObjHandle* handle,
			  const VlNamedObj* parent,
			  const ElbEnv& env,
			  const PtExpr* pt_expr,
			  bool& has_range_select,
			  bool& has_bit_select,
			  ElbExpr*& index1,
			  ElbExpr*& index2)
{
  // 対象の宣言要素
  ElbDecl* decl = handle->decl();
  // 配列の次元
  ymuint dsize = 0;
  // プライマリ式の次元 (ビット指定を含んでいる可能性あり)
  ymuint isize = pt_expr->index_num();

  // 範囲指定があるとき true となるフラグ
  has_range_select = (pt_expr->left_range() && pt_expr->right_range());

  // 添字が定数のとき true となるフラグ
  bool const_mode = pt_expr->is_const_index();

  // 添字には constant/constant function 以外の情報は引き継がない
  ElbEnv index_env;
  if ( const_mode ) {
    index_env = ElbConstantEnv();
  }
  else if ( env.inside_constant_function() ) {
    index_env = ElbConstantFunctionEnv(env.constant_function());
  }

  if ( decl == NULL ) {
    // そのハンドルが宣言要素の配列なら要素を取り出す．
    ElbDeclArray* decl_array = handle->decl_array();
    if ( decl_array ) {
      // 配列の次元
      dsize = decl_array->dimension();
      if ( isize != dsize && (isize != dsize + 1 || has_range_select) ) {
	// 次元が合わない．
	error_dimension_mismatch(pt_expr);
	return NULL;
      }

      // 添字を取り出す．
      vector<ElbExpr*> index_list(dsize);
      for (ymuint i = 0; i < dsize; ++ i) {
	const PtExpr* pt_expr1 = pt_expr->index(i);
	ElbExpr* expr1 = instantiate_expr(parent, index_env, pt_expr1);
	if ( !expr1 ) {
	  return NULL;
	}
	index_list.push_back(expr1);
      }

      decl = factory().new_DeclElem(pt_expr, decl_array, index_list);
    }
  }
  if ( decl == NULL ) {
    // 適切な型ではなかった．
    error_illegal_object_cf(pt_expr);
    return NULL;
  }

  // ビット指定があるとき true となるフラグ
  has_bit_select = false;

  if ( isize == dsize + 1 && !has_range_select ) {
    // 識別子の添字の次元と配列の次元が 1 違いで
    // 範囲がなければ識別子の最後の添字はビット指定と見なす．
    has_bit_select = true;
    -- isize;
  }

  if ( isize != dsize ) {
    error_dimension_mismatch(pt_expr);
    return NULL;
  }

  index1 = NULL;
  index2 = NULL;

  if ( has_bit_select ) {
    if ( decl->value_type() == kVpiValueReal ) {
      error_select_for_real(pt_expr);
      return NULL;
    }
    const PtExpr* pt_expr1 = pt_expr->index(0);
    index1 = instantiate_expr(parent, index_env, pt_expr1);
    if ( !index1 ) {
      return NULL;
    }
  }
  else if ( has_range_select ) {
    if ( decl->value_type() == kVpiValueReal ) {
      error_select_for_real(pt_expr);
      return NULL;
    }
    if ( pt_expr->range_mode() == kVpiConstRange ) {
      index1 = instantiate_constant_expr(parent, pt_expr->left_range());
    }
    else {
      index1 = instantiate_expr(parent, index_env, pt_expr->left_range());
    }
    if ( !index1 ) {
      return NULL;
    }

    // 範囲の2番目の式は常に定数でなければならない．
    index2 = instantiate_constant_expr(parent, pt_expr->right_range());
    if ( !index2 ) {
      return NULL;
    }
  }

  return decl;
}

// @brief decl の型が適切がチェックする．
bool
ExprGen::check_decl(const ElbEnv& env,
		    const PtExpr* pt_expr,
		    const ElbDecl* decl,
		    bool has_select)
{
  tVpiObjType type = decl->type();

  if ( env.is_pca_lhs() ) {
    // procedural continuous assignment 文の左辺式
    if ( decl->is_array_member() ) {
      // 配列要素はダメ
      error_array_in_pca(pt_expr);
      return false;
    }
    if ( has_select ) {
      // 部分指定はダメ
      error_select_in_pca(pt_expr);
      return false;
    }
    if ( type != kVpiReg &&
	 type != kVpiIntegerVar &&
	 type != kVpiRealVar &&
	 type != kVpiTimeVar) {
      // reg/変数以外はダメ
      error_illegal_object(pt_expr);
      return false;
    }
  }
  else if ( env.is_force_lhs() ) {
    // force 文の左辺式
    if ( decl->is_array_member() ) {
      // 配列要素はダメ
      error_array_in_force(pt_expr);
      return false;
    }
    if ( has_select ) {
      // 部分指定はダメ
      error_select_in_force(pt_expr);
      return false;
    }
    if ( type != kVpiNet &
	 type != kVpiReg &
	 type != kVpiIntegerVar &
	 type != kVpiRealVar &
	 type != kVpiTimeVar) {
      // net/reg/変数以外はダメ
      error_illegal_object(pt_expr);
      return false;
    }
  }
  else if ( env.is_net_lhs() &&
       type != kVpiNet ) {
    // net 以外はダメ
    error_illegal_object(pt_expr);
    return false;
  }
  else if ( env.is_var_lhs() &&
       ( type != kVpiReg &&
	 type != kVpiIntegerVar &&
	 type != kVpiRealVar &&
	 type != kVpiTimeVar ) ) {
    // reg/変数以外はダメ
    error_illegal_object(pt_expr);
    return false;
  }
  else {
    // 右辺系の環境
    if ( env.is_constant() ) {
      // 定数式
      if ( type != kVpiParameter &&
	   type != kVpiSpecParam ) {
	// 定数(parameter)でないのでダメ
	error_illegal_object(pt_expr);
	return false;
      }
    }

    // あとは個別の型ごとにチェックする．
    if ( type == kVpiRealVar && has_select ) {
      // real の部分選択は無効
      error_select_for_real(pt_expr);
      return false;
    }
    if ( type == kVpiNamedEvent && !env.is_event_expr() ) {
      // イベント式以外では名前つきイベントは使えない．
      error_illegal_object(pt_expr);
      return false;
    }
  }
  return true;
}

// @brief プライマリに対して式の値を評価する．
// @param[in] parent 親のスコープ
// @param[in] pt_expr 式を表すパース木
ElbValue
ExprGen::evaluate_primary(const VlNamedObj* parent,
			  const PtExpr* pt_expr)
{
  // 識別子の階層
  PtNameBranchArray nb_array = pt_expr->namebranch_array();
  if ( nb_array.size() > 0 ) {
    // 階層つき識別子はだめ
    error_hname_in_ce(pt_expr);
    return ElbValue();
  }

  bool has_range_select = (pt_expr->left_range() && pt_expr->right_range());

  ymuint isize = pt_expr->index_num();
  if (  isize > 1 || (isize == 1 && has_range_select) ) {
    // 配列型ではない．
    error_dimension_mismatch(pt_expr);
    return ElbValue();
  }
  bool has_bit_select = (isize == 1);

  int index1 = 0;
  int index2 = 0;
  if ( has_bit_select ) {
    if ( !evaluate_expr_int(parent, pt_expr->index(0), index1) ) {
      return ElbValue();
    }
  }
  if ( has_range_select ) {
    bool stat1 = evaluate_expr_int(parent, pt_expr->left_range(), index1);
    bool stat2 = evaluate_expr_int(parent, pt_expr->right_range(), index2);
    if ( !stat1 || !stat2 ) {
      return ElbValue();
    }
  }

  // 識別子の名前
  const char* name = pt_expr->name();

  // モジュール内の識別子を探索する．
  ElbObjHandle* handle = find_obj_up(parent, PtNameBranchArray(), name,
				     parent->parent_module());
  if ( !handle ) {
    // 見つからなかった．
    error_not_found(pt_expr);
    return ElbValue();
  }

  // そのオブジェクトが genvar の場合
  ElbGenvar* genvar = handle->genvar();
  if ( genvar ) {
    if ( has_bit_select ) {
      // ビット選択
      BitVector bv(genvar->value());
      return ElbValue(bv.bit_select(index1));
    }
    else if ( has_range_select ) {
      // 範囲選択
      BitVector bv(genvar->value());
      return ElbValue(bv.part_select(index1, index2));
    }
    else {
      return ElbValue(genvar->value());
    }
  }

  // それ以外の宣言要素の場合
  // しかしこの場合には parameter でなければならない．
  ElbDecl* param = handle->decl();
  if ( !param || param->type() != kVpiParameter ) {
    error_not_a_parameter(pt_expr);
    return ElbValue();
  }
  ElbValue val;
  if ( param->value_type() == kVpiValueReal ) {
    if ( has_bit_select || has_range_select ) {
      error_illegal_real_type(pt_expr);
      return ElbValue();
    }
    val = ElbValue(param->get_real());
  }
  else {
    BitVector bv;
    param->get_bitvector(bv);
    val = ElbValue(bv);

    if ( has_bit_select ) {
      val.to_bitvector();
      if ( val.is_error() ) {
	error_illegal_real_type(pt_expr);
	return ElbValue();
      }
      int offset = param->bit_offset(index1);
      return ElbValue(val.bitvector_value().bit_select(offset));
    }
    else if ( has_range_select ) {
      val.to_bitvector();
      if ( val.is_error() ) {
	error_illegal_real_type(pt_expr);
	return ElbValue();
      }
      int msb_offset;
      int lsb_offset;
      switch ( pt_expr->range_mode() ) {
      case kVpiConstRange:
	msb_offset = param->bit_offset(index1);
	lsb_offset = param->bit_offset(index2);
	break;

      case kVpiPlusRange:
#warning "TODO: 仕様を確認"
	assert_not_reached(__FILE__, __LINE__);
	break;

      case kVpiMinusRange:
#warning "TODO: 仕様を確認"
	assert_not_reached(__FILE__, __LINE__);
	break;

      case kVpiNoRange:
	assert_not_reached(__FILE__, __LINE__);
	break;
      }
      return ElbValue(val.bitvector_value().part_select(msb_offset, lsb_offset));
    }
  }

  return val;
}

END_NAMESPACE_YM_VERILOG
