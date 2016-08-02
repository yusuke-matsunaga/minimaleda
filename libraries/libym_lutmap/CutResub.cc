
/// @file libym_lutmap/CutResub.cc
/// @brief CutResub の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: CutResub.cc 2274 2009-06-10 07:45:29Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "CutResub.h"
#include "ym_sbj/SbjGraph.h"
#include "Cut.h"
#include "CutHolder.h"
#include "MapRecord.h"

//#define DEBUG_UPDATE

BEGIN_NAMESPACE_YM_LUTMAP

//////////////////////////////////////////////////////////////////////
// クラス CutResub
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CutResub::CutResub() :
  mAlloc(4096)
{
}

// デストラクタ
CutResub::~CutResub()
{
}

// @brief カットの置き換えを行って LUT 数の削減を行う．
// @param[in] sbjgraph サブジェクトグラフ
// @param[in] cut_holder サブジェクトグラフ上のカット集合
// @param[inout] maprec マッピング結果
void
CutResub::operator()(const SbjGraph& sbjgraph,
		     const CutHolder& cut_holder,
		     MapRecord& maprec,
		     int slack)
{
  // 作業領域の初期化(mHeap はあとで)
  ymuint n = sbjgraph.max_node_id();
  mNodeArray.resize(n);
  for (ymuint i = 0; i < n; ++ i) {
    mNodeArray[i] = NULL;
  }
  mHasLevelConstr = (slack >= 0);
  ymuint max_size = sbjgraph.level();
  mGQ.init(max_size);
  mLQ.init(max_size);
  mRQ.init(max_size);

  vector<const SbjNode*> snode_list;
  sbjgraph.sort(snode_list);

  // 外部入力ノードの対応付けを行う．
  vector<const SbjNode*> tmp_list;
  sbjgraph.ppi_list(tmp_list);
  for (vector<const SbjNode*>::const_iterator p = tmp_list.begin();
       p != tmp_list.end(); ++ p) {
    const SbjNode* sbjnode = *p;
    CrNode* node = alloc_node();
    mNodeArray[sbjnode->id()] = node;
    node->set_sbjnode(sbjnode);
  }

  // 外部出力からバックトレースを行う．
  sbjgraph.ppo_list(tmp_list);
  for (vector<const SbjNode*>::const_iterator p = tmp_list.begin();
       p != tmp_list.end(); ++ p) {
    const SbjNode* onode = *p;
    const SbjNode* sbjnode = onode->fanin(0);
    if ( sbjnode && !sbjnode->is_ppi() ) {
      back_trace(sbjnode, maprec, NULL);
    }
  }

  // 置き換えのための初期計算を行う．
  {
    // 現在のカットの根のノードを root_list に格納
    // 代理カットを求める．
    vector<CrNode*> root_list;
    root_list.reserve(sbjgraph.max_node_id());
    ymuint max_level = 0;
    for (vector<const SbjNode*>::const_iterator p = snode_list.begin();
	 p != snode_list.end(); ++ p) {
      const SbjNode* node = *p;
      CrNode* crnode = mNodeArray[node->id()];
      if ( crnode == NULL ) continue;

      root_list.push_back(crnode);

      // crnode のレベルを求める．
      ymuint level = 0;
      const Cut* cut = crnode->cut();
      ymuint ni = cut->ni();
      for (ymuint i = 0; i < ni; ++ i) {
	CrNode* inode = cut_input(cut, i);
	if ( inode->is_input() ) continue;
	ymuint level1 = inode->level();
	if ( level < level1 ) {
	  level = level1;
	}
      }
      ++ level;
      crnode->set_level(level);
      if ( max_level < level ) {
	max_level = level;
      }

      // 代理カットを求める．
      const CutList& cut_list = cut_holder.cut_list(node);
      for(CutListIterator p = cut_list.begin();
	  p != cut_list.end(); ++ p) {
	const Cut* cut1 = *p;
	if ( cut1 == cut ) continue;

	bool ok = true;
	ymuint ni = cut1->ni();
	for (ymuint i = 0; i < ni; ++ i) {
	  CrNode* inode = cut_input(cut1, i);
	  if ( inode == NULL ) {
	    ok = false;
	    break;
	  }
	}
	if ( ok ) {
	  crnode->mAltCutList.push_back(cut1);
	}
      }
    }

    // ゲインの計算を行い，置き換え可能なノードをヒープにつむ．
    mHeap.init(root_list.size());
    for (vector<CrNode*>::const_iterator p = root_list.begin();
	 p != root_list.end(); ++ p) {
      CrNode* node = *p;
      node->set_gain(calc_gain(node));

      if ( check_structure(node) ) {
	mHeap.put(node);
      }
    }

    if ( mHasLevelConstr ) {
      // 要求レベルの計算を行う．
      ymuint poreq = max_level + slack;
      for (vector<CrNode*>::reverse_iterator p = root_list.rbegin();
	   p != root_list.rend(); ++ p) {
	CrNode* node = *p;
	if ( node->fanout_list().empty() ) {
	  if ( node->is_output() ) {
	    node->set_req_level(poreq);
	  }
	}
	else {
	  const vector<CrNode*>& folist = node->fanout_list();
	  vector<CrNode*>::const_iterator q = folist.begin();
	  ymuint req = (*q)->req_level();
	  for ( ; q != folist.end(); ++ q) {
	    CrNode* fonode = *q;
	    ymuint r1 = fonode->req_level();
	    if ( req > r1 ) {
	      req = r1;
	    }
	  }
	  node->set_req_level(req - 1);
	}
      }
    }
  }

  // 改善ループ
  vector<const Cut*> subst_list;
  if ( mHasLevelConstr ) {
#if 0
    for ( ; ; ) {
      bool changed = false;
      for (vector<SbjNode*>::const_reverse_iterator p = snode_list.rbegin();
	   p != snode_list.rend(); ++ p) {
	const SbjNode* node = *p;
	CrNode* crnode = mNodeArray[node->id()];
	if ( crnode == NULL || crnode->is_output() ) continue;
	if ( find_subst2(crnode, subst_list) ) {
	  update(crnode, subst_list);
	  changed = true;
	}
      }
      if ( !changed ) {
	break;
      }
    }
#else
    for ( ; ; ) {
      CrNode* node = mHeap.get();
      if ( node == NULL ) {
	break;
      }
      if ( node->gain() == 0 ) continue;
      if ( find_subst2(node, subst_list) ) {
	update(node, subst_list);
      }
    }
#endif
  }
  else {
    for ( ; ; ) {
      CrNode* node = mHeap.get();
      if ( node == NULL ) {
	break;
      }
      if ( node->gain() == 0) continue;
      if ( find_subst(node, subst_list) ) {
	// 情報の更新
	update(node, subst_list);
      }
    }
  }

  // 最終的なカットを maprec にコピーする．
  // 作業領域をクリアする．
  for (ymuint i = 0; i < n; ++ i) {
    CrNode* node = mNodeArray[i];
    if ( node ) {
      maprec.set_cut(node->sbjnode(), node->cut());
      free_node(node);
    }
  }
  mNodeArray.clear();
}

