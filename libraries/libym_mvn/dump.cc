
/// @file libym_mvn/dump.cc
/// @brief dump() の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_mvn/MvMgr.h"

#include "ym_mvn/MvModule.h"
#include "ym_mvn/MvPort.h"
#include "ym_mvn/MvNode.h"
#include "ym_mvn/MvPin.h"

#include "ym_verilog/vl/VlDecl.h"
#include "ym_verilog/vl/VlRange.h"


BEGIN_NAMESPACE_YM_MVN

BEGIN_NONAMESPACE

// MvInputPin の内容を出力する．
void
dump_inputpin(ostream& s,
	      const MvInputPin* pin)
{
  s << "  InputPin#" << pin->pos()
    << "(" << pin->bit_width() << ")" << endl;
  const MvOutputPin* opin = pin->src_pin();
  if ( opin ) {
    s << "    <== OutputPin#" << opin->pos()
      << "@node#" << opin->node()->id() << endl;
  }
}

// MvOutputPin の内容を出力する．
void
dump_outputpin(ostream& s,
	       const MvOutputPin* pin)
{
  s << "  OutputPin#" << pin->pos()
    << "(" << pin->bit_width() << ")" << endl;
  const MvInputPinList& fo_list = pin->dst_pin_list();
  for (MvInputPinList::const_iterator p = fo_list.begin();
       p != fo_list.end(); ++ p) {
    const MvInputPin* ipin = *p;
    s << "    ==> InputPin#" << ipin->pos()
      << "@node#" << ipin->node()->id() << endl;
  }
}

// MvNode の内容を出力する．
void
dump_node(ostream& s,
	  const MvNode* node)
{
  s << "Node#" << node->id()
    << ":";
  switch ( node->type() ) {
  case MvNode::kInput:      s << "Input"; break;
  case MvNode::kInout:      s << "Inout"; break;
  case MvNode::kOutput:     s << "Output"; break;
  case MvNode::kDff1:       s << "DFF1"; break;
  case MvNode::kDff2:       s << "DFF2"; break;
  case MvNode::kThrough:    s << "Through"; break;
  case MvNode::kNot:        s << "Not"; break;
  case MvNode::kAnd:        s << "And"; break;
  case MvNode::kOr:         s << "Or"; break;
  case MvNode::kXor:        s << "Xor"; break;
  case MvNode::kRand:       s << "Rand"; break;
  case MvNode::kRor:        s << "Ror"; break;
  case MvNode::kRxor:       s << "Rxor"; break;
  case MvNode::kEq:         s << "Eq"; break;
  case MvNode::kLt:         s << "Lt"; break;
  case MvNode::kSll:        s << "Sll"; break;
  case MvNode::kSrl:        s << "Srl"; break;
  case MvNode::kSla:        s << "Sla"; break;
  case MvNode::kSra:        s << "Sra"; break;
  case MvNode::kAdd:        s << "Add"; break;
  case MvNode::kSub:        s << "Sub"; break;
  case MvNode::kMult:       s << "Mult"; break;
  case MvNode::kDiv:        s << "Div"; break;
  case MvNode::kMod:        s << "Mod"; break;
  case MvNode::kPow:        s << "Pow"; break;
  case MvNode::kIte:        s << "Ite"; break;
  case MvNode::kConcat:     s << "Concat"; break;
  case MvNode::kConstBitSelect:
    s << "ConstBitSelect["
      << node->bitpos()
      << "]";
    break;
  case MvNode::kConstPartSelect:
    s << "ConstPartSelect["
      << node->msb()
      << ":"
      << node->lsb()
      << "]";
    break;
  case MvNode::kBitSelect:  s << "BitSelect"; break;
  case MvNode::kPartSelect: s << "PartSelect"; break;
  case MvNode::kCombUdp:    s << "Combinational UDP"; break;
  case MvNode::kSeqUdp:     s << "Sequential UDP"; break;
  case MvNode::kConst:
    s << "Const";
    break;
  default:
    assert_not_reached(__FILE__, __LINE__);
  }
  s << endl;
  ymuint ni = node->input_num();
  for (ymuint i = 0; i < ni; ++ i) {
    const MvInputPin* pin = node->input(i);
    dump_inputpin(s, pin);
  }
  ymuint no = node->output_num();
  for (ymuint i = 0; i < no; ++ i) {
    const MvOutputPin* pin = node->output(i);
    dump_outputpin(s, pin);
  }
  s << endl;
}

