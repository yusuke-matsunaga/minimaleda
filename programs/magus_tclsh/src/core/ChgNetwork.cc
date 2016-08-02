
/// @file src/core/ChgNetwork.cc
/// @brief ChgNetwork の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: NtwkBaseCmd.cc 2274 2009-06-10 07:45:29Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ChgNetwork.h"
#include "NetHandle.h"
#include "ym_tclpp/TclPopt.h"


BEGIN_NAMESPACE_MAGUS

//////////////////////////////////////////////////////////////////////
// カレントネットワークを変更するコマンド
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
ChgNetwork::ChgNetwork(MagMgr* mgr) :
  MagCmd(mgr)
{
  mPoptNewBNet = new TclPopt(this, "new_bnet",
			     "new bnetwork before change");
  mPoptNewBdn = new TclPopt(this, "new_bdn",
			    "new bdnetwork before change");
  mPoptNewMvn = new TclPopt(this, "new_mvn",
			    "new mvnetwork before change");
  set_usage_string("<network-name>");
}

// @brief デストラクタ
ChgNetwork::~ChgNetwork()
{
}

// コマンドを実行する．
int
ChgNetwork::cmd_proc(TclObjVector& objv)
{
  size_t objc = objv.size();
  if ( objc != 2 ) {
    print_usage();
    return TCL_ERROR;
  }

  string arg = objv[1];
  int stat = new_opt(arg);
  if ( stat != TCL_OK ) {
    return stat;
  }

  // カレントネットワークにする．
  change_cur_nethandle(arg);

  // 正常終了
  return TCL_OK;
}

// @brief -new_bnet, -new_bdn オプションの処理を行う．
// @param[in] name ネットワーク名
int
ChgNetwork::new_opt(string name)
{
  if ( mPoptNewBNet->is_specified() ) {
    TclObj script;
    script.append_element("::magus::new_bnetwork");
    script.append_element(name);
    int stat = eval(script);
    if ( stat == TCL_ERROR ) {
      return TCL_ERROR;
    }
  }
  if ( mPoptNewBdn->is_specified() ) {
    TclObj script;
    script.append_element("::magus::new_bdnetwork");
    script.append_element(name);
    int stat = eval(script);
    if ( stat == TCL_ERROR ) {
      return TCL_ERROR;
    }
  }
  if ( mPoptNewMvn->is_specified() ) {
    TclObj script;
    script.append_element("::magus::new_mvnetwork");
    script.append_element(name);
    int stat = eval(script);
    if ( stat == TCL_ERROR ) {
      return TCL_ERROR;
    }
  }
  return TCL_OK;
}

END_NAMESPACE_MAGUS
