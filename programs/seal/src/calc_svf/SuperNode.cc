
/// @file calc_svf/SuperNode.cc
/// @brief SuperNode の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
/// 
/// $Id: SuperNode.cc 1920 2008-12-20 15:52:42Z matsunaga $
///
/// Copyright (C) 2005-2008 Yusuke Matsunaga
/// All rights reserved.

#if HAVE_CONFIG_H
#include "seal_config.h"
#endif


#include "SuperNode.h"


BEGIN_NAMESPACE_YM_SEAL_SVF

//////////////////////////////////////////////////////////////////////
// クラス SuperNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SuperNode::SuperNode() :
  mId(0),
  mPos(-1),
  mCost(0),
  mMark(false)
{
}

// @brief デストラクタ
SuperNode::~SuperNode()
{
}

// @brief 初期化する．
void
SuperNode::init(size_t id,
		SuperNode* inode0,
		SuperNode* inode1,
		size_t max_ni)
{
  mId = id;
  if ( inode0 != NULL && inode1 != NULL ) {
    assert_cond(inode0 != inode1, __FILE__, __LINE__);
    mInputs.resize(2);
    mInputs[0] = inode0;
    inode0->mFanouts.push_back(this);
    mInputs[1] = inode1;
    inode1->mFanouts.push_back(this);
  }
}

// @brief コストを計算する．
void
SuperNode::calc_cost()
{
  mCost = 0;
  for (vector<SuperNode*>::iterator p = mFanouts.begin();
       p != mFanouts.end(); ++ p) {
    SuperNode* onode = *p;
    size_t delta = ni();
    size_t oni = onode->ni();
    for (size_t i = 0; i < oni; ++ i) {
      SuperNode* inode = onode->input(i);
      for (size_t j = 0; j < ni(); ++ j) {
	if ( inode == input(j) ) {
	  -- delta;
	}
      }
    }
    if ( oni + delta - 1 > 30 ) {
      mCost = 0x7FFFFFFF;
      return;
    }
    if ( delta == 0 ) {
      mCost -= (1U << (oni - 1));
    }
    else {
      mCost += ((1UL << (delta - 1)) - 1) * (1UL << oni);
    }
  }
}
  
// @brief 自分自身をファンアウト先にマージする．
void
SuperNode::eliminate(NodeHeap& node_heap)
{
  vector<SuperNode*> tmp;
  for (size_t i = 0; i < ni(); ++ i) {
    SuperNode* inode = input(i);
    // inode のファンアウトから this をとりのぞく．
    size_t ino = inode->nfo();
    size_t wpos = 0;
    for (size_t rpos = 0; rpos < ino; ++ rpos) {
      SuperNode* onode = inode->fanout(rpos);
      if ( onode != this ) {
	if ( wpos != rpos ) {
	  inode->mFanouts[wpos] = onode;
	}
	++ wpos;
      }
    }
    assert_cond(wpos == ino - 1, __FILE__, __LINE__);
    inode->mFanouts.erase(inode->mFanouts.end() - 1,
			  inode->mFanouts.end());
    if ( !inode->mMark ) {
      inode->mMark = true;
      tmp.push_back(inode);
    }
  }
  
  for (vector<SuperNode*>::iterator p = mFanouts.begin();
       p != mFanouts.end(); ++ p) {
    SuperNode* onode = *p;
    if ( !onode->mMark ) {
      onode->mMark = true;
      tmp.push_back(onode);
    }

    size_t oni = onode->ni();
    size_t first_pos = oni;
    for (size_t i = 0; i < oni; ++ i) {
      SuperNode* oinode = onode->input(i);
      if ( oinode == this ) {
	first_pos = i;
      }
      else {
	if ( !oinode->mMark ) {
	  oinode->mMark = true;
	  tmp.push_back(oinode);
	}
      }
    }
    assert_cond(first_pos < oni, __FILE__, __LINE__);
    
    for (size_t i = 0; i < ni(); ++ i) {
      SuperNode* inode0 = input(i);
      // 基本的には inode0 のファンアウトから this を取り除き
      // 代わりに onode を加える処理と
      // onode の入力から this をとりのぞき inode0 を加える処理
      // を行えばよいが，inode0 がすでに onode の入力に含まれて
      // いるときに注意が必要となる．
      
      bool found = false;
      for (size_t j = 0; j < oni; ++ j) {
	SuperNode* inode = onode->input(j);
	if ( inode == inode0 ) {
	  found = true;
	  break;
	}
      }
      if ( !found ) {
	if ( first_pos < oni ) {
	  onode->mInputs[first_pos] = inode0;
	  first_pos = oni;
	}
	else {
	  onode->mInputs.push_back(inode0);
	}
	inode0->mFanouts.push_back(onode);
      }
    }
    if ( first_pos < oni ) {
      // onode の入力から this を取り除く．
      for (size_t i = first_pos + 1; i < oni; ++ i) {
	onode->mInputs[i - 1] = onode->mInputs[i];
      }
      onode->mInputs.erase(onode->mInputs.end() - 1,
			   onode->mInputs.end());
    }
  }

  for (vector<SuperNode*>::iterator p = tmp.begin();
       p != tmp.end(); ++ p) {
    SuperNode* node = *p;
    node->mMark = false;
    if ( node->mPos != -1 ) {
      node_heap.update_cost(node);
    }
  }
}


