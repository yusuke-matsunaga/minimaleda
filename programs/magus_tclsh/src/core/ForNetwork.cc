
/// @file src/core/ForNetwork.cc
/// @brief ForNetwork の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: NtwkBaseCmd.cc 2274 2009-06-10 07:45:29Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ForNetwork.h"
#include "MagMgr.h"


BEGIN_NAMESPACE_MAGUS

//////////////////////////////////////////////////////////////////////
// ネットワークを一つづつ取り出して処理するコマンド
// その都度，カレントネットワークを切替える．
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
ForNetwork::ForNetwork(MagMgr* mgr) :
  MagCmd(mgr)
{
  set_usage_string("<varname> <command>");
}

// @brief デストラクタ
ForNetwork::~ForNetwork()
{
}

// コマンドを実行する．
int
ForNetwork::cmd_proc(TclObjVector& objv)
{
  size_t objc = objv.size();
  if ( objc != 3 ) {
    // このコマンドは2つの引数をとる．
    print_usage();
    return TCL_ERROR;
  }

  // ネットワーク名を入れる変数名を取り出す．
  string varname = objv[1];

  // 実行すべきコマンドを取り出す．
  const TclObj& commands = objv[2];

  // ネットワーク名を一つずつ取り出して処理する．
  int result = TCL_OK;
  vector<string> name_list;
  mgr()->get_network_names(name_list);
  for (vector<string>::iterator p = name_list.begin();
       p != name_list.end(); ++ p) {
    string name = *p;

    // 実行結果をクリアしておく．
    reset_result();

    // 空文字列でなければネットワーク名をその変数にセットする．
    if ( varname != "" ) {
      set_var(varname, name, 0);
    }

    // ネットワークエントリをとってきてカレントネットワークに設定する．
    TclObj tmp_script;
    tmp_script.append_element("push_current_network");
    tmp_script.append_element(name);
    int push_stat = eval(tmp_script);
    if ( push_stat != TCL_OK ) {
      return push_stat;
    }

    // コマンドを実行する．
    int code = eval(commands);

    // カレントネットワークを元に戻す．
    int pop_stat = eval("pop_current_network");
    if ( pop_stat != TCL_OK ) {
      return pop_stat;
    }

    // 実行結果の解析
    if ( code == TCL_CONTINUE ) {
      // 実行結果が continue の場合
      continue;
    }
    else if ( code == TCL_BREAK ) {
      // 実行結果が break の場合
      result = TCL_OK; // 念のため．
      break;
    }
    else if ( code != TCL_OK ) {
      // それ以外の異常値の場合はそれを返す．
      result = code;
      break;
    }
  }

  return result;
}

END_NAMESPACE_MAGUS
