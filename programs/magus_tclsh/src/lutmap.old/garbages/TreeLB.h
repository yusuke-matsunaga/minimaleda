#ifndef MAGUS_LUTMAP_TREELB_H
#define MAGUS_LUTMAP_TREELB_H

/// @file magus/lutmap/TreeLB.h
/// @brief TreeLB のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: TreeLB.h 1077 2007-12-03 10:18:47Z matsunaga $
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
class TreeLB :
  public LowerBound
{
public:

  // コンストラクタ
  TreeLB(size_t n);

  // デストラクタ
  virtual
  ~TreeLB();

  
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

  // node から tree フラグをたどって到達可能な入力に印をつける．
  bool
  check_path(SbjNode* node,
	     const Cut* cut,
	     bool reached);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  size_t mN;
  
  vector<int> mBestCost;

  vector<bool> mReached;
  
};

END_NAMESPACE_MAGUS_LUTMAP

#endif // MAGUS_LUTMAP_TREELB_H
