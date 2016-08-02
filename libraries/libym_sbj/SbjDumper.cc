
/// @file libym_sbj/SbjDumper.cc
/// @brief SbjDumper の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: SbjGraph.cc 2274 2009-06-10 07:45:29Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_sbj/SbjDumper.h"
#include "ym_sbj/SbjGraph.h"


BEGIN_NAMESPACE_YM_SBJ

// @brief コンストラクタ
SbjDumper::SbjDumper()
{
}

// @brief デストラクタ
SbjDumper::~SbjDumper()
{
}

/// @brief 独自形式で出力する．
/// @param[in] s 出力先のストリーム
/// @param[in] sbjgraph 対象のサブジェクトグラフ
void
SbjDumper::dump(ostream& s,
		const SbjGraph& sbjgraph)
{
  ymuint np = sbjgraph.port_num();
  for (ymuint i = 0; i < np; ++ i) {
    s << "Port#" << i << ":";
    const SbjPort* port = sbjgraph.port(i);
    s << " " << port->name() << " = ";
    ymuint nb = port->bit_width();
    assert_cond( nb > 0, __FILE__, __LINE__);
    if ( nb == 1 ) {
      const SbjNode* node = port->bit(0);
      s << " " << node->id_str();
    }
    else {
      s << "{";
      const char* comma = "";
      for (ymuint i = 0; i < nb; ++ i) {
	ymuint idx = nb - i - 1;
	const SbjNode* node = port->bit(idx);
	s << comma << node->id_str();
	comma = ", ";
      }
      s << "}";
    }
    s << endl;
  }
  s << endl;

  const SbjNodeList& input_list = sbjgraph.input_list();
  for (SbjNodeList::const_iterator p = input_list.begin();
       p != input_list.end(); ++ p) {
    const SbjNode* node = *p;
    s << "Input#" << node->subid() << ": " << node->id_str()
      << " : " << sbjgraph.port(node)->name()
      << "[" << sbjgraph.port_pos(node) << "]"
      << endl;
  }

  const SbjNodeList& output_list = sbjgraph.output_list();
  for (SbjNodeList::const_iterator p = output_list.begin();
       p != output_list.end(); ++ p) {
    const SbjNode* node = *p;
    const SbjEdge* e = node->fanin_edge(0);
    s << "Output#" << node->subid() << ": " << node->id_str()
      << " : " << sbjgraph.port(node)->name()
      << "[" << sbjgraph.port_pos(node) << "]"
      << " = ";
    const SbjNode* inode = e->from();
    if ( inode ) {
      // 普通のノードの場合
      if ( node->output_inv() ) {
	s << "~";
      }
      s << inode->id_str();
    }
    else {
      // 定数ノードの場合
      if ( node->output_inv() ) {
	s << "1";
      }
      else {
	s << "0";
      }
    }
    s << endl;
  }

  const SbjNodeList& dff_list = sbjgraph.dff_list();
  for (SbjNodeList::const_iterator p = dff_list.begin();
       p != dff_list.end(); ++ p) {
    const SbjNode* node = *p;
    s << "DFF(" << node->id_str() << ") :";
    s << "DATA = ";
    const SbjNode* inode = node->fanin_data();
    if ( inode ) {
      // 普通のノードの場合
      if ( node->fanin_data_inv() ) {
	s << "~";
      }
      s << inode->id_str();
    }
    else {
      // 定数ノードの場合
      if ( node->fanin_data_inv() ) {
	s << "1";
      }
      else {
	s << "0";
      }
    }

    const SbjNode* cnode = node->fanin_clock();
    if ( cnode ) {
      s << ", CLOCK = ";
      if ( node->fanin_clock_inv() ) {
	s << "~";
      }
      s << cnode->id_str();
    }

    const SbjNode* snode = node->fanin_set();
    if ( snode ) {
      s << ", SET = ";
      if ( node->fanin_set_inv() ) {
	s << "~";
      }
      s << snode->id_str();
    }

    const SbjNode* rnode = node->fanin_rst();
    if ( rnode ) {
      s << ", RST = ";
      if ( node->fanin_rst_inv() ) {
	s << "~";
      }
      s << rnode->id_str();
    }

    s << endl;
  }

  const SbjNodeList& lnode_list = sbjgraph.lnode_list();
  for (SbjNodeList::const_iterator p = lnode_list.begin();
       p != lnode_list.end(); ++ p) {
    const SbjNode* node = *p;
    const char* pol0 = node->fanin_inv(0) ? "~" : "";
    const char* pol1 = node->fanin_inv(1) ? "~" : "";
    const char* op   = node->is_xor() ? "^" : "&";
    s << "Logic(" << node->id_str() << ") = "
      << pol0 << node->fanin(0)->id_str()
      << " " << op << " "
      << pol1 << node->fanin(1)->id_str()
      << endl;
  }
}

