/*
 *	matpg - Mat's Automatic Testpattern Generator
 *	10/5/91 by Mat.
 *	$Log: testpat.c,v $
 * Revision 2.3  1992/01/22  23:49:14  yusuke
 * no difference
 *
 * Revision 2.3  1992/01/22  23:49:14  yusuke
 * no difference
 *
 * Revision 2.2  91/12/28  16:59:27  yusuke
 * Final , Final revision
 * 
 * Revision 2.1  91/12/23  23:12:29  yusuke
 * a slightly speed-up
 * 
 * Revision 2.0  91/12/21  18:58:29  yusuke
 * '91 Cristmas version
 * 
 * Revision 1.5  1991/10/05  08:18:18  yusuke
 * add Log and RCSid for RCS
 *
 */

#include "matpg.h"
#include "testpat.h"
#include <ym_utils/RandGen.h>


BEGIN_NAMESPACE_YM_MATPG

static
testpat_t* top = NULL;

static
testpat_t* tail = NULL;

static
testpat_t* cur = NULL;

static
int num = 0;

void
tp_clr()
{
  delete top;
  top = NULL;
  tail = NULL;
  cur = NULL;
  num = 0;
}

void
tp_add(testpat_t* tp)
{
  tp->next = NULL;
  tp->prev = NULL;
  if (top) {
    tp->prev = tail;
    tail->next = tp;
    tail = tp;
  }
  else {
    top = tail = tp;
  }
  num ++;
}

int
tp_sweep()
{
  testpat_t* nx = top;
  testpat_t* tp;
  while ( (tp = nx) ) {
    nx = tp->next;
    if (tp->det_count > 0) {
      continue;
    }
    if (tp->prev) {
      if (tp->next) {
	tp->next->prev = tp->prev;
      }
      else {
	tail = tp->prev;
      }
      tp->prev->next = tp->next;
    }
    else {
      if (tp->next) {
	tp->next->prev = NULL;
	top = tp->next;
      }
      else {
	top = tail = NULL;
      }
    }
    tp->next = tp->prev = NULL;
    delete tp;
    num --;
  }
  return num;
}

testpat_t*
tp_get()
{
  return cur;
}

void
tp_seek_top()
{
  cur = top;
}

void
tp_seek_tail()
{
  cur = tail;
}

void
tp_seek_next()
{
  if (cur) cur = cur->next;
}

void
tp_seek_prev()
{
  if (cur) cur = cur->prev;
}

int
tp_get_num()
{
  return num;
}

void
tp_print(FILE* fp,
	 bool num_flag,
	 bool hex_flag)
{
  testpat_t* tp;
  int i = 0;
  for (tp_seek_top(); (tp = tp_get()) ; tp_seek_next()) {
    if (num_flag == true) {
      fprintf(fp, "%05d: ", ++ i);
    }
    if (hex_flag == false) {
      tp->dump(fp);
    }
    else {
      tp->hexdump(fp);
    }
  }
}

void
testpat_t::change_size(int new_ni)
{
  ni = new_ni;
  int new_size = (ni + pckval_bitlen - 1) / pckval_bitlen;
  if (pat_size != new_size) {
    delete [] pat;
    pat = new pckval[pat_size = new_size];
  }
}

void
testpat_t::andor(pckval* and_mask,
		 pckval* or_mask)
{
  pckval* dst = pat;
  pckval* end = dst + pat_size;
  while (dst != end) {
    (*dst) &= ~(*and_mask ++);
    (*dst ++) |= (*or_mask ++);
  }
}

void
testpat_t::hex2pat(const char* hex,
		   int new_ni)
{
  change_size(new_ni);
  const char* ptr = hex;
  det_count = 0;
  int n = pckval_bitlen / 4;
  for ( int i = 0; i < pat_size; i ++) {
    pat[i] = 0;
    for ( int j = 0; j < n; j ++) {
      char c = *ptr ++;
      ymuint val = 0;
      if (c >= '0' && c <= '9') {
	val = (int) c - (int) '0';
      }
      else if (c >= 'a' && c <= 'f') {
	val = (int) c - (int) 'a' + 10;
      }
      else if (c >= 'A' && c <= 'F') {
	val = (int) c - (int) 'A' + 10;
      }
      pat[i] |= val << ((n - j - 1) * 4);
    }
  }
}

testpat_t&
testpat_t::operator=(const testpat_t& src)
{
  change_size(src.ni);
  pckval* s = src.pat;
  pckval* d = pat;
  pckval* e = pat + pat_size;
  while (d != e) {
    *d ++ = *s ++;
  }
  det_count = src.det_count;
  return *this;
}

void
testpat_t::randpat(int new_ni,
		   RandGen& randgen)
{
  change_size(new_ni);
  det_count = 0;
  for ( int i = 0; i < pat_size; i ++) {
    pat[i] = randgen.ulong();
  }
}

void
testpat_t::dump(FILE* fp) const
{
  for (int i = 0; i < ni; i ++) {
    putc((get_val01(i) ? '1' : '0'), fp);
  }
  putc('\n', fp);
}

void
testpat_t::hexdump(FILE* fp) const
{
  ymuint tmp = 0U;
  ymuint bit = 1U;
  for (size_t i = 0; ; ++ i) {
    if ( i < ni ) {
      if ( get_val01(i) ) {
	tmp += bit;
      }
      bit <<= 1;
      if ( bit != 16U ) {
	continue;
      }
    }
    else if ( bit == 1U ) {
      break;
    }
    
    if ( tmp <= 9 ) {
      putc(static_cast<char>('0' + tmp), fp);
    }
    else {
      putc(static_cast<char>('A' + tmp - 10), fp);
    }
    bit = 1U;
    tmp = 0U;
  }
  putc('\n', fp);
}

END_NAMESPACE_YM_MATPG
