
/// @file seal/src/bddsim/BddsimCmd.cc
/// @brief 故障シミュレーションを行うコマンド
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: BddsimCmd.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2006 Yusuke Matsunaga
/// All rights reserved.


#if HAVE_CONFIG_H
#include "seal_config.h"
#endif

#include "BddsimCmd.h"
#include <ym_tgnet/TgNetwork.h>
#include <ym_tgnet/TgNode.h>
#include "SimNode.h"

#include <ym_bdd/BmcFactory.h>
#include <ym_tclpp/TclPopt.h>


BEGIN_NAMESPACE_YM_SEAL_BDDSIM

//////////////////////////////////////////////////////////////////////
// 故障シミュレーションを実行するコマンド
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
BddsimCmd::BddsimCmd(SealMgr* mgr) :
  SealCmd(mgr),
  mMgr(nsBdd::BmcFactory("mgr for Bddsim"))
{
  mPoptTrace = new TclPopt(this, "trace",
			   "set trace mode");
  mPoptTraceCount = new TclPopt(this, "trace_count",
				"set trace-count mode");
  mPoptMethod = new TclPoptStr(this, "method",
			       "specify method",
			       "naive|ffr|dss|fast");
  set_usage_string("set|dump|random2|random3");
}

// @brief デストラクタ
BddsimCmd::~BddsimCmd()
{
}


BEGIN_NONAMESPACE

// word 中の1のビット数を数える．
size_t
count_ones(ymulong word)
{
#if SIZEOF_UNSIGNED_LONG == 4
  const size_t mask1   = 0x55555555;
  const size_t mask2   = 0x33333333;
  const size_t mask4   = 0x0f0f0f0f;
  const size_t mask8   = 0x00ff00ff;
  const size_t mask16  = 0x0000ffff;

  word = (word & mask1)  + ((word >>  1) & mask1);
  word = (word & mask2)  + ((word >>  2) & mask2);
  word = (word & mask4)  + ((word >>  4) & mask4);
  word = (word & mask8)  + ((word >>  8) & mask8);
  word = (word & mask16) + ((word >> 16) & mask16);
  return word;
#else
#if SIZEOF_UNSIGNED_LONG == 8
  const size_t mask1  = 0x5555555555555555;
  const size_t mask2  = 0x3333333333333333;
  const size_t mask4  = 0x0f0f0f0f0f0f0f0f;
  const size_t mask8  = 0x00ff00ff00ff00ff;
  const size_t mask16 = 0x0000ffff0000ffff;
  const size_t mask32 = 0x00000000ffffffff;

  word = (word & mask1)  + ((word >>  1) & mask1);
  word = (word & mask2)  + ((word >>  2) & mask2);
  word = (word & mask4)  + ((word >>  4) & mask4);
  word = (word & mask8)  + ((word >>  8) & mask8);
  word = (word & mask16) + ((word >> 16) & mask16);
  word = (word & mask32) + ((word >> 32) & mask32);
  return word;
#else
  size_t n = SIZEOF_UNSIGNED_LONG * 8;
  size_t c = 0;
  for (size_t i = 0; i < n; ++ i) {
    if ( word & 1 ) {
      ++ c;
    }
    word >>= 1;
  }
  return word;
#endif
#endif
}

END_NONAMESPACE


