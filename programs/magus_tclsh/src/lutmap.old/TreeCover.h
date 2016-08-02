#ifndef MAGUS_LUTMAP_TREECOVER_H
#define MAGUS_LUTMAP_TREECOVER_H

/// @file matus/lutmap/TreeCover.h
/// @brief tree covering を行うクラス
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: TreeCover.h 1077 2007-12-03 10:18:47Z matsunaga $
///
/// Copyright (C) 2005-2007 Yusuke Matsunaga
/// All rights reserved.

#include "DagACover.h"


BEGIN_NAMESPACE_MAGUS_LUTMAP

//////////////////////////////////////////////////////////////////////
/// @class TreeCover TreeCover.h "TreeCover.h"
/// @brief tree covering を行うクラス
//////////////////////////////////////////////////////////////////////
class TreeCover :
  public DagACover
{
public:

  /// @brief コンストラクタ
  TreeCover();

  /// @brief デストラクタ
  virtual
  ~TreeCover();


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
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////
  
  vector<int> mBestCost;
  
};
	
END_NAMESPACE_MAGUS_LUTMAP

#endif // MAGUS_LUTMAP_TREECOVER_H
