
/// @file magus/lutmap/MapRecord.cc
/// @brief MapRecord の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: MapRecord.cc 2274 2009-06-10 07:45:29Z matsunaga $
///
/// Copyright (C) 2005-2007 Yusuke Matsunaga
/// All rights reserved.


#if HAVE_CONFIG_H
#include "magus_config.h"
#endif

#include "MapRecord.h"
#include "SbjGraph.h"
#include "Cut.h"
#include "LutNetwork.h"


BEGIN_NAMESPACE_MAGUS_LUTMAP

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
  mNodeInfo.resize(sbjgraph.max_node_id());
}

// @brief カットの情報だけコピーする．
void
MapRecord::copy(const MapRecord& src)
{
  ymuint n = src.mNodeInfo.size();
  mNodeInfo.clear();
  mNodeInfo.resize(n);
  for (ymuint i = 0; i < n; ++ i) {
    mNodeInfo[i].mCut = src.mNodeInfo[i].mCut;
  }
}

// @blif カットを記録する．
// @param[in] node 該当のノード
// @param[in] cut 対応するカット
void
MapRecord::set_cut(SbjNode* node,
		   const Cut* cut)
{
  mNodeInfo[node->id()].mCut = cut;
}

// @brief カットを取り出す．
// @param[in] node 該当のノード
const Cut*
MapRecord::get_cut(SbjNode* node)
{
  return mNodeInfo[node->id()].mCut;
}

// @brief マッピング結果を LnGraph にセットする．
void
MapRecord::gen_mapgraph(const SbjGraph& sbjgraph,
			LnGraph& mapgraph,
			int& lut_num,
			int& depth)
{
  mapgraph.clear();
  
  // 外部入力の生成
  const SbjNodeList& input_list = sbjgraph.input_list();
  for (SbjNodeList::const_iterator p = input_list.begin();
       p != input_list.end(); ++ p) {
    SbjNode* node = *p;
    LnNode* mapnode = mapgraph.new_input(node->name());
    NodeInfo& node_info = mNodeInfo[node->id()];
    node_info.mMapNode[0] = mapnode;
    node_info.mDepth = 0;
  }

  // 外部出力からバックトレースを行い全ノードの生成を行う．
  const SbjNodeList& output_list = sbjgraph.output_list();
  int max_depth = 0;
  for (SbjNodeList::const_iterator p = output_list.begin();
       p != output_list.end(); ++ p) {
    SbjNode* onode = *p;
    SbjNode* node = onode->fanin(0);
    bool inv = onode->output_inv();
    const string& name = onode->name();
    LnNode* mapnode = NULL;
    if ( node ) {
      mapnode = back_trace(node, inv, mapgraph, name);
      int depth1 = mNodeInfo[node->id()].mDepth;
      if ( max_depth < depth1 ) {
	max_depth = depth1;
      }
    }
    else {
      LogExpr expr;
      vector<int> tv(1);
      if ( inv ) {
	expr = LogExpr::make_one();
	tv[0] = 1;
      }
      else {
	expr = LogExpr::make_zero();
	tv[0] = 0;
      }
      mapnode = mapgraph.new_lut(name, vector<LnNode*>(0), expr, tv);
    }
    mapgraph.new_output(name, mapnode);
  }
  lut_num = mapgraph.n_lnodes();
  depth = max_depth;
}

BEGIN_NONAMESPACE

