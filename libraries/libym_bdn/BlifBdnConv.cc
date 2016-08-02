
/// @file libym_bdn/BlifBdnConv.cc
/// @brief BlifBdnConv の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: BlifBdnConv.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_bdn/BlifBdnConv.h"
#include "ym_bdn/BdNetwork.h"
#include "ym_bdn/BdnNode.h"
#include "ym_bdn/BdnNodeHandle.h"
#include "ym_blif/BlifNetwork.h"


BEGIN_NAMESPACE_YM_BDN

// @brief コンストラクタ
BlifBdnConv::BlifBdnConv()
{
}

// @brief デストラクタ
BlifBdnConv::~BlifBdnConv()
{
}

// @brief 変換する
// @param[in] blif_network 変換元のネットワーク
// @param[in] network 変換先のネットワーク
bool
BlifBdnConv::operator()(const BlifNetwork& blif_network,
			BdNetwork& network)
{
  mNetwork = &network;
  ymuint32 n = blif_network.max_node_id();
  mNodeMap.resize(n);
  mNodeFlag.resize(n, false);
  
  // モデル名の設定
  mNetwork->set_name(blif_network.name());

  // 外部入力ノードの生成
  ymuint32 npi = blif_network.npi();
  for (ymuint32 i = 0; i < npi; ++ i) {
    const BlifNode* blif_node = blif_network.pi(i);
    BdnNode* node = mNetwork->new_input(blif_node->name());
    put_node(blif_node, BdnNodeHandle(node, false));
  }

  // ラッチの出力(擬似入力)ノードの生成
  ymuint32 nff = blif_network.nff();
  for (ymuint32 i = 0; i < nff; ++ i) {
    const BlifNode* blif_node = blif_network.ff(i);
    int reset_val = 2;
    if ( blif_node->opat() == '0' ) {
      reset_val = 0;
    }
    else if ( blif_node->opat() == '1' ) {
      reset_val = 1;
    }
    BdnNode* node = mNetwork->new_latch(reset_val);
    put_node(blif_node, BdnNodeHandle(node, false));
  }

  // 外部出力に用いられているノードを再帰的に生成
  ymuint32 npo = blif_network.npo();
  for (ymuint32 i = 0; i < npo; ++ i) {
    const BlifNode* blif_node = blif_network.po(i);
    BdnNodeHandle node_h = make_node(blif_node);
    (void) mNetwork->new_output(blif_node->name(), node_h);
  }

  // ラッチに用いられているノードを再帰的に生成
  for (ymuint32 i = 0; i < nff; ++ i) {
    const BlifNode* blif_node = blif_network.ff(i);
    BdnNodeHandle node_h;
    bool stat = get_node(blif_node, node_h);
    assert_cond(stat, __FILE__, __LINE__);
    assert_cond(node_h.inv() == false, __FILE__, __LINE__);
    BdnNode* latch = node_h.node();
    
    BdnNodeHandle inode_h = make_node(blif_node->fanin(0));
    mNetwork->change_latch(latch, inode_h);
  }

  return true;
}

// blif_node に対応するノードを作る．
BdnNodeHandle
BlifBdnConv::make_node(const BlifNode* blif_node)
{
  BdnNodeHandle node_handle;
  if ( !get_node(blif_node, node_handle) ) {
    assert_cond( blif_node->type() == BlifNode::kLogic, __FILE__, __LINE__);
    ymuint32 ni = blif_node->ni();
    vector<BdnNodeHandle> fanins(ni);
    for (ymuint32 i = 0; i < ni; ++ i) {
      fanins[i] = make_node(blif_node->fanin(i));
    }
    
    ymuint32 nc = blif_node->nc();
    if ( blif_node->opat() == '1' ) {
      vector<BdnNodeHandle> or_leaves;
      or_leaves.reserve(nc);
      for (ymuint32 c = 0; c < nc; ++ c) {
	vector<BdnNodeHandle> and_leaves;
	and_leaves.reserve(ni);
	for (ymuint32 i = 0; i < ni; ++ i) {
	  char v = blif_node->cube_pat(c, i);
	  if ( v == '0' ) {
	    and_leaves.push_back(~fanins[i]);
	  }
	  else if ( v == '1' ) {
	    and_leaves.push_back(fanins[i]);
	  }
	}
	ymuint32 n = and_leaves.size();
	assert_cond( n > 0, __FILE__, __LINE__);
	or_leaves.push_back(bidecomp(0x8, 0, n, and_leaves));
      }
      ymuint32 n = or_leaves.size();
      if ( n == 0 ) {
	node_handle = BdnNodeHandle::make_one();
      }
      else {
	node_handle = bidecomp(0xE, 0, n, or_leaves);
      }
    }
    else {
      vector<BdnNodeHandle> and_leaves;
      and_leaves.reserve(nc);
      for (ymuint32 c = 0; c < nc; ++ c) {
	vector<BdnNodeHandle> or_leaves;
	or_leaves.reserve(ni);
	for (ymuint32 i = 0; i < ni; ++ i) {
	  char v = blif_node->cube_pat(c, i);
	  if ( v == '0' ) {
	    or_leaves.push_back(fanins[i]);
	  }
	  else if ( v == '1' ) {
	    or_leaves.push_back(~fanins[i]);
	  }
	}
	ymuint32 n = or_leaves.size();
	assert_cond( n > 0, __FILE__, __LINE__);
	and_leaves.push_back(bidecomp(0xE, 0, n, or_leaves));
      }
      ymuint32 n = and_leaves.size();
      if ( n == 0 ) {
	node_handle = BdnNodeHandle::make_zero();
      }
      else {
	node_handle = bidecomp(0x8, 0, n, and_leaves);
      }
    }
    put_node(blif_node, node_handle);
  }
  return node_handle;
}
  
// @brief 2分木を生成する．
BdnNodeHandle
BlifBdnConv::bidecomp(ymuint32 fcode,
		      ymuint32 start,
		      ymuint32 size,
		      const vector<BdnNodeHandle>& child_array)
{
  if ( size == 1 ) {
    return child_array[start];
  }
  ymuint32 nl = size / 2;
  BdnNodeHandle l = bidecomp(fcode, start, nl, child_array);
  ymuint32 nr = size - nl;
  BdnNodeHandle r = bidecomp(fcode, start + nl, nr, child_array);
  return mNetwork->new_logic(fcode, l, r);
}

// @brief blif_node に対応した BdnNode を取り出す．
bool
BlifBdnConv::get_node(const BlifNode* blif_node,
		      BdnNodeHandle& node_handle)
{
  ymuint id = blif_node->id();
  if ( mNodeFlag[id] ) {
    node_handle = mNodeMap[id];
    return true;
  }
  return false;
}

// @brief blif_node に対応した BdnNode を登録する．
void
BlifBdnConv::put_node(const BlifNode* blif_node,
		      BdnNodeHandle node_handle)
{
  ymuint id = blif_node->id();
  mNodeMap[id] = node_handle;
  mNodeFlag[id] = true;
}

END_NAMESPACE_YM_BDN
