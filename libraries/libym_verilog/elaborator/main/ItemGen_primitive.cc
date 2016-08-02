
/// @file libym_verilog/elaborator/ItemGen_primitive.cc
/// @brief ElbMgr の実装ファイル(UDP インスタンスの実体化)
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: ItemGen_primitive.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ItemGen.h"
#include "ElbEnv.h"

#include "ym_verilog/BitVector.h"

#include "ym_verilog/pt/PtItem.h"
#include "ym_verilog/pt/PtExpr.h"
#include "ym_verilog/pt/PtMisc.h"

#include "ElbUdp.h"
#include "ElbPrimitive.h"
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
// プリミティブインスタンス関係の instantiate 関数 (変な日本語)
//////////////////////////////////////////////////////////////////////

// @brief gate instance の生成を行う
// @param[in] parent 親のスコープ
// @param[in] pt_head ヘッダ
void
ItemGen::instantiate_gateheader(const VlNamedObj* parent,
				const PtItem* pt_head)
{
  const PtDelay* pt_delay = pt_head->delay();
  bool has_delay = (pt_delay != NULL);

  ElbPrimHead* prim_head = factory().new_PrimHead(parent, pt_head, has_delay);
  if ( has_delay ) {
    add_phase3stub(make_stub(this, &ItemGen::link_gate_delay,
			     prim_head, pt_delay));
  }
  
  for (ymuint i = 0; i < pt_head->size(); ++ i) {
    const PtInst* pt_inst = pt_head->inst(i);
    const FileRegion& fr = pt_inst->file_region();
    ymuint port_num = pt_inst->port_num();
    ymuint output_num;
    ymuint inout_num;
    ymuint input_num;
    switch ( ElbPrimitive::get_port_size(pt_head->prim_type(), port_num,
					 output_num, inout_num, input_num) ) {
    case -1:
      put_msg(__FILE__, __LINE__,
	      pt_inst->file_region(),
	      kMsgError,
	      "ELAB",
	      "Too few port connections.");
      continue;

    case 1:
      put_msg(__FILE__, __LINE__,
	      pt_inst->file_region(),
	      kMsgError,
	      "ELAB",
	      "Too many port connections.");
      continue;
    }

    const PtExpr* pt_left = pt_inst->left_range();
    const PtExpr* pt_right = pt_inst->right_range();
    if ( pt_left && pt_right ) {
      // 配列の場合
      ElbExpr* left = NULL;
      ElbExpr* right = NULL;
      int left_val = 0;
      int right_val = 0;
      if ( !instantiate_range(parent, pt_left, pt_right,
			      left, right,
			      left_val, right_val) ) {
	return;
      }

      ElbPrimArray* prim_array = factory().new_PrimitiveArray(prim_head,
							      pt_inst,
							      left, right,
							      left_val, right_val);
      reg_primarray(prim_array);
  
      // attribute instance の生成
      //instantiate_attribute(pt_head->attr_top(), false, prim_array);

      ostringstream buf;
      buf << "instantiating primitive array: " << prim_array->full_name();
      put_msg(__FILE__, __LINE__,
	      fr,
	      kMsgInfo,
	      "ELAB",
	      buf.str());
      
      add_phase3stub(make_stub(this, &ItemGen::link_prim_array,
			       prim_array, pt_inst));
    }
    else {
      // 単一の要素の場合
      ElbPrimitive* prim = factory().new_Primitive(prim_head,
						   pt_inst);
      reg_primitive(prim);
  
      // attribute instance の生成
      //instantiate_attribute(pt_head->attr_top(), false, prim);

      ostringstream buf;
      buf << "instantiating primitive: " << prim->full_name();
      put_msg(__FILE__, __LINE__,
	      fr,
	      kMsgInfo,
	      "ELAB",
	      buf.str());

      add_phase3stub(make_stub(this, &ItemGen::link_primitive,
			       prim, pt_inst));
    }
  }
}

