
/// @file magus/logbase/BNetInfoCmd.cc
/// @brief BNetInfoCmd の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: BNetInfoCmd.cc 2274 2009-06-10 07:45:29Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "BNetInfoCmd.h"
#include "ym_bnet/BNetwork.h"


BEGIN_NAMESPACE_MAGUS

//////////////////////////////////////////////////////////////////////
// ネットワークの諸元を取り出すコマンド
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
BNetInfo::BNetInfo(MagMgr* mgr) :
  BNetCmd(mgr)
{
  const char* usage =
    "<ATTR> ?<value>?\n"
    "\t<ATTR> is model_name, input_num, output_num, logic_node_num,\n"
    "\t\tlatch_node_num, lit_num, sop_lit_num, library_name, varname_rule";
  set_usage_string(usage);
}

// @brief デストラクタ
BNetInfo::~BNetInfo()
{
}

// コマンドを実行する．
int
BNetInfo::cmd_proc(TclObjVector& objv)
{
  size_t objc = objv.size();
  // このコマンドは属性を表す引数をとる．
  if ( objc != 2 && objc != 3 ) {
    // 引数の数が合わない．
    print_usage();
    // 異常終了
    return TCL_ERROR;
  }

  BNetwork* network = cur_network();

  string attr = objv[1];
  TclObj result;
  if ( attr == "model_name" ) {
    if ( objc == 3 ) {
      // 値の設定
      string name = objv[2];
      network->set_model_name(name);
      result = objv[2];
    }
    else {
      result = network->model_name();
    }
  }
  else if ( attr == "varname_rule" ) {
    if ( objc == 3 ) {
      // 値の設定
      TclObjVector elems;
      int code = list_conv(objv[2], elems);
      if ( code == TCL_ERROR ) {
	return TCL_ERROR;
      }
      if ( elems.size() != 2 ) {
	TclObj emsg;
	emsg << objv[2] << " : illegal format for varname_rule";
	set_result(emsg);
	return TCL_ERROR;
      }
      string prefix = elems[0];
      string suffix = elems[1];
      network->change_name_rule(prefix, suffix);
      result = objv[2];
    }
    else {
      string argv[2];
      network->name_rule(argv[0], argv[1]);
      TclObjVector objv(2);
      objv[0] = argv[0];
      objv[1] = argv[1];
      result.set_list(objv);
    }
  }
  else {
    if ( attr == "input_num" ) {
      result = network->input_num();
    }
    else if ( attr == "output_num" ) {
      result = network->output_num();
    }
    else if ( attr == "logic_node_num" ) {
      result = network->logic_node_num();
    }
    else if ( attr == "latch_node_num" ) {
      result = network->latch_node_num();
    }
    else if ( attr == "lit_num" ) {
      result = network->litnum();
    }
    else if ( attr == "sop_lit_num" ) {
      result = network->sop_litnum();
    }
    else if ( attr == "library_name" ) {
#if defined(USE_LIBCELL)
      const hf_CCellMgr* cell_mgr = network->CellMgr();
      if (cell_mgr == NULL) {
	result = "not mapped";
      }
      else {
	result = FindLibraryName(cell_mgr);
      }
#else
      result = "not mapped";
#endif
    }
    else {
      TclObj emsg;
      emsg << attr << " : illegal attribute";
      set_result(emsg);
      return TCL_ERROR;
    }
    if ( objc == 3 ) {
      // この属性は読み出し専用
      TclObj emsg;
      emsg << attr << " is read only";
      set_result(emsg);
      return TCL_ERROR;
    }
  }
  set_result(result);

  return TCL_OK;
}


//////////////////////////////////////////////////////////////////////
// ネットワークの諸元を一括して取り出して連想配列に入れるコマンド
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
BNetAllInfo::BNetAllInfo(MagMgr* mgr) :
  BNetCmd(mgr)
{
  set_usage_string("<varname>");
}

// @brief デストラクタ
BNetAllInfo::~BNetAllInfo()
{
}

// コマンドを実行する．
int
BNetAllInfo::cmd_proc(TclObjVector& objv)
{
  size_t objc = objv.size();
  // このコマンドは連想配列名のみを引数にとる．
  if ( objc != 2 ) {
    // 引数の数が合わない．
    print_usage();
    // 異常終了
    return TCL_ERROR;
  }

  TclObj base = objv[1];

  BNetwork* network = cur_network();

  // モデル名
  set_var(base, "model_name", network->model_name(), 0);
  // 入力数
  set_var(base, "input_num", network->input_num(), 0);
  // 出力数
  set_var(base, "output_num", network->output_num(), 0);
  // 中間変数の数
  set_var(base, "logic_node_num", network->logic_node_num(), 0);
  // FF の数
  set_var(base, "latch_node_num", network->latch_node_num(), 0);
  // リテラル数
  set_var(base, "lit_num", network->litnum(), 0);
  // SOPリテラル数
  set_var(base, "sop_lit_num", network->sop_litnum(), 0);
  // ライブラリ名
#if defined(USE_LIBCELL)
  const hf_CCellMgr* cell_mgr = network->CellMgr();
  if (cell_mgr == NULL) {
    set_var(base, "library_name", "not mapped", 0);
  }
  else {
    string libname = FindLibraryName(cell_mgr);
    set_var(base, "library_name", libname, 0);
  }
#else
  set_var(base, "library_name", "not mapped", 0);
#endif
  return TCL_OK;
}


//////////////////////////////////////////////////////////////////////
// ネットワークの緒元を出力するコマンド
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
BNetPrintStats::BNetPrintStats(MagMgr* mgr) :
  BNetCmd(mgr)
{
}

// @brief デストラクタ
BNetPrintStats::~BNetPrintStats()
{
}

// コマンド処理関数
int
BNetPrintStats::cmd_proc(TclObjVector& objv)
{
  BNetwork* network = cur_network();
  TclObj msg;
  msg << network->model_name()
      << "\tpi=" << TclObj(network->input_num())
      << "\tpo=" << TclObj(network->output_num())
      << "\tnodes=" << TclObj(network->logic_node_num())
      << "\tlatches=0\n"
      << "lits(sop)=" << TclObj(network->sop_litnum())
      << "\tlits(fac)=" << TclObj(network->litnum());
  set_result(msg);

  return TCL_OK;
}

END_NAMESPACE_MAGUS
