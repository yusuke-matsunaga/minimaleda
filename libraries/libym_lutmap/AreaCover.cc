
/// @file magus/lutmap/WeightCover.cc
/// @brief DAG covering のヒューリスティック
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: WeightCover.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "AreaCover.h"
#include "ym_sbj/SbjGraph.h"
#include "Cut.h"
#include "MapRecord.h"


BEGIN_NAMESPACE_YM_LUTMAP

// コンストラクタ
AreaCover::AreaCover()
{
}

// デストラクタ
AreaCover::~AreaCover()
{
}

// @brief 面積最小化マッピングを行う．
// @param[in] sbjgraph サブジェクトグラフ
// @param[in] limit LUT の入力数
// @param[in] mode モード
//  - 0: fanout フロー, resub なし
//  - 1: weighted フロー, resub なし
//  - 2: fanout フロー, resub あり
//  - 3: weighted フロー, resub あり
// @param[out] mapnetwork マッピング結果
// @param[out] lut_num LUT数
// @param[out] depth 段数
void
AreaCover::operator()(const SbjGraph& sbjgraph,
		      ymuint limit,
		      ymuint mode,
		      LnGraph& mapnetwork,
		      ymuint& lut_num,
		      ymuint& depth)
{
  mMode = mode;

  // カットを列挙する．
  mCutHolder.enum_cut(sbjgraph, limit);

  // 最良カットを記録する．
  MapRecord maprec;
  record_cuts(sbjgraph, limit, maprec);

  if ( mode & 2 ) {
    // cut resubstituion
    mCutResub(sbjgraph, mCutHolder, maprec);
  }

  // 最終的なネットワークを生成する．
  maprec.gen_mapgraph(sbjgraph, mapnetwork, lut_num, depth);
}

// @brief best cut の記録を行う．
// @param[in] sbjgraph サブジェクトグラフ
// @param[in] limit LUT の入力数
// @param[out] maprec マッピング結果を記録するオブジェクト
void
AreaCover::record_cuts(const SbjGraph& sbjgraph,
		       ymuint limit,
		       MapRecord& maprec)
{
  ymuint n = sbjgraph.max_node_id();
  mBestCost.clear();
  mBestCost.resize(n);
  mWeight.resize(limit);

  maprec.init(sbjgraph);

  // 入力のコストを設定
  const SbjNodeList& input_list = sbjgraph.input_list();
  for (SbjNodeList::const_iterator p = input_list.begin();
       p != input_list.end(); ++ p) {
    const SbjNode* node = *p;
    maprec.set_cut(node, NULL);
    mBestCost[node->id()] = 0.0;
  }

  // 論理ノードのコストを入力側から計算
  vector<const SbjNode*> snode_list;
  sbjgraph.sort(snode_list);
  for (vector<const SbjNode*>::const_iterator p = snode_list.begin();
       p != snode_list.end(); ++ p) {
    const SbjNode* node = *p;

    double min_cost = DBL_MAX;
    const Cut* best_cut = NULL;
    const CutList& cut_list = mCutHolder.cut_list(node);
    for (CutListIterator p = cut_list.begin();
	 p != cut_list.end(); ++ p) {
      const Cut* cut = *p;

      ymuint ni = cut->ni();
      bool ng = false;
      for (ymuint i = 0; i < ni; ++ i) {
	const SbjNode* inode = cut->input(i);
	if ( mBestCost[inode->id()] == DBL_MAX ) {
	  ng = true;
	  break;
	}
      }
      if ( ng ) continue;

      if ( mMode & 1 ) {
	// ファンアウトモード
	for (ymuint i = 0; i < ni; ++ i) {
	  const SbjNode* inode = cut->input(i);
	  mWeight[i] = 1.0 / inode->fanout_num();
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
	const SbjNode* inode = cut->input(i);
	cur_cost += mBestCost[inode->id()] * mWeight[i];
      }
      if ( min_cost > cur_cost ) {
	min_cost = cur_cost;
	best_cut = cut;
      }
    }
    assert_cond(min_cost < DBL_MAX, __FILE__, __LINE__);
    assert_cond( best_cut != NULL, __FILE__, __LINE__);
    maprec.set_cut(node, best_cut);
    mBestCost[node->id()] = min_cost;
  }
}

// node から各入力にいたる経路の重みを計算する．
void
AreaCover::calc_weight(const SbjNode* node,
		       const Cut* cut,
		       double cur_weight)
{
  for ( ; ; ) {
    for (ymuint i = 0; i < cut->ni(); ++ i) {
      if ( cut->input(i) == node ) {
	// node は cut の葉だった．
	if  ( !node->pomark() ) {
	  mWeight[i] += cur_weight;
	}
	return;
      }
    }
    const SbjNode* inode0 = node->fanin(0);
    double cur_weight0 = cur_weight / inode0->fanout_num();
    calc_weight(inode0, cut, cur_weight0);
    node = node->fanin(1);
    cur_weight /= node->fanout_num();
  }
}

// @brief 面積最小化 DAG covering のヒューリスティック関数
// @param[in] sbjgraph サブジェクトグラフ
// @param[in] limit カットサイズ
// @param[in] mode モード
//  - 0: fanout フロー, resub なし
//  - 1: weighted フロー, resub なし
//  - 2: fanout フロー, resub あり
//  - 3: weighted フロー, resub あり
// @param[out] mapnetwork マッピング結果
// @param[out] lut_num LUT数
// @param[out] depth 段数
void
area_map(const SbjGraph& sbjgraph,
	 ymuint limit,
	 ymuint mode,
	 LnGraph& mapnetwork,
	 ymuint& lut_num,
	 ymuint& depth)
{
  AreaCover area_cover;

  area_cover(sbjgraph, limit, mode, mapnetwork, lut_num, depth);
}

END_NAMESPACE_YM_LUTMAP
