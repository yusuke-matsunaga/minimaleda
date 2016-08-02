/*
 *	matpg - Mat's Automatic Testpattern Generator
 *	10/5/91 by Mat.
 *	$Log: network.c,v $
 * Revision 2.4  1992/01/22  23:49:14  yusuke
 * no difference
 *
 * Revision 2.3  91/12/28  16:59:09  yusuke
 * Final , Final revision
 * 
 * Revision 2.2  91/12/26  19:57:29  yusuke
 * Final revision of version 2
 * 
 * Revision 2.1  91/12/23  23:11:56  yusuke
 * a slightly speed-up
 * 
 * Revision 2.0  91/12/21  18:54:50  yusuke
 * '91 Cristmas version
 * 
 * Revision 1.7  1991/10/17  02:55:53  yusuke
 * made a new function eq_imp()
 *
 * Revision 1.6  1991/10/05  08:18:18  yusuke
 * add Log and RCSid for RCS
 *
 */

#include "matpg.h"
#include "gate.h"
#include "network.h"
#include <ym_tgnet/TgNetwork.h>
#include <ym_tgnet/TgNode.h>
#include <ym_tgnet/TgBlifReader.h>
#include "fault.h"
#include "testpat.h"
#include "learn.h"
#include "cfna.h"
#include "maqueue.h"
#include "t_analyze.h"
#include "used_gate.h"


BEGIN_NAMESPACE_YM_MATPG

/*  <<  gate_t Network  >>  */
static
ymuint npi = 0;

static
ymuint npo = 0;

static
ymuint ngate = 0;

gate_t** pos = NULL;

static
ymuint pos_size = 0;

gate_t** gates = NULL;

static
ymuint gates_size = 0;

static
ymuint act_ngate = 0;

gate_t** act_gates = NULL;

static
ymuint act_gates_size = 0;

static
ymuint max_level = 0;


/* Used in gn_read_blif */
static
gate_t** next_ptr;

static
void
ord_ffr(gate_t* gate);

static
void
set_eq_gate();

static
bool
eq(gate_t* gate1,
   gate_t* gate2);

static
void
calc_basis();

static
gate_t*
cap_dom(gate_t* d1,
	gate_t* d2);


inline
void
init_array(gate_t**& array,
	   ymuint& size,
	   ymuint new_size)
{
  if (size < new_size) {
    if (size) {
      delete [] array;
    }
    else {
      size = 1;
    }
    while (size < new_size) {
      size <<= 1;
    }
    array = new gate_t*[size];
  }
  else {
    for (size_t i = 0; i < size; ++ i) {
      array[i] = NULL;
    }
  }
}

ymuint
gn_get_npi()
{
  return npi;
}

ymuint
gn_get_npo()
{
  return npo;
}

ymuint
gn_get_ngate()
{
  return ngate;
}

ymuint
gn_get_max_level()
{
  return max_level;
}