END_NONAMESPACE


//////////////////////////////////////////////////////////////////////
// グローバル関数
//////////////////////////////////////////////////////////////////////

// @brief 内容を出力する
// @param[in] s 出力先のストリーム
// @param[in] mgr MvMgr
// @note デバッグ用
void
dump(ostream& s,
     const MvMgr& mgr)
{
  ymuint n = mgr.max_module_id();
  for (ymuint i = 0; i < n; ++ i) {
    const MvModule* module = mgr.module(i);
    if ( module == NULL ) continue;

    s << "Module#" << module->id() << "(";
    if ( module->name() ) {
      s << module->name();
    }
    s << ")" << endl;
    const MvNode* pnode = module->parent();
    if ( pnode ) {
      s << "  parent node: Module#" << pnode->parent()->id()
	<< ":" << pnode->id() << endl;
    }
    else {
      s << "  toplevel module" << endl;
    }

    ymuint np = module->port_num();
    for (ymuint j = 0; j < np; ++ j) {
      const MvPort* port = module->port(j);
      s << "  Port#" << j << "(";
      if ( port->name() ) {
	s << port->name();
      }
      s << ")" << endl;
      ymuint n = port->port_ref_num();
      for (ymuint k = 0; k < n; ++ k) {
	const MvPortRef* port_ref = port->port_ref(k);
	s << "    node#" << port_ref->node()->id();
	if ( port_ref->has_bitselect() ) {
	  s << "[" << port_ref->bitpos() << "]";
	}
	else if ( port_ref->has_partselect() ) {
	  s << "[" << port_ref->msb() << ":" << port_ref->lsb() << "]";
	}
	s << endl;
      }
    }

    ymuint ni = module->input_num();
    for (ymuint j = 0; j < ni; ++ j) {
      dump_node(s, module->input(j));
    }
    ymuint no = module->output_num();
    for (ymuint j = 0;j < no; ++ j) {
      dump_node(s, module->output(j));
    }
    for (list<MvNode*>::const_iterator p = module->nodes_begin();
	 p != module->nodes_end(); ++ p) {
      MvNode* node = *p;
      dump_node(s, node);
    }

    s << endl;
  }
}

// @brief ノード番号ともとのVerilog名の対応を出力する．
void
dump_node_map(ostream& s,
	      const MvMgr& mgr,
	      const vector<pair<const VlDecl*, ymuint> >& node_map)
{
  ymuint n = mgr.max_node_id();
  for (ymuint i = 0; i < n; ++ i) {
    const MvNode* node = mgr.node(i);
    if ( node == NULL ) continue;
    if ( node_map.size() <= node->id() ) continue;
    const pair<const VlDecl*, ymuint>& p = node_map[node->id()];
    const VlDecl* decl = p.first;
    if ( decl == NULL ) continue;
    s << "// node" << node->id() << " : ";
    ymuint offset = p.second;
    s << decl->full_name();
    if ( decl->is_array() ) {
      ymuint d = decl->dimension();
      vector<int> index_array(d);
      for (ymuint i = 0; i < d; ++ i) {
	const VlRange* range = decl->range(i);
	ymuint n = range->size();
	index_array[i] = offset % n;
	offset /= n;
      }
      for (ymuint i = 0; i < d; ++ i) {
	s << "[" << index_array[d - i - 1] << "]";
      }
    }
    s << endl;
  }
}

END_NAMESPACE_YM_MVN
