
/// @file src/dtpg_classic/PGraph.cc
/// @brief PGraph の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: PGraph.cc 2203 2009-04-16 05:04:40Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "PGraph.h"
#include "LvlQ.h"
#include "SearchMgr.h"


BEGIN_NAMESPACE_YM_ATPG_DTPGCL

PGraph pgraph;


// @brief コンストラクタ
PGraph::PGraph() :
  mFaultSrc(NULL),
  mChangeFlag(false)
{
}

// @brief デストラクタ
PGraph::~PGraph()
{
}

// @brief PGraph を構築する．
void
PGraph::construct(Gate* f_gate)
{
  // f_gate の TFO に fcone フラグと pg/dif フラグをつける．
  // fcone フラグは destruct() するまで固定
  // dif フラグは gval と fval が X 以外で同じになったら下ろす．
  // pg フラグは自分の TFO に dif フラグを持ったゲートが
  // なくなったら下ろす．
  mFaultSrc = f_gate;
  lvlq.clear();
  for (ymuint i = 0; i < f_gate->act_nfo(); ++ i) {
    Gate* gate1 = f_gate->act_fanout(i);
    if ( gate1->is_f_site() ) {
      lvlq.put(gate1);
    }
  }
  for (Gate* gate1; (gate1 = lvlq.get_from_bottom()); ) {
    gate1->set_fcone();
    gate1->set_pg_dif();
    for (ymuint i = 0; i < gate1->act_nfo(); ++ i) {
      Gate* o_gate = gate1->act_fanout(i);
      lvlq.put(o_gate);
    }
  }
  mChangeFlag = true;
  mIndifGates.clear();
}

// @brief PGraph を消去する．
void
PGraph::destruct()
{
  for (ymuint i = 0; i < mFaultSrc->act_nfo(); ++ i) {
    Gate* o_gate = mFaultSrc->act_fanout(i);
    if ( o_gate->is_f_site() ) {
      dest_sub(o_gate);
    }
  }
}

// @brief destruct() の下請け関数
void
PGraph::dest_sub(Gate* gate)
{
  gate->rst_fcone();
  gate->rst_pg_dif();
  for (ymuint i = 0; i < gate->act_nfo(); ++ i) {
    Gate* o_gate = gate->act_fanout(i);
    if ( o_gate->chk_fcone() ) {
      dest_sub(o_gate);
    }
  }
}

// @brief 更新する．
// @return 故障伝搬の可能性がなくなったら false を返す．
bool
PGraph::update()
{
  if ( mIndifGates.empty() ) {
    return true;
  }
  
  mChangeFlag = true;

  // 故障差の伝搬の可能性のなくなったゲートの pg/dif フラグを降ろす．
  lvlq.clear();
  for (vector<Gate*>::iterator p = mIndifGates.begin();
       p != mIndifGates.end(); ++ p) {
    Gate* gate = *p;
    gate->rst_pg_dif();
    for (ymuint i = 0; i < gate->act_nfo(); ++ i) {
      Gate* o_gate = gate->act_fanout(i);
      if ( o_gate->chk_dif() ) {
	lvlq.put(o_gate);
      }
    }
  }
  for (Gate* gate; (gate = lvlq.get_from_bottom());  ) {
    bool dif = false;
    for (ymuint i = 0; i < gate->nfi(); ++ i) {
      if ( gate->fanin(i)->chk_dif() ) {
	dif = true;
	break;
      }
    }
    if ( !dif ) {
      sm.save_value(gate);
      gate->rst_pg_dif();
      for (ymuint i = 0; i < gate->act_nfo(); ++ i) {
	Gate* o_gate = gate->act_fanout(i);
	if ( o_gate->chk_dif() ) {
	  lvlq.put(o_gate);
	}
      }
    }
  }

  // ファンアウト先に pg フラグの立ったゲートがないゲートの pg フラグを降ろす．
  lvlq.clear();
  for (vector<Gate*>::iterator p = mIndifGates.begin();
       p != mIndifGates.end(); ++ p) {
    Gate* gate = *p;
    for (ymuint i = 0; i < gate->nfi(); ++ i) {
      Gate* i_gate = gate->fanin(i);
      if ( i_gate->chk_pg() && !i_gate->is_po() ) {
	lvlq.put(i_gate);
      }
    }
  }
  for (Gate* gate; (gate = lvlq.get_from_top()); ) {
    bool pg = false;
    for (ymuint i = 0; i < gate->act_nfo(); ++ i) {
      if ( gate->act_fanout(i)->chk_pg() ) {
	pg = true;
	break;
      }
    }
    if ( !pg ) {
      sm.save_value(gate);
      gate->rst_pg();
      for (ymuint i = 0; i < gate->nfi(); ++ i) {
	Gate* i_gate = gate->fanin(i);
	if ( i_gate->chk_pg() && !i_gate->is_po() ) {
	  lvlq.put(i_gate);
	}
      }
    }
  }
  // どうせ clear_indif() でクリアするが念のため
  mIndifGates.clear();
  
  // mFaultSrc に故障伝搬の可能性があるか調べる．
  for (ymuint i = 0; i < mFaultSrc->act_nfo(); ++ i) {
    if ( mFaultSrc->act_fanout(i)->chk_pg() ) {
      return true;
    }
  }
#ifdef DEBUG
  cerr << "propagation graph disappeares..." << endl;
#endif
  return false;
}

// @brief 内容をダンプする(デバッグ用)
void
PGraph::dump(ostream& s) const
{
  s << "***************** PGraph::dump() ********************" << endl;
  hash_set<ymuint> mark;
  dump_sub(mFaultSrc, mark, s);
  s << "***************** PGraph::dump() ********************" << endl;
}

// @brief dump() の下請け関数
void
PGraph::dump_sub(Gate* gate,
		 hash_set<ymuint>& mark,
		 ostream& s)
{
  if ( mark.count(gate->id()) > 0 ) {
    return;
  }
  mark.insert(gate->id());

  if ( gate->is_po() ) {
    s << "<PO> ";
  }
  s << gate->name() << ": value = "
    << value_name(gate->gval(), gate->fval())
    << ": ";
  if ( gate->chk_pg() ) {
    s << "DIFF";
  }
  else {
    s << "____";
  }
  s << endl;
  
  ymuint ni = gate->nfi();
  for (ymuint i = 0; i < ni; ++ i) {
    Gate* gate1 = gate->fanin(i);
    s << "    [" << (i + 1) << "] "
      << gate1->name()
      << ": value = "
      << value_name(gate1->gval(), gate1->fval())
      << endl;
  }
  s << endl;
  
  ymuint no = gate->act_nfo();
  for (ymuint i = 0; i < no; ++ i) {
    Gate* gate1 = gate->act_fanout(i);
    if ( gate1->chk_fcone() ) {
      dump_sub(gate1, mark, s);
    }
  }
}

END_NAMESPACE_YM_ATPG_DTPGCL