void
gn_gen(const TgNetwork& tgnetwork)
{
  gn_erase();
  npi = tgnetwork.input_num2();
  npo = tgnetwork.output_num2();
  size_t nl = tgnetwork.logic_num();
  ngate = nl + npi;
  if (npi == 0 && npo == 0 && nl == 0) {
    return;
  }
  init_array(pos, pos_size, npo);
  init_array(gates, gates_size, ngate + 1);
  init_array(act_gates, act_gates_size, ngate);

  vector<gate_t*> gate_map(tgnetwork.node_num());
  size_t gate_id = 0;
  for (size_t i = 0; i < npi; ++ i) {
    const TgNode* node = tgnetwork.input(i);
    gate_t* gate = new PI_gate_t(node, i);
    gate->id = gate_id;
    gates[gate_id] = gate;
    ++ gate_id;
    gate_map[node->gid()] = gate;
  }
  for (size_t i = 0; i < nl; ++ i) {
    const TgNode* node = tgnetwork.sorted_logic(i);
    size_t ni = node->ni();
    gate_t* gate =  new_gate_t(node->type(), node, ni);
    for (size_t j = 0; j < ni; ++ j) {
      const TgNode* inode = node->fanin(j);
      gate_t* igate = gate_map[inode->gid()];
      gate->init_figate(j, igate);
    }
    gate->id = gate_id;
    gates[gate_id] = gate;
    ++ gate_id;
    gate_map[node->gid()] = gate;
    if (gate->get_gtype() == kTgConst0) {
      const0_add(gate);
    }
    if (gate->get_gtype() == kTgConst1) {
      const1_add(gate);
    }
  }
  
  for (size_t i = 0; i < npo; ++ i) {
    const TgNode* node = tgnetwork.output(i);
    const TgNode* inode = node->fanin(0);
    gate_t* gate = gate_map[inode->gid()];
    pos[i] = gate;
    gate->set_po();
  }
  
  // Connect output
  for (size_t i = npi; i < ngate; ++ i) {
    gate_t* gate = gn_get_gate(i);
    size_t ni = gate->get_ni();
    for (size_t j = 0; j < ni; ++ j) {
      gate->get_figate(j)->init_fogate(gate, j);
    }
  }
  for (size_t i = 0; i < npo; ++ i) {
    gn_get_po(i)->init_fogate(NULL, -1);
  }
  
  // find fanout stems
  for (int i = ngate; -- i >= 0; ) {
    gate_t* gate = gn_get_gate(i);
    if (gate->get_no() == 1 && gate->is_po() == false) {
      gate->set_fos(gate->get_fogate(0)->get_fos());
    }
    else {
      gate->set_fos(gate);
    }
  }
  
  // set level and ordering in FFR
  for (size_t i = 0; i < ngate; ++ i) {
    gate_t* gate = gn_get_gate(i);
    int ni = gate->get_ni();
    int level = -1;
    for (int j = ni; -- j >= 0; ) {
      int level1 = gate->get_figate(j)->lvl_i;
      if ( level < level1 ) {
	level = level1;
      }
    }
    gate->lvl_i = level + 1;
    if ( gate->is_fos() ) {
      next_ptr = &(gate->next_eval);
      for (size_t j = 0; j < ni; ++ j) {
	ord_ffr(gate->get_figate(j));
      }
      *next_ptr = gate;
    }
  }
  max_level = 0;
  for (size_t i = 0; i < npo; ++ i) {
    size_t level1 = gn_get_po(i)->lvl_i;
    if ( max_level < level1 ) {
      max_level = level1;
    }
  }
  lvlq_init(max_level);
  for (int i = ngate; -- i >= 0; ) {
    gate_t* gate = gn_get_gate(i);
    if (gate->is_po() == true) {
      gate->min_lvl_i = gate->lvl_i;
    }
    else {
      int j = gate->get_no();
      int level = max_level + 1;
      for ( ; -- j >= 0; ) {
	int level1 = gate->get_fogate(j)->min_lvl_i;
	if ( level > level1 ) {
	  level = level1;
	}
      }
      gate->min_lvl_i = level;
    }
  }
  
  // set equivallent gate
  set_eq_gate();
  
  used_gate_init(ngate);

  // find conflict-free gate assignment
  gn_activate_all();
  calc_basis();
  cfna_init();
  gn_deactivate();
  
  // testability measure
  t_analyze();
}

static
void
ord_ffr(gate_t* gate)
{
  if ( !gate->is_fos() && !gate->is_pi() ) {
    *next_ptr = gate;
    next_ptr = &(gate->next_eval);
    for (size_t i = 0; i < gate->get_ni(); ++ i) {
      ord_ffr(gate->get_figate(i));
    }
  }
}

static
void
set_eq_gate()
{
  for (size_t i = npi; i < ngate; i ++) {
    gate_t* gate = gn_get_gate(i);
    if (gate->eq_gate != gate) {
      continue;
    }
    int lvl_i = gate->lvl_i;
    int ni = gate->get_ni();
    if (ni == 0) {
      continue;
    }
    tTgGateType gtype = gate->get_gtype();
    gate_t* igate0 = gate->get_figate(0);
    gate_t* igate = igate0;
    if (igate->eq_gate == igate && igate->get_no() == 1) {
      continue;
    }
    do {
      for (int j = igate->get_no(); -- j >= 0; ) {
	gate_t* ogate = igate->get_fogate(j);
	if (ogate != gate && ogate->lvl_i == lvl_i
	    && ogate->eq_gate == ogate
	    && ogate->get_gtype() == gtype && ogate->get_ni() == ni) {
	  if (ni == 1 || eq(gate, ogate) == true) {
	    gate_t* top = gate->eq_gate;
	    gate->eq_gate = ogate;
	    ogate->eq_gate = top;
	  }
	}
      }
      igate = igate->eq_gate;
    } while (igate != igate0);
  }
  for (size_t i = ngate; -- i >= npi; ) {
    gate_t* gate = gn_get_gate(i);
    if (gate->get_ni() == 1 || gate->eq_gate == gate) {
      gate->eq_gate = NULL;
    }
    int j;
    for (j = gate->get_ni(); -- j >= 0; ) {
      if (gate->get_figate(j)->eq_gate == NULL) {
	break;
      }
    }
    if (j >= 0) {
      gate->eq_gate = NULL;
    }
  }
}

