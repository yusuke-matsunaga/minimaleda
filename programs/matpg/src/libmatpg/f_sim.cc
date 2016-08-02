/*
 *	matpg - Mat's Automatic Testpattern Generator
 *	10/5/91 by Mat.
 *	$Log: f_sim.c,v $
 * Revision 2.8  1992/01/31  00:20:11  yusuke
 * Febrary '92 version
 *
 * Revision 2.7  1992/01/22  23:49:14  yusuke
 * no difference
 *
 * Revision 2.6  92/01/03  17:40:24  yusuke
 * divide calc_gval
 * no differential version
 * 
 * Revision 2.5  92/01/01  20:15:41  yusuke
 * bug fixed 
 * 
 * Revision 2.4  91/12/30  21:29:23  yusuke
 * change chk_EVENT_STRONG() and chk_EVENT_PROP() to
 * Ulong event = get_EVENT()
 * 
 * Revision 2.3  91/12/29  19:02:17  yusuke
 * 
 * add EVENT_WEAK
 * 
 * Revision 2.2  91/12/28  16:58:43  yusuke
 * Final , Final revision
 * 
 * Revision 2.1  91/12/23  23:11:05  yusuke
 * a slightly speed-up
 * 
 * Revision 2.0  91/12/21  18:50:28  yusuke
 * '91 Cristmas version
 * 
 * Revision 1.7  1991/10/05  08:18:18  yusuke
 * add Log and RCSid for RCS
 *
 */

#include "matpg.h"
#include "network.h"
#include "f_sim.h"
#include "fault.h"
#include "testpat.h"


BEGIN_NAMESPACE_YM_MATPG

const ymulong EVENT_NONE = 0L;
const ymulong EVENT_PROP = GN_event_p;

static gate_t** array = NULL;
static size_t size = 0;
static gate_t** clr_array = NULL;
static size_t size2 = 0;
static int ca_num = 0;

static int event_num;
static int min_num;

static void calc_gval(testpat_t** tps, size_t bitlen);
static pckval calc_ob(gate_t* gate);
static void event_queue_init();
static pckval event_queue_simulate();


inline
void
event_queue_set(gate_t* g,
		ymulong event)
{
  if (g->get_EVENT() == EVENT_NONE) {
    int lvl = g->lvl_i;
    g->next = array[lvl];
    array[lvl] = g;
    event_num ++;
  }
  g->set_EVENT(event);
}

inline
void
set_min_level(int lvl)
{
  if (min_num > lvl) {
    min_num = lvl;
  }
}

void
fault_sim(testpat_t** tps,
	  int bitlen,
	  vector<list<fault_t*> >& det_faults)
{
  if ( bitlen == 0 ) {
    return;
  }

  int old_tm_id = sw_timer.change(TM_FSIM);

  /* calculate good circuit's value */
  calc_gval(tps, bitlen);

  event_queue_init();
  gn_clr_mark();

  size_t nf = fault_u_num();
  for (size_t i = 0; i < nf; ) {
    fault_t* f = fault_get(i);
    if ( !f->is_undetected() && !f->is_aborted() ) {
      ++ i;
      continue;
    }
    
    gate_t* fos = f->get_gate()->get_fos();
    size_t j;
    for (j = i; j < nf; ++ j) {
      if ( fault_get(j)->get_gate()->get_fos() != fos ) {
	break;
      }
    }

    // i から j - 1 までが f と同じ FFR の故障となる．
    pckval diff_all = pckval_ALL0;
    for (size_t k = i; k < j; ++ k) {
      f = fault_get(k);
      if ( !f->is_undetected() && !f->is_aborted() ) {
	continue;
      }
      gate_t* gate = f->get_gate();
      int ipos = f->get_ipos();
      pckval diff = f->get_input_gate()->g_pckval;
      if ( f->get_fval() == val_1 ) {
	diff ^= pckval_ALL1;
      }
      if ( ipos >= 0 ) {
	diff &= gate->calc_obs(ipos);
      }
      diff &= calc_ob(gate);
      diff_all |= (f->fpckval = diff);
    }
    if ( diff_all == pckval_ALL0 ) {
      i = j;
      continue;
    }
    
    pckval obs_fos;
    if ( fos->is_po() ) {
      obs_fos = pckval_ALL1;
    }
    else {
      set_min_level(fos->lvl_i);
      clr_array[ca_num ++] = fos;
      fos->f_pckval = fos->g_pckval ^ diff_all;
      for (int k = fos->get_no(); -- k >= 0; ) {
	event_queue_set(fos->get_fogate(k), EVENT_PROP);
      }
      obs_fos = event_queue_simulate();
    }
    if ( obs_fos ) {
      for (size_t k = i; k < j; ++ k) {
	f = fault_get(k);
	if ( !f->is_undetected() && !f->is_aborted() ) {
	  continue;
	}
	pckval diff = f->fpckval & obs_fos;
	if ( diff ) {
	  size_t l;
	  for (l = 0; (diff & 1UL) == pckval_ALL0; ++ l, diff >>= 1) ;
	  det_faults[l].push_back(f);
	  testpat_t* tp = tps[l];
	  tp->det_count ++;
#if 0
	  f->set_detected(tp);
#endif
	}
      }
    }
    i = j;
  }

  sw_timer.change(old_tm_id);
}

