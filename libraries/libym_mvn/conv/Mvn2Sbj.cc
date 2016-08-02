
/// @file libym_mvn/conv/mvn2sbj.cc
/// @brief MvNetwork を SbjGraph に変換する関数
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_mvn/Mvn2Sbj.h"
#include "ym_sbj/SbjGraph.h"
#include "ym_mvn/MvMgr.h"
#include "ym_mvn/MvModule.h"
#include "ym_mvn/MvPort.h"
#include "ym_mvn/MvNode.h"
#include "ym_mvn/MvPin.h"
#include "ym_mvn/MvNodeMap.h"


BEGIN_NAMESPACE_YM_MVN

BEGIN_NONAMESPACE

void
make_and(SbjGraph& sbjgraph,
	 SbjNode* sbjnode0,
	 bool inv0,
	 SbjNode* sbjnode1,
	 bool inv1,
	 SbjNode*& sbjnode,
	 bool& inv)
{
  sbjnode = NULL;
  inv = false;
  if ( sbjnode0 == NULL ) {
    if ( inv0 ) {
      // 入力0が1固定
      sbjnode = sbjnode1;
      inv = inv1;
    }
    else {
      // 入力0が0固定
      sbjnode = NULL;
      inv = false;
    }
  }
  else if ( sbjnode1 == NULL ) {
    if ( inv1 ) {
      // 入力1が1固定
      sbjnode = sbjnode0;
      inv = inv0;
    }
    else {
      // 入力1が0固定
      sbjnode = NULL;
      inv = false;
    }
  }
  else {
    sbjnode = sbjgraph.new_and(sbjnode0, sbjnode1, inv0, inv1);
    inv = false;
  }
}

void
make_or(SbjGraph& sbjgraph,
	SbjNode* sbjnode0,
	bool inv0,
	SbjNode* sbjnode1,
	bool inv1,
	SbjNode*& sbjnode,
	bool& inv)
{
  sbjnode = NULL;
  inv = false;
  if ( sbjnode0 == NULL ) {
    if ( inv0 ) {
      // 入力0が1固定
      sbjnode = NULL;
      inv = true;
    }
    else {
      // 入力0が0固定
      sbjnode = sbjnode1;
      inv = inv1;
    }
  }
  else if ( sbjnode1 == NULL ) {
    if ( inv1 ) {
      // 入力1が1固定
      sbjnode = NULL;
      inv = true;
    }
    else {
      // 入力1が0固定
      sbjnode = sbjnode0;
      inv = inv0;
    }
  }
  else {
    sbjnode = sbjgraph.new_and(sbjnode0, sbjnode1, !inv0, !inv1);
    inv = true;
  }
}

void
make_xor(SbjGraph& sbjgraph,
	 SbjNode* sbjnode0,
	 bool inv0,
	 SbjNode* sbjnode1,
	 bool inv1,
	 SbjNode*& sbjnode,
	 bool& inv)
{
  sbjnode = NULL;
  inv = inv0 ^ inv1;
  if ( sbjnode0 == NULL ) {
    sbjnode = sbjnode1;
  }
  else if ( sbjnode1 == NULL ) {
    sbjnode = sbjnode0;
  }
  else {
    sbjnode = sbjgraph.new_xor(sbjnode0, sbjnode1);
  }
}

void
enqueue(const MvNode* node0,
	list<const MvNode*>& queue,
	vector<bool>& mark)
{
  const MvInputPinList& folist = node0->output(0)->dst_pin_list();
  for (MvInputPinList::const_iterator p = folist.begin();
       p != folist.end(); ++ p) {
    const MvInputPin* dst_pin = *p;
    const MvNode* node = dst_pin->node();
    if ( node->type() == MvNode::kDff1 ||
	 node->type() == MvNode::kDff2 ||
	 node->type() == MvNode::kConst ) {
      continue;
    }
    if ( mark[node->id()] ) {
      continue;
    }
    ymuint ni = node->input_num();
    bool marked = true;
    const MvNode* unmark = NULL;
    for (ymuint i = 0; i < ni; ++ i) {
      const MvInputPin* ipin = node->input(i);
      const MvOutputPin* opin = ipin->src_pin();
      if ( opin == NULL ) {
	cerr << "node" << node->id() << "->input(" << i
	     << ") has no source" << endl;
	abort();
      }
      const MvNode* inode = opin->node();
      if ( !mark[inode->id()] ) {
	marked = false;
	unmark = inode;
	break;
      }
    }
    if ( marked ) {
      mark[node->id()] = true;
      queue.push_back(node);
    }
  }
}

