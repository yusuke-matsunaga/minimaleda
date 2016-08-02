
/// @file src/dtpg_classic/Network.cc
/// @brief Network の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: Network.cc 2203 2009-04-16 05:04:40Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "Network.h"
#include "Gate.h"
#include "GateConst.h"
#include "GateInput.h"
#include "GateBuf.h"
#include "GateAnd.h"
#include "GateOr.h"
#include "GateXor.h"
#include "GateSa.h"
#include "SaFault.h"
#include "ym_tgnet/TgNetwork.h"
#include "ym_tgnet/TgNode.h"
#include "LvlQ.h"
#include "SearchMgr.h"


BEGIN_NAMESPACE_YM_ATPG_DTPGCL

Network gn;

// @brief コンストラクタ
Network::Network() :
  mNpi(0),
  mMaxLevel(0)
{
}

// @brief デストラクタ
Network::~Network()
{
  clear();
  
  for (list<GateSa*>::iterator p = mSa0List.begin();
       p != mSa0List.end(); ++ p) {
    delete *p;
  }
  for (list<GateSa*>::iterator p = mSa1List.begin();
       p != mSa1List.end(); ++ p) {
    delete *p;
  }
}


// @brief 内容をクリアする．
void
Network::clear()
{
  sm.clear_const();
  for (vector<Gate*>::iterator p = mGateList.begin();
       p != mGateList.end(); ++ p) {
    delete *p;
  }
  mNpi = 0;
  mOutputList.clear();
  mActGateList.clear();
  mGateMap.clear();
}

// @brief 内容を設定する．
// @param[in] tgnetwork もとのネットワーク
void
Network::set(const TgNetwork& tgnetwork)
{
  clear();

  ymuint npi = tgnetwork.input_num2();
  ymuint npo = tgnetwork.output_num2();
  ymuint nl = tgnetwork.logic_num();
  ymuint ngate = nl + npi;
  if ( npi == 0 && npo == 0 && nl == 0 ) {
    return;
  }
  mGateList.reserve(ngate);
  mActGateList.reserve(ngate);
  mOutputList.resize(npo);
  
  mNpi = npi;
  ymuint max_id = tgnetwork.node_num();
  mGateMap.resize(max_id);
  
  // 入力の生成
  for (ymuint i = 0; i < npi; ++ i) {
    const TgNode* node = tgnetwork.input(i);
    new_Gate(node, i);
  }

  // 論理ゲートの生成
  for (ymuint i = 0; i < nl; ++ i) {
    const TgNode* node = tgnetwork.sorted_logic(i);
    Gate* gate =  new_Gate(node);
    ymuint ni = node->ni();
    for (ymuint j = 0; j < ni; ++ j) {
      const TgNode* inode = node->fanin(j);
      Gate* igate = mGateMap[inode->gid()];
      gate->set_fanin(j, igate);
      ++ igate->mNo;
    }
    if ( gate->gate_type() == kTgConst0 ) {
      sm.add_const0(gate);
    }
    if ( gate->gate_type() == kTgConst1 ) {
      sm.add_const1(gate);
    }
  }

  // 出力の生成
  for (ymuint i = 0; i < npo; ++ i) {
    const TgNode* node = tgnetwork.output(i);
    const TgNode* inode = node->fanin(0);
    Gate* gate = mGateMap[inode->gid()];
    mOutputList[i] = gate;
    gate->set_po();
  }

  // ファンアウト用配列の確保
  for (ymuint i = 0; i < ngate; ++ i) {
    Gate* gate = mGateList[i];
    gate->init_fogate(gate->mNo);
    gate->mNo = 0;
  }
  
  // ファンアウトの接続
  for (ymuint i = npi; i < ngate; ++ i) {
    Gate* gate = mGateList[i];
    ymuint ni = gate->nfi();
    for (ymuint j = 0; j < ni; ++ j) {
      Gate* igate = gate->fanin(j);
      igate->set_fanout(igate->mNo, gate, j);
      ++ igate->mNo;
    }
  }
  
  // FFR の根 (fanout stem) を求める．
  for (int i = ngate; -- i >= 0; ) {
    Gate* gate = mGateList[i];
    if ( gate->nfo() == 1 && gate->is_po() == false ) {
      gate->set_fos(gate->fanout(0)->fos());
    }
    else {
      gate->set_fos(gate);
    }
  }
  
  // レベルを計算する．
  for (ymuint i = 0; i < ngate; ++ i) {
    Gate* gate = mGateList[i];
    ymuint ni = gate->nfi();
    ymuint level = 0;
    for (ymuint j = 0; j < ni; ++ j) {
      ymuint level1 = gate->fanin(j)->level() + 2;
      if ( level < level1 ) {
	level = level1;
      }
    }
    gate->mLevel = level;
  }
  mMaxLevel = 0;
  for (ymuint i = 0; i < npo; ++ i) {
    ymuint level1 = output(i)->level();
    if ( mMaxLevel < level1 ) {
      mMaxLevel = level1;
    }
  }
  lvlq.resize(mMaxLevel + 2);

  // 各ゲートから到達可能な外部出力の最小レベルを求める．
  for (int i = ngate; -- i >= 0; ) {
    Gate* gate = mGateList[i];
    if ( gate->is_po() ) {
      gate->mMinLevel = gate->level();
    }
    else {
      ymuint level = mMaxLevel + 1;
      for (ymuint j = 0; j < gate->nfo(); ++ j) {
	ymuint level1 = gate->fanout(j)->mMinLevel;
	if ( level > level1 ) {
	  level = level1;
	}
      }
      gate->mMinLevel = level;
    }
  }
  
  // 等価ゲートを求める．
  set_eq_gate();
  
  // basis node の計算
  calc_basis();
  
  // testability measure の計算
  t_analyze();
}
  
