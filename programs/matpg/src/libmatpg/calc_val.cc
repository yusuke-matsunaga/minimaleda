/*
 *	matpg - Mat's Automatic Testpattern Generator
 *	10/5/91 by Mat.
 *	$Log: calc_val.c,v $
 * Revision 1.2  1992/01/22  23:49:14  yusuke
 * no difference
 *
 * Revision 1.2  1992/01/22  23:49:14  yusuke
 * no difference
 *
 * Revision 1.1  92/01/03  17:39:40  yusuke
 * Initial revision
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


BEGIN_NAMESPACE_YM_MATPG

/* dummy */
void
gate_t::calc_gval()
{
  g_pckval = f_pckval = pckval_ALL0;
}

pckval
gate_t::calc_fval() const
{
  return pckval_ALL0;
}

pckval
gate_t::calc_obs(int) const
{
  FATAL("calc_obs: error!\n");
  return pckval_ALL0;
}

/* PI_gate_t */
pckval
PI_gate_t::calc_fval() const
{
  return g_pckval;
}

/* C0_gate_t */
void
C0_gate_t::calc_gval()
{
  g_pckval = f_pckval = pckval_ALL0;
}

pckval
C0_gate_t::calc_fval() const
{
  return pckval_ALL0;
}

/* C1_gate_t */
void
C1_gate_t::calc_gval()
{
  g_pckval = f_pckval = pckval_ALL1;
}

pckval
C1_gate_t::calc_fval() const
{
  return pckval_ALL1;
}

/* BUF_gate_t */
void
BUF_gate_t::calc_gval()
{
  g_pckval = f_pckval = (*igate)->g_pckval;
}

pckval
BUF_gate_t::calc_fval() const
{
  return (*igate)->f_pckval;
}

pckval
BUF_gate_t::calc_obs(int) const
{
  return pckval_ALL1;
}

/* NOT_gate_t */
void
NOT_gate_t::calc_gval()
{
  g_pckval = f_pckval = (*igate)->g_pckval ^ pckval_ALL1;
}

pckval
NOT_gate_t::calc_fval() const
{
  return (*igate)->f_pckval ^ pckval_ALL1;
}

pckval
NOT_gate_t::calc_obs(int) const
{
  return pckval_ALL1;
}

/* AND_gate_t */
void
AND_gate_t::calc_gval()
{
  gate_t** ptr = igate;
  gate_t** end = igate + ni;
  pckval val = (*ptr)->g_pckval;
  while (++ ptr != end) {
    val &= (*ptr)->g_pckval;
  }
  g_pckval = f_pckval = val;
}

void
AND_2_gate_t::calc_gval()
{
  g_pckval = f_pckval = igate[0]->g_pckval & igate[1]->g_pckval;
}

void
AND_3_gate_t::calc_gval()
{
  g_pckval = f_pckval = igate[0]->g_pckval & igate[1]->g_pckval
    & igate[2]->g_pckval;
}

void
AND_4_gate_t::calc_gval()
{
  g_pckval = f_pckval = igate[0]->g_pckval & igate[1]->g_pckval
    & igate[2]->g_pckval & igate[3]->g_pckval;
}
pckval
AND_gate_t::calc_fval() const
{
  gate_t** ptr = igate;
  gate_t** end = igate + ni;
  pckval val = (*ptr)->f_pckval;
  while (++ ptr != end) {
    val &= (*ptr)->f_pckval;
  }
  return val;
}

pckval
AND_2_gate_t::calc_fval() const
{
  return igate[0]->f_pckval & igate[1]->f_pckval;
}

pckval
AND_3_gate_t::calc_fval() const
{
  return igate[0]->f_pckval & igate[1]->f_pckval & igate[2]->f_pckval;
}

pckval
AND_4_gate_t::calc_fval() const
{
  return igate[0]->f_pckval & igate[1]->f_pckval & igate[2]->f_pckval
    & igate[3]->f_pckval;
}

pckval
AND_gate_t::calc_obs(int ipos) const
{
  pckval obs = pckval_ALL1;
  gate_t** ptr = igate;
  gate_t** end = igate + ipos;
  while (ptr != end) {
    obs &= (*ptr ++)->g_pckval;
  }
  end = igate + ni;
  while (++ ptr != end) {
    obs &= (*ptr)->g_pckval;
  }
  return obs;
}

