
/// @file src/fsim3/Fsim3.cc
/// @brief Fsim3 の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: Fsim3.cc 2203 2009-04-16 05:04:40Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "Fsim3.h"
#include "ym_tgnet/TgNetwork.h"
#include "ym_tgnet/TgNode.h"
#include "SimNode.h"
#include "SimFFR.h"
#include "SaFault.h"
#include "TestVector.h"
#include "ym_utils/StopWatch.h"


BEGIN_NAMESPACE_YM_ATPG_FSIM3

BEGIN_NONAMESPACE

void
clear_lobs(SimNode* node)
{
  while ( node->check_lobs() ) {
    node->clear_lobs();
    node = node->fanout(0);
  }
}

END_NONAMESPACE

//////////////////////////////////////////////////////////////////////
// Fsim
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
Fsim3::Fsim3()
{
}

// @brief デストラクタ
Fsim3::~Fsim3()
{
  clear();
}
  
// @brief 対象の故障をセットする
// @param[in] flist 対象の故障リスト
void
Fsim3::set_faults(const vector<SaFault*>& flist)
{
  clear_faults();
  
  ymuint n = flist.size();
  mFsimFaults.resize(n);
  for (ymuint i = 0; i < n; ++ i) {
    SaFault* f = flist[i];
    const TgNode* node = f->node();
    SimNode* simnode = NULL;
    ymuint ipos = 0;
    SimNode* isimnode = NULL;
    if ( f->is_input_fault() ) {
      find_simedge(node, f->pos(), simnode, ipos);
      const TgNode* inode = node->fanin(f->pos());
      isimnode = find_simnode(inode);
    }
    else {
      simnode = find_simnode(node);
      isimnode = simnode;
    }
    mFsimFaults[i].set(f, simnode, ipos, isimnode);
    SimFFR* ffr = simnode->ffr();
    ffr->fault_list().push_back(&mFsimFaults[i]);
  }
}

// @brief 故障シミュレーションを行う．
// @param[in] tv テストベクタ
// @param[out] det_faults 検出された故障を格納するリスト
void
Fsim3::run(TestVector* tv,
	   list<SaFault*>& det_faults)
{
  det_faults.clear();
  
  ymuint npi = mNetwork->input_num2();
  for (ymuint i = 0; i < npi; ++ i) {
    SimNode* simnode = mInputArray[i];
    simnode->set_gval(tv->val3(i));
  }
  
  // 正常値の計算を行う．
  calc_gval();

  // FFR ごとに処理を行う．
  for (vector<SimFFR>::iterator p = mFFRArray.begin();
       p != mFFRArray.end(); ++ p) {
    SimFFR* ffr = &(*p);
    if ( ffr->fault_list().empty() ) continue;
    
    SimNode* root = ffr->root();
    Val3 gval = root->gval();
    
    // FFR の根の値が X なら故障の検出はできない．
    if ( gval == kValX ) continue;

    // FFR 内の故障伝搬を行う．
    // 結果は Fsim3Fault.mObsMask に保存される．
    // FFR 内の全ての obs マスクを ffr_req に入れる．
    // 検出済みの故障は ffr->fault_list() から取り除かれる．
    vector<FsimFault*> det_flist;
    bool ffr_req = ffr_simulate(ffr, det_flist);

    // ffr_req が 0 ならその後のシミュレーションを行う必要はない．
    if ( ffr_req == false ) {
      continue;
    }

    if ( !root->is_output() ) {
      // ffr から外部出力までの故障シミュレーションを行う．
      bool obs = calc_fval(root);
      if ( !obs ) {
	continue;
      }
    }

    for (vector<FsimFault*>::iterator p = det_flist.begin();
	 p != det_flist.end(); ++ p) {
      FsimFault* ff = *p;
      SaFault* f = ff->mOrigF;
      det_faults.push_back(f);
    }
  }
  
  // 値をクリアする．
  clear_gval();
}

