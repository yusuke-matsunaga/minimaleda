/*
 *	matpg - Mat's Automatic Testpattern Generator
 *	10/5/91 by Mat.
 *	$Log: cfna.c,v $
 * Revision 2.5  1992/01/22  23:49:14  yusuke
 * no difference
 *
 * Revision 2.5  1992/01/22  23:49:14  yusuke
 * no difference
 *
 * Revision 2.4  91/12/28  16:58:37  yusuke
 * Final , Final revision
 * 
 * Revision 2.3  91/12/26  19:56:52  yusuke
 * Final revision of version 2
 * 
 * Revision 2.2  91/12/24  14:39:33  yusuke
 * 
 * a little bit modification
 * 
 * Revision 2.1  91/12/23  23:10:54  yusuke
 * a slightly speed-up
 * 
 * Revision 2.0  91/12/21  18:49:11  yusuke
 * '91 Cristmas version
 * 
 * Revision 1.5  1991/10/05  08:18:18  yusuke
 * add Log and RCSid for RCS
 *
 */

#include "matpg.h"
#include "network.h"
#include "search_mgr.h"
#include "learn.h"
#include "backtrace.h"
#include "cfna.h"
#include "maqueue.h"
#include "testpat.h"


BEGIN_NAMESPACE_YM_MATPG

static
int cur_pat_size = -1;

static
code2val cur_codeval;

static
vector<gate_t*> temp_array;

static
bool
rf_chk(gate_t* gate);

static
bool
basis_just(gate_t* jnode,
	   val3 val);

static
void
clr_pat();

static
void
set_pat();

static
void
record_pat(gate_t* gate,
	   val3 val);

static
void
get_pat(gate_t* gate,
	val3 val);

inline
void
set_0(int pos)
{
  int blk = pos / pckval_bitlen;
  const pckval msk = 1UL << ((pckval_bitlen - 1 - pos) % pckval_bitlen);
  cur_codeval.v0[blk] |= msk;
  //cur_codeval.v1[blk] &= ~ msk;
}

inline
void
set_1(int pos)
{
  int blk = pos / pckval_bitlen;
  const pckval msk = 1UL << ((pckval_bitlen - 1 - pos) % pckval_bitlen);
  cur_codeval.v1[blk] |= msk;
  //cur_codeval.v0[blk] &= ~ msk;
}

#if 0
inline
ymulong&
rf_gate(gate_t* gate)
{
  return gate->side_val;
}
#else
static
vector<ymuint> rf_gate_array;

static
vector<ymuint> br_mark_array;

inline
ymuint&
rf_gate(gate_t* gate)
{
  return rf_gate_array[gate->id];
}
#endif

void
cfna_erase()
{
  size_t n = gn_get_ngate();
  for (size_t i = 0; i < n; ++ i) {
    gate_t* gate = gn_get_gate(i);
    for (size_t j = 0; j < 2; ++ j) {
      code2val& c2v = gate->cfna_pat[j];
      delete [] c2v.v0;
      delete [] c2v.v1;
      c2v.v0 = c2v.v1 = NULL;
    }
  }
  cur_pat_size = -1;
}

void
cfna_clr()
{
  temp_array.clear();
}

void
cfna_add(gate_t* gate)
{
  temp_array.push_back(gate);
}

void
cfna_recpat(testpat_t* tp)
{
  static RandGen randgen;
  set_pat();
  tp->randpat(gn_get_npi(), randgen);
  tp->andor(cur_codeval.v0, cur_codeval.v1);
}

