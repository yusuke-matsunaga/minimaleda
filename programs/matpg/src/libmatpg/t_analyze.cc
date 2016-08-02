/*
 *	matpg - Mat's Automatic Testpattern Generator
 *	10/5/91 by Mat.
 *	$Log: t_analyze.c,v $
 * Revision 2.1  1992/01/22  23:49:14  yusuke
 * no difference
 *
 * Revision 2.1  1992/01/22  23:49:14  yusuke
 * no difference
 *
 * Revision 2.0  91/12/21  18:57:52  yusuke
 * '91 Cristmas version
 * 
 * Revision 1.7  1991/10/15  03:12:17  yusuke
 * *** empty log message ***
 *
 * Revision 1.6  1991/10/05  08:25:16  yusuke
 * bug fix
 * add #include "network.h"
 *
 * Revision 1.5  1991/10/05  08:18:18  yusuke
 * add Log and RCSid for RCS
 *
 */

#include "network.h"
#include "t_analyze.h"

//#define DEBUG 1


BEGIN_NAMESPACE_YM_MATPG

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
set_c0(gate_t* gate,
       ymuint val)
{
  c0_array[gate->id] = val;
}

inline
void
set_c1(gate_t* gate,
       ymuint val)
{
  c1_array[gate->id] = val;
}

inline
void
set_ob(gate_t* gate,
       ymuint val)
{
  ob_array[gate->id] = val;
}

inline
ymuint
get_c0(gate_t* gate)
{
  return c0_array[gate->id];
}

inline
ymuint
get_c1(gate_t* gate)
{
  return c1_array[gate->id];
}

inline
ymuint
get_ob(gate_t* gate)
{
  return ob_array[gate->id];
}