// @brief 故障シミュレーションを行う．
// @param[in] tv_array テストベクタの配列
// @param[out] det_faults 検出された故障を格納するリストの配列
void
Fsim3::run(const vector<TestVector*>& tv_array,
	   vector<list<SaFault*> >& det_faults)
{
  ymuint nb = tv_array.size();
  assert_cond(det_faults.size() >= nb, __FILE__, __LINE__);
  
  // このクラスは複数パタンを扱えない．
  for (ymuint i = 0; i < nb; ++ i) {
    run(tv_array[i], det_faults[i]);
  }
}
  
// @brief 一つのパタンで一つの故障に対するシミュレーションを行う．
// @param[in] tv テストベクタ
// @param[in] f 対象の故障
bool
Fsim3::run(TestVector* tv,
	   SaFault* f)
{
  ymuint npi = mNetwork->input_num2();
  
  for (ymuint i = 0; i < npi; ++ i) {
    SimNode* simnode = mInputArray[i];
    simnode->set_gval(tv->val3(i));
  }
  
  // 正常値の計算を行う．
  calc_gval();

  // FFR 内の故障伝搬を行う．
  SimNode* simnode = NULL;
  bool lobs;
  if ( f->is_input_fault() ) {
    ymuint ipos = 0;
    find_simedge(f->node(), f->pos(), simnode, ipos);
    lobs = simnode->calc_lobs() & simnode->calc_gobs3(ipos);
  }
  else {
    simnode = find_simnode(f->node());
    lobs = simnode->calc_lobs();
  }
  clear_lobs(simnode);
  
  SimNode* isimnode = find_simnode(f->source_node());
  Val3 gval = isimnode->gval();
  if ( gval == kValX ) {
    return false;
  }
  
  bool valdiff = (gval != f->val3());
  lobs &= valdiff;
  
  bool ans = false;
  
  // lobs が 0 ならその後のシミュレーションを行う必要はない．
  if ( lobs ) {
    SimNode* root = simnode->ffr()->root();
    Val3 gval = root->gval();
    if ( gval == kValX ) {
      ans = false;
    }
    else if ( root->is_output() ) {
      ans = true;
    }
    else {
      ans = calc_fval(root);
    }
  }
    
  // 値をクリアする．
  clear_gval();
  
  return ans;
}

