
/// @file libym_techmap/SbjMinDepth.cc
/// @brief SbjMinDepth の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "SbjMinDepth.h"
#include "SmdNode.h"
#include "ym_sbj/SbjGraph.h"


BEGIN_NAMESPACE_YM_SBJ

//////////////////////////////////////////////////////////////////////
// クラス SmdNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SmdNode::SmdNode() :
  mFanoutNum(0),
  mFanoutArray(NULL)
{
}

// @brief デストラクタ
SmdNode::~SmdNode()
{
}

// @brief ID番号を設定する．
// @param[in] id ID番号
// @param[in] logic 論理ノードの時 true にセットするフラグ
void
SmdNode::set_id(ymuint id,
		bool logic)
{
  mId = (id << 1) | static_cast<ymuint>(logic);
}

// @brief 1つ目のファンインを設定する．
void
SmdNode::set_fanin0(SmdNode* from)
{
  mFanin0.mFrom = from;
  mFanin0.mTo = this;
  mFanin0.mFlags = 0U;
}

// @brief 2つ目のファンインを設定する．
void
SmdNode::set_fanin1(SmdNode* from)
{
  mFanin1.mFrom = from;
  mFanin1.mTo = this;
  mFanin1.mFlags = 1U;
}

// @brief ファンアウト数を設定する．
void
SmdNode::set_fanout_num(ymuint n,
			void* p)
{
  mFanoutNum = n;
  mFanoutArray = new (p) SmdEdge*[n];
}

// @brief pos 番目のファンアウトを設定する．
void
SmdNode::set_fanout(ymuint pos,
		    SmdEdge* edge)
{
  mFanoutArray[pos] = edge;
}


//////////////////////////////////////////////////////////////////////
// クラス SbjMinDepth
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] sbjgraph 対象のサブジェクトグラフ
SbjMinDepth::SbjMinDepth(const SbjGraph& sbjgraph) :
  mAlloc(4096),
  mSbjGraph(sbjgraph)
{
  ymuint n = sbjgraph.max_node_id();

  mTfiNodeList.reserve(n);

  mNodeNum = n;
  void* p = mAlloc.get_memory(sizeof(SmdNode) * n);
  mNodeArray = new (p) SmdNode[n];

  // sbjgraph の構造を SmdNode にコピーする．
  vector<const SbjNode*> ppi_list;
  sbjgraph.ppi_list(ppi_list);
  mInputList.reserve(ppi_list.size());
  for (vector<const SbjNode*>::const_iterator p = ppi_list.begin();
       p != ppi_list.end(); ++ p) {
    const SbjNode* sbjnode = *p;
    ymuint id = sbjnode->id();
    SmdNode* node = &mNodeArray[id];
    node->set_id(id, false);
    mInputList.push_back(node);
  }
  vector<const SbjNode*> sbjnode_list;
  sbjgraph.sort(sbjnode_list);
  mLogicNodeList.reserve(sbjgraph.lnode_num());
  for (vector<const SbjNode*>::const_iterator p = sbjnode_list.begin();
       p != sbjnode_list.end(); ++ p) {
    const SbjNode* sbjnode = *p;
    assert_cond( sbjnode->is_logic(), __FILE__, __LINE__);
    ymuint id = sbjnode->id();
    SmdNode* node = &mNodeArray[id];
    mLogicNodeList.push_back(node);
    node->set_id(id, true);
    const SbjNode* isbjnode0 = sbjnode->fanin(0);
    SmdNode* inode0 = &mNodeArray[isbjnode0->id()];
    node->set_fanin0(inode0);
    const SbjNode* isbjnode1 = sbjnode->fanin(1);
    SmdNode* inode1 = &mNodeArray[isbjnode1->id()];
    node->set_fanin1(inode1);
  }
  for (ymuint i = 0; i < n; ++ i) {
    const SbjNode* sbjnode = sbjgraph.node(i);
    if ( sbjnode == NULL ) continue;
    SmdNode* node = &mNodeArray[sbjnode->id()];
    const SbjEdgeList& fanout_list = sbjnode->fanout_list();
    ymuint nfo = 0;
    for (SbjEdgeList::const_iterator p = fanout_list.begin();
	 p != fanout_list.end(); ++ p) {
      const SbjEdge* sbjedge = *p;
      const SbjNode* sbjfonode = sbjedge->to();
      if ( !sbjfonode->is_ppo() ) {
	++nfo;
      }
    }
    void* p = mAlloc.get_memory(sizeof(SmdEdge*) * nfo);
    node->set_fanout_num(nfo, p);
    ymuint pos = 0;
    for (SbjEdgeList::const_iterator p = fanout_list.begin();
	 p != fanout_list.end(); ++ p, ++ pos) {
      const SbjEdge* sbjedge = *p;
      const SbjNode* sbjfonode = sbjedge->to();
      if ( sbjfonode->is_ppo() ) continue;
      SmdNode* fonode = &mNodeArray[sbjfonode->id()];
      assert_cond( fonode->id() == sbjfonode->id(), __FILE__, __LINE__);
      SmdEdge* edge = NULL;
      if ( sbjedge->pos() == 0 ) {
	edge = fonode->fanin0_edge();
      }
      else {
	edge = fonode->fanin1_edge();
      }
      assert_cond( edge->to() == fonode, __FILE__, __LINE__);
      assert_cond( edge->from() == node, __FILE__, __LINE__);
      node->set_fanout(pos, edge);
    }
  }
}

