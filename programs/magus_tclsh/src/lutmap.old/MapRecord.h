#ifndef MAGUS_LUTMAP_MAP_H
#define MAGUS_LUTMAP_MAP_H

/// @file magus/lutmap/MapRecord.h
/// @brief MapRecord のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: MapRecord.h 1293 2008-02-21 02:25:52Z matsunaga $
///
/// Copyright (C) 2005-2006 Yusuke Matsunaga
/// All rights reserved.

#include "lutmap.h"


BEGIN_NAMESPACE_MAGUS_LUTMAP

class SbjGraph;
class SbjNode;
class Cut;
class LnGraph;
class LnNode;


//////////////////////////////////////////////////////////////////////
/// @class MapRecord MapRecord.h "MapRecord.h"
/// @brief マッピングの解を保持するクラス
//////////////////////////////////////////////////////////////////////
class MapRecord
{
public:

  /// @brief コンストラクタ
  MapRecord();

  /// @brief デストラクタ
  virtual
  ~MapRecord();


public:

  /// @brief @brief 作業領域を初期化する．
  /// @param[in] sbjgraph サブジェクトグラフ
  void
  init(const SbjGraph& sbjgraph);

  /// @brief カットの情報だけコピーする．
  void
  copy(const MapRecord& src);

  /// @brief カットを記録する．
  /// @param[in] node 該当のノード
  /// @param[in] cut 対応するカット
  void
  set_cut(SbjNode* node,
	  const Cut* cut);

  /// @brief カットを取り出す．
  /// @param[in] node 該当のノード
  const Cut*
  get_cut(SbjNode* node);
  
  /// @brief マッピング結果を LnGraph にセットする．
  /// @param[in] sbjgraph サブジェクトグラフ
  /// @param[in] dag_cover DAG covering 結果
  /// @param[out] mapgraph マッピング結果を格納するネットワーク
  /// @param[out] lut_num LUT数
  /// @param[out] depth 最大段数
  void
  gen_mapgraph(const SbjGraph& sbjgraph,
	       LnGraph& mapgraph,
	       int& lut_num,
	       int& depth);

  /// @brief マッピング結果の LUT 数を数える．
  int
  estimate(const SbjGraph& sbjgraph);

  /// @brief 直前の estimate の結果 node が fanout node なら true を返す．
  bool
  check_fonode(SbjNode* node);

  /// @brief 直前の estimate の結果で node のカバーされている回数を返す．
  int
  cover_count(SbjNode* node);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部でのみ用いられる関数
  //////////////////////////////////////////////////////////////////////

  // 最終結果を作るためのバックトレースを行う．
  LnNode*
  back_trace(SbjNode* node,
	     bool inv,
	     LnGraph& mapnetwork,
	     const string& oname);

  // estimate() で用いるバックトレース
  int
  back_trace2(SbjNode* node,
	      bool inv);

  // cut でカバーされるノードの mCovCount を一つ増やす．
  void
  mark_cover(SbjNode* node,
	     const Cut* cut);

  // mark_cover でつけた mTmpFlag を下ろす．
  void
  clear_mark(SbjNode* node);


private:

  // 内部で使われるデータ構造
  struct NodeInfo
  {
    NodeInfo()
    {
      mCut = NULL;
      mMapNode[0] = NULL;
      mMapNode[1] = NULL;
      mMapCount[0] = 0;
      mMapCount[1] = 0;
      mDepth = 0;
    }

    // カット
    const Cut* mCut;
    
    // マップ結果
    LnNode* mMapNode[2];

    // estimate で用いるカウンタ
    int mMapCount[2];

    // estimate でカットにカバーされている回数を保持するカウンタ
    int mCovCount;

    // mCovCount の更新で用いるフラグ
    bool mTmpFlag;
    
    // 段数
    int mDepth;
  };

  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 各ノードごと作業領域を格納した配列
  vector<NodeInfo> mNodeInfo;

  // back_trace 中に用いる作業領域
  vector<LnNode*> mTmpFanins;
  
};

END_NAMESPACE_MAGUS_LUTMAP

#endif // MAGUS_LUTMAP_MAPPERBASE_H
