#ifndef YM_LUTMAP_AREAMAP_H
#define YM_LUTMAP_AREAMAP_H

/// @file ym_lutmap/AreaMap.h
/// @brief AreaMap のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: DagACover.h 1077 2007-12-03 10:18:47Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_lutmap/lutmap_nsdef.h"
#include "ym_sbj/sbj_nsdef.h"


BEGIN_NAMESPACE_YM_LUTMAP

//////////////////////////////////////////////////////////////////////
/// @class AreaMap AreaMap.h "AreaMap.h"
/// @brief 面積最小化 DAG covering のヒューリスティック
//////////////////////////////////////////////////////////////////////
class AreaMap
{
public:

  /// @brief コンストラクタ
  AreaMap();

  /// @brief デストラクタ
  ~AreaMap();


public:
  //////////////////////////////////////////////////////////////////////
  // 継承クラスが実装しなければならない仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief best cut の記録を行う．
  /// @param[in] sbjgraph サブジェクトグラフ
  /// @param[out] mapnetwork マッピング結果を保持するネットワーク
  /// @param[in] limit カットサイズ
  /// @param[in] resub cut resubstitution を行うとき true にするフラグ
  int
  record_cuts(const SbjGraph& sbjgraph,
	      LnGraph& mapnetwork,
	      ymuint limit,
	      bool resub);

};

END_NAMESPACE_YM_LUTMAP

#endif // YM_LUTMAP_AREAMAP_H
