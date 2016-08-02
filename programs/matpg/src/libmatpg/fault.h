#ifndef H_MATPG_FAULT
#define H_MATPG_FAULT

/*
 *	matpg - Mat's Automatic Testpattern Generator
 *	10/5/91 by Mat.
 *	$Log: fault.h,v $
 * Revision 2.1  1992/01/22  23:49:14  yusuke
 * no difference
 *
 * Revision 2.1  1992/01/22  23:49:14  yusuke
 * no difference
 *
 * Revision 2.0  91/12/21  18:50:54  yusuke
 * '91 Cristmas version
 * 
 * Revision 1.5  1991/10/05  08:18:18  yusuke
 * add Log and RCSid for RCS
 *
 */

#include "matpg.h"
#include "val.h"
#include "network.h"


BEGIN_NAMESPACE_YM_MATPG

class bn_node_t;
class testpat_t;

enum FTYPE {
  FTYPE_U,
  FTYPE_D,
  FTYPE_R,
  FTYPE_A,
  FTYPE_W
};

/*  <<  fault  >>  */
class fault_t
{
private:
  gate_t* gate;
  int ipos;
  val3 fval;
  testpat_t* det_pat;
  FTYPE ftype;
  
public:
  pckval fpckval;
  
  fault_t(val3 v,
	  gate_t* gate,
	  int ipos);
  
  ~fault_t();
  
  val3
  get_fval() const { return fval; }
  
  void
  print(FILE* fp,
	bool nl_sw = true) const;
  
  FTYPE
  get_ftype() const { return ftype; }
  
  bool
  is_undetected() const { return bool(ftype == FTYPE_U); }
  
  bool
  is_detected() const { return bool(ftype == FTYPE_D); }
  
  bool
  is_redundant() const { return bool(ftype == FTYPE_R); }
  
  bool
  is_aborted() const { return bool(ftype == FTYPE_A); }
  
  void
  set_undetected() { ftype = FTYPE_U; }
  
  void
  set_detected(testpat_t* const pat) { ftype = FTYPE_D; det_pat = pat; }
  
  void
  set_redundant() { ftype = FTYPE_R; }
  
  void
  set_aborted() { ftype = FTYPE_A; }
  
  gate_t*
  get_gate() const { return (gate_t*) gate; }
  
  int
  get_ipos() const { return ipos; }
  
  gate_t*
  get_input_gate() const
  {
    if (ipos == -1) {
      return (gate_t*) gate;
    }
    else {
      return gate->get_figate(ipos);
    }
  }
  
  bool
  is_active() const { return gate->is_active(); }
  
  bool
  chk_scope() const { return gate->chk_scope(); }
  
  friend
  void
  fault_add(val3 v,
	    gate_t* gate,
	    int ipos);
  
  friend
  void
  fault_sweep();
  
  friend
  void
  fault_retryP();
  
  friend
  void
  fault_retryD();
  
  friend
  void
  fault_print(FILE* fp,
	      FTYPE ftype);
  
};

extern
vector<fault_t*> fault_whole;
extern
vector<fault_t*> fault_undetected;
extern
vector<fault_t*> fault_detected;
extern
vector<fault_t*> fault_redundant;
extern int fault_total_num;
extern int fault_undet_num;
extern int fault_det_num;
extern int fault_red_num;

extern
void
fault_clr();

extern
void
fault_set_all();

extern
void
fault_add(val3 v,
	  gate_t* gate,
	  int ipos);

extern
void
fault_sweep();

extern
void
fault_retryP();

extern
void
fault_retryD();

extern
int
fault_u_num();

extern
int
fault_d_num();

extern
int
fault_r_num();

extern
int
fault_w_num();

extern
fault_t*
fault_get(int pos,
	  FTYPE ftype);

inline
fault_t*
fault_get(int pos)
{
  return fault_undetected[pos];
}

extern
void
fault_print(FILE* fp,
	    FTYPE ftype);

END_NAMESPACE_YM_MATPG

#endif
