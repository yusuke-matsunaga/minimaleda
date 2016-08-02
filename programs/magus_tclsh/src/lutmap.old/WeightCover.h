#ifndef MAGUS_LUTMAP_WEIGHTCOVER_H
#define MAGUS_LUTMAP_WEIGHTCOVER_H

/// @file magus/lutmap/WeightCover.h
/// @brief DAG covering のヒューリスティック
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: WeightCover.h 1077 2007-12-03 10:18:47Z matsunaga $
///
/// Copyright (C) 2005-2007 Yusuke Matsunaga
/// All rights reserved.

#include "DagACover.h"
#include "CutResub.h"


BEGIN_NAMESPACE_MAGUS_LUTMAP

class SbjNode;
class Cut;

//////////////////////////////////////////////////////////////////////
// DAG covering のヒューリスティック
//////////////////////////////////////////////////////////////////////
class WeightCover :
  public DagACover
{
public:

  /// @brief コンストラクタ
  /// @param[in] mode 重みの計算方法
  ///  - 1: 根の出力のファンアウト数で割る．
  ///  - 2: 入力から根の出力までのファンアウト数で割る．
  WeightCover(int mode = 2);

  /// @brief デストラクタ
  virtual
  ~WeightCover();


public:

  /// @brief best cut の記録を行う．
  /// @param[in] sbjgraph サブジェクトグラフ
  /// @param[in] cut_holder カットを保持するオブジェクト
  /// @param[out] maprec マッピング結果を記録するオブジェクト
  virtual
  int
  record_cuts(const SbjGraph& sbjgraph,
	      const CutHolder& cut_holder,
	      MapRecord& maprec);

  
private:

  // node から各入力にいたる経路の重みを計算する．
  void
  calc_weight(SbjNode* node,
	      const Cut* cut,
	      double cur_weight);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // cut resubstitution を実行するファンクター
  CutResub mCutResub;
  
  // 各ノードのベスト値を記録する配列．
  vector<double> mBestCost;

  // 各入力から根の出力に抜ける経路上の重みを入れる配列
  vector<double> mWeight;

  // モード
  int mMode;
  
};

END_NAMESPACE_MAGUS_LUTMAP

#endif // MAGUS_LUTMAP_WEIGHTCOVER_H
