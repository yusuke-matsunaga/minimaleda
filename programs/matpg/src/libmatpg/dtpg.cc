/*
 *	matpg - Mat's Automatic Testpattern Generator
 *	10/5/91 by Mat.
 *	$Log: dtpg.c,v $
 * Revision 2.5  1992/01/22  23:49:14  yusuke
 * no difference
 *
 * Revision 2.5  1992/01/22  23:49:14  yusuke
 * no difference
 *
 * Revision 2.4  1992/01/11  08:57:20  yusuke
 * bug fixed on FSIM_NONE mode
 *
 * Revision 2.3  1991/12/28  16:58:41  yusuke
 * Final , Final revision
 *
 * Revision 2.2  91/12/26  19:56:59  yusuke
 * Final revision of version 2
 * 
 * Revision 2.1  91/12/23  23:11:02  yusuke
 * a slightly speed-up
 * 
 * Revision 2.0  91/12/21  18:50:09  yusuke
 * '91 Cristmas version
 * 
 * Revision 1.7  1991/10/05  08:18:18  yusuke
 * add Log and RCSid for RCS
 *
 */

#include "matpg.h"
#include "network.h"
#include "sa_node.h"
#include "fault.h"
#include "testpat.h"
#include "search_mgr.h"
#include "backtrace.h"
#include "maqueue.h"
#include "learn.h"
#include "pgraph.h"
#include "f_sim.h"
#include "sense.h"
#include "cfna.h"
#include <ym_utils/RandGen.h>


BEGIN_NAMESPACE_YM_MATPG

extern
bool debug_flag;

static
FSIM_MODE fsim_mode;

static
DYN_MODE dyn_mode;

static
ymuint dtpg_num;

static
ymuint total_btnum;

int us_mode;

static
int p_num;

static
size_t f_num = 0;

static
testpat_t* tps[pckval_bitlen];

static
testpat_t* cur_tp;

static
fault_t* flts[pckval_bitlen];

static
vector<gate_t*> ex_cands;

static
int
num_ex_cands = 0;

static
int cur_pos;

static
vector<fault_t*> cur_faults;

static
size_t cf_num;

static
void
dtpg_s();

static
void
dtpg_p();

static
bool
dtpg1(fault_t* f,
      int strategy);

static
void
remove_fault(fault_t* f);

static
void
inject_fault(fault_t* f);

static
sa_node*
new_fault(val3 fval);

static
void
set_ex_cands(gate_t* gate);

static
void
examine_dyn_imp();

static
void
drop_fault(fault_t* f0);


