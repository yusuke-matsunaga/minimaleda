/*
 *	matpg - Mat's Automatic Testpattern Generator
 *	10/5/91 by Mat.
 *	$Log: gate.c,v $
 * Revision 2.1  1992/01/22  23:49:14  yusuke
 * no difference
 *
 * Revision 2.1  1992/01/22  23:49:14  yusuke
 * no difference
 *
 * Revision 2.0  91/12/21  18:51:04  yusuke
 * '91 Cristmas version
 * 
 * Revision 1.5  1991/10/05  08:18:18  yusuke
 * add Log and RCSid for RCS
 *
 */

#include "gate.h"
#include "fault.h"
#include <ym_tgnet/TgNode.h>


BEGIN_NAMESPACE_YM_MATPG


gate_t*
new_gate_t(tTgGateType id,
	   const TgNode* node,
	   int ni)
{
  switch (id) {
  case kTgConst0:   return new C0_gate_t(node);
  case kTgConst1:   return new C1_gate_t(node);
  case kTgBuff:  return new BUF_gate_t(node);
  case kTgNot:  return new NOT_gate_t(node);
  case kTgAnd:
    switch (ni) {
    case 2:		return new AND_2_gate_t(node);
    case 3:		return new AND_3_gate_t(node);
    case 4:		return new AND_4_gate_t(node);
    }
    return new AND_gate_t(node, ni);
    
  case kTgNand:
    switch (ni) {
    case 2:		return new NAND_2_gate_t(node);
    case 3:		return new NAND_3_gate_t(node);
    case 4:		return new NAND_4_gate_t(node);
    }
    return new NAND_gate_t(node, ni);
    
  case kTgOr:
    switch (ni) {
    case 2:		return new OR_2_gate_t(node);
    case 3:		return new OR_3_gate_t(node);
    case 4:		return new OR_4_gate_t(node);
    }
    return new OR_gate_t(node, ni);
    
  case kTgNor:
    switch (ni) {
    case 2:		return new NOR_2_gate_t(node);
    case 3:		return new NOR_3_gate_t(node);
    case 4:		return new NOR_4_gate_t(node);
    }
    return new NOR_gate_t(node, ni);
    
  case kTgXor:
    if (ni == 2) return new XOR_2_gate_t(node);
    return new XOR_gate_t(node, ni);
    
  case kTgXnor:
    if (ni == 2) return new XNOR_2_gate_t(node);
    return new XNOR_gate_t(node, ni);
    
  default:
    break;
  }
  assert_not_reached(__FILE__, __LINE__);
  return NULL;
}
  
gate_t::gate_t(const TgNode* node0,
	       int ni0)
{
  node = node0;
  gen_flag = 0;
  gen_count = 0;
  ni = ni0;
  if ( ni ) {
    igate = new gate_t*[ni];
    sorted_pos = new int[ni];
    for (size_t i = 0; i < ni; ++ i) {
      sorted_pos[i] = i;
    }
  }
  else {
    igate = NULL;
    sorted_pos = NULL;
  }
  no = 0;
  outputs = NULL;
  act_no = 0;
  act_outputs = NULL;
  lvl_i = -1;
  imp0 = NULL;
  imp1 = NULL;
  eq_gate = this;
}

gate_t::~gate_t()
{
  delete [] igate;
  delete [] sorted_pos;
  delete [] outputs;
  delete [] act_outputs;
}

void
gate_t::init_figate(int pos,
		    gate_t* input)
{
  set_figate(pos, input);
  ++ input->no;
}

void
gate_t::init_fogate(gate_t* output,
		    int pos)
{
  if (outputs == NULL) {
    outputs = new net_t[no];
    for (size_t i = 0; i < no; ++ i) {
      outputs[i].gate = NULL;
    }
    act_outputs = new net_t[no + 1];
  }
  if (output != NULL) {
    int i;
    for (i = 0; i < no; i ++) {
      if (get_fogate(i) == NULL) {
	set_fogate(i, output, pos);
	break;
      }
    }
    if (i >= no) {
      fputs("init_fogate: ERROR!\n", stderr);
      exit(1);
    }
  }
}

val3
gate_t::get_c_val() const
{
  return val_X;
}

val3
gate_t::get_nc_val() const
{
  return val_X;
}

val3
gate_t::get_o_val() const
{
  return val_X;
}

val3
gate_t::get_no_val() const
{
  return val_X;
}

bool
gate_t::is_pi() const
{
  return false;
}

int
gate_t::get_pi_id() const
{
  return -1;
}
  
bool
gate_t::is_f_site() const
{
  return false;
}

bool
gate_t::is_sa0() const
{
  return false;
}

bool
gate_t::is_sa1() const
{
  return false;
}

const char*
gate_t::get_name() const
{
  return node->name();
}

tTgGateType
gate_t::get_gtype() const
{
  return node->type();
}

void
gate_t::dump(FILE*) const
{
}


/* PI_gate_t */
bool
PI_gate_t::is_pi() const
{
  return true;
}

int
PI_gate_t::get_pi_id() const
{
  return pi_id;
}

