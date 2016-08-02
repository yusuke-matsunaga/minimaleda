
/// @file libym_bdn/bnet2bdn.cc
/// @brief bnet2bdn() の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: bnet2bdn.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_bnet/BNetwork.h"
#include "ym_bdn/BdNetwork.h"
#include "ym_bdn/BdnNode.h"
#include "ym_bdn/BdnNodeHandle.h"


BEGIN_NAMESPACE_YM_BDN

// BNetwork を BdnGraph に変換する．
// network はすでに 2-feasible ネットワークに分解されているとする．
// 2-feasible でない場合には false を返す．
bool
bnet2bdn(const BNetwork& network,
	 BdNetwork& bdn,
	 ostream& err_out)
{
  bdn.set_name(network.model_name());
  
  ymuint n = network.max_node_id();
  
  // BNetwork 中のノードと BinGraph 中のノードの対応を持つ配列
  vector<BdnNodeHandle> assoc(n);

  bdn.clear();

  // 外部入力を作る．
  for (BNodeListConstIter p = network.inputs_begin();
       p != network.inputs_end(); ++p) {
    BNode* bnode = *p;
    BdnNode* node = bdn.new_input(bnode->name());
    assoc[bnode->id()] = BdnNodeHandle(node, false);
  }

  // 定数0に縮退している外部出力を求めておく
  vector<bool> zero_outs(n, false);
  for (BNodeList::const_iterator p = network.outputs_begin();
       p != network.outputs_end(); ++ p) {
    // 外部出力
    BNode* obnode = *p;
    // 外部出力から参照されているノード
    BNode* ibnode = obnode->fanin(0);
    if ( ibnode->ni() == 0 ) {
      zero_outs[ibnode->id()] = true;
    }
  }

  // 内部ノードを作る．
  // まず入力からのトポロジカル順にソートし bnode_list に入れる．
  ymuint nv = network.logic_node_num();
  BNodeVector node_list;
  network.tsort(node_list);
  vector<BdnNode*> inode;
  for (ymuint i = 0; i < nv; ++ i) {
    BNode* bnode = node_list[i];
    ymuint ni = bnode->ni();
    if ( ni == 0 ) {
      if ( zero_outs[bnode->id()] == false ) {
	err_out << "ERROR: number of fanins is 0" << endl;
	err_out << bnode->name() << endl;
	return false;
      }
      bool inv = bnode->is_one();
      assoc[bnode->id()] = BdnNodeHandle(NULL, inv);
    }
    else if ( ni == 1 ) {
      BdnNodeHandle inode_handle = assoc[bnode->fanin(0)->id()];
      assert_cond( inode_handle.node() != NULL, __FILE__, __LINE__);
      if ( bnode->is_buffer() ) {
	assoc[bnode->id()] = inode_handle;
      }
      else if ( bnode->is_inverter() ) {
	assoc[bnode->id()] = ~inode_handle;
      }
      else {
	err_out << "ERROR: " << bnode->name()
	     << " should be a buffer or an inverter" << endl;
	return false;
      }
    }
    else if ( ni == 2 ) {
      LogExpr expr = bnode->func();
      assert_cond(expr.child(0).is_literal(), __FILE__, __LINE__);
      assert_cond(expr.child(1).is_literal(), __FILE__, __LINE__);

      BdnNodeHandle inode0_handle = assoc[bnode->fanin(0)->id()];
      BdnNodeHandle inode1_handle = assoc[bnode->fanin(1)->id()];

      if ( expr.child(0).is_negaliteral() ) {
	if ( expr.child(0).varid() == 0 ) {
	  inode0_handle = ~inode0_handle;
	}
	else {
	  inode1_handle = ~inode1_handle;
	}
      }

      if ( expr.child(1).is_negaliteral() ) {
	if ( expr.child(1).varid() == 0 ) {
	  inode0_handle = ~inode0_handle;
	}
	else {
	  inode1_handle = ~inode1_handle;
	}
      }
      
      ymuint fcode = 0;
      if ( expr.is_and() ) {
	fcode = 0x8; // 1000
      }
      else if ( expr.is_or() ) {
	fcode = 0xe; // 1110
      }
      else if ( expr.is_xor() ) {
	fcode = 0x6; // 0110
      }
      else {
	assert_not_reached(__FILE__, __LINE__);
      }
      BdnNodeHandle h = bdn.new_logic(fcode, inode0_handle, inode1_handle);
      assoc[bnode->id()] = h;
    }
    else { // ni > 2
      err_out << "ERROR: number of fanins exceeds 2" << endl;
      err_out << bnode->name() << " : " << bnode->ni() << endl;
      return false;
    }
  }

  // 外部出力ノードを作る．
  for (BNodeList::const_iterator p = network.outputs_begin();
       p != network.outputs_end(); ++ p) {
    BNode* obnode = *p;
    BNode* ibnode = obnode->fanin(0);
    BdnNodeHandle inode_handle = assoc[ibnode->id()];
    (void) bdn.new_output(obnode->name(), inode_handle);
  }

  return true;
}

END_NAMESPACE_YM_BDN
