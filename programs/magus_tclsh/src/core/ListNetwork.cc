
/// @file src/core/ListNtwork.cc
/// @brief ListNtwork の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: ListNtwork.cc 2274 2009-06-10 07:45:29Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ListNetwork.h"
#include "MagMgr.h"
#include "ym_tclpp/TclPopt.h"


BEGIN_NAMESPACE_MAGUS

//////////////////////////////////////////////////////////////////////
// ネットワークのリストを作るコマンド
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
ListNetwork::ListNetwork(MagMgr* mgr) :
  MagCmd(mgr)
{
  mPoptAll = new TclPopt(this, "all",
			 "list all networks");
}

// @brief デストラクタ
ListNetwork::~ListNetwork()
{
}

// コマンドを実行する．
int
ListNetwork::cmd_proc(TclObjVector& objv)
{
  // @xxx という形のネットワークもリストするかどうか
  bool all = mPoptAll->is_specified();
  size_t objc = objv.size();
  if ( objc != 1 ) {
    // このコマンドは引数を取らない．
    print_usage();
    return TCL_ERROR;
  }

  TclObj result;
  result.clear();
  vector<string> name_list;
  mgr()->get_network_names(name_list);
  for (vector<string>::iterator p = name_list.begin();
       p != name_list.end(); ++ p) {
    string name = *p;
    if ( all || name[0] != '@' ) {
      result.append_element(name, interp());
    }
  }
  set_result(result);

  // 正常終了
  return TCL_OK;
}

END_NAMESPACE_MAGUS
