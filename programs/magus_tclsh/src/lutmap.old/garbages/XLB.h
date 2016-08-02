#ifndef MAGUS_LUTMAP_XLB_H
#define MAGUS_LUTMAP_XLB_H

/// @file magus/lutmap/XLB.h
/// @brief XLB のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: XLB.h 1077 2007-12-03 10:18:47Z matsunaga $
///
/// Copyright (C) 2005-2007 Yusuke Matsunaga
/// All rights reserved.

#include "LowerBound.h"
#include "SbjGraph.h"
#include "WeightCover.h"


BEGIN_NAMESPACE_MAGUS_LUTMAP

class Cut;

//////////////////////////////////////////////////////////////////////
// DAG covering の下限を計算するためのクラス
//////////////////////////////////////////////////////////////////////
class XLB :
  public LowerBound
{
public:
  
  // コンストラクタ
  XLB(size_t n);

  // デストラクタ
  virtual
  ~XLB();

  
public:

  // 下限を計算する．
  virtual
  int
  calc(const SbjGraph& sbjgraph,
       const CutHolder& cut_holder);


private:

  int
  search(const SbjGraph& sbjgraph,
	 const CutHolder& cut_holder,
	 size_t b,
	 size_t level,
	 int lb0);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////
  
  size_t mDlimit;
  
  // lower bound を計算するファンクター
  LowerBound* mLowerBound;
  
};

END_NAMESPACE_MAGUS_LUTMAP

#endif // MAGUS_LUTMAP_XLB_H
