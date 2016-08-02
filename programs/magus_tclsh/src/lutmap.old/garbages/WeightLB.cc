
/// @file magus/lutmap/WeightLB.cc
/// @brief WeightLB の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: WeightLB.cc 1077 2007-12-03 10:18:47Z matsunaga $
///
/// Copyright (C) 2005-2007 Yusuke Matsunaga
/// All rights reserved.


#if HAVE_CONFIG_H
#include <magus_config.h>
#endif

#include <float.h>

#include <ym_utils/RandGen.h>
#include "WeightLB.h"
#include "SbjGraph.h"
#include "Cut.h"
#include "CutHolder.h"


BEGIN_NAMESPACE_MAGUS_LUTMAP

//////////////////////////////////////////////////////////////////////
// クラス WeightLB
//////////////////////////////////////////////////////////////////////

// コンストラクタ
WeightLB::WeightLB()
{
}

// デストラクタ
WeightLB::~WeightLB()
{
}

// 下限を計算する．
int
WeightLB::calc(const SbjGraph& sbjgraph,
	       const CutHolder& cut_holder)
{
  size_t limit = cut_holder.limit();
  size_t n = sbjgraph.max_node_id();
  mBestCost.resize(n);
  mWeight.resize(limit);

  const SbjNodeList& input_list = sbjgraph.input_list();
  const SbjNodeList& node_list = sbjgraph.lnode_list();

  for (SbjNodeList::const_iterator p = input_list.begin();
       p != input_list.end(); ++ p) {
    SbjNode* node = *p;
    mBestCost[node->id()] = 0.0;
  }

  vector<SbjNode*> snode_list;
  sbjgraph.sort(snode_list);
  for (vector<SbjNode*>::iterator p = snode_list.begin();
       p != snode_list.end(); ++ p) {
    SbjNode* node = *p;
    count(node, cut_holder);
  }
    
  double cost = 0.0;
  for (SbjNodeList::const_iterator p = node_list.begin();
       p != node_list.end(); ++ p) {
    SbjNode* node = *p;
    if ( node->pomark() ) {
      if ( mBestCost[node->id()] == DBL_MAX ) {
	return -1;
      }
      cost += mBestCost[node->id()];
    }
  }
  return static_cast<int>(ceil(cost));
}

// node のコストを計算する．
void
WeightLB::count(SbjNode* node,
		const CutHolder& cut_holder)
{
  double min_cost = DBL_MAX;
  if ( !node->is_unselected() ) {
    const CutList& cut_list = cut_holder.cut_list(node);
    for (CutListIterator p = cut_list.begin();
	 p != cut_list.end(); ++ p) {
      const Cut* cut = *p;

      size_t ni = cut->ni();
      bool ng = false;
      for (size_t i = 0; i < ni; ++ i) {
	SbjNode* inode = cut->input(i);
	if ( mBestCost[inode->id()] == DBL_MAX ) {
	  ng = true;
	  break;
	}
      }
      if ( ng ) continue;
      if ( check_nodes(node, cut) ) continue;

      // 経路上の重みを計算する．
      for (size_t i = 0; i < ni; ++ i) {
	mWeight[i] = 0.0;
      }
      calc_weight(node, cut, 1.0);

      double cur_cost = 1.0;
      for (size_t i = 0; i < ni; ++ i) {
	SbjNode* inode = cut->input(i);
	cur_cost += mBestCost[inode->id()] * mWeight[i];
      }
      if ( min_cost > cur_cost ) {
	min_cost = cur_cost;
      }
    }
  }
  mBestCost[node->id()] = min_cost;
}

// node から各入力にいたる経路の重みを計算する．
void
WeightLB::calc_weight(SbjNode* node,
		      const Cut* cut,
		      double cur_weight)
{
  for (size_t i = 0; i < cut->ni(); ++ i) {
    if ( cut->input(i) == node ) {
      if  ( !node->pomark() ) {
	mWeight[i] += cur_weight;
      }
      return;
    }
  }
  SbjNode* inode0 = node->fanin(0);
  size_t fo0 = inode0->n_fanout();
  double cur_weight0 = cur_weight / fo0;
  calc_weight(inode0, cut, cur_weight0);

  SbjNode* inode1 = node->fanin(1);
  size_t fo1 = inode1->n_fanout();
  double cur_weight1 = cur_weight / fo1;
  calc_weight(inode1, cut, cur_weight1);
}

// selected ノードをカットの内部に含んでいたら true を返す．
bool
WeightLB::check_nodes(SbjNode* node,
		      const Cut* cut)
{
  for ( ; ; ) {
    for (size_t i = 0; i < cut->ni(); ++ i) {
      if ( cut->input(i) == node ) {
	// node は cut の葉だった．
	return false;
      }
    }
    if ( node->is_fo() && node != cut->root() ) {
      return true;
    }
    SbjNode* inode0 = node->fanin(0);
    if ( check_nodes(inode0, cut) ) {
      return true;
    }
    node = node->fanin(1);
  }
}

END_NAMESPACE_MAGUS_LUTMAP
