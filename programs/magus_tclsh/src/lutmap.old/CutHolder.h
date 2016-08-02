#ifndef MAGUS_LUTMAP_CUTHOLDER_H
#define MAGUS_LUTMAP_CUTHOLDER_H

/// @file magus/lutmap/CutHolder.h
/// @brief CutHolder のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: CutHolder.h 2274 2009-06-10 07:45:29Z matsunaga $
///
/// Copyright (C) 2005-2007 Yusuke Matsunaga
/// All rights reserved.


#include "EnumCutOp.h"
#include "Cut.h"
#include "SbjGraph.h"


BEGIN_NAMESPACE_MAGUS_LUTMAP

//////////////////////////////////////////////////////////////////////
/// @class CutHolder
/// @brief 各ノードの cut を保持するクラス
//////////////////////////////////////////////////////////////////////
class CutHolder :
  public EnumCutOp
{
public:

  /// @brief コンストラクタ
  CutHolder();

#if 0
  /// @brief 特殊なコンストラクタ
  CutHolder(const CutHolder& src,
	    const SbjGraph& sbjgraph,
	    ymuint limit,
	    const vector<SbjNode*>& dup_list);
#endif

  /// @brief デストラクタ
  virtual
  ~CutHolder();


public:

  /// @brief node を根とするカットのリストを取り出す．
  const CutList&
  cut_list(SbjNode* node) const;

  /// @brief 現在のカットを列挙したときのカットサイズを返す．
  ymuint
  limit() const;

  /// @brief 現在のカットを列挙したときのモードを返す．
  tCutMode
  cut_mode() const;
  
  /// @brief 保持しているカットのリストを削除する．
  void
  clear();


private:
  //////////////////////////////////////////////////////////////////////
  // EnumCutOp の継承クラスが定義する必要のある仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 処理の最初に呼ばれる関数
  /// @param[in] sbjgraph 対象のサブジェクトグラフ
  /// @param[in] limit カットサイズ
  /// @param[in] mode カット列挙モード
  virtual
  void
  all_init(const SbjGraph& sbjgraph,
	   ymuint limit,
	   tCutMode mode);
  
  /// @brief node を根とするカットを列挙する直前に呼ばれる関数
  /// @param[in] node 根のノード
  /// @param[in] cur_pos node の処理順
  virtual
  void
  node_init(SbjNode* node,
	    ymuint cur_pos);

  /// @brief cut が一つ見つかったときに呼ばれる関数(singlton cut)
  /// @param[in] root 根のノード
  virtual
  void
  found(SbjNode* root);

  /// @brief cut が一つ見つかったときに呼ばれる関数(non-trivial cut)
  /// @param[in] root 根のノード
  /// @param[in] ni 入力数
  /// @param[in] inputs 入力ノードの配列
  virtual
  void
  found(SbjNode* root,
	ymuint ni,
	SbjNode* inputs[]);

  /// @brief node を根とするカットを列挙し終わった直後に呼ばれる関数
  /// @param[in] node 根のノード
  /// @param[in] cur_pos node の処理順
  /// @param[in] ncuts 列挙されたカット数
  virtual
  void
  node_end(SbjNode* node,
	   ymuint cur_pos,
	   ymuint ncuts);

  /// @brief 処理の最後に呼ばれる関数
  /// @param[in] sbjgraph 対象のサブジェクトグラフ
  /// @param[in] limit カットサイズ
  /// @param[in] mode カット列挙モード
  /// @note sbjgraph, limit, mode は 対となる all_init で
  /// 用いられたものと同じものが与えられる．
  virtual
  void
  all_end(const SbjGraph& sbjgraph,
	  ymuint limit,
	  tCutMode mode);


private:

#if 0
  // ノードの複製を考慮してカットを複製する．
  Cut*
  duplicate(const Cut* cut,
	    ymuint limit,
	    const vector<SbjNode*>& dup_list);
#endif

  // カットの領域を確保する．
  Cut*
  alloc_cut(ymuint ni);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////
  
  // カットサイズ
  ymuint32 mLimit;

  // カットモード
  tCutMode mCutMode;
  
  // 各ノードのカットのリスト
  CutList* mCutList;

  static
  const ymuint kPageSize = 8192;
  
  char* mCurChunk;

  ymuint32 mLast;
  
  vector<char*> mChunk;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief node を根とするカットのリストを取り出す．
inline
const CutList&
CutHolder::cut_list(SbjNode* node) const
{
  return mCutList[node->id()];
}

// @brief 現在のカットを列挙したときのカットサイズを返す．
inline
ymuint
CutHolder::limit() const
{
  return mLimit;
}

// @brief 現在のカットを列挙したときのモードを返す．
inline
tCutMode
CutHolder::cut_mode() const
{
  return mCutMode;
}

END_NAMESPACE_MAGUS_LUTMAP

#endif // MAGUS_LUTMAP_CUTHOLDER_H
