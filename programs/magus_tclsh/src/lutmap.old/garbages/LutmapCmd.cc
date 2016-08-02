
/// @file magus/lutmap/LutmapCmd.cc
/// @brief LutmapCmd の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: LutmapCmd.cc 1920 2008-12-20 15:52:42Z matsunaga $
///
/// Copyright (C) 2005-2006 Yusuke Matsunaga
/// All rights reserved.


#if HAVE_CONFIG_H
#include <magus_config.h>
#endif

#include <MagMgr.h>
#include "LutmapCmd.h"
#include "LutmapCmd2.h"
#include "SbjGraph.h"
#include "LutNetwork.h"
#include "CutHolder.h"
#include "MapRecord.h"
#include "XDagCover.h"
#include "DagACover.h"
#include "DagDCover.h"


BEGIN_NAMESPACE_MAGUS_LUTMAP

// BNetwork から SbjGraph への変換
bool
bnet2sbj(const BNetwork& network,
	 SbjGraph& bingraph,
	 ostream& err_out);

// LutNetwork から BNetwork への変換
void
lut2bnet(const LnGraph& src_network,
	 BNetwork& dst_network);

// DAG covering 用の制約を作る．
void
generate_ILP(const SbjGraph& sbjgraph,
	     size_t limit,
	     tCutMode mode,
	     ostream& s);


//////////////////////////////////////////////////////////////////////
// マッピングを行うコマンド
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
LutmapCmd::LutmapCmd(MagMgr* mgr) :
  MagCmd(mgr)
{
}
  
