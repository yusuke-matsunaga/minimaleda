
/// @file magus/lutmap/DFCover.cc
/// @brief duplicaton free covering を行うクラス
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: DFCover.cc 2274 2009-06-10 07:45:29Z matsunaga $
///
/// Copyright (C) 2005-2006 Yusuke Matsunaga
/// All rights reserved.


#if HAVE_CONFIG_H
#include "magus_config.h"
#endif

#include "DFCover.h"
#include "SbjGraph.h"
#include "Cut.h"
#include "CutHolder.h"
#include "MapRecord.h"


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
DFCover::DFCover()
{
}

// デストラクタ
DFCover::~DFCover()
{
}

// @brief best cut の記録を行う．
// @param[in] sbjgraph サブジェクトグラフ
// @param[in] cut_holder カットを保持するオブジェクト
// @param[out] maprec マッピング結果を記録するオブジェクト
int
DFCover::record_cuts(const SbjGraph& sbjgraph,
		     const CutHolder& cut_holder,
		     MapRecord& maprec)
{
  ymuint n = sbjgraph.max_node_id();
  mBestCost.clear();
  mBestCost.resize(n);
  
  mReached.resize(cut_holder.limit());

  maprec.init(sbjgraph);
  
  // tree フラグを付ける．
  // 単純には最初のファンアウトの枝に tree フラグをつければよい．
  // ただし，外部出力から指されているノードの場合はその枝を優先する．
  const SbjNodeList& input_list = sbjgraph.input_list();
  const SbjNodeList& node_list = sbjgraph.lnode_list();
  const SbjNodeList& output_list = sbjgraph.output_list();
  for (SbjNodeList::const_iterator p = output_list.begin();
       p != output_list.end(); ++ p) {
    SbjNode* onode = *p;
    SbjNode* node = onode->fanin(0);
    if ( node && check_tree(node->fanout_list()) == false ) {
      SbjEdge* e = onode->fanin_edge(0);
      e->set_tree();
    }
  }
  for (SbjNodeList::const_iterator p = node_list.begin();
       p != node_list.end(); ++ p) {
    SbjNode* node = *p;
    const SbjEdgeList& fo_list = node->fanout_list();
    if ( check_tree(fo_list) == false && node->n_fanout() > 0 ) {
      fo_list.front()->set_tree();
    }
  }

  // 入力のコストを設定
  for (SbjNodeList::const_iterator p = input_list.begin();
       p != input_list.end(); ++ p) {
    SbjNode* node = *p;
    maprec.set_cut(node, NULL);
    mBestCost[node->id()] = 0;
  }

  // 論理ノードのコストを入力側から計算
  vector<SbjNode*> snode_list;
  sbjgraph.sort(snode_list);
  for (vector<SbjNode*>::const_iterator p = snode_list.begin();
       p != snode_list.end(); ++ p) {
    SbjNode* node = *p;

    int min_cost = INT_MAX;
    const Cut* best_cut = NULL;
    const CutList& cut_list = cut_holder.cut_list(node);
    for (CutListIterator p = cut_list.begin();
	 p != cut_list.end(); ++ p) {
      const Cut* cut = *p;

      ymuint ni = cut->ni();

      for (ymuint i = 0; i < ni; ++ i) {
	mReached[i] = false;
      }
      check_path(node, cut);
      
      int cur_cost = 1;
      for (ymuint i = 0; i < ni; ++ i) {
	if ( mReached[i] ) {
	  SbjNode* inode = cut->input(i);
	  if ( inode->n_fanout() == 1 ) {
	    cur_cost += mBestCost[inode->id()];
	  }
	}
      }
      if ( min_cost > cur_cost ) {
	min_cost = cur_cost;
	best_cut = cut;
      }
    }
    maprec.set_cut(node, best_cut);
    mBestCost[node->id()] = min_cost;
  }

  mBestCost.clear();

  // tree フラグを消す．
  for (SbjNodeList::const_iterator p = node_list.begin();
       p != node_list.end(); ++ p) {
    SbjNode* node = *p;
    const SbjEdgeList& fo_list = node->fanout_list();
    for (SbjEdgeList::const_iterator q = fo_list.begin();
	 q != fo_list.end(); ++ q) {
      (*q)->clear_tree();
    }
  }
  return 0;
}

// node から tree フラグをたどって到達可能な入力に印をつける．
void
DFCover::check_path(SbjNode* node,
		    const Cut* cut)
{
  for (ymuint i = 0; i < cut->ni(); ++ i) {
    if ( cut->input(i) == node ) {
      mReached[i] = true;
      return;
    }
  }
  for (ymuint i = 0; i < 2; ++ i) {
    if ( node->fanin_edge(i)->tree() ) {
      check_path(node->fanin(i), cut);
    }
  }
}

END_NAMESPACE_MAGUS_LUTMAP
