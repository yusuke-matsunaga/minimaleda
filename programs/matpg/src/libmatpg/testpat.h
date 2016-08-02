#ifndef H_MATPG_TESTPAT
#define H_MATPG_TESTPAT

/*
 *	matpg - Mat's Automatic Testpattern Generator
 *	10/5/91 by Mat.
 *	$Log: testpat.h,v $
 * Revision 2.2  1992/01/22  23:49:14  yusuke
 * no difference
 *
 * Revision 2.2  1992/01/22  23:49:14  yusuke
 * no difference
 *
 * Revision 2.1  91/12/23  23:12:31  yusuke
 * a slightly speed-up
 * 
 * Revision 2.0  91/12/21  18:58:40  yusuke
 * '91 Cristmas version
 * 
 * Revision 1.5  1991/10/05  08:18:18  yusuke
 * add Log and RCSid for RCS
 *
 */

#include "matpg.h"
#include "val.h"
#include <ym_utils/RandGen.h>


BEGIN_NAMESPACE_YM_MATPG


class testpat_t
{
private:
  testpat_t* next;
  testpat_t* prev;
  
public:
  int pat_size;
  pckval* pat;
  int ni;
  int det_count;
  
  testpat_t(int arg_ni = 0)
  {
    next = NULL;
    prev = NULL;
    pat = NULL;
    pat_size = 0;
    ni = 0;
    det_count = 0;
    change_size(arg_ni);
  }
  
  ~testpat_t()
  {
    delete [] pat;
    delete next;
  }
  
  testpat_t&
  operator=(const testpat_t& src);
  
  void
  change_size(int new_size);
  
  int
  get_idx(int pos) const
  {
    return pos / pckval_bitlen;
  }
  
  int
  get_shift_num(int pos) const
  {
    return (pckval_bitlen - 1 - pos) % pckval_bitlen;
  }
  
  pckval
  get_mask(int pos) const
  {
    return (1 << get_shift_num(pos));
  }
  
  void
  hex2pat(const char* hex,
	  int ni);
  
  void
  randpat(int ni,
	  RandGen& randgen);
  
  void
  andor(pckval* and_mask,
	pckval* or_mask);
  
  int
  get_val01(int pos) const
  {
    return ((pat[get_idx(pos)] >> get_shift_num(pos)) & 1);
  }
  
  void
  set_val(int pos,
	  val3 val)
  {
    if (val == val_0) {
      pat[get_idx(pos)] &= ~get_mask(pos);
    }
    else {	/* val must be val_1 */
      pat[get_idx(pos)] |= get_mask(pos);
    }
  }
  
  void
  dump(FILE* fp) const;
  
  void
  hexdump(FILE* fp) const;
  
  friend
  void
  tp_clr();
  
  friend
  void
  tp_add(testpat_t* tp);
  
  friend
  int
  tp_sweep();
  
  friend
  testpat_t*
  tp_get();
  
  friend
  void
  tp_seek_top();
  
  friend
  void
  tp_seek_tail();
  
  friend
  void
  tp_seek_next();
  
  friend
  void
  tp_seek_prev();
  
  friend
  int
  tp_get_num();
  
  friend
  void
  tp_print(FILE* fp,
	   bool num_flag,
	   bool hex_flag);
  
};

void
tp_clr();

void
tp_add(testpat_t* tp);

int
tp_sweep();

testpat_t*
tp_get();

void
tp_seek_top();

void
tp_seek_tail();

void
tp_seek_next();

void
tp_seek_prev();

int
tp_get_num();

void
tp_print(FILE* fp,
	 bool num_flag,
	 bool hex_flag);

END_NAMESPACE_YM_MATPG

#endif
