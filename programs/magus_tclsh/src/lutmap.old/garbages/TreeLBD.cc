
/// @file magus/lutmap/TreeLBD.cc
/// @brief TreeLBD の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: TreeLBD.cc 1077 2007-12-03 10:18:47Z matsunaga $
///
/// Copyright (C) 2005-2007 Yusuke Matsunaga
/// All rights reserved.


#if HAVE_CONFIG_H
#include <magus_config.h>
#endif


#include <float.h>
#include <ym_utils/RandGen.h>

#include "TreeLBD.h"
#include "SbjGraph.h"
#include "Cut.h"
#include "CutHolder.h"


BEGIN_NAMESPACE_MAGUS_LUTMAP

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
TreeLBD::TreeLBD(size_t n) :
  mN(n)
{
}

// デストラクタ
TreeLBD::~TreeLBD()
{
}

// @brief 深さ制約下での lower bound を求める．
// @param[in] cut_holder カットを保持するオブジェクト
// @param[in] slack 最小段数に対するスラック
// @return covering の結果の LUT 数
int
TreeLBD::operator()(const SbjGraph& sbjgraph,
		    const CutHolder& cut_holder,
		    size_t slack)
{
  size_t limit = cut_holder.limit();
  size_t n = sbjgraph.max_node_id();
  const SbjNodeList& input_list = sbjgraph.input_list();
  const SbjNodeList& output_list = sbjgraph.output_list();
  const SbjNodeList& node_list = sbjgraph.lnode_list();
  vector<SbjNode*> snode_list;
  sbjgraph.sort(snode_list);

  // 作業領域の初期化
  mNodeInfo.clear();
  mNodeInfo.resize(n);
  for (size_t i = 0; i < n; ++ i) {
    mNodeInfo[i].mCostList.set_mgr(&mCostMgr);
  }
  mReached.resize(limit);

  RandGen rg;

  int max_lb = 0;
  for (size_t c = 0; c < mN; ++ c) {
    // tree フラグを付ける．
    // 外部出力から指されているノードの場合はその枝を優先する．
    for (SbjNodeList::const_iterator p = output_list.begin();
	 p != output_list.end(); ++ p) {
      SbjNode* onode = *p;
      SbjNode* node = onode->fanin(0);
      if ( check_tree(node->fanout_list()) == false ) {
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

    // 入力のコストの設定
    for (SbjNodeList::const_iterator p = input_list.begin();
	 p != input_list.end(); ++ p) {
      SbjNode* node = *p;
      NodeInfo& t = mNodeInfo[node->id()];
      t.mCostList.insert(NULL, 0, 0);
      t.mMinDepth = 0;
    }

    // 各ノードごとにカットを記録
    for (vector<SbjNode*>::iterator p = snode_list.begin();
	 p != snode_list.end(); ++ p) {
      SbjNode* node = *p;
      record(node, cut_holder);
    }

    // 最小段数をもとめる．
    int min_depth = 0;
    for (SbjNodeList::const_iterator p = output_list.begin();
	 p != output_list.end(); ++ p) {
      SbjNode* onode = *p;
      SbjNode* node = onode->fanin(0);
      if ( node ) {
	int depth = mNodeInfo[node->id()].mMinDepth;
	if ( min_depth < depth ) {
	  min_depth = depth;
	}
      }
    }
    // それに slack を足したものが制約となる．
    min_depth += slack;

    int lb = 0;
    for (SbjNodeList::const_iterator p = output_list.begin();
	 p != output_list.end(); ++ p) {
      SbjNode* onode = *p;
      SbjNode* node = onode->fanin(0);
      if ( node && onode->fanin_edge(0)->tree() ) {
	lb += count(node, min_depth);
      }
    }
    if ( max_lb < lb ) {
      max_lb = lb;
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

// node のカットを選択する．
void
TreeLBD::record(SbjNode* node,
		const CutHolder& cut_holder)
{
  int min_depth = INT_MAX;
  NodeInfo& t = mNodeInfo[node->id()];
  const CutList& cut_list = cut_holder.cut_list(node);
  for (CutListIterator p = cut_list.begin();
       p != cut_list.end(); ++ p) {
    const Cut* cut = *p;
    size_t ni = cut->ni();
    for (size_t i = 0; i < ni; ++ i) {
      mReached[i] = false;
    }
    bool stat = check_path(node, cut, true);
    if ( !stat ) continue;

    int max_input_depth = 0;
    vector<ADCostIterator<int> > icost_lists(ni);
    for (size_t i = 0; i < ni; ++ i) {
      SbjNode* inode = cut->input(i);
      NodeInfo& u = mNodeInfo[inode->id()];
      if ( max_input_depth < u.mMinDepth ) {
	max_input_depth = u.mMinDepth;
      }
      icost_lists[i] = u.mCostList.begin();
    }
    int cur_depth = max_input_depth + 1;
    if ( min_depth > cur_depth ) {
      min_depth = cur_depth;
    }
    
    // icost_lists から解を作る．
    for ( ; ; ) {
      // 各入力のなかでもっとも深い値を求める．
      int idepth = 0;
      int area = 1;
      bool empty = false;
      for (size_t i = 0; i < ni; ++ i) {
	if ( icost_lists[i].is_end() ) {
	  empty = true;
	  break;
	}
	ADCost<int>* cost = *icost_lists[i];
	if ( idepth < cost->depth() ) {
	  idepth = cost->depth();
	}
	if ( mReached[i] ) {
	  area += cost->area();
	}
      }
      if ( empty ) {
	break;
      }
      int depth = idepth + 1;
      
      // (depth, area) を登録
      t.mCostList.insert(cut, depth, area);
      
      // 深さが idepth に等しい解を次に進める．
      for (size_t i = 0; i < ni; ++ i) {
	ADCost<int>* cost = *icost_lists[i];
	if ( cost->depth() == idepth ) {
	  ++ icost_lists[i];
	}
      }
    }
  }
  t.mMinDepth = min_depth;
}
  
// node を根とする木のノード数を数える．
int
TreeLBD::count(SbjNode* node,
	       int req)
{
  if ( node->is_input() ) {
    return 0;
  }
  
  NodeInfo& t = mNodeInfo[node->id()];
  for (ADCostIterator<int> p = t.mCostList.begin();
       !p.is_end(); ++ p) {
    ADCost<int>* cost = *p;
    if ( cost->depth() <= req ) {
      return cost->area();
    }
  }
  assert_not_reached(__FILE__, __LINE__);
  return 0;
}

// node から tree フラグをたどって到達可能な入力に印をつける．
bool
TreeLBD::check_path(SbjNode* node,
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
