
/// @file magus/lutmap/XLB.cc
/// @brief eXact DAG covering algorithm
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: XLB.cc 1077 2007-12-03 10:18:47Z matsunaga $
///
/// Copyright (C) 2005-2007 Yusuke Matsunaga
/// All rights reserved.


#if HAVE_CONFIG_H
#include <magus_config.h>
#endif

#include "XLB.h"
#include "CutHolder.h"
#include "WeightCover.h"



BEGIN_NAMESPACE_MAGUS_LUTMAP


// コンストラクタ
XLB::XLB(size_t n) :
  mDlimit(n)
{
  LBFactory lbfactory;
  mLowerBound = lbfactory("weight");
}

// デストラクタ
XLB::~XLB()
{
}

// 下限を計算する
int
XLB::calc(const SbjGraph& sbjgraph,
	  const CutHolder& cut_holder)
{
  return search(sbjgraph, cut_holder, 0, 0, 0);
}

// 分枝限定法による探索
int
XLB::search(const SbjGraph& sbjgraph,
	    const CutHolder& cut_holder,
	    size_t b,
	    size_t level,
	    int lb0)
{
  if ( level >= mDlimit ) {
    return lb0;
  }

  int lb = mLowerBound->calc(sbjgraph, cut_holder);
  if ( lb < lb0 ) {
    lb = lb0;
  }
  
  // 次に分岐するノードを探す．
  size_t n = sbjgraph.max_node_id();
  for (size_t i = b; i < n; ++ i) {
    SbjNode* dec_node = sbjgraph.node(i);
    if ( dec_node == NULL ) continue;
    // dec_node を unselected にする．
    dec_node->set_unselected();

    int lb1 = search(sbjgraph, cut_holder, i + 1, level + 1, lb);
    if ( lb < lb1 ) {
      lb = lb1;
    }

    dec_node->clear_unselected();
  }
  return lb;
}

END_NAMESPACE_MAGUS_LUTMAP
