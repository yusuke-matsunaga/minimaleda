
/// @file calc_cvf/CalcCvf.cc
/// @brief CalcCvf の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
/// 
/// $Id: CalcCvf.cc 1978 2009-02-06 12:29:16Z matsunaga $
///
/// Copyright (C) 2005-2008 Yusuke Matsunaga
/// All rights reserved.

#if HAVE_CONFIG_H
#include "seal_config.h"
#endif


#include "seal_utils.h"
#include "CalcCvf.h"
#include <ym_tgnet/TgNetwork.h>
#include <ym_tgnet/TgNode.h>
#include "SimNode.h"
#include "SimFFR.h"
#include "PoMark.h"
#include "SnInput.h"
#include "SnAnd.h"
#include "SnOr.h"
#include "SnXor.h"
#include "NodeSet.h"


BEGIN_NAMESPACE_YM_SEAL_CVF

//////////////////////////////////////////////////////////////////////
// 故障シミュレータ
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
CalcCvf::CalcCvf() :
  mNodeAlloc(4096)
{
}
  
// @brief デストラクタ
CalcCvf::~CalcCvf()
{
  clear();
}

// @brief 現在保持している SimNode のネットワークを破棄する．
void
CalcCvf::clear()
{
  mSimMap.clear();

  // mNodeArray が全てのノードを持っている
  for (vector<SimNode*>::iterator p = mNodeArray.begin();
       p != mNodeArray.end(); ++ p) {
    SimNode* node = *p;
    // デストラクタを明示的に呼び出す．
    node->~SimNode();
  }
  mNodeArray.clear();
  mInputArray.clear();
  mOutputArray.clear();
  mLogicArray.clear();

  mFFRArray.clear();

  mClearArray.clear();
  
  mNodeAlloc.destroy();
  
  // 念のため
  mNetwork = NULL;
}

