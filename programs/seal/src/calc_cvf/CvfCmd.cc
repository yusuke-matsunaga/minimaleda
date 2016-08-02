
/// @file src/calc_cvf/CvfCmd.cc
/// @brief CvfCmd の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: CvfCmd.cc 1978 2009-02-06 12:29:16Z matsunaga $
///
/// Copyright (C) 2005-2008 Yusuke Matsunaga
/// All rights reserved.


#if HAVE_CONFIG_H
#include "seal_config.h"
#endif


#include "seal_utils.h"
#include "CvfCmd.h"
#include <ym_tgnet/TgNetwork.h>
#include "TestVector.h"
#include <ym_tclpp/TclPopt.h>
#include "CalcCvf.h"


BEGIN_NAMESPACE_YM_SEAL_CVF

//////////////////////////////////////////////////////////////////////
// 可観測性を計算するコマンド
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
CvfCmd::CvfCmd(SealMgr* mgr) :
  SealCmd(mgr)
{
  mPoptLoop = new TclPoptUint(this, "loop",
			      "loop count");
  mPoptTrace = new TclPopt(this, "trace",
			   "trace mode");
  mPoptTraceCount = new TclPopt(this, "trace_count",
				"trace count mode");
  mPoptInit = new TclPopt(this, "init",
			 "initialize");
  mPoptDss = new TclPopt(this, "init-dss",
			 "initialize with DSS");
  mPoptOldDss = new TclPopt(this, "init-old-dss",
			    "initialize with old DSS");
  mPoptExact = new TclPopt(this, "exact",
			   "exact method");
  mPoptMin = new TclPopt(this, "pseudo-min",
			 "pseudo minimal method");
  mPoptMax = new TclPopt(this, "max",
			 "maximal method");
  mPoptMaxSample = new TclPoptInt(this, "max_sample",
				  "number of samples in maximal method");
  mPoptDiff = new TclPopt(this, "diff",
			  "calclate diffence");
  mPoptGate = new TclPopt(this, "gate",
			  "display gates' information");
  new_popt_group(mPoptInit, mPoptDss, mPoptOldDss);
  new_popt_group(mPoptExact, mPoptMin, mPoptMax, mPoptDiff);
}

// @brief デストラクタ
CvfCmd::~CvfCmd()
{
}
  