// @brief コマンドを実行する仮想関数
int
LutmapCmd::cmd_proc(TclObjVector& objv)
{
  int mode = 0;
  int climit = 0;
  int slimit = 0;
  double tlimit = 0.0;
  int xlbcount = 0;
  int slack = 0;
  bool verbose = false;
  BNetwork* p_network = NULL;
  tCutMode cutmode = kCmTree;
  string fargs;
  
  // 読み出し位置
  TclObjVector::iterator rpos = objv.begin();
  // コマンド名をスキップ
  ++ rpos;
  // 書き込み位置
  TclObjVector::iterator wpos = rpos;
  // 終了位置
  TclObjVector::iterator end = objv.end();
  // オプション解析ループ
  for ( ; rpos != end; ++ rpos) {
    TclObj obj = *rpos;
    string arg_str = obj;
    if ( arg_str[0] == '-' ) {
      if ( arg_str == "-dst_network" ) {
	if ( p_network ) {
	  TclObj msg;
	  msg << arg_str << ": appears more than once";
	  set_result(msg);
	  return TCL_ERROR;
	}
	++ rpos;
	if ( rpos == end ) {
	  TclObj msg;
	  msg << arg_str << ": need an argument (network name)";
	  set_result(msg);
	  return TCL_ERROR;
	}
	p_network = find_or_new_network(*rpos);
	if ( p_network == NULL ) {
	  // 見付からなかった
	  // エラーメッセージは find_network() の中でセットされている．
	  return TCL_ERROR;
	}
      }
      else if ( arg_str == "-tree" ) {
	mode = 1;
	cutmode = kCmTree;
	fargs = "tree";
      }
      else if ( arg_str == "-df" ) {
	mode = 1;
	cutmode = kCmDF;
	fargs = "df";
      }
      else if ( arg_str == "-dmin" ) {
	mode = 6;
	cutmode = kCmAll;
	fargs = "dmin";
      }
      else if ( arg_str == "-dag" ) {
	mode = 1;
	cutmode = kCmAll;
	fargs = "dag";
      }
      else if ( arg_str == "-dagr" ) {
	mode = 1;
	cutmode = kCmAll;
	fargs = "dagr";
      }
      else if ( arg_str == "-fo" ) {
	mode = 1;
	cutmode = kCmAll;
	fargs = "fo";
      }
      else if ( arg_str == "-for" ) {
	mode = 1;
	cutmode = kCmAll;
	fargs = "for";
      }
      else if ( arg_str == "-dagd" ) {
	mode = 6;
	fargs = "dagd";
      }
      else if ( arg_str == "-dagdr" ) {
	mode = 6;
	fargs = "dagdr";
      }
      else if ( arg_str == "-fod" ) {
	mode = 6;
	fargs = "fod";
      }
      else if ( arg_str == "-fodr" ) {
	mode = 6;
	fargs = "fodr";
      }
      else if ( arg_str == "-xdag" ) {
	mode = 7;
	fargs = "dag";
      }
      else if ( arg_str == "-xdagr" ) {
	mode = 7;
	fargs = "dagr";
      }
      else if ( arg_str == "-v" ) {
	verbose = true;
      }
      else if ( arg_str == "-dump" ) {
	mode = 99;
      }
      else if ( arg_str == "-slack" ) {
	++ rpos;
	if ( rpos == end ) {
	  TclObj msg;
	  msg << arg_str << ": need an argument (n)";
	  set_result(msg);
	  return TCL_ERROR;
	}
	int stat = (*rpos).get_int(slack, interp());
	if ( stat != TCL_OK ) {
	  // int ではなかった．
	  TclObj msg;
	  msg << "'" << *rpos << "': Illegal value for " << arg_str
	      << ": should be an integer";
	  set_result(msg);
	  return TCL_ERROR;
	}
      }
      else if ( arg_str == "-climit" ) {
	++ rpos;
	if ( rpos == end ) {
	  TclObj msg;
	  msg << arg_str << ": need an argument (n)";
	  set_result(msg);
	  return TCL_ERROR;
	}
	int stat = (*rpos).get_int(climit, interp());
	if ( stat != TCL_OK ) {
	  // int ではなかった．
	  TclObj msg;
	  msg << "'" << *rpos << "': Illegal value for " << arg_str
	      << ": should be an integer";
	  set_result(msg);
	  return TCL_ERROR;
	}
      }
      else if ( arg_str == "-slimit" ) {
	++ rpos;
	if ( rpos == end ) {
	  TclObj msg;
	  msg << arg_str << ": need an argument (n)";
	  set_result(msg);
	  return TCL_ERROR;
	}
	int stat = (*rpos).get_int(slimit, interp());
	if ( stat != TCL_OK ) {
	  // int ではなかった．
	  TclObj msg;
	  msg << "'" << *rpos << "': Illegal value for " << arg_str
	      << ": should be an integer";
	  set_result(msg);
	  return TCL_ERROR;
	}
      }
      else if ( arg_str == "-tlimit" ) {
	++ rpos;
	if ( rpos == end ) {
	  TclObj msg;
	  msg << arg_str << ": need an argument (n)";
	  set_result(msg);
	  return TCL_ERROR;
	}
	int stat = (*rpos).get_double(tlimit, interp());
	if ( stat != TCL_OK ) {
	  // double ではなかった．
	  TclObj msg;
	  msg << "'" << *rpos << "': Illegal value for " << arg_str
	      << ": should be a double";
	  set_result(msg);
	  return TCL_ERROR;
	}
      }
      else if ( arg_str == "-xlbcount" ) {
	++ rpos;
	if ( rpos == end ) {
	  TclObj msg;
	  msg << arg_str << ": need an argument (n)";
	  set_result(msg);
	  return TCL_ERROR;
	}
	int stat = (*rpos).get_int(xlbcount, interp());
	if ( stat != TCL_OK ) {
	  // int ではなかった．
	  TclObj msg;
	  msg << "'" << *rpos << "': Illegal value for " << arg_str
	      << ": should be an integer";
	  set_result(msg);
	  return TCL_ERROR;
	}
      }
      else if ( arg_str == "-ilp" ) {
	mode = 50;
      }
      else {
	TclObj emsg;
	emsg << "unknown option: " << arg_str;
	set_result(emsg);
	return TCL_ERROR;
      }
    }
    else {
      // それ以外の引数はあとで解析する．
      if ( rpos != wpos ) {
	*wpos = obj;
      }
      ++ wpos;
    }
  }
  // wpos 以降を削除
  objv.erase(wpos, end);

  if ( objv.size() != 2 ) {
    print_usage();
    return TCL_ERROR;
  }
  
  int limit;
  int code = int_conv(objv[1], limit);
  if ( code != TCL_OK ) {
    return code;
  }

  try {
    SbjGraph sbjgraph;

    // subject graph を用意
    BNetwork& src_network = *cur_network();

    BNetwork& dst_network = (p_network) ? *p_network : src_network;

    size_t lim = 2;
    if ( mode == 100 ) {
      lim = limit;
    }
    bool stat1 = bnet2sbj(src_network, sbjgraph, cerr);
    if ( !stat1 ) {
      return TCL_ERROR;
    }

    LnGraph mapnetwork;
    int lut_num;
    int depth;
    switch ( mode ) {
    case 1:
      {
	DagACoverFactory factory;
	DagACover* dagcov = factory(fargs);
	CutHolder ch;
	ch.enum_cut(sbjgraph, limit, cutmode);
	MapRecord maprec;
	dagcov->record_cuts(sbjgraph, ch, maprec);
	maprec.gen_mapgraph(sbjgraph, mapnetwork, lut_num, depth);
	delete dagcov;
	lut2bnet(mapnetwork, dst_network);
      }
      break;

    case 6:
      {
	DagDCoverFactory ddfactory;
	CutHolder ch;
	ch.enum_cut(sbjgraph, limit, kCmAll);
	DagDCover* dagdcov = ddfactory(fargs);
	MapRecord maprec;
	dagdcov->record_cuts(sbjgraph, ch, 0, maprec);
	maprec.gen_mapgraph(sbjgraph, mapnetwork, lut_num, depth);
	delete dagdcov;
	lut2bnet(mapnetwork, dst_network);
      }
      break;

    case 7:
      {
#if 0
	if ( xlbcount > 0 ) {
	  lbfactory.set_lbcount(xlbcount);
	}
#endif
	XDagCover xdagcov(fargs, verbose);
	xdagcov.set_climit(climit);
	xdagcov.set_slimit(slimit);
	if ( tlimit > 0.0 ) {
	  xdagcov.set_tlimit(tlimit);
	}
	CutHolder ch;
	ch.enum_cut(sbjgraph, limit, kCmAll);
	MapRecord maprec;
	xdagcov.record_cuts(sbjgraph, ch, maprec);
	maprec.gen_mapgraph(sbjgraph, mapnetwork, lut_num, depth);
	lut2bnet(mapnetwork, dst_network);
      }
      break;

    case 50: // ILP
      generate_ILP(sbjgraph, limit, kCmAll, cout);
      break;

    case 99: // dump
      dump(cout, sbjgraph);
      break;

    default:
      break;
    }

    if ( mode < 10 ) {
      set_var("::magus::lutmap_stats", "lut_num",
	      lut_num,
	      TCL_NAMESPACE_ONLY | TCL_LEAVE_ERR_MSG);
      set_var("::magus::lutmap_stats", "level",
	      depth,
	      TCL_NAMESPACE_ONLY | TCL_LEAVE_ERR_MSG);
    }
  }
  catch ( AssertError x ) {
    cerr << x << endl;
    TclObj emsg;
    emsg << "Assertion Error";
    set_result(emsg);
    return TCL_ERROR;
  }
  
  return TCL_OK;
}

END_NAMESPACE_MAGUS_LUTMAP

BEGIN_NAMESPACE_MAGUS

int
lutmap_old_init(Tcl_Interp* interp,
		MagMgr* mgr)
{
  TclCmdBinder1<nsLutmap::LutmapCmd, MagMgr*>::reg(interp, mgr,
						   "magus::lutmap_old");

  const char* init =
    "namespace eval tclreadline {\n"
    "namespace eval magus {\n"
    "proc complete(lutmap_old) { text start end line pos mod } { return \"\" }\n"
    "}\n"
    "}\n";

  if ( Tcl_Eval(interp, (char*) init) == TCL_ERROR ) {
    return TCL_ERROR;
  }

  return TCL_OK;
}

END_NAMESPACE_MAGUS
