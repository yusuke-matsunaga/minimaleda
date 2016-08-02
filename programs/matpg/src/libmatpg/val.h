#ifndef H_MATPG_VAL
#define H_MATPG_VAL

/*
 *	matpg - Mat's Automatic Testpattern Generator
 *	10/5/91 by Mat.
 *	$Log: val.h,v $
 * Revision 2.1  1992/01/22  23:49:14  yusuke
 * no difference
 *
 * Revision 2.1  1992/01/22  23:49:14  yusuke
 * no difference
 *
 * Revision 2.0  91/12/21  18:59:18  yusuke
 * '91 Cristmas version
 * 
 * Revision 1.5  1991/10/05  08:18:18  yusuke
 * add Log and RCSid for RCS
 *
 */

#include "matpg.h"

BEGIN_NAMESPACE_YM_MATPG

/* Values */
typedef ymulong pckval;		/* packed value for parallel simulation */

enum val3 {
  val_X = 0,		/* undefined */
  val_0 = 1,		/* '0' */
  val_1 = 2		/* '1' */
};

inline
val3
neg3(val3 val)
{
  ymuint tmp = static_cast<ymuint>(val);
  // 0 ビットめと 1 ビットめを交換する．
  return static_cast<val3>(((tmp << 1) & 2U) | ((tmp >> 1) & 1U));
}

inline
bool
is_different(val3 val1,
	     val3 val2)
{
  // 01 と 10 (もしくは逆)の時にしか 11 にならない
  return (static_cast<ymuint>(val1) | static_cast<ymuint>(val2)) == 3U;
}

const ymuint pckval_bitlen = sizeof(pckval) * 8;
const ymulong pckval_msb = (1UL << (pckval_bitlen - 1));
const ymulong pckval_ALL0 = 0UL;
const ymulong pckval_ALL1 = ~0UL;

/* 2-bit coded value */
/* value   v0 v1     */
/*     0    1  0     */
/*     1    0  1     */
/*     X    0  0     */
/*     Z    1  1     */
struct code2val
{
  pckval* v0;
  pckval* v1;
};

/* for debugging */
inline
char
value_name1(val3 val)
{
  if (val == val_X) {
    return 'X';
  }
  if (val == val_0) {
    return '0';
  }
  if (val == val_1) {
    return '1';
  }
  return '-';
}

inline
const char*
value_name(val3 gval,
	   val3 fval)
{
  if (gval == val_X) {
    if (fval == val_X)      return "X/X";
    else if (fval == val_0) return "X/0";
    else if (fval == val_1) return "X/1";
  }
  else if (gval == val_0) {
    if (fval == val_X)      return "0/X";
    else if (fval == val_0) return "0/0";
    else if (fval == val_1) return "0/1";
  }
  else if (gval == val_1) {
    if (fval == val_X)      return "1/X";
    else if (fval == val_0) return "1/0";
    else if (fval == val_1) return "1/1";
  }
  return "illegal data";
}

END_NAMESPACE_YM_MATPG

#endif
