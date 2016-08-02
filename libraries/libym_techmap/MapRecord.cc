
/// @file libym_techmap/MapRecord.cc
/// @brief MapRecord の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: MapRecord.cc 2274 2009-06-10 07:45:29Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "MapRecord.h"
#include "ym_sbj/SbjGraph.h"
#include "ym_techmap/CnGraph.h"
#include "Match.h"


BEGIN_NAMESPACE_YM_TECHMAP

// コンストラクタ
MapRecord::MapRecord()
{
}

// デストラクタ
MapRecord::~MapRecord()
{
}

// @brief @brief 作業領域を初期化する．
// @param[in] sbjgraph サブジェクトグラフ
void
MapRecord::init(const SbjGraph& sbjgraph)
{
  mNodeInfo.clear();
  mNodeInfo.resize(sbjgraph.max_node_id() * 2);
}

// @brief マッチの情報だけコピーする．
void
MapRecord::copy(const MapRecord& src)
{
  ymuint n = src.mNodeInfo.size();
  mNodeInfo.clear();
  mNodeInfo.resize(n);
  for (ymuint i = 0; i < n; ++ i) {
    mNodeInfo[i].mMatch = src.mNodeInfo[i].mMatch;
  }
}

// @brief マッチを記録する．
// @param[in] node 該当のノード
// @param[in] inv 極性
// @param[in] match 対応するマッチ
// @param[in] cell セル
void
MapRecord::set_match(const SbjNode* node,
		     bool inv,
		     const Match& match,
		     const Cell* cell)
{
  NodeInfo& ni = node_info(node, inv);
  ni.mMatch = match;
  ni.mCell = cell;
}

// @brief インバータのマッチを記録する．
// @param[in] node 該当のノード
// @param[in] inv 極性
// @param[in] cell セル
void
MapRecord::set_inv_match(const SbjNode* node,
			 bool inv,
			 const Cell* cell)
{
  NodeInfo& ni = node_info(node, inv);
  ni.mMatch.resize(1);
  ni.mMatch.set_leaf(0, node, !inv);
  ni.mCell = cell;
}

// @brief マッチを取り出す．
// @param[in] node 該当のノード
// @param[in] inv 極性
const Match&
MapRecord::get_match(const SbjNode* node,
		     bool inv)
{
  return node_info(node, inv).mMatch;
}

