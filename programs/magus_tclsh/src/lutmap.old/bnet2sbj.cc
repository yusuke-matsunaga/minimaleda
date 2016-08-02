
/// @file magus/lutmap/bnet2sbj.cc
/// @brief bnet2sbj() の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: bnet2sbj.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2009 Yusuke Matsunaga
/// All rights reserved.


#if HAVE_CONFIG_H
#include "magus_config.h"
#endif

#include <ym_bnet/BNetwork.h>
#include "SbjGraph.h"


BEGIN_NAMESPACE_MAGUS_LUTMAP

//////////////////////////////////////////////////////////////////////
// BNode へのポインタをキーにして SbjNode と極性を保持する連想配列．
//////////////////////////////////////////////////////////////////////
class BNodeMap
{
public:

  // コンストラクタ
  BNodeMap(ymuint n);
  
  // 登録する．
  void
  put(BNode* bnode,
      SbjNode* sbjnode,
      bool inv);

  // 探す．
  bool
  get(BNode* bnode,
      SbjNode*& sbjnode,
      bool& inv) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ただの配列
  vector<ympuint> mArray;

};

// コンストラクタ
BNodeMap::BNodeMap(ymuint n) :
  mArray(n, 0UL)
{
}

// 登録する．
void
BNodeMap::put(BNode* bnode,
	      SbjNode* sbjnode,
	      bool inv)
{
  mArray[bnode->id()] = reinterpret_cast<ympuint>(sbjnode) | (inv & 1);
}

// 探す．
bool
BNodeMap::get(BNode* bnode,
	      SbjNode*& sbjnode,
	      bool& inv) const
{
  ympuint tmp = mArray[bnode->id()];
  sbjnode = reinterpret_cast<SbjNode*>(tmp & ~1UL);
  inv = static_cast<bool>(tmp & 1);
  return sbjnode != NULL;
}


// BNetwork を SbjGraph に変換する．
// network はすでに 2-feasible ネットワークに分解されているとする．
// 2-feasible でない場合には false を返す．
bool
bnet2sbj(const BNetwork& network,
	 SbjGraph& sbjgraph,
	 ostream& err_out)
{
  ymuint n = network.max_node_id();
  // BNetwork 中のノードと BinGraph 中のノードの対応を持つ連想配列
  BNodeMap assoc(n);

  sbjgraph.clear();

  // 外部入力を作る．
  for (BNodeList::const_iterator p = network.inputs_begin();
       p != network.inputs_end(); ++p) {
    BNode* bnode = *p;
    SbjNode* node = sbjgraph.new_input(bnode->name());
    assoc.put(bnode, node, false);
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
  vector<SbjNode*> inode;
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
      assoc.put(bnode, NULL, inv);
    }
    else if ( ni == 1 ) {
      SbjNode* inode;
      bool inv;
      bool stat = assoc.get(bnode->fanin(0), inode, inv);
      assert_cond(stat, __FILE__, __LINE__);
      if ( bnode->is_buffer() ) {
	assoc.put(bnode, inode, inv);
      }
      else if ( bnode->is_inverter() ) {
	assoc.put(bnode, inode, !inv);
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
      
      SbjNode* inode0;
      SbjNode* inode1;
      bool inv0;
      bool inv1;

      bool stat0 = assoc.get(bnode->fanin(0), inode0, inv0);
      assert_cond(stat0, __FILE__, __LINE__);
      bool stat1 = assoc.get(bnode->fanin(1), inode1, inv1);
      assert_cond(stat1, __FILE__, __LINE__);

      if ( expr.child(0).is_negaliteral() ) {
	if ( expr.child(0).varid() == 0 ) {
	  inv0 = !inv0;
	}
	else {
	  inv1 = !inv1;
	}
      }

      if ( expr.child(1).is_negaliteral() ) {
	if ( expr.child(1).varid() == 0 ) {
	  inv0 = !inv0;
	}
	else {
	  inv1 = !inv1;
	}
      }

      ymuint fcode = 0;
      if ( expr.is_and() ) {
	if ( inv0 ) {
	  if ( inv1 ) {
	    fcode = 0x1; // 0001
	  }
	  else {
	    fcode = 0x4; // 0100
	  }
	}
	else {
	  if ( inv1 ) {
	    fcode = 0x2; // 0010
	  }
	  else {
	    fcode = 0x8; // 1000
	  }
	}
      }
      else if ( expr.is_or() ) {
	if ( inv0 ) {
	  if ( inv1 ) {
	    fcode = 0x7; // 0111
	  }
	  else {
	    fcode = 0xd; // 1101
	  }
	}
	else {
	  if ( inv1 ) {
	    fcode = 0xb; // 1011
	  }
	  else {
	    fcode = 0xe; // 1110
	  }
	}
      }
      else if ( expr.is_xor() ) {
	if ( inv0 ^ inv1 ) {
	  fcode = 0x9; // 1001
	}
	else {
	  fcode = 0x6; // 0110
	}
      }
      else {
	assert_not_reached(__FILE__, __LINE__);
      }
      SbjNode* node = sbjgraph.new_logic(bnode->name(), fcode,
					 inode0, inode1);
      assoc.put(bnode, node, false);
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
    SbjNode* inode = NULL;
    bool inv;
    assoc.get(ibnode, inode, inv);
    (void) sbjgraph.new_output(obnode->name(), inode, inv);
  }

  sbjgraph.set_efo();
  sbjgraph.find_imm_dom();

  return true;
}

END_NAMESPACE_MAGUS_LUTMAP