END_NONAMESPACE


// @brief コンストラクタ
Mvn2Sbj::Mvn2Sbj()
{
}

// @brief デストラクタ
Mvn2Sbj::~Mvn2Sbj()
{
}

// @brief MvMgr の内容を SbjGraph に変換する．
// @param[in] mvmgr 対象の MvNetwork
// @param[out] sbjgraph 変換先のサブジェクトグラフ
// @param[out] mvnode_map 対応関係を格納するオブジェクト
void
Mvn2Sbj::operator()(const MvMgr& mvmgr,
		    SbjGraph& sbjgraph,
		    MvNodeMap& mvnode_map)
{
  sbjgraph.clear();

  list<const MvModule*> module_list;
  ymuint n = mvmgr.topmodule_list(module_list);
  if ( n != 1 ) {
    cerr << "# of topmodules is not 1" << endl;
    return;
  }

  ymuint nmax = mvmgr.max_node_id();

  const MvModule* module = module_list.front();

  sbjgraph.set_name(module->name());

  vector<bool> mark(nmax, false);
  list<const MvNode*> queue;

  // 外部入力を作る．
  ymuint ni = module->input_num();
  for (ymuint i = 0; i < ni; ++ i) {
    const MvNode* node = module->input(i);
    ymuint bw = node->output(0)->bit_width();
    for (ymuint j = 0; j < bw; ++ j) {
      SbjNode* sbjnode = sbjgraph.new_input();
      mvnode_map.put(node, j, sbjnode, false);
    }
    mark[node->id()] = true;
    enqueue(node, queue, mark);
  }

  // 外部入出力を作る．
  ymuint nio = module->inout_num();
  for (ymuint i = 0; i < nio; ++ i) {
    const MvNode* node = module->inout(i);
    ymuint bw = node->output(0)->bit_width();
    for (ymuint j = 0; j < bw; ++ j) {
      SbjNode* sbjnode = sbjgraph.new_input();
      mvnode_map.put(node, j, sbjnode, false);
    }
    mark[node->id()] = true;
    enqueue(node, queue, mark);
  }

  // DFF と定数ノードを作る．
  const list<MvNode*>& node_list = module->node_list();
  for (list<MvNode*>::const_iterator p = node_list.begin();
       p != node_list.end(); ++ p) {
    const MvNode* node = *p;
    if ( node->type() == MvNode::kDff1 || node->type() == MvNode::kDff2 ) {
      // DFF
      ymuint bw = node->output(0)->bit_width();
      for (ymuint j = 0; j < bw; ++ j) {
	SbjNode* sbjnode = sbjgraph.new_dff();
	mvnode_map.put(node, j, sbjnode, false);
      }
      mark[node->id()] = true;
      enqueue(node, queue, mark);
    }
    else if ( node->type() == MvNode::kConst ) {
      // 定数
      ymuint bw = node->output(0)->bit_width();
      vector<ymuint32> value;
      node->const_value(value);
      ymuint bpos = 0;
      ymuint32 pat = value[bpos];
      for (ymuint j = 0; j < bw; ++ j) {
	if ( pat & 1U ) {
	  // 1
	  mvnode_map.put(node, j, NULL, true);
	}
	else {
	  // 0
	  mvnode_map.put(node, j, NULL, false);
	}
	if ( j % 32 == 31 ) {
	  ++ bpos;
	  pat = value[bpos];
	}
	else {
	  pat >>= 1;
	}
      }
      mark[node->id()] = true;
      enqueue(node, queue, mark);
    }
  }

  // 論理ノードを作る．
  while ( !queue.empty() ) {
    const MvNode* node = queue.front();
    queue.pop_front();

    // node に対応する SbjNode を作る．
    switch ( node->type() ) {
    case MvNode::kOutput:
      break;

    case MvNode::kThrough:
      {
	const MvInputPin* ipin = node->input(0);
	const MvOutputPin* opin = ipin->src_pin();
	const MvNode* src_node = opin->node();
	ymuint bw = opin->bit_width();
	for (ymuint i = 0; i < bw; ++ i) {
	  SbjNode* sbjnode;
	  bool inv;
	  bool stat = mvnode_map.get(src_node, i, sbjnode, inv);
	  assert_cond( stat , __FILE__, __LINE__);
	  mvnode_map.put(node, i, sbjnode, inv);
	}
      }
      break;

    case MvNode::kNot:
      {
	const MvInputPin* ipin = node->input(0);
	const MvOutputPin* opin = ipin->src_pin();
	const MvNode* src_node = opin->node();
	ymuint bw = opin->bit_width();
	assert_cond( node->output(0)->bit_width(), __FILE__, __LINE__);
	for (ymuint i = 0; i < bw; ++ i) {
	  SbjNode* sbjnode;
	  bool inv;
	  bool stat = mvnode_map.get(src_node, i, sbjnode, inv);
	  assert_cond( stat , __FILE__, __LINE__);
	  mvnode_map.put(node, i, sbjnode, !inv);
	}
      }
      break;

    case MvNode::kAnd:
      {
	const MvInputPin* ipin0 = node->input(0);
	const MvOutputPin* src_pin0 = ipin0->src_pin();
	const MvNode* src_node0 = src_pin0->node();

	const MvInputPin* ipin1 = node->input(1);
	const MvOutputPin* src_pin1 = ipin1->src_pin();
	const MvNode* src_node1 = src_pin1->node();

	ymuint bw = node->output(0)->bit_width();
	assert_cond( src_pin0->bit_width() == bw, __FILE__, __LINE__);
	assert_cond( src_pin1->bit_width() == bw, __FILE__, __LINE__);
	for (ymuint i = 0; i < bw; ++ i) {
	  SbjNode* sbjnode0;
	  bool inv0;
	  bool stat0 = mvnode_map.get(src_node0, i, sbjnode0, inv0);
	  SbjNode* sbjnode1;
	  bool inv1;
	  bool stat1 = mvnode_map.get(src_node1, i, sbjnode1, inv1);
	  assert_cond( stat0 && stat1 , __FILE__, __LINE__);
	  SbjNode* sbjnode = NULL;
	  bool inv = false;
	  make_and(sbjgraph, sbjnode0, inv0, sbjnode1, inv1, sbjnode, inv);
	  mvnode_map.put(node, i, sbjnode, inv);
	}
      }
      break;

    case MvNode::kOr:
      {
	const MvInputPin* ipin0 = node->input(0);
	const MvOutputPin* src_pin0 = ipin0->src_pin();
	const MvNode* src_node0 = src_pin0->node();

	const MvInputPin* ipin1 = node->input(1);
	const MvOutputPin* src_pin1 = ipin1->src_pin();
	const MvNode* src_node1 = src_pin1->node();

	ymuint bw = node->output(0)->bit_width();
	assert_cond( src_pin0->bit_width() == bw, __FILE__, __LINE__);
	assert_cond( src_pin1->bit_width() == bw, __FILE__, __LINE__);
	for (ymuint i = 0; i < bw; ++ i) {
	  SbjNode* sbjnode0;
	  bool inv0;
	  bool stat0 = mvnode_map.get(src_node0, i, sbjnode0, inv0);
	  SbjNode* sbjnode1;
	  bool inv1;
	  bool stat1 = mvnode_map.get(src_node1, i, sbjnode1, inv1);
	  assert_cond( stat0 && stat1 , __FILE__, __LINE__);
	  SbjNode* sbjnode = NULL;
	  bool inv = false;
	  make_or(sbjgraph, sbjnode0, inv0, sbjnode1, inv1, sbjnode, inv);
	  mvnode_map.put(node, i, sbjnode, inv);
	}
      }
      break;

    case MvNode::kXor:
      {
	const MvInputPin* ipin0 = node->input(0);
	const MvOutputPin* src_pin0 = ipin0->src_pin();
	const MvNode* src_node0 = src_pin0->node();

	const MvInputPin* ipin1 = node->input(1);
	const MvOutputPin* src_pin1 = ipin1->src_pin();
	const MvNode* src_node1 = src_pin1->node();

	ymuint bw = node->output(0)->bit_width();
	assert_cond( src_pin0->bit_width() == bw, __FILE__, __LINE__);
	assert_cond( src_pin1->bit_width() == bw, __FILE__, __LINE__);
	for (ymuint i = 0; i < bw; ++ i) {
	  SbjNode* sbjnode0;
	  bool inv0;
	  bool stat0 = mvnode_map.get(src_node0, i, sbjnode0, inv0);
	  SbjNode* sbjnode1;
	  bool inv1;
	  bool stat1 = mvnode_map.get(src_node1, i, sbjnode1, inv1);
	  assert_cond( stat0 && stat1 , __FILE__, __LINE__);
	  SbjNode* sbjnode = NULL;
	  bool inv = false;
	  make_xor(sbjgraph, sbjnode0, inv0, sbjnode1, inv1, sbjnode, inv);
	  mvnode_map.put(node, i, sbjnode, inv);
	}
      }
      break;

    case MvNode::kRand:
    case MvNode::kRor:
    case MvNode::kRxor:
      {
	assert_not_reached(__FILE__, __LINE__);
      }
      break;

    case MvNode::kEq:
    case MvNode::kLt:
      {
	assert_not_reached(__FILE__, __LINE__);
      }
      break;

    case MvNode::kAdd:
    case MvNode::kSub:
    case MvNode::kMult:
    case MvNode::kDiv:
    case MvNode::kMod:
    case MvNode::kPow:
      {
	assert_not_reached(__FILE__, __LINE__);
      }
      break;

    case MvNode::kIte:
      {
	const MvInputPin* ipin0 = node->input(0);
	const MvOutputPin* src_pin0 = ipin0->src_pin();
	const MvNode* src_node0 = src_pin0->node();

	const MvInputPin* ipin1 = node->input(1);
	const MvOutputPin* src_pin1 = ipin1->src_pin();
	const MvNode* src_node1 = src_pin1->node();

	const MvInputPin* ipin2 = node->input(2);
	const MvOutputPin* src_pin2 = ipin2->src_pin();
	const MvNode* src_node2 = src_pin2->node();

	ymuint bw = node->output(0)->bit_width();
	assert_cond( src_pin1->bit_width() == bw, __FILE__, __LINE__);
	assert_cond( src_pin2->bit_width() == bw, __FILE__, __LINE__);
	SbjNode* sbjnode0;
	bool inv0;
	bool stat0 = mvnode_map.get(src_node0, sbjnode0, inv0);
	assert_cond( stat0, __FILE__, __LINE__);

	for (ymuint i = 0; i < bw; ++ i) {
	  SbjNode* sbjnode1;
	  bool inv1;
	  bool stat1 = mvnode_map.get(src_node1, i, sbjnode1, inv1);
	  SbjNode* sbjnode2;
	  bool inv2;
	  bool stat2 = mvnode_map.get(src_node2, i, sbjnode2, inv2);
	  assert_cond( stat1 && stat2 , __FILE__, __LINE__);
	  SbjNode* and1 = NULL;
	  bool and1_inv = false;
	  make_and(sbjgraph, sbjnode0, inv0, sbjnode1, inv1, and1, and1_inv);
	  SbjNode* and2 = NULL;
	  bool and2_inv = false;
	  make_and(sbjgraph, sbjnode0, !inv0, sbjnode2, inv2, and2, and2_inv);
	  SbjNode* or1 = NULL;
	  bool or1_inv = false;
	  make_or(sbjgraph, and1, and1_inv, and2, and2_inv, or1, or1_inv);
	  mvnode_map.put(node, i, or1, or1_inv);
	}
      }
      break;

    case MvNode::kConcat:
      {
	ymuint bw = node->output(0)->bit_width();
	ymuint ni = node->input_num();
	ymuint offset = bw;
	for (ymuint i = 0; i < ni; ++ i) {
	  const MvInputPin* ipin = node->input(i);
	  const MvOutputPin* opin = ipin->src_pin();
	  const MvNode* src_node = opin->node();
	  ymuint bw1 = opin->bit_width();
	  offset -= bw1;
	  for (ymuint j = 0; j < bw1; ++ j) {
	    ymuint index = offset + j;
	    SbjNode* sbjnode0;
	    bool inv0;
	    bool stat0 = mvnode_map.get(src_node, j, sbjnode0, inv0);
	    assert_cond( stat0, __FILE__, __LINE__);
	    mvnode_map.put(node, index, sbjnode0, inv0);
	  }
	}
	assert_cond( offset == 0U, __FILE__, __LINE__);
      }
      break;

    case MvNode::kConstBitSelect:
      {
	const MvInputPin* ipin = node->input(0);
	const MvOutputPin* src_pin = ipin->src_pin();
	const MvNode* src_node = src_pin->node();

	SbjNode* sbjnode0;
	bool inv0;
	bool stat0 = mvnode_map.get(src_node, node->bitpos(), sbjnode0, inv0);
	assert_cond( stat0 , __FILE__, __LINE__);
	mvnode_map.put(node, sbjnode0, inv0);
      }
      break;

    case MvNode::kConstPartSelect:
      {
	const MvInputPin* ipin = node->input(0);
	const MvOutputPin* src_pin = ipin->src_pin();
	const MvNode* src_node = src_pin->node();

	ymuint bw = node->output(0)->bit_width();
	ymuint msb = node->msb();
	ymuint lsb = node->lsb();
	assert_cond( bw == msb - lsb + 1,
		     __FILE__, __LINE__);
	for (ymuint i = 0; i < bw; ++ i) {
	  SbjNode* sbjnode0;
	  bool inv0;
	  bool stat0 = mvnode_map.get(src_node, i + lsb, sbjnode0, inv0);
	  assert_cond( stat0 , __FILE__, __LINE__);
	  mvnode_map.put(node, i, sbjnode0, inv0);
	}
      }
      break;

    case MvNode::kBitSelect:
      {
	assert_not_reached(__FILE__, __LINE__);
      }
      break;

    case MvNode::kPartSelect:
      {
	assert_not_reached(__FILE__, __LINE__);
      }
      break;

    case MvNode::kCombUdp:
      {
	assert_not_reached(__FILE__, __LINE__);
      }
      break;

    case MvNode::kSeqUdp:
      {
	assert_not_reached(__FILE__, __LINE__);
      }
      break;

    default:
      assert_not_reached(__FILE__, __LINE__);
      break;
    }
    enqueue(node, queue, mark);
  }

  // DFFノードの入力を接続する．
  for (ymuint i = 0; i < nmax; ++ i) {
    const MvNode* node = mvmgr.node(i);
    if ( node == NULL ) continue;
    if ( node->type() == MvNode::kDff1 || node->type() == MvNode::kDff2 ) {
      const MvInputPin* ipin = node->input(0);
      const MvOutputPin* opin = ipin->src_pin();
      assert_cond( opin != NULL, __FILE__, __LINE__);
      const MvNode* src_node = opin->node();
      ymuint bw = ipin->bit_width();
      for (ymuint j = 0; j < bw; ++ j) {
	SbjNode* sbjnode;
	bool inv;
	bool stat = mvnode_map.get(node, j, sbjnode, inv);
	SbjNode* isbjnode;
	bool iinv;
	bool stat1 = mvnode_map.get(src_node, j, isbjnode, iinv);
	assert_cond( stat && stat1 , __FILE__, __LINE__);
	assert_cond( inv == false, __FILE__, __LINE__);
	sbjgraph.set_dff_data(sbjnode, isbjnode, iinv);
      }

      const MvInputPin* ipin1 = node->input(1);
      const MvOutputPin* opin1 = ipin1->src_pin();
      assert_cond( opin1 != NULL, __FILE__, __LINE__);
      assert_cond( opin1->bit_width() == 1, __FILE__, __LINE__);
      const MvNode* src_node1 = opin1->node();
      SbjNode* isbjnode1;
      bool iinv1;
      bool stat1 = mvnode_map.get(src_node1, isbjnode1, iinv1);
      assert_cond( stat1 , __FILE__, __LINE__);
      for (ymuint j = 0; j < bw; ++ j) {
	SbjNode* sbjnode;
	bool inv;
	bool stat = mvnode_map.get(node, j, sbjnode, inv);
	assert_cond( stat, __FILE__, __LINE__);
	sbjgraph.set_dff_clock(sbjnode, isbjnode1, iinv1);
      }

      const MvInputPin* ipin2 = node->input(2);
      const MvOutputPin* opin2 = ipin2->src_pin();
      if ( opin2 ) {
	assert_cond( opin2->bit_width() == 1, __FILE__, __LINE__);
	const MvNode* src_node2 = opin2->node();
	SbjNode* isbjnode2;
	bool iinv2;
	bool stat2 = mvnode_map.get(src_node2, isbjnode2, iinv2);
	assert_cond( stat2 , __FILE__, __LINE__);
	for (ymuint j = 0; j < bw; ++ j) {
	  SbjNode* sbjnode;
	  bool inv;
	  bool stat = mvnode_map.get(node, j, sbjnode, inv);
	  assert_cond( stat, __FILE__, __LINE__);
	  sbjgraph.set_dff_rst(sbjnode, isbjnode2, iinv2);
	}
      }

      const MvInputPin* ipin3 = node->input(3);
      const MvOutputPin* opin3 = ipin3->src_pin();
      if ( opin3 ) {
	assert_cond( opin3->bit_width() == 1, __FILE__, __LINE__);
	const MvNode* src_node3 = opin3->node();
	SbjNode* isbjnode3;
	bool iinv3;
	bool stat3 = mvnode_map.get(src_node3, isbjnode3, iinv3);
	assert_cond( stat3 , __FILE__, __LINE__);
	for (ymuint j = 0; j < bw; ++ j) {
	  SbjNode* sbjnode;
	  bool inv;
	  bool stat = mvnode_map.get(node, j, sbjnode, inv);
	  assert_cond( stat, __FILE__, __LINE__);
	  sbjgraph.set_dff_set(sbjnode, isbjnode3, iinv3);
	}
      }
    }
  }

  // 外部出力ノードを作る．
  ymuint no = module->output_num();
  for (ymuint i = 0; i < no; ++ i) {
    const MvNode* node = module->output(i);
    const MvInputPin* ipin = node->input(0);
    const MvOutputPin* opin = ipin->src_pin();
    assert_cond( opin != NULL, __FILE__, __LINE__);
    const MvNode* src_node = opin->node();

    ymuint bw = ipin->bit_width();
    for (ymuint j = 0; j < bw; ++ j) {
      SbjNode* sbjnode;
      bool inv;
      bool stat = mvnode_map.get(src_node, j, sbjnode, inv);
      assert_cond( stat, __FILE__, __LINE__);
      SbjNode* osbjnode = sbjgraph.new_output(sbjnode, inv);
      mvnode_map.put(node, j, osbjnode, false);
    }
  }

  // ポートを生成する．
  ymuint np = module->port_num();
  for (ymuint i = 0; i < np; ++ i) {
    const MvPort* port = module->port(i);
    ymuint nb = port->bit_width();
    vector<SbjNode*> tmp;
    tmp.reserve(nb);
    ymuint n = port->port_ref_num();
    for (ymuint j = 0; j < n; ++ j) {
      const MvPortRef* port_ref = port->port_ref(j);
      ymuint nb1 = port_ref->bit_width();
      const MvNode* node = port_ref->node();
      if ( port_ref->is_simple() ) {
	for (ymuint k = 0; k < nb1; ++ k) {
	  SbjNode* sbjnode;
	  bool inv;
	  bool stat = mvnode_map.get(node, k, sbjnode, inv);
	  assert_cond( stat , __FILE__, __LINE__);
	  assert_cond( inv == false, __FILE__, __LINE__);
	  tmp.push_back(sbjnode);
	}
      }
      else if ( port_ref->has_bitselect() ) {
	SbjNode* sbjnode;
	bool inv;
	bool stat = mvnode_map.get(node, port_ref->bitpos(), sbjnode, inv);
	assert_cond( stat , __FILE__, __LINE__);
	assert_cond( inv == false, __FILE__, __LINE__);
	tmp.push_back(sbjnode);
      }
      else if ( port_ref->has_partselect() ) {
	ymuint msb = port_ref->msb();
	ymuint lsb = port_ref->lsb();
	for (ymuint k = lsb; k <= msb; ++ k) {
	  SbjNode* sbjnode;
	  bool inv;
	  bool stat = mvnode_map.get(node, k, sbjnode, inv);
	  assert_cond( stat , __FILE__, __LINE__);
	  assert_cond( inv == false, __FILE__, __LINE__);
	  tmp.push_back(sbjnode);
	}
      }
      else {
	assert_not_reached(__FILE__, __LINE__);
      }
    }
    sbjgraph.add_port(port->name(), tmp);
  }
}

END_NAMESPACE_YM_MVN
