
/// @file src/dtpg_classic/t_analyze.cc
/// @brief testability measure の計算を行う関数の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: t_analyze.cc 2203 2009-04-16 05:04:40Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "Network.h"
#include "GateConst.h"
#include "GateInput.h"
#include "GateBuf.h"
#include "GateAnd.h"
#include "GateOr.h"
#include "GateXor.h"
#include "GateSa.h"

//#define DEBUG 1


BEGIN_NAMESPACE_YM_ATPG_DTPGCL

static
const ymuint MAX_VAL = 0xffffffff;

static
vector<ymuint> c0_array;

static
vector<ymuint> c1_array;

static
vector<ymuint> ob_array;

inline
void
set_c0(Gate* gate,
       ymuint val)
{
  c0_array[gate->id()] = val;
}

inline
void
set_c1(Gate* gate,
       ymuint val)
{
  c1_array[gate->id()] = val;
}

inline
void
set_ob(Gate* gate,
       ymuint val)
{
  ob_array[gate->id()] = val;
}

inline
ymuint
get_c0(Gate* gate)
{
  return c0_array[gate->id()];
}

inline
ymuint
get_c1(Gate* gate)
{
  return c1_array[gate->id()];
}

inline
ymuint
get_ob(Gate* gate)
{
  return ob_array[gate->id()];
}

void
Network::t_analyze()
{
  c0_array.clear();
  c0_array.resize(ngate(), 0);
  c1_array.clear();
  c1_array.resize(ngate(), 0);
  ob_array.clear();
  ob_array.resize(ngate(), 0);
  
  // 可制御性の計算
  for (ymuint i = 0; i < ngate(); ++ i) {
    Gate* gate = mGateList[i];
    gate->calc_C();
    if ( gate->is_basis() || gate->chk_cfna0() ) {
      set_c0(gate, 1);
    }
    if ( gate->is_basis() || gate->chk_cfna1() ) {
      set_c1(gate, 1);
    }
  }

  // 可観測性の計算
  for (ymuint i = ngate(); i > 0; ) {
    -- i;
    Gate* gate = mGateList[i];
    if ( gate->is_po() ) {
      set_ob(gate, 0);
      continue;
    }

    ymuint no = gate->nfo();
    if ( no == 0 ) {
      set_ob(gate, 0);
      continue;
    }
    
    vector<ymuint> obs(no);
    obs[0] = gate->fanout(0)->calc_O(gate->fanout_ipos(0));
    for (ymuint j = 1; j < no; ++ j) {
      Gate* ogate = gate->fanout(j);
      ymuint ipos = gate->fanout_ipos(j);
      ymuint ob = obs[j] = ogate->calc_O(ipos);
      ymuint k;
      for (k = 0; k < j; ++ k) {
	if ( obs[k] > ob ) {
	  break;
	}
      }
      if ( k < j ) {
	for (ymuint l = k; l <= j; ++ l) {
	  Gate* ogate2 = gate->fanout(l);
	  ymuint ipos2 = gate->fanout_ipos(l);
	  ymuint ob2 = obs[l];
	  gate->set_fanout(l, ogate, ipos);
	  obs[l] = ob;
	  ogate = ogate2;
	  ipos = ipos2;
	  ob = ob2;
	}
      }
    }
    set_ob(gate, obs[0]);
  }
}


//////////////////////////////////////////////////////////////////////
// クラス GateConst
//////////////////////////////////////////////////////////////////////

ymuint
GateConst::calc_O(ymuint)
{
  return get_ob(this);
}


//////////////////////////////////////////////////////////////////////
// クラス GateConst0
//////////////////////////////////////////////////////////////////////

void
GateConst0::calc_C()
{
  set_c0(this, 0);
  set_c1(this, MAX_VAL);

#ifdef DEBUG
  cerr << "GateConst0::calc_C = ("
       << get_c0(this) << ", " << get_c1(this) << ")"
       << endl;
#endif
}


//////////////////////////////////////////////////////////////////////
// クラス GateConst1
//////////////////////////////////////////////////////////////////////

void
GateConst1::calc_C()
{
  set_c0(this, MAX_VAL);
  set_c1(this, 0);

#ifdef DEBUG
  cerr << "GateConst1::calc_C = ("
       << get_c0(this) << ", " << get_c1(this) << ")"
       << endl;
#endif
}


//////////////////////////////////////////////////////////////////////
// クラス GateInput
//////////////////////////////////////////////////////////////////////

void
GateInput::calc_C()
{
  set_c0(this, 1);
  set_c1(this, 1);

#ifdef DEBUG
  cerr << name() << "::calc_C = ("
       << get_c0(this) << ", " << get_c1(this) << ")"
       << endl;
#endif
}

ymuint
GateInput::calc_O(ymuint)
{
  return get_ob(this);
}


//////////////////////////////////////////////////////////////////////
// クラス GateBuf
//////////////////////////////////////////////////////////////////////