// @brief シミュレーション対象のネットワークをセットする．
void
CalcCvf::set_network(const TgNetwork& network,
		     bool dss,
		     bool new_algorithm)
{
  clear();

  mNetwork = &network;

  size_t nn = mNetwork->node_num();
  size_t ni = mNetwork->input_num2();
  size_t no = mNetwork->output_num2();
  size_t nl = mNetwork->logic_num();

  // SimNode の生成
  // 対応付けを行うマップの初期化
  mSimMap.resize(nn);
  mInputArray.resize(ni);
  mOutputArray.resize(no);
  mNodeArray.reserve(nn);
  
  // 外部入力に対応する SimNode の生成
  for (size_t i = 0; i < ni; ++ i) {
    const TgNode* tgnode = mNetwork->input(i);
    SimNode* node = make_node(kTgInput, vector<SimNode*>());
    mSimMap[tgnode->gid()] = node;
    mInputArray[i] = node;
  }
  // 論理ノードに対応する SimNode の生成
  for (size_t i = 0; i < nl; ++ i) {
    const TgNode* tgnode = mNetwork->sorted_logic(i);
    size_t ni = tgnode->ni();
    vector<SimNode*> inputs(ni);
    for (size_t i = 0; i < ni; ++ i) {
      const TgNode* itgnode = tgnode->fanin(i);
      SimNode* inode = find_simnode(itgnode);
      assert_cond(inode, __FILE__, __LINE__);
      inputs[i] = inode;
    }

    // 出力の論理を表す SimNode を作る．
    SimNode* simnode = NULL;
    if ( tgnode->is_cplx_logic() ) {
      LogExpr lexp = mNetwork->get_lexp(tgnode);
      simnode = make_logic(lexp, inputs);
    }
    else {
      tTgGateType type = tgnode->type();
      simnode = make_node(type, inputs);
    }
    mSimMap[tgnode->gid()] = simnode;
  }
  // 外部出力に対応する SimNode の生成
  for (size_t i = 0; i < no; ++ i) {
    const TgNode* onode = mNetwork->output(i);
    SimNode* inode = find_simnode(onode->fanin(0));
    inode->set_output();
    mSimMap[onode->gid()] = inode;
    mOutputArray[i] = inode;
  }
  
  // 各ノードのファンアウト数を数える．
  // complex gate を分解しているので TgNode と異なる場合がある．
  for (vector<SimNode*>::iterator p = mNodeArray.begin();
       p != mNodeArray.end(); ++ p) {
    SimNode* node = *p;
    size_t ni = node->nfi();
    for (size_t i = 0; i < ni; ++ i) {
      SimNode* inode = node->fanin(i);
      ++ inode->mNfo;
    }
  }
  // 各ノードのファンアウト配列を確保
  for (vector<SimNode*>::iterator p = mNodeArray.begin();
       p != mNodeArray.end(); ++ p) {
    SimNode* node = *p;
    if ( node->mNfo > 0 ) {
      void* p = mNodeAlloc.get_memory(sizeof(SimNode*) * node->mNfo);
      node->mFanouts = static_cast<SimNode**>(p);
    }
    node->mNfo = 0;
  }
  for (vector<SimNode*>::iterator p = mNodeArray.begin();
       p != mNodeArray.end(); ++ p) {
    SimNode* node = *p;
    size_t ni = node->nfi();
    for (size_t i = 0; i < ni; ++ i) {
      SimNode* inode = node->fanin(i);
      inode->mFanouts[inode->mNfo] = node;
      ++ inode->mNfo;
    }
  }
  
  // FFR の設定
  size_t node_num = mNodeArray.size();
  size_t fn = 0;
  for (size_t i = node_num; i > 0; ) {
    -- i;
    SimNode* node = mNodeArray[i];
    if ( node->is_output() || node->nfo() != 1 ) {
      ++ fn;
    }
  }
  mFFRArray.resize(fn);
  fn = 0;
  for (size_t i = node_num; i > 0; ) {
    -- i;
    SimNode* node = mNodeArray[i];
    if ( node->is_output() || node->nfo() != 1 ) {
      SimFFR* ffr = &mFFRArray[fn];
      ffr->set_root(node);
      node->mFFR = ffr;
      ++ fn;
    }
    else {
      node->mFFR = node->fanout(0)->mFFR;
    }
  }
  
  // mClearArray の最大サイズは全ノード数
  mClearArray.reserve(node_num);
  
  // 最大レベルを求め，イベントキューを初期化する．
  size_t max_level = 0;
  for (size_t i = 0; i < no; ++ i) {
    SimNode* inode = mOutputArray[i];
    if ( max_level < inode->level() ) {
      max_level = inode->level();
    }
  }
  mEventQ.init(max_level);

  if ( dss ) {
    find_dss(new_algorithm);
  }
}