//////////////////////////////////////////////////////////////////////
// クラス NodeHeap
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] max_size 最大要素数
NodeHeap::NodeHeap(size_t max_size) :
  mSize(max_size),
  mNum(0),
  mArray(new SuperNode*[max_size])
{
}

// @brief デストラクタ
NodeHeap::~NodeHeap()
{
  delete [] mArray;
}

// pos で指定された位置のノードを取り除く
// そのノードを返す．
SuperNode*
NodeHeap::delete_node(size_t pos)
{
  SuperNode* node = mArray[pos];
  -- mNum;
  if ( mNum > 0 ) {
    SuperNode* last = mArray[mNum];
    set_pos(pos, last);
    move_down(pos);
  }
  return node;
}

// 適当な位置まで沈めて行く
void
NodeHeap::move_down(size_t pos)
{
  size_t ppos = pos;
  SuperNode* pnode = mArray[ppos];
  int cost_p = pnode->cost();
  for ( ; ; ) {
    // ppos の子供の位置を求める．
    size_t lpos = ppos + ppos + 1;
    size_t rpos = lpos + 1;
    if ( lpos >= mNum ) {
      // 左右の子供を持たない場合
      break;
    }
    SuperNode* lnode = mArray[lpos];
    int cost_l = lnode->cost();
    if ( rpos == mNum ) {
      // 右の子供を持たない場合
      if ( cost_p > cost_l ) {
	// 逆転
	set_pos(ppos, lnode);
	set_pos(lpos, pnode);
      }
      break;
    }
    else {
      SuperNode* rnode = mArray[rpos];
      int cost_r = rnode->cost();
      if ( cost_p > cost_l &&
	   cost_l <= cost_r ) {
	// 左の子供と取り替える．次は左の子供で同じ処理をする．
	set_pos(ppos, lnode);
	set_pos(lpos, pnode);
	ppos = lpos;
      }
      else if ( cost_p > cost_r &&
		cost_r <= cost_l ) {
	// 右の子供と取り替える．次は左の子供で同じ処理をする．
	set_pos(ppos, rnode);
	set_pos(rpos, pnode);
	ppos = rpos;
      }
      else {
	break;
      }
    }
  }
}

// 適当な位置まで上げて行く
void
NodeHeap::move_up(size_t pos)
{
  SuperNode* node = mArray[pos];
  int cost = node->cost();
  while ( pos > 0 ) {
    size_t ppos = (pos - 1) >> 1;
    SuperNode* pnode = mArray[ppos];
    if ( pnode->cost() <= cost ) {
      break;
    }
    set_pos(ppos, node);
    set_pos(pos, pnode);
    pos = ppos;
  }
}

END_NAMESPACE_YM_SEAL_SVF
