
/// @file magus/src/seal/McaCmd.cc
/// @brief McaCmd の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: McaCmd.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.

#include "McaCmd.h"
#include "ym_seal/MCAnalysis.h"
#include "ym_bnet/BNetwork.h"


BEGIN_NAMESPACE_MAGUS

// @brief コンストラクタ
McaCmd::McaCmd(MagMgr* mgr) :
  BNetCmd(mgr)
{
  mPoptBdd = new TclPopt(this, "use_bdd",
			 "use BDD");

  mPoptDump = new TclPopt(this, "dump_trans",
			 "dump transition probability");

  mPoptRestore = new TclPoptStr(this, "restore",
				"restore transition probability",
				"<probability filename>");

  mPoptKiss = new TclPoptStr(this, "kiss",
			     "specify KISS file",
			     "<file-name>");
}

// @brief デストラクタ
McaCmd::~McaCmd()
{
}


// コマンド処理関数
int
McaCmd::cmd_proc(TclObjVector& objv)
{
  size_t objc = objv.size();

  if ( objc != 1 ) {
    print_usage();
    return TCL_ERROR;
  }

  BNetwork* bnetwork = cur_network();
  vector<State> init_states;
  bool use_bdd = false;
  bool dump_trans = false;

  if ( mPoptBdd->is_specified() ) {
    use_bdd = true;
  }
  if ( mPoptDump->is_specified() ) {
    dump_trans = true;
  }
  if ( mPoptRestore->is_specified() ) {
    string file_name = mPoptRestore->val();
    // ファイル名の展開を行う．
    string ex_file_name;
    bool stat = tilde_subst(file_name, ex_file_name);
    if ( !stat ) {
      // ファイル名文字列の中に誤りがあった．
      return TCL_ERROR;
    }
    ifstream fi(ex_file_name.c_str());
    if( !fi ) {
      TclObj emsg;
      emsg << "Could not open file: " << ex_file_name;
      set_result(emsg);
      return TCL_ERROR;
    }
    nsSeal::MCAnalysis mca;
    mca.analyze2(fi);
    return TCL_OK;
  }
  if ( mPoptKiss->is_specified() ) {
    string file_name = mPoptKiss->val();
    // ファイル名の展開を行う．
    string ex_file_name;
    bool stat = tilde_subst(file_name, ex_file_name);
    if ( !stat ) {
      // ファイル名文字列の中に誤りがあった．
      return TCL_ERROR;
    }
    ifstream fi(ex_file_name.c_str());
    if( !fi ) {
      TclObj emsg;
      emsg << "Could not open file: " << ex_file_name;
      set_result(emsg);
      return TCL_ERROR;
    }
    read_kiss(fi, init_states);
  }
  else {
    ymuint32 n = bnetwork->latch_node_num();
    State s;
    for (ymuint32 i = 0; i < n; ++ i) {
      s += '0';
    }
    init_states.push_back(s);
  }

  nsSeal::MCAnalysis mca;

  mca.analyze(*bnetwork, init_states, use_bdd, dump_trans);

  return TCL_OK;
}

void
McaCmd::read_kiss(ifstream &fi,
		  vector<State>& init_states)
{
  string tmp;

  fi >> tmp >> tmp >> tmp;

  ymuint input_num;
  ymuint output_num;
  ymuint state_num;
  ymuint product_num;

  fi >> tmp >> input_num;
  fi >> tmp >> output_num;
  fi >> tmp >> state_num;
  fi >> tmp >> product_num;
  fi >> tmp >> tmp;

  //真理値表を読み込む
  for (ymuint i = 0; i < product_num; ++ i) {
    fi >> tmp >> tmp >> tmp >> tmp;
  }

  fi >> tmp;

  init_states.clear();
  for (ymuint i = 0; i < state_num; ++ i) {
    string tmp_name;
    string tmp_code;
    fi >> tmp >> tmp_name >> tmp_code;
    init_states.push_back(tmp_code);
  }
}

END_NAMESPACE_MAGUS