// @brief DSS を求める．
void
CalcCvf::find_dss(bool new_algorithm)
{
  size_t fn = mFFRArray.size();
  size_t no = mOutputArray.size();
  size_t nn = mNodeArray.size();
  
  // po-mark の設定
  // po-mark は FFR の根のノードでしか計算しない．
  vector<PoMark> pmarray(nn);
  for (size_t i = 0; i < fn; ++ i) {
    SimFFR& ffr = mFFRArray[i];
    SimNode* node = ffr.root();
    pmarray[node->id()].init(no);
  }
  for (size_t i = 0; i < no; ++ i) {
    // もちろん PO は FFR の根のノードになっているはず．
    SimNode* node = mOutputArray[i];
    pmarray[node->id()].set(i);
  }
  for (size_t i = 0; i < fn; ++ i) {
    SimFFR& ffr = mFFRArray[i];
    SimNode* node = ffr.root();
    if ( node->is_output() ) {
      // PO のファンアウト側にあるマークは気にしなくてよい．
      continue;
    }
    PoMark& pomark = pmarray[node->id()];
    size_t nfo = node->nfo();
    for (size_t j = 0; j < nfo; ++ j) {
      SimNode* onode = node->fanout(j);
      SimNode* ffronode = onode->ffr()->root();
      const PoMark& pomark1 = pmarray[ffronode->id()];
      pomark.merge(pomark1);
    }
  }
    

  NodeSet cur_set(nn);
  NodeSet tmp_set(nn);
  vector<SimNode*> next_set;
  next_set.reserve(nn);
  vector<SimNode*> dss;
  dss.reserve(nn);
  NodeSet q0(nn);
  NodeSet q1(nn);
  PoMark sharedpo(no);
  for (size_t i = 0; i < fn; ++ i) {
    SimFFR& ffr = mFFRArray[i];
    SimNode* node = ffr.root();
    
    // node のファンアウトを cur_set に入れる．
    cur_set.clear();
    size_t nfo = node->nfo();
    for (size_t j = 0; j < nfo; ++ j) {
      SimNode* node1 = node->fanout(j);
      cur_set.add(node1);
    }

    dss.clear();
    for ( ; ; ) {
      // cur_set の中で他のノードと PO を共有しないノードを求める．
      // 共有しているノードは next_set にコピーする．
      sharedpo.clear();
      next_set.clear();
      
      // next_set の要素のレベルの最小値
      size_t min_level = 0;
      SimNode* min_node = NULL;
      
      size_t n = cur_set.num();
      for (size_t i = 0; i < n; ++ i) {
	SimNode* node = cur_set.node(i);
	if ( node == NULL ) continue;
	const PoMark& pomark = pmarray[node->ffr()->root()->id()];
	bool shared = false;
	if ( sharedpo && pomark ) {
	  shared = true;
	  sharedpo.merge(pomark);
	}
	else {
	  for (size_t j = i + 1; j < n; ++ j) {
	    SimNode* node1 = cur_set.node(j);
	    if ( node1 == NULL ) continue;
	    
	    const PoMark& pomark1 = pmarray[node1->ffr()->root()->id()];
	    if ( pomark && pomark1 ) {
	      shared = true;
	      sharedpo.merge(pomark);
	      sharedpo.merge(pomark1);
	      next_set.push_back(node1);
	      if ( next_set.size() == 1 || min_level > node1->level() ) {
		min_level = node1->level();
		min_node = node1;
	      }
	      
	      // 処理済みの印として NULL にしておく
	      cur_set.del(j);
	      break;
	    }
	  }
	}
	if ( shared ) {
	  // 他のノードとPOを共有しているので next_set に入れる．
	  next_set.push_back(node);
	  if ( next_set.size() == 1 || min_level > node->level() ) {
	    min_level = node->level();
	    min_node = node;
	  }
	}
	else {
	  // node は DSS だった．
	  dss.push_back(node);
	}
      }
      if ( next_set.empty() ) {
	break;
      }

      if ( new_algorithm ) {
	// min_node とオーバーラップするノードを求める．
	// それ以外のノードは cur_set にコピーする．
	SimFFR* ffr0 = min_node->ffr();
	const PoMark& pomark0 = pmarray[ffr0->root()->id()];
	cur_set.clear();
	tmp_set.clear();
	tmp_set.add(min_node);
	bool local_reconvergent = true;
	for (vector<SimNode*>::iterator p = next_set.begin();
	     p != next_set.end(); ++ p) {
	  SimNode* node = *p;
	  if ( node == min_node) continue;
	  SimFFR* ffr1 = node->ffr();
	  if ( ffr1 == ffr0 ) {
	    tmp_set.add(node);
	  }
	  else {
	    const PoMark& pomark = pmarray[ffr1->root()->id()];
	    int stat = check(pomark0, pomark);
	    if ( stat ) {
	      local_reconvergent = false;
	      // 他の FFR に属するノードがオーバーラップしていたら
	      // 少なくともその出口まで進めなければ状況は変わらない．
	      if ( stat & 1 ) {
		cur_set.add(node);
	      }
	      else {
		cur_set.add(ffr1->root());
	      }
	    }
	    else {
	      // そのまま cur_set にコピーする．
	      cur_set.add(node);
	    }
	  }
	}
	if ( local_reconvergent ) {
	  // tmp に含まれるノードはすべて同一の FFR に含まれる．
	  // tmp 全てに共通の支配点を求める．
	  size_t min_level = 0;
	  SimNode* min_node = NULL;
	  size_t n = tmp_set.num();
	  q0.clear();
	  for (size_t i = 0; i < n; ++ i) {
	    SimNode* node = tmp_set.node(i);
	    q0.add(node);
	    if ( min_node == NULL || min_level > node->level() ) {
	      min_node = node;
	      min_level = node->level();
	    }
	  }
	  size_t cur = 0;
	  for ( ; ; ) {
	    NodeSet& q_cur = (cur == 0) ? q0 : q1;
	    NodeSet& q_next = (cur == 0) ? q1 : q0;
	    size_t n_cur = q_cur.num();
	    size_t min_level = 0;
	    SimNode* next_min_node = NULL;
	    q_next.clear();
	    for (size_t i = 0; i < n_cur; ++ i) {
	      SimNode* node = q_cur.node(i);
	      if ( node == min_node ) {
		SimNode* onode = node->fanout(0);
		q_next.add(onode);
		if ( next_min_node == NULL || min_level > onode->level() ) {
		  next_min_node = onode;
		  min_level = onode->level();
		}
	      }
	      else {
		q_next.add(node);
		if ( next_min_node == NULL || min_level > node->level() ) {
		  next_min_node = node;
		  min_level = node->level();
		}
	      }
	    }
	    assert_cond(q_next.num() > 0, __FILE__, __LINE__);
	    if ( q_next.num() == 1 ) {
	      dss.push_back(q_next.node(0));
	      break;
	    }
	    min_node = next_min_node;
	    cur ^= 1;
	  }
	}
	else {
	  if ( min_node == ffr0->root() ) {
	    // min_node をその DSS と置き換える．
	    const vector<SimNode*>& dss_list = ffr0->dss_list();
	    for (vector<SimNode*>::const_iterator p = dss_list.begin();
		 p != dss_list.end(); ++ p) {
	      SimNode* onode = *p;
	      cur_set.add(onode);
	    }
	  }
	  else {
	    cur_set.add(ffr0->root());
	  }
	}
      }
      else {
	// next_set に含まれるノードのうち，もっとも PI に近いものを
	// そのファンアウトに置き換えて cur_set に入れる．
	// それ以外のノードはそのまま cur_set にコピーする．
	cur_set.clear();
	for (vector<SimNode*>::iterator p = next_set.begin();
	     p != next_set.end(); ++ p) {
	  SimNode* node = *p;
	  if ( node == min_node ) {
	    size_t nfo = node->nfo();
	    for (size_t i = 0; i < nfo; ++ i) {
	      SimNode* node1 = node->fanout(i);
	      cur_set.add(node1);
	    }
	  }
	  else {
	    cur_set.add(node);
	  }
	}
      }
    }
    // DSS を設定する．
    ffr.set_dss(dss);
  }
}


