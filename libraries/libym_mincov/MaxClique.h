#ifndef LIBYM_MINCOV_MAXCLIQUE_H
#define LIBYM_MINCOV_MAXCLIQUE_H

/// @file libym_mincov/MaxClique.h
/// @brief MaxClique のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
/// 
/// $Id: MaxClique.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.

#include "ym_mincov/mincov_nsdef.h"


BEGIN_NAMESPACE_YM_MINCOV

class MincovCost;

//////////////////////////////////////////////////////////////////////
/// @class MaxClique MaxClique.h "MaxClique.h"
/// @brief 最大クリーク問題を解くクラス
//////////////////////////////////////////////////////////////////////
class MaxClique
{
public:

  /// @brief コンストラクタ
  /// @param[in] size ノード数
  MaxClique(ymuint size);
  
  /// @brief デストラクタ
  ~MaxClique();


public:

  /// @brief コストを設定する．
  void
  set_cost(ymuint32 id,
	   const MincovCost* cost);
  
  /// @brief 2つのノードを隣接させる．
  /// @param[in] id1, id2 ノード番号
  void
  connect(ymuint32 id1,
	  ymuint32 id2);
  
  /// @brief 最大クリークを求める．
  /// @param[out] ans 解のノード番号を入れる配列
  MincovCost
  solve(vector<ymuint32>& ans);

  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // コストの配列
  vector<const MincovCost*> mCostArray;
  
  // 隣接ノードリストの配列
  vector<vector<ymuint32> > mNlistArray;

};

END_NAMESPACE_YM_MINCOV

#endif // LIBYM_MINCOV_MAXCLIQUE_H
