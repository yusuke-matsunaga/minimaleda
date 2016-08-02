
/// @file magus/logbase/BNetElimCmd.cc
/// @brief ElimCmd の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: BNetElimCmd.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.

// ネットワークの eliminate, sweep, clean-up, decomp を行うコマンド

#include "BNetElimCmd.h"

#include "ym_bnet/BNetManip.h"
#include "ym_bnet/BNetDecomp.h"
#include "ym_bnet/BNetGateDecomp.h"
#include "ym_bnet/BNetSopDecomp.h"
#include "ym_tclpp/TclPopt.h"


BEGIN_NAMESPACE_MAGUS

//////////////////////////////////////////////////////////////////////
// eliminate を行うコマンド
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
ElimCmd::ElimCmd(MagMgr* mgr) :
  BNetCmd(mgr)
{
  mPoptL = new TclPoptUint(this, "limit",
			   "specify SOP size limit");
  mPoptAutoLimit = new TclPopt(this, "autolimit",
			       "decide SOP size limit automatically (default)");
  mPoptNoAutoLimit = new TclPopt(this, "noautolimit",
				 "not using autolimit");
  new_popt_group(mPoptAutoLimit, mPoptNoAutoLimit);
  set_usage_string("threshold[=INT]");
}

// @brief デストラクタ
ElimCmd::~ElimCmd()
{
}

// コマンドを実行する
int
ElimCmd::cmd_proc(TclObjVector& objv)
{
  // このコマンドはthreshold値のみを引数にとる．
  // ただし，-l オプションをとる場合がある．

  ymuint limit = UINT_MAX;
  bool autolimit = true;  // 自動的に limit を調節する．

  if ( mPoptL->is_specified() ) {
    limit = mPoptL->val();
  }
  else {
    // limit のデフォルトを tcl 変数から取ってくる．
    TclObj limit_var = var("::magus::default", "elim_limit", 0);
    if ( !limit_var.is_validptr() ) {
      TclObj emsg;
      emsg << objv[0] << " ::magus::default(elim_limit) is not set.";
      set_result(emsg);
      return TCL_ERROR;
    }
    if ( limit_var.get_string() != "NO_LIMIT" &&
	 uint_conv(limit_var, limit) != TCL_OK ) {
      TclObj emsg;
      emsg << objv[0] << " ::magus::default(elim_limit) should be an integer"
	   << " or \"NO_LIMIT\".";
      set_result(emsg);
      return TCL_ERROR;
    }
  }

  if ( mPoptAutoLimit->is_specified() ) {
    autolimit = true;
  }
  else if ( mPoptNoAutoLimit->is_specified() ) {
    autolimit = false;
  }
  else {
    // autolimit のデフォルトを tcl 変数から取ってくる．
    TclObj autolimit_var = var("::magus::default", "elim_autolimit", 0);
    if ( !autolimit_var.is_validptr() ) {
      TclObj emsg;
      emsg << objv[0] << " ::magus::default(autoelim_limit) is not set.";
      set_result(emsg);
      return TCL_ERROR;
    }
    if ( bool_conv(autolimit_var, autolimit) != TCL_OK ) {
      TclObj emsg;
      emsg << objv[0] << " ::magus::default(elim_autolimit) is not a boolean.";
      set_result(emsg);
      return TCL_ERROR;
    }
  }

  size_t objc = objv.size();
  if ( objc != 2 ) {
    print_usage();
    return TCL_ERROR;
  }

  // コマンド行の引数を解析して thresh の値を取って来る．
  int thresh;
  int stat = int_conv(objv[1], thresh);
  if ( stat != TCL_OK ) {
    // エラー
    return stat;
  }

  BNetwork* network = cur_network();
  network->eliminate(thresh, limit, autolimit);

  // 正常終了
  return TCL_OK;
}


