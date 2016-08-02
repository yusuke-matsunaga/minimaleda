#ifndef MAGUS_LUTMAP_WEIGHTLBD_H
#define MAGUS_LUTMAP_WEIGHTLBD_H

/// @file magus/lutmap/WEIGHTLBD.h
/// @brief WeightLBD のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: WeightLBD.h 1077 2007-12-03 10:18:47Z matsunaga $
///
/// Copyright (C) 2005-2007 Yusuke Matsunaga
/// All rights reserved.

#include "lutmap.h"
#include "ADCost.h"

BEGIN_NAMESPACE_MAGUS_LUTMAP

class SbjGraph;
class SbjNode;
class Cut;
class CutHolder;


//////////////////////////////////////////////////////////////////////
// lower bound with depth constraint を求めるためのクラス
//////////////////////////////////////////////////////////////////////
class WeightLBD
{
public:

  /// @brief コンストラクタ
  WeightLBD();

  /// @brief デストラクタ
  ~WeightLBD();


public:

  /// @brief depth 制約下での lower bound を求める．
  /// @param[in] cut_holder カットを保持するオブジェクト
  /// @param[in] slack 最小段数に対するスラック
  /// @return covering の結果の LUT 数
  int
  operator()(const SbjGraph& sbjgraph,
	     const CutHolder& cut_holder,
	     size_t slack);


private:
  
  // node のカットを記録する．
  void
  record(SbjNode* node,
	 const CutHolder& cut_holder);
  
  // node のカットを選択する．
  double
  count(SbjNode* node,
	int req);

  // node から各入力にいたる経路の重みを計算する．
  void
  calc_weight(SbjNode* node,
	      const Cut* cut,
	      double cur_weight);


private:

  struct NodeInfo {

    // コンストラクタ
    NodeInfo() :
      mMinDepth(0)
    {
    }

    ADCostList<double> mCostList;
    int mMinDepth;
  };
  
  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////
  
  vector<NodeInfo> mNodeInfo;

  vector<double> mWeight;

  ADCostMgr<double> mCostMgr;
  
};

END_NAMESPACE_MAGUS_LUTMAP

#endif // MAGUS_LUTMAP_WEIGHTLBD_H