// @brief blif 形式で出力する．
// @param[in] s 出力先のストリーム
// @param[in] sbjgraph 対象のサブジェクトグラフ
void
SbjDumper::dump_blif(ostream& s,
		     const SbjGraph& sbjgraph)
{
  s << ".model " << sbjgraph.name() << endl;
  const SbjNodeList& input_list = sbjgraph.input_list();
  for (SbjNodeList::const_iterator p = input_list.begin();
       p != input_list.end(); ++ p) {
    const SbjNode* node = *p;
    s << ".inputs " << node->id_str() << endl;
  }

  const SbjNodeList& output_list = sbjgraph.output_list();
  for (SbjNodeList::const_iterator p = output_list.begin();
       p != output_list.end(); ++ p) {
    const SbjNode* node = *p;
    s << ".outputs " << node->id_str() << endl;
  }
  for (SbjNodeList::const_iterator p = output_list.begin();
       p != output_list.end(); ++ p) {
    const SbjNode* node = *p;
    const SbjNode* inode = node->fanin(0);
    if ( inode == 0 ) {
      s << ".names " << node->id_str() << endl;
      if ( node->output_inv() ) {
	s << "1" << endl;
      }
      else {
	s << "0" << endl;
      }
    }
    else {
      s << ".names " << inode->id_str() << " " << node->id_str() << endl;
      if ( node->output_inv() ) {
	s << "0 1" << endl;
      }
      else {
	s << "1 1" << endl;
      }
    }
    s << endl;
  }

  const SbjNodeList& dff_list = sbjgraph.dff_list();
  for (SbjNodeList::const_iterator p = dff_list.begin();
       p != dff_list.end(); ++ p) {
    const SbjNode* node = *p;
    const SbjNode* inode = node->fanin(0);
    s << ".latch " << node->id_str() << " "
      << inode->id_str() << endl;
  }

  const SbjNodeList& lnode_list = sbjgraph.lnode_list();
  for (SbjNodeList::const_iterator p = lnode_list.begin();
       p != lnode_list.end(); ++ p) {
    const SbjNode* node = *p;
    s << ".names " << node->fanin(0)->id_str()
      << " " << node->fanin(1)->id_str()
      << " " << node->id_str() << endl;
    if ( node->is_and() ) {
      if ( node->fanin_inv(0) ) {
	if ( node->fanin_inv(1) ) {
	  s << "00 1" << endl;
	}
	else {
	  s << "01 1" << endl;
	}
      }
      else {
	if ( node->fanin_inv(1) ) {
	  s << "10 1" << endl;
	}
	else {
	  s << "11 1" << endl;
	}
      }
    }
    else {
      s << "10 1" << endl
	<< "01 1" << endl;
    }
    s << endl;
  }
  s << ".end" << endl;
}


BEGIN_NONAMESPACE

// ノード名を返す．
string
node_name(const SbjNode* node)
{
  return node->id_str();
}

END_NONAMESPACE


