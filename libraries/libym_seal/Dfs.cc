
/// @file libym_seal/Dfs.cc
/// @brief Dfs の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: Dfs.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "Dfs.h"


BEGIN_NAMESPACE_YM_SEAL

// @brief コンストラクタ
// @param[in] num ノード数
Dfs::Dfs(ymuint num) :
  mNum(num),
  mNodeArray(new DfsNode[num])
{
  for (ymuint i = 0; i < num; ++ i) {
    DfsNode* node = &mNodeArray[i];
    node->mId = i;
    node->mVisited = false;
    node->mOnStack = false;
    node->mLink = NULL;
  }
  mRepNodes.reserve(num);
  mStack.reserve(num);
}

// @brief デストラクタ
Dfs::~Dfs()
{
  delete [] mNodeArray;
}

// @biref ノードの接続関係を設定する．
// @param[in] id ノード番号
// @param[in] adjlist 遷移先のノード番号のリスト
void
Dfs::set_adjlist(ymuint id,
		 const vector<ymuint>& adjlist)
{
  DfsNode* node = &mNodeArray[id];
  ymuint n = adjlist.size();
  node->mAdjList.resize(n);
  for (ymuint i = 0; i < n; ++ i) {
    node->mAdjList[i] = &mNodeArray[adjlist[i]];
  }
}

// @brief 遷移元のノードのリストを設定する．
// @param[in] id ノード番号
// @param[in] fromlist 遷移元のノード番号のリスト
void
Dfs::set_fromlist(ymuint id,
		  const list<ymuint>& fromlist)
{
  DfsNode* node = &mNodeArray[id];
  ymuint n = fromlist.size();
  node->mFromList.clear();
  node->mFromList.reserve(n);
  for (list<ymuint>::const_iterator p = fromlist.begin();
       p != fromlist.end(); ++ p) {
    node->mFromList.push_back(&mNodeArray[*p]);
  }
}

// @brief dfs を行って強連結成分を求める．
ymuint
Dfs::scc()
{
  mCount = 0;
  mRepNodes.clear();
  mStack.clear();
  for (ymuint i = 0; i < mNum; ++ i) {
    DfsNode* node = &mNodeArray[i];
    node->mVisited = false;
    node->mOnStack = false;
    node->mLink = NULL;
  }
  for (ymuint i = 0; i < mNum; ++ i) {
    DfsNode* node = &mNodeArray[i];
    if ( !node->mVisited ) {
      dfs(node);
    }
  }
  return mRepNodes.size();
}

// @brief 強連結成分の代表ノードを返す．
// @param[in] id 強連結成分の番号
DfsNode*
Dfs::repnode(ymuint id)
{
  return mRepNodes[id];
}

// @brief scc のために dfs を行う関数
void
Dfs::dfs(DfsNode* node)
{
  node->mVisited = true;
  node->mDefnumber = mCount;
  node->mLowlink = mCount;
  ++ mCount;
  mStack.push_back(node);
  node->mOnStack = true;
  for (vector<DfsNode*>::iterator p = node->mAdjList.begin();
       p != node->mAdjList.end(); ++ p) {
    DfsNode* node1 = *p;
    if ( node1->mVisited ) {
      if ( node1->mDefnumber < node->mDefnumber && node1->mOnStack ) {
	if ( node->mLowlink > node1->mLowlink ) {
	  node->mLowlink = node1->mLowlink;
	}
      }
    }
    else {
      dfs(node1);
      if ( node->mLowlink > node1->mLowlink ) {
	node->mLowlink = node1->mLowlink;
      }
    }
  }
  if ( node->mLowlink == node->mDefnumber ) {
    for (DfsNode* last = NULL; last != node; ) {
      DfsNode* node1 = mStack.back();
      mStack.pop_back();
      node1->mOnStack = false;
      node1->mLink = last;
      last = node1;
    }
    mRepNodes.push_back(node);
  }
}


//////////////////////////////////////////////////////////////////////
// クラス DfsHeap
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] size_hint 容量のヒント
DfsHeap::DfsHeap(ymuint size_hint)
{
  mArray.reserve(size_hint);
}

// @brief デストラクタ
DfsHeap::~DfsHeap()
{
}
  
// @brief 要素が空の時 true を返す．
bool
DfsHeap::empty() const
{
  return mArray.empty();
}

// @brief ノードを追加する．
void
DfsHeap::push(DfsNode* node)
{
  mArray.push_back(node);
  ymuint pos = mArray.size() - 1;
  node->mHeapPos = pos;
  popup(pos);
}

// @brief 最小値を持つノードを取り出す．
DfsNode*
DfsHeap::get_min()
{
  ymuint n = mArray.size();
  if ( n > 0 ) {
    DfsNode* node = mArray[0];
    -- n;
    DfsNode* node0 = mArray[n];
    mArray[0] = node0;
    node0->mHeapPos = 0;
    mArray.pop_back();
    pushdown(0);
    return node;
  }
  else {
    return NULL;
  }
}

// @brief ノードをコストが変化した時の更新処理
void
DfsHeap::rebalance(DfsNode* node)
{
  popup(node->mHeapPos);
}

// @brief ノードを適切な位置まで沈めてゆく関数
void
DfsHeap::pushdown(ymuint ppos)
{
  ymuint n = mArray.size();
  for ( ; ; ) {
    ymuint lpos = ppos + ppos + 1;
    ymuint rpos = lpos + 1;
    if ( rpos > n ) {
      // 左右の子供を持たない場合
      break;
    }
    DfsNode* node_p = mArray[ppos];
    DfsNode* node_l = mArray[lpos];
    if ( rpos == n ) {
      // 右の子供を持たない場合
      if ( node_p->mCost > node_l->mCost ) {
	// 逆転
	mArray[ppos] = node_l;
	node_l->mHeapPos = ppos;
	mArray[lpos] = node_p;
	node_p->mHeapPos = lpos;
      }
      // どちらにせよ最低れべるまで下げられたので次はない．
      break;
    }
    else {
      // 左右の子供を持つ場合
      DfsNode* node_r = mArray[rpos];
      if ( node_p->mCost > node_l->mCost &&
	   node_l->mCost <= node_r->mCost ) {
	// 左の子供と入れ替える．次は左の子供で同じ処理をする．
	mArray[ppos] = node_l;
	node_l->mHeapPos = ppos;
	mArray[lpos] = node_p;
	node_p->mHeapPos = lpos;
	ppos = lpos;
      }
      else if ( node_p->mCost > node_r->mCost &&
		node_r->mCost < node_l->mCost ) {
	// 右の子供と取り替える．次は右の子供で同じ処理をする．
	mArray[ppos] = node_r;
	node_r->mHeapPos = ppos;
	mArray[rpos] = node_p;
	node_p->mHeapPos = rpos;
	ppos = rpos;
      }
      else {
	break;
      }
    }
  }
}

// @brief ノードを適切な位置まで浮かび上がらせる関数
void
DfsHeap::popup(ymuint pos)
{
  while ( pos > 0 ) {
    DfsNode* node = mArray[pos];
    ymuint ppos = (pos - 1) >> 1;
    DfsNode* pnode = mArray[ppos];
    if ( pnode->mCost > node->mCost ) {
      // pnode と node を入れ替える．
      mArray[ppos] = node;
      node->mHeapPos = ppos;
      mArray[pos] = pnode;
      pnode->mHeapPos = pos;
      pos = ppos;
    }
    else {
      break;
    }
  }
}

END_NAMESPACE_YM_SEAL