// @brief UDP instance の生成を行う
// @param[in] parent 親のスコープ
// @param[in] pt_head ヘッダ
// @param[in] udpdefn UDP
void
ItemGen::instantiate_udpheader(const VlNamedObj* parent,
			       const PtItem* pt_head,
			       const ElbUdpDefn* udpdefn)
{
  PtConnectionArray pa_array = pt_head->paramassign_array();
  ymuint param_size = pa_array.size();
  const PtDelay* pt_delay = pt_head->delay();
  bool has_delay = ( pt_delay || param_size == 1 );
  
  ElbPrimHead* prim_head = factory().new_UdpHead(parent,
						 pt_head,
						 udpdefn,
						 has_delay);
  if ( has_delay ) {
    add_phase3stub(make_stub(this, &ItemGen::link_udp_delay,
			     prim_head, pt_head));
  }
  
  for (ymuint i = 0; i < pt_head->size(); ++ i) {
    const PtInst* pt_inst = pt_head->inst(i);
    if ( pt_inst->port_num() > 0 &&
	 pt_inst->port(0)->name() != NULL ) {
      put_msg(__FILE__, __LINE__,
	      pt_inst->file_region(),
	      kMsgError,
	      "ELAB",
	      "UDP instance cannot have named port list.");
      continue;
    }
    
    ymuint port_num = pt_inst->port_num();
    if ( udpdefn->port_num() != port_num ) {
      put_msg(__FILE__, __LINE__,
	      pt_inst->file_region(),
	      kMsgError,
	      "ELAB",
	      "Number of ports mismatch.");
      continue;
    }

    const PtExpr* pt_left = pt_inst->left_range();
    const PtExpr* pt_right = pt_inst->right_range();
    if ( pt_left && pt_right ) {
      // 配列
      ElbExpr* left = NULL;
      ElbExpr* right = NULL;
      int left_val = 0;
      int right_val = 0;
      if ( !instantiate_range(parent, pt_left, pt_right,
			      left, right,
			      left_val, right_val) ) {
	return;
      }
      
      ElbPrimArray* prim_array = factory().new_PrimitiveArray(prim_head,
							      pt_inst,
							      left, right,
							      left_val, right_val);
      reg_primarray(prim_array);
  
      // attribute instance の生成
      //instantiate_attribute(pt_head->attr_top(), false, prim_array);

      add_phase3stub(make_stub(this, &ItemGen::link_prim_array,
			       prim_array, pt_inst));
    }
    else {
      // 単一の要素
      ElbPrimitive* primitive = factory().new_Primitive(prim_head,
							pt_inst);
      reg_primitive(primitive);
  
      // attribute instance の生成
      //instantiate_attribute(pt_head->attr_top(), false, primitive);
      
      add_phase3stub(make_stub(this, &ItemGen::link_primitive,
			       primitive, pt_inst));
    }
  }
}

// @brief gate delay の生成を行う
// @param[in] prim_head ゲートプリミティブのヘッダ
// @param[in] pt_delay パース木の遅延式
void
ItemGen::link_gate_delay(ElbPrimHead* prim_head,
			 const PtDelay* pt_delay)
{
  const VlNamedObj* parent = prim_head->parent();
  ElbDelay* delay = instantiate_delay(parent, pt_delay);
  prim_head->set_delay(delay);
}

// @brief UDP instance に付随する遅延式の生成を行う．
// @param[in] parent 親のスコープ
// @param[in] prim_head ヘッダ
// @param[in] pt_head パース木のインスタンス定義ヘッダ
void
ItemGen::link_udp_delay(ElbPrimHead* prim_head,
			const PtItem* pt_head)
{
  const VlNamedObj* parent = prim_head->parent();
  ymuint param_size = pt_head->paramassign_array().size();
  const PtDelay* pt_delay = pt_head->delay();
  ElbDelay* delay = NULL;
  if ( pt_delay ) {
    delay = instantiate_delay(parent, pt_delay);
  }
  else if ( param_size == 1 ) {
    // ordered_param_list が実は遅延式だった．
    delay = instantiate_delay(parent, pt_head);
  }
  if ( delay ) {
    prim_head->set_delay(delay);
  }
}

