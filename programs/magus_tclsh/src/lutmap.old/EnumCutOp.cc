
/// @file magus/lutmap/EnumCutOp.cc 
/// @brief EnumCutOp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: EnumCutOp.cc 2274 2009-06-10 07:45:29Z matsunaga $
///
/// Copyright (C) 2005-2006 Yusuke Matsunaga
/// All rights reserved.


#if HAVE_CONFIG_H
#include "magus_config.h"
#endif

#include "SbjGraph.h"
#include "EnumCutOp.h"
#include "EnumCut.h"
#include "EnumCut2.h"

//#define DEBUG_ENUM_RECUR


BEGIN_NAMESPACE_MAGUS_LUTMAP


//////////////////////////////////////////////////////////////////////
/// クラス EnumCutOp
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
EnumCutOp::EnumCutOp()
{
}

// @brief デストラクタ
EnumCutOp::~EnumCutOp()
{
}

// @brief カットの列挙を行う．
// @param[in] 対象のサブジェクトグラフ
// @param[in] limit 入力数の制限
// @param[in] mode cut 列挙モード
// @return 全 cut 数を返す．
ymuint
EnumCutOp::enum_cut(const SbjGraph& sbjgraph,
		    ymuint limit,
		    tCutMode mode)
{
  EnumCut ec;
  return ec(sbjgraph, limit, mode, this);
}

// @brief カットの列挙を行う．
// @param[in] sbjgraph 対象のサブジェクトグラフ
// @param[in] maprec 現在のマッピング結果
// @param[in] limit 入力数の制限
// @param[in] mode cut 列挙モード
// @return 全 cut 数を返す．
ymuint
EnumCutOp::enum_cut(const SbjGraph& sbjgraph,
		    MapRecord& maprec,
		    ymuint limit,
		    tCutMode mode)
{
  EnumCut2 ec;
  return ec(sbjgraph, maprec, limit, mode, this);
}

// @brief 処理の最初に呼ばれる関数
// @param[in] sbjgraph 対象のサブジェクトグラフ
// @param[in] limit カットサイズ
// @param[in] mode カット列挙モード
// @note デフォルトの実装ではなにもしない．
void
EnumCutOp::all_init(const SbjGraph& sbjgraph,
		    ymuint limit,
		    tCutMode mode)
{
}

// @brief node を根とするカットを列挙する直前に呼ばれる関数
// @param[in] node 根のノード
// @param[in] cur_pos node の処理順
// @note デフォルトの実装ではなにもしない．
void
EnumCutOp::node_init(SbjNode* node,
		     ymuint cur_pos)
{
}

// @brief cut が一つ見つかったときに呼ばれる関数(singlton cut)
// @param[in] root 根のノード
// @note デフォルトの実装ではなにもしない．
void
EnumCutOp::found(SbjNode* root)
{
}

// @brief cut が一つ見つかったときに呼ばれる関数(non-trivial cut)
// @param[in] root 根のノード
// @param[in] ni 入力数
// @param[in] inputs 入力ノードの配列
// @note デフォルトの実装ではなにもしない．
void
EnumCutOp::found(SbjNode* root,
		 ymuint ni,
		 SbjNode* inputs[])
{
}

// @brief node を根とするカットを列挙し終わった直後に呼ばれる関数
// @param[in] node 根のノード
// @param[in] cur_pos node の処理順
// @param[in] ncuts 列挙されたカット数
// @note デフォルトの実装ではなにもしない．
void
EnumCutOp::node_end(SbjNode* node,
		    ymuint cur_pos,
		    ymuint ncuts)
{
}

// @brief 処理の最後に呼ばれる関数
// @note デフォルトの実装ではなにもしない．
void
EnumCutOp::all_end(const SbjGraph& sbjgraph,
		   ymuint limit,
		   tCutMode mode)
{
}

END_NAMESPACE_MAGUS_LUTMAP