// @brief ゲートを生成する．
// @param[in] node オリジナルのノード
// @param[in] input_id 入力番号
Gate*
Network::new_Gate(const TgNode* node,
		  ymuint input_id)
{
  tTgGateType type = node->type();
  ymuint ni = node->ni();
  Gate* gate = NULL;
  ymuint id = mGateList.size();
  switch ( type ) {
  case kTgConst0: gate = new GateConst0(id, node); break;
  case kTgConst1: gate = new GateConst1(id, node); break;
  case kTgInput:  gate = new GateInput(id, node, input_id); break;
  case kTgBuff:   gate = new GateBuf(id, node); break;
  case kTgNot:    gate = new GateNot(id, node); break;
  case kTgAnd:    gate = new GateAnd(id, node, ni); break;
  case kTgNand:   gate = new GateNand(id, node, ni); break;
  case kTgOr:     gate = new GateOr(id, node, ni); break;
  case kTgNor:    gate = new GateNor(id, node, ni); break;
  case kTgXor:    gate = new GateXor(id, node, ni); break;
  case kTgXnor:   gate = new XGateNor(id, node, ni); break;
  default:
    assert_not_reached(__FILE__, __LINE__);
  }
  mGateList.push_back(gate);
  mGateMap[node->gid()] = gate;
  return gate;
}

static
bool
eq(Gate* gate1,
   Gate* gate2)
{
  ymuint ni = gate1->nfi();
  vector<int> match_bits(ni, 0);
  for (ymuint i = 0; i < ni; ++ i) {
    Gate* igate1 = gate1->fanin(i);
    ymuint lvl1 = igate1->level();
    int j;
    for (j = ni; -- j >= 0; ) {
      Gate* igate2 = gate2->fanin(j);
      if ( igate2->level() != lvl1 ) {
	continue;
      }
      Gate* temp = igate2;
      while ( (temp = temp->eq_gate()) != igate2 ) {
	if ( temp == igate1 ) {
	  break;
	}
      }
      if ( igate1 == temp ) {
	match_bits[j] = 1;
	break;
      }
    }
    if ( j < 0 ) {
      return false;
    }
  }
  for (ymuint i = 0; i < ni; ++ i) {
    if ( match_bits[i] == 1 ) {
      continue;
    }
    Gate* igate2 = gate2->fanin(i);
    ymuint lvl2 = igate2->level();
    int j;
    for (j = ni; -- j >= 0; ) {
      Gate* igate1 = gate1->fanin(j);
      if ( igate1->level() != lvl2 ) {
	continue;
      }
      Gate* temp = igate1;
      while ( (temp = temp->eq_gate()) != igate1 ) {
	if ( temp == igate2 ) {
	  break;
	}
      }
      if ( igate2 == temp ) {
	break;
      }
    }
    if ( j < 0 ) {
      return false;
    }
  }
  return true;
}