// @brief UDP/primitive array instance で使われている式の名前解決を行う．
// @param[in] pt_inst インスタンス定義
// @param[in] prim_array primitive 配列
void
ItemGen::link_prim_array(ElbPrimArray* prim_array,
			 const PtInst* pt_inst)
{
  const VlNamedObj* parent = prim_array->parent();
  ymuint arraysize = prim_array->elem_num();
  
  // ポートの情報を得るために先頭の要素を取り出す．
  const VlPrimitive* prim = prim_array->elem_by_offset(0);
  
  ElbEnv env1;
  ElbNetLhsEnv env2(env1);
  for (ymuint index = 0; index < pt_inst->port_num(); ++ index) {
    const PtConnection* pt_con = pt_inst->port(index);
    const PtExpr* pt_expr = pt_con->expr();
    if ( !pt_expr ) {
      // 空の接続式は許されない．
      put_msg(__FILE__, __LINE__,
	      pt_con->file_region(),
	      kMsgError,
	      "ELAB",
	      "Empty expression in UDP/primitive instance port.");
      continue;
    }
    
    const VlPrimTerm* term = prim->prim_term(index);
    ElbExpr* tmp = NULL;
    if ( term->direction() == kVpiInput ) {
      // 入力に接続するのは通常の右辺式
      tmp = instantiate_expr(parent, env1, pt_expr);
    }
    else {
      // それ以外は左辺式
      tmp = instantiate_lhs(parent, env2, pt_expr);
    }
    if ( !tmp ) {
      continue;
    }

    tVpiValueType type = tmp->value_type();
    if ( type == kVpiValueReal ) {
      put_msg(__FILE__, __LINE__,
	      tmp->file_region(),
	      kMsgError,
	      "ELAB",
	      "Real expression cannot connect to UDP port.");
      continue;
    }
    ymuint expr_size = unpack_size(type);
    if ( expr_size == 1 ) {
      // サイズが等しければそのまま接続する．
      for (ymuint i = 0; i < arraysize; ++ i) {
	ElbPrimitive* prim = prim_array->_primitive_by_offset(i);
	prim->connect(index, tmp);
      }
    }
    else if ( expr_size == 0 ) {
      // サイズがなければ1ビットに直してから接続する．
      tmp->set_reqsize(pack(kVpiValueUS, 1));
      for (ymuint i = 0; i < arraysize; ++ i) {
	ElbPrimitive* prim = prim_array->_primitive_by_offset(i);
	prim->connect(index, tmp);
      }
    }
    else if ( expr_size == arraysize ) {
      // tmp を 1 ビットずつに分割して接続する．
      for (ymuint i = 0; i < arraysize; ++ i) {
	ElbPrimitive* prim = prim_array->_primitive_by_offset(i);
	ElbExpr* bit = factory().new_BitSelect(pt_expr, tmp, i);
	prim->connect(index, bit);
      }
    }
    else {
      ostringstream buf;
      buf << (index + 1) << num_suffix(index + 1)
	  << " port expression has illegal size.";
      put_msg(__FILE__, __LINE__,
	      pt_con->file_region(),
	      kMsgError,
	      "ELAB",
	      buf.str());
    }
  }
}

// @brief UDP/primitive instance で使われている式の名前解決を行う．
// @param[in] pt_inst インスタンス定義
// @param[in] primitive primitive
void
ItemGen::link_primitive(ElbPrimitive* primitive,
			const PtInst* pt_inst)
{
  const VlNamedObj* parent = primitive->parent();
  
  ElbEnv env1;
  ElbNetLhsEnv env2(env1);
  for (ymuint index = 0; index < pt_inst->port_num(); ++ index) {
    const PtConnection* pt_con = pt_inst->port(index);
    // UDP instance の場合には ai_list は無視する．
    const PtExpr* pt_expr = pt_con->expr();
    if ( !pt_expr ) {
      continue;
    }

    const VlPrimTerm* term = primitive->prim_term(index);
    ElbExpr* tmp = NULL;
    if ( term->direction() == kVpiInput ) {
      // 入力に接続するのは通常の右辺式
      tmp = instantiate_expr(parent, env1, pt_expr);
    }
    else {
      // それ以外は左辺式
      tmp = instantiate_lhs(parent, env2, pt_expr);
    }
    if ( !tmp ) {
      continue;
    }

    tVpiValueType type = tmp->value_type();
    if ( type == kVpiValueReal ) {
      put_msg(__FILE__, __LINE__,
	      tmp->file_region(),
	      kMsgError,
	      "ELAB",
	      "Real expression cannot connect to UDP port.");
      continue;
    }
    ymuint expr_size = unpack_size(type);
    if ( expr_size == 1 ) {
      // サイズが等しければそのまま接続する．
      primitive->connect(index, tmp);
    }
    else if ( expr_size == 0 ) {
      // サイズがなければ1ビットに直してから接続する．
      tmp->set_reqsize(pack(kVpiValueUS, 1));
      primitive->connect(index, tmp);
    }
    else {
      ostringstream buf;
      buf << (index + 1) << num_suffix(index + 1)
	  << " port expression has illegal size.";
      put_msg(__FILE__, __LINE__,
	      pt_con->file_region(),
	      kMsgError,
	      "ELAB",
	      buf.str());
    }
  }
}

END_NAMESPACE_YM_VERILOG
