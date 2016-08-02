
/// @file magus/lutmap/KfnMapperX.cc
/// @brief KfnMapperX の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: KfnMapperX.cc 1077 2007-12-03 10:18:47Z matsunaga $
///
/// Copyright (C) 2005-2007 Yusuke Matsunaga
/// All rights reserved.


#if HAVE_CONFIG_H
#include <magus_config.h>
#endif

#include "KfnMapperX.h"
#include "SbjGraph.h"
#include "Cut.h"
#include "CutHolder.h"
#include "DagACover.h"


BEGIN_NAMESPACE_MAGUS_LUTMAP

// コンストラクタ
KfnMapperX::KfnMapperX()
{
}

// デストラクタ
KfnMapperX::~KfnMapperX()
{
}

// @brief セットされたカットの基づいてマッピングを行う．
// @param[in] cut_holder サブジェクトグラフ上のカット集合
// @param[in] dc Dag Covering を実行するオブジェクト
// @param[out] lower_bound 下限値
// @return LUT 数を返す．
int
KfnMapperX::estimate(const SbjGraph& sbjgraph,
		     const CutHolder& cut_holder,
		     DagACover* dc,
		     int& lower_bound)
{
  dc->record_cuts(sbjgraph, cut_holder, mMapRec);
  
  mNodeInfo.resize(sbjgraph.max_node_id());

  // 外部出力からバックトレースを行う．
  const SbjNodeList& output_list = sbjgraph.output_list();
  for (SbjNodeList::const_iterator p = output_list.begin();
       p != output_list.end(); ++ p) {
    SbjNode* onode = *p;
    SbjNode* node = onode->fanin(0);
    if ( node ) {
      back_trace1(node);
    }
  }

#if 0
  // カットの置き換えによる最少化
  cut_resub(sbjgraph, cut_holder);
#endif
  
  int count = 0;

  // 外部入力を訪問済みにする．
  const SbjNodeList& input_list = sbjgraph.input_list();
  for (SbjNodeList::const_iterator p = input_list.begin();
       p != input_list.end(); ++ p) {
    SbjNode* node = *p;
    mNodeInfo[node->id()].mVisited = true;
  }

  // 外部出力が必要とする要素数を数える．
  for (SbjNodeList::const_iterator p = output_list.begin();
       p != output_list.end(); ++ p) {
    SbjNode* onode = *p;
    SbjNode* node = onode->fanin(0);
    if ( node ) {
      count += count_node(node);
    }
  }
  return count;
}

// node の最適カットを選ぶ．
void
KfnMapperX::back_trace1(SbjNode* node)
{
  NodeInfo& node_info = mNodeInfo[node->id()];
  if ( node_info.mCurCut == NULL ) {
    const Cut* cut = mMapRec.get_cut(node);
    node_info.mCurCut = cut;

    // ファンインのノードのカットを選ぶ．
    // 同時にファンアウトリストを構築する．
    for (size_t i = 0; i < cut->ni(); ++ i) {
      SbjNode* inode = cut->input(i);
      if ( !inode->is_input() ){
	back_trace1(inode);
      }
    }
  }
}

// @brief node を根とするカットが選ばれているか調べる．
// @param[in] node 対象のノード
// @return node を根とするカットが選ばれている時そのカットを返す．
// 選ばれていなければ NULL を返す．
// @note map/count の後に呼ぶ．
const Cut*
KfnMapperX::selected_cut(SbjNode* node) const
{
  const NodeInfo& node_info = mNodeInfo[node->id()];
  return node_info.mCurCut;
}

// @brief node をカバーしているカットのリストを求める．
// @param[in] node 対象のノード
// @return node をカバーしているカットのリスト
const list<const Cut*>&
KfnMapperX::cover_list(SbjNode* node) const
{
  return mNodeInfo[node->id()].mCover;
}

// ノードの生成を行わずに数だけ数えるバージョンのバックトレース
size_t
KfnMapperX::count_node(SbjNode* node)
{
  size_t id = node->id();
  NodeInfo& node_info = mNodeInfo[id];
  if ( node_info.mVisited ) {
    // すでに生成済み
    return 0;
  }

  // node を根とするカットを取り出す．
  const Cut* cut = node_info.mCurCut;

  mark_cover(node, cut);
  clear_mark(node);
  
  // その入力に対応するノードを再帰的に生成する．
  size_t count = 1;
  size_t ni = cut->ni();
  for (size_t i = 0; i < ni; ++ i) {
    SbjNode* inode = cut->input(i);
    count += count_node(inode);
  }

  node_info.mVisited = true;
  
  return count;
}

// カットのカバーしているノードに印をつける．
void
KfnMapperX::mark_cover(SbjNode* node,
		       const Cut* cut)
{
  for (size_t i = 0; i < cut->ni(); ++ i) {
    if ( cut->input(i) == node ) {
      return;
    }
  }

  NodeInfo& node_info = mNodeInfo[node->id()];
  if ( !node_info.mTmpFlag ) {
    node_info.mTmpFlag = true;
    node_info.mCover.push_back(cut);
  }

  mark_cover(node->fanin(0), cut);
  mark_cover(node->fanin(1), cut);
}

// mark_cover で付けた印を消す．
void
KfnMapperX::clear_mark(SbjNode* node)
{
  NodeInfo& node_info = mNodeInfo[node->id()];
  if ( node_info.mTmpFlag ) {
    node_info.mTmpFlag = false;
    clear_mark(node->fanin(0));
    clear_mark(node->fanin(1));
  }
}

END_NAMESPACE_MAGUS_LUTMAP