// node の最適カットを選ぶ．
void
CutResub::back_trace(const SbjNode* sbjnode,
		     MapRecord& maprec,
		     CrNode* from)
{
  CrNode* node = mNodeArray[sbjnode->id()];
  if ( node == NULL ) {
    node = alloc_node();
    mNodeArray[sbjnode->id()] = node;
    node->set_sbjnode(sbjnode);
    const Cut* cut = maprec.get_cut(sbjnode);
    assert_cond(cut != NULL, __FILE__, __LINE__);
    node->set_cut(cut);

    // ファンインのノードのカットを選ぶ．
    // 同時にファンアウトリストを構築する．
    for (ymuint i = 0; i < cut->ni(); ++ i) {
      const SbjNode* inode = cut->input(i);
      if ( !inode->is_ppi() ) {
	back_trace(inode, maprec, node);
      }
    }
  }
  if ( from ) {
    node->add_fanout(from);
  }
}

// ゲインの計算を行う．
ymuint
CutResub::calc_gain(CrNode* node)
{
  if ( node->is_output() ) return 0;

  const Cut* cut = node->cut();
  ymuint ni = cut->ni();
  ymuint gain = 1;
  for (ymuint i = 0; i < ni; ++ i) {
    CrNode* inode = cut_input(cut, i);
    if ( !inode->is_input() && inode->fanout_list().size() == 1 ) {
      gain += inode->gain();
    }
  }
  return gain;
}