// @brief デストラクタ
SbjMinDepth::~SbjMinDepth()
{
}

// @brief 各ノードの minimum depth を求める．
// @param[in] sbjgraph 対象のサブジェクトグラフ
// @param[in] k LUT の最大入力数
// @param[out] depth_array 各ノードの深さを収める配列
// @return 出力の最大深さを返す．
ymuint
SbjMinDepth::operator()(ymuint k,
			vector<ymuint>& depth_array)
{
  depth_array.clear();
  depth_array.resize(mNodeNum, 0);

  // 入力側から depth を計算してゆく
  for (vector<SmdNode*>::iterator p = mInputList.begin();
       p != mInputList.end(); ++ p) {
    SmdNode* node = *p;
    node->clear_rtfmark();
    node->clear_vmark();
    node->set_depth(0);
    depth_array[node->id()] = 0;
  }
  ymuint ans = 0;
  for (vector<SmdNode*>::iterator p = mLogicNodeList.begin();
       p != mLogicNodeList.end(); ++ p) {
    SmdNode* node = *p;
    node->clear_rtfmark();
    node->clear_vmark();
    // ファンインの depth の最大値を max_depth に入れる．
    SmdNode* inode0 = node->fanin0();
    ymuint max_depth = inode0->depth();
    SmdNode* inode1 = node->fanin1();
    ymuint d1 = inode1->depth();
    if ( max_depth < d1 ) {
      max_depth = d1;
    }

    // max_depth 以下の depth を持つノードのみで構成される k-feasible cut を
    // 見つけることができたら node の depth も max_depth となる．
    // そうでなければ max_depth + 1
    if ( !find_k_cut(node, k, max_depth) ) {
      ++ max_depth;
    }
    node->set_depth(max_depth);
    depth_array[node->id()] = max_depth;
    if ( ans < max_depth ) {
      ans = max_depth;
    }
  }
  return ans;
}