// @brief ネットワークをセットした後に呼ばれる関数
void
Fsim3::after_set_network(const TgNetwork& network,
			 TvMgr& tvmgr)
{
  clear();

  mNetwork = &network;

  ymuint nn = mNetwork->node_num();
  ymuint ni = mNetwork->input_num2();
  ymuint no = mNetwork->output_num2();
  ymuint nl = mNetwork->logic_num();

  // SimNode の生成
  // 対応付けを行うマップの初期化
  mSimMap.resize(nn);
  mEdgeMap.resize(nn);
  mInputArray.resize(ni);
  mOutputArray.resize(no);
  
  // 外部入力に対応する SimNode の生成
  for (ymuint i = 0; i < ni; ++ i) {
    const TgNode* tgnode = mNetwork->input(i);
    SimNode* node = make_node(kTgInput, vector<SimNode*>());
    mSimMap[tgnode->gid()] = node;
    mInputArray[i] = node;
  }
  // 論理ノードに対応する SimNode の生成
  for (ymuint i = 0; i < nl; ++ i) {
    const TgNode* tgnode = mNetwork->sorted_logic(i);
    ymuint ni = tgnode->ni();
    vector<SimNode*> inputs(ni);
    for (ymuint i = 0; i < ni; ++ i) {
      const TgNode* itgnode = tgnode->fanin(i);
      SimNode* inode = find_simnode(itgnode);
      assert_cond(inode, __FILE__, __LINE__);
      inputs[i] = inode;
    }

    // 出力の論理を表す SimNode を作る．
    SimNode* simnode = NULL;
    mEdgeMap[tgnode->gid()].resize(ni);
    if ( tgnode->is_cplx_logic() ) {
      vector<SimNode*> inputs2(ni * 2);
      LogExpr lexp = mNetwork->get_lexp(tgnode);
      vector<EdgeMap*> emap(ni, NULL);
      for (ymuint i = 0; i < ni; ++ i) {
	// 各変数の使われ方をチェック
	// - A) 肯定リテラルが 1 つ．
	// - B) 肯定リテラルが 2 つ以上
	// - C) 否定リテラルのみ．数は問わない．
	// - D) 肯定と否定リテラルが各々 1 つ以上
	ymuint np = lexp.litnum(i, kPolPosi);
	ymuint nn = lexp.litnum(i, kPolNega);
	EdgeMap& edge_map = mEdgeMap[tgnode->gid()][i];
	if ( np == 1 && nn == 0 ) {
	  inputs2[i * 2] = inputs[i];
	  emap[i] = &edge_map;
	}
	else if ( np > 1 && nn == 0 ) {
	  vector<SimNode*> tmp(1, inputs[i]);
	  SimNode* buf = make_node(kTgBuff, tmp);
	  inputs2[i * 2] = buf;
	  edge_map.mNode = buf;
	  edge_map.mPos = 0;
	}
	else if ( np == 0 && nn > 0 ) {
	  vector<SimNode*> tmp(1, inputs[i]);
	  SimNode* inv = make_node(kTgNot, tmp);
	  inputs2[i * 2 + 1] = inv;
	  edge_map.mNode = inv;
	  edge_map.mPos = 0;
	}
	else if ( np > 0 && nn > 0 ) {
	  vector<SimNode*> tmp(1, inputs[i]);
	  SimNode* buf = make_node(kTgBuff, tmp);
	  tmp[0] = buf;
	  SimNode* inv = make_node(kTgNot, tmp);
	  inputs2[i * 2] = buf;
	  inputs2[i * 2 + 1] = inv;
	  edge_map.mNode = buf;
	  edge_map.mPos = 0;
	}
	else {
	  cout << "np = " << np << endl
	       << "nn = " << nn << endl;
	  assert_not_reached(__FILE__, __LINE__);
	}
      }
      simnode = make_logic(lexp, inputs2, emap);
    }
    else {
      tTgGateType type = tgnode->type();
      simnode = make_node(type, inputs);
      for (ymuint i = 0; i < ni; ++ i) {
	EdgeMap& edge_map = mEdgeMap[tgnode->gid()][i];
	edge_map.mNode = simnode;
	edge_map.mPos = i;
      }
    }
    mSimMap[tgnode->gid()] = simnode;
  }
  // 外部出力に対応する SimNode の生成
  for (ymuint i = 0; i < no; ++ i) {
    const TgNode* onode = mNetwork->output(i);
    SimNode* inode = find_simnode(onode->fanin(0));
    inode->set_output();
    mSimMap[onode->gid()] = inode;
    mOutputArray[i] = inode;
  }

  // 各ノードのファンアウトリストの設定
  ymuint node_num = mNodeArray.size();
  {
    vector<vector<SimNode*> > fanout_lists(node_num);
    vector<ymuint> ipos(node_num);
    for (vector<SimNode*>::iterator p = mNodeArray.begin();
	 p != mNodeArray.end(); ++ p) {
      SimNode* node = *p;
      ymuint ni = node->nfi();
      for (ymuint i = 0; i < ni; ++ i) {
	SimNode* inode = node->fanin(i);
	fanout_lists[inode->id()].push_back(node);
	ipos[inode->id()] = i;
      }
    }
    for (ymuint i = 0; i < node_num; ++ i) {
      SimNode* node = mNodeArray[i];
      node->set_fanout_list(fanout_lists[i], ipos[i]);
    }
  }

  // FFR の設定
  ymuint ffr_num = 0;
  for (ymuint i = node_num; i > 0; ) {
    -- i;
    SimNode* node = mNodeArray[i];
    if ( node->is_output() || node->nfo() != 1 ) {
      ++ ffr_num;
    }
  }
  mFFRArray.resize(ffr_num);
  ffr_num = 0;
  for (ymuint i = node_num; i > 0; ) {
    -- i;
    SimNode* node = mNodeArray[i];
    if ( node->is_output() || node->nfo() != 1 ) {
      SimFFR* ffr = &mFFRArray[ffr_num];
      node->set_ffr(ffr);
      ffr->set_root(node);
      ++ ffr_num;
    }
    else {
      SimFFR* ffr = node->fanout(0)->ffr();
      node->set_ffr(ffr);
    }
  }
  
  // 消去用の配列の大きさはノード数を越えない．
  mGvalClearArray.reserve(mNodeArray.size());
  mFvalClearArray.reserve(mNodeArray.size());

  // 最大レベルを求め，イベントキューを初期化する．
  ymuint max_level = 0;
  for (ymuint i = 0; i < no; ++ i) {
    SimNode* inode = mOutputArray[i];
    if ( max_level < inode->level() ) {
      max_level = inode->level();
    }
  }
  mEventQ.init(max_level);

  // 値を X に初期化しておく
  for (vector<SimNode*>::iterator p = mNodeArray.begin();
       p != mNodeArray.end(); ++ p) {
    SimNode* simnode = *p;
    simnode->set_gval(kValX);
  }
}