// 構造のみで置き換えが可能かどうか判断する．
bool
CutResub::check_structure(CrNode* node)
{
  if ( node->is_output() ) {
    return false;
  }

  const vector<CrNode*>& fo_list = node->fanout_list();
  for(vector<CrNode*>::const_iterator p = fo_list.begin();
      p != fo_list.end(); ++ p) {
    CrNode* fo = *p;
    bool found = false;
    const vector<const Cut*>& cut_list = fo->mAltCutList;
    for (vector<const Cut*>::const_iterator p = cut_list.begin();
	 p != cut_list.end(); ++ p) {
      const Cut* cut = *p;
      ymuint ni = cut->ni();
      bool ok = true;
      for (ymuint i = 0; i < ni; ++ i) {
	CrNode* inode = cut_input(cut, i);
	if ( inode == node ) {
	  ok = false;
	  break;
	}
      }
      if ( ok ) {
	found = true;
	break;
      }
    }
    if ( !found ) {
      return false;
    }
  }
  return true;
}

// node を冗長にする置き換えカットを求める．
bool
CutResub::find_subst(CrNode* node,
		     vector<const Cut*>& subst_list)
{
  subst_list.clear();
  const vector<CrNode*>& fo_list = node->fanout_list();
  for(vector<CrNode*>::const_iterator p = fo_list.begin();
      p != fo_list.end(); ++ p) {
    CrNode* fo = *p;
    bool found = false;
    const vector<const Cut*>& cut_list = fo->mAltCutList;
    for (vector<const Cut*>::const_iterator p = cut_list.begin();
	 p != cut_list.end(); ++ p) {
      const Cut* cut = *p;
      ymuint ni = cut->ni();
      bool ok = true;
      for (ymuint i = 0; i < ni; ++ i) {
	CrNode* inode = cut_input(cut, i);
	if ( inode == NULL || inode == node ) {
	  ok = false;
	  break;
	}
      }
      if ( ok ) {
	found = true;
	subst_list.push_back(cut);
	break;
      }
    }
    if ( !found ) {
      return false;
    }
  }
  return true;
}

BEGIN_NONAMESPACE

struct CrNodeLt
{
  bool
  operator()(CrNode* node1,
	     CrNode* node2)
  {
    return node1->sbjnode()->level() < node2->sbjnode()->level();
  }
};

END_NONAMESPACE

// レベルを考慮しつつ node を冗長にする置き換えカットを求める．
bool
CutResub::find_subst2(CrNode* node,
		      vector<const Cut*>& subst_list)
{
  subst_list.clear();

  // 現在処理中のノードを「ロック」しておく
  const vector<CrNode*>& fo_list = node->fanout_list();
  for(vector<CrNode*>::const_iterator p = fo_list.begin();
      p != fo_list.end(); ++ p) {
    CrNode* fo = *p;
    fo->lock();
  }

  // 処理順を SbjNode のレベルの低い順にする．
  // こうすることによって fo が他のノードをファンインに持つ場合でも
  // ただしくレベルの計算が行える．
  bool ans = true;
  vector<CrNode*> tmp_list(fo_list.begin(), fo_list.end());
  sort(tmp_list.begin(), tmp_list.end(), CrNodeLt());
  for(vector<CrNode*>::const_iterator p = tmp_list.begin();
      p != tmp_list.end(); ++ p) {
    CrNode* fo = *p;
    const Cut* best_cut = NULL;
    ymuint best_level = 0;
    const vector<const Cut*>& cut_list = fo->mAltCutList;
    for (vector<const Cut*>::const_iterator p = cut_list.begin();
	 p != cut_list.end(); ++ p) {
      const Cut* cut = *p;
      ymuint ni = cut->ni();
      bool ok = true;
      ymuint level = 0;
      for (ymuint i = 0; i < ni; ++ i) {
	CrNode* inode = cut_input(cut, i);
	if ( inode == NULL || inode == node ) {
	  ok = false;
	  break;
	}
	// 現在処理中のノードの場合 level() は使えない．
	ymuint level1 = inode->is_locked() ? inode->mTmpLevel : inode->level();
	if ( level < level1 ) {
	  level = level1;
	}
      }
      level += 1;
      if ( !ok || level > fo->req_level() ) {
	continue;
      }

      if ( best_cut == NULL || best_level > level ) {
	best_cut = cut;
	best_level = level;
      }
    }
    if ( best_cut ) {
      subst_list.push_back(best_cut);
      fo->mTmpLevel = best_level;
    }
    else {
      ans =false;
      break;
    }
  }

  for(vector<CrNode*>::const_iterator p = fo_list.begin();
      p != fo_list.end(); ++ p) {
    CrNode* fo = *p;
    fo->unlock();
  }

  return ans;
}