// 論理式から真理値表を作る．
void
make_tv(ymuint ni,
	const LogExpr& expr,
	vector<int>& tv)
{
  ymuint np = 1 << ni;
  
  vector<ymulong> vals(ni);
  for (ymuint i = 0; i < ni; ++ i) {
    vals[i] = 0UL;
  }

  tv.resize(np);
  ymulong s = 1UL;
  ymuint p0 = 0;
  for (ymuint p = 0; p < np; ++ p) {
    for (ymuint i = 0; i < ni; ++ i) {
      if ( p & (1U << i) ) {
	vals[i] |= s;
      }
    }
    s <<= 1;
    if ( s == 0UL ) {
      ymulong tmp = expr.eval(vals);
      for (ymuint p1 = p0; p1 <= p; ++ p1) {
	if ( tmp & (1UL << (p1 - p0)) ) {
	  tv[p1] = 1;
	}
	else {
	  tv[p1] = 0;
	}
      }
      s = 1UL;
      p0 = p + 1;
      for (ymuint i = 0; i < ni; ++ i) {
	vals[i] = 0UL;
      }
    }
  }
  if ( s != 1UL ) {
    ymulong tmp = expr.eval(vals);
    for (ymuint p1 = p0; p1 < np; ++ p1) {
      if ( tmp & (1UL << (p1 - p0)) ) {
	tv[p1] = 1;
      }
      else {
	tv[p1] = 0;
      }
    }
  }
}

END_NONAMESPACE

// サブジェクトグラフの node に対応するマップされたノードを
// 生成し，それを返す．
LnNode*
MapRecord::back_trace(SbjNode* node,
		      bool inv,
		      LnGraph& mapnetwork,
		      const string& oname)
{
  NodeInfo& node_info = mNodeInfo[node->id()];
  ymuint idx = (inv) ? 1 : 0;
  LnNode* mapnode = node_info.mMapNode[idx];
  if ( mapnode ) {
    // すでに生成済みならそのノードを返す．
    return mapnode;
  }

  if ( node->is_input() ) {
    // ということは inv = true のはず．
    assert_cond(inv, __FILE__, __LINE__);
    // NOT ゲートを表す LUT を作る．
    vector<LnNode*> inputs(1, node_info.mMapNode[0]);
    LogExpr expr; // ダミー
    vector<int> tv(2);
    tv[0] = 1;
    tv[1] = 0;
    LnNode* mapnode1 = mapnetwork.new_lut(oname, inputs, expr, tv);
    node_info.mMapNode[1] = mapnode1;
    return mapnode1;
  }
  
  // node を根とするカットを取り出す．
  const Cut* cut = node_info.mCut;
  
  // その入力に対応するノードを再帰的に生成する．
  ymuint ni = cut->ni();
  for (ymuint i = 0; i < ni; ++ i) {
    SbjNode* inode = cut->input(i);
    back_trace(inode, false, mapnetwork, string());
  }

  mTmpFanins.clear();
  mTmpFanins.resize(ni);
  int idepth = 0;
  for (ymuint i = 0; i < ni; ++ i) {
    SbjNode* inode = cut->input(i);
    NodeInfo& inode_info = mNodeInfo[inode->id()];
    LnNode* imapnode = inode_info.mMapNode[0];
    mTmpFanins[i] = imapnode;
    int idepth1 = inode_info.mDepth;
    if ( idepth < idepth1 ) {
      idepth = idepth1;
    }
  }

  ymuint np = 1 << ni;
  vector<int> tv(np);
  LogExpr expr = cut->expr();
  if ( inv ) {
    expr = ~expr;
  }
  
  // 論理式から真理値表を作る．
  make_tv(ni, expr, tv);
  
  // 新しいノードを作り mNodeMap に登録する．
  string name = (inv) ? oname : node->name();
  mapnode = mapnetwork.new_lut(name, mTmpFanins, expr, tv);
  node_info.mMapNode[idx] = mapnode;
  node_info.mDepth = idepth + 1;
  
  return mapnode;
}