void
cfna_init()
{
#ifdef DBG
  fprintf(stderr, "Finding conflict-free node assignment.\n");
#endif

  size_t npi = gn_get_npi();
  size_t nn = gn_get_ngate();

  cur_pat_size = (npi + pckval_bitlen - 1) / pckval_bitlen;
  cur_codeval.v0 = new pckval[cur_pat_size];
  cur_codeval.v1 = new pckval[cur_pat_size];
  
  rf_gate_array.clear();
  rf_gate_array.resize(nn);
  br_mark_array.clear();
  br_mark_array.resize(nn);
  
  // mark PI as a cfna node
  for (size_t i = 0; i < npi; ++ i) {
    gate_t* gate = gn_get_gate(i);
    gate->set_cfna0();
    gate->set_cfna1();
  }
  
  // find basis node
  for (size_t i = npi; i < nn; ++ i) {
    gate_t* gate = gn_get_gate(i);
    if ( gate->is_basis() ) {
      if ( basis_just(gate, val_0) ) {
	gate->set_cfna0();
      }
      if ( basis_just(gate, val_1) ) {
	gate->set_cfna1();
      }
    }
  }
  
  for (size_t i = 0; i < npi; ++ i) {
    gate_t* gate = gn_get_gate(i);
    if ( rf_chk(gate) ) {
      rf_gate(gate) = 1;
    }
    else {
      rf_gate(gate) = 0;
    }
  }
  
  clr_pat();
  for (size_t i = npi; i < nn; ++ i) {
    gate_t* gate = gn_get_gate(i);
    if ( !gate->chk_term0() && gate->just_cfna(val_0) ) {
      record_pat(gate, val_0);
      clr_pat();
      gate->set_cfna0();
    }
    if ( !gate->chk_term1() && gate->just_cfna(val_1) ) {
      record_pat(gate, val_1);
      clr_pat();
      gate->set_cfna1();
    }
    if ( (gate->chk_cfna0() || gate->chk_cfna1()) &&
	 !rf_chk(gate) ) {
      rf_gate(gate) = 0;
    }
    else {
      rf_gate(gate) = 1;
    }
  }
  
#ifdef DBG
  fprintf(stderr, "   ...end.\n");
#endif
}

bool
gate_t::just_cfna(val3)
{
  FATAL("gate_t::just_cfna() never occures\n");
  return false;
}

bool
C0_gate_t::just_cfna(val3 val)
{
  if (val == val_0) {
    return true;
  }
  return false;
}

bool
C1_gate_t::just_cfna(val3 val)
{
  if (val == val_1) {
    return true;
  }
  return false;
}

bool
BUF_gate_t::just_cfna(val3 val)
{
  gate_t* i_gate = get_figate(0);
  if ( rf_gate(i_gate) == 1 ) {
    return false;
  }
  if ( get_gtype() == kTgNot ) {
    val = neg3(val);
  }
  if ( (val == val_0 && i_gate->chk_term0()) ||
       (val == val_1 && i_gate->chk_term1()) ) {
    get_pat(i_gate, val);
    return true;
  }
  return false;
}

bool
SIMPLE_gate_t::just_cfna(val3 val)
{
  if ( val == o_val ) {
    for (int i = 0; i < ni; i ++) {
      gate_t* i_gate = get_figate(i);
      if ( rf_gate(i_gate) == 1 ) {
	continue;
      }
      if ( (c_val == val_0 && i_gate->chk_term0()) ||
	   (c_val == val_1 && i_gate->chk_term1()) ) {
	get_pat(i_gate, c_val);
	return true;
      }
    }
  }
  else {
    val3 nc_val = neg3(c_val);
    for (int i = 0; i < ni; i ++) {
      gate_t* i_gate = get_figate(i);
      if ( rf_gate(i_gate) == 1 ) {
	return false;
      }
      if ( (nc_val == val_0 && !i_gate->chk_term0()) ||
	   (nc_val == val_1 && !i_gate->chk_term1()) ) {
	return false;
      }
    }
    for (int i = 0; i < ni; i ++) {
      get_pat(get_figate(i), nc_val);
    }
    return true;
  }
  return false;
}

bool
XOR_gate_t::just_cfna(val3 val)
{
  if ( get_gtype() == kTgXnor ) {
    val = neg3(val);
  }

  bool slack = false;
  gate_t* slack_gate = NULL;
  for (int i = 0; i < ni; ++ i) {
    gate_t* i_gate = get_figate(i);
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
      val = neg3(val);
    }
    else {
      return false;
    }
  }
  if ( val == val_0 ) {
    slack_gate = NULL;
  }
  for (int i = 0; i < ni; ++ i) {
    gate_t* i_gate = get_figate(i);
    if ( i_gate != slack_gate && i_gate->chk_term0() ) {
      get_pat(i_gate, val_0);
    }
    else {
      get_pat(i_gate, val_1);
    }
  }
  return true;
}

#if 0
inline
ymulong&
br_mark(gate_t* gate)
{
  return gate->g_pckval;
}
#else
inline
ymuint&
br_mark(gate_t* gate)
{
  return br_mark_array[gate->id];
}
#endif

static
bool
rf_chk_dfs(gate_t* gate,
	   ymuint mark)
{
  if ( gate->chk_mark() ) {
    if ( br_mark(gate) != mark ) {
      return true;
    }
    return false;
  }
  gate->set_mark();
  br_mark(gate) = mark;
  for (int i = gate->get_no(); -- i >= 0; ) {
    bool stat = rf_chk_dfs(gate->get_fogate(i), mark);
    if ( stat ) {
      return true;
    }
  }
  return false;
}