static
bool
eq(gate_t* gate1,
   gate_t* gate2)
{
  int ni = gate1->get_ni();
  vector<int> match_bits(ni, 0);
  for (int i = ni; -- i >= 0; ) {
    gate_t* igate1 = gate1->get_figate(i);
    int lvl1 = igate1->lvl_i;
    int j;
    for (j = ni; -- j >= 0; ) {
      gate_t* igate2 = gate2->get_figate(j);
      if (igate2->lvl_i != lvl1) {
	continue;
      }
      gate_t* temp = igate2;
      while ((temp = temp->eq_gate) != igate2) {
	if (temp == igate1) {
	  break;
	}
      }
      if (igate1 == temp) {
	match_bits[j] = 1;
	break;
      }
    }
    if (j < 0) {
      return false;
    }
  }
  for (int i = ni; -- i >= 0; ) {
    if (match_bits[i] == 1) {
      continue;
    }
    gate_t* igate2 = gate2->get_figate(i);
    int lvl2 = igate2->lvl_i;
    int j;
    for (j = ni; -- j >= 0; ) {
      gate_t* igate1 = gate1->get_figate(j);
      if (igate1->lvl_i != lvl2) {
	continue;
      }
      gate_t* temp = igate1;
      while ((temp = temp->eq_gate) != igate1) {
	if (temp == igate2) {
	  break;
	}
      }
      if (igate2 == temp) {
	break;
      }
    }
    if (j < 0) {
      return false;
    }
  }
  return true;
}

#if 0
inline
int&
gate_id(gate_t* gate)
{
  return (int&) gate->state;
}

inline
gate_t*&
dominator(gate_t* gate)
{
  return gate->next;
}

inline
gate_t*&
d_intsct(gate_t* gate)
{
  return gate->ggate;
}
#else
static
vector<ymuint> gid_array;
static
vector<gate_t*> dom_array;
static
vector<gate_t*> di_array;
inline
ymuint&
gate_id(gate_t* gate)
{
  return gid_array[gate->id];
}
inline
gate_t*&
dominator(gate_t* gate)
{
  return dom_array[gate->id];
}
inline
gate_t*&
d_intsct(gate_t* gate)
{
  return di_array[gate->id];
}
#endif

static
void
calc_basis()
{
  int nn = gn_get_ngate();
  int i;
  int j;
  
  gid_array.clear();
  gid_array.resize(nn);
  dom_array.clear();
  dom_array.resize(nn);
  di_array.clear();
  di_array.resize(nn);
  
  j = 1;
  for (i = nn; -- i >= 0; ) {
    gate_id(gn_get_gate(i)) = j ++;
  }

  for (i = nn; -- i >= 0; ) {
    gate_t* gate = gn_get_gate(i);
    int no = gate->get_no();
    gate_t* d = NULL;
    if (no > 0 && gate->is_po() == false) {
      d = gate->get_fogate(0);
      for (j = 1; j < no; j ++) {
	if ((d = cap_dom(d, gate->get_fogate(j))) == NULL) {
	  break;
	}
      }
    }
    dominator(gate) = d;
  }
  
  for (i = 0; i < nn; i ++) {
    gate_t* gate = gn_get_gate(i);
    int ni = gate->get_ni();
    gate_t* d;
    if (ni > 1) {
      d = d_intsct(gate->get_figate(0));
      for (j = 1; j < ni; j ++) {
	if ((d = cap_dom(d, d_intsct(gate->get_figate(j)))) == NULL) {
	  break;
	}
      }
    }
    else if (ni == 1) {
      d = gate->get_figate(0);
      if (d_intsct(d) == d) {
	d = dominator(d);
      }
      else {
	d = d_intsct(d);
      }
    }
    else {
      d = gate;
    }
    d_intsct(gate) = d;
    if (d == gate || (d != NULL && dominator(d) == gate)) {
      if (gate->is_po() == false || gate->get_no() > 0) {
	gate->set_basis();
      }
    }
  }
  
  for (i = nn; -- i >= 0; ) {
    gn_get_gate(i)->state = 0L;
  }
}

