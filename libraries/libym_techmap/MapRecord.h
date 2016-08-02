#ifndef LIBYM_TECHMAP_MAPRECORD_H
#define LIBYM_TECHMAP_MAPRECORD_H

/// @file libym_techmap/MapRecord.h
/// @brief MapRecord のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: MapRecord.h 1293 2008-02-21 02:25:52Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_techmap/techmap_nsdef.h"
#include "ym_cell/cell_nsdef.h"
#include "Match.h"


BEGIN_NAMESPACE_YM_TECHMAP

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

  /// @brief マッチの情報だけコピーする．
  void
  copy(const MapRecord& src);

  /// @brief マッチを記録する．
  /// @param[in] node 該当のノード
  /// @param[in] inv 極性
  /// @param[in] match 対応するマッチ
  /// @param[in] cell セル
  void
  set_match(const SbjNode* node,
	    bool inv,
	    const Match& match,
	    const Cell* cell);

  /// @brief インバータのマッチを記録する．
  /// @param[in] node 該当のノード
  /// @param[in] inv 極性
  /// @param[in] cell セル
  void
  set_inv_match(const SbjNode* node,
		bool inv,
		const Cell* cell);

  /// @brief マッチを取り出す．
  /// @param[in] node 該当のノード
  /// @param[in] inv
  const Match&
  get_match(const SbjNode* node,
	    bool inv);

  /// @brief マッピング結果を CnGraph にセットする．
  /// @param[in] sbjgraph サブジェクトグラフ
  /// @param[in] const0_cell 定数0のセル
  /// @param[in] const1_cell 定数1のセル
  /// @param[out] mapgraph マッピング結果を格納するネットワーク
  void
  gen_mapgraph(const SbjGraph& sbjgraph,
	       const Cell* const0_cell,
	       const Cell* const1_cell,
	       CnGraph& mapgraph);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部でのみ用いられる関数
  //////////////////////////////////////////////////////////////////////

  // 最終結果を作るためのバックトレースを行う．
  CnNode*
  back_trace(const SbjNode* node,
	     bool inv,
	     CnGraph& mapnetwork);


private:

  // 内部で使われるデータ構造
  struct NodeInfo
  {
    NodeInfo()
    {
      mCell = NULL;
      mMapNode = NULL;
    }

    // マッチ
    Match mMatch;

    // セル
    const Cell* mCell;

    // マップ結果
    CnNode* mMapNode;

  };


private:

  /// @brief NodeInfo を取り出す．
  NodeInfo&
  node_info(const SbjNode* node,
	    bool inv);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 各ノードの極性ごと作業領域を格納した配列
  vector<NodeInfo> mNodeInfo;

  // back_trace 中に用いる作業領域
  vector<CnNode*> mTmpFanins;

};

END_NAMESPACE_YM_TECHMAP

#endif // LIBYM_TECHMAP_MAPRECORD_H
