#ifndef MAGUS_LUTMAP_KFNMAPPERX_H
#define MAGUS_LUTMAP_KFNMAPPERX_H

/// @file magus/lutmap/KfnMapperX.h
/// @brief KfnMapperX のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: KfnMapperX.h 1077 2007-12-03 10:18:47Z matsunaga $
///
/// Copyright (C) 2005-2006 Yusuke Matsunaga
/// All rights reserved.

#include "Cut.h"
#include "MapRecord.h"


BEGIN_NAMESPACE_MAGUS_LUTMAP

class SbjGraph;
class SbjNode;
class DagACover;

//////////////////////////////////////////////////////////////////////
/// @class KfnMapperX KfnMapperX.h "KfnMapperX.h"
/// @brief DAG covering の解を表すクラス
//////////////////////////////////////////////////////////////////////
class KfnMapperX
{
public:

  /// @brief コンストラクタ
  KfnMapperX();

  /// @brief デストラクタ
  virtual
  ~KfnMapperX();


public:
  
  /// @brief マッピング結果の上限と下限を求める．
  /// @param[in] cut_holder サブジェクトグラフ上のカット集合
  /// @param[in] dc Dag Covering を実行するオブジェクト
  /// @param[out] lower_bound 下限値
  /// @return LUT 数を返す．
  virtual
  int
  estimate(const SbjGraph& sbjgraph,
	   const CutHolder& cut_holder,
	   DagACover* dc,
	   int& lower_bound);

  /// @brief node を根とするカットが選ばれているか調べる．
  /// @param[in] node 対象のノード
  /// @return node を根とするカットが選ばれている時そのカットを返す．
  /// 選ばれていなければ NULL を返す．
  /// @note map/count の後に呼ぶ．
  virtual
  const Cut*
  selected_cut(SbjNode* node) const;

  /// @brief node をカバーしているカットのリストを求める．
  /// @param[in] node 対象のノード
  /// @return node をカバーしているカットのリスト
  virtual
  const list<const Cut*>&
  cover_list(SbjNode* node) const;

private:
  //////////////////////////////////////////////////////////////////////
  // 内部でのみ用いられる関数
  //////////////////////////////////////////////////////////////////////

  // node の最適カットを選ぶ．
  void
  back_trace1(SbjNode* node);

  // ノードの生成を行わずに数だけ数えるバージョンのバックトレース
  size_t
  count_node(SbjNode* node);

  // カットのカバーしているノードに印をつける．
  void
  mark_cover(SbjNode* node,
	     const Cut* cut);

  // mark_cover で付けた印を消す．
  void
  clear_mark(SbjNode* node);


private:

  // マッピングに用いるノードごとの情報
  struct NodeInfo
  {
    // 空のコンストラクタ
    NodeInfo() :
      mCurCut(NULL),
      mVisited(false),
      mTmpFlag(false)
    {
    }
    
    // このノードを根とする最適カット
    const Cut* mCurCut;
    
    // count_node で用いるフラグ
    bool mVisited;

    // このノードをカバーしているカットのリスト
    list<const Cut*> mCover;

    bool mTmpFlag;

  };
  

private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  MapRecord mMapRec;

  // 各ノードごとの情報を納める配列
  vector<NodeInfo> mNodeInfo;
  
};

END_NAMESPACE_MAGUS_LUTMAP

#endif // MAGUS_LUTMAP_KFNCOVERX_H