void
dtpg(FSIM_MODE f_mode,
     DYN_MODE dyn,
     bool scope)
{
  if ( fault_u_num() == 0 ) {
    return;
  }

  int old_tm_id = sw_timer.change(TM_DTPG);
  
  StopWatch timer;
  timer.start();
  
  total_btnum = 0;
  int d_num0 = fault_d_num();
  int r_num0 = fault_r_num();
  p_num = 0;
  
  fsim_mode = f_mode;
  
  dyn_mode = dyn;
  
  for (size_t i = 0; i < pckval_bitlen; ++ i) {
    tps[i] = new testpat_t(gn_get_npi());
  }

  dtpg_num = 0;
  f_num = 0;
  if ( scope == false ) {
    gn_activate_all();
    cf_num = 0;
    cur_faults.clear();
    cur_faults.resize(fault_u_num());
    for (int i = 0; i < fault_u_num(); ++ i) {
      cur_faults[cf_num ++] = fault_get(i);
    }
    if ( fsim_mode != FSIM_PARALLEL ) {
      dtpg_s();
    }
    else {
      dtpg_p();
    }
    gn_deactivate();
  }
  else {
    for ( ; ; ) {
      fault_t* f0 = NULL;
      size_t i;
      for (i = 0; i < fault_u_num(); ++ i) {
	fault_t* f = fault_get(i);
	if ( f->is_undetected() ) {
	  f0 = f;
	  break;
	}
      }
      if ( f0 == NULL ) {
	// 検査すべき故障は残っていない．
	break;
      }

      bool stat = gn_activate_1fault(f0->get_input_gate());
      if ( stat ) {
	cf_num = 0;
	cur_faults.clear();
	cur_faults.resize(fault_u_num());
	for ( ; i < fault_u_num(); ++ i) {
	  fault_t* f = fault_get(i);
	  if ( f->is_undetected() && f->chk_scope() ) {
	    cur_faults[cf_num ++] = f;
	  }
	}
	if ( fsim_mode != FSIM_PARALLEL ) {
	  dtpg_s();
	}
	else {
	  dtpg_p();
	}
      }
      else {
	f0->set_redundant();
      }
      gn_deactivate();
    }
  }
  
  if ( f_num > 0 ) {
    vector<list<fault_t*> > det_faults(f_num);
    fault_sim(tps, f_num, det_faults);
    for (size_t i = 0; i < f_num; ++ i) {
      testpat_t* tp = tps[i];
      for (list<fault_t*>::iterator p = det_faults[i].begin();
	   p != det_faults[i].end(); ++ p) {
	fault_t* f = *p;
	f->set_detected(tp);
      }
      tp_add(tp);
      ++ p_num;
      tps[i] = new testpat_t(gn_get_npi());
    }
  }
  
  fault_sweep();
  
  for (size_t i = 0; i < pckval_bitlen; ++ i) {
    delete tps[i];
  }
  
  if ( log_flag ) {
    int d_num = fault_d_num() - d_num0;
    int r_num = fault_r_num() - r_num0;
    int a_num = fault_u_num();
    timer.stop();
    USTime time = timer.time();
    double utime = time.usr_time();
    double stime = time.sys_time();
    fprintf(log_fp, "********* dtpg *********\n");
    fprintf(log_fp, "%10d: # of detected faults\n", d_num);
    fprintf(log_fp, "%10d: # of redundant faults\n", r_num);
    fprintf(log_fp, "%10d: # of aborted faults\n", a_num);
    fprintf(log_fp, "%10d: # of generated patterns\n", p_num);
    fprintf(log_fp, "%10d: # of total runs\n", dtpg_num);
    fprintf(log_fp, "%10d: # of total backtracks\n", total_btnum);
    fprintf(log_fp, "%10.2fu %10.2fs: CPU time\n", utime, stime);
  }
  
  sw_timer.change(old_tm_id);
}

void
dtpg_s()
{
  for (size_t pos = 0; pos < cf_num; ++ pos) {
    fault_t* f = cur_faults[pos];
    
    if ( !f->is_undetected() ) {
      // すでに検出済みになっていた．
      continue;
    }

    ++ dtpg_num;
    cur_tp = tps[0];
    if ( !dtpg1(f, 0) ) {
      // 検出失敗
      continue;
    }
    if ( fsim_mode == FSIM_SINGLE ) {
      vector<list<fault_t*> > det_faults(1);
      fault_sim(tps, 1, det_faults);
      testpat_t* tp = tps[0];
      for (list<fault_t*>::iterator p = det_faults[0].begin();
	   p != det_faults[0].end(); ++ p) {
	fault_t* f = *p;
	f->set_detected(tp);
      }
    }
    else {
      f->set_detected(tps[0]);
    }
    tp_add(tps[0]);
    tps[0] = new testpat_t(gn_get_npi());
    ++ p_num;
  }
}

void
dtpg_p()
{
  static RandGen randgen;

  // この関数内で順番を変えるので作業用のコピーを作る．
  vector<fault_t*> tmp_faults(cf_num);
  for (size_t i = 0; i < cf_num; ++ i) {
    tmp_faults[i] = cur_faults[i];
  }
  size_t tmp_num = cf_num;
  
  while ( tmp_num > 0 ) {
    // ランダムに故障を抜き出す．
    size_t pos = size_t(randgen.int32() % tmp_num);
    fault_t* f = tmp_faults[pos];
    -- tmp_num;
    if ( pos < tmp_num ) {
      tmp_faults[pos] = tmp_faults[tmp_num];
    }

    if ( !f->is_undetected() ) {
      continue;
    }
    
    ++ dtpg_num;
    cur_tp = tps[f_num];
    if ( !dtpg1(f, 0) ) {
      continue;
    }

    flts[f_num] = f;

    ++ f_num;
    if ( f_num == pckval_bitlen ) {
      vector<list<fault_t*> > det_faults(pckval_bitlen);
      fault_sim(tps, pckval_bitlen, det_faults);
      for (size_t i = 0; i < pckval_bitlen; ++ i) {
	testpat_t* tp = tps[i];
	for (list<fault_t*>::iterator p = det_faults[i].begin();
	     p != det_faults[i].end(); ++ p) {
	  fault_t* f = *p;
	  f->set_detected(tp);
	}
	tp_add(tps[i]);
	++ p_num;
	tps[i] = new testpat_t(gn_get_npi());
      }
      f_num = 0;
    }
  }
}

