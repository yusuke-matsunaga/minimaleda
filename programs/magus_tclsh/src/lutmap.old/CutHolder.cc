
/// @file magus/lutmap/CutHolder.cc
/// @brief CutHolder の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: CutHolder.cc 2274 2009-06-10 07:45:29Z matsunaga $
///
/// Copyright (C) 2005-2007 Yusuke Matsunaga
/// All rights reserved.


#if HAVE_CONFIG_H
#include "magus_config.h"
#endif

#include "CutHolder.h"
#include "Cut.h"
#include "SbjGraph.h"


BEGIN_NAMESPACE_MAGUS_LUTMAP

// コンストラクタ
CutHolder::CutHolder()
{
  mCutList = NULL;
  mCurChunk = NULL;
  mLast = 0;
}

#if 0
// @brief 特殊なコンストラクタ
CutHolder::CutHolder(const CutHolder& src,
		     const SbjGraph& sbjgraph,
		     ymuint limit,
		     const vector<SbjNode*>& dup_list) :
  mCutList(sbjgraph->max_node_id())
{
  SbjNode* dnode = dup_list[0];
  ymuint n = src.mCutList.size();
  for (ymuint i = 0; i < n; ++ i) {
    SbjNode* node = sbjgraph->node(i);
    const CutList& cut_list = src.mCutList[i];
    for (CutListIterator p = cut_list.begin();
	 p != cut_list.end(); ++ p) {
      const Cut* cut = *p;
      Cut* new_cut = duplicate(cut, limit, dup_list);
      if ( new_cut ) {
	mCutList[i].push_back(new_cut);
      }
    }
    if ( node == dnode ) {
      for (ymuint j = 1; j < dup_list.size(); ++ j) {
	for (CutListIterator p = cut_list.begin();
	     p != cut_list.end(); ++ p) {
	  Cut* cut = *p;
	  mCutList[dup_list[j]->id()].push_back(cut);
	}
      }
    }
  }
}
#endif

// デストラクタ
CutHolder::~CutHolder()
{
  clear();
}

// @brief 保持しているカットのリストを削除する．
void
CutHolder::clear()
{
  delete [] mCutList;
  for (vector<char*>::iterator p = mChunk.begin();
       p != mChunk.end(); ++ p) {
    delete [] *p;
  }
  mCurChunk = NULL;
  mLast = 0;
  mChunk.clear();
}

// 最初に呼ばれる関数
void
CutHolder::all_init(const SbjGraph& sbjgraph,
		    ymuint limit,
		    tCutMode mode)
{
  clear();
  ymuint n = sbjgraph.max_node_id();
  mLimit = limit;
  mCutMode = mode;
  mCutList = new CutList[n];
}

void
CutHolder::node_init(SbjNode* node,
		     ymuint pos)
{
}

void
CutHolder::found(SbjNode* root)
{
}

void
CutHolder::found(SbjNode* root,
		 ymuint ni,
		 SbjNode* inputs[])
{
  Cut* cut = alloc_cut(ni);
  cut->mRoot = root;
  cut->mNi = ni;
  for (ymuint i = 0; i < ni; ++ i) {
    cut->mInputs[i] = inputs[i];
  }
  mCutList[root->id()].push_back(cut);
}

void
CutHolder::node_end(SbjNode* node,
		    ymuint pos,
		    ymuint ncuts)
{
}

// 最後に呼ばれる関数
void
CutHolder::all_end(const SbjGraph& sbjgraph,
		   ymuint limit,
		   tCutMode mode)
{
}

#if 0
BEGIN_NONAMESPACE

// 複製を考慮した真の cut を作る．
bool
check_cut(SbjNode* node,
	  ymuint limit,
	  const vector<SbjNode*>& dup_list,
	  vector<SbjNode*>& inputs)
{
  for (ymuint i = 0; i < inputs.size(); ++ i) {
    if ( inputs[i] == node ) {
      return true;
    }
  }
  for (ymuint i = 0; i < dup_list.size(); ++ i) {
    if ( dup_list[i] == node ) {
      if ( inputs.size() == limit ) {
	return false;
      }
      inputs.push_back(node);
      return true;
    }
  }
  for (ymuint i = 0; i < node->ni(); ++ i) {
    bool stat = check_cut(node->fanin(i), limit, dup_list, inputs);
    if ( !stat ) {
      return false;
    }
  }
  return true;
}

END_NONAMESPACE

// ノードの複製を考慮してカットを複製する．
Cut*
CutHolder::duplicate(const Cut* cut,
		     ymuint limit,
		     const vector<SbjNode*>& dup_list)
{
  SbjNode* dnode = dup_list[0];
  ymuint ni = cut->ni();
  bool dup_leaf = false;
  for (ymuint i = 0; i < ni; ++ i) {
    if ( cut->input(i) == dnode ) {
      dup_leaf = true;
      break;
    }
  }
  if ( dup_leaf ) {
    vector<SbjNode*> inputs;
    for (ymuint i = 0; i < ni; ++ i) {
      SbjNode* node = cut->input(i);
      if ( node != dnode ) {
	inputs.push_back(node);
      }
    }
    if ( !check_cut(cut->root(), limit, dup_list, inputs) ) {
      return NULL;
    }
    ymuint new_ni = inputs.size();
    Cut* new_cut = alloc_cut(new_ni);
    new_cut->mRoot = cut->root();
    new_cut->mNi = new_ni;
    for (ymuint i = 0; i < new_ni; ++ i) {
      new_cut->mInputs[i] = inputs[i];
    }
    return new_cut;
  }
  else {
    return cut;
  }
}
#endif

// カットの領域を確保する．
Cut*
CutHolder::alloc_cut(ymuint ni)
{
  ymuint size = sizeof(Cut) + (ni - 1) * sizeof(SbjNode*);
  assert_cond(size < kPageSize, __FILE__, __LINE__);
  if ( mCurChunk == NULL || mLast + size > kPageSize ) {
    mCurChunk = new char[kPageSize];
    mLast = 0;
    mChunk.push_back(mCurChunk);
  }
  char* top = mCurChunk + mLast;
  mLast += size;
  return reinterpret_cast<Cut*>(top);
}

END_NAMESPACE_MAGUS_LUTMAP