static
gate_t*
cap_dom(gate_t* d1,
	gate_t* d2)
{
  while (d1 != NULL && d2 != NULL) {
    int sign = gate_id(d1) - gate_id(d2);
    if (sign > 0) {
      d1 = dominator(d1);
    }
    else if (sign < 0) {
      d2 = dominator(d2);
    }
    else {
      return d1;
    }
  }
  return NULL;
}

void
gn_erase()
{
  fault_clr();
  tp_clr();
  const_clr();
  forget_learning();
  cfna_erase();
  for (size_t i = 0; i < ngate; ++ i) {
    delete gates[i];
  }
  npi = 0;
  npo = 0;
  ngate = 0;
  act_ngate = 0;
}

void
gn_dump(FILE* fp)
{
  for (size_t i = 0; i < ngate; ++ i) {
    gn_get_gate(i)->dump(fp);
  }
  for (size_t i = 0; i < npi; ++ i) {
    gate_t* gate = gn_get_pi(i);
    fprintf(fp, "P.I.(%Zd) = %s\n", i + 1, gate->get_name());
  }
  for (size_t i = 0; i < npo; ++ i) {
    gate_t* gate = gn_get_po(i);
    fprintf(fp, "P.O.(%Zd) = %s\n", i + 1, gate->get_name());
  }
}

void
gn_activate_all()
{
  act_ngate = ngate;
  for (int i = ngate; -- i >= 0; ) {
    gate_t* gate = act_gates[i] = gates[i];
    gate->set_active();
    int no = gate->get_no();
    int k = 0;
    for (int j = 0; j < no; j ++) {
      gate_t* ogate = gate->get_fogate(j);
      if (ogate->is_active() == true) {
	gate->set_act_fogate(k ++, ogate, gate->get_fogate_ipos(j));
      }
    }
    gate->set_act_no(k);
  }
}

static
bool reach_to_po;

static
void
mark_tfo(gate_t* gate)
{
  if (gate->is_active() == false) {
    gate->set_active();
    if ( gate->is_po() ) {
      reach_to_po = true;
    }
    lvlq_put(gate);
    for ( int i = gate->get_no(); -- i >= 0; ) {
      mark_tfo(gate->get_fogate(i));
    }
  }
}

bool
gn_activate_1fault(gate_t* f_gate)
{
  lvlq_clr();
  reach_to_po = false;
  mark_tfo(f_gate);
  if ( !reach_to_po ) {
    return false;
  }
  act_ngate = 0;
  gate_t* gate;
  int i;
  while ( (gate = lvlq_get_from_top()) ) {
    int k = 0;
    bool sc_flag = true;
    int no = gate->get_no();
    for (i = 0; i < no; i ++) {
      gate_t* o_gate = gate->get_fogate(i);
      if (o_gate->is_active() == true) {
	gate->set_act_fogate(k ++, o_gate, gate->get_fogate_ipos(i));
	if (o_gate->chk_scope() == false) {
	  sc_flag = false;
	}
      }
      else {
	sc_flag = false;
      }
    }
    if (sc_flag == true) {
      gate->set_scope();
    }
    gate->set_act_no(k);
    act_gates[act_ngate ++] = gate;
    for (i = gate->get_ni(); -- i >= 0; ) {
      gate_t* i_gate = gate->get_figate(i);
      if (i_gate->is_active() == false) {
	i_gate->set_active();
	lvlq_put(i_gate);
      }
    }
  }
  return true;
}

void
gn_deactivate()
{
  gate_t** ptr = act_gates;
  gate_t** end = act_gates + act_ngate;
  while (ptr != end) {
    gate_t* gate = (*ptr ++);
    gate->rst_active();
    gate->rst_scope();
    gate->set_act_no(0);
  }
  act_ngate = 0;
}

/* common count for chk_mark() */
ymuint gn_common_count = count_limit;

void
gn_clr_mark1()
{
  gate_t** ptr = gates;
  gate_t** end = gates + ngate;
  while (ptr != end) {
    (*ptr ++)->rst_mark();
  }
  gn_common_count = 0;
}

END_NAMESPACE_YM_MATPG
