
/// @file libym_lutmap/LnGraph_dump.cc
/// @brief LnGraph 用の dump() の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: LutNetwork.cc 2274 2009-06-10 07:45:29Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_lutmap/LnGraph.h"


BEGIN_NAMESPACE_YM_LUTMAP

// 内容を s に出力する．
void
dump(ostream& s,
     const LnGraph& lngraph)
{
  ymuint np = lngraph.port_num();
  for (ymuint i = 0; i < np; ++ i) {
    const LnPort* port = lngraph.port(i);
    s << "PORT#" << i << "(" << port->name() << "): ";

    ymuint nb = port->bit_width();
    assert_cond( nb > 0 , __FILE__, __LINE__);
    if ( nb == 1 ) {
      const LnNode* node = port->bit(0);
      s << node->id_str();
    }
    else {
      s << "{";
      const char* comma = "";
      for (ymuint j = 0; j < nb; ++ j) {
	ymuint idx = nb - j - 1;
	const LnNode* node = port->bit(idx);
	s << comma << node->id_str();
	comma = ", ";
      }
      s << "}";
    }
    s << endl;
  }

  const LnNodeList& input_list = lngraph.input_list();
  for (LnNodeList::const_iterator p = input_list.begin();
       p != input_list.end(); ++ p) {
    const LnNode* node = *p;
    s << "INPUT#" << node->subid() << "(" << node->id_str() << ")"
      << " : " << lngraph.port(node)->name()
      << "[" << lngraph.port_pos(node) << "]"
      << endl;
  }
  const LnNodeList& output_list = lngraph.output_list();
  for (LnNodeList::const_iterator p = output_list.begin();
       p != output_list.end(); ++ p) {
    const LnNode* node = *p;
    const LnEdge* e = node->fanin_edge(0);
    s << "OUTPUT#" << node->subid() << "(" << node->id_str() << ")"
      << " : " << lngraph.port(node)->name()
      << "[" << lngraph.port_pos(node) << "]"
      << " = ";
    const LnNode* inode = e->from();
    if ( inode ) {
      // 普通のノードの場合
      s << inode->id_str();
    }
    else {
      // 定数ノードの場合
      s << "0";
    }
    s << endl;
  }
  const LnNodeList& dff_list = lngraph.dff_list();
  for (LnNodeList::const_iterator p = dff_list.begin();
       p != dff_list.end(); ++ p) {
    const LnNode* node = *p;
    const LnNode* inode = node->fanin(0);
    const LnNode* cnode = node->fanin(1);
    s << "DFF(" << node->id_str() << "):"
      << " DATA = " << inode->id_str()
      << " , CLOCK = " << cnode->id_str();
    const LnNode* snode = node->fanin(2);
    if ( snode ) {
      s << ", SET = " << snode->id_str();
    }
    const LnNode* rnode = node->fanin(3);
    if ( rnode ) {
      s << ", RST = " << rnode->id_str();
    }
    s << endl;
  }

  const LnNodeList& lut_list = lngraph.lnode_list();
  for (LnNodeList::const_iterator p = lut_list.begin();
       p != lut_list.end(); ++ p) {
    const LnNode* node = *p;
    s << "LUT(" << node->id_str() << ")  = (";
    const char* comma = "";
    ymuint ni = node->fanin_num();
    for (ymuint i = 0; i < ni; ++ i) {
      const LnNode* inode = node->fanin(i);
      s << comma << inode->id_str();
      comma = ", ";
    }
    s << ")";
    s << endl;
    ymuint np = 1U << ni;
    vector<int> tv;
    node->tv(tv);
    s << "\t";
    for (ymuint i = 0; i < np; ++ i) {
      s << tv[i];
    }
    s << endl;
  }
}


BEGIN_NONAMESPACE

// ノード名を返す．
string
node_name(const LnNode* node)
{
  return "n" + node->id_str();
}

// LUT の名前を作る．
string
lut_name(const LnNode* node)
{
  ymuint ni = node->fanin_num();
  vector<int> tv;
  node->tv(tv);
  ymuint nip = 1U << ni;
  assert_cond( tv.size() == nip, __FILE__, __LINE__);

  ostringstream buf;
  buf << "lut" << ni << "_";
  if ( ni < 2 ) {
    ymuint val = 0U;
    for (ymuint b = 0; b < nip; ++ b) {
      if ( tv[b] ) {
	val += (1U << b);
      }
    }
    buf << hex << val;
  }
  else {
    for (ymuint b0 = nip; b0 > 0; ) {
      b0 -= 4;
      ymuint val = 0U;
      for (ymuint b = 0; b < 4; ++ b) {
	if ( tv[b0 + b] ) {
	  val += (1U << b);
	}
      }
      buf << hex << val;
    }
  }
  string name = buf.str();
  return name;
}

// LUT の内容を表すプリミティブ記述を作る．
void
dump_lut(ostream& s,
	 const LnNode* node,
	 const string& name)
{
  ymuint ni = node->fanin_num();
  ymuint nip = 1U << ni;

  vector<int> tv;
  node->tv(tv);
  assert_cond( tv.size() == nip, __FILE__, __LINE__);

  s << endl;
  s << "primitive " << name << " ( ";
  s << " O";
  for (ymuint i = 0; i < ni; ++ i) {
    s << ", I" << i;
  }
  s << " );" << endl;
  s << "  output O;" << endl;
  s << "  input ";
  const char* sep = "";
  for (ymuint i = 0; i < ni; ++ i) {
    s << sep << " I" << i;
    sep = ", ";
  }
  s << ";" << endl;
  s << endl;
  s << "  table" << endl;
  for (ymuint b = 0; b < nip; ++ b) {
    s << "    ";
    for (ymuint i = 0; i < ni; ++ i) {
      if ( b & (1U << i) ) {
	s << " 1";
      }
      else {
	s << " 0";
      }
    }
    s << " : ";
    if ( tv[b] ) {
      s << "1";
    }
    else {
      s << "0";
    }
    s << ";" << endl;
  }
  s << "  endtable" << endl;
  s << "endprimitive" << endl;
}

