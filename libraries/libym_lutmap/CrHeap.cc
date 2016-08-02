
/// @file libym_lutmap/CrHeap.cc
/// @brief CrHeap の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: CrHeap.cc 2274 2009-06-10 07:45:29Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "CrHeap.h"
#include "CrNode.h"
#include "ym_sbj/SbjGraph.h"


BEGIN_NAMESPACE_YM_LUTMAP

//////////////////////////////////////////////////////////////////////
// クラス CrHeap
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
CrHeap::CrHeap() :
  mNum(0)
{
}

// @brief デストラクタ
CrHeap::~CrHeap()
{
}

// ヒープを初期化する．
void
CrHeap::init(ymuint max_n)
{
  mHeap.resize(max_n);
  mNum = 0;
}

// ヒープに追加する．
void
CrHeap::put(CrNode* node)
{
  set(mNum, node);
  ++ mNum;
  move_up(node->mHeapIndex);
}

// ヒープの先頭要素を抜き出す．
CrNode*
CrHeap::get()
{
  if ( mNum == 0 ) {
    return NULL;
  }

  CrNode* node = get(0);
  remove(node);
  return node;
}

// ヒープから削除する．
void
CrHeap::remove(CrNode* node)
{
  int i = node->mHeapIndex;
  if ( i != -1 ) {
    node->mHeapIndex = -1;
    -- mNum;
    ymuint p0 = static_cast<ymuint>(i);
    if ( mNum > p0 ) {
      CrNode* node1 = get(mNum);
      set(p0, node1);
      move_down(p0);
      move_up(p0);
    }
  }
}

// 値の更新に伴ってヒープの再構築を行う．
void
CrHeap::update(CrNode* node,
	       ymuint new_gain)
{
  int pos = node->mHeapIndex;
  node->mCurGain = new_gain;
  if ( pos != -1 ) {
    move_down(pos);
    move_up(pos);
  }
}

// 引数の位置にある要素を適当な位置まで沈めてゆく
void
CrHeap::move_down(ymuint pos)
{
  for ( ; ; ) {
    // ヒープ木の性質から親から子の位置がわかる
    ymuint left = pos + pos + 1;
    ymuint right = left + 1;
    if ( right > mNum ) {
      // 左右の子どもを持たない場合
      break;
    }
    CrNode* node_p = get(pos);
    CrNode* node_l = get(left);
    if ( right == mNum ) {
      // 右の子どもを持たない場合
      if ( compare(node_l, node_p) ) {
	// 逆転
	set(left, node_p);
	set(pos, node_l);
      }
      break;
    }

    // 左右の子供を持つ場合
    CrNode* node_r = get(right);
    if ( compare(node_l, node_r) ) {
      if ( compare(node_l, node_p) ) {
	// 左の子どもと取り替える．次は左の子で同じ処理をする
	set(left, node_p);
	set(pos, node_l);
	pos = left;
      }
      else {
	break;
      }
    }
    else {
      if ( compare(node_r, node_p) ) {
	// 右の子どもと取り替える．次は右の子で同じ処理をする
	set(right, node_p);
	set(pos, node_r);
	pos = right;
      }
      else {
	break;
      }
    }
  }
}

// 引数の位置にある要素を適当な位置まで上げてゆく
void
CrHeap::move_up(ymuint pos)
{
  while ( pos > 0 ) {
    CrNode* node = get(pos);
    ymuint parent = (pos - 1) >> 1;
    CrNode* node_p = get(parent);
    if ( compare(node, node_p) ) {
      set(parent, node);
      set(pos, node_p);
      pos = parent;
    }
    else {
      break;
    }
  }
}

// pos の位置に node を置く
void
CrHeap::set(ymuint pos,
	    CrNode* node)
{
  mHeap[pos] = node;
  node->mHeapIndex = pos;
}

// pos の位置の要素を返す．
CrNode*
CrHeap::get(ymuint pos)
{
  return mHeap[pos];
}

END_NAMESPACE_YM_LUTMAP
