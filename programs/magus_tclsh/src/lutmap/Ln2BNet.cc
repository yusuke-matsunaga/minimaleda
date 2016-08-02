
/// @file libym_lutmap/Ln2BNet.cc
/// @brief Ln2BNet の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: lut2bnet.cc 2274 2009-06-10 07:45:29Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "Ln2BNet.h"
#include "ym_lutmap/LnGraph.h"
#include "ym_bnet/BNetwork.h"
#include "ym_bnet/BNetManip.h"


BEGIN_NAMESPACE_YM

// @brief LnGraph を BNetwork に変換する．
// @param[in] src_network 変換元のネットワーク
// @param[out] dst_network 変換先のネットワーク
void
Ln2BNet::operator()(const LnGraph& src_network,
		    BNetwork& dst_network)
{
  ymuint n = src_network.max_node_id();
  vector<BNode*> node_assoc(n);

  dst_network.clear();

  BNetManip manip(&dst_network);

  // 外部入力を作る．
  const LnNodeList& input_list = src_network.input_list();
  for (LnNodeList::const_iterator p = input_list.begin();
       p != input_list.end(); ++ p) {
    LnNode* src_node = *p;
    BNode* dst_node = manip.new_input(string());
    node_assoc[src_node->id()] = dst_node;
  }

  // DFF を作る．
  const LnNodeList& dff_list = src_network.dff_list();
  for (LnNodeList::const_iterator p = dff_list.begin();
       p != dff_list.end(); ++ p) {
    LnNode* src_node = *p;
    BNode* dst_node = manip.new_latch();
    node_assoc[src_node->id()] = dst_node;
  }

  // 内部ノードを作る．
  vector<LnNode*> node_list;
  src_network.sort(node_list);
  for (vector<LnNode*>::const_iterator p = node_list.begin();
       p != node_list.end(); ++ p) {
    LnNode* src_node = *p;
    ymuint ni = src_node->fanin_num();
    vector<int> tv;
    src_node->tv(tv);
    BNode* dst_node = NULL;
    if ( ni == 0 ) {
      dst_node = manip.make_const(tv[0]);
    }
    else {
      vector<BNode*> dst_inodes(ni);
      for (ymuint i = 0; i < ni; ++ i) {
	LnNode* src_inode = src_node->fanin(i);
	BNode* dst_inode = node_assoc[src_inode->id()];
	assert_cond(dst_inode, __FILE__, __LINE__);
	dst_inodes[i] = dst_inode;
      }
      LogExpr expr = LogExpr::make_zero();
      ymuint np = 1U << ni;
      for (ymuint p = 0; p < np; ++ p) {
	if ( tv[p] ) {
	  LogExpr prd = LogExpr::make_one();
	  for (ymuint i = 0; i < ni; ++ i) {
	    if ( p & (1U << i) ) {
	      prd &= LogExpr::make_posiliteral(i);
	    }
	    else {
	      prd &= LogExpr::make_negaliteral(i);
	    }
	  }
	  expr |= prd;
	}
      }
      dst_node = manip.new_logic();
      bool stat = manip.change_logic(dst_node, expr, dst_inodes, false);
      assert_cond(stat, __FILE__, __LINE__);
    }
    node_assoc[src_node->id()] = dst_node;
  }

  // 外部出力ノードを作る．
  const LnNodeList& output_list = src_network.output_list();
  for (LnNodeList::const_iterator p = output_list.begin();
       p != output_list.end(); ++ p) {
    LnNode* onode = *p;
    LnNode* inode = onode->fanin(0);
    BNode* dst_inode = node_assoc[inode->id()];
    assert_cond(dst_inode, __FILE__, __LINE__);
    BNode* dst_onode = manip.new_output(string());
    bool stat = manip.change_output(dst_onode, dst_inode);
    assert_cond(stat, __FILE__, __LINE__);
  }

  // DFF ノードの入力を接続を行う．
  for (LnNodeList::const_iterator p = dff_list.begin();
       p != dff_list.end(); ++ p) {
    LnNode* onode = *p;
    LnNode* inode = onode->fanin(0);
    BNode* dst_inode = node_assoc[inode->id()];
    assert_cond(dst_inode, __FILE__, __LINE__);
    BNode* dst_onode = node_assoc[onode->id()];
    bool stat = manip.change_latch(dst_onode, dst_inode, 2);
    assert_cond(stat, __FILE__, __LINE__);
  }
}

END_NAMESPACE_YM