static
bool
dtpg1(fault_t* f,
      int strategy)
{
  if ( get_verbose_level() > 0 ) {
    fprintf(ver_fp, "Target fault: ");
    f->print(ver_fp);
  }
  
  gate_t* i_gate = f->get_input_gate();
  
  bool dyn_flag = false;
  if ( dyn_mode != DYN_NONE ) {
    dyn_flag = true;
    set_ex_cands(i_gate);
  }
  
  inject_fault(f);
  maq_clr(true);
  cfna_clr();

  bool retry = false;
  set_request(i_gate, neg3(f->get_fval()), val_X);
  pgraph_construct(i_gate);
  for ( ; ; ) {
    if ( !implication() ) {
      if ( !back_track() ) {
	if ( is_redundant() || retry ) {
	  break;
	}
	one_more_chance();
	strategy ^= 1;
	retry = true;
      }
      continue;
    }
    
    // unique sensitization
    sensitize(i_gate);
    if ( maq_get() ) {
      continue;
    }
    
    // dynamic implication
    if ( dyn_flag ) {
      examine_dyn_imp();
      if ( maq_get() ) {
	continue;
      }
    }
    
    if ( dyn_mode == DYN_SINGLE ) {
      dyn_flag = false;
    }

    if ( strategy == 0 ) {
      if ( !C_decision(i_gate) ) {
	if ( !O_decision(i_gate) ) {
	  // 割り当てる変数がなくなったということは
	  // パタンが見つかったということ
	  cfna_recpat(cur_tp);
	  drop_fault(f);
	  break;
	}
      }
    }
    else {
      if ( !O_decision(i_gate) ) {
	if ( !C_decision(i_gate) ) {
	  // 割り当てる変数がなくなったということは
	  // パタンが見つかったということ
	  cfna_recpat(cur_tp);
	  drop_fault(f);
	  break;
	}
      }
    }
  }
  restore_value();
  pgraph_destruct();
  remove_fault(f);
  total_btnum += get_backtrack_count();
  
  if ( is_redundant() ) {
    f->set_redundant();
    if ( get_verbose_level() > 0 ) {
      fprintf(ver_fp, "... redundant\n\n");
    }
    return false;
  }
  
  if ( is_aborted() ) {
    f->set_aborted();
    if ( get_verbose_level() > 0 ) {
      fprintf(ver_fp, "... aborted\n\n");
    }
    return false;
  }
  
  if ( get_verbose_level() > 0 ) {
    fprintf(ver_fp, "... detected\n\n");
  }
  return true;
}

static
sa_node* avail_sa0 = NULL;
static
sa_node* avail_sa1 = NULL;

static
void
inject_fault(fault_t* f)
{
  if ( get_verbose_level() > 1 ) {
    fputs("FAULT_ASSIGN: ", ver_fp);
    f->print(ver_fp);
  }

  sa_node* f_gate;
  gate_t* gate = f->get_gate();
  int ipos = f->get_ipos();
  if ( ipos == -1 ) {
    if ( gate->is_po() ) {
      f_gate = new_fault(f->get_fval());
      f_gate->insert(gate);
    }
    else {
      for (int i = gate->get_act_no(); -- i >= 0; ) {
	f_gate = new_fault(f->get_fval());
	f_gate->insert(gate->get_act_fogate(i),
		       gate->get_act_fogate_ipos(i));
      }
    }
  }
  else {
    f_gate = new_fault(f->get_fval());
    f_gate->insert(gate, ipos);
  }
}