// node を根とする深さ d - 1 の k-feasible cut が存在するかどうか調べる．
bool
SbjMinDepth::find_k_cut(SmdNode* node,
			ymuint k,
			ymuint d)
{
  if ( d == 0 ) {
    return false;
  }

  // node の transitive fanin に rmark を付ける．
  // node および 深さ d のノードに tmark を付ける．
  mTfiNodeList.clear();
  node->set_tmark();
  mark_tfi(node, d);

  // PI から tmark の付いたノードまで至る素な経路が
  // k + 1 本以上あれば k-feasible cut は存在しない．
  bool found = true;
  ymuint c = 0;
  for (vector<SmdNode*>::const_iterator p = mInputList.begin();
       p != mInputList.end(); ++ p) {
    SmdNode* start = *p;
    if ( !start->rmark() ) {
      continue;
    }
    bool stat = dfs_fanout(start);
    for (vector<SmdNode*>::iterator q = mTfiNodeList.begin();
	 q != mTfiNodeList.end(); ++ q) {
      SmdNode* node = *q;
      node->clear_vmark();
    }
    if ( stat ) {
      ++ c;
      if ( c > k ) {
	found = false;
	break;
      }
    }
  }
  for (vector<SmdNode*>::iterator q = mTfiNodeList.begin();
       q != mTfiNodeList.end(); ++ q) {
    SmdNode* node = *q;
    node->clear_rtfmark();
    if ( node->is_logic() ) {
      node->fanin0_edge()->clear_flow();
      node->fanin1_edge()->clear_flow();
    }
  }

  return found;
}

// node およびその TFI に rmark を付ける．
// depth が d のノードに tmark を付ける．
void
SbjMinDepth::mark_tfi(SmdNode* node,
		      ymuint d)
{
  if ( !node->check_rmark() ) {
    mTfiNodeList.push_back(node);
    if ( node->depth() == d ) {
      node->set_tmark();
    }
    if ( node->is_logic() ) {
      mark_tfi(node->fanin0(), d);
      mark_tfi(node->fanin1(), d);
    }
  }
}

// node のファンアウトを探索する．
bool
SbjMinDepth::dfs_fanout(SmdNode* node)
{
  ymuint n = node->fanout_num();
  for (ymuint i = 0; i < n; ++ i) {
    SmdEdge* edge = node->fanout_edge(i);
    SmdNode* to_node = edge->to();
    if ( !to_node->rmark() ) {
      // target の TFI ではない．
      continue;
    }
    if ( !to_node->check_vmark1() ) {
      bool stat = dfs(to_node, 0);
      if ( stat ) {
	edge->set_flow();
	return true;
      }
    }
  }
  return false;
}

bool
SbjMinDepth::dfs(SmdNode* cur_node,
		 int dir)
{
  if ( cur_node->tmark() ) {
    // target にたどり着いた
    return true;
  }

  if ( dir == 0 ) {
    // 前向き
    SmdEdge* edge0 = cur_node->fanin0_edge();
    SmdEdge* edge1 = cur_node->fanin1_edge();
    if ( edge0->flow() ) {
      SmdNode* from_node = edge0->from();
      if ( !from_node->check_vmark2() ) {
	bool stat = dfs(from_node, 1);
	if ( stat ) {
	  edge0->clear_flow();
	  return true;
	}
      }
    }
    else if ( edge1->flow() ) {
      SmdNode* from_node = edge1->from();
      if ( !from_node->check_vmark2() ) {
	bool stat = dfs(from_node, 1);
	if ( stat ) {
	  edge1->clear_flow();
	  return true;
	}
      }
    }
    else if ( !cur_node->check_vmark2() ) {
      bool stat = dfs_fanout(cur_node);
      if ( stat ) {
	return true;
      }
    }
  }
  else {
    if ( cur_node->is_logic() && !cur_node->check_vmark1() ) {
      SmdEdge* edge0 = cur_node->fanin0_edge();
      if ( edge0->flow() ) {
	SmdNode* from_node = edge0->from();
	if ( !from_node->check_vmark2() ) {
	  bool stat = dfs(from_node, 1);
	  if ( stat ) {
	    edge0->clear_flow();
	    return true;
	  }
	}
      }
      SmdEdge* edge1 = cur_node->fanin1_edge();
      if ( edge1->flow() ) {
	SmdNode* from_node = edge1->from();
	if ( !from_node->check_vmark2() ) {
	  bool stat = dfs(from_node, 1);
	  if ( stat ) {
	    edge1->clear_flow();
	    return true;
	  }
	}
      }
    }
    bool stat = dfs_fanout(cur_node);
    if ( stat ) {
      return true;
    }
  }
  return false;
}

END_NAMESPACE_YM_SBJ
