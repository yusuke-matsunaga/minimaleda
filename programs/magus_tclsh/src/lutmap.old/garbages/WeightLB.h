#ifndef MAGUS_LUTMAP_WEIGHTLB_H
#define MAGUS_LUTMAP_WEIGHTLB_H

/// @file magus/lutmap/LB2.h
/// @brief LB2 のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: WeightLB.h 1077 2007-12-03 10:18:47Z matsunaga $
///
/// Copyright (C) 2005-2007 Yusuke Matsunaga
/// All rights reserved.

#include "LowerBound.h"


BEGIN_NAMESPACE_MAGUS_LUTMAP

class SbjNode;
class Cut;

//////////////////////////////////////////////////////////////////////
// DAG covering の下限を計算するためのクラス
//////////////////////////////////////////////////////////////////////
class WeightLB :
  public LowerBound
{
public:

  // コンストラクタ
  WeightLB();

  // デストラクタ
  virtual
  ~WeightLB();

  
public:

  // 下限を計算する．
  virtual
  int
  calc(const SbjGraph& sbjgraph,
       const CutHolder& cut_holder);


private:

  // node のコストを計算する．
  void
  count(SbjNode* node,
	const CutHolder& cut_holder);
  
  // node から各入力にいたる経路の重みを計算する．
  void
  calc_weight(SbjNode* node,
	      const Cut* cut,
	      double cur_weight);

  // selected ノードをカットの内部に含んでいたら true を返す．
  static
  bool
  check_nodes(SbjNode* node,
	      const Cut* cut);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////
  
  vector<double> mBestCost;

  vector<double> mWeight;
  
};

END_NAMESPACE_MAGUS_LUTMAP

#endif // MAGUS_LUTMAP_WEIGHTLB_H