void
GateBuf::calc_C()
{
  set_c0(this, get_c0(fanin(0)));
  set_c1(this, get_c1(fanin(0)));

#ifdef DEBUG
  cerr << name() << "::calc_C = ("
       << get_c0(this) << ", " << get_c1(this) << ")"
       << endl;
#endif
}

ymuint
GateBuf::calc_O(ymuint)
{
#ifdef DEBUG
  cerr << name() << "::calc_O(0) = " << get_ob(this) << endl;
#endif

  return get_ob(this);
}


//////////////////////////////////////////////////////////////////////
// クラス GateNot
//////////////////////////////////////////////////////////////////////

void
GateNot::calc_C()
{
  set_c0(this, get_c1(fanin(0)));
  set_c1(this, get_c0(fanin(0)));

#ifdef DEBUG
  cerr << name() << "::calc_C = ("
       << get_c0(this) << ", " << get_c1(this) << ")"
       << endl;
#endif
}

ymuint
GateNot::calc_O(ymuint)
{
#ifdef DEBUG
  cerr << name() << "::calc_O(0) = " << get_ob(this) << endl;
#endif

  return get_ob(this);
}


//////////////////////////////////////////////////////////////////////
// クラス GateAnd
//////////////////////////////////////////////////////////////////////

void
GateAnd::calc_C()
{
  ymuint min_0;
  ymuint sum_1;
  calc_C_sub(min_0, sum_1);
  set_c0(this, min_0);
  set_c1(this, sum_1);

#ifdef DEBUG
  cerr << name() << "::calc_C = ("
       << get_c0(this) << ", " << get_c1(this) << ")"
       << endl;
  cerr << "  [";
  for (ymuint i = 0; i < nfi(); ++ i) {
    cerr << " " << sorted_pos(i);
  }
  cerr << " ]" << endl;
#endif
}

ymuint
GateAnd::calc_O(ymuint pos)
{
  ymuint o_c = get_c1(this);
  ymuint i_c = get_c1(fanin(pos));

#ifdef DEBUG
  cerr << name() << "::calc_O(" << pos << ") = "
       << get_ob(this) + o_c - i_c
       << endl;
#endif

  return get_ob(this) + o_c - i_c;
}

void
GateAnd::calc_C_sub(ymuint& min_0,
		    ymuint& sum_1)
{
  sum_1 = get_c1(sorted_fanin(0));
  for (ymuint i = 1; i < nfi(); ++ i) {
    ymuint pos = sorted_pos(i);
    Gate* gate = fanin(pos);
    ymuint c0 = get_c0(gate);
    sum_1 += get_c1(gate);
    ymuint j;
    for (j = 0; j < i; ++ j) {
      if ( get_c0(sorted_fanin(j)) > c0 ) {
	break;
      }
    }
    if ( j < i ) {
      for (ymuint k = j; k <= i; ++ k) {
	ymuint pos2 = sorted_pos(k);
	set_sorted_pos(k, pos);
	pos = pos2;
      }	
    }
  }
  min_0 = get_c0(sorted_fanin(0));
}


//////////////////////////////////////////////////////////////////////
// クラス GateNand
//////////////////////////////////////////////////////////////////////

void
GateNand::calc_C()
{
  ymuint min_0;
  ymuint sum_1;
  calc_C_sub(min_0, sum_1);
  set_c0(this, sum_1);
  set_c1(this, min_0);

#ifdef DEBUG
  cerr << name() << "::calc_C = ("
       << get_c0(this) << ", " << get_c1(this) << ")"
       << endl;
  cerr << "  [";
  for (ymuint i = 0; i < nfi(); ++ i) {
    cerr << " " << sorted_pos(i);
  }
  cerr << " ]" << endl;
#endif
}

ymuint
GateNand::calc_O(ymuint pos)
{
  ymuint o_c = get_c0(this);
  ymuint i_c = get_c1(fanin(pos));

#ifdef DEBUG
  cerr << name() << "::calc_O(" << pos << ") = "
       << get_ob(this) + o_c - i_c
       << endl;
#endif

  return get_ob(this) + o_c - i_c;
}


//////////////////////////////////////////////////////////////////////
// クラス GateOr
//////////////////////////////////////////////////////////////////////

void
GateOr::calc_C()
{
  ymuint sum_0;
  ymuint min_1;
  calc_C_sub(sum_0, min_1);
  set_c0(this, sum_0);
  set_c1(this, min_1);

#ifdef DEBUG
  cerr << name() << "::calc_C = ("
       << get_c0(this) << ", " << get_c1(this) << ")"
       << endl;
  cerr << "  [";
  for (ymuint i = 0; i < nfi(); ++ i) {
    cerr << " " << sorted_pos(i);
  }
  cerr << " ]" << endl;
#endif
}

ymuint
GateOr::calc_O(ymuint pos)
{
  ymuint o_c = get_c0(this);
  ymuint i_c = get_c0(fanin(pos));

#ifdef DEBUG
  cerr << name() << "::calc_O(" << pos << ") = "
       << get_ob(this) + o_c - i_c
       << endl;
#endif

  return get_ob(this) + o_c - i_c;
}