// @brief 故障リストの内容が変わった後に呼ばれる関数
void
Fsim3::after_update_faults(const vector<SaFault*>& flist)
{
}

// @brief 現在保持している SimNode のネットワークを破棄する．
void
Fsim3::clear()
{
  clear_faults();
  
  mSimMap.clear();

  // mNodeArray が全てのノードを持っている
  for (vector<SimNode*>::iterator p = mNodeArray.begin();
       p != mNodeArray.end(); ++ p) {
    delete *p;
  }
  mNodeArray.clear();
  mInputArray.clear();
  mOutputArray.clear();
  mLogicArray.clear();

  mFFRArray.clear();

  mGvalClearArray.clear();
  mFvalClearArray.clear();
  
  
  // 念のため
  mNetwork = NULL;
}
  
// @brief FsimFault を破棄する．
void
Fsim3::clear_faults()
{
  for (vector<SimFFR>::iterator p = mFFRArray.begin();
       p != mFFRArray.end(); ++ p) {
    (*p).fault_list().clear();
  }
  
  mFsimFaults.clear();
}
  
// @brief 正常値の計算を行う．
// @note 値の変わったノードは mGvalClearArray に積まれる．
void
Fsim3::calc_gval()
{
  mGvalClearArray.clear();
  ymuint npi = mNetwork->input_num2();
  for (ymuint i = 0; i < npi; ++ i) {
    SimNode* node = mInputArray[i];
    Val3 gval = node->gval();
    if ( gval != kValX ) {
      mGvalClearArray.push_back(node);
      ymuint no = node->nfo();
      for (ymuint i = 0; i < no; ++ i) {
	mEventQ.put(node->fanout(i));
      }
    }
  }
  for ( ; ; ) {
    SimNode* node = mEventQ.get();
    if ( node == NULL ) break;
    if ( node->calc_gval3() ) {
      mGvalClearArray.push_back(node);
      ymuint no = node->nfo();
      for (ymuint i = 0; i < no; ++ i) {
	mEventQ.put(node->fanout(i));
      }
    }
  }
}

// @brief 正常値をクリアする．
// @note mGvalClearArray を使う．
void
Fsim3::clear_gval()
{
  for (vector<SimNode*>::iterator p = mGvalClearArray.begin();
       p != mGvalClearArray.end(); ++ p) {
    SimNode* node = *p;
    node->set_gval(kValX);
  }
}