END_NONAMESPACE

// 内容を Verilog-HDL 形式で s に出力する．
void
dump_verilog(ostream& s,
	     const LnGraph& lngraph)
{
  const LnNodeList& input_list = lngraph.input_list();
  const LnNodeList& output_list = lngraph.output_list();
  const LnNodeList& dff_list = lngraph.dff_list();
  const LnNodeList& lut_list = lngraph.lnode_list();

  hash_set<string> lut_name_set;
  ymuint n = lngraph.max_node_id();
  vector<string> lut_names(n);

  // プリミティブ記述の生成
  for (LnNodeList::const_iterator p = lut_list.begin();
       p != lut_list.end(); ++ p) {
    const LnNode* node = *p;
    if ( node->fanin_num() > 0 ) {
      string name = lut_name(node);
      lut_names[node->id()] = name;
      if ( lut_name_set.count(name) == 0 ) {
	dump_lut(s, node, name);
	lut_name_set.insert(name);
      }
    }
  }

  s << endl;
  s << "module " << lngraph.name() << "(";
  ymuint np = lngraph.port_num();
  const char* sep = "";
  for (ymuint i = 0; i < np; ++ i) {
    const LnPort* port = lngraph.port(i);
    s << sep << "." << port->name() << "(";
    ymuint nb = port->bit_width();
    assert_cond( nb > 0 , __FILE__, __LINE__);
    if ( nb == 1 ) {
      const LnNode* node = port->bit(0);
      s << node_name(node);
    }
    else {
      s << "{";
      const char* comma = "";
      for (ymuint j = 0; j < nb; ++ j) {
	ymuint idx = nb - j - 1;
	const LnNode* node = port->bit(idx);
	s << comma << node_name(node);
	comma = ", ";
      }
      s << "}";
    }
    s << ")";
    sep = ", ";
  }
  s << ");" << endl;

  for (LnNodeList::const_iterator p = input_list.begin();
       p != input_list.end(); ++ p) {
    const LnNode* node = *p;
    s << "  input  " << node_name(node) << ";" << endl;
  }
  for (LnNodeList::const_iterator p = output_list.begin();
       p != output_list.end(); ++ p) {
    const LnNode* node = *p;
    s << "  output " << node_name(node) << ";" << endl;
  }
  s << endl;

  for (LnNodeList::const_iterator p = dff_list.begin();
       p != dff_list.end(); ++ p) {
    const LnNode* node = *p;
    s << "  reg    " << node_name(node) << ";" << endl;
  }
  s << endl;

  for (LnNodeList::const_iterator p = lut_list.begin();
       p != lut_list.end(); ++ p) {
    const LnNode* node = *p;
    s << "  wire   " << node_name(node) << ";" << endl;
  }
  s << endl;

  for (LnNodeList::const_iterator p = output_list.begin();
       p != output_list.end(); ++ p) {
    const LnNode* node = *p;
    const LnEdge* e = node->fanin_edge(0);
    const LnNode* inode = e->from();
    assert_cond( inode != NULL, __FILE__, __LINE__);
    s << "  assign " << node_name(node)
      << " = " << node_name(inode) << ";" << endl;
  }

  // プリミティブインスタンス記述の生成
  for (LnNodeList::const_iterator p = lut_list.begin();
       p != lut_list.end(); ++ p) {
    const LnNode* node = *p;
    ymuint ni = node->fanin_num();
    if ( ni == 0 ) {
      // 特例
      s << "  assign " << node_name(node) << " = 1'b";
      vector<int> tv;
      node->tv(tv);
      if ( tv[0] ) {
	s << "1";
      }
      else {
	s << "0";
      }
      s << ";" << endl;
    }
    else {
      s << "  " << lut_names[node->id()] << " U" << node->id()
	<< " ( " << node_name(node);
      for (ymuint i = 0; i < ni; ++ i) {
	const LnNode* inode = node->fanin(i);
	s << ", " << node_name(inode);
      }
      s << " );" << endl;
    }
  }
  s << endl;

  // ff 用の always 文
  for (LnNodeList::const_iterator p = dff_list.begin();
       p != dff_list.end(); ++ p) {
    const LnNode* node = *p;
    const LnNode* inode = node->fanin(0);
    const LnNode* cnode = node->fanin(1);
    const LnNode* snode = node->fanin(2);
    const LnNode* rnode = node->fanin(3);
    s << "  always @ ( posedge " << node_name(cnode);
    if ( snode ) {
      s << " or posedge " << node_name(snode);
    }
    if ( rnode ) {
      s << " or posedge " << node_name(rnode);
    }
    s << " )" << endl;
    if ( snode ) {
      s << "    if ( " << node_name(snode) << " )" << endl
	<< "      " << node_name(node) << " <= 1;" << endl;
    }
    if ( rnode ) {
      s << "    ";
      if ( snode ) {
	s << "else ";
      }
      s << "if ( " << node_name(rnode) << " )" << endl
	<< "      " << node_name(node) << " <= 0;" << endl;
    }
    if ( snode || rnode ) {
      s << "    else" << endl
	<< "  ";
    }
    s << "    " << node_name(node) << " <= "
      << node_name(inode) << ";" << endl
      << endl;
  }
  s << "endmodule" << endl;
}

END_NAMESPACE_YM_LUTMAP