static
void
remove_fault(fault_t* f)
{
  if ( f->get_ipos() == -1 ) {
    gate_t* gate = f->get_gate();
    for (int i = gate->get_act_no(); -- i >= 0; ) {
      gate_t* gate1 = gate->get_act_fogate(i);
      if ( gate1->is_f_site() ) {
	sa_node* f_gate = dynamic_cast<sa_node*>(gate1);
	assert_cond(f_gate, __FILE__, __LINE__);
	f_gate->remove();
	f_gate->clr_flag();
	f_gate->state = 0L;
	if ( f_gate->is_sa0() ) {
	  f_gate->next = avail_sa0;
	  avail_sa0 = f_gate;
	}
	else {
	  f_gate->next = avail_sa1;
	  avail_sa1 = f_gate;
	}
      }
    }
  }
  else {
    sa_node* f_gate = dynamic_cast<sa_node*>(f->get_input_gate());
    f_gate->remove();
    f_gate->clr_flag();
    f_gate->state = 0L;
    if ( f_gate->is_sa0() ) {
      f_gate->next = avail_sa0;
      avail_sa0 = f_gate;
    }
    else {
      f_gate->next = avail_sa1;
      avail_sa1 = f_gate;
    }
  }
}

static
sa_node*
new_fault(val3 fval)
{
  sa_node* f_gate;
  if ( fval == val_0 ) {
    if ( (f_gate = avail_sa0) ) {
      avail_sa0 = (sa_node*) f_gate->next;
    }
    else {
      f_gate = (sa_node*) new sa0_node;
    }
  }
  else {
    if ( (f_gate = avail_sa1) ) {
      avail_sa1 = (sa_node*) f_gate->next;
    }
    else {
      f_gate = (sa_node*) new sa1_node;
    }
  }
  f_gate->set_active();
  f_gate->set_fcone();
  f_gate->state = 0L;
  
  return f_gate;
}

static
void
mark_tfi(gate_t* gate)
{
  if ( gate->chk_mark() ) {
    return;
  }
  gate->set_mark();
  lvlq_put(gate);
  for (int i = gate->get_ni(); -- i >= 0; ) {
    mark_tfi(gate->get_figate(i));
  }
}

static
void
mark_tfi_tfo(gate_t* gate)
{
  if ( gate->chk_mark() ) {
    return;
  }
  gate->set_mark();
  lvlq_put(gate);
  for (int i = gate->get_act_no(); -- i >= 0; ) {
    mark_tfi_tfo(gate->get_act_fogate(i));
  }
  for (int i = gate->get_ni(); -- i >= 0; ) {
    mark_tfi(gate->get_figate(i));
  }
}

static
void
set_ex_cands(gate_t* gate)
{
  num_ex_cands = 0;
  ex_cands.clear();
  ex_cands.resize(gn_get_ngate());
  lvlq_clr();
  gn_clr_mark();
  mark_tfi_tfo(gate);
  for (gate_t* g; (g = lvlq_get_from_top()); ) {
    if ( g->get_ni() >= 2 ) {
      ex_cands[num_ex_cands ++] = g;
    }
  }
  cur_pos = 0;
}

static
void
examine_dyn_imp()
{
  int i = cur_pos;
  for ( ; ; ) {
    gate_t* gate = ex_cands[i];
    if ( !gate->chk_fcone() ) {
      if ( gate->get_gval() == val_X ) {
	if ( !dyn_imp(gate, val_0, val_X) ) {
	  maq_add(gate, val_1, val_X);
	  break;
	}
	if ( !dyn_imp(gate, val_1, val_X) ) {
	  maq_add(gate, val_0, val_X);
	  break;
	}
      }
    }
    else if ( !gate->chk_dif() ) {
      if ( gate->get_gval() == val_X || gate->get_fval() == val_X) {
	if ( !dyn_imp(gate, val_0, val_0) ) {
	  maq_add(gate, val_1, val_1);
	  break;
	}
	if ( !dyn_imp(gate, val_1, val_1) ) {
	  maq_add(gate, val_0, val_0);
	  break;
	}
      }
    }
    else {
      if ( gate->get_gval() == val_X ) {
	if ( !dyn_imp(gate, val_0, val_X) ) {
	  maq_add(gate, val_1, val_X);
	  break;
	}
	if ( !dyn_imp(gate, val_1, val_X) ) {
	  maq_add(gate, val_0, val_X);
	  break;
	}
      }
      if ( gate->get_fval() == val_X ) {
	if ( !dyn_imp(gate, val_X, val_0) ) {
	  maq_add(gate, val_X, val_1);
	  break;
	}
	if ( !dyn_imp(gate, val_X, val_1) ) {
	  maq_add(gate, val_X, val_0);
	  break;
	}
      }
    }
    ++ i;
    if ( i == num_ex_cands ) {
      i = 0;
    }
    if ( i == cur_pos ) {
      break;
    }
  }
  cur_pos = i;
}

