#ifndef YM_LUTMAP_LUTMAP_NSDEF_H
#define YM_LUTMAP_LUTMAP_NSDEF_H

/// @file magus/lutmap/lutmap.h
/// @brief luttmap パッケージに共通な定義ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: lutmap.h 555 2007-04-08 07:20:30Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ymtools.h"
#include "ym_sbj/sbj_nsdef.h"
#include "ym_utils/DlList.h"


/// @brief lutmap 用の名前空間の開始
#define BEGIN_NAMESPACE_YM_LUTMAP \
BEGIN_NAMESPACE_YM \
BEGIN_NAMESPACE(nsLutmap)

/// @brief lutmap 用の名前空間の終了
#define END_NAMESPACE_YM_LUTMAP \
END_NAMESPACE(nsLutmap) \
END_NAMESPACE_YM


BEGIN_NAMESPACE_YM_LUTMAP

class LnGraph;
class LnEdge;
class LnNode;

typedef DlList<LnEdge> LnEdgeList;
typedef DlList<LnNode> LnNodeList;


/// @brief 面積最小化 DAG covering のヒューリスティック関数
/// @param[in] sbjgraph サブジェクトグラフ
/// @param[in] limit カットサイズ
/// @param[in] mode モード
///  - 0: fanout フロー, resub なし
///  - 1: weighted フロー, resub なし
///  - 2: fanout フロー, resub あり
///  - 3: weighted フロー, resub あり
/// @param[out] mapnetwork マッピング結果
/// @param[out] lut_num LUT数
/// @param[out] depth 段数
void
area_map(const SbjGraph& sbjgraph,
	 ymuint limit,
	 ymuint mode,
	 LnGraph& mapnetwork,
	 ymuint& lut_num,
	 ymuint& depth);

/// @brief 段数最小化 DAG covering のヒューリスティック関数
/// @param[in] sbjgraph サブジェクトグラフ
/// @param[in] limit カットサイズ
/// @param[in] slack 最小段数に対するスラック
/// @param[in] mode モード
///  - 0: fanout フロー, resub なし
///  - 1: weighted フロー, resub なし
///  - 2: fanout フロー, resub あり
///  - 3: weighted フロー, resub あり
/// @param[out] mapnetwork マッピング結果
/// @param[out] lut_num LUT数
/// @param[out] depth 段数
void
delay_map(const SbjGraph& sbjgraph,
	  ymuint limit,
	  ymuint slack,
	  ymuint mode,
	  LnGraph& mapnetwork,
	  ymuint& lut_num,
	  ymuint& depth);

END_NAMESPACE_YM_LUTMAP

BEGIN_NAMESPACE_YM

using nsLutmap::LnGraph;
using nsLutmap::LnEdge;
using nsLutmap::LnNode;

using nsLutmap::LnEdgeList;
using nsLutmap::LnNodeList;

using nsLutmap::area_map;
using nsLutmap::delay_map;

END_NAMESPACE_YM

#endif // YM_LUTMAP_LUTMAP_NSDEF_H