//////////////////////////////////////////////////////////////////////
// clean up (ファンアウト数が0のノードの削除) を行うコマンド
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
CleanCmd::CleanCmd(MagMgr* mgr) :
  BNetCmd(mgr)
{
  mPoptRI = new TclPopt(this, "remove_input",
			"remove inputs if possible");
  mPoptPI = new TclPopt(this, "preserve_input",
			"leave all the inputs remain");
  mPoptRL = new TclPopt(this, "remove_latch",
			"remove FFs if possible");
  mPoptPL = new TclPopt(this, "preserve_latch",
			"leave all the FFs remain");
  new_popt_group(mPoptRI, mPoptPI);
  new_popt_group(mPoptRL, mPoptPL);
}

// @brief デストラクタ
CleanCmd::~CleanCmd()
{
}

// コマンドを実行する．
int
CleanCmd::cmd_proc(TclObjVector& objv)
{
  bool remove_latch = false;
  bool remove_input = false;

  if ( mPoptRI->is_specified() ) {
    remove_input = true;
  }
  else if ( mPoptPI->is_specified() ) {
    remove_input = false;
  }

  if ( mPoptRL->is_specified() ) {
    remove_latch = true;
  }
  else if ( mPoptPL->is_specified() ) {
    remove_input = false;
  }

  size_t objc = objv.size();
  if ( objc != 1 ) {
    print_usage();
    return TCL_ERROR;
  }

  // clean_up を行う．
  cur_network()->clean_up();

  if ( remove_input ) {
    // 使われていない外部入力の削除を行う．
    cur_network()->delete_unused_input();
  }

  if ( remove_latch ) {
    // 使われていないラッチの削除を行う．
    ;
  }

  // 正常終了
  return TCL_OK;
}


//////////////////////////////////////////////////////////////////////
// sweep (ファンイン数が1以下のノードの削除) を行うコマンド
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SweepCmd::SweepCmd(MagMgr* mgr) :
  BNetCmd(mgr)
{
}

// @brief デストラクタ
SweepCmd::~SweepCmd()
{
}

// コマンドを実行する．
int
SweepCmd::cmd_proc(TclObjVector& objv)
{
  size_t objc = objv.size();
  if ( objc != 1 ) {
    print_usage();
    return TCL_ERROR;
  }

  // sweep を行う．
  cur_network()->sweep();

  // 正常終了
  return TCL_OK;
}


//////////////////////////////////////////////////////////////////////
// simplify を行うコマンド
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SimplifyCmd::SimplifyCmd(MagMgr* mgr) :
  BNetCmd(mgr)
{
}

// @brief デストラクタ
SimplifyCmd::~SimplifyCmd()
{
}

// コマンドを実行する．
int
SimplifyCmd::cmd_proc(TclObjVector& objv)
{
  size_t objc = objv.size();
  if ( objc != 1 ) {
    print_usage();
    return TCL_ERROR;
  }

  cur_network()->lexp_simplify();

  // 正常終了
  return TCL_OK;
}


//////////////////////////////////////////////////////////////////////
// decompose を行うコマンド
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
DecompCmd::DecompCmd(MagMgr* mgr) :
  BNetCmd(mgr)
{
  mPoptXor = new TclPopt(this, "xor",
			 "allow xor decomposition");
  mPoptMaxFanin = new TclPoptInt(this, "max_fanin",
				 "specify the maximum number of fanins");
  mPoptBalanced = new TclPopt(this, "balanced",
			      "balanced (minimum depth) decomposition");
  mPoptRandom = new TclPopt(this, "random",
			    "randomized decomposition");
  mPoptRandomSeed = new TclPoptInt(this, "random_seed",
				   "specify the random seed");
  new_popt_group(mPoptBalanced, mPoptRandom);
}

// @brief デストラクタ
DecompCmd::~DecompCmd()
{
}

