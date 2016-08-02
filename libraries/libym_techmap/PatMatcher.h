#ifndef LIBYM_TECHMAP_PATMATCHER_H
#define LIBYM_TECHMAP_PATMATCHER_H

/// @file libym_techmap/PatMatcher.h
/// @brief PatMatcher のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_techmap/techmap_nsdef.h"


BEGIN_NAMESPACE_YM_TECHMAP

class PatMgr;
class PatGraph;

//////////////////////////////////////////////////////////////////////
/// @class PatMatcher PatMatcher.h "PatMatcher.h"
/// @brief パタンマッチングを行うクラス
//////////////////////////////////////////////////////////////////////
class PatMatcher
{
public:

  /// @brief コンストラクタ
  /// @param[in] pat_mgr パタンを管理するクラス
  PatMatcher(const PatMgr& pat_mgr);

  /// @brief デストラクタ
  ~PatMatcher();


public:

  /// @brief パタンマッチングを行う．
  /// @param[in] sbj_root サブジェクトグラフの根のノード
  /// @param[in] pat_graph パタングラフ
  /// @retval true マッチした．
  /// @retval false マッチしなかった．
  bool
  operator()(const SbjNode* sbj_root,
	     const PatGraph& pat_graph);

  /// @brief 直前のマッチングにおける出力の極性を得る．
  /// @retval true 反転あり
  /// @retval false 反転なし
  bool
  root_inv() const;

  /// @brief 直前のマッチングにおける入力のノードを得る．
  /// @param[in] pos 入力番号
  const SbjNode*
  leaf_node(ymuint pos) const;

  /// @brief 直前のマッチングにおける入力の極性を得る．
  /// @param[in] pos 入力番号
  /// @retval true 反転あり
  /// @retval false 反転なし
  bool
  leaf_inv(ymuint pos) const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief サブジェクトノードとパタンノードをバインドする．
  /// @param[in] sbj_node サブジェクトノード
  /// @param[in] pat_id パタンノードのID
  /// @param[in] inv 反転フラグ
  /// @retval true バインドが成功した．
  /// @retval false バインドが失敗した．
  bool
  bind(const SbjNode* sbj_node,
       ymuint pat_id,
       bool inv);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // パタングラフを管理するクラス
  const PatMgr& mPatMgr;

  // パタンノードの ID をキーとしてサブジェクトノードを入れる配列
  vector<const SbjNode*> mSbjMap;

  // サブジェクトノードの ID をキーとしてパタンノードの ID を
  // 入れる配列
  hash_map<ymuint, ymuint> mPatMap;

  // パタンノードの ID をキーとして極性を入れる配列
  vector<bool> mInvMap;

  // mSbjMap と mInvMap のクリア用キュー
  vector<ymuint> mClearQueue;

  // 直前のマッチングにおけるパタンの入力ノードを記録する配列
  vector<const SbjNode*> mLeafNodeArray;

  // 直前のマッチングにおけるパタンの入力の極性を記録する配列
  vector<bool> mLeafInvArray;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 直前のマッチングにおける入力のノードを得る．
// @param[in] pos 入力番号
inline
const SbjNode*
PatMatcher::leaf_node(ymuint pos) const
{
  return mLeafNodeArray[pos];
}

// @brief 直前のマッチングにおける入力の極性を得る．
// @param[in] pos 入力番号
// @retval true 反転あり
// @retval false 反転なし
inline
bool
PatMatcher::leaf_inv(ymuint pos) const
{
  return mLeafInvArray[pos];
}

END_NAMESPACE_YM_TECHMAP

#endif // LIBYM_TECHMAP_PATMATCHER_H