/* NAND_gate_t */
void
NAND_gate_t::calc_gval()
{
  gate_t** ptr = igate;
  gate_t** end = igate + ni;
  pckval val = (*ptr)->g_pckval;
  while (++ ptr != end) {
    val &= (*ptr)->g_pckval;
  }
  g_pckval = f_pckval = val ^ pckval_ALL1;
}

void
NAND_2_gate_t::calc_gval()
{
  g_pckval = f_pckval = ~ (igate[0]->g_pckval & igate[1]->g_pckval);
}

void
NAND_3_gate_t::calc_gval()
{
  g_pckval = f_pckval = ~ (igate[0]->g_pckval & igate[1]->g_pckval
			   & igate[2]->g_pckval);
}

void
NAND_4_gate_t::calc_gval()
{
  g_pckval = f_pckval = ~ (igate[0]->g_pckval & igate[1]->g_pckval
			   & igate[2]->g_pckval & igate[3]->g_pckval);
}

pckval
NAND_gate_t::calc_fval() const
{
  gate_t** ptr = igate;
  gate_t** end = igate + ni;
  pckval val = (*ptr)->f_pckval;
  while (++ ptr != end) {
    val &= (*ptr)->f_pckval;
  }
  return val ^ pckval_ALL1;
}

pckval
NAND_2_gate_t::calc_fval() const
{
  return ~ (igate[0]->f_pckval & igate[1]->f_pckval);
}

pckval
NAND_3_gate_t::calc_fval() const
{
  return ~ (igate[0]->f_pckval & igate[1]->f_pckval & igate[2]->f_pckval);
}

pckval
NAND_4_gate_t::calc_fval() const
{
  return ~ (igate[0]->f_pckval & igate[1]->f_pckval & igate[2]->f_pckval
	    & igate[3]->f_pckval);
}

pckval
NAND_gate_t::calc_obs(int ipos) const
{
  pckval obs = pckval_ALL1;
  gate_t** ptr = igate;
  gate_t** end = igate + ipos;
  while (ptr != end) {
    obs &= (*ptr ++)->g_pckval;
  }
  end = igate + ni;
  while (++ ptr != end) {
    obs &= (*ptr)->g_pckval;
  }
  return obs;
}

/* OR_gate_t */
void
OR_gate_t::calc_gval()
{
  gate_t** ptr = igate;
  gate_t** end = igate + ni;
  pckval val = (*ptr)->g_pckval;
  while (++ ptr != end) {
    val |= (*ptr)->g_pckval;
  }
  g_pckval = f_pckval = val;
}

void
OR_2_gate_t::calc_gval()
{
  g_pckval = f_pckval = igate[0]->g_pckval | igate[1]->g_pckval;
}

void
OR_3_gate_t::calc_gval()
{
  g_pckval = f_pckval = igate[0]->g_pckval | igate[1]->g_pckval
    | igate[2]->g_pckval;
}

void
OR_4_gate_t::calc_gval()
{
  g_pckval = f_pckval = igate[0]->g_pckval | igate[1]->g_pckval
    | igate[2]->g_pckval | igate[3]->g_pckval;
}

pckval
OR_gate_t::calc_fval() const
{
  gate_t** ptr = igate;
  gate_t** end = igate + ni;
  pckval val = (*ptr)->f_pckval;
  while (++ ptr != end) {
    val |= (*ptr)->f_pckval;
  }
  return val;
}

pckval
OR_2_gate_t::calc_fval() const
{
  return igate[0]->f_pckval | igate[1]->f_pckval;
}

pckval
OR_3_gate_t::calc_fval() const
{
  return igate[0]->f_pckval | igate[1]->f_pckval | igate[2]->f_pckval;
}

pckval
OR_4_gate_t::calc_fval() const
{
  return igate[0]->f_pckval | igate[1]->f_pckval | igate[2]->f_pckval
    | igate[3]->f_pckval;
}

pckval
OR_gate_t::calc_obs(int ipos) const
{
  pckval obs = pckval_ALL0;
  gate_t** ptr = igate;
  gate_t** end = igate + ipos;
  while (ptr != end) {
    obs |= (*ptr ++)->g_pckval;
  }
  end = igate + ni;
  while (++ ptr != end) {
    obs |= (*ptr)->g_pckval;
  }
  return obs ^ pckval_ALL1;
}