// @brief マッピング結果を CnGraph にセットする．
// @param[in] sbjgraph サブジェクトグラフ
// @param[in] const0_cell 定数0のセル
// @param[in] const1_cell 定数1のセル
// @param[out] mapgraph マッピング結果を格納するネットワーク
void
MapRecord::gen_mapgraph(const SbjGraph& sbjgraph,
			const Cell* const0_cell,
			const Cell* const1_cell,
			CnGraph& mapgraph)
{
  mapgraph.clear();

  // 外部入力の生成
  const SbjNodeList& input_list = sbjgraph.input_list();
  for (SbjNodeList::const_iterator p = input_list.begin();
       p != input_list.end(); ++ p) {
    const SbjNode* node = *p;
    CnNode* mapnode = mapgraph.new_input();
    NodeInfo& node_info = this->node_info(node, false);
    node_info.mMapNode = mapnode;
  }

  // DFFの生成
  const SbjNodeList& dff_list = sbjgraph.dff_list();
  for (SbjNodeList::const_iterator p = dff_list.begin();
       p != dff_list.end(); ++ p) {
    const SbjNode* node = *p;
    CnNode* mapnode = mapgraph.new_dff();
    NodeInfo& node_info = this->node_info(node, false);
    node_info.mMapNode = mapnode;
  }

  // 外部出力からバックトレースを行い全ノードの生成を行う．
  const SbjNodeList& output_list = sbjgraph.output_list();
  for (SbjNodeList::const_iterator p = output_list.begin();
       p != output_list.end(); ++ p) {
    const SbjNode* onode = *p;
    const SbjNode* node = onode->fanin(0);
    bool inv = onode->output_inv();
    CnNode* mapnode = NULL;
    if ( node ) {
      mapnode = back_trace(node, inv, mapgraph);
    }
    else {
      if ( inv ) {
	// 定数1ノードを作る．
	mapnode = mapgraph.new_cellnode(vector<CnNode*>(0), const1_cell);
      }
      else {
	// 定数0ノードを作る．
	mapnode = mapgraph.new_cellnode(vector<CnNode*>(0), const0_cell);
      }
    }
    CnNode* omapnode = mapgraph.new_output(mapnode);
    node_info(onode, false).mMapNode = omapnode;
  }

  // DFFからバックトレースを行い全ノードの生成を行う．
  for (SbjNodeList::const_iterator p = dff_list.begin();
       p != dff_list.end(); ++ p) {
    const SbjNode* onode = *p;
    const SbjNode* node = onode->fanin_data();
    bool inv = onode->fanin_data_inv();
    CnNode* mapnode = NULL;
    if ( node ) {
      mapnode = back_trace(node, inv, mapgraph);
    }
    else {
      if ( inv ) {
	// 定数1ノードを作る．
	mapnode = mapgraph.new_cellnode(vector<CnNode*>(0), const1_cell);
      }
      else {
	// 定数0ノードを作る．
	mapnode = mapgraph.new_cellnode(vector<CnNode*>(0), const0_cell);
      }
    }
    CnNode* omapnode = node_info(onode, false).mMapNode;
    mapgraph.set_dff_input(omapnode, mapnode);
    {
      const SbjNode* node = onode->fanin_clock();
      bool inv = onode->fanin_clock_inv();
      CnNode* mapnode = NULL;
      if ( node ) {
	mapnode = back_trace(node, inv, mapgraph);
	CnNode* omapnode = node_info(onode, false).mMapNode;
	mapgraph.set_dff_clock(omapnode, mapnode);
      }
    }
    {
      const SbjNode* node = onode->fanin_set();
      bool inv = onode->fanin_set_inv();
      CnNode* mapnode = NULL;
      if ( node ) {
	mapnode = back_trace(node, inv, mapgraph);
	CnNode* omapnode = node_info(onode, false).mMapNode;
	mapgraph.set_dff_set(omapnode, mapnode);
      }
    }
    {
      const SbjNode* node = onode->fanin_rst();
      bool inv = onode->fanin_rst_inv();
      CnNode* mapnode = NULL;
      if ( node ) {
	mapnode = back_trace(node, inv, mapgraph);
	CnNode* omapnode = node_info(onode, false).mMapNode;
	mapgraph.set_dff_rst(omapnode, mapnode);
      }
    }
  }

  // ポートを生成する．
  ymuint np = sbjgraph.port_num();
  for (ymuint i = 0; i < np; ++ i) {
    const SbjPort* sbjport = sbjgraph.port(i);
    ymuint nb = sbjport->bit_width();
    vector<CnNode*> tmp(nb);
    for (ymuint j = 0; j < nb; ++ j) {
      const SbjNode* sbjnode = sbjport->bit(j);
      CnNode* node = node_info(sbjnode, false).mMapNode;
      assert_cond( node != NULL, __FILE__, __LINE__);
      tmp[j] = node;
    }
    mapgraph.add_port(sbjport->name(), tmp);
  }
}

// サブジェクトグラフの node に対応するマップされたノードを
// 生成し，それを返す．
CnNode*
MapRecord::back_trace(const SbjNode* node,
		      bool inv,
		      CnGraph& mapnetwork)
{
  NodeInfo& node_info = this->node_info(node, inv);
  CnNode* mapnode = node_info.mMapNode;
  if ( mapnode ) {
    // すでに生成済みならそのノードを返す．
    return mapnode;
  }

  // node を根とするマッチを取り出す．
  const Match& match = node_info.mMatch;

  // その入力に対応するノードを再帰的に生成する．
  ymuint ni = match.leaf_num();
  for (ymuint i = 0; i < ni; ++ i) {
    const SbjNode* inode = match.leaf_node(i);
    bool iinv = match.leaf_inv(i);
    back_trace(inode, iinv, mapnetwork);
  }

  mTmpFanins.clear();
  mTmpFanins.resize(ni);
  for (ymuint i = 0; i < ni; ++ i) {
    const SbjNode* inode = match.leaf_node(i);
    bool iinv = match.leaf_inv(i);
    NodeInfo& inode_info = this->node_info(inode, iinv);
    CnNode* imapnode = inode_info.mMapNode;
    mTmpFanins[i] = imapnode;
  }

  // 新しいノードを作り mNodeMap に登録する．
  assert_cond( node_info.mCell != NULL, __FILE__, __LINE__);
  mapnode = mapnetwork.new_cellnode(mTmpFanins, node_info.mCell);
  node_info.mMapNode = mapnode;

  return mapnode;
}

// @brief NodeInfo を取り出す．
MapRecord::NodeInfo&
MapRecord::node_info(const SbjNode* node,
		     bool inv)
{
  return mNodeInfo[node->id() * 2 + static_cast<ymuint>(inv)];
}

END_NAMESPACE_YM_TECHMAP