// コマンドを実行する．
int
DecompCmd::cmd_proc(TclObjVector& objv)
{
  bool xor_decomp = mPoptXor->is_specified();
  int max_fanin = 0;
  if ( mPoptMaxFanin->is_specified() ) {
    max_fanin = mPoptMaxFanin->val();
    if ( max_fanin < 2 ) {
      TclObj msg;
      msg << "Less than 2 max_fanin value is useless";
      set_result(msg);
      return TCL_ERROR;
    }
  }
  bool balanced = mPoptBalanced->is_specified();
  bool random = mPoptRandom->is_specified();
  if ( !balanced && !random ) {
    balanced = true;
  }

  size_t objc = objv.size();
  if ( objc != 1 ) {
    print_usage();
    return TCL_ERROR;
  }

  BNetDecomp decomp;

  if ( balanced ) {
    decomp(*(cur_network()), max_fanin, xor_decomp);
  }
  else {
    assert_cond(random, __FILE__, __LINE__);
    RandGen randgen;
    if ( mPoptRandomSeed->is_specified() ) {
      randgen.init(mPoptRandomSeed->val());
    }
    decomp(*(cur_network()), max_fanin, randgen, xor_decomp);
  }

  // 正常終了
  return TCL_OK;
}


//////////////////////////////////////////////////////////////////////
// gate_decompose を行うコマンド
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
GateDecompCmd::GateDecompCmd(MagMgr* mgr) :
  BNetCmd(mgr)
{
  mPoptMaxFanin = new TclPoptInt(this, "max_fanin",
				 "specify the maximum number of fanins");
  mPoptBalanced = new TclPopt(this, "balanced",
			      "balanced (minimum depth) decomposition");
  mPoptRandom = new TclPopt(this, "random",
			    "randomized decomposition");
  mPoptRandomSeed = new TclPoptInt(this, "random_seed",
				   "specify the random seed");
  new_popt_group(mPoptBalanced, mPoptRandom);

  set_usage_string("gate-type(and|nand|or|nor|xor|xnor) ...");
}

// @brief デストラクタ
GateDecompCmd::~GateDecompCmd()
{
}

// コマンドを実行する．
int
GateDecompCmd::cmd_proc(TclObjVector& objv)
{
  ymuint32 type_mask = 0U;
  size_t objc = objv.size();
  if ( objc < 2 ) {
    print_usage();
    return TCL_ERROR;
  }
  for (size_t i = 1; i < objc; ++ i) {
    string tmp = objv[i];
    if ( tmp == "and" || tmp == "AND" ) {
      type_mask |= BNetGateDecomp::kAnd;
    }
    else if ( tmp == "nand" || tmp == "NAND" ) {
      type_mask |= BNetGateDecomp::kNand;
    }
    else if ( tmp == "or" || tmp == "OR" ) {
      type_mask |= BNetGateDecomp::kOr;
    }
    else if ( tmp == "nor" || tmp == "NOR" ) {
      type_mask |= BNetGateDecomp::kNor;
    }
    else if ( tmp == "xor" || tmp == "XOR" ) {
      type_mask |= BNetGateDecomp::kXor;
    }
    else if ( tmp == "xnor" || tmp == "XNOR" ) {
      type_mask |= BNetGateDecomp::kXnor;
    }
    else {
      print_usage();
      return TCL_ERROR;
    }
  }
  if ( (type_mask & BNetGateDecomp::kBaseGrp) == 0U ) {
    TclObj msg;
    msg << "No basic type (and|nand|or|nor) specified.";
    set_result(msg);
    return TCL_ERROR;
  }

  int max_fanin = 0;
  if ( mPoptMaxFanin->is_specified() ) {
    max_fanin = mPoptMaxFanin->val();
    if ( max_fanin < 2 ) {
      TclObj msg;
      msg << "Less than 2 max_fanin value is useless";
      set_result(msg);
      return TCL_ERROR;
    }
  }
  bool balanced = mPoptBalanced->is_specified();
  bool random = mPoptRandom->is_specified();
  if ( !balanced && !random ) {
    balanced = true;
  }

  BNetGateDecomp decomp;

  if ( balanced ) {
    decomp(*(cur_network()), type_mask, max_fanin);
  }
  else {
    assert_cond(random, __FILE__, __LINE__);
    RandGen randgen;
    if ( mPoptRandomSeed->is_specified() ) {
      randgen.init(mPoptRandomSeed->val());
    }
    decomp(*(cur_network()), type_mask, max_fanin, randgen);
  }

  // 正常終了
  return TCL_OK;
}