/* NOR_gate_t */
void
NOR_gate_t::calc_gval()
{
  gate_t** ptr = igate;
  gate_t** end = igate + ni;
  pckval val = (*ptr)->g_pckval;
  while (++ ptr != end) {
    val |= (*ptr)->g_pckval;
  }
  g_pckval = f_pckval = val ^ pckval_ALL1;
}

void
NOR_2_gate_t::calc_gval()
{
  g_pckval = f_pckval = ~ (igate[0]->g_pckval | igate[1]->g_pckval);
}

void
NOR_3_gate_t::calc_gval()
{
  g_pckval = f_pckval = ~ (igate[0]->g_pckval | igate[1]->g_pckval
			   | igate[2]->g_pckval);
}

void
NOR_4_gate_t::calc_gval()
{
  g_pckval = f_pckval = ~ (igate[0]->g_pckval | igate[1]->g_pckval
			   | igate[2]->g_pckval | igate[3]->g_pckval);
}

pckval
NOR_gate_t::calc_fval() const
{
  gate_t** ptr = igate;
  gate_t** end = igate + ni;
  pckval val = (*ptr)->f_pckval;
  while (++ ptr != end) {
    val |= (*ptr)->f_pckval;
  }
  return val ^ pckval_ALL1;
}

pckval
NOR_2_gate_t::calc_fval() const
{
  return ~ (igate[0]->f_pckval | igate[1]->f_pckval);
}

pckval
NOR_3_gate_t::calc_fval() const
{
  return ~ (igate[0]->f_pckval | igate[1]->f_pckval | igate[2]->f_pckval);
}

pckval
NOR_4_gate_t::calc_fval() const
{
  return ~ (igate[0]->f_pckval | igate[1]->f_pckval | igate[2]->f_pckval
	    | igate[3]->f_pckval);
}

pckval
NOR_gate_t::calc_obs(int ipos) const
{
  pckval obs = pckval_ALL0;
  gate_t** ptr = igate;
  gate_t** end = igate + ipos;
  while (ptr != end) {
    obs |= (*ptr ++)->g_pckval;
  }
  end = igate + ni;
  while (++ ptr != end) {
    obs |= (*ptr)->g_pckval;
  }
  return obs ^ pckval_ALL1;
}

/* XOR_gate_t */
void
XOR_gate_t::calc_gval()
{
  gate_t** ptr = igate;
  gate_t** end = igate + ni;
  pckval val = (*ptr)->g_pckval;
  while (++ ptr != end) {
    val ^= (*ptr)->g_pckval;
  }
  g_pckval = f_pckval = val;
}

void
XOR_2_gate_t::calc_gval()
{
  g_pckval = f_pckval = igate[0]->g_pckval ^ igate[1]->g_pckval;
}

pckval
XOR_gate_t::calc_fval() const
{
  gate_t** ptr = igate;
  gate_t** end = igate + ni;
  pckval val = (*ptr)->f_pckval;
  while (++ ptr != end) {
    val ^= (*ptr)->f_pckval;
  }
  return val;
}

pckval
XOR_2_gate_t::calc_fval() const
{
  return igate[0]->f_pckval ^ igate[1]->f_pckval;
}

pckval
XOR_gate_t::calc_obs(int) const
{
  return pckval_ALL1;
}

/* XNOR_gate_t */
void
XNOR_gate_t::calc_gval()
{
  gate_t** ptr = igate;
  gate_t** end = igate + ni;
  pckval val = (*ptr)->g_pckval;
  while (++ ptr != end) {
    val ^= (*ptr)->g_pckval;
  }
  g_pckval = f_pckval = val ^ pckval_ALL1;
}

void
XNOR_2_gate_t::calc_gval()
{
  g_pckval = f_pckval = ~ (igate[0]->g_pckval ^ igate[1]->g_pckval);
}

pckval
XNOR_gate_t::calc_fval() const
{
  gate_t** ptr = igate;
  gate_t** end = igate + ni;
  pckval val = (*ptr)->f_pckval;
  while (++ ptr != end) {
    val ^= (*ptr)->f_pckval;
  }
  return val ^ pckval_ALL1;
}

pckval
XNOR_2_gate_t::calc_fval() const
{
  return ~ (igate[0]->f_pckval ^ igate[1]->f_pckval);
}

pckval
XNOR_gate_t::calc_obs(int) const
{
  return pckval_ALL1;
}

END_NAMESPACE_YM_MATPG
