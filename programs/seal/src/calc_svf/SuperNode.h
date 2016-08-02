#ifndef CALC_SVF_SUPERNODE_H
#define CALC_SVF_SUPERNODE_H

/// @file calc_svf/SuperNode.h
/// @brief SuperNode のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
/// 
/// $Id: SuperNode.h 1920 2008-12-20 15:52:42Z matsunaga $
///
/// Copyright (C) 2005-2008 Yusuke Matsunaga
/// All rights reserved.


#include "nsdef.h"


BEGIN_NAMESPACE_YM_SEAL_SVF

class NodeHeap;

//////////////////////////////////////////////////////////////////////
/// @class SuperNode SuperNode.h "SuperNode.h"
/// @brief 複数のノードを一まとめにしたもの
//////////////////////////////////////////////////////////////////////
class SuperNode
{
  friend class NodeHeap;
  
public:

  /// @brief コンストラクタ
  SuperNode();

  /// @brief デストラクタ
  ~SuperNode();

  
public:
  
  /// @brief ID番号を得る．
  size_t
  id() const;
  
  /// @brief 入力数を得る．
  size_t
  ni() const;

  /// @brief pos 番めの入力を得る．
  SuperNode*
  input(size_t pos) const;

  /// @brief ファンアウト数を得る．
  size_t
  nfo() const;

  /// @brief pos 番めのファンアウトを得る．
  SuperNode*
  fanout(size_t pos) const;


public:

  /// @brief 初期化する．
  void
  init(size_t id,
       SuperNode* inode0,
       SuperNode* inode1,
       size_t max_ni);
  
  /// @brief コストを返す．
  int
  cost();
  
  /// @brief 自分自身をファンアウト先にマージする．
  void
  eliminate(NodeHeap& node_heap);
  
  /// @brief コストを計算する．
  void
  calc_cost();
  

private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ID番号
  size_t mId;

  // ヒープ中の位置
  // ヒープになければ -1
  int mPos;
  
  // 入力の配列
  vector<SuperNode*> mInputs;

  // ファンアウトの配列
  vector<SuperNode*> mFanouts;
  
  // 現在のコスト
  int mCost;

  bool mMark;
  
};


//////////////////////////////////////////////////////////////////////
/// @class NodeHeap SuperNode.h "SuperNode.h"
/// @brief SuperNode のヒープ木
//////////////////////////////////////////////////////////////////////
class NodeHeap
{
public:

  /// @brief コンストラクタ
  /// @param[in] max_size 最大要素数
  NodeHeap(size_t max_size);

  /// @brief デストラクタ
  ~NodeHeap();


public:

  /// @brief 要素数を返す．
  size_t
  num() const;
  
  /// @brief 要素を追加する．
  void
  put_node(SuperNode* node);

  /// @brief もっともコストの小さいノードを取り出す．
  SuperNode*
  get_min();

  /// @brief node のコストを変更する．
  void
  update_cost(SuperNode* node);


private:

  // pos で指定された位置のノードを取り除く
  // そのノードを返す．
  SuperNode*
  delete_node(size_t pos);
  
  // 適当な位置まで沈めて行く
  void
  move_down(size_t pos);

  // 適当な位置まで上げて行く
  void
  move_up(size_t pos);

  // ノードの位置を変える．
  void
  set_pos(size_t pos,
	  SuperNode* node);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 配列のサイズ
  size_t mSize;

  // 要素数
  size_t mNum;

  // 配列本体
  SuperNode** mArray;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief ID番号を得る．
inline
size_t
SuperNode::id() const
{
  return mId;
}

// @brief 入力数を得る．
inline
size_t
SuperNode::ni() const
{
  return mInputs.size();
}

// @brief pos 番めの入力を得る．
inline
SuperNode*
SuperNode::input(size_t pos) const
{
  return mInputs[pos];
}

// @brief ファンアウト数を得る．
inline
size_t
SuperNode::nfo() const
{
  return mFanouts.size();
}

// @brief pos 番めのファンアウトを得る．
inline
SuperNode*
SuperNode::fanout(size_t pos) const
{
  return mFanouts[pos];
}

// @brief コストを返す．
inline
int
SuperNode::cost()
{
  return mCost;
}

// @brief 要素数を返す．
inline
size_t
NodeHeap::num() const
{
  return mNum;
}

// @brief 要素を追加する．
inline
void
NodeHeap::put_node(SuperNode* node)
{
  size_t pos = mNum;
  set_pos(pos, node);
  ++ mNum;
  move_up(pos);
}

// @brief もっともコストの小さいノードを取り出す．
inline
SuperNode*
NodeHeap::get_min()
{
  return delete_node(0);
}

// @brief node のコストを変更する．
inline
void
NodeHeap::update_cost(SuperNode* node)
{
  node->calc_cost();
  delete_node(node->mPos);
  put_node(node);
}

// ノードの位置を変える．
inline
void
NodeHeap::set_pos(size_t pos,
		  SuperNode* node)
{
  mArray[pos] = node;
  node->mPos = pos;
}

END_NAMESPACE_YM_SEAL_SVF

#endif // CALC_SVF_SUPERNODE_H
