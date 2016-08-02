
/// @file libym_techmap/PatMatcher.cc
/// @brief PatMatcher の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "PatMatcher.h"
#include "ym_sbj/SbjGraph.h"
#include "ym_techmap/PatMgr.h"
#include "PatGraph.h"
#include "Match.h"


BEGIN_NAMESPACE_YM_TECHMAP

// @brief コンストラクタ
// @param[in] pat_mgr パタンを管理するクラス
PatMatcher::PatMatcher(const PatMgr& pat_mgr) :
  mPatMgr(pat_mgr),
  mSbjMap(pat_mgr.node_num(), NULL),
  mInvMap(pat_mgr.node_num(), false),
  mLeafNodeArray(pat_mgr.max_input()),
  mLeafInvArray(pat_mgr.max_input())
{
}

// @brief デストラクタ
PatMatcher::~PatMatcher()
{
}

// @brief パタンマッチングを行う．
// @param[in] sbj_root サブジェクトグラフの根のノード
// @param[in] pat_graph パタングラフ
// @param[out] match マッチング結果
// @retval true マッチした．
// @retval false マッチしなかった．
// @note input_map の中身は (SbjNode->i() << 1) | pol
bool
PatMatcher::operator()(const SbjNode* sbj_root,
		       const PatGraph& pat_graph)
{
  // 根のノードを調べる．
  ymuint root_id = pat_graph.root_id();
  switch ( mPatMgr.node_type(root_id) ) {
  case PatMgr::kInput:
    // これはなんでも OK
    break;

  case PatMgr::kAnd:
    if ( !sbj_root->is_and() ) {
      // 型が違う．
      return false;
    }
    break;

  case PatMgr::kXor:
    if ( !sbj_root->is_xor() ) {
      // 型が違う．
      return false;
    }
    break;

  default:
    assert_not_reached(__FILE__, __LINE__);
    break;
  }
  bind(sbj_root, root_id, false);

  bool success = false;
  // 各枝の入力と出力の対応を調べる．
  ymuint ne = pat_graph.edge_num();
  for (ymuint i = 0; i < ne; ++ i) {
    ymuint edge_id = pat_graph.edge(i);
    ymuint to_id = mPatMgr.edge_to(edge_id);
    ymuint from_id = mPatMgr.edge_from(edge_id);
    ymuint f_pos = mPatMgr.edge_pos(edge_id);
    const SbjNode* to_node = mSbjMap[to_id];
    const SbjNode* from_node = to_node->fanin(f_pos);
    bool iinv = to_node->fanin_inv(f_pos);
    bool inv = false;
    switch ( mPatMgr.node_type(from_id) ) {
    case PatMgr::kInput:
      // どんな型でも OK
      // 極性が違っても OK
      inv =  mPatMgr.edge_inv(edge_id) ^ iinv;
      break;

    case PatMgr::kAnd:
      if ( !from_node->is_and() ) {
	// 型が違う
	goto end;
      }
      if ( mPatMgr.edge_inv(edge_id) != iinv ) {
	// 極性が違う
	goto end;
      }
      break;

    case PatMgr::kXor:
      if ( !from_node->is_xor() ) {
	// 型が違う
	goto end;
      }
      if ( mPatMgr.edge_inv(edge_id) != iinv ) {
	// 極性が違う
	goto end;
      }
      break;

    default:
      assert_not_reached(__FILE__, __LINE__);
      break;
    }
    if ( !bind(from_node, from_id, inv) ) {
      goto end;
    }
  }

  { // 成功した．
    success= true;
    ymuint ni = pat_graph.input_num();
    for (ymuint i = 0; i < ni; ++ i) {
      ymuint node_id = mPatMgr.input_node(i);
      mLeafNodeArray[i] = mSbjMap[node_id];
      mLeafInvArray[i] = mInvMap[node_id];
    }
  }

 end:// 後始末．

  for (vector<ymuint>::iterator p = mClearQueue.begin();
       p != mClearQueue.end(); ++ p) {
    ymuint id = *p;
    mSbjMap[id] = NULL;
    mInvMap[id] = false;
  }
  mClearQueue.clear();
  mPatMap.clear();

  return success;
}

// @brief サブジェクトノードとパタンノードをバインドする．
// @param[in] sbj_node サブジェクトノード
// @param[in] pat_id パタンノードのID
// @param[in] inv 反転フラグ
// @retval true バインドが成功した．
// @retval false バインドが失敗した．
bool
PatMatcher::bind(const SbjNode* sbj_node,
		 ymuint pat_id,
		 bool inv)
{
  if ( mSbjMap[pat_id] != NULL ) {
    if ( mSbjMap[pat_id] != sbj_node ) {
      return false;
    }
    if ( mInvMap[pat_id] != inv ) {
      return false;
    }
  }
  else {
    mSbjMap[pat_id] = sbj_node;
    mInvMap[pat_id] = inv;
    mClearQueue.push_back(pat_id);
  }

  hash_map<ymuint, ymuint>::iterator p = mPatMap.find(sbj_node->id());
  if ( p != mPatMap.end() ) {
    if ( p->second != pat_id ) {
      return false;
    }
  }
  else {
    mPatMap.insert(make_pair(sbj_node->id(), pat_id));
  }

  return true;
}

END_NAMESPACE_YM_TECHMAP