// コマンド処理関数
int
CvfCmd::cmd_proc(TclObjVector& objv)
{
  if ( objv.size() != 1 ) {
    print_usage();
    return TCL_ERROR;
  }
  
  // オプション解析
  ymuint loop_num = 1000;
  if ( mPoptLoop->is_specified() ) {
    loop_num = mPoptLoop->val();
  }
  bool trace = mPoptTrace->is_specified();
  bool trace_count = mPoptTraceCount->is_specified();
  bool init = mPoptInit->is_specified();
  bool dss = mPoptDss->is_specified();
  bool old_dss = mPoptOldDss->is_specified();
  bool gate = mPoptGate->is_specified();
  bool exact = true;
  bool min = false;
  bool max = false;
  int max_sample = 1;
  bool diff = false;
  if ( mPoptExact->is_specified() ) {
    ;
  }
  else if ( mPoptMin->is_specified() ) {
    exact = false;
    min = true;
    max = false;
    diff = false;
  }
  else if ( mPoptMax->is_specified() ) {
    exact = false;
    min = false;
    max = true;
    diff = false;
    if ( mPoptMaxSample->is_specified() ) {
      max_sample = mPoptMaxSample->val();
    }
  }
  else if ( mPoptDiff->is_specified() ) {
    exact = false;
    min = false;
    max = false;
    diff = true;
    trace = false;
    trace_count = false;
  }
  
  if ( init ) {
    mCalc.set_network(_network(), false);
    return TCL_OK;
  }
  else if ( dss ) {
    mCalc.set_network(_network(), true, true);
    return TCL_OK;
  }
  else if ( old_dss ) {
    mCalc.set_network(_network(), true, false);
    return TCL_OK;
  }
  
  RandGen rgen;
  
  ostringstream out;
  const TgNetwork& network = _network();
  
  size_t ni = network.input_num2();
  size_t nl = network.logic_num();
  size_t nn = network.node_num();
  
  vector<TestVector*> tv_array(kPvBitLen, NULL);
  for (size_t i = 0; i < kPvBitLen; ++ i) {
    tv_array[i] = TestVector::new_vector(ni);
  }
  
  double total = 0.0;
  double total2 = 0.0;
  vector<size_t> samples1(nn, 0);
  vector<size_t> samples2(nn, 0);
  for (size_t l = 0; l < loop_num; ++ l) {
    for (size_t i = 0; i < kPvBitLen; ++ i) {
      TestVector* tv = tv_array[i];
      tv->set_from_random(rgen);
    }
    
    if ( exact ) {
      mCalc.calc_exact(tv_array);
    }
    else if ( min ) {
      mCalc.calc_pseudo_min(tv_array);
    }
    else if ( max ) {
      mCalc.calc_max(tv_array, max_sample);
    }
    else if ( diff ) {
      mCalc.calc_exact(tv_array);
      size_t n_total = 0;
      for (size_t i = 0; i < ni; ++ i) {
	const TgNode* node = network.input(i);
	tPackedVal obs = mCalc.get_obs(node);
	size_t n = count_ones(obs);
	samples1[node->gid()] += n;
	n_total += n;
      }
      for (size_t i = 0; i < nl; ++ i) {
	const TgNode* node = network.logic(i);
	tPackedVal obs = mCalc.get_obs(node);
	size_t n = count_ones(obs);
	samples1[node->gid()] += n;
	n_total += n;
      }
      total += static_cast<double>(n_total) / static_cast<double>(kPvBitLen);
      
      mCalc.calc_pseudo_min(tv_array);
      size_t n_total2 = 0;
      for (size_t i = 0; i < ni; ++ i) {
	const TgNode* node = network.input(i);
	tPackedVal obs = mCalc.get_obs(node);
	size_t n = count_ones(obs);
	samples2[node->gid()] += n;
	n_total2 += n;
      }
      for (size_t i = 0; i < nl; ++ i) {
	const TgNode* node = network.logic(i);
	tPackedVal obs = mCalc.get_obs(node);
	size_t n = count_ones(obs);
	samples2[node->gid()] += n;
	n_total2 += n;
      }
      total2 += static_cast<double>(n_total2) / static_cast<double>(kPvBitLen);
    }

    if ( trace ) {
      out << "===============[ " << l << " ]==============" << endl;
      for (size_t i = 0; i < ni; ++ i) {
	const TgNode* node = network.input(i);
	tPackedVal obs = mCalc.get_obs(node);
	if ( node->name() ) {
	  out << setw(20) << std::setfill(' ') << node->name();
	}
	out << ": " << hex << setw(16) << std::setfill('0') << obs << dec << endl;
      }
      for (size_t i = 0; i < nl; ++ i) {
	const TgNode* node = network.logic(i);
	tPackedVal obs = mCalc.get_obs(node);
	if ( node->name() ) {
	  out << setw(20) << std::setfill(' ') << node->name();
	}
	out << ": " << hex << setw(16) << std::setfill('0') << obs << dec << endl;
      }
      out << endl;
    }
    if ( trace_count ) {
      size_t n_total = 0;
      for (size_t i = 0; i < ni; ++ i) {
	const TgNode* node = network.input(i);
	tPackedVal obs = mCalc.get_obs(node);
	size_t n = count_ones(obs);
	samples1[node->gid()] += n;
	n_total += n;
      }
      for (size_t i = 0; i < nl; ++ i) {
	const TgNode* node = network.logic(i);
	tPackedVal obs = mCalc.get_obs(node);
	size_t n = count_ones(obs);
	samples1[node->gid()] += n;
	n_total += n;
      }
      double v = static_cast<double>(n_total) / static_cast<double>(kPvBitLen);
      total += v;
    }
  }
  if ( trace_count ) {
    double ave = total / static_cast<double>(loop_num);
    out << "Total: " << ave << endl;
    if ( gate ) {
      for (size_t i = 0; i < ni; ++ i) {
	const TgNode* node = network.input(i);
	size_t n = samples1[node->gid()];
	double d = n / static_cast<double>(loop_num * kPvBitLen);
	out << node->name() << ": " << d << endl;
      }
      for (size_t i = 0; i < nl; ++ i) {
	const TgNode* node = network.logic(i);
	size_t n = samples1[node->gid()];
	double d = n / static_cast<double>(loop_num * kPvBitLen);
	out << node->name() << ": " << d << endl;
      }
    }
  }
  
  if ( diff ) {
    for (size_t i = 0; i < ni; ++ i) {
      const TgNode* node = network.input(i);
      size_t n1 = samples1[node->gid()];
      size_t n2 = samples2[node->gid()];
      double d1 = n1 / static_cast<double>(loop_num * kPvBitLen);
      double d2 = n2 / static_cast<double>(loop_num * kPvBitLen);
      double diff = d1 - d2;
      if ( gate ) {
	out << node->name() << ": "
	    << setw(10) << d1 << "\t"
	    << setw(10) << d2 << "\t"
	    << setw(10) << (diff / d1) * 100.0 << endl;
      }
    }
    for (size_t i = 0; i < nl; ++ i) {
      const TgNode* node = network.logic(i);
      size_t n1 = samples1[node->gid()];
      size_t n2 = samples2[node->gid()];
      double d1 = n1 / static_cast<double>(loop_num * kPvBitLen);
      double d2 = n2 / static_cast<double>(loop_num * kPvBitLen);
      double diff = d1 - d2;
      if ( gate ) {
	out << node->name() << ": "
	    << setw(10) << d1 << "\t"
	    << setw(10) << d2 << "\t"
	    << setw(10) << (diff / d1) * 100.0 << endl;
      }
    }
    double ave = total / static_cast<double>(loop_num);
    out << "Total (exact): " << ave << endl;
    double ave2 = total2 / static_cast<double>(loop_num);
    out << "Total (approx): " << ave2 << endl;
    out << "Ave. diff(exact - approx): " << (total - total2) / total * 100.0
	<< endl;
  }
  
  TclObj msg = out.str();
  set_result(msg);
  
  for (size_t i = 0; i < kPvBitLen; ++ i) {
    TestVector::delete_vector(tv_array[i]);
  }
  
  return TCL_OK;
}

END_NAMESPACE_YM_SEAL_CVF


BEGIN_NAMESPACE_YM_SEAL

// CVF 計算関係のコマンドを作る
int
make_cvf_commands(Tcl_Interp* interp,
		  SealMgr* mgr,
		  const string& ns)
{
  string tmp;

  tmp = ns + "::" + "calc_cvf";
  TclCmdBinder1<nsCvf::CvfCmd, SealMgr*>::reg(interp, mgr, tmp);
  
  
  //////////////////////////////////////////////////////////////////////
  // tclreadline 用の処理
  // すごい手抜き
  //////////////////////////////////////////////////////////////////////
  ostringstream buf;
  buf << "namespace eval tclreadline {" << endl
      << "namespace eval " << ns << " {" << endl
      << "proc complete(calc_cvf) { t s e l p m } { return \"\" }" << endl
      << "}" << endl
      << "}" << endl;
  string str = buf.str();
  if ( Tcl_Eval(interp, str.c_str()) == TCL_ERROR ) {
    return TCL_ERROR;
  }
  return TCL_OK;
}

END_NAMESPACE_YM_SEAL
