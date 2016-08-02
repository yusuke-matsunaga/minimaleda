/*
 *	matpg - Mat's Automatic Testpattern Generator
 *	10/5/91 by Mat.
 *	$Log: fault.c,v $
 * Revision 2.1  1992/01/22  23:49:14  yusuke
 * no difference
 *
 * Revision 2.1  1992/01/22  23:49:14  yusuke
 * no difference
 *
 * Revision 2.0  91/12/21  18:50:46  yusuke
 * '91 Cristmas version
 * 
 * Revision 1.5  1991/10/05  08:18:18  yusuke
 * add Log and RCSid for RCS
 *
 */

#include "gate.h"
#include "network.h"
#include "fault.h"
#include <ym_tgnet/TgNode.h>


BEGIN_NAMESPACE_YM_MATPG

vector<fault_t*> fault_whole;
vector<fault_t*> fault_undetected;
vector<fault_t*> fault_detected;
vector<fault_t*> fault_redundant;
int fault_total_num = 0;
int fault_undet_num = 0;
int fault_det_num = 0;
int fault_red_num = 0;

void
fault_clr()
{
  fault_total_num = 0;
  fault_undet_num = 0;
  fault_det_num = 0;
  fault_red_num = 0;
  fault_whole.clear();
  fault_undetected.clear();
  fault_detected.clear();
  fault_redundant.clear();
}

void
fault_set_all()
{
  size_t nn = gn_get_ngate();
  size_t npi = gn_get_npi();
  for (size_t i = npi; i < nn; i ++) {
    gate_t* gate = gn_get_gate(i);
    if ( !gate->is_fos() ) {
      continue;
    }
    for (gate_t* gate1 = gate; ; gate1 = gate1->next_eval) {
      if ( gate1->is_po() || gate1->get_no() > 1 ) {
	fault_add(val_0, gate1, -1);
	fault_add(val_1, gate1, -1);
      }
      gate1->set_fi_fault();
      if ( gate1->next_eval == gate ) {
	break;
      }
    }
  }
  for (size_t i = 0; i < npi; ++ i) {
    gate_t* gate = gn_get_gate(i);
    if ( gate->is_po() || gate->get_no() > 1 ) {
      fault_add(val_0, gate, -1);
      fault_add(val_1, gate, -1);
    }
  }
}

void
fault_add(val3 v,
	  gate_t* gate,
	  int ipos)
{
  fault_t* f = new fault_t(v, gate, ipos);
  fault_whole.push_back(f);
  ++ fault_total_num;
  fault_undetected.push_back(f);
  ++ fault_undet_num;
}

void
fault_sweep()
{
  int new_undet_num = 0;
  for (int i = 0; i < fault_undet_num; i ++) {
    fault_t* f = fault_undetected[i];
    switch (f->ftype) {
    case FTYPE_A:
      f->ftype = FTYPE_U;
    case FTYPE_U:
      fault_undetected[new_undet_num ++] = f;
      break;
      
    case FTYPE_D:
      fault_detected.push_back(f);
      ++ fault_det_num;
      break;
      
    case FTYPE_R:
      fault_redundant.push_back(f);
      ++ fault_red_num;
      break;
      
    case FTYPE_W:
      /* never happens */
      break;
    }
  }
  fault_undet_num = new_undet_num;
}

void
fault_retryD()
{
  fault_undet_num = 0;
  fault_det_num = 0;
  for (int i = 0; i < fault_total_num; i ++) {
    fault_t* f = fault_whole[i];
    switch (f->ftype) {
    case FTYPE_A:
    case FTYPE_U:
    case FTYPE_D:
      f->ftype = FTYPE_U;
      fault_undetected[fault_undet_num ++] = f;
      break;
      
    case FTYPE_R:
    case FTYPE_W:
      break;
    }
  }
}

int
fault_u_num()
{
  return fault_undet_num;
}

int
fault_d_num()
{
  return fault_det_num;
}

int
fault_r_num()
{
  return fault_red_num;
}

int
fault_w_num()
{
  return fault_total_num;
}

fault_t*
fault_get(int pos,
	  FTYPE ftype)
{
  switch (ftype) {
  case FTYPE_W:
    return fault_whole[pos];
    
  case FTYPE_U:
    return fault_undetected[pos];
    
  case FTYPE_D:
    return fault_detected[pos];
    
  case FTYPE_R:
    return fault_redundant[pos];
    
  case FTYPE_A:
    break;
  }
  return NULL;
}

void
fault_print(FILE* fp,
	    FTYPE ftype)
{
  int c = 0;
  for (int i = 0; i < fault_total_num; i ++) {
    fault_t* f = fault_whole[i];
    if (f->ftype == ftype) {
      c ++;
      f->print(fp);
    }
  }
  fprintf(fp, "Total %d fault(s)\n", c);
}

fault_t::fault_t(val3 v,
		 gate_t* gate0,
		 int ipos0)
{
  gate = gate0;
  ipos = ipos0;
  fval = v;
  det_pat = NULL;
  ftype = FTYPE_U;
}

fault_t::~fault_t()
{
}

void
fault_t::print(FILE* fp,
	       bool nl_sw) const
{
  const TgNode* node = gate->get_node();
  fprintf(fp, "%s:", node->name());
  if (ipos >= 0) {
    fprintf(fp, "I%d:", ipos);
  }
  else {
    fprintf(fp, "O:");
  }
  if ( fval == val_0 ) {
    fputs("SA0", fp);
  }
  else {
    fputs("SA1", fp);
  }
  if (nl_sw == true) {
    putc('\n', fp);
  }
}

/* C0_gate_t */
/* C1_gate_t */
/* BUF_gate_t */
/* NOT_gate_t */
void
gate_t::set_fi_fault()
{
}

/* SIMPLE_gate_t */
void
SIMPLE_gate_t::set_fi_fault()
{
  // AND/NAND/OR/NOR タイプの場合，non-controling value の故障のみが
  // 代表故障となる．
  for (size_t i = 0; i < ni; ++ i) {
    fault_add(get_nc_val(), this, i);
  }
}

/* XOR_gate_t */
void
XOR_gate_t::set_fi_fault()
{
  // XOR/XNOR タイプの場合両方の故障が代表故障となる．
  for (size_t i = 0; i < ni; ++ i) {
    fault_add(val_0, this, i);
    fault_add(val_1, this, i);
  }
}

END_NAMESPACE_YM_MATPG
