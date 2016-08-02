/*
 *	matpg - Mat's Automatic Testpattern Generator
 *	10/5/91 by Mat.
 *	$Log: rtpg.c,v $
 * Revision 2.2  1992/01/22  23:49:14  yusuke
 * no difference
 *
 * Revision 2.2  1992/01/22  23:49:14  yusuke
 * no difference
 *
 * Revision 2.1  91/12/23  23:12:08  yusuke
 * a slightly speed-up
 * 
 * Revision 2.0  91/12/21  18:55:36  yusuke
 * '91 Cristmas version
 * 
 * Revision 1.5  1991/10/05  08:18:18  yusuke
 * add Log and RCSid for RCS
 *
 */

#include "matpg.h"
#include "network.h"
#include "fault.h"
#include "testpat.h"
#include "f_sim.h"
#include <ym_utils/RandGen.h>

#define DEBUG 1


BEGIN_NAMESPACE_YM_MATPG

void
rtpg(size_t min_f,
     size_t max_i,
     size_t max_pat,
     FILE* fp)
{
  static RandGen randgen;
  
  if (fault_u_num() == 0) {
    return;
  }

  int old_tm_id = sw_timer.change(TM_FSIM);

  StopWatch timer;
  timer.start();
  
  size_t pnum = 0;
  size_t num = 0;
  testpat_t* tps[pckval_bitlen];

  for (size_t i = 0; i < pckval_bitlen; ++ i) {
    tps[i] = NULL;
  }

  int i_count = 0;
  bool input_eof = false;
  int total_d_count = 0;
  int e_pat = 0;
  int npi = gn_get_npi();
  for ( ; ; ) {
    if (input_eof == false) {
      if (fp) {
	const size_t BUFSIZE = 1024;
	static char buff[BUFSIZE];
	if (fgets(buff, BUFSIZE, fp) == NULL) {
	  input_eof = true;
	}
	else {
	  if (tps[num] == NULL) {
	    tps[num] = new testpat_t;
	  }
	  tps[num ++]->hex2pat(buff, npi);
	  pnum ++;
	  if (pnum == max_pat) {
	    input_eof = true;
	  }
	}
      }
      else {
	if (tps[num] == NULL) {
	  tps[num] = new testpat_t;
	}
	tps[num ++]->randpat(npi, randgen);
	pnum ++;
	if (pnum == max_pat) {
	  input_eof = true;
	}
      }
      if (num < pckval_bitlen) {
	continue;
      }
    }
    else if (num == 0) {
      break;
    }

    vector<list<fault_t*> > det_faults(num);
    fault_sim(tps, num, det_faults);

    if ( get_verbose_level() > 1 ) {
      for (size_t i = 0; i < num; ++ i) {
	tps[i]->dump(ver_fp);
	fprintf(ver_fp, "  %d\n", det_faults[i].size());
      }
      fprintf(ver_fp, "\n");
    }
    
    size_t d_count = 0;
    for (size_t i = 0; i < num; i ++) {
      testpat_t* tp = tps[i];
      if ( !det_faults[i].empty() ) {
	tp_add(tp);
	e_pat ++;
	d_count += tp->det_count;
	tps[i] = NULL;
	list<fault_t*>& flist = det_faults[i];
	for (list<fault_t*>::iterator p = flist.begin();
	     p != flist.end(); ++ p) {
	  fault_t* f = *p;
	  f->set_detected(tp);
	}
      }
    }
    for (size_t i = num; i < pckval_bitlen; i ++) {
      tps[i] = NULL;
    }
    total_d_count += d_count;
    fault_sweep();
    if (fault_u_num() == 0) {
      break;
    }
    if (d_count < min_f) {
      break;
    }
    if (d_count == 0) {
      i_count ++;
      if (max_i != -1 && i_count > max_i) {
	break;
      }
    }
    else {
      i_count = 0;
    }
    num = 0;

    if ( pnum % 1000 == 0 ) {
      fprintf(log_fp, "********* rtpg(%d %d %d) *********\n",
	      min_f, max_i, max_pat);
      fprintf(log_fp, "%10d: # of simulated patterns\n", pnum);
      fprintf(log_fp, "%10d: # of effective patterns\n", e_pat);
      fprintf(log_fp, "%10d: # of detected faults\n", total_d_count);
    }
  }
  for (size_t i = 0; i < pckval_bitlen; i ++) {
    delete tps[i];
  }
  
  if (log_flag == true) {
    timer.stop();
    USTime time = timer.time();
    double utime = time.usr_time();
    double stime = time.sys_time();
    fprintf(log_fp, "********* rtpg(%d %d %d) *********\n",
	    min_f, max_i, max_pat);
    fprintf(log_fp, "%10d: # of simulated patterns\n", pnum);
    fprintf(log_fp, "%10d: # of effective patterns\n", e_pat);
    fprintf(log_fp, "%10d: # of detected faults\n", total_d_count);
    fprintf(log_fp, "%10.2fu %10.2fs: CPU time\n", utime, stime);
  }
  
  sw_timer.change(old_tm_id);
}

/* test pattern reduction using reverse order fault simulation */
void
rfsim()
{

  int old_tm_id = sw_timer.change(TM_FSIM);

  StopWatch timer;
  timer.start();

  fault_retryD();
  testpat_t* tps[pckval_bitlen];
  size_t num = 0;
  int pnum = 0;
  tp_seek_tail();
  for ( ; ; ) {
    testpat_t* tp = tp_get();
    if ( tp ) {
      tp->det_count = 0;
      tps[num ++] = tp;
      pnum ++;
      tp_seek_prev();
      if (num < pckval_bitlen) {
	continue;
      }
    }
    else if (num == 0) {
      break;
    }
    vector<list<fault_t*> > det_faults(num);
    fault_sim(tps, num, det_faults);
    for (size_t i = 0; i < num; ++ i) {
      testpat_t* tp = tps[i];
      for (list<fault_t*>::iterator p = det_faults[i].begin();
	   p != det_faults[i].end(); ++ p) {
	fault_t* f = *p;
	f->set_detected(tp);
      }
    }
    fault_sweep();
    if (fault_u_num() == 0) {
      break;
    }
    num = 0;
  }
  
  int e_num = tp_sweep();
  
  if (log_flag == true) {
    timer.stop();
    USTime time = timer.time();
    double utime = time.usr_time();
    double stime = time.sys_time();
    fprintf(log_fp, "********* rfsim *********\n");
    fprintf(log_fp, "%10d: # of simulated patterns\n", pnum);
    fprintf(log_fp, "%10d: # of effective patterns\n", e_num);
    fprintf(log_fp, "%10.2fu %10.2fs: CPU time\n", utime, stime);
  }
  
  sw_timer.change(old_tm_id);
}

END_NAMESPACE_YM_MATPG