void
GateOr::calc_C_sub(ymuint& sum_0,
		   ymuint& min_1)
{
  sum_0 = get_c0(sorted_fanin(0));
  for (ymuint i = 1; i < nfi(); ++ i) {
    ymuint pos = sorted_pos(i);
    Gate* gate = fanin(pos);
    ymuint c1 = get_c1(gate);
    sum_0 += get_c0(gate);
    ymuint j;
    for (j = 0; j < i; ++ j) {
      if ( get_c1(sorted_fanin(j)) > c1 ) {
	break;
      }
    }
    if ( j < i ) {
      for (ymuint k = j; k <= i; ++ k) {
	ymuint pos2 = sorted_pos(k);
	set_sorted_pos(k, pos);
	pos = pos2;
      }	
    }
  }
  min_1 = get_c1(sorted_fanin(0));
}


//////////////////////////////////////////////////////////////////////
// クラス GateNor
//////////////////////////////////////////////////////////////////////

void
GateNor::calc_C()
{
  ymuint sum_0;
  ymuint min_1;
  calc_C_sub(sum_0, min_1);
  set_c0(this, min_1);
  set_c1(this, sum_0);

#ifdef DEBUG
  cerr << name() << "::calc_C = ("
       << get_c0(this) << ", " << get_c1(this) << ")"
       << endl;
  cerr << "  [";
  for (ymuint i = 0; i < nfi(); ++ i) {
    cerr << " " << sorted_pos(i);
  }
  cerr << " ]" << endl;
#endif
}

ymuint
GateNor::calc_O(ymuint pos)
{
  ymuint o_c = get_c0(this);
  ymuint i_c = get_c1(fanin(pos));

#ifdef DEBUG
  cerr << name() << "::calc_O(" << pos << ") = "
       << get_ob(this) + o_c - i_c
       << endl;
#endif

  return get_ob(this) + o_c - i_c;
}


//////////////////////////////////////////////////////////////////////
// クラス GateXor
//////////////////////////////////////////////////////////////////////

void
GateXor::calc_C()
{
  if ( nfi() >= sizeof(ymulong) * 8 ) {
    FATAL("Too many fanins\n");
  }
  ymuint min_0 = MAX_VAL;
  ymuint min_1 = MAX_VAL;
  ymulong num_minterm = 1UL << nfi();
  for (ymulong bitpat = 0; bitpat < num_minterm; ++ bitpat) {
    ymulong mask = 1UL;
    int p = 0;
    ymuint sum = 0;
    for (ymuint i = 0; i < nfi(); ++ i) {
      if ( (bitpat & mask) == 0 ) {
	sum += get_c0(fanin(i));
      }
      else {
	sum += get_c1(fanin(i));
	p ^= 1;
      }
      mask <<= 1;
    }
    if ( p == 0 ) {
      if ( min_0 > sum ) {
	min_0 = sum;
      }
    }
    else {
      if ( min_1 > sum ) {
	min_1 = sum;
      }
    }
  }
  if ( gate_type() == kTgXor ) {
    set_c0(this, min_0);
    set_c1(this, min_1);
  }
  else {
    set_c1(this, min_0);
    set_c0(this, min_1);
  }

#ifdef DEBUG
  cerr << name() << "::calc_C = ("
       << get_c0(this) << ", " << get_c1(this) << ")"
       << endl;
#endif
}

ymuint
GateXor::calc_O(ymuint pos)
{
  if ( nfi() >= sizeof(ymulong) * 8 ) {
    FATAL("Too many fanins\n");
  }
  ymuint min_ob = MAX_VAL;
  ymulong num_minterm = 1UL << (nfi() - 1);
  for (ymulong bitpat = 0; bitpat < num_minterm; bitpat ++) {
    ymulong mask = 1UL;
    ymuint sum = 0;
    for (ymuint i = 0; i < nfi(); ++ i) {
      if ( i == pos ) {
	continue;
      }
      if ( (bitpat & mask) == 0 ) {
	sum += get_c0(fanin(i));
      }
      else {
	sum += get_c1(fanin(i));
      }
      mask <<= 1;
    }
    if ( min_ob > sum ) {
      min_ob = sum;
    }
  }
  
#ifdef DEBUG
  cerr << name() << "::calc_O(" << pos << ") = "
       << get_ob(this) + min_ob
       << endl;
#endif

  return min_ob + get_ob(this);
}


//////////////////////////////////////////////////////////////////////
// クラス GateSa
//////////////////////////////////////////////////////////////////////

void
GateSa::calc_C()
{
  assert_not_reached(__FILE__, __LINE__);
}

ymuint
GateSa::calc_O(ymuint)
{
  assert_not_reached(__FILE__, __LINE__);
  return 0;
}


END_NAMESPACE_YM_ATPG_DTPGCL