// コマンド処理関数
int
BddsimCmd::cmd_proc(TclObjVector& objv)
{
  // オプション解析
  bool trace = mPoptTrace->is_specified();
  bool trace_count = mPoptTraceCount->is_specified();
  int method = 1;

  if ( mPoptMethod->is_specified() ) {
    string method_str = mPoptMethod->val();
    if ( method_str == "naive" ) {
      method = 1;
    }
    else if ( method_str == "ffr" ) {
      method = 2;
    }
    else if ( method_str == "dss" ) {
      method = 3;
    }
    else if ( method_str == "fast" ) {
      method = 4;
    }
    else {
      string emsg;
      emsg = method_str + ": illegal value for -method";
      set_result(emsg);
      return TCL_ERROR;
    }
  }
  
  size_t objc = objv.size();

  // このコマンドは引数をとらない．
  if ( objc != 1 ) {
    print_usage();
    return TCL_ERROR;
  }

  ostringstream out;

  const TgNetwork& network = _network();
  size_t ni = network.input_num2();
  size_t no = network.output_num2();
  size_t nl = network.logic_num();
  size_t nn = network.node_num();
  mNodeArray.resize(nn);
  
  double dtotal = 0;

  // 正常回路の論理関数を計算する．
  for (size_t i = 0; i < ni; ++ i) {
    const TgNode* node = network.input(i);
    SimNode* simnode = make_input(node);
    Bdd tmp = mMgr.make_posiliteral(i);
    simnode->set_gfunc(tmp);
  }
  for (size_t i = 0; i < nl; ++ i) {
    const TgNode* node = network.sorted_logic(i);
    SimNode* simnode = make_logic(node);
    simnode->calc_gfunc();
  }
  for (size_t i = 0; i < no; ++ i) {
    const TgNode* node = network.output(i);
    const TgNode* inode = node->fanin(0);
    SimNode* simnode = mNodeArray[inode->gid()];
    mNodeArray[node->gid()] = simnode;
    simnode->set_output();
  }
  for (size_t i = 0; i < nn; ++ i) {
    const TgNode* node = network.node(i);
    size_t nfo = node->fanout_num();
    vector<SimNode*> fanout_list;
    fanout_list.reserve(nfo);
    for (size_t j = 0; j < nfo; ++ j) {
      const TgNode* onode = node->fanout(j);
      fanout_list.push_back(mNodeArray[onode->gid()]);
    }
    mNodeArray[node->gid()]->set_fanout_list(fanout_list);
  }
  
  // 可観測性を表す論理関数を計算する．
  switch ( method ) {
  case 1:
    calc_obs_naive();
    break;

  case 2:
    calc_obs_ffr();
    break;

  case 3:
    calc_obs_dss();
    break;

  case 4:
    calc_obs_fast();
    break;
  }
  
  for (size_t i = 0; i < ni; ++ i) {
    const TgNode* node = network.input(i);
    SimNode* simnode = mNodeArray[node->gid()];
    Bdd obs = simnode->obs();
    double d1 = obs.density(ni);
    dtotal += d1;
    if ( trace ) {
      out << node->name() << endl;
      out << "  gfunc:" << endl;
      simnode->get_gfunc().display_sop(out);
      out << endl;
      out << "  obs:" << endl;
      obs.dump(out);
      out << endl;
    }
    if ( trace_count ) {
      out << node->name() << "\t" << d1 << endl;
    }
  }
  for (size_t i = 0; i < nl; ++ i) {
    const TgNode* node = network.logic(i);
    SimNode* simnode = mNodeArray[node->gid()];
    Bdd obs = simnode->obs();
    double d1 = obs.density(ni);
    dtotal += d1;
    if ( trace ) {
      out << node->name() << endl;
      out << "  gfunc:" << endl;
      simnode->get_gfunc().display_sop(out);
      out << endl;
      out << "  obs:" << endl;
      obs.dump(out);
      out << endl;
    }
    if ( trace_count ) {
      out << node->name() << "\t" << d1 << endl;
    }
  }
  double cave = dtotal / static_cast<double>(ni + nl);
  out << "ave.\t" << cave << endl;
  TclObj msg = out.str();
  set_result(msg);
  
  return TCL_OK;
}

// 全ノードの可観測性を単純に計算する．
void
BddsimCmd::calc_obs_naive()
{
  const TgNetwork& network = _network();
  size_t nn = network.node_num();
  size_t no = network.output_num2();

  for (size_t i = 0; i < nn; ++ i) {
    const TgNode* node = network.node(i);
    SimNode* simnode = mNodeArray[node->gid()];
    simnode->set_obs(mMgr.make_zero());
  }
  for (size_t i = 0; i < no; ++ i) {
    const TgNode* node = network.output(i);
    SimNode* simnode = mNodeArray[node->gid()];
    simnode->set_target();
    simnode->set_obs(mMgr.make_one());
  }

  for (size_t i = 0; i < nn; ++ i) {
    const TgNode* node = network.node(i);
    SimNode* simnode = mNodeArray[node->gid()];
    if ( !simnode->target() ) {
      Bdd pat = simnode->get_gfunc();
      Bdd obs = calc_ffunc(simnode, ~pat);
      simnode->set_obs(obs);
    }
  }

  for (size_t i = 0; i < no; ++ i) {
    const TgNode* node = network.output(i);
    SimNode* simnode = mNodeArray[node->gid()];
    simnode->clear_target();
  }
}

// 全ノードの可観測性を FFR を考慮して計算する．
void
BddsimCmd::calc_obs_ffr()
{
#if 0
  const TgNetwork& network = _network();
  size_t nn = network.node_num();
  size_t ni = network.input_num();
  size_t no = network.output_num();
  size_t nl = network.logic_num();
  
  for (size_t i = 0; i < nn; ++ i) {
    const TgNode* node = network.node(i);
    mSimEngine->set_obs(node, 0UL);
  }
  for (size_t i = 0; i < no; ++ i) {
    const TgNode* node = network.output(i);
    mSimEngine->set_target(node);
    mSimEngine->set_obs(node, ~0UL);
  }

  for (size_t i = 0; i < nl; ++ i) {
    const TgNode* node = network.logic(i);
    if ( node->fanout_num() > 1 ) {
      ymulong pat = mSimEngine->get_gval2(node);
      mSimEngine->set_fval2(node, ~pat);
      ymulong obs = mSimEngine->calc_fval2();
      mSimEngine->set_obs(node, obs);
    }
    if ( mSimEngine->get_obs(node) != 0UL ) {
      calc_obs_ffr_sub(node);
    }
  }
  for (size_t i = 0; i < ni; ++ i) {
    const TgNode* node = network.input(i);
    if ( node->fanout_num() > 1 ) {
      ymulong pat = mSimEngine->get_gval2(node);
      mSimEngine->set_fval2(node, ~pat);
      ymulong obs = mSimEngine->calc_fval2();
      mSimEngine->set_obs(node, obs);
    }
  }

  for (size_t i = 0; i < no; ++ i) {
    const TgNode* node = network.output(i);
    mSimEngine->clear_target(node);
  }
#endif
}

