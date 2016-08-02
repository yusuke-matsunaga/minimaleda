#ifndef LIBYM_TECHMAP_AREACOVER_H
#define LIBYM_TECHMAP_AREACOVER_H

/// @file libym_techmap/AreaCover.h
/// @brief AreaCover のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_techmap/techmap_nsdef.h"
#include "ym_sbj/SbjGraph.h"


BEGIN_NAMESPACE_YM_TECHMAP

class PatMgr;
class FuncGroup;
class MapRecord;

//////////////////////////////////////////////////////////////////////
/// @class AreaCover AreaCover.h "AreaCover.h"
/// @brief 面積モードの DAG covering のヒューリスティック
//////////////////////////////////////////////////////////////////////
class AreaCover
{
public:

  /// @brief コンストラクタ
  AreaCover();

  /// @brief デストラクタ
  ~AreaCover();


public:

  /// @brief 面積最小化マッピングを行う．
  /// @param[in] sbjgraph サブジェクトグラフ
  /// @param[in] pat_mgr パタングラフを管理するオブジェクト
  /// @param[out] mapnetwork マッピング結果
  void
  operator()(const SbjGraph& sbjgraph,
	     const PatMgr& patmgr,
	     CnGraph& mapnetwork);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief best cut の記録を行う．
  /// @param[in] sbjgraph サブジェクトグラフ
  /// @param[in] pat_mgr パタングラフを管理するオブジェクト
  /// @param[in] maprec マッピング結果を保持するオブジェクト
  void
  record_cuts(const SbjGraph& sbjgraph,
	      const PatMgr& patmgr,
	      MapRecord& maprec);

  /// @brief 逆極性の解にインバーターを付加した解を追加する．
  /// @param[in] node 対象のノード
  /// @param[in] inv 極性
  /// @param[in] inv_func インバータの関数グループ
  /// @param[in] maprec マッピング結果を保持するオブジェクト
  void
  add_inv(const SbjNode* node,
	  bool inv,
	  const FuncGroup& inv_func,
	  MapRecord& maprec);

  /// @brief node から各入力にいたる経路の重みを計算する．
  /// @param[in] node 対象のノード
  /// @param[in] cur_weight
  void
  calc_weight(const SbjNode* node,
	      double cur_weight);

  /// @brief (node, inv) に対応するコストを取り出す．
  double&
  cost(const SbjNode* node,
       bool inv);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 各ノードのコストを保持する配列
  vector<double> mCostArray;

  // 各入力から根の出力に抜ける経路上の重みを入れる配列
  vector<double> mWeight;

  // calc_weight で用いる作業領域
  vector<int> mLeafNum;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief (node, inv) に対応する NodeInfo を取り出す．
inline
double&
AreaCover::cost(const SbjNode* node,
		bool inv)
{
  return mCostArray[node->id() * 2 + static_cast<ymuint>(inv)];
}

END_NAMESPACE_YM_TECHMAP

#endif // LIBYM_TECHMAP_AREACOVER_H
