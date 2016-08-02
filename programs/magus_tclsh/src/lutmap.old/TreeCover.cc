
/// @file magus/lutmap/TreeCover.cc
/// @brief tree covering を行うクラス
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: TreeCover.cc 2274 2009-06-10 07:45:29Z matsunaga $
///
/// Copyright (C) 2005-2007 Yusuke Matsunaga
/// All rights reserved.


#if HAVE_CONFIG_H
#include "magus_config.h"
#endif

#include "TreeCover.h"
#include "SbjGraph.h"
#include "Cut.h"
#include "CutHolder.h"
#include "MapRecord.h"


BEGIN_NAMESPACE_MAGUS_LUTMAP

//////////////////////////////////////////////////////////////////////
// クラス TreeCover
//////////////////////////////////////////////////////////////////////

// コンストラクタ
TreeCover::TreeCover()
{
}

// デストラクタ
TreeCover::~TreeCover()
{
}

// @brief best cut の記録を行う．
// @param[in] sbjgraph サブジェクトグラフ
// @param[in] cut_holder カットを保持するオブジェクト
// @param[out] maprec マッピング結果を記録するオブジェクト
int
TreeCover::record_cuts(const SbjGraph& sbjgraph,
		       const CutHolder& cut_holder,
		       MapRecord& maprec)
{
  ymuint n = sbjgraph.max_node_id();
  mBestCost.clear();
  mBestCost.resize(n);

  maprec.init(sbjgraph);
  
  // 入力のコストを設定
  const SbjNodeList& input_list = sbjgraph.input_list();
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

      int cur_cost = 1;
      for (ymuint i = 0; i < ni; ++ i) {
	SbjNode* inode = cut->input(i);
	if ( inode->n_fanout() == 1 ) {
	  cur_cost += mBestCost[inode->id()];
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
  return 0;
}

END_NAMESPACE_MAGUS_LUTMAP