void
t_analyze()
{
  size_t nn = gn_get_ngate();
  
  c0_array.clear();
  c0_array.resize(nn, 0);
  c1_array.clear();
  c1_array.resize(nn, 0);
  ob_array.clear();
  ob_array.resize(nn, 0);

  // calculating controllability measure
  for (size_t i = 0; i < nn; ++ i) {
    gate_t* gate = gn_get_gate(i);
    gate->calc_C();
    if ( gate->is_basis() || gate->chk_cfna0() ) {
      set_c0(gate, 1);
    }
    if ( gate->is_basis() || gate->chk_cfna1() ) {
      set_c1(gate, 1);
    }
  }

  // calculating observability measure
  for (size_t i = nn; i > 0; ) {
    -- i;
    gate_t* gate = gn_get_gate(i);
    if ( gate->is_po() ) {
      set_ob(gate, 0);
      continue;
    }

    size_t no = gate->get_no();
    if ( no == 0 ) {
      set_ob(gate, 0);
      continue;
    }
    
    vector<ymuint> obs(no);
    obs[0] = gate->get_fogate(0)->calc_O(gate->get_fogate_ipos(0));
    for (size_t j = 1; j < no; ++ j) {
      gate_t* ogate = gate->get_fogate(j);
      int ipos = gate->get_fogate_ipos(j);
      ymuint ob = obs[j] = ogate->calc_O(ipos);
      size_t k;
      for (k = 0; k < j; ++ k) {
	if ( obs[k] > ob ) {
	  break;
	}
      }
      if ( k < j ) {
	for (size_t l = k; l <= j; ++ l) {
	  gate_t* ogate2 = gate->get_fogate(l);
	  int ipos2 = gate->get_fogate_ipos(l);
	  ymuint ob2 = obs[l];
	  gate->set_fogate(l, ogate, ipos);
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

/* dummy */
void
gate_t::calc_C()
{
}

ymuint
gate_t::calc_O(int)
{
  return 0;
}

/* C0_gate_t */
void
C0_gate_t::calc_C()
{
  set_c0(this, 0);
  set_c1(this, MAX_VAL);

#ifdef DEBUG
  fprintf(stderr, "C0_gate_t::calc_C = (%d, %d)\n", get_c0(this), get_c1(this));
#endif
}

/* C1_gate_t */
void
C1_gate_t::calc_C()
{
  set_c0(this, MAX_VAL);
  set_c1(this, 0);

#ifdef DEBUG
  fprintf(stderr, "C1_gate_t::calc_C = (%d, %d)\n", get_c0(this), get_c1(this));
#endif
}

/* BUF_gate_t */
void
BUF_gate_t::calc_C()
{
  set_c0(this, get_c0(get_figate(0)));
  set_c1(this, get_c1(get_figate(0)));

#ifdef DEBUG
  fprintf(stderr, "%s::calc_C = (%d, %d)\n",
	  get_name(), get_c0(this), get_c1(this));
#endif
}

ymuint
BUF_gate_t::calc_O(int)
{
#ifdef DEBUG
  fprintf(stderr, "%s::calc_O(%d) = %d\n", get_name(), 0, get_ob(this));
#endif

  return get_ob(this);
}

/* NOT_gate_t */
void
NOT_gate_t::calc_C()
{
  set_c0(this, get_c1(get_figate(0)));
  set_c1(this, get_c0(get_figate(0)));

#ifdef DEBUG
  fprintf(stderr, "%s::calc_C = (%d, %d)\n",
	  get_name(), get_c0(this), get_c1(this));
#endif
}

ymuint
NOT_gate_t::calc_O(int)
{
#ifdef DEBUG
  fprintf(stderr, "%s::calc_O(%d) = %d\n", get_name(), 0, get_ob(this));
#endif

  return get_ob(this);
}

/* PI_gate_t */
void
PI_gate_t::calc_C()
{
  set_c0(this, 1);
  set_c1(this, 1);

#ifdef DEBUG
  fprintf(stderr, "%s::calc_C = (%d, %d)\n",
	  get_name(), get_c0(this), get_c1(this));
#endif
}

/* SIMPLE_gate_t */
void
SIMPLE_gate_t::calc_C()
{
  bool and_flag = bool(get_c_val() == val_0);
  ymuint min_0;
  ymuint sum_1;
  if ( and_flag ) {
    sum_1 = get_c1(get_sorted_figate(0));
    for (int i = 1; i < ni; ++ i) {
      int pos = get_sorted_pos(i);
      gate_t* gate = get_figate(pos);
      ymuint c0 = get_c0(gate);
      sum_1 += get_c1(gate);
      int j;
      for (j = 0; j < i; j ++) {
	if ( get_c0(get_sorted_figate(j)) > c0 ) {
	  break;
	}
      }
      if ( j < i ) {
	for (int k = j; k <= i; k ++) {
	  int pos2 = get_sorted_pos(k);
	  set_sorted_pos(k, pos);
	  pos = pos2;
	}	
      }
    }
    min_0 = get_c0(get_sorted_figate(0));
  }
  else {
    sum_1 = get_c0(get_sorted_figate(0));
    for (int i = 1; i < ni; ++ i) {
      int pos = get_sorted_pos(i);
      gate_t* gate = get_figate(pos);
      ymuint c0 = get_c1(gate);
      sum_1 += get_c0(gate);
      int j;
      for (j = 0; j < i; j ++) {
	if (get_c1(get_sorted_figate(j)) > c0) {
	  break;
	}
      }
      if (j < i) {
	for (int k = j; k <= i; k ++) {
	  int pos2 = get_sorted_pos(k);
	  set_sorted_pos(k, pos);
	  pos = pos2;
	}	
      }
    }
    min_0 = get_c1(get_sorted_figate(0));
  }
  if ( get_o_val() == val_0 ) {
    set_c0(this, min_0);
    set_c1(this, sum_1);
  }
  else {
    set_c1(this, min_0);
    set_c0(this, sum_1);
  }

#ifdef DEBUG
  fprintf(stderr, "%s::calc_C = (%d, %d)\n",
	  get_name(), get_c0(this), get_c1(this));
#endif
}

ymuint
SIMPLE_gate_t::calc_O(int pos)
{
  ymuint o_c;
  ymuint i_c;
  if ( get_no_val() == val_0 ) {
    o_c = get_c0(this);
  }
  else {
    o_c = get_c1(this);
  }
  if ( get_nc_val() == val_0 ) {
    i_c = get_c0(get_figate(pos));
  }
  else {
    i_c = get_c1(get_figate(pos));
  }

#ifdef DEBUG
  fprintf(stderr, "%s::calc_O(%d) = %d\n",
	  get_name(), pos, get_ob(this) + o_c - i_c);
#endif

  return get_ob(this) + o_c - i_c;
}

/* XOR_gate_t */
void
XOR_gate_t::calc_C()
{
  if ( ni >= sizeof(ymulong) * 8 ) {
    FATAL("Too many fanins\n");
  }
  ymuint min_0 = MAX_VAL;
  ymuint min_1 = MAX_VAL;
  ymulong num_minterm = 1UL << ni;
  for (ymulong bitpat = 0; bitpat < num_minterm; ++ bitpat) {
    ymulong mask = 1UL;
    int p = 0;
    ymuint sum = 0;
    for (int i = 0; i < ni; ++ i) {
      if ( (bitpat & mask) == 0 ) {
	sum += get_c0(get_figate(i));
      }
      else {
	sum += get_c1(get_figate(i));
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
  if ( get_gtype() == kTgXor ) {
    set_c0(this, min_0);
    set_c1(this, min_1);
  }
  else {
    set_c1(this, min_0);
    set_c0(this, min_1);
  }

#ifdef DEBUG
  fprintf(stderr, "%s::calc_C = (%d, %d)\n",
	  get_name(), get_c0(this), get_c1(this));
#endif
}

ymuint
XOR_gate_t::calc_O(int pos)
{
  if ( ni >= sizeof(ymulong) * 8 ) {
    FATAL("Too many fanins\n");
  }
  ymuint min_ob = MAX_VAL;
  ymulong num_minterm = 1UL << (ni - 1);
  for (ymulong bitpat = 0; bitpat < num_minterm; bitpat ++) {
    ymulong mask = 1UL;
    ymuint sum = 0;
    for (int i = 0; i < ni; ++ i) {
      if ( i == pos ) {
	continue;
      }
      if ( (bitpat & mask) == 0 ) {
	sum += get_c0(get_figate(i));
      }
      else {
	sum += get_c1(get_figate(i));
      }
      mask <<= 1;
    }
    if ( min_ob > sum ) {
      min_ob = sum;
    }
  }
  
#ifdef DEBUG
  fprintf(stderr, "%s::calc_O(%d) = %d\n",
	  get_name(), pos, min_ob + get_ob(this));
#endif

  return min_ob + get_ob(this);
}

END_NAMESPACE_YM_MATPG