void
CutResub::update(CrNode* node,
		 const vector<const Cut*>& subst_list)
{
#ifdef DEBUG_UPDATE
  cout << "update at " << node->sbjnode()->id_str()
       << " begins" << endl;
#endif

  mGQ.clear();
  mLQ.clear();
  mRQ.clear();

  // カットの置き換えを行う．
  // 古いカットは mDeletedCuts に入れる．
  mDeletedCuts.clear();
  for (vector<const Cut*>::const_iterator p = subst_list.begin();
       p != subst_list.end(); ++ p) {
    const Cut* new_cut = *p;
    CrNode* root = cut_root(new_cut);
    const Cut* old_cut = root->cut();
    subst_cut_fanouts(root, old_cut, new_cut);
    root->set_cut(new_cut);

    mDeletedCuts.push_back(old_cut);
    put_lq(root);

#ifdef DEBUG_UPDATE
    cout << "subst cut at " << root->sbjnode()->id_str() << endl;
    old_cut->dump(cout);
    new_cut->dump(cout);
    cout << endl;
#endif
  }

  // mDeletedCuts に含まれるカットの入力が他にファンアウトしていなければ
  // そのカットも削除する．
  mDeletedNodes.clear();
  for (vector<const Cut*>::iterator p = mDeletedCuts.begin();
       p != mDeletedCuts.end(); ++ p ){
    const Cut* cut = *p;
    ymuint ni = cut->ni();
    for (ymuint i = 0; i < ni; ++ i) {
      CrNode* inode = cut_input(cut, i);
      if ( inode->is_input() ) continue;
      if ( !inode->is_output() &&
	   !inode->deleted() &&
	   inode->fanout_list().empty() ) {
	inode->set_deleted();
	mDeletedNodes.push_back(inode);
      }
    }
  }
  while ( !mDeletedNodes.empty() ) {
    CrNode* crnode = mDeletedNodes.back();
    mDeletedNodes.pop_back();

#ifdef DEBUG_UPDATE
    cout << " remove cut at " << crnode->sbjnode()->id_str() << endl;
#endif

    const Cut* cut = crnode->cut();
    ymuint ni = cut->ni();
    for ( ymuint i = 0; i < ni; ++ i) {
      CrNode* inode = cut_input(cut, i);
      if ( inode->is_input() ) continue;
      inode->delete_fanout(crnode);
      put_rq(inode);
      if ( !inode->is_output() &&
	   !inode->deleted() &&
	   inode->fanout_list().empty() ) {
	inode->set_deleted();
	mDeletedNodes.push_back(inode);
      }
    }
    mNodeArray[crnode->sbjnode()->id()] = NULL;
    mHeap.remove(crnode);
    free_node(crnode);
  }

  // ゲインの再計算
  while ( mGQ.num() > 0 ) {
    CrNode* node = get_gq();
    if ( node->deleted() ) {
      continue;
    }
    ymuint new_gain = calc_gain(node);
    if ( node->gain() != new_gain ) {
      mHeap.update(node, new_gain);
      const vector<CrNode*>& folist = node->fanout_list();
      if ( folist.size() == 1 ) {
	CrNode* fo = folist[0];
	put_gq(fo);
      }
    }
  }

  if ( mHasLevelConstr ) {
    // レベルの再計算
    while ( mLQ.num() > 0 ) {
      CrNode* node = get_lq();
      const Cut* cut = node->cut();
      ymuint max_level = 0;
      ymuint ni = cut->ni();
      for (ymuint i = 0; i < ni; ++ i) {
	CrNode* inode = cut_input(cut, i);
	ymuint level = inode->level();
	if ( max_level < level ) {
	  max_level = level;
	}
      }
      max_level += 1;
      if ( node->level() != max_level ) {
	node->set_level(max_level);
	const vector<CrNode*>& fo_list = node->fanout_list();
	for (vector<CrNode*>::const_iterator p = fo_list.begin();
	     p != fo_list.end(); ++ p) {
	  CrNode* fo = *p;
	  put_lq(fo);
	}
      }
    }

    // 要求レベルの再計算
    while ( mRQ.num() > 0 ) {
      CrNode* node = get_rq();
      if ( node->deleted() ) {
	continue;
      }
      const vector<CrNode*>& fo_list = node->fanout_list();
#if 0
      if ( fo_list.empty() ) continue;
#endif
      vector<CrNode*>::const_iterator p = fo_list.begin();
      ymuint min_req = (*p)->req_level();
      for ( ; p != fo_list.end(); ++ p) {
	CrNode* fo = *p;
	ymuint req = fo->req_level();
	if ( min_req > req ) {
	  min_req = req;
	}
      }
      min_req -= 1;
      if ( node->req_level() != min_req ) {
	node->set_req_level(min_req);
	const Cut* cut = node->cut();
	ymuint ni = cut->ni();
	for (ymuint i = 0; i < ni; ++ i) {
	  CrNode* inode = cut_input(cut, i);
	  if ( !inode->is_input() ) {
	    put_rq(inode);
	  }
	}
      }
    }
  }

#ifdef DEBUG_UPDATE
  cout << "update ends" << endl << endl;
#endif
}