// reconvergent fanout を持つとき true を返す．
static
bool
rf_chk(gate_t* gate0)
{
  int no = gate0->get_no();
  
  if ( no < 2 ) {
    return false;
  }
  
  gn_clr_mark();
  for (int i = no; -- i >= 0; ) {
    bool stat = rf_chk_dfs(gate0->get_fogate(i), i);
    if ( stat ) {
      return true;
    }
  }
  return false;
}

// jnode を val にするパタンを求める．
static
bool
basis_just(gate_t* jnode,
	   val3 val)
{
  // ラーニングは一時的に禁止しておく
  push_learn_flag();
  dont_use_learning();

  // jnode のファンアウトも一時的に切っておく
  int jnode_no = jnode->get_act_no();
  jnode->set_act_no(0);

  // 一連の初期化
  maq_clr(true);
  cfna_clr();

  // 要求値の設定
  set_request(jnode, val, val_X);

  // main loop
  bool flag = false;
  for (; ; ) {
    if ( !implication2() ) {
      if ( back_track() ) {
	continue;
      }
      if ( is_aborted() ) {
	if ( get_verbose_level() > 1 ) {
	  fprintf(ver_fp, "justificatoin aborted\n");
	}
      }
      else {
	if ( get_verbose_level() > 1 ) {
	  fprintf(ver_fp, "%s %s never occurs\n",
		  jnode->get_name(), value_name(val, val));
	}
	if ( val == val_0 ) {
	  const1_add(jnode);
	}
	else {
	  const0_add(jnode);
	}
	flag = true;
      }
      break;
    }
    if ( !C_decision(jnode) ) {
      // justified
      set_pat();
      record_pat(jnode, val);
      flag = true;
      break;
    }
  }

  // post-processing
  restore_value();
  jnode->set_act_no(jnode_no);
  pop_learn_flag();
  
  return flag;
}

static
void
clr_pat()
{
  for (int i = cur_pat_size; -- i >= 0; ) {
    cur_codeval.v0[i] = pckval_ALL0;
    cur_codeval.v1[i] = pckval_ALL0;
  }
}

static
void
set_pat()
{
  for (int i = cur_pat_size; -- i >= 0; ) {
    cur_codeval.v0[i] = pckval_ALL0;
    cur_codeval.v1[i] = pckval_ALL0;
  }
  for (size_t i = 0; i < temp_array.size(); ++ i) {
    gate_t* gate = temp_array[i];
    if ( !gate->chk_gj() ) {
      continue;
    }
    if ( gate->get_gval() == val_0 ) {
      if ( gate->chk_term0() ) {
	get_pat(gate, val_0);
      }
    }
    else if ( gate->get_gval() == val_1 ) {
      if ( gate->chk_term1() ) {
	get_pat(gate, val_1);
      }
    }
  }
}

static
void
record_pat(gate_t* gate,
	   val3 val)
{
  code2val& entry = gate->cfna_pat[((val == val_1) ? 1 : 0)];
  entry.v0 = new pckval[cur_pat_size];
  entry.v1 = new pckval[cur_pat_size];
  pckval* dst0 = entry.v0;
  pckval* dst1 = entry.v1;
  pckval* src0 = cur_codeval.v0;
  pckval* src1 = cur_codeval.v1;
  pckval* end = entry.v0 + cur_pat_size;
  while (dst0 != end) {
    (*dst0 ++) = (*src0 ++);
    (*dst1 ++) = (*src1 ++);
  }
}

static
void
get_pat(gate_t* gate,
	val3 val)
{
  if ( gate->is_pi() ) {
    if ( val == val_0 ) {
      set_0(gate->get_pi_id());
    }
    else {
      set_1(gate->get_pi_id());
    }
  }
  else {
    code2val& entry = gate->cfna_pat[((val == val_1) ? 1 : 0)];
    assert_cond(entry.v0, __FILE__, __LINE__);
    pckval* dst0 = cur_codeval.v0;
    pckval* dst1 = cur_codeval.v1;
    pckval* src0 = entry.v0;
    pckval* src1 = entry.v1;
    pckval* end = cur_codeval.v0 + cur_pat_size;
    while (dst0 != end) {
      (*dst0 ++) |= (*src0 ++);
      (*dst1 ++) |= (*src1 ++);
    }
  }
}

END_NAMESPACE_YM_MATPG