// @brief マッピング結果の LUT 数を見積もる．
int
MapRecord::estimate(const SbjGraph& sbjgraph)
{
  for (vector<NodeInfo>::iterator p = mNodeInfo.begin();
       p != mNodeInfo.end(); ++ p) {
    NodeInfo& node_info = *p;
    node_info.mMapCount[0] = 0;
    node_info.mMapCount[1] = 0;
    node_info.mCovCount = 0;
    node_info.mTmpFlag = false;
  }

  // 外部入力の生成
  const SbjNodeList& input_list = sbjgraph.input_list();
  for (SbjNodeList::const_iterator p = input_list.begin();
       p != input_list.end(); ++ p) {
    SbjNode* node = *p;
    NodeInfo& node_info = mNodeInfo[node->id()];
    node_info.mMapCount[0] = 1;
  }

  // 外部出力からバックトレースを行い全ノードの生成を行う．
  int lut_num = 0;
  const SbjNodeList& output_list = sbjgraph.output_list();
  for (SbjNodeList::const_iterator p = output_list.begin();
       p != output_list.end(); ++ p) {
    SbjNode* onode = *p;
    SbjNode* node = onode->fanin(0);
    if ( node && !node->is_input() ) {
      if ( mNodeInfo[node->id()].mCut == NULL ) {
	lut_num = -1;
	break;
      }
      bool inv = onode->output_inv();
      lut_num += back_trace2(node, inv);
    }
  }
  return lut_num;
}

// サブジェクトグラフの node に対応するマップされたノードを
// 生成し，それを返す．
int
MapRecord::back_trace2(SbjNode* node,
		       bool inv)
{
  NodeInfo& node_info = mNodeInfo[node->id()];
  ymuint idx = (inv) ? 1 : 0;
  ++ node_info.mMapCount[idx];
  if ( node_info.mMapCount[idx] > 1 ) {
    return 0;
  }

  if ( node->is_input() ) {
    // ということは inv = true のはず．
    node_info.mMapCount[1] = 1;
    // インバーターが必要ということ
    return 1;
  }
  
  // node を根とするカットを取り出す．
  const Cut* cut = node_info.mCut;

  // cut のカバーしているノードの mCovCount を1つ増やす．
  for (ymuint i = 0; i < cut->ni(); ++ i) {
    SbjNode* leaf = cut->input(i);
    mNodeInfo[leaf->id()].mTmpFlag = true;
  }
  mark_cover(node, cut);
  clear_mark(node);

  int lut_num = 1;
  // その入力に対応するノードを再帰的に生成する．
  ymuint ni = cut->ni();
  for (ymuint i = 0; i < ni; ++ i) {
    SbjNode* inode = cut->input(i);
    lut_num += back_trace2(inode, false);
  }

  return lut_num;
}

// @brief 直前の estimate の結果 node が fanout node なら true を返す．
bool
MapRecord::check_fonode(SbjNode* node)
{
  NodeInfo& node_info = mNodeInfo[node->id()];
  return (node_info.mMapCount[0] + node_info.mMapCount[1]) > 1;
}

// @brief 直前の estimate の結果で node のカバーされている回数を返す．
int
MapRecord::cover_count(SbjNode* node)
{
  NodeInfo& node_info = mNodeInfo[node->id()];
  return node_info.mCovCount;
}

// cut でカバーされるノードの mCovCount を一つ増やす．
void
MapRecord::mark_cover(SbjNode* node,
		      const Cut* cut)
{
  NodeInfo& node_info = mNodeInfo[node->id()];
  if ( !node_info.mTmpFlag ) {
    node_info.mTmpFlag = true;
    ++ node_info.mCovCount;
    mark_cover(node->fanin(0), cut);
    mark_cover(node->fanin(1), cut);
  }
}

// mark_cover でつけた mTmpFlag を下ろす．
void
MapRecord::clear_mark(SbjNode* node)
{
  NodeInfo& node_info = mNodeInfo[node->id()];
  if ( node_info.mTmpFlag ) {
    node_info.mTmpFlag = false;
    if ( !node->is_input() ) {
      clear_mark(node->fanin(0));
      clear_mark(node->fanin(1));
    }
  }
}

END_NAMESPACE_MAGUS_LUTMAP
