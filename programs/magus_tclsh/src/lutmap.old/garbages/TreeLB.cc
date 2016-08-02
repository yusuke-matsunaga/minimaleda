
/// @file magus/lutmap/TreeLB.cc
/// @brief TreeLB の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: TreeLB.cc 1077 2007-12-03 10:18:47Z matsunaga $
///
/// Copyright (C) 2005-2007 Yusuke Matsunaga
/// All rights reserved.


#if HAVE_CONFIG_H
#include <magus_config.h>
#endif

#include <ym_utils/RandGen.h>
#include "TreeLB.h"
#include "SbjGraph.h"
#include "Cut.h"
#include "CutHolder.h"


BEGIN_NAMESPACE_MAGUS_LUTMAP

//////////////////////////////////////////////////////////////////////
// クラス TreeLB
//////////////////////////////////////////////////////////////////////

BEGIN_NONAMESPACE

// fo_list のなかに tree マークの付いた枝があれば true を返す．
bool
check_tree(const SbjEdgeList& fo_list)
{
  for (SbjEdgeList::const_iterator p = fo_list.begin();
       p != fo_list.end(); ++ p) {
    if ( (*p)->tree() ) {
      return true;
    }
  }
  return false;
}

END_NONAMESPACE


// コンストラクタ
TreeLB::TreeLB(size_t n) :
  mN(n)
{
}

// デストラクタ
TreeLB::~TreeLB()
{
}

// 下限を計算する．
int
TreeLB::calc(const SbjGraph& sbjgraph,
	     const CutHolder& cut_holder)
{
  size_t limit = cut_holder.limit();
  size_t n = sbjgraph.max_node_id();
  mBestCost.resize(n);
  mReached.resize(limit);

  const SbjNodeList& node_list = sbjgraph.lnode_list();
  const SbjNodeList& output_list = sbjgraph.output_list();

  RandGen rg;

  int max_lb = 0;
  for (size_t c = 0; c < mN; ++ c) {
    // tree フラグを付ける．
    // 外部出力から指されているノードの場合はその枝を優先する．
    for (SbjNodeList::const_iterator p = output_list.begin();
	 p != output_list.end(); ++ p) {
      SbjNode* onode = *p;
      SbjNode* node = onode->fanin(0);
      if ( node && check_tree(node->fanout_list()) == false ) {
	SbjEdge* e = onode->fanin_edge(0);
	e->set_tree();
      }
    }
    // 残りはランダムにつける．
    for (SbjNodeList::const_iterator p = node_list.begin();
	 p != node_list.end(); ++ p) {
      SbjNode* node = *p;
      size_t nfo = node->n_fanout();
      const SbjEdgeList& fo_list = node->fanout_list();
      if ( check_tree(fo_list) == false && node->n_fanout() > 0 ) {
	if ( nfo == 1 || mN == 1 ) {
	  fo_list.front()->set_tree();
	}
	else {
	  size_t r = rg.int32() % nfo;
	  SbjEdgeList::const_iterator q = fo_list.begin();
	  for (size_t i = 0; i < r; ++ i, ++ q) ;
	  SbjEdge* e = *q;
	  e->set_tree();
	}
      }
    }

    for (size_t i = 0; i < n; ++ i) {
      SbjNode* node = sbjgraph.node(i);
      if ( node && node->is_input() ) {
	mBestCost[i] = 0;
      }
      else {
	mBestCost[i] = INT_MAX;
      }
    }

    vector<SbjNode*> snode_list;
    sbjgraph.sort(snode_list);
    for (vector<SbjNode*>::iterator p = snode_list.begin();
	 p != snode_list.end(); ++ p) {
      SbjNode* node = *p;
      count(node, cut_holder);
    }
    
    int sum = 0;
    for (SbjNodeList::const_iterator p = output_list.begin();
	 p != output_list.end(); ++ p) {
      SbjNode* onode = *p;
      SbjNode* node = onode->fanin(0);
      if ( node != NULL && onode->fanin_edge(0)->tree() ) {
	sum += mBestCost[node->id()];
      }
    }
    if ( max_lb < sum ) {
      max_lb = sum;
    }

    // tree フラグを消す．
    for (SbjNodeList::const_iterator p = node_list.begin();
	 p != node_list.end(); ++ p) {
      SbjNode* node = *p;
      const SbjEdgeList& fo_list = node->fanout_list();
      for (SbjEdgeList::const_iterator q = fo_list.begin();
	   q != fo_list.end(); ++ q) {
	SbjEdge* e = *q;
	e->clear_tree();
      }
    }
  }
  
  return max_lb;
}

// node のコストを計算する．
void
TreeLB::count(SbjNode* node,
	      const CutHolder& cut_holder)
{
  int min_cost = INT_MAX;
  const CutList& cut_list = cut_holder.cut_list(node);
  for (CutListIterator p = cut_list.begin();
       p != cut_list.end(); ++ p) {
    const Cut* cut = *p;

    size_t ni = cut->ni();
    // tree edge の経路が存在するか調べる．
    for (size_t i = 0; i < ni; ++ i) {
      mReached[i] = false;
    }
    bool stat = check_path(node, cut, true);
    if ( !stat ) continue;

    int cur_cost = 1;
    for (size_t i = 0; i < ni; ++ i) {
      if ( mReached[i] ) {
	SbjNode* inode = cut->input(i);
	cur_cost += mBestCost[inode->id()];
      }
    }
    if ( min_cost > cur_cost ) {
      min_cost = cur_cost;
    }
  }
  mBestCost[node->id()] = min_cost;
}

// node から tree フラグをたどって到達可能な入力に印をつける．
bool
TreeLB::check_path(SbjNode* node,
		   const Cut* cut,
		   bool reached)
{
  for (size_t i = 0; i < cut->ni(); ++ i) {
    if ( cut->input(i) == node ) {
      if ( reached ) {
	mReached[i] = true;
      }
      return true;
    }
  }

  for (size_t i = 0; i < 2; ++ i) {
    bool reached1 = reached;
    if ( !node->fanin_edge(i)->tree() ) {
      reached1 = false;
    }
    SbjNode* inode = node->fanin(i);
    bool stat = check_path(inode, cut, reached1);
    if ( !stat ) {
      return false;
    }
  }
  return true;
}

END_NAMESPACE_MAGUS_LUTMAP
