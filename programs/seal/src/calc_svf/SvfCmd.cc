
/// @file src/calc_svf/SvfCmd.cc
/// @brief SvfCmd の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: SvfCmd.cc 1920 2008-12-20 15:52:42Z matsunaga $
///
/// Copyright (C) 2005-2008 Yusuke Matsunaga
/// All rights reserved.


#if HAVE_CONFIG_H
#include "seal_config.h"
#endif


#include "seal_utils.h"
#include "SvfCmd.h"
#include <ym_tgnet/TgNetwork.h>
#include "TestVector.h"
#include <ym_tclpp/TclPopt.h>
#include "CalcSvf.h"


BEGIN_NAMESPACE_YM_SEAL_SVF

//////////////////////////////////////////////////////////////////////
// 可観測性を計算するコマンド
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SvfCmd::SvfCmd(SealMgr* mgr) :
  SealCmd(mgr)
{
  mPoptLoop = new TclPoptUint(this, "loop",
			      "loop count");
  mPoptTf = new TclPoptUint(this, "timeframe",
			    "maximum time frame");
  mPoptTrace = new TclPopt(this, "trace",
			   "trace mode");
  mPoptTraceCount = new TclPopt(this, "trace_count",
				"trace count mode");
  mPoptInit = new TclPopt(this, "init",
			 "initialize");
  mPoptDss = new TclPopt(this, "init-dss",
			 "initialize with DSS");
  mPoptExact = new TclPopt(this, "exact",
			   "exact method");
  mPoptExact2 = new TclPopt(this, "exact2",
			   "exact method(2)");
  mPoptMin = new TclPopt(this, "pseudo-min",
			 "pseudo minimal method");
  mPoptMax = new TclPopt(this, "max",
			 "maximal method");
  mPoptDiff = new TclPopt(this, "diff",
			  "calclate diffence");
  mPoptGate = new TclPopt(this, "gate",
			  "display gates' information");
  new_popt_group(mPoptInit, mPoptDss);
  new_popt_group(mPoptExact, mPoptMin, mPoptMax, mPoptDiff);
}

// @brief デストラクタ
SvfCmd::~SvfCmd()
{
}
  
