
/// @file src/fsim/Fsim.cc
/// @brief Fsim の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: Fsim.cc 2203 2009-04-16 05:04:40Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "Fsim.h"
#include "ym_tgnet/TgNetwork.h"
#include "ym_tgnet/TgNode.h"
#include "SimNode.h"
#include "SimFFR.h"
#include "SaFault.h"
#include "TestVector.h"
#include "ym_utils/StopWatch.h"


BEGIN_NAMESPACE_YM_ATPG_FSIM

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
Fsim::Fsim()
{
}

// @brief デストラクタ
Fsim::~Fsim()
{
  clear();
}
  
// @brief 対象の故障をセットする
// @param[in] flist 対象の故障リスト
void
Fsim::set_faults(const vector<SaFault*>& flist)
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
Fsim::run(TestVector* tv,
	  list<SaFault*>& det_faults)
{
  ymuint npi = mNetwork->input_num2();
  
  det_faults.clear();
  
  // tv を全ビットにセットしていく．
  for (ymuint i = 0; i < npi; ++ i) {
    SimNode* simnode = mInputArray[i];
    PackedVal val = (tv->val3(i) == kVal1) ? kPvAll1 : kPvAll0;
    simnode->set_gval(val);
  }
  
  // 正常値の計算を行う．
  for (vector<SimNode*>::iterator q = mLogicArray.begin();
       q != mLogicArray.end(); ++ q) {
    SimNode* node = *q;
    node->calc_gval2();
  }

  ymuint bitpos = 0;
  SimFFR* ffr_buff[kPvBitLen];
  // FFR ごとに処理を行う．
  for (vector<SimFFR>::iterator p = mFFRArray.begin();
       p != mFFRArray.end(); ++ p) {
    SimFFR* ffr = &(*p);
    if ( ffr->fault_list().empty() ) continue;
    
    // FFR 内の故障伝搬を行う．
    // 結果は FsimFault.mObsMask に保存される．
    // FFR 内の全ての obs マスクを ffr_req に入れる．
    // 検出済みの故障は ffr->fault_list() から取り除かれる．
    PackedVal ffr_req = ffr_simulate(ffr);

    // ffr_req が 0 ならその後のシミュレーションを行う必要はない．
    if ( ffr_req == kPvAll0 ) {
      continue;
    }

    SimNode* root = ffr->root();
    if ( root->is_output() ) {
      // 常に観測可能
      fault_sweep(ffr, det_faults);
      continue;
    }

    // キューに積んでおく
    PackedVal bitmask = 1UL << bitpos;
    PackedVal pat = root->gval() ^ bitmask;
    root->set_fval(pat);
    root->set_fmask(~bitmask);

    mClearArray.push_back(root);
    ymuint no = root->nfo();
    for (ymuint i = 0; i < no; ++ i) {
      mEventQ.put(root->fanout(i));
    }
    ffr_buff[bitpos] = ffr;

    ++ bitpos;
    if ( bitpos == kPvBitLen ) {
      PackedVal obs = eventq_simulate();
      for (ymuint i = 0; i < kPvBitLen; ++ i, obs >>= 1) {
	if ( obs & 1UL ) {
	  fault_sweep(ffr_buff[i], det_faults);
	}
      }
      bitpos = 0;
    }
  }
  if ( bitpos > 0 ) {
    PackedVal obs = eventq_simulate();
    for (ymuint i = 0; i < bitpos; ++ i, obs >>= 1) {
      if ( obs & 1UL ) {
	fault_sweep(ffr_buff[i], det_faults);
      }
    }
  }
}