// @brief logic ノードを作る．
SimNode*
CalcCvf::make_logic(const LogExpr& lexp,
		    const vector<SimNode*>& inputs)
{
  SimNode* node = NULL;
  if ( lexp.is_posiliteral() ) {
    size_t pos = lexp.varid();
    node = inputs[pos];
    assert_cond(node, __FILE__, __LINE__);
  }
  else if ( lexp.is_negaliteral() ) {
    size_t pos = lexp.varid();
    SimNode* inode = inputs[pos];
    assert_cond(inode, __FILE__, __LINE__);
    node = make_node(kTgNot, vector<SimNode*>(1, inode));
  }
  else {
    size_t nc = lexp.child_num();
    vector<SimNode*> tmp(nc);
    for (size_t i = 0; i < nc; ++ i) {
      tmp[i] = make_logic(lexp.child(i), inputs);
    }
    if ( lexp.is_and() ) {
      node = make_node(kTgAnd, tmp);
    }
    else if ( lexp.is_or() ) {
      node = make_node(kTgOr, tmp);
    }
    else if ( lexp.is_xor() ) {
      node = make_node(kTgXor, tmp);
    }
  }
  return node;
}

// @brief 単純な logic ノードを作る．
SimNode*
CalcCvf::make_node(tTgGateType type,
		   const vector<SimNode*>& inputs)
{
  ymuint32 id = mNodeArray.size();
  size_t ni = inputs.size();
  void* p;
  SimNode* node = NULL;
  switch ( type ) {
  case kTgInput:
    p = mNodeAlloc.get_memory(sizeof(SnInput));
    node = new (p) SnInput(id);
    break;
    
  case kTgBuff:
    p = mNodeAlloc.get_memory(sizeof(SnBuff));
    node = new (p) SnBuff(id, inputs);
    break;

  case kTgNot:
    p = mNodeAlloc.get_memory(sizeof(SnNot));
    node = new (p) SnNot(id, inputs);
    break;

  case kTgAnd:
    switch ( ni ) {
    case 2:
      p = mNodeAlloc.get_memory(sizeof(SnAnd2));
      node = new (p) SnAnd2(id, inputs);
      break;

    case 3:
      p = mNodeAlloc.get_memory(sizeof(SnAnd3));
      node = new (p) SnAnd3(id, inputs);
      break;
      
    case 4:
      p = mNodeAlloc.get_memory(sizeof(SnAnd4));
      node = new (p) SnAnd4(id, inputs);
      break;
      
    default:
      p = mNodeAlloc.get_memory(sizeof(SnAnd));
      node = new (p) SnAnd(id, inputs);
      break;
    }
    break;
    
  case kTgNand:
    switch ( ni ) {
    case 2:
      p = mNodeAlloc.get_memory(sizeof(SnNand2));
      node = new (p) SnNand2(id, inputs);
      break;

    case 3:
      p = mNodeAlloc.get_memory(sizeof(SnNand3));
      node = new (p) SnNand3(id, inputs);
      break;
      
    case 4:
      p = mNodeAlloc.get_memory(sizeof(SnNand4));
      node = new (p) SnNand4(id, inputs);
      break;

    default:
      p = mNodeAlloc.get_memory(sizeof(SnNand));
      node = new (p) SnNand(id, inputs);
      break;
    }
    break;

  case kTgOr:
    switch ( ni ) {
    case 2:
      p = mNodeAlloc.get_memory(sizeof(SnOr2));
      node = new (p) SnOr2(id, inputs);
      break;
      
    case 3:
      p = mNodeAlloc.get_memory(sizeof(SnOr3));
      node = new (p) SnOr3(id, inputs);
      break;

    case 4:
      p = mNodeAlloc.get_memory(sizeof(SnOr4));
      node = new (p) SnOr4(id, inputs);
      break;
      
    default:
      p = mNodeAlloc.get_memory(sizeof(SnOr));
      node = new (p) SnOr(id, inputs);
      break;
    }
    break;

  case kTgNor:
    switch ( ni ) {
    case 2:
      p = mNodeAlloc.get_memory(sizeof(SnNor2));
      node = new (p) SnNor2(id, inputs);
      break;
      
    case 3:
      p = mNodeAlloc.get_memory(sizeof(SnNor3));
      node = new (p) SnNor3(id, inputs);
      break;
      
    case 4:
      p = mNodeAlloc.get_memory(sizeof(SnNor4));
      node = new (p) SnNor4(id, inputs);
      break;

    default:
      p = mNodeAlloc.get_memory(sizeof(SnNor));
      node = new (p) SnNor(id, inputs);
      break;
    }
    break;

  case kTgXor:
    switch ( ni ) {
    case 2:
      p = mNodeAlloc.get_memory(sizeof(SnXor2));
      node = new (p) SnXor2(id, inputs);
      break;
      
    default:
      p = mNodeAlloc.get_memory(sizeof(SnXor));
      node = new (p) SnXor(id, inputs);
      break;
    }
    break;

  case kTgXnor:
    switch ( ni ) {
    case 2:
      p = mNodeAlloc.get_memory(sizeof(SnXnor2));
      node = new (p) SnXnor2(id, inputs);
      break;
      
    default:
      p = mNodeAlloc.get_memory(sizeof(SnXnor));
      node = new (p) SnXnor(id, inputs);
      break;
    }
    break;

  default:
    assert_not_reached(__FILE__, __LINE__);
    break;
  }
  mNodeArray.push_back(node);
  if ( type != kTgInput ) {
    mLogicArray.push_back(node);
  }
  return node;
}
  