// コマンド処理関数
int
SvfCmd::cmd_proc(TclObjVector& objv)
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
  ymuint time_frame = 1;
  if ( mPoptTf->is_specified() ) {
    time_frame = mPoptTf->val();
  }
  bool trace = mPoptTrace->is_specified();
  bool trace_count = mPoptTraceCount->is_specified();
  bool init = mPoptInit->is_specified();
  bool dss = mPoptDss->is_specified();
  bool gate = mPoptGate->is_specified();
  bool exact = true;
  bool exact2 = false;
  bool min = false;
  bool max = false;
  bool diff = false;
  if ( mPoptExact->is_specified() ) {
    ;
  }
  else if ( mPoptExact2->is_specified() ) {
    exact = false;
    exact2 = true;
  }
  else if ( mPoptMin->is_specified() ) {
    exact = false;
    min = true;
  }
  else if ( mPoptMax->is_specified() ) {
    exact = false;
    max = true;
  }
  else if ( mPoptDiff->is_specified() ) {
    exact = false;
    diff = true;
    trace_count = false;
  }
  
  if ( init ) {
    mCalc.set_network(_network(), time_frame, false);
    return TCL_OK;
  }
  if ( dss ) {
    mCalc.set_network(_network(), time_frame, true);
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
  double total3 = 0.0;
  vector<size_t> samples1(nn, 0);
  vector<size_t> samples2(nn, 0);
  vector<size_t> samples3(nn, 0);
  for (size_t l = 0; l < loop_num; ++ l) {
    for (size_t i = 0; i < kPvBitLen; ++ i) {
      TestVector* tv = tv_array[i];
      tv->set_from_random(rgen);
    }
    
    if ( exact ) {
      mCalc.calc_exact(tv_array);
    }
    else if ( exact2 ) {
      mCalc.calc_exact2(tv_array);
    }
    else if ( min ) {
      mCalc.calc_pseudo_min(tv_array);
    }
    else if ( max ) {
      mCalc.calc_max(tv_array);
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
      
      mCalc.calc_exact2(tv_array);
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
      
      mCalc.calc_pseudo_min(tv_array);
      size_t n_total3 = 0;
      for (size_t i = 0; i < ni; ++ i) {
	const TgNode* node = network.input(i);
	tPackedVal obs = mCalc.get_obs(node);
	size_t n = count_ones(obs);
	samples3[node->gid()] += n;
	n_total3 += n;
      }
      for (size_t i = 0; i < nl; ++ i) {
	const TgNode* node = network.logic(i);
	tPackedVal obs = mCalc.get_obs(node);
	size_t n = count_ones(obs);
	samples3[node->gid()] += n;
	n_total3 += n;
      }
      total3 += static_cast<double>(n_total3) / static_cast<double>(kPvBitLen);
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
    out << "Total: " << ave;
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
    double dsum1 = 0.0;
    double dsum2 = 0.0;
    for (size_t i = 0; i < ni; ++ i) {
      const TgNode* node = network.input(i);
      size_t n1 = samples1[node->gid()];
      size_t n2 = samples2[node->gid()];
      size_t n3 = samples3[node->gid()];
      double d1 = n1 / static_cast<double>(loop_num * kPvBitLen);
      double d2 = n2 / static_cast<double>(loop_num * kPvBitLen);
      double d3 = n3 / static_cast<double>(loop_num * kPvBitLen);
      double diff1 = d1 - d2;
      double diff2 = d1 - d3;
      dsum1 += diff1 * diff1;
      dsum2 += diff2 * diff2;
      if ( gate ) {
	out << node->name() << ": "
	    << setw(10) << d1 << "\t"
	    << setw(10) << d2
	    << " (" << setw(10) << (diff1 / d1) * 100.0 << "%\t"
	    << setw(10) << d3
	    << " (" << setw(10) << (diff2 / d1) * 100.0 << "%" << endl;
      }
    }
    for (size_t i = 0; i < nl; ++ i) {
      const TgNode* node = network.logic(i);
      size_t n1 = samples1[node->gid()];
      size_t n2 = samples2[node->gid()];
      size_t n3 = samples3[node->gid()];
      double d1 = n1 / static_cast<double>(loop_num * kPvBitLen);
      double d2 = n2 / static_cast<double>(loop_num * kPvBitLen);
      double d3 = n3 / static_cast<double>(loop_num * kPvBitLen);
      double diff1 = d1 - d2;
      double diff2 = d1 - d3;
      dsum1 += diff1 * diff1;
      dsum2 += diff2 * diff2;
      if ( gate ) {
	out << node->name() << ": "
	    << setw(10) << d1 << "\t"
	    << setw(10) << d2
	    << " (" << setw(10) << (diff1 / d1) * 100.0 << "%\t"
	    << setw(10) << d3
	    << " (" << setw(10) << (diff2 / d1) * 100.0 << "%" << endl;
      }
    }
    double ave = total / static_cast<double>(loop_num);
    out << "Total (exact): " << ave << endl;
    double ave2 = total2 / static_cast<double>(loop_num);
    out << "Total (exact2): " << ave2 << endl;
    double ave3 = total3 / static_cast<double>(loop_num);
    out << "Total (approx): " << ave3 << endl;
    out << "Ave. diff(exact - exact2): " << (total - total2) / total * 100.0
	<< endl;
    out << "Ave. diff(exact - approx): " << (total - total3) / total * 100.0
	<< endl;
    out << "Ave. div.(exact - exact2): " << sqrt(dsum1 / nn) << endl;
    out << "Ave. div.(exact - approx): " << sqrt(dsum2 / nn) << endl;
  }
  
  TclObj msg = out.str();
  set_result(msg);
  
  for (size_t i = 0; i < kPvBitLen; ++ i) {
    TestVector::delete_vector(tv_array[i]);
  }
  
  return TCL_OK;
}

END_NAMESPACE_YM_SEAL_SVF


BEGIN_NAMESPACE_YM_SEAL

// SVF 計算関係のコマンドを作る
int
make_svf_commands(Tcl_Interp* interp,
		  SealMgr* mgr,
		  const string& ns)
{
  string tmp;

  tmp = ns + "::" + "calc_svf";
  TclCmdBinder1<nsSvf::SvfCmd, SealMgr*>::reg(interp, mgr, tmp);
  
  
  //////////////////////////////////////////////////////////////////////
  // tclreadline 用の処理
  // すごい手抜き
  //////////////////////////////////////////////////////////////////////
  ostringstream buf;
  buf << "namespace eval tclreadline {" << endl
      << "namespace eval " << ns << " {" << endl
      << "proc complete(calc_svf) { t s e l p m } { return \"\" }" << endl
      << "}" << endl
      << "}" << endl;
  string str = buf.str();
  if ( Tcl_Eval(interp, str.c_str()) == TCL_ERROR ) {
    return TCL_ERROR;
  }
  return TCL_OK;
}

END_NAMESPACE_YM_SEAL
