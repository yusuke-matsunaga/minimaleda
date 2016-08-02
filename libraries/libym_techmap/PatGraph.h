#ifndef LIBYM_TECHMAP_PATGRAPH_H
#define LIBYM_TECHMAP_PATGRAPH_H

/// @file libym_techmap/PatGraph.h
/// @brief PatGraph のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_techmap/techmap_nsdef.h"


BEGIN_NAMESPACE_YM_TECHMAP

//////////////////////////////////////////////////////////////////////
/// @class PatGraph PatGraph.h "PatGraph.h"
/// @brief パタングラフを表すクラス
///
/// 実際には根の反転属性と枝のリストしか持っていない．
/// その他の情報は PatMgr から補完する．
//////////////////////////////////////////////////////////////////////
class PatGraph
{
  friend class PatMgr;

public:

  /// @brief コンストラクタ
  PatGraph();

  /// @brief デストラクタ
  ~PatGraph();


public:

  /// @brief 代表関数番号を返す．
  ymuint
  rep_id() const;

  /// @brief 根のノード番号を返す．
  ymuint
  root_id() const;

  /// @brief 根の反転属性を返す．
  bool
  root_inv() const;

  /// @brief 入力数を返す．
  ymuint
  input_num() const;

  /// @brief 枝数を返す．
  ymuint
  edge_num() const;

  /// @brief 枝(の番号)を返す．
  /// @param[in] pos 位置 ( 0 <= pos < edge_num() )
  ymuint
  edge(ymuint pos) const;


private:
  //////////////////////////////////////////////////////////////////////
  // 情報設定用の関数
  // PatMgr のみが用いる．
  //////////////////////////////////////////////////////////////////////

  /// @brief データを読み込んでセットする．
  /// @param[in] s 入力元のストリーム
  /// @retval true 読み込みが成功した．
  /// @retval false 読み込みが失敗した．
  bool
  load(istream& s);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 代表関数番号
  ymuint32 mRepId;

  // 入力数 + 根の反転属性
  ymuint32 mInputNum;

  // 枝数
  ymuint32 mEdgeNum;

  // 枝番号の配列
  ymuint32* mEdgeList;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 代表関数番号を返す．
inline
ymuint
PatGraph::rep_id() const
{
  return mRepId;
}

// @brief 根のノード番号を返す．
inline
ymuint
PatGraph::root_id() const
{
  // 枝の番号を2で割ればファンアウト先のノード番号
  return mEdgeList[0] / 2;
}

// @brief 根の反転属性を返す．
inline
bool
PatGraph::root_inv() const
{
  return static_cast<bool>(mInputNum & 1U);
}

// @brief 入力数を返す．
inline
ymuint
PatGraph::input_num() const
{
  return (mInputNum >> 1);
}

// @brief 枝数を返す．
inline
ymuint
PatGraph::edge_num() const
{
  return mEdgeNum;
}

// @brief 枝(の番号)を返す．
// @param[in] pos 位置 ( 0 <= pos < edge_num() )
inline
ymuint
PatGraph::edge(ymuint pos) const
{
  return mEdgeList[pos];
}

END_NAMESPACE_YM_TECHMAP

#endif // LIBYM_TECHMAP_PATGRAPH_H