void
PI_gate_t::dump(FILE* fp) const
{
  fputs(get_name(), fp);
  fputs(" = PI\n", fp);
}

/* C0_gate_t */
tTgGateType
C0_gate_t::get_gtype() const
{
  return kTgConst0;
}

void
C0_gate_t::dump(FILE* fp) const
{
  fputs(get_name(), fp);
  fputs(" = C0\n", fp);
}

/* C1_gate_t */
tTgGateType
C1_gate_t::get_gtype() const
{
  return kTgConst1;
}

void
C1_gate_t::dump(FILE* fp) const
{
  fputs(get_name(), fp);
  fputs(" = C0\n", fp);
}

/* BUF_gate_t */
tTgGateType
BUF_gate_t::get_gtype() const
{
  return kTgBuff;
}

void
BUF_gate_t::dump(FILE* fp) const
{
  fputs(get_name(), fp);
  fputs(" = BUF(", fp);
  fputs(get_figate(0)->get_name(), fp);
  fputs(")\n", fp);
}

/* NOT_gate_t */
tTgGateType
NOT_gate_t::get_gtype() const
{
  return kTgNot;
}

void
NOT_gate_t::dump(FILE* fp) const
{
  fputs(get_name(), fp);
  fputs(" = NOT(", fp);
  fputs(get_figate(0)->get_name(), fp);
  fputs(")\n", fp);
}

val3
SIMPLE_gate_t::get_c_val() const
{
  return c_val;
}

val3
SIMPLE_gate_t::get_nc_val() const
{
  return neg3(c_val);
}

val3
SIMPLE_gate_t::get_o_val() const
{
  return o_val;
}

val3
SIMPLE_gate_t::get_no_val() const
{
  return neg3(o_val);
}

/* AND_gate_t */
tTgGateType
AND_gate_t::get_gtype() const
{
  return kTgAnd;
}

void
AND_gate_t::dump(FILE* fp) const
{
  fputs(get_name(), fp);
  fputs(" = AND(", fp);
  for ( int i = 0; i < ni; i ++) {
    if (i > 0) fputs(", ", fp);
    if (get_figate(i)->get_name()) {
      fputs(get_figate(i)->get_name(), fp);
    }
    else {
      get_figate(i)->dump(fp);
    }
  }
  fputs(")\n", fp);
}

/* NAND_gate_t */
tTgGateType
NAND_gate_t::get_gtype() const
{
  return kTgNand;
}

void
NAND_gate_t::dump(FILE* fp) const
{
  fputs(get_name(), fp);
  fputs(" = NAND(", fp);
  for ( int i = 0; i < ni; i ++) {
    if (i > 0) fputs(", ", fp);
    if (get_figate(i)->get_name()) {
      fputs(get_figate(i)->get_name(), fp);
    }
    else {
      get_figate(i)->dump(fp);
    }
  }
  fputs(")\n", fp);
}

/* OR_gate_t */
tTgGateType
OR_gate_t::get_gtype() const
{
  return kTgOr;
}

void
OR_gate_t::dump(FILE* fp) const
{
  fputs(get_name(), fp);
  fputs(" = OR(", fp);
  for ( int i = 0; i < ni; i ++) {
    if (i > 0) fputs(", ", fp);
    if (get_figate(i)->get_name()) {
      fputs(get_figate(i)->get_name(), fp);
    }
    else {
      get_figate(i)->dump(fp);
    }
  }
  fputs(")\n", fp);
}

/* NOR_gate_t */
tTgGateType
NOR_gate_t::get_gtype() const
{
  return kTgNor;
}

void
NOR_gate_t::dump(FILE* fp) const
{
  fputs(get_name(), fp);
  fputs(" = NOR(", fp);
  for ( int i = 0; i < ni; i ++) {
    if (i > 0) fputs(", ", fp);
    if (get_figate(i)->get_name()) {
      fputs(get_figate(i)->get_name(), fp);
    }
    else {
      get_figate(i)->dump(fp);
    }
  }
  fputs(")\n", fp);
}

/* XOR_gate_t */
tTgGateType
XOR_gate_t::get_gtype() const
{
  return kTgXor;
}

void
XOR_gate_t::dump(FILE* fp) const
{
  fputs(get_name(), fp);
  fputs(" = XOR(", fp);
  for ( int i = 0; i < ni; i ++) {
    if (i > 0) fputs(", ", fp);
    if (get_figate(i)->get_name()) {
      fputs(get_figate(i)->get_name(), fp);
    }
    else {
      get_figate(i)->dump(fp);
    }
  }
  fputs(")\n", fp);
}

/* XNOR_gate_t */
tTgGateType
XNOR_gate_t::get_gtype() const
{
  return kTgXnor;
}

void
XNOR_gate_t::dump(FILE* fp) const
{
  fputs(get_name(), fp);
  fputs(" = XNOR(", fp);
  for ( int i = 0; i < ni; i ++) {
    if (i > 0) fputs(", ", fp);
    if (get_figate(i)->get_name()) {
      fputs(get_figate(i)->get_name(), fp);
    }
    else {
      get_figate(i)->dump(fp);
    }
  }
  fputs(")\n", fp);
}


END_NAMESPACE_YM_MATPG
