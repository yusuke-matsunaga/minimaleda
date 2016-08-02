
/// @file libym_techmap/CnGraph_dump.cc
/// @brief CnGraph の内容を出力する関数
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_techmap/CnGraph.h"
#include "ym_cell/Cell.h"
#include "ym_cell/CellPin.h"


BEGIN_NAMESPACE_YM_TECHMAP

void
dump(ostream& s,
     const CnGraph& network)
{
  ymuint np = network.port_num();
  for (ymuint i = 0; i < np; ++ i) {
    const CnPort* port = network.port(i);
    s << "PORT#" << i << "(" << port->name() << "): ";

    ymuint nb = port->bit_width();
    assert_cond( nb > 0, __FILE__, __LINE__);
    if ( nb == 1 ) {
      const CnNode* node = port->bit(0);
      s << node->id_str();
    }
    else {
      s << "{";
      const char* comma = "";
      for (ymuint j = 0; j < nb; ++ j) {
	ymuint idx = nb - j - 1;
	const CnNode* node = port->bit(idx);
	s << comma << node->id_str();
	comma = ", ";
      }
      s << "}";
    }
    s << endl;
  }

  const CnNodeList& input_list = network.input_list();
  for (CnNodeList::const_iterator p = input_list.begin();
       p != input_list.end(); ++ p) {
    const CnNode* node = *p;
    s << "INPUT#" << node->subid() << "(" << node->id_str() << ")"
      << " : " << network.port(node)->name()
      << "[" << network.port_pos(node) << "]"
      << endl;
  }

  const CnNodeList& output_list = network.output_list();
  for (CnNodeList::const_iterator p = output_list.begin();
       p != output_list.end(); ++ p) {
    const CnNode* node = *p;
    const CnEdge* e = node->fanin_edge(0);
    const CnNode* inode = e->from();
    s << "OUTPUT#" << node->subid() << "(" << node->id_str() << ")"
      << " : " << network.port(node)->name()
      << "[" << network.port_pos(node) << "]"
      << " = " << inode->id_str() << endl;
  }

  const CnNodeList& dff_list = network.dff_list();
  for (CnNodeList::const_iterator p = dff_list.begin();
       p != dff_list.end(); ++ p) {
    const CnNode* node = *p;
    const CnNode* inode = node->fanin(0);
    const CnNode* cnode = node->fanin(1);
    s << "DFF(" << node->id_str() << "):"
      << " DATA = " << inode->id_str()
      << " , CLOCK = " << cnode->id_str();
    const CnNode* snode = node->fanin(2);
    if ( snode ) {
      s << ", SET = " << snode->id_str();
    }
    const CnNode* rnode = node->fanin(3);
    if ( rnode ) {
      s << ", RST = " << rnode->id_str();
    }
    s << endl;
  }

  const CnNodeList& cellnode_list = network.cellnode_list();
  for (CnNodeList::const_iterator p = cellnode_list.begin();
       p != cellnode_list.end(); ++ p) {
    const CnNode* node = *p;
    const Cell* cell = node->cell();
    s << "CELL(" << node->id_str() << ") = "
      << cell->name() << "(";
    const char* comma = "";
    ymuint ni = node->ni();
    for (ymuint i = 0; i < ni; ++ i) {
      const CnNode* inode = node->fanin(i);
      s << comma << inode->id_str();
      comma = ", ";
    }
    s << ")" << endl;
  }
}


BEGIN_NONAMESPACE

// ノード名を返す．
string
node_name(const CnNode* node)
{
  return "n" + node->id_str();
}

END_NONAMESPACE

// 内容を Verilog-HDL 形式で s に出力する．
void
dump_verilog(ostream& s,
	     const CnGraph& network)
{
  const CnNodeList& input_list = network.input_list();
  const CnNodeList& output_list = network.output_list();
  const CnNodeList& dff_list = network.dff_list();
  const CnNodeList& cellnode_list = network.cellnode_list();

  s << "module " << network.name() << "(";
  ymuint np = network.port_num();
  const char* sep = "";
  for (ymuint i = 0; i < np; ++ i) {
    const CnPort* port = network.port(i);
    s << sep << "." << port->name() << "(";
    ymuint nb = port->bit_width();
    assert_cond( nb > 0, __FILE__, __LINE__);
    if ( nb == 1 ) {
      const CnNode* node = port->bit(0);
      s << node_name(node);
    }
    else {
      s << "{";
      const char* comma = "";
      for (ymuint j = 0; j < nb; ++ j) {
	ymuint idx = nb - j - 1;
	const CnNode* node = port->bit(idx);
	s << comma << node_name(node);
	comma = ", ";
      }
      s << "}";
    }
    s << ")";
    sep = ", ";
  }
  s << ");" << endl;

  for (CnNodeList::const_iterator p = input_list.begin();
       p != input_list.end(); ++ p) {
    const CnNode* node = *p;
    s << "  input  " << node_name(node) << ";" << endl;
  }

  for (CnNodeList::const_iterator p = output_list.begin();
       p != output_list.end(); ++ p) {
    const CnNode* node = *p;
    s << "  output " << node_name(node) << ";" << endl;
  }

  for (CnNodeList::const_iterator p = dff_list.begin();
       p != dff_list.end(); ++ p) {
  }

  for (CnNodeList::const_iterator p = cellnode_list.begin();
       p != cellnode_list.end(); ++ p) {
    const CnNode* node = *p;
    s << "  wire   " << node_name(node) << ";" << endl;
  }

  for (CnNodeList::const_iterator p = output_list.begin();
       p != output_list.end(); ++ p) {
    const CnNode* node = *p;
    const CnEdge* e = node->fanin_edge(0);
    const CnNode* inode = e->from();
    assert_cond( inode != NULL, __FILE__, __LINE__);
    s << "  assign " << node_name(node)
      << " = " << node_name(inode) << ";" << endl;
  }

  for (CnNodeList::const_iterator p = cellnode_list.begin();
       p != cellnode_list.end(); ++ p) {
    const CnNode* node = *p;
    const Cell* cell = node->cell();
    assert_cond( cell != NULL, __FILE__, __LINE__);
    ymuint np = cell->pin_num();
    s << "  " << cell->name() << " U" << node->id() << " (";
    ymuint ipos = 0;
    const char* comma = "";
    for (ymuint i = 0; i < np; ++ i) {
      const CellPin* pin = cell->pin(i);
      const CnNode* node1 = NULL;
      if ( pin->direction() == nsCell::kDirInput ) {
	node1 = node->fanin(ipos);
	++ ipos;
      }
      else if ( pin->direction() == nsCell::kDirOutput ) {
	node1 = node;
      }
      s << comma << "." << pin->name() << "(" << node_name(node1) << ")";
      comma = ", ";
    }
    s << ");" << endl;
  }
  s << "endmodule" << endl;
}

END_NAMESPACE_YM_TECHMAP