// @brief 故障シミュレーションを行う．
// @param[in] tv_array テストベクタの配列
// @param[out] det_faults 検出された故障を格納するリストの配列
void
Fsim::run(const vector<TestVector*>& tv_array,
	  vector<list<SaFault*> >& det_faults)
{
  ymuint npi = mNetwork->input_num2();
  ymuint nb = tv_array.size();
  assert_cond(det_faults.size() >= nb, __FILE__, __LINE__);
  
  for (ymuint i = 0; i < nb; ++ i) {
    det_faults[i].clear();
  }
  
  // tv_array を入力ごとに固めてセットしていく．
  for (ymuint i = 0; i < npi; ++ i) {
    PackedVal val = kPvAll0;
    PackedVal bit = 1UL;
    for (ymuint j = 0; j < nb; ++ j, bit <<= 1) {
      if ( tv_array[j]->val3(i) == kVal1 ) {
	val |= bit;
      }
    }
    // 残ったビットには 0 番めのパタンを詰めておく．
    if ( tv_array[0]->val3(i) == kVal1 ) {
      for (ymuint j = nb; j < kPvBitLen; ++ j, bit <<= 1) {
	val |= bit;
      }
    }
    SimNode* simnode = mInputArray[i];
    simnode->set_gval(val);
  }
  
  // 正常値の計算を行う．
  for (vector<SimNode*>::iterator q = mLogicArray.begin();
       q != mLogicArray.end(); ++ q) {
    SimNode* node = *q;
    node->calc_gval2();
  }
  
  // FFR ごとに処理を行う．
  for (vector<SimFFR>::iterator p = mFFRArray.begin();
       p != mFFRArray.end(); ++ p) {
    SimFFR* ffr = &(*p);
    if ( ffr->fault_list().empty() ) continue;
    
    // FFR 内の故障伝搬を行う．
    // 結果は FsimFault.mObsMask に保存される．
    // FFR 内の全ての obs マスクを ffr_req に入れる．
    // 検出済みの故障は ffr->fault_list() から取り除かれる．
    PackedVal ffr_req = ffr_simulate(ffr);

    // ffr_req が 0 ならその後のシミュレーションを行う必要はない．
    if ( ffr_req == kPvAll0 ) {
      continue;
    }
    
    // FFR の出力の故障伝搬を行う．
    SimNode* root = ffr->root();
    PackedVal obs = kPvAll0;
    if ( root->is_output() ) {
      obs = kPvAll1;
    }
    else {
      PackedVal pat = root->gval() ^ ffr_req;
      root->set_fval(pat);
      mClearArray.clear();
      mClearArray.push_back(root);
      ymuint no = root->nfo();
      for (ymuint i = 0; i < no; ++ i) {
	mEventQ.put(root->fanout(i));
      }
      obs = eventq_simulate();
    }
    
    // obs と各々の故障の mObsMask との AND が 0 でなければ故障検出
    // できたということ．対応するテストベクタを記録する．
    // また，検出済みとなった故障をリストから取り除く
    vector<FsimFault*>& flist = ffr->fault_list();
    ymuint fnum = flist.size();
    ymuint wpos = 0;
    for (ymuint rpos = 0; rpos < fnum; ++ rpos) {
      FsimFault* ff = flist[rpos];
      SaFault* f = ff->mOrigF;
      if ( f->status() == kFsUndetected || f->status() == kFsAbort ) {
	PackedVal dbits = obs & ff->mObsMask;
	if ( dbits ) {
	  ymuint l;
	  for (l = 0; (dbits & 1UL) == kPvAll0; ++ l, dbits >>= 1) ;
	  assert_cond(l < nb, __FILE__, __LINE__);
	  det_faults[l].push_back(f);
	}
	else {
	  if ( wpos != rpos ) {
	    flist[wpos] = ff;
	  }
	  ++ wpos;
	}
      }
    }
    if ( wpos < fnum ) {
      flist.erase(flist.begin() + wpos, flist.end());
    }
  }
}
  
// @brief 一つのパタンで一つの故障に対するシミュレーションを行う．
// @param[in] tv テストベクタ
// @param[in] f 対象の故障
bool
Fsim::run(TestVector* tv,
	  SaFault* f)
{
  cout << "run(" << f->str() << ")" << endl;
  
  ymuint npi = mNetwork->input_num2();
  
  // tv を全ビットにセットしていく．
  for (ymuint i = 0; i < npi; ++ i) {
    SimNode* simnode = mInputArray[i];
    PackedVal val = (tv->val3(i) == kVal1) ? kPvAll1 : kPvAll0;
    simnode->set_gval(val);
  }
  
  // 正常値の計算を行う．
  for (vector<SimNode*>::iterator q = mLogicArray.begin();
       q != mLogicArray.end(); ++ q) {
    SimNode* node = *q;
    node->calc_gval2();
  }

  // FFR 内の故障伝搬を行う．
  SimNode* simnode = NULL;
  PackedVal lobs;
  if ( f->is_input_fault() ) {
    ymuint ipos = 0;
    find_simedge(f->node(), f->pos(), simnode, ipos);
    lobs = simnode->calc_lobs() & simnode->calc_gobs2(ipos);
  }
  else {
    simnode = find_simnode(f->node());
    lobs = simnode->calc_lobs();
  }
  clear_lobs(simnode);

  SimNode* isimnode = find_simnode(f->source_node());
  PackedVal valdiff = isimnode->gval();
  if ( f->val() == 1 ) {
    valdiff = ~valdiff;
  }
  lobs &= valdiff;

  cout << f->str() << " : lobs = " << hex << lobs << dec << endl;
  
  // lobs が 0 ならその後のシミュレーションを行う必要はない．
  if ( lobs == kPvAll0 ) {
    return false;
  }
  
  SimNode* root = simnode->ffr()->root();
  if ( root->is_output() ) {
    return (lobs != kPvAll0);
  }

  root->set_fval(~root->gval());

  mClearArray.push_back(root);
  ymuint no = root->nfo();
  for (ymuint i = 0; i < no; ++ i) {
    mEventQ.put(root->fanout(i));
  }

  PackedVal obs = eventq_simulate() & lobs;
  return (obs != kPvAll0);
}

