#ifndef LIBYM_LUTMAP_CUTRESUB_H
#define LIBYM_LUTMAP_CUTRESUB_H

/// @file libym_lutmap/CutResub.h
/// @brief CutResub のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: CutResub.h 2274 2009-06-10 07:45:29Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_lutmap/lutmap_nsdef.h"
#include "ym_sbj/sbj_nsdef.h"

#include "ym_utils/Alloc.h"
#include "CrNode.h"
#include "CrHeap.h"
#include "CrLevelQ.h"


BEGIN_NAMESPACE_YM_LUTMAP

class Cut;
class CutHolder;
class MapRecord;

//////////////////////////////////////////////////////////////////////
/// @class CutResub CutResub.h "CutResub.h"
/// @brief カットの置き換えを行うクラス
//////////////////////////////////////////////////////////////////////
class CutResub
{
public:

  /// @brief コンストラクタ
  CutResub();

  /// @brief デストラクタ
  virtual
  ~CutResub();


public:

  /// @brief カットの置き換えを行って LUT 数の削減を行う．
  /// @param[in] sbjgraph サブジェクトグラフ
  /// @param[in] cut_holder サブジェクトグラフ上のカット集合
  /// @param[inout] maprec マッピング結果
  void
  operator()(const SbjGraph& sbjgraph,
	     const CutHolder& cut_holder,
	     MapRecord& maprec,
	     int slack = -1);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部でのみ用いられる関数
  //////////////////////////////////////////////////////////////////////

  // node の最適カットを選ぶ．
  void
  back_trace(const SbjNode* node,
	     MapRecord& maprec,
	     CrNode* from);

  // ゲインの計算を行う．
  ymuint
  calc_gain(CrNode* crnode);

  // 構造のみで置き換えが可能かどうか判断する．
  bool
  check_structure(CrNode* node);

  // node を冗長にする置き換えカットを求める．
  bool
  find_subst(CrNode* node,
	     vector<const Cut*>& subst_list);

  // node を冗長にする置き換えカットを求める．
  bool
  find_subst2(CrNode* node,
	      vector<const Cut*>& subst_list);

  // カットの置き換えを行い，段数の情報を更新する．
  void
  update(CrNode* node,
	 const vector<const Cut*>& subst_list);

  // node のカットを old_cut から new_cut に置き換える．
  void
  subst_cut_fanouts(CrNode* node,
		    const Cut* old_cut,
		    const Cut* new_cut);

  // nodeのファンアウトリストにadd_nodeを追加する．
  bool
  add_fo_list(vector<CrNode*>& fo_list,
	      CrNode* add_node);

  // ゲイン計算用のキューにつむ．
  void
  put_gq(CrNode* node);

  // ゲイン計算用のキューから取り出す．
  CrNode*
  get_gq();

  // レベル計算用のキューにつむ．
  void
  put_lq(CrNode* node);

  // レベル計算用のキューから取り出す．
  CrNode*
  get_lq();

  // 要求レベル計算用のキューにつむ．
  void
  put_rq(CrNode* node);

  // 要求レベル計算用のキューから取り出す．
  CrNode*
  get_rq();

  // カットの根に対応するノードを取り出す．
  CrNode*
  cut_root(const Cut* cut);

  // カットの入力に対応するノードを取り出す．
  CrNode*
  cut_input(const Cut* cut,
	    ymuint pos);

  // CrNode を確保する．
  CrNode*
  alloc_node();

  // CrNode を解放する．
  void
  free_node(CrNode* node);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // CrNode の領域確保用のオブジェクト
  SimpleAlloc mAlloc;

  // 各ノードごとの情報を納める配列
  vector<CrNode*> mNodeArray;

  // 削除されたノードを貯めておくリスト
  list<CrNode*> mGarbageList;

  // レベル制約がある時 true となるフラグ
  bool mHasLevelConstr;

  // ゲインをキーとしたヒープ
  CrHeap mHeap;

  // ゲイン計算用のレベル付きキュー
  CrLevelQ mGQ;

  // レベル計算用のレベル付きキュー
  CrLevelQ mLQ;

  // 要求レベル計算用のレベル付きキュー
  CrLevelQ mRQ;

  // 削除されるカットを入れておく作業領域
  vector<const Cut*> mDeletedCuts;

  // 削除されるノードを入れておく作業領域
  vector<CrNode*> mDeletedNodes;

};

END_NAMESPACE_YM_LUTMAP

#endif // YM_LUTMAP_CUTRESUB_H