// @brief 正常値のシミュレーションを行う．
// @param[in] tv_array テストベクタの配列
void
CalcCvf::calc_gval(const vector<TestVector*>& tv_array)
{
  size_t nt = tv_array.size();
  size_t ni = mNetwork->input_num2();
  for (size_t i = 0; i < ni; ++ i) {
    tPackedVal val = kPvAll0;
    tPackedVal bit = 1UL;
    for (size_t b = 0; b < nt; ++ b, bit <<= 1) {
      if ( tv_array[b]->val(i) ) {
	val |= bit;
      }
    }
    SimNode* simnode = mInputArray[i];
    simnode->set_gval(val);
  }

  for (vector<SimNode*>::iterator q = mLogicArray.begin();
       q != mLogicArray.end(); ++ q) {
    SimNode* node = *q;
    node->calc_gval();
  }
}

// @brief 全てのノードの出力に対する観測性の計算を行う．
// @param[in] tv_array テストベクタの配列
void
CalcCvf::calc_exact(const vector<TestVector*>& tv_array)
{
  calc_gval(tv_array);
  
  for (vector<SimFFR>::iterator p = mFFRArray.begin();
       p != mFFRArray.end(); ++ p) {
    SimFFR& ffr = *p;
    SimNode* root = ffr.root();
    tPackedVal obs = kPvAll0;
    
    if ( root->is_output() ) {
      // 外部出力ならすべて可観測
      obs = kPvAll1;
    }
    else {
      // このノード(roo) に対する DSS をターゲットとする．
      tPackedVal req1 = ffr.set_target();
      
      // root の値を反転させてその影響が PO で観測されるか調べる．
      tPackedVal pat = root->get_gval() ^ req1;
      root->set_fval(pat);
      mClearArray.clear();
      mClearArray.push_back(root);
      size_t no = root->nfo();
      for (size_t i = 0; i < no; ++ i) {
	mEventQ.put(root->fanout(i));
      }
      for ( ; ; ) {
	SimNode* node = mEventQ.get();
	if ( node == NULL ) break;
	tPackedVal diff = node->calc_fval(~obs);
	if ( diff != kPvAll0 ) {
	  mClearArray.push_back(node);
	  if ( node->target() ) {
	    obs |= diff & node->get_obs();
	  }
	  else if ( node->is_output() ) {
	    obs |= diff;
	  }
	  else {
	    size_t no = node->nfo();
	    for (size_t i = 0; i < no; ++ i) {
	      mEventQ.put(node->fanout(i));
	    }
	  }
	}
      }
      
      // 今の故障シミュレーションで値の変わったノードを元にもどしておく
      for (vector<SimNode*>::iterator p = mClearArray.begin();
	   p != mClearArray.end(); ++ p) {
	(*p)->clear_fval();
      }

      // ターゲットマークを消す．
      ffr.clear_target();
    }

    // FFR 内のノードの obs を計算しセットする．
    root->calc_iobs(obs, true);
  }
}