#if 0
// FFR 内部の可観測性を計算する．
void
BddsimCmd::calc_obs_ffr_sub(const TgNode* node)
{
  size_t nfi = node->ni();
  vector<ymulong> iobs(nfi);
  mSimEngine->calc_iobs2(node, iobs);
  for (size_t i = 0; i < nfi; ++ i) {
    const TgNode* inode = node->fanin(i);
    if ( inode->fanout_num() == 1 ) {
      mSimEngine->set_obs(inode, iobs[i]);
      calc_obs_ffr_sub(inode);
    }
  }
}
#endif

// 全ノードの可観測性を DSS を考慮して計算する．
void
BddsimCmd::calc_obs_dss()
{
}

// 全ノードの可観測性を高速に近似的に計算する．
void
BddsimCmd::calc_obs_fast()
{
#if 0
  const TgNetwork& network = _network();
  size_t nn = network.node_num();
  size_t no = network.output_num();
  size_t nl = network.logic_num();
  
  for (size_t i = 0; i < nn; ++ i) {
    const TgNode* node = network.node(i);
    mSimEngine->set_obs(node, 0UL);
  }
  for (size_t i = 0; i < no; ++ i) {
    const TgNode* node = network.output(i);
    mSimEngine->set_obs(node, ~0UL);
  }

  for (size_t i = 0; i < nl; ++ i) {
    const TgNode* node = network.sorted_logic(nl - i - 1);
    size_t nfi = node->ni();
    vector<ymulong> iobs(nfi);
    mSimEngine->calc_iobs2(node, iobs);
    for (size_t i = 0; i < nfi; ++ i) {
      const TgNode* inode = node->fanin(i);
      ymulong tmp = mSimEngine->get_obs(inode);
      mSimEngine->set_obs(inode, tmp | iobs[i]);
    }
  }
#endif
}

// simnode に ffunc を設定して故障シミュレーションを行う．
Bdd
BddsimCmd::calc_ffunc(SimNode* simnode,
		      Bdd ffunc)
{
#if 0
  simnode->set_ffunc(ffunc);
  mClearArray.push_back(simnode);
  mEventQ.put(simnode);

  Bdd obs = mMgr.make_zero();
  for ( ; ; ) {
    SimNode* simnode = mEventQ.get();
    if ( simnode == NULL ) break;
    if ( simnode->target() ) {
      Bdd obs1 = simnode->calc_diff() & simnode->obs();
      obs |= obs1;
    }
    else {
      size_t nfo = simnode->nfo();
      for (size_t i = 0; i < nfo; ++ i) {
	SimNode* onode = simnode->fanout(i);
	bool changed = onode->calc_ffunc();
	if ( changed ) {
	  mClearArray.push_back(onode);
	  mEventQ.put(onode);
	}
      }
    }
  }
    
  // 今の故障シミュレーションで値の変わったノードを元にもどしておく
  for (vector<SimNode*>::iterator p = mClearArray.begin();
       p != mClearArray.end(); ++ p) {
    SimNode* node = *p;
    node->clear_ffunc();
  }
  mClearArray.clear();

  return obs;
#else
  return mMgr.make_zero();
#endif
}

// 入力用の SimNode を生成する．
SimNode*
BddsimCmd::make_input(const TgNode* node)
{
  ymuint32 id = node->gid();
  SimNode* simnode = new SimNode(id);
  mNodeArray[id] = simnode;
  return simnode;
}

// logic 用の SimNode を生成する．
SimNode*
BddsimCmd::make_logic(const TgNode* node)
{
  ymuint32 id = node->gid();
  SimNode* simnode = NULL;
  size_t ni = node->ni();
  vector<SimNode*> inputs(ni);
  for (size_t i = 0; i < ni; ++ i) {
    const TgNode* inode = node->fanin(i);
    inputs[i] = mNodeArray[inode->gid()];
  }
  if ( node->is_cplx_logic() ) {
    LogExpr lexp = _network().get_lexp(node);
    simnode = new SimNode(id, node->type(), lexp, inputs);
  }
  else {
    simnode = new SimNode(id, node->type(), inputs);
  }
  mNodeArray[id] = simnode;
  return simnode;
}

END_NAMESPACE_YM_SEAL_BDDSIM
