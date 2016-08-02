#ifndef MAGUS_LUTMAP_TREELBD_H
#define MAGUS_LUTMAP_TREELBD_H

/// @file magus/lutmap/TreeLBD.h
/// @brief TreeLBD のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: TreeLBD.h 1077 2007-12-03 10:18:47Z matsunaga $
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
class TreeLBD
{
public:

  /// @brief コンストラクタ
  TreeLBD(size_t n);

  /// @brief デストラクタ
  ~TreeLBD();


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
  int
  count(SbjNode* node,
	int req);

  // node から tree フラグをたどって到達可能な入力に印をつける．
  bool
  check_path(SbjNode* node,
	     const Cut* cut,
	     bool reached);


private:

  struct NodeInfo {

    // コンストラクタ
    NodeInfo() :
      mMinDepth(0)
    {
    }

    ADCostList<int> mCostList;
    int mMinDepth;
  };
  
  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  size_t mN;
  
  vector<NodeInfo> mNodeInfo;

  vector<bool> mReached;

  ADCostMgr<int> mCostMgr;
  
};

END_NAMESPACE_MAGUS_LUTMAP

#endif // MAGUS_LUTMAP_TREELBD_H