static
void
calc_gval(testpat_t** tps,
	  size_t bitlen)
{
  size_t npi = gn_get_npi();
  pckval* ppats[pckval_bitlen];
  pckval pats[pckval_bitlen];
  for (size_t i = 0; i < pckval_bitlen; i ++) {
    if (i < bitlen) {
      ppats[i] = tps[i]->pat;
    }
    else {
      ppats[i] = tps[0]->pat;
    }
    pats[i] = *ppats[i];
  }
  gate_t** ptr = gates;
  int sht_num = pckval_bitlen - 1;
  for (size_t i = 0; i < npi; i ++) {
    pckval val = 0L;
    pckval bitmask = 1UL;
    for (size_t j = 0; j < pckval_bitlen; ++ j, bitmask <<= 1) {
      if ( (pats[j] >> sht_num) & 1UL ) {
	val |= bitmask;
      }
    }
    if (sht_num == 0) {
      sht_num = pckval_bitlen;
      for (size_t j = 0; j < pckval_bitlen; j ++) {
	pats[j] = *(++ ppats[j]);
      }
    }
    sht_num --;
    gate_t* gate = *ptr ++;
    gate->g_pckval = gate->f_pckval = val;
  }

  size_t ng = gn_get_ngate();
  for (size_t i = npi; i < ng; ++ i) {
    gn_get_gate(i)->calc_gval();
  }
}

inline
pckval&
obs(gate_t* gate)
{
  return (pckval&) gate->side_val;
}

static
pckval
calc_ob(gate_t* gate)
{
  if ( gate->is_fos() ) {
    return pckval_ALL1;
  }
  if ( gate->chk_mark() ) {
    return obs(gate);
  }
  gate->set_mark();
  gate_t* ogate = gate->get_fogate(0);
  int ipos = gate->get_fogate_ipos(0);
  pckval tmp = calc_ob(ogate) & ogate->calc_obs(ipos);
  obs(gate) = tmp;
  return tmp;
}

static
void
event_queue_init()
{
  size_t new_size = gn_get_max_level() + 1;
  if ( new_size < 128 ) {
    new_size = 128;
  }

  if (new_size > size) {
    if (size > 0) {
      delete [] array;
    }
    size = 1;
    do {
      size <<= 1;
    } while (size < new_size);
    array = new gate_t*[size];
  }
  event_num = 0;
  min_num = size + 1;
  for (size_t i = 0; i < size; ++ i) {
    array[i] = NULL;
  }
  
  size_t new_size2 = gn_get_ngate();
  if ( new_size2 < 128 ) {
    new_size2 = 128;
  }
  if (new_size2 > size2) {
    if (size2 > 0) {
      delete [] clr_array;
    }
    size2 = 1;
    do {
      size2 <<= 1;
    } while (size2 < new_size2);
    clr_array = new gate_t*[size2];
  }
  ca_num = 0;
}

static
pckval
event_queue_simulate()
{
  pckval diff_all = pckval_ALL0;
  gate_t** ptr = array + min_num;
  gate_t** end;
  while ( event_num > 0 ) {
    for ( gate_t* gate = *ptr; gate; gate = gate->next) {
      -- event_num;
      pckval val = gate->calc_fval();
      gate->rst_EVENT();
      if ( val == gate->f_pckval ) {
	continue;
      }
      clr_array[ca_num ++] = gate;
      gate->f_pckval = val;
      if ( gate->is_po() ) {
	diff_all |= val ^ gate->g_pckval;
      }
      for ( int j = gate->get_no(); -- j >= 0; ) {
	event_queue_set(gate->get_fogate(j), EVENT_PROP);
      }
    }
    *ptr ++  = NULL;
  }
  event_num = 0;
  min_num = size + 1;
  
  ptr = clr_array;
  end = clr_array + ca_num;
  while (ptr != end) {
    gate_t* gate = *ptr ++;
    gate->f_pckval = gate->g_pckval;
  }
  ca_num = 0;

  return diff_all;
}

END_NAMESPACE_YM_MATPG