// @brief 構造的に等価なゲートを求める．
void
Network::set_eq_gate()
{
  for (ymuint i = npi(); i < ngate(); ++ i) {
    Gate* gate = mGateList[i];
    if ( gate->eq_gate() != gate ) {
      continue;
    }
    ymuint level = gate->level();
    ymuint ni = gate->nfi();
    if ( ni == 0 ) {
      continue;
    }
    tTgGateType gtype = gate->gate_type();
    Gate* igate0 = gate->fanin(0);
    Gate* igate = igate0;
    if (igate->eq_gate() == igate && igate->nfo() == 1) {
      continue;
    }
    do {
      for (ymuint j = 0; j < igate->nfo(); ++ j) {
	Gate* ogate = igate->fanout(j);
	if ( ogate != gate &&
	     ogate->level() == level &&
	     ogate->eq_gate() == ogate &&
	     ogate->gate_type() == gtype &&
	     ogate->nfi() == ni ) {
	  if ( ni == 1 || eq(gate, ogate) ) {
	    Gate* top = gate->eq_gate();
	    gate->mEqGate = ogate;
	    ogate->mEqGate = top;
	  }
	}
      }
      igate = igate->eq_gate();
    } while ( igate != igate0 );
  }
  for (ymuint i = ngate(); -- i >= npi(); ) {
    Gate* gate = mGateList[i];
    if ( gate->nfi() == 1 || gate->eq_gate() == gate ) {
      gate->mEqGate = NULL;
    }
    bool ng = false;
    for (ymuint j = 0; j < gate->nfi(); ++ j) {
      if ( gate->fanin(j)->eq_gate() == NULL ) {
	ng = true;
	break;
      }
    }
    if ( ng ) {
      gate->mEqGate = NULL;
    }
  }
}

static
Gate*
cap_dom(Gate* d1,
	Gate* d2,
	const vector<Gate*>& dom_array)
{
  while ( d1 != NULL && d2 != NULL ) {
    int sign = d1->id() - d2->id();
    if ( sign < 0 ) {
      d1 = dom_array[d1->id()];
    }
    else if ( sign > 0 ) {
      d2 = dom_array[d2->id()];
    }
    else { // sign == 0
      return d1;
    }
  }
  return NULL;
}

// @brief basis node を求める．
void
Network::calc_basis()
{
  vector<Gate*> dom_array(ngate());
  vector<Gate*> di_array(ngate());
  
  // 各ゲートの dominator を求める．
  for (int i = ngate(); -- i >= 0; ) {
    Gate* gate = mGateList[i];
    ymuint no = gate->nfo();
    Gate* d = NULL;
    if ( no > 0 && !gate->is_po() ) {
      d = gate->fanout(0);
      for (ymuint j = 1; j < no; ++ j) {
	Gate* o_gate = gate->fanout(j);
	if ( (d = cap_dom(d, o_gate, dom_array)) == NULL ) {
	  break;
	}
      }
    }
    dom_array[gate->id()] = d;
  }

  // 各ゲートのファンインの dominator の共通部分を求める． 
  for (ymuint i = 0; i < ngate(); ++ i) {
    Gate* gate = mGateList[i];
    ymuint ni = gate->nfi();
    Gate* d;
    if ( ni > 1 ) {
      d = di_array[gate->fanin(0)->id()];
      for (ymuint j = 1; j < ni; ++ j) {
	Gate* d1 = di_array[gate->fanin(j)->id()];
	if ( (d = cap_dom(d, d1, dom_array)) == NULL ) {
	  break;
	}
      }
    }
    else if ( ni == 1 ) {
      d = gate->fanin(0);
      if ( di_array[d->id()] == d ) {
	d = dom_array[d->id()];
      }
      else {
	d = di_array[d->id()];
      }
    }
    else {
      d = gate;
    }
    di_array[gate->id()] = d;
    if ( d == gate || (d != NULL && dom_array[d->id()] == gate) ) {
      if ( !gate->is_po() || gate->nfo() > 0 ) {
	gate->set_basis();
      }
    }
  }
}