// @brief FFR 内の故障シミュレーションを行う．
bool
Fsim3::ffr_simulate(SimFFR* ffr,
		    vector<FsimFault*>& det_flist)
{
  vector<FsimFault*>& flist = ffr->fault_list();
  ymuint fnum = flist.size();
  ymuint wpos = 0;
  det_flist.clear();
  det_flist.reserve(fnum);
  for (ymuint rpos = 0; rpos < fnum; ++ rpos) {
    FsimFault* ff = flist[rpos];
    SaFault* f = ff->mOrigF;
    FaultStatus fs = f->status();
    if ( fs == kFsDetected || fs == kFsUntestable ) {
      continue;
    }
    
    if ( wpos != rpos ) {
      flist[wpos] = ff;
    }
    ++ wpos;
    
    // ff の故障伝搬を行う．
    SimNode* simnode = ff->mNode;
    Val3 igval = ff->mInode->gval();
    if ( igval != ~f->val3() ) {
      continue;
    }
    
    bool lobs = simnode->calc_lobs();
    if ( !lobs ) {
      continue;
    }
    
    if ( f->is_input_fault() ) {
      // 入力の故障
      ymuint ipos = ff->mIpos;
      if ( !simnode->calc_gobs3(ipos) ) {
	continue;
      }
    }
    det_flist.push_back(ff);
  }

  if ( wpos < fnum ) {
    // flist を切り詰める．
    flist.erase(flist.begin() + wpos, flist.end());
    fnum = wpos;
  }
  
  for (ymuint rpos = 0; rpos < fnum; ++ rpos) {
    FsimFault* ff = flist[rpos];
    SimNode* node = ff->mNode;
    clear_lobs(node);
  }
  
  return !det_flist.empty();
}
  
// @brief イベントキューを用いてシミュレーションを行う．
bool
Fsim3::calc_fval(SimNode* root)
{
  root->set_fval(~root->gval());
      
  mEventQ.clear();
  mFvalClearArray.clear();
  mFvalClearArray.push_back(root);
  ymuint no = root->nfo();
  for (ymuint i = 0; i < no; ++ i) {
    mEventQ.put(root->fanout(i));
  }
  
  bool obs = false;
  for ( ; ; ) {
    SimNode* node = mEventQ.get();
    if ( node == NULL ) break;
    bool diff = node->calc_fval3();
    if ( diff ) {
      mFvalClearArray.push_back(node);
      if ( node->is_output() ) {
	if ( (node->gval() ^ node->fval()) == kVal1 ) {
	  obs = true;
	  break;
	}
      }
      else {
	ymuint no = node->nfo();
	for (ymuint i = 0; i < no; ++ i) {
	  mEventQ.put(node->fanout(i));
	}
      }
    }
  }
  
  // 今の故障シミュレーションで値の変わったノードを元にもどしておく
  for (vector<SimNode*>::iterator p = mFvalClearArray.begin();
       p != mFvalClearArray.end(); ++ p) {
    (*p)->clear_fval();
  }
  return obs;
}

// @brief logic ノードを作る．
SimNode*
Fsim3::make_logic(const LogExpr& lexp,
		  const vector<SimNode*>& inputs,
		  const vector<EdgeMap*>& emap)
{
  SimNode* node = NULL;
  if ( lexp.is_posiliteral() ) {
    ymuint pos = lexp.varid();
    node = inputs[pos * 2];
    assert_cond(node, __FILE__, __LINE__);
  }
  else if ( lexp.is_negaliteral() ) {
    ymuint pos = lexp.varid();
    node = inputs[pos * 2 + 1];
    assert_cond(node, __FILE__, __LINE__);
  }
  else {
    ymuint nc = lexp.child_num();
    vector<SimNode*> tmp(nc);
    for (ymuint i = 0; i < nc; ++ i) {
      tmp[i] = make_logic(lexp.child(i), inputs, emap);
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
    // ちょっとかっこわるい探し方
    ymuint ni = inputs.size() / 2;
    for (ymuint i = 0; i < nc; ++ i) {
      SimNode* inode = tmp[i];
      for (ymuint j = 0; j < ni; ++ j) {
	if ( inode == inputs[j * 2] ) {
	  if ( emap[j] != NULL ) {
	    emap[j]->mNode = node;
	    emap[j]->mPos = i;
	  }
	  break;
	}
      }
    }
  }
  return node;
}

// @brief 単純な logic ノードを作る．
SimNode*
Fsim3::make_node(tTgGateType type,
		 const vector<SimNode*>& inputs)
{
  ymuint32 id = mNodeArray.size();
  SimNode* node = SimNode::new_node(id, type, LogExpr(), inputs);
  mNodeArray.push_back(node);
  if ( type != kTgInput ) {
    mLogicArray.push_back(node);
  }
  return node;
}

END_NAMESPACE_YM_ATPG_FSIM3
