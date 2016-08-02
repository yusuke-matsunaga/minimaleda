
/// @file magus/lutmap/WeightCover.cc
/// @brief DAG covering のヒューリスティック
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: WeightCover.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2007 Yusuke Matsunaga
/// All rights reserved.


#if HAVE_CONFIG_H
#include "magus_config.h"
#endif

#include "WeightCover.h"
#include "SbjGraph.h"
#include "Cut.h"
#include "CutHolder.h"
#include "MapRecord.h"


BEGIN_NAMESPACE_MAGUS_LUTMAP

// コンストラクタ
WeightCover::WeightCover(int mode) :
  mMode(mode)
{
}

// デストラクタ
WeightCover::~WeightCover()
{
}

// @brief best cut の記録を行う．
// @param[in] sbjgraph サブジェクトグラフ
// @param[in] cut_holder カットを保持するオブジェクト
// @param[out] maprec マッピング結果を記録するオブジェクト
int
WeightCover::record_cuts(const SbjGraph& sbjgraph,
			 const CutHolder& cut_holder,
			 MapRecord& maprec)
{
  ymuint limit = cut_holder.limit();
  ymuint n = sbjgraph.max_node_id();
  mBestCost.clear();
  mBestCost.resize(n);
  mWeight.resize(limit);

  maprec.init(sbjgraph);
  
  // 入力のコストを設定
  const SbjNodeList& input_list = sbjgraph.input_list();
  for (SbjNodeList::const_iterator p = input_list.begin();
       p != input_list.end(); ++ p) {
    SbjNode* node = *p;
    maprec.set_cut(node, NULL);
    mBestCost[node->id()] = 0.0;
  }

  // 論理ノードのコストを入力側から計算
  vector<SbjNode*> snode_list;
  sbjgraph.sort(snode_list);
  for (vector<SbjNode*>::const_iterator p = snode_list.begin();
       p != snode_list.end(); ++ p) {
    SbjNode* node = *p;

    double min_cost = DBL_MAX;
    if ( !node->is_unselected() ) {
      const Cut* best_cut = NULL;
      const CutList& cut_list = cut_holder.cut_list(node);
      for (CutListIterator p = cut_list.begin();
	   p != cut_list.end(); ++ p) {
	const Cut* cut = *p;
      
	ymuint ni = cut->ni();
	bool ng = false;
	for (ymuint i = 0; i < ni; ++ i) {
	  SbjNode* inode = cut->input(i);
	  if ( mBestCost[inode->id()] == DBL_MAX ) {
	    ng = true;
	    break;
	  }
	}
	if ( ng ) continue;
	if ( cut->fo_check() ) continue;

	if ( mMode & 1 ) {
	  // ファンアウトモード
	  for (ymuint i = 0; i < ni; ++ i) {
	    SbjNode* inode = cut->input(i);
	    mWeight[i] = 1.0 / inode->n_fanout();
	  }
	}
	else {
	  // フローモード
	  for (ymuint i = 0; i < ni; ++ i) {
	    mWeight[i] = 0.0;
	  }
	  calc_weight(node, cut, 1.0);
	}
      
	double cur_cost = 1.0;
	for (ymuint i = 0; i < ni; ++ i) {
	  SbjNode* inode = cut->input(i);
	  cur_cost += mBestCost[inode->id()] * mWeight[i];
	}
	if ( min_cost > cur_cost ) {
	  min_cost = cur_cost;
	  best_cut = cut;
	}
      }
      //assert_cond(min_cost < DBL_MAX, __FILE__, __LINE__);
      // unselected の状態によっては解がない場合もある．
      if ( best_cut ) {
	maprec.set_cut(node, best_cut);
      }
    }
    mBestCost[node->id()] = min_cost;
  }
    
  double cost = 0.0;
  const SbjNodeList& node_list = sbjgraph.lnode_list();
  for (SbjNodeList::const_iterator p = node_list.begin();
       p != node_list.end(); ++ p) {
    SbjNode* node = *p;
    if ( node->pomark() || node->is_fo() ) {
      if ( mBestCost[node->id()] == DBL_MAX ) {
	return -1;
      }
      cost += mBestCost[node->id()];
    }
  }
  mBestCost.clear();
  int lb = static_cast<int>(ceil(cost));

  if ( mMode & 2 ) {
    // cut resubstitution
    mCutResub(sbjgraph, cut_holder, maprec);
  }

  return lb;
}

// node から各入力にいたる経路の重みを計算する．
void
WeightCover::calc_weight(SbjNode* node,
			 const Cut* cut,
			 double cur_weight)
{
  for ( ; ; ) {
    for (ymuint i = 0; i < cut->ni(); ++ i) {
      if ( cut->input(i) == node ) {
	// node は cut の葉だった．
	if  ( !node->pomark() && !node->is_fo() ) {
	  mWeight[i] += cur_weight;
	}
	return;
      }
    }
    SbjNode* inode0 = node->fanin(0);
    double cur_weight0 = cur_weight / inode0->n_efo();
    calc_weight(inode0, cut, cur_weight0);
    node = node->fanin(1);
    cur_weight /= node->n_efo();
  }
}

END_NAMESPACE_MAGUS_LUTMAP