// @brief 全体をアクティブにする．
void
Network::activate_all()
{
  mActGateList.clear();
  for (int i = ngate(); -- i >= 0; ) {
    Gate* gate = mGateList[i];
    mActGateList.push_back(gate);
    gate->set_active();
    ymuint no = gate->nfo();
    ymuint k = 0;
    for (ymuint j = 0; j < no; ++ j) {
      Gate* ogate = gate->fanout(j);
      if ( ogate->is_active() ) {
	gate->set_act_fanout(k ++, ogate, gate->fanout_ipos(j));
      }
    }
    gate->set_act_no(k);
  }
}

// @brief 指定されたゲートの TFI of TFO のみアクティブにする．
bool
Network::activate_tfitfo(Gate* pivot)
{
  lvlq.clear();
  bool reached = mark_tfo(pivot);
  if ( !reached ) {
    return false;
  }
  
  mActGateList.clear();
  for (Gate* gate; (gate = lvlq.get_from_top());  ) {
    ymuint k = 0;
    bool sc_flag = true;
    ymuint no = gate->nfo();
    for (ymuint i = 0; i < no; ++ i) {
      Gate* o_gate = gate->fanout(i);
      if ( o_gate->is_active() ) {
	gate->set_act_fanout(k ++, o_gate, gate->fanout_ipos(i));
	if ( !o_gate->chk_scope() ) {
	  sc_flag = false;
	}
      }
      else {
	sc_flag = false;
      }
    }
    if ( sc_flag ) {
      gate->set_scope();
    }
    gate->set_act_no(k);
    mActGateList.push_back(gate);
    for (ymuint i = 0; i < gate->nfi(); ++ i) {
      Gate* i_gate = gate->fanin(i);
      if ( !i_gate->is_active() ) {
	i_gate->set_active();
	lvlq.put(i_gate);
      }
    }
  }
  return true;
}

// @brief activate_tfitfo() の下請け関数
bool
Network::mark_tfo(Gate* gate)
{
  bool reached = false;
  if ( !gate->is_active() ) {
    gate->set_active();
    if ( gate->is_po() ) {
      reached = true;
    }
    lvlq.put(gate);
    for (ymuint i = 0; i < gate->nfo(); ++ i) {
      if ( mark_tfo(gate->fanout(i)) ) {
	reached = true;
      }
    }
  }
  return reached;
}

// @brief 全体をアクティブでなくす．
void
Network::deactivate()
{
  for (vector<Gate*>::iterator p = mActGateList.begin();
       p != mActGateList.end(); ++ p) {
    Gate* gate = *p;
    gate->rst_active();
    gate->rst_scope();
    gate->set_act_no(0);
  }
  mActGateList.clear();
}

// @brief f に対応する故障ノードを挿入する．
void
Network::inject_fault(SaFault* f)
{
  if ( get_verbose_level() > 1 ) {
    fprintf(ver_fp, "FAULT_ASSIGN: %s\n", f->str().c_str());
  }

  Gate* gate = mGateMap[f->node()->gid()];
  if ( f->is_output_fault() ) {
    if ( gate->is_po() ) {
      GateSa* f_gate = new_fault(f->val3());
      insert(gate, f_gate);
    }
    else {
      for (ymuint i = 0; i < gate->act_nfo(); ++ i) {
	GateSa* f_gate = new_fault(f->val3());
	Gate* ogate = gate->act_fanout(i);
	ymuint ipos = gate->act_fanout_ipos(i);
	insert(gate, i, ogate, ipos, f_gate);
      }
    }
  }
  else {
    ymuint ipos = f->pos();
    Gate* i_gate = gate->fanin(ipos);
    bool found = false;
    for (ymuint i = 0; i < i_gate->act_nfo(); ++ i) {
      if ( i_gate->act_fanout(i) == gate &&
	   i_gate->act_fanout_ipos(i) == ipos ) {
	GateSa* f_gate = new_fault(f->val3());
	insert(i_gate, i, gate, ipos, f_gate);
	found = true;
	break;
      }
    }
    assert_cond(found, __FILE__, __LINE__);
  }
}