static
void
drop_fault(fault_t* f0)
{
  gate_t* fos = f0->get_gate()->get_fos();
  size_t i;
  for (i = 0; i < cf_num; ++ i) {
    fault_t* f = cur_faults[i];
    if ( f->get_gate()->get_fos() == fos ) {
      break;
    }
  }
  
  for ( ; i < cf_num; i ++) {
    fault_t* f = cur_faults[i];
    if ( f->get_gate()->get_fos() != fos ) {
      break;
    }
    if ( f == f0 ) {
      continue;
    }
    gate_t* gate = f->get_gate();	
    int ipos = f->get_ipos();
    if ( f->get_fval() == val_0 ) {
      if ( f->get_input_gate()->get_gval() != val_1 ) {
	continue;
      }
    }
    else {
      if ( f->get_input_gate()->get_gval() != val_0 ) {
	continue;
      }
    }
    if ( ipos >= 0 ) {
      if ( !gate->calc_obs2(ipos) ) {
	continue;
      }
    }
    bool reached = false;
    for ( ; ; ) {
      if ( gate->is_fos() ) {
	reached = true;
	break;
      }
      gate_t* ogate = gate->get_fogate(0);
      int ipos = gate->get_fogate_ipos(0);
      if ( !ogate->calc_obs2(ipos) ) {
	break;
      }
      gate = ogate;
    }
    if ( reached ) {
      f->set_detected(NULL);
    }
  }
}

// dummy
bool
gate_t::calc_obs2(int) const
{
  FATAL("calc_obs2: error!\n");
  return false;
}

// BUF_gate_t
bool
BUF_gate_t::calc_obs2(int) const
{
  return true;
}

// AND_gate_t
bool
AND_gate_t::calc_obs2(int ipos) const
{
  int i;
  for (i = ni; -- i > ipos; ) {
    if ( get_figate(i)->get_gval() != val_1 ) {
      return false;
    }
  }
  for ( ; -- i >= 0; ) {
    if ( get_figate(i)->get_gval() != val_1 ) {
      return false;
    }
  }
  return true;
}

// NAND_gate_t
bool
NAND_gate_t::calc_obs2(int ipos) const
{
  int i;
  for (i = ni; -- i > ipos; ) {
    if ( get_figate(i)->get_gval() != val_1 ) {
      return false;
    }
  }
  for ( ; -- i >= 0; ) {
    if ( get_figate(i)->get_gval() != val_1 ) {
      return false;
    }
  }
  return true;
}

// OR_gate_t
bool
OR_gate_t::calc_obs2(int ipos) const
{
  int i;
  for (i = ni; -- i > ipos; ) {
    if ( get_figate(i)->get_gval() != val_0 ) {
      return false;
    }
  }
  for ( ; -- i >= 0; ) {
    if ( get_figate(i)->get_gval() != val_0 ) {
      return false;
    }
  }
  return true;
}

// NOR_gate_t
bool
NOR_gate_t::calc_obs2(int ipos) const
{
  int i;
  for (i = ni; -- i > ipos; ) {
    if ( get_figate(i)->get_gval() != val_0 ) {
      return false;
    }
  }
  for ( ; -- i >= 0; ) {
    if ( get_figate(i)->get_gval() != val_0 ) {
      return false;
    }
  }
  return true;
}

// XOR_gate_t
bool
XOR_gate_t::calc_obs2(int ipos) const
{
  int i;
  for (i = ni; -- i > ipos; ) {
    if ( get_figate(i)->get_gval() == val_X ) {
      return false;
    }
  }
  for ( ; -- i >= 0; ) {
    if ( get_figate(i)->get_gval() == val_X ) {
      return false;
    }
  }
  return true;
}

END_NAMESPACE_YM_MATPG
