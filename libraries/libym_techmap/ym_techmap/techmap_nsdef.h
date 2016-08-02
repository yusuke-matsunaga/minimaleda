#ifndef YM_TECHMAP_TECHMAP_NSDEF_H
#define YM_TECHMAP_TECHMAP_NSDEF_H

/// @file libym_techmap/ym_sbj/sbj_nsdef.h
/// @brief techmap パッケージに共通な定義ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: lutmap.h 555 2007-04-08 07:20:30Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ymtools.h"
#include "ym_sbj/sbj_nsdef.h"
#include "ym_utils/DlList.h"


/// @brief techmap 用の名前空間の開始
#define BEGIN_NAMESPACE_YM_TECHMAP \
BEGIN_NAMESPACE_YM \
BEGIN_NAMESPACE(nsTechmap)

/// @brief techmap 用の名前空間の終了
#define END_NAMESPACE_YM_TECHMAP \
END_NAMESPACE(nsTechmap) \
END_NAMESPACE_YM


BEGIN_NAMESPACE_YM_TECHMAP

class  PatMgr;

class CnGraph;
class CnEdge;
class CnNode;

typedef DlList<CnEdge> CnEdgeList;
typedef DlList<CnNode> CnNodeList;


/// @brief 面積最小化 DAG covering のヒューリスティック関数
/// @param[in] sbjgraph サブジェクトグラフ
/// @param[in] pat_mgr パタンマネージャ
/// @param[in] mode モード
///  - 0: fanout フロー, resub なし
///  - 1: weighted フロー, resub なし
///  - 2: fanout フロー, resub あり
///  - 3: weighted フロー, resub あり
/// @param[out] mapnetwork マッピング結果
void
area_map(const SbjGraph& sbjgraph,
	 const PatMgr& pat_mgr,
	 ymuint mode,
	 CnGraph& mapnetwork);

#if 0
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
#endif

END_NAMESPACE_YM_TECHMAP

BEGIN_NAMESPACE_YM

using nsTechmap::PatMgr;

using nsTechmap::CnGraph;
using nsTechmap::CnEdge;
using nsTechmap::CnNode;

using nsTechmap::CnEdgeList;
using nsTechmap::CnNodeList;

using nsTechmap::area_map;
#if 0
using nsTechmap::delay_map;
#endif

END_NAMESPACE_YM

#endif // YM_TECHMAP_TECHMAP_NSDEF_H
