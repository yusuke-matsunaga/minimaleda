#ifndef LIBYM_LUTMAP_AREACOVER_H
#define LIBYM_LUTMAP_AREACOVER_H

/// @file magus/lutmap/WeightCover.h
/// @brief DAG covering のヒューリスティック
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: WeightCover.h 1077 2007-12-03 10:18:47Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_lutmap/lutmap_nsdef.h"
#include "ym_sbj/sbj_nsdef.h"
#include "CutHolder.h"
#include "CutResub.h"


BEGIN_NAMESPACE_YM_LUTMAP

class MapRecord;

//////////////////////////////////////////////////////////////////////
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
  /// @param[in] limit LUT の入力数
  /// @param[in] mode モード
  ///  - 0: fanout フロー, resub なし
  ///  - 1: weighted フロー, resub なし
  ///  - 2: fanout フロー, resub あり
  ///  - 3: weighted フロー, resub あり
  /// @param[out] mapnetwork マッピング結果
  /// @param[out] lut_num LUT数
  /// @param[out] depth 段数
  void
  operator()(const SbjGraph& sbjgraph,
	     ymuint limit,
	     ymuint mode,
	     LnGraph& mapnetwork,
	     ymuint& lut_num,
	     ymuint& depth);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief best cut の記録を行う．
  /// @param[in] sbjgraph サブジェクトグラフ
  /// @param[in] limit LUT の入力数
  /// @param[out] maprec マッピング結果を記録するオブジェクト
  void
  record_cuts(const SbjGraph& sbjgraph,
	      ymuint limit,
	      MapRecord& maprec);

  // node から各入力にいたる経路の重みを計算する．
  void
  calc_weight(const SbjNode* node,
	      const Cut* cut,
	      double cur_weight);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // モード
  ymuint mMode;

  // カットを保持するオブジェクト
  CutHolder mCutHolder;

  // cut resubstitution を実行するファンクター
  CutResub mCutResub;

  // 各ノードのベスト値を記録する配列．
  vector<double> mBestCost;

  // 各入力から根の出力に抜ける経路上の重みを入れる配列
  vector<double> mWeight;

};

END_NAMESPACE_YM_LUTMAP

#endif // LIBYM_LUTMAP_AREACOVER_H
