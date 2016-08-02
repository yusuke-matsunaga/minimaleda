
/// @file src/dtpg_classic/Cfna.cc
/// @brief Cfna の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: Cfna.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "Cfna.h"
#include "Network.h"
#include "SearchMgr.h"
#include "backtrace.h"
#include "TestVector.h"
#include "TvMgr.h"
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

Cfna cfna;

// @brief コンストラクタ
// @param[in] tvmgr TvMgr
Cfna::Cfna() :
  mTvMgr(NULL),
  mNpi(0)
{
}

// @brief デストラクタ
Cfna::~Cfna()
{
  clear();
}

// @brief 初期化する．
void
Cfna::init(Network& network,
	   TvMgr& tvmgr)
{
  clear();
  
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
Cfna::clear()
{
  if ( mTvMgr ) {
    for (hash_map<ymuint, TestVector*>::iterator p = mAssignMap.begin();
	 p != mAssignMap.end(); ++ p) {
      mTvMgr->delete_vector(p->second);
    }
    mAssignMap.clear();
    mTvMgr = NULL;
  }
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

// @brief テストベクタをクリアする．
void
Cfna::clear_vector()
{
  mCurVector->init();
}

// @brief テストベクタをセットする．
void
Cfna::set_vector(const vector<pair<Gate*, Val3> >& term_list)
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
Cfna::record_vector(Gate* gate,
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
Cfna::get_vector(Gate* gate,
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
Cfna::record(const vector<pair<Gate*, Val3> >& term_list,
	     TestVector* tv)
{
  set_vector(term_list);
  tv->init();
  tv->copy(*mCurVector);
}

/// @brief ゲートごとの正当化を行う．
bool
Cfna::justify_gate(Gate* gate,
		   Val3 val)
{
  switch ( gate->gate_type() ) {
  case kTgInput:
    // 外部入力はもちろん正当化可能
    return true;
    
  case kTgConst0:
    if ( val == kVal0 ) {
      // 定数0は0のみ正当化可能
      return true;
    }
    return false;

  case kTgConst1:
    if ( val == kVal1 ) {
      // 定数1は1のみ正当化可能
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
	// ファンインが val を正当化できるならOK
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
      // 要求される値が制御出力値の場合
      for (ymuint i = 0; i < gate->nfi(); ++ i) {
	Gate* i_gate = gate->fanin(i);
	if ( rf_gate(i_gate) == 1 ) {
	  continue;
	}
	if ( i_gate->chk_term(gate->c_val()) ) {
	  // ファンインに制御入力値を正当化できるものがいれば OK
	  get_vector(i_gate, gate->c_val());
	  return true;
	}
      }
    }
    else {
      // 要求される値が非制御出力値の場合
      for (ymuint i = 0; i < gate->nfi(); ++ i) {
	Gate* i_gate = gate->fanin(i);
	if ( rf_gate(i_gate) == 1 ) {
	  return false;
	}
	if ( !i_gate->chk_term(gate->nc_val()) ) {
	  // ファンインに非制御出力値を正当化できないものがいたらNG
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