// @brief ネットワークをセットした後に呼ばれる関数
void
Fsim::after_set_network(const TgNetwork& network,
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
  mClearArray.reserve(mNodeArray.size());

  // 最大レベルを求め，イベントキューを初期化する．
  ymuint max_level = 0;
  for (ymuint i = 0; i < no; ++ i) {
    SimNode* inode = mOutputArray[i];
    if ( max_level < inode->level() ) {
      max_level = inode->level();
    }
  }
  mEventQ.init(max_level);
}

// @brief 故障リストの内容が変わった後に呼ばれる関数
void
Fsim::after_update_faults(const vector<SaFault*>& flist)
{
}

// @brief 現在保持している SimNode のネットワークを破棄する．
void
Fsim::clear()
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

  mClearArray.clear();
  
  
  // 念のため
  mNetwork = NULL;
}
  
// @brief FsimFault を破棄する．
void
Fsim::clear_faults()
{
  for (vector<SimFFR>::iterator p = mFFRArray.begin();
       p != mFFRArray.end(); ++ p) {
    (*p).fault_list().clear();
  }
  
  mFsimFaults.clear();
}

// @brief FFR 内の故障シミュレーションを行う．
PackedVal
Fsim::ffr_simulate(SimFFR* ffr)
{
  PackedVal ffr_req = kPvAll0;
  vector<FsimFault*>& flist = ffr->fault_list();
  ymuint fnum = flist.size();
  ymuint wpos = 0;
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
    PackedVal lobs = simnode->calc_lobs();
    PackedVal valdiff = ff->mInode->gval();
    if ( f->is_input_fault() ) {
      // 入力の故障
      ymuint ipos = ff->mIpos;
      lobs &= simnode->calc_gobs2(ipos);
    }
    if ( f->val() == 1 ) {
      valdiff = ~valdiff;
    }
    lobs &= valdiff;
    
    ff->mObsMask = lobs;
    ffr_req |= lobs;
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
  
  return ffr_req;
}
  
// @brief イベントキューを用いてシミュレーションを行う．
PackedVal
Fsim::eventq_simulate()
{
  PackedVal obs = kPvAll0;
  for ( ; ; ) {
    SimNode* node = mEventQ.get();
    if ( node == NULL ) break;
    PackedVal diff = node->calc_fval2(~obs);
    if ( diff != kPvAll0 ) {
      mClearArray.push_back(node);
      if ( node->is_output() ) {
	obs |= diff;
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
  for (vector<SimNode*>::iterator p = mClearArray.begin();
       p != mClearArray.end(); ++ p) {
    (*p)->clear_fval();
  }
  mClearArray.clear();
  return obs;
}

// @brief ffr 内の故障が検出可能か調べる．
void
Fsim::fault_sweep(SimFFR* ffr,
		  list<SaFault*>& det_faults)
{
  vector<FsimFault*>& flist = ffr->fault_list();
  ymuint fnum = flist.size();
  ymuint wpos = 0;
  for (ymuint rpos = 0; rpos < fnum; ++ rpos) {
    FsimFault* ff = flist[rpos];
    SaFault* f = ff->mOrigF;
    if ( f->status() == kFsUndetected || f->status() == kFsAbort ) {
      if ( ff->mObsMask ) {
	det_faults.push_back(f);
      }
      else {
	if ( wpos != rpos ) {
	  flist[wpos] = ff;
	}
	++ wpos;
      }
    }
  }
  if ( wpos < fnum ) {
    flist.erase(flist.begin() + wpos, flist.end());
  }
}

// @brief logic ノードを作る．
SimNode*
Fsim::make_logic(const LogExpr& lexp,
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
Fsim::make_node(tTgGateType type,
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

// @brief node に対応する SimNode* を得る．
SimNode*
Fsim::find_simnode(const TgNode* node) const
{
  return mSimMap[node->gid()];
}

// @brief node の pos 番めの入力に対応する枝を得る．
void
Fsim::find_simedge(const TgNode* node,
		   ymuint pos,
		   SimNode*& simnode,
		   ymuint& ipos) const
{
  const EdgeMap& edge_map = mEdgeMap[node->gid()][pos];
  simnode = edge_map.mNode;
  ipos = edge_map.mPos;
}

END_NAMESPACE_YM_ATPG_FSIM
