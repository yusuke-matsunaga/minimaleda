#ifndef MAGUS_LUTMAP_DFCOVER_H
#define MAGUS_LUTMAP_DFCOVER_H

/// @file magus/lutmap/DFCover.h
/// @brief duplication free covering を行うクラス
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: DFCover.h 1077 2007-12-03 10:18:47Z matsunaga $
///
/// Copyright (C) 2005-2007 Yusuke Matsunaga
/// All rights reserved.

#include "DagACover.h"


BEGIN_NAMESPACE_MAGUS_LUTMAP

class SbjNode;
class Cut;

//////////////////////////////////////////////////////////////////////
/// @class DFCover DFCover.h "DFCover.h"
/// @brief duplication free cover を求めるためのクラス
//////////////////////////////////////////////////////////////////////
class DFCover :
  public DagACover
{
public:

  /// @brief コンストラクタ
  DFCover();

  /// @brief デストラクタ
  virtual
  ~DFCover();


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

  // node から tree フラグをたどって到達可能な入力に印をつける．
  void
  check_path(SbjNode* node,
	     const Cut* cut);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  vector<int> mBestCost;

  vector<bool> mReached;

};

END_NAMESPACE_MAGUS_LUTMAP

#endif // MAGUS_LUTMAP_DFCOVER_H