// node のカットを old_cut から new_cut に置き換える．
void
CutResub::subst_cut_fanouts(CrNode* node,
			    const Cut* old_cut,
			    const Cut* new_cut)
{
  ymuint old_ni = old_cut->ni();
  ymuint new_ni = new_cut->ni();

  // old/new フラグをつける．
  for (ymuint i = 0; i < old_ni; ++ i) {
    CrNode* inode = cut_input(old_cut, i);
    inode->set_oldmark();
  }
  for (ymuint i = 0; i < new_ni; ++ i) {
    CrNode* inode = cut_input(new_cut, i);
    inode->set_newmark();
  }

  // 古いファンインで新しいファンインに含まれていないものの処理
  for (ymuint i = 0; i < old_ni; ++ i) {
    CrNode* inode = cut_input(old_cut, i);
    if ( inode->is_input() || inode->newmark() ) continue;

    inode->delete_fanout(node);
    const vector<CrNode*>& fo_list = inode->fanout_list();
    if ( fo_list.size() == 1 ) {
      CrNode* fo = fo_list[0];
      put_gq(fo);
    }
    put_rq(inode);
  }

  // 新しいファンインで古いファンインに含まれていないものの処理
  for (ymuint i = 0; i < new_ni; ++ i) {
    CrNode* inode = cut_input(new_cut, i);
    if ( inode->is_input() || inode->oldmark() ) continue;

    if ( inode->add_fanout(node) ) {
      const vector<CrNode*>& fo_list = inode->fanout_list();
      if ( fo_list.size() == 2 ) {
	CrNode* fo = fo_list[0];
	put_gq(fo);
      }
      put_gq(node);
      put_rq(inode);
    }
  }

  // old/new フラグを消す．
  for (ymuint i = 0; i < old_ni; ++ i) {
    CrNode* inode = cut_input(old_cut, i);
    inode->clear_oldmark();
  }
  for (ymuint i = 0; i < new_ni; ++ i) {
    CrNode* inode = cut_input(new_cut, i);
    inode->clear_newmark();
  }
}

// ゲイン計算用のキューにつむ．
void
CutResub::put_gq(CrNode* node)
{
  if ( !node->is_output() && !node->in_GQ() ) {
    node->set_GQ();
    mGQ.put(node);
  }
}

// ゲイン計算用のキューから取り出す．
CrNode*
CutResub::get_gq()
{
  CrNode* node = mGQ.getmin();
  node->clear_GQ();
  return node;
}

// レベル計算用のキューにつむ．
void
CutResub::put_lq(CrNode* node)
{
  if ( !node->in_LQ() ) {
    node->set_LQ();
    mLQ.put(node);
  }
}

// レベル計算用のキューから取り出す．
CrNode*
CutResub::get_lq()
{
  CrNode* node = mLQ.getmin();
  node->clear_LQ();
  return node;
}

// 要求レベル計算用のキューにつむ．
void
CutResub::put_rq(CrNode* node)
{
  if ( !node->in_RQ() ) {
    node->set_RQ();
    mRQ.put(node);
  }
}

// 要求レベル計算用のキューから取り出す．
CrNode*
CutResub::get_rq()
{
  CrNode* node = mRQ.getmin();
  node->clear_RQ();
  return node;
}

// カットの根に対応するノードを取り出す．
CrNode*
CutResub::cut_root(const Cut* cut)
{
  return mNodeArray[cut->root()->id()];
}

// カットの入力に対応するノードを取り出す．
CrNode*
CutResub::cut_input(const Cut* cut,
		    ymuint pos)
{
  return mNodeArray[cut->input(pos)->id()];
}

// CrNode を確保する．
CrNode*
CutResub::alloc_node()
{
  if ( mGarbageList.empty() ) {
    void* p =  mAlloc.get_memory(sizeof(CrNode));
    return new (p) CrNode;
  }
  else {
    CrNode* node = mGarbageList.back();
    mGarbageList.pop_back();
    return node;
  }
}

// CrNode を解放する．
void
CutResub::free_node(CrNode* node)
{
  mGarbageList.push_back(node);
}

END_NAMESPACE_YM_LUTMAP
