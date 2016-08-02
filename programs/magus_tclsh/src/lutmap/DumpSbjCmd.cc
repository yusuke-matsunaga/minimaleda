
/// @file magus/lutmap/DumpSbjCmd.cc
/// @brief DumpSbjCmd の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: DumpSbjCmd.cc 2274 2009-06-10 07:45:29Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "LutmapCmd.h"
#include "ym_sbj/SbjGraph.h"
#include "ym_sbj/SbjDumper.h"
#include "ym_tclpp/TclPopt.h"


BEGIN_NAMESPACE_MAGUS

//////////////////////////////////////////////////////////////////////
// SbjGraph の内容をダンプするコマンド
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
DumpSbjCmd::DumpSbjCmd(MagMgr* mgr,
		       LutmapData* data) :
  LutmapCmd(mgr, data)
{
  mPoptBlif = new TclPopt(this, "blif",
			  "blif mode");
  mPoptVerilog = new TclPopt(this, "verilog",
			     "verilog mode");
  new_popt_group(mPoptBlif, mPoptVerilog);
  set_usage_string("?<filename>?");
}

// @brief デストラクタ
DumpSbjCmd::~DumpSbjCmd()
{
}

// @brief コマンドを実行する仮想関数
int
DumpSbjCmd::cmd_proc(TclObjVector& objv)
{
  bool blif = mPoptBlif->is_specified();
  bool verilog = mPoptVerilog->is_specified();

  ymuint objc = objv.size();
  if ( objc > 2 ) {
    print_usage();
    return TCL_ERROR;
  }

  try {
    ostream* outp = &cout;
    ofstream ofs;
    if ( objc == 2 ) {
      string file_name = objv[1];
      if ( !open_ofile(ofs, file_name) ) {
	// ファイルが開けなかった．
	return TCL_ERROR;
      }
      outp = &ofs;
    }

    SbjDumper d;
    if ( blif ) {
      d.dump_blif(*outp, sbjgraph());
    }
    else if ( verilog ) {
      d.dump_verilog(*outp, sbjgraph());
    }
    else {
      d.dump(*outp, sbjgraph());
    }
    return TCL_OK;
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

END_NAMESPACE_MAGUS
