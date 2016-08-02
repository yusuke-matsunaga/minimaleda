
/// @file magus/src/seal/McaCmd2.cc
/// @brief McaCmd2 の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: McaCmd2.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.

#include "McaCmd2.h"
#include "ym_seal/MCAnalysis.h"
#include "ym_bnet/BNetwork.h"


BEGIN_NAMESPACE_MAGUS

// @brief コンストラクタ
McaCmd2::McaCmd2(MagMgr* mgr) :
  BNetCmd(mgr)
{
  mPoptKiss = new TclPoptStr(this, "kiss",
			     "specify KISS file",
			     "<file-name>");
  mPoptState = new TclPoptStr(this, "state",
			      "reset state");
  mPoptComb = new TclPopt(this, "comb",
			  "do comb_analysis");
  mPoptFFPO = new TclPopt(this, "ffpo",
			  "do ff_po_analysis");
}

// @brief デストラクタ
McaCmd2::~McaCmd2()
{
}


// コマンド処理関数
int
McaCmd2::cmd_proc(TclObjVector& objv)
{
  if ( objv.size() != 1 ) {
    print_usage();
    return TCL_ERROR;
  }

  BNetwork* bnetwork = cur_network();
  ymuint ff_num = bnetwork->latch_node_num();

  //オプション解析
  bool ffpo_flag = false;
  if ( mPoptFFPO->is_specified() ) {
    ffpo_flag = true;
  }
  bool comb_flag = false;
  if ( mPoptComb->is_specified() ) {
    comb_flag = true;
  }
  State reset_state;
  if ( mPoptState->is_specified() ) {
    reset_state = mPoptState->val();
    if ( reset_state.size() != ff_num ) {
      cerr << "reset state size is incorrect" << endl;
      return TCL_ERROR;
    }
    cout << "reset_state: " << reset_state << endl;
  }
  else {//出発点が指定されなければオール0にする
    reset_state = "";
    for (size_t i = 0; i < ff_num; ++ i) {
      reset_state += "0";
    }
  }

  vector<State> init_states;
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
    init_states.push_back(reset_state);
  }

  nsSeal::MCAnalysis mca;
  mca.analyze(*bnetwork, init_states);

  return TCL_OK;
}

void
McaCmd2::read_kiss(ifstream &fi,
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