// @brief Verilog-HDL 形式で出力する関数
// @param[in] s 出力先のストリーム
// @param[in] sbjgraph 対象のネットワーク
void
SbjDumper::dump_verilog(ostream& s,
			const SbjGraph& sbjgraph)
{
  // module 文
  s << "module " << sbjgraph.name() << "(";
  ymuint np = sbjgraph.port_num();
  const char* sep = "";
  for (ymuint i = 0; i < np; ++ i) {
    const SbjPort* port = sbjgraph.port(i);
    s << sep << "." << port->name() << "(";
    ymuint nb = port->bit_width();
    assert_cond( nb > 0 , __FILE__, __LINE__);
    if ( nb == 1 ) {
      const SbjNode* node = port->bit(0);
      s << node_name(node);
    }
    else {
      s << "{";
      const char* comma = "";
      for (ymuint j = 0; j < nb; ++ j) {
	ymuint idx = nb - j - 1;
	const SbjNode* node = port->bit(idx);
	s << comma << node_name(node);
	comma = ", ";
      }
      s << "}";
    }
    s << ")";
    sep = ", ";
  }
  s << ");" << endl;

  const SbjNodeList& input_list = sbjgraph.input_list();
  const SbjNodeList& output_list = sbjgraph.output_list();
  const SbjNodeList& lnode_list = sbjgraph.lnode_list();
  const SbjNodeList& dff_list = sbjgraph.dff_list();

  // input 文
  for (SbjNodeList::const_iterator p = input_list.begin();
       p != input_list.end(); ++ p) {
    const SbjNode* node = *p;
    s << "  input  " << node_name(node) << ";" << endl;
  }
  s << endl;

  // output 文
  for (SbjNodeList::const_iterator p = output_list.begin();
       p != output_list.end(); ++ p) {
    const SbjNode* node = *p;
    s << "  output " << node_name(node) << ";" << endl;
  }
  s << endl;

  // reg 定義
  for (SbjNodeList::const_iterator p = dff_list.begin();
       p != dff_list.end(); ++ p) {
    const SbjNode* node = *p;
    s << "  reg    " << node_name(node) << ";" << endl;
  }
  s << endl;

  // wire 定義
  for (SbjNodeList::const_iterator p = lnode_list.begin();
       p != lnode_list.end(); ++ p) {
    const SbjNode* node = *p;
    s << "  wire   " << node_name(node) << ";" << endl;
  }
  s << endl;

  // output 用の assign 文
  for (SbjNodeList::const_iterator p = output_list.begin();
       p != output_list.end(); ++ p) {
    const SbjNode* node = *p;
    const SbjNode* inode = node->fanin(0);
    s << "  assign " << node_name(node) << " = ";
    if ( inode == 0 ) {
      if ( node->output_inv() ) {
	s << "1'b1";
      }
      else {
	s << "1'b0";
      }
    }
    else {
      if ( node->output_inv() ) {
	s << "~";
      }
      s << node_name(inode);
    }
    s << ";" << endl;
  }
  s << endl;

  // 論理ノード用の assign 文
  for (SbjNodeList::const_iterator p = lnode_list.begin();
       p != lnode_list.end(); ++ p) {
    const SbjNode* node = *p;
    s << "  assign " << node_name(node) << " = ";

    const char* pol0 = node->fanin_inv(0) ? "~" : "";
    const char* pol1 = node->fanin_inv(1) ? "~" : "";
    const char* op   = node->is_xor() ? "^" : "&";
    s << pol0 << node_name(node->fanin(0))
      << " " << op << " "
      << pol1 << node_name(node->fanin(1))
      << ";" << endl;
  }
  s << endl;

  // ff 用の always 文
  for (SbjNodeList::const_iterator p = dff_list.begin();
       p != dff_list.end(); ++ p) {
    const SbjNode* node = *p;
    const SbjNode* dnode = node->fanin_data();
    bool dinv = node->fanin_data_inv();
    const SbjNode* cnode = node->fanin_clock();
    bool cinv = node->fanin_clock_inv();
    const SbjNode* snode = node->fanin_set();
    bool sinv = node->fanin_set_inv();
    const SbjNode* rnode = node->fanin_rst();
    bool rinv = node->fanin_rst_inv();
    assert_cond( cnode != NULL, __FILE__, __LINE__);
    s << "  always @ ( ";
    if ( cinv ) {
      s << "negedge";
    }
    else {
      s << "posedge";
    }
    s << " " << node_name(cnode);
    if ( snode ) {
      s << " or ";
      if ( sinv ) {
	s << "negedge";
      }
      else {
	s << "posedge";
      }
      s << " " << node_name(snode);
    }
    if ( rnode ) {
      s << " or ";
      if ( rinv ) {
	s << "negedge";
      }
      else {
	s << "posedge";
      }
      s << " " << node_name(rnode);
    }
    s << " )" << endl;
    if ( snode ) {
      s << "    if ( ";
      if ( sinv ) {
	s << "!";
      }
      s << " " << node_name(snode) << " )" << endl;
      s << "      " << node_name(node)
	<< " <= 1;" << endl;
    }
    if ( rnode ) {
      s << "    ";
      if ( snode ) {
	s << "else ";
      }
      s << "if ( ";
      if ( rinv ) {
	s << "!";
      }
      s << " " << node_name(rnode) << " )" << endl;
      s << "      " << node_name(node)
	<< " <= 0;" << endl;
    }
    if ( snode || rnode ) {
      s << "    else" << endl
	<< "  ";
    }
    s << "    " << node_name(node) << " <= ";
    if ( dinv ) {
      s << "~";
    }
    s << node_name(dnode) << ";" << endl;
    s << endl;
  }
  s << "endmodule" << endl;
}

END_NAMESPACE_YM_SBJ
