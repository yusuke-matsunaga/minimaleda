
/// @file magus/techmap/DumpCnCmd.cc
/// @brief DumpCnCmd の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: DumpLutCmd.cc 2274 2009-06-10 07:45:29Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "DumpCnCmd.h"
#include "ym_tclpp/TclPopt.h"


BEGIN_NAMESPACE_MAGUS_TECHMAP

//////////////////////////////////////////////////////////////////////
// マッピングを行うコマンド
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
DumpCnCmd::DumpCnCmd(MagMgr* mgr,
		     TechmapData* data) :
  TechmapCmd(mgr, data)
{
  mPoptVerilog = new TclPopt(this, "verilog",
			     "verilog mode");
  set_usage_string("?<filename>");
}

// @brief デストラクタ
DumpCnCmd::~DumpCnCmd()
{
}

// @brief コマンドを実行する仮想関数
int
DumpCnCmd::cmd_proc(TclObjVector& objv)
{
  ymuint objc = objv.size();
  if ( objc > 2 ) {
    print_usage();
    return TCL_ERROR;
  }
  bool verilog = mPoptVerilog->is_specified();

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
    if ( verilog ) {
      dump_verilog(*outp, cngraph());
    }
    else {
      dump(*outp, cngraph());
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

END_NAMESPACE_MAGUS_TECHMAP