// @brief f に対応する故障ノードを取り除く．
void
Network::remove_fault(SaFault* f)
{
  if ( f->is_output_fault() ) {
    Gate* i_gate = mGateMap[f->node()->gid()];
    for (ymuint i = 0; i < i_gate->act_nfo(); ++ i) {
      Gate* gate1 = i_gate->act_fanout(i);
      if ( gate1->is_f_site() ) {
	GateSa* f_gate = dynamic_cast<GateSa*>(gate1);
	assert_cond(f_gate, __FILE__, __LINE__);
	if ( f_gate->is_po() ) {
	  f_gate->rst_po();
	  i_gate->set_po();
	  i_gate->set_act_no(i_gate->act_nfo() - 1);
	}
	else {
	  Gate* o_gate = f_gate->act_fanout(0);
	  ymuint ipos = f_gate->act_fanout_ipos(0);
	  i_gate->set_act_fanout(i, o_gate, ipos);
	  o_gate->set_fanin(ipos, i_gate);
	}
	delete_fault(f_gate);
      }
    }
  }
  else {
    Gate* gate = mGateMap[f->node()->gid()];
    GateSa* f_gate = dynamic_cast<GateSa*>(gate->fanin(f->pos()));
    assert_cond(f_gate, __FILE__, __LINE__);
    Gate* i_gate = f_gate->fanin(0);
    Gate* o_gate = f_gate->act_fanout(0);
    ymuint ipos = f_gate->act_fanout_ipos(0);
    bool found = false;
    for (ymuint i = 0; i < i_gate->act_nfo(); ++ i) {
      if ( i_gate->act_fanout(i) == f_gate &&
	   i_gate->act_fanout_ipos(i) == 0 ) {
	i_gate->set_act_fanout(i, o_gate, ipos);
	o_gate->set_fanin(ipos, i_gate);
	found = true;
	break;
      }
    }
    assert_cond(found, __FILE__, __LINE__);
    delete_fault(f_gate);
  }
}

// @brief inject_fault 用の下請け関数
void
Network::insert(Gate* i_gate,
		ymuint opos,
		Gate* o_gate,
		ymuint ipos,
		Gate* f_gate)
{
  i_gate->set_act_fanout(opos, f_gate, 0);
  f_gate->set_fanin(0, i_gate);
  f_gate->set_act_fanout(0, o_gate, ipos);
  o_gate->set_fanin(ipos, f_gate);
  f_gate->mLevel = i_gate->level() + 1;
}

// @brief 故障ノードを挿入する．(外部出力の場合)
void
Network::insert(Gate* i_gate,
		Gate* f_gate)
{
  i_gate->rst_po();
  ymuint no = i_gate->act_nfo();
  i_gate->set_act_fanout(no, f_gate, 0);
  i_gate->set_act_no(no + 1);
  f_gate->set_po();
  f_gate->set_fanin(0, i_gate);
  f_gate->set_act_no(0);
  f_gate->mLevel = i_gate->level() + 1;
}

// @brief 新しい故障ノードを取り出す．
// @param[in] fval 故障値
GateSa*
Network::new_fault(Val3 fval)
{
  GateSa* f_gate;
  if ( fval == kVal0 ) {
    if ( mSa0List.empty() ) {
      f_gate = new GateSa0;
    }
    else {
      f_gate = mSa0List.front();
      mSa0List.pop_front();
    }
  }
  else {
    if ( mSa1List.empty() ) {
      f_gate = new GateSa1;
    }
    else {
      f_gate = mSa1List.front();
      mSa1List.pop_front();
    }
  }
  f_gate->init_fogate(0);
  f_gate->set_active();
  f_gate->set_act_no(1);
  f_gate->set_fcone();
  f_gate->mState = 0;
  f_gate->clr_flag();
  f_gate->set_fj();
  
  return f_gate;
}
  
// @brief 故障ノードを削除する．
void
Network::delete_fault(GateSa* f_gate)
{
  if ( f_gate->gate_type() == kTgConst0 ) {
    mSa0List.push_back(f_gate);
  }
  else {
    mSa1List.push_back(f_gate);
  }
}

const ymuint32 count_limit = 0xffffffff;

// common count for chk_mark()
ymuint32 gn_common_count = count_limit;

// @brief 全ゲートのマークを消す．
void
Network::clear_mark()
{
  if ( gn_common_count >= count_limit ) {
    for (vector<Gate*>::iterator p = mGateList.begin();
	 p != mGateList.end(); ++ p) {
      (*p)->rst_mark();
    }
    gn_common_count = 0;
  }
  ++ gn_common_count;
}

END_NAMESPACE_YM_ATPG_DTPGCL
