
/// @file src/dtpg_classic/Analyzer.cc
/// @brief Analyzer の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: Analyzer.cc 2203 2009-04-16 05:04:40Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "Analyzer.h"
#include "Network.h"
#include "base/TestVector.h"
#include "base/TvMgr.h"
#include "ym_tgnet/TgNode.h"

//#define DEBUG 1


BEGIN_NAMESPACE_YM_ATPG_DTPGCL

static
bool
rf_chk(Gate* gate,
       Network& network);

static
vector<ymuint> rf_array;

inline
ymuint&
rf_gate(Gate* gate)
{
  return rf_array[gate->id()];
}

Analyzer cfna;

// @brief コンストラクタ
// @param[in] tvmgr TvMgr
Analyzer::Analyzer() :
  mTvMgr(NULL),
  mNpi(0),
  mAigMgr(NULL)
{
}

// @brief デストラクタ
Analyzer::~Analyzer()
{
  clear();
}

// @brief 解析を行う．
void
Analyzer::run(Network& network,
	      TvMgr& tvmgr)
{
  clear();
  
  // AIG を作る．
  ymuint npi = network.input_num2();
  ymuint npo = network.output_num2();
  ymuint nl = network.logic_num();
  ymuint ngate = nl + npi;

  ymuint max_id = network.node_num();
  vector<AigHandle> node_map(max_id);
  
  mAigMgr = new AigMgr;

  // 入力の生成
  for (ymuint i = 0; i < npi; ++ i) {
    const TgNode* node = network.input(i);
    AigHandle aig = mAigMgr->make_input();
    node_map[node->gid()] = aig;
  }

  // 内部ノードの生成
  for (ymuint i = 0; i < nl; ++ i) {
    const TgNode* node = network.sorted_logic(i);
    ymuint nfi = node->ni();
    vector<AigHandle> fanins(nfi);
    for (ymuint j = 0; j < nfi; ++ j) {
      const TgNode* inode = node->fanin(j);
      fanins[j] = node_map[inode->gid()];
    }
    if ( node->is_cplx_logic() ) {
      LogExpr expr = network.get_lexp(node);
      node_map[node->gid()] = mAigMgr->make_logic(expr, fanins);
    }
    else {
      node_map[node->gid()] = gate2aig(node->type(), fanins);
    }
  }
  
  // AIG 上でランダムシミュレーションを行う．
  // その結果，
  // - 0/1 に固定してそうなノード
  // - 等価みたいなノード
  // の候補を求める．
  RandGen randgen;
  ymuint naig = mAigMgr->node_num();
  vector<PackedVal> val_array(naig);
  vector<bool> c0_cands(naig, false);
  vector<bool> c1_cands(naig, false);
  vector<list<AigNode*> > eq_cands(naig);
  vector<bool> eq_rep(naig, false);
  bool first = true;
  for ( ; ; ) {
    for (ymuint i = 0; i < naig; ++ i) {
      AigNode* anode = mAigMgr->node(i);
      PackedVal val;
      if ( anode->is_input() ) {
	val = randgen.ulong();
      }
      else {
	AigNode* inode0 = anode->fanin0();
	PackedVal val0 = val_array[inode0->node_id()];
	if ( anode->fanin_inv0() ) {
	  val0 = ~val0;
	}
	AigNode* inode1 = anode->fanin1();
	PackedVal val1 = val_array[inode1->node_id()];
	if ( anode->fanin_inv1() ) {
	  val1 = ~val1;
	}
	val = val0 & val1;
      }
      val_array[anode->node_id()] = val;
    }
    if ( first ) {
      first = false;
      hash_map<ymulong, AigNode*> sigmap;
      for (ymuint i = 0; i < naig; ++ i) {
	AigNode* node = mAigMgr->node(i);
	PackedVal val = val_array[node->node_id()];
	if ( val == kPvAll0 ) {
	  c0_cands[node->node_id()] = true;
	}
	else if ( val == kPvAll1 ) {
	  c1_cands[node->node_id()] = true;
	}
	else {
	  hash_map<ymulong, AigNode*>::iterator p = sigmap.find(val);
	  if ( p == sigmap.end() ) {
	    sigmap.insert(val, node);
	    eq_rep[node->node_id()] = true;
	  }
	  else {
	    AigNode* rep = p->second;
	    eq_cands[rep->node_id()].push_back(node);
	  }
	}
      }
    }
    else {
      bool update = false;
      for (ymuint i = 0; i < naig; ++ i) {
	AigNode* node = mAigMgr->node(i);
	PackedVal val = val_array[node->node_id()];
	if ( c0_cands[node->node_id()] && val != kPvAll0 ) {
	  c0_cands[node->node_id()] = false;
	  update = true;
	}
	if ( c1_cands[node->node_id()] && val != kPvAll1 ) {
	  c1_cands[node->node_id()] = false;
	  update = true;
	}
	if ( eq_rep[node->node_id()] ) {
	  list<AigNode*>& eq_list = eq_cands[node->node_id()];
	  if ( eq_list.empty() ) {
	    continue;
	  }
	  hash_map<ymulong, AigNode*> sigmap;
	  sigmap.insert(val, node);
	  for (list<AigNode*>::iterator p = eq_list.begin();
	       p != eq_list.end(); ) {
	    list<AigNode*>::iterator cur = p;
	    ++ p;
	    AigNode* node1 = *p;
	    PackedVal val1 = val_array[node1->node_id()];
	    hash_map<ymulong, AigNode*>::iterator q = sigmap.find(val1);
	    if ( q == sigmap.end() ) {
	      sigmap.insert(val1, node1);
	      eq_rep[node1->node_id()] = true;
	      eq_list.erase(cur);
	      update = true;
	    }
	    else {
	      AigNode* rep = q->second;
	      if ( rep != node ) {
		eq_cands[rep->node_id()].push_back(node1);
		eq_list.erase(cur);
		update = true;
	      }
	    }
	  }
	}
      }
      if ( !update ) {
	break;
      }
    }
  }
  
  bool need_sat = false;
  for (ymuint i = 0; i < naig; ++ i) {
    if ( c0_cands[i] || c1_cands[i] ) {
      need_sat = true;
      break;
    }
    if ( !eq_cands[i].empty() ) {
      need_sat = true;
      break;
    }
  }
  if ( need_sat ) {
    SatSolver* solverp = SatSolverFactory::gen_solver();
    // CNF の生成
    for (ymint i = 0; i < naig; ++ i) {
      ymuint id = solver->new_var();
      assert_cond(id == i, __FILE__, __LINE__);
      AigNode* node = mAigMgr->node(i);
      if ( !node->is_input() ) {
	ymuint id0 = node->fanin0()->node_id();
	tPol pol0 = kPolPosi;
	if ( node->fanin_inv0() ) {
	  pol0 = kPolNega;
	}
	Literal lit0(id0, pol0);
	
	ymuint id1 = node->fanin1()->node_id();
	tPol pol1 = kPolPosi;
	if ( node->fanin_inv1() ) {
	  pol1 = kPolNega;
	}
	Literal lit1(id1, pol1);
	
	Literal olit(id, kPolPosi);
	solver->add_clause(lit0, ~olit);
	solver->add_clause(lit1, ~olit);
	solver->add_clause(~lit0, ~lit1, olit);
      }
    }
    for (ymuint i = 0; i < naig; ++ i) {
      if ( c0_cands[i] ) {
	vector<Literal> assump(1);
	assump[0] = Literal(i, kPolPosi);
	vector<tBool3> model;
	tBool3 stat = solver->solve(assump, model);
	if ( stat == kB3False ) {
	  // 確かに0に固定している．
	}
      }
      else if ( c1_cands[i] ) {
	vector<Literal> assump(1);
	assump[0] = Literal(i, kPolNega);
	vector<tBool3> model;
	tBool3 stat = solver->solve(assump, model);
	if ( stat == kB3False ) {
	  // 確かに1に固定している．
	}
      }
      else if ( eq_rep[i] && !eq_cands[i].empty() ) {
	list<AigNode*>& eq_list = eq_cands[i];
	vector<Literal> assump(2);
	Literal lit1(
	assump[0] = Literal(i, kPolNega);
	vector<tBool3> model;
	tBool3 stat = solver->solve(assump, model);
	if ( stat == kB3False ) {
	  // 確かに1に固定している．
	}
      }
    }
  }
  
#ifdef DEBUG
  cerr << "Finding conflict-free node assignment." << endl;
#endif
  
  mTvMgr = &tvmgr;
  
  mNpi = network.npi();
  ymuint nn = network.ngate();
  
  rf_array.clear();
  rf_array.resize(nn);

  mCurVector = mTvMgr->new_vector();

  // 外部入力はもちろん CFNA
  for (ymuint i = 0; i < mNpi; ++ i) {
    Gate* gate = network.gate(i);
    gate->set_cfna0();
    gate->set_cfna1();
  }
  
  // basis node に対して正当化ができるか試す．
  for (ymuint i = mNpi; i < nn; ++ i) {
    Gate* gate = network.gate(i);
    if ( gate->is_basis() ) {
      if ( sm.basis_just(gate, kVal0) ) {
	gate->set_cfna0();
	set_vector(sm.term_list());
	record_vector(gate, kVal0);
      }
      if ( sm.basis_just(gate, kVal1) ) {
	gate->set_cfna1();
	set_vector(sm.term_list());
	record_vector(gate, kVal1);
      }
    }
  }

  for (ymuint i = 0; i < mNpi; ++ i) {
    Gate* gate = network.gate(i);
    if ( rf_chk(gate, network) ) {
      rf_gate(gate) = 1;
    }
    else {
      rf_gate(gate) = 0;
    }
  }
  
  clear_vector();
  for (ymuint i = mNpi; i < nn; ++ i) {
    Gate* gate = network.gate(i);
    if ( !gate->chk_term0() ) {
      if ( justify_gate(gate, kVal0) ) {
	record_vector(gate, kVal0);
	clear_vector();
	gate->set_cfna0();
      }
    }
    if ( !gate->chk_term1() ) {
      if ( justify_gate(gate, kVal1) ) {
	record_vector(gate, kVal1);
	clear_vector();
	gate->set_cfna1();
      }
    }
    if ( (gate->chk_cfna0() || gate->chk_cfna1()) &&
	 !rf_chk(gate, network) ) {
      rf_gate(gate) = 0;
    }
    else {
      rf_gate(gate) = 1;
    }
  }

#ifdef DEBUG
  cerr << "   ...end." << endl;
#endif
}

/// @brief 内容をすべてクリアする．
void
Analyzer::clear()
{
  if ( mTvMgr ) {
    for (hash_map<ymuint, TestVector*>::iterator p = mAssignMap.begin();
	 p != mAssignMap.end(); ++ p) {
      mTvMgr->delete_vector(p->second);
    }
    mAssignMap.clear();
    mTvMgr = NULL;
  }
  delete mAigMgr;
  mAigMgr = NULL;
}

static
bool
rf_chk_dfs(Gate* gate,
	   ymuint mark,
	   vector<ymuint>& br_marks)
{
  if ( gate->chk_mark() ) {
    if ( br_marks[gate->id()] != mark ) {
      return true;
    }
    return false;
  }
  gate->set_mark();
  br_marks[gate->id()] = mark;
  ymuint no = gate->nfo();
  for (ymuint i = 0; i < no; ++ i) {
    bool stat = rf_chk_dfs(gate->fanout(i), mark, br_marks);
    if ( stat ) {
      return true;
    }
  }
  return false;
}

// reconvergent fanout を持つとき true を返す．
static
bool
rf_chk(Gate* gate0,
       Network& network)
{
  ymuint no = gate0->nfo();
  
  if ( no < 2 ) {
    return false;
  }

  vector<ymuint> br_marks(network.ngate());
  network.clear_mark();
  for (ymuint i = 0; i < no; ++ i) {
    bool stat = rf_chk_dfs(gate0->fanout(i), i, br_marks);
    if ( stat ) {
      return true;
    }
  }
  return false;
}

// Gate から AIG を作る
AigHandle
Analyzer::gate2aig(tTgGateType type,
		   const vector<AigHandle>& fanins)
{
  ymuint ni = fanins.size();
  bool inv = false;
  AigHandle aig;
  switch ( type ) {
  case kTgNot:
    inv = true;
    // わざと次に続く
  case kTgOutput:
  case kTgBuff:
    assert_cond(fanins.size() == 1, __FILE__, __LINE__);
    aig = fanins[0];
    break;

  case kTgNand:
    inv = true;
    // わざと次に続く
  case kTgAnd:
    assert_cond(ni >= 2, __FILE__, __LINE__);
    aig = mAigMgr->make_and(fanins[0], fanins[1]);
    for (ymuint i = 2; i < ni; ++ i) {
      aig = mAigMgr->make_and(aig, fanins[i]);
    }
    break;

  case kTgNor:
    inv = true;
    // わざと次に続く
  case kTgOr:
    assert_cond(ni >= 2, __FILE__, __LINE__);
    aig = mAigMgr->make_or(fanins[0], fanins[1]);
    for (ymuint i = 2; i < ni; ++ i) {
      aig = mAigMgr->make_or(aig, fanins[i]);
    }
    break;

  case kTgXnor:
    inv = true;
    // わざと次に続く
  case kTgXor:
    assert_cond(ni >= 2, __FILE__, __LINE__);
    aig = mAigMgr->make_xor(fanins[0], fanins[1]);
    for (ymuint i = 2; i < ni; ++ i) {
      aig = mAigMgr->make_xor(aig, fanins[i]);
    }
    break;

  case kTgUndef:
  case kTgInput:
  default:
    assert_not_reached(__FILE__, __LINE__);
    break;
  }
  if ( inv ) {
    return ~aig;
  }
  else {
    return aig;
  }
}

// @brief テストベクタをクリアする．
void
Analyzer::clear_vector()
{
  mCurVector->init();
}

// @brief テストベクタをセットする．
void
Analyzer::set_vector(const vector<pair<Gate*, Val3> >& term_list)
{
  clear_vector();
  for (vector<pair<Gate*, Val3> >::const_iterator p = term_list.begin();
       p != term_list.end(); ++ p) {
    Gate* gate = p->first;
    Val3 val = p->second;
    get_vector(gate, val);
  }
}

// @brief 現在の割り当てを gate の割り当てとして記録する．
void
Analyzer::record_vector(Gate* gate,
		    Val3 val)
{
  ymuint key = gate->id() * 2;
  if ( val == kVal1 ) {
    key += 1;
  }
  hash_map<ymuint, TestVector*>::iterator p = mAssignMap.find(key);
  assert_cond(p == mAssignMap.end(), __FILE__, __LINE__);
  TestVector* tmp = mTvMgr->new_vector();
  tmp->copy(*mCurVector);
  mAssignMap.insert(make_pair(key, tmp));
}

// @brief gate の割り当てを取り出す．
void
Analyzer::get_vector(Gate* gate,
		 Val3 val)
{
  if ( gate->is_pi() ) {
    mCurVector->set_val(gate->input_id(), val);
  }
  else {
    ymuint key = gate->id() * 2;
    if ( val == kVal1 ) {
      key += 1;
    }
    hash_map<ymuint, TestVector*>::iterator p = mAssignMap.find(key);
    assert_cond(p != mAssignMap.end(), __FILE__, __LINE__);
    TestVector* tmp = p->second;
    mCurVector->copy(*tmp);
  }
}

// @brief 現在の割り当てを記録する．
void
Analyzer::record(const vector<pair<Gate*, Val3> >& term_list,
	     TestVector* tv)
{
  set_vector(term_list);
  tv->init();
  tv->copy(*mCurVector);
}

/// @brief ゲートごとの正当化を行う．
bool
Analyzer::justify_gate(Gate* gate,
		   Val3 val)
{
  switch ( gate->gate_type() ) {
  case kTgInput:
    return true;
    
  case kTgConst0:
    if ( val == kVal0 ) {
      return true;
    }
    return false;

  case kTgConst1:
    if ( val == kVal1 ) {
      return true;
    }
    return false;
    
  case kTgNot:
    val = ~val;
    // わざと次に続く
    
  case kTgBuff:
    {
      Gate* i_gate = gate->fanin(0);
      if ( rf_gate(i_gate) == 1 ) {
	return false;
      }
      if ( i_gate->chk_term(val) ) {
	get_vector(i_gate, val);
	return true;
      }
      return false;
    }

  case kTgAnd:
  case kTgNand:
  case kTgOr:
  case kTgNor:
    if ( val == gate->o_val() ) {
      for (ymuint i = 0; i < gate->nfi(); ++ i) {
	Gate* i_gate = gate->fanin(i);
	if ( rf_gate(i_gate) == 1 ) {
	  continue;
	}
	if ( i_gate->chk_term0() ) {
	  get_vector(i_gate, gate->c_val());
	  return true;
	}
      }
    }
    else {
      for (ymuint i = 0; i < gate->nfi(); ++ i) {
	Gate* i_gate = gate->fanin(i);
	if ( rf_gate(i_gate) == 1 ) {
	  return false;
	}
	if ( !i_gate->chk_term1() ) {
	  return false;
	}
      }
      for (ymuint i = 0; i < gate->nfi(); ++ i) {
	Gate* i_gate = gate->fanin(i);
	get_vector(i_gate, gate->nc_val());
      }
      return true;
    }
    return false;

  case kTgXnor:
    val = ~val;
    // わざと次に続く
    
  case kTgXor:
    {
      bool slack = false;
      Gate* slack_gate = NULL;
      for (ymuint i = 0; i < gate->nfi(); ++ i) {
	Gate* i_gate = gate->fanin(i);
	if ( rf_gate(i_gate) == 1 ) {
	  return false;
	}
	if ( i_gate->chk_term0() ) {
	  if ( i_gate->chk_term1() ) {
	    if ( !slack ) {
	      slack = true;
	      slack_gate = i_gate;
	    }
	  }
	}
	else if ( i_gate->chk_term1() ) {
	  val = ~val;
	}
	else {
	  return false;
	}
      }
      if ( val == kVal0 ) {
	slack_gate = NULL;
      }
      for (ymuint i = 0; i < gate->nfi(); ++ i) {
	Gate* i_gate = gate->fanin(i);
	if ( i_gate != slack_gate && i_gate->chk_term0() ) {
	  get_vector(i_gate, kVal0);
	}
	else {
	  get_vector(i_gate, kVal1);
	}
      }
      return true;
    }

  default:
    break;
  }
  assert_not_reached(__FILE__, __LINE__);
  return false;
}

END_NAMESPACE_YM_ATPG_DTPGCL
