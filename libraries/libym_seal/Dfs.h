#ifndef LIBYM_SEAL_DFS_H
#define LIBYM_SEAL_DFS_H

/// @file libym_seal/Dfs.h
/// @brief Dfs のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: Matrix.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_seal/seal_nsdef.h"
#include "ym_utils/Alloc.h"


BEGIN_NAMESPACE_YM_SEAL


//////////////////////////////////////////////////////////////////////
/// @class DfsNode Dfs.h "Dfs.h"
/// @brief 状態に対応するノードを表すクラス
//////////////////////////////////////////////////////////////////////
struct DfsNode
{
  // 状態番号
  ymuint32 mId;

  // DEFNUMBER
  ymuint32 mDefnumber;

  // LOWLINK
  ymuint32 mLowlink;

  // 訪問したかどうかを表すフラグ
  bool mVisited;

  // スタックにつまれているかを表すフラグ
  bool mOnStack;

  // 遷移先のリスト
  vector<DfsNode*> mAdjList;

  // 遷移元のリスト
  vector<DfsNode*> mFromList;
  
  // 同じ強連結成分をつなぐリンク
  DfsNode* mLink;

  // ヒープ用の値
  ymuint32 mCost;
  
  // ヒープ上の位置
  ymuint32 mHeapPos;

  // 選ばれたかどうかを表すフラグ
  bool mSelected;
  
};


//////////////////////////////////////////////////////////////////////
/// @class Dfs Dfs.h "Dfs.h"
/// @brief DFS を行って強連結成分を求めるクラス
//////////////////////////////////////////////////////////////////////
class Dfs
{
public:

  /// @brief コンストラクタ
  /// @param[in] num ノード数
  Dfs(ymuint num);

  /// @brief デストラクタ
  ~Dfs();

  
public:

  /// @biref ノードの接続関係を設定する．
  /// @param[in] id ノード番号
  /// @param[in] adjlist 遷移先のノード番号のリスト
  void
  set_adjlist(ymuint id,
	      const vector<ymuint>& adjlist);

  /// @brief 遷移元のノードのリストを設定する．
  /// @param[in] id ノード番号
  /// @param[in] fromlist 遷移元のノード番号のリスト
  void
  set_fromlist(ymuint id,
	       const list<ymuint>& fromlist);

  /// @brief dfs を行って強連結成分を求める．
  /// @return 強連結成分の数を返す．
  ymuint
  scc();

  /// @brief 強連結成分の代表ノードを返す．
  /// @param[in] id 強連結成分の番号
  DfsNode*
  repnode(ymuint id);


private:

  /// @brief scc のために dfs を行う関数
  void
  dfs(DfsNode* node);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ノード数
  ymuint32 mNum;

  // ノードの配列
  DfsNode* mNodeArray;

  // 代表ノードを収める配列
  vector<DfsNode*> mRepNodes;
  
  // カウント値
  ymuint32 mCount;

  // ノードスタック
  vector<DfsNode*> mStack;
  
};


//////////////////////////////////////////////////////////////////////
/// @class DfsHeap Dfs.h "Dfs.h"
/// @brief DfsNode 用のヒープ木
//////////////////////////////////////////////////////////////////////
class DfsHeap
{
public:

  /// @brief コンストラクタ
  /// @param[in] size_hint 容量のヒント
  DfsHeap(ymuint size_hint = 0);

  /// @brief デストラクタ
  ~DfsHeap();


public:
  
  /// @brief 要素が空の時 true を返す．
  bool
  empty() const;

  /// @brief ノードを追加する．
  void
  push(DfsNode* node);

  /// @brief 最小値を持つノードを取り出す．
  DfsNode*
  get_min();

  /// @brief ノードをコストが変化した時の更新処理
  void
  rebalance(DfsNode* node);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部でのみ使用される関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ノードを適切な位置まで沈めてゆく関数
  void
  pushdown(ymuint pos);

  /// @brief ノードを適切な位置まで浮かび上がらせる関数
  void
  popup(ymuint pos);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ヒープ木本体
  vector<DfsNode*> mArray;
  
};

END_NAMESPACE_YM_SEAL

#endif // LIBYM_SEAL_SMATRIX_H