// @brief 全てのノードの出力に対する観測性の最小値もどきの計算を行う．
// @param[in] tv_array テストベクタの配列
void
CalcCvf::calc_pseudo_min(const vector<TestVector*>& tv_array)
{
  calc_gval(tv_array);
  
  size_t no = mOutputArray.size();
  for (size_t i = 0; i < no; ++ i) {
    SimNode* node = mOutputArray[i];
    node->set_obs(kPvAll1);
  }
  
  size_t nl = mLogicArray.size();
  for (size_t i = nl; i > 0; ) {
    -- i;
    SimNode* node = mLogicArray[i];
    node->calc_pseudo_min_iobs();
  }
}

// @brief 全てのノードの出力に対する観測性の最大値の計算を行う．
// @param[in] tv_array テストベクタの配列
void
CalcCvf::calc_max(const vector<TestVector*>& tv_array,
		  size_t ns)
{
  calc_gval(tv_array);
  
  size_t nl = mLogicArray.size();
  size_t ni = mInputArray.size();
  size_t no = mOutputArray.size();

#if 1
  for (size_t i = 0; i < ni; ++ i) {
    SimNode* node = mInputArray[i];
    node->set_obs(kPvAll1);
  }
  for (size_t i = 0; i < nl; ++ i ) {
    SimNode* node = mLogicArray[i];
    node->set_obs(kPvAll1);
  }

  RandGen randgen;
  for (size_t i = 0; i < ns; ++ i) {
    for (size_t i = 0; i < no; ++ i) {
      SimNode* node = mOutputArray[i];
      node->set_obs2(kPvAll1);
    }
    for (size_t i = nl; i > 0; ) {
      -- i;
      SimNode* node = mLogicArray[i];
      node->calc_max_iobs(randgen);
    }
    for (size_t i = 0; i < ni; ++ i) {
      SimNode* node = mInputArray[i];
      node->and_obs(node->get_obs2());
      node->set_obs2(kPvAll0);
    }
    for (size_t i = 0; i < nl; ++ i) {
      SimNode* node = mLogicArray[i];
      node->and_obs(node->get_obs2());
      node->set_obs2(kPvAll0);
    }
  }
#else
  RandGen randgen;
  for (size_t i = 0; i < no; ++ i) {
    SimNode* node = mOutputArray[i];
    node->set_obs2(kPvAll1);
  }
  for (size_t i = nl; i > 0; ) {
    -- i;
    SimNode* node = mLogicArray[i];
    node->calc_max_iobs(randgen);
  }
  for (size_t i = 0; i < nl; ++ i) {
    SimNode* node = mLogicArray[i];
    if ( node->is_output() || node->nfo() > 1 ) {
      node->calc_iobs(node->get_obs2(), true);
    }
  }
#endif
}