//////////////////////////////////////////////////////////////////////
// SOP decompose を行うコマンド
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SopDecompCmd::SopDecompCmd(MagMgr* mgr) :
  BNetCmd(mgr)
{
}

// @brief デストラクタ
SopDecompCmd::~SopDecompCmd()
{
}

// コマンドを実行する．
int
SopDecompCmd::cmd_proc(TclObjVector& objv)
{
  size_t objc = objv.size();
  if ( objc != 1 ) {
    print_usage();
    return TCL_ERROR;
  }

  BNetSopDecomp decomp;

  decomp(*(cur_network()));

  // 正常終了
  return TCL_OK;
}


//////////////////////////////////////////////////////////////////////
// 組み合わせ回路化する行うコマンド
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
ConvCombiCmd::ConvCombiCmd(MagMgr* mgr) :
  BNetCmd(mgr)
{
}

// @brief デストラクタ
ConvCombiCmd::~ConvCombiCmd()
{
}

BEGIN_NONAMESPACE

// 適当な名前を生成する．
string
make_name(const string& base,
	  const string& ext,
	  const BNetwork* network)
{
  string name0 = base + ext;
  if ( network->find_node(name0) == NULL &&
       network->find_ponode(name0) == NULL ) {
    return name0;
  }
  for (size_t i = 0; i < 999; ++ i) {
    ostringstream buf;
    buf << name0 << i;
    string name1 = buf.str();
    if ( network->find_node(name1) == NULL &&
	 network->find_ponode(name1) == NULL ) {
      return name1;
    }
  }
  return string();
}

END_NONAMESPACE

// コマンド処理関数
int
ConvCombiCmd::cmd_proc(TclObjVector& objv)
{
  size_t objc = objv.size();
  if ( objc != 1 ) {
    print_usage();
    return TCL_ERROR;
  }

  BNetwork* network = cur_network();
  BNetManip manip(network);

  BNodeVector tmp;
  tmp.reserve(network->latch_node_num());
  for (BNodeList::const_iterator p = network->latch_nodes_begin();
       p != network->latch_nodes_end(); ++ p) {
    BNode* latch = *p;
    tmp.push_back(latch);
    string in_name = make_name(latch->name(), "_OUT", network);
    if ( in_name == string() ) {
      TclObj msg;
      msg << "Error: no name available for " << latch->name();
      set_result(msg);
      return TCL_ERROR;
    }
    BNode* input = manip.new_input(in_name);
    if ( input == NULL ) {
      TclObj msg;
      msg << "Unexpected error: new_input(" << in_name << ") failed";
      set_result(msg);
      return TCL_ERROR;
    }
    bool stat1 = manip.replace_node(latch, input);
    if ( !stat1 ) {
      TclObj msg;
      msg << "Unexpected error: replace_node(latch, input) failed";
      set_result(msg);
      return TCL_ERROR;
    }
    string out_name = make_name(latch->name(), "_IN", network);
    if ( out_name == string() ) {
      TclObj msg;
      msg << "Error: no name available for " << latch->name();
      set_result(msg);
      return TCL_ERROR;
    }
    BNode* output = manip.new_output(out_name);
    if ( output == NULL ) {
      TclObj msg;
      msg << "Unexpected error: new_input(" << out_name << ") failed";
      set_result(msg);
      return TCL_ERROR;
    }
    BNode* inode = latch->fanin(0);
    bool stat2 = manip.change_output(output, inode);
    if ( !stat2 ) {
      TclObj msg;
      msg << "Unexpected error: change_output(output, inode) failed";
      set_result(msg);
      return TCL_ERROR;
    }
  }
  for (BNodeVector::iterator p = tmp.begin(); p != tmp.end(); ++ p) {
    BNode* latch = *p;
    manip.delete_node(latch);
  }
  return TCL_OK;
}

END_NAMESPACE_MAGUS
