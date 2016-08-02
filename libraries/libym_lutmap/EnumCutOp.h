#ifndef LIBYM_LUTMAP_ENUMCUTOP_H
#define LIBYM_LUTMAP_ENUMCUTOP_H

/// @file libym_lutmap/EnumCutOp.h
/// @brief EnumCutOp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: EnumCutOp.h 2274 2009-06-10 07:45:29Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_lutmap/lutmap_nsdef.h"
#include "ym_sbj/sbj_nsdef.h"


BEGIN_NAMESPACE_YM_LUTMAP

class EnumCut;

//////////////////////////////////////////////////////////////////////
/// @class EnumCutOp
/// @brief EnumCut 中でカットを見つけた時に呼び出されるクラス
/// @note 実際にはこのクラスを継承したクラスを用意する必要がある．
//////////////////////////////////////////////////////////////////////
class EnumCutOp
{
  friend class EnumCut;

protected:

  /// @brief コンストラクタ
  EnumCutOp();

  /// @brief デストラクタ
  virtual
  ~EnumCutOp();


public:

  /// @brief カットの列挙を行う．
  /// @param[in] sbjgraph 対象のサブジェクトグラフ
  /// @param[in] limit 入力数の制限
  /// @param[in] mode cut 列挙モード
  /// @return 全 cut 数を返す．
  /// @note 内部で下の仮想関数が呼び出される．
  ymuint
  enum_cut(const SbjGraph& sbjgraph,
	   ymuint limit);


private:
  //////////////////////////////////////////////////////////////////////
  // 継承クラスが定義する必要のある仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 処理の最初に呼ばれる関数
  /// @param[in] sbjgraph 対象のサブジェクトグラフ
  /// @param[in] limit カットサイズ
  /// @param[in] mode カット列挙モード
  /// @note デフォルトの実装ではなにもしない．
  virtual
  void
  all_init(const SbjGraph& sbjgraph,
	   ymuint limit);

  /// @brief node を根とするカットを列挙する直前に呼ばれる関数
  /// @param[in] node 根のノード
  /// @param[in] cur_pos node の処理順
  /// @note デフォルトの実装ではなにもしない．
  virtual
  void
  node_init(const SbjNode* node,
	    ymuint cur_pos);

  /// @brief cut が一つ見つかったときに呼ばれる関数(singlton cut)
  /// @param[in] root 根のノード
  /// @note デフォルトの実装ではなにもしない．
  virtual
  void
  found(const SbjNode* root);

  /// @brief cut が一つ見つかったときに呼ばれる関数(non-trivial cut)
  /// @param[in] root 根のノード
  /// @param[in] ni 入力数
  /// @param[in] inputs 入力ノードの配列
  /// @note デフォルトの実装ではなにもしない．
  virtual
  void
  found(const SbjNode* root,
	ymuint ni,
	const SbjNode* inputs[]);

  /// @brief node を根とするカットを列挙し終わった直後に呼ばれる関数
  /// @param[in] node 根のノード
  /// @param[in] cur_pos node の処理順
  /// @param[in] ncuts 列挙されたカット数
  /// @note デフォルトの実装ではなにもしない．
  virtual
  void
  node_end(const SbjNode* node,
	   ymuint cur_pos,
	   ymuint ncuts);

  /// @brief 処理の最後に呼ばれる関数
  /// @note デフォルトの実装ではなにもしない．
  virtual
  void
  all_end(const SbjGraph& sbjgraph,
	  ymuint limit);

};

END_NAMESPACE_YM_LUTMAP

#endif // LIBYM_LUTMAP_ENUMCUTOP_H