// @brief node の出力における可観測性パタンを返す．
tPackedVal
CalcCvf::get_obs(const TgNode* node)
{
  SimNode* simnode = find_simnode(node);
  return simnode->get_obs();
}

// @brief SimNode のネットワークをダンプする．
void
CalcCvf::dump(ostream& s) const
{
  size_t ni = mNetwork->input_num2();
  size_t no = mNetwork->output_num2();
  size_t nl = mNetwork->logic_num();
  size_t nq = mNetwork->ff_num();

  for (vector<SimNode*>::const_iterator p = mNodeArray.begin();
       p != mNodeArray.end(); ++ p) {
    SimNode* node = *p;
    s << node->id() << " [" << node->level() << "]   = ";
    node->dump(s);
  }
  s << endl;
  s << "# of total nodes: " << mNodeArray.size() << endl
    << "# of FFRs:        " << mFFRArray.size() << endl;

  s << "======= mapping ========" << endl;
  for (size_t i = 0; i < ni; ++ i) {
    const TgNode* node = mNetwork->input(i);
    SimNode* simnode = find_simnode(node);
    if ( i < ni - nq ) {
      s << "INPUT#" << i;
    }
    else {
      s << "FFOUT#" << i;
    }
    s << ":\t" << node->name() << ": " << simnode->id() << endl;
  }
  for (size_t i = 0; i < nl; ++ i) {
    const TgNode* node = mNetwork->logic(i);
    SimNode* simnode = find_simnode(node);
    s << "LOGIC#" << i
      << ": \t" << node->name() << ": " << simnode->id() << endl;
    size_t ni = node->ni();
    for (size_t j = 0; j < ni; ++ j) {
      s << "\t\tI" << j << ": ";
      const TgNode* inode = node->fanin(j);
      SimNode* isimnode = find_simnode(inode);
      s << inode->name() << ": " << isimnode->id() << endl;
    }
  }
  for (size_t i = 0; i < no; ++ i) {
    const TgNode* node = mNetwork->output(i);
    SimNode* simnode = find_simnode(node);
    if ( i < no - nq ) {
      s << "OUTPUT#" << i;
    }
    else {
      s << "FFIN#" << i;
    }
    s << ":\t "
      << node->name() << ": " << simnode->id() << endl;
  }
}

END_NAMESPACE_YM_SEAL_CVF
