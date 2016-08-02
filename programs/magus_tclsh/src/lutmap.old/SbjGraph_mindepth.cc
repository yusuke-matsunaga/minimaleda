
/// @file magus/lutmap/SbjGraph_mindepth.cc 
/// @brief SbjGraph の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: SbjGraph_mindepth.cc 2274 2009-06-10 07:45:29Z matsunaga $
///
/// Copyright (C) 2005-2006 Yusuke Matsunaga
/// All rights reserved.


#if HAVE_CONFIG_H
#include "magus_config.h"
#endif

#include "SbjGraph.h"


BEGIN_NAMESPACE_MAGUS_LUTMAP

//////////////////////////////////////////////////////////////////////
// minimum depth 関係のコード
//////////////////////////////////////////////////////////////////////

BEGIN_NONAMESPACE

bool
dfs(SbjNode* cur_node,
    int dir);

// node およびその TFI に rmark を付ける．
// depth が d のノードに tmark を付ける．
void
mark_tfi(SbjNode* node,
	 ymuint d,
	 list<SbjNode*>& node_list)
{
  if ( !node->rmark() ) {
    node->set_rmark();
    node_list.push_back(node);
    if ( node->depth() == d ) {
      node->set_tmark();
    }
    if ( node->is_logic() ) {
      mark_tfi(node->fanin(0), d, node_list);
      mark_tfi(node->fanin(1), d, node_list);
    }
  }
}

// node のファンアウトを探索する．
bool
dfs_fanout(SbjNode* node)
{
  const SbjEdgeList& fanout_list = node->fanout_list();
  for (SbjEdgeList::const_iterator p = fanout_list.begin();
       p != fanout_list.end(); ++ p) {
    SbjEdge* edge = *p;
    SbjNode* to_node = edge->to();
    if ( !to_node->rmark() ) {
      // target の TFI ではない．
      continue;
    }
    if ( to_node->vmark1() ) {
      // 既に処理済み
      continue;
    }
    to_node->set_vmark1();
    bool stat = dfs(to_node, 0);
    if ( stat ) {
      edge->set_flow();
      return true;
    }
  }
  return false;
}

bool
dfs(SbjNode* cur_node,
    int dir)
{
  if ( cur_node->tmark() ) {
    // target にたどり着いた
    return true;
  }
  
  if ( dir == 0 ) {
    // 前向き
    if ( cur_node->fanin_edge(0)->flow() ) {
      SbjEdge* edge = cur_node->fanin_edge(0);
      SbjNode* from_node = edge->from();
      if ( !from_node->vmark2() ) {
	from_node->set_vmark2();
	bool stat = dfs(from_node, 1);
	if ( stat ) {
	  edge->clear_flow();
	  return true;
	}
      }
    }
    else if ( cur_node->fanin_edge(1)->flow() ) {
      SbjEdge* edge = cur_node->fanin_edge(1);
      SbjNode* from_node = edge->from();
      if ( !from_node->vmark2() ) {
	from_node->set_vmark2();
	bool stat = dfs(from_node, 1);
	if ( stat ) {
	  edge->clear_flow();
	  return true;
	}
      }
    }
    else if ( !cur_node->vmark2() ) {
      cur_node->set_vmark2();
      bool stat = dfs_fanout(cur_node);
      if ( stat ) {
	return true;
      }
    }
  }
  else {
    if ( !cur_node->vmark1() && cur_node->is_logic() ) {
      cur_node->set_vmark1();
      for (ymuint i = 0; i < 2; ++ i) {
	SbjEdge* edge = cur_node->fanin_edge(i);
	if ( edge->flow() ) {
	  SbjNode* from_node = edge->from();
	  if ( !from_node->vmark2() ) {
	    from_node->set_vmark2();
	    bool stat = dfs(from_node, 1);
	    if ( stat ) {
	      edge->clear_flow();
	      return true;
	    }
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

END_NONAMESPACE


// node を根とする深さ d - 1 の k-feasible cut が存在するかどうか調べる．
bool
SbjGraph::find_k_cut(SbjNode* node,
		     ymuint k,
		     ymuint d) const
{
  if ( d == 0 ) {
    return false;
  }

  // node の transitive fanin に rmark を付ける．
  // node および 深さ d のノードに tmark を付ける．
  list<SbjNode*> node_list;
  node->set_tmark();
  mark_tfi(node, d, node_list);

  // PI から tmark の付いたノードまで至る素な経路が
  // k + 1 本以上あれば k-feasible cut は存在しない．
  bool found = true;
  ymuint c = 0;
  for (SbjNodeList::const_iterator p = input_list().begin();
       p != input_list().end(); ++ p) {
    SbjNode* start = *p;
    if ( !start->rmark() ) {
      continue;
    }
    bool stat = dfs_fanout(start);
    for (list<SbjNode*>::iterator q = node_list.begin();
	 q != node_list.end(); ++ q) {
      SbjNode* node = *q;
      node->clear_vmark1();
      node->clear_vmark2();
    }
    if ( stat ) {
      ++ c;
      if ( c > k ) {
	found = false;
	break;
      }
    }
  }
  for (list<SbjNode*>::iterator q = node_list.begin();
       q != node_list.end(); ++ q) {
    SbjNode* node = *q;
    node->clear_rmark();
    node->clear_tmark();
    node->clear_fmark();
    if ( node->is_logic() ) {
      node->fanin_edge(0)->clear_flow();
      node->fanin_edge(1)->clear_flow();
    }
  }
  
  return found;
}

// @brief 各ノードの minimum depth を求める．
void
SbjGraph::get_min_depth(ymuint k) const
{
  // 外部入力ノードの depth を 0 にする．
  for (SbjNodeList::const_iterator p = input_list().begin();
       p != input_list().end(); ++ p) {
    SbjNode* node = *p;
    node->mDepth = 0;
  }

  // 入力側から depth を計算してゆく
  vector<SbjNode*> node_list;
  sort(node_list);
  for (vector<SbjNode*>::const_iterator p = node_list.begin();
       p != node_list.end(); ++ p) {
    SbjNode* node = *p;
    // ファンインの depth の最大値を max_depth に入れる．
    ymuint max_depth = 0;
    for (ymuint i = 0; i < 2; ++ i) {
      SbjNode* inode0 = node->fanin(i);
      ymuint d0 = inode0->depth();
      if ( max_depth < d0 ) {
	max_depth = d0;
      }
    }
    
    // max_depth 以下の depth を持つノードのみで構成される k-feasible cut を
    // 見つけることができたら node の depth も max_depth となる．
    // そうでなければ max_depth + 1
    if ( !find_k_cut(node, k, max_depth) ) {
      ++ max_depth;
    }
    node->mDepth = max_depth;
  }
}

END_NAMESPACE_MAGUS_LUTMAP
