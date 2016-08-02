
/// @file magus/logbase/NtwkBaseCmd.cc
/// @brief NtwkBaseCmd の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: NtwkBaseCmd.cc 2274 2009-06-10 07:45:29Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "NtwkBaseCmd.h"
#include "NetMgr.h"
#include "NetHandle.h"
#include "ym_bnet/BNetManip.h"
#include "ym_tclpp/TclPopt.h"


BEGIN_NAMESPACE_MAGUS

//////////////////////////////////////////////////////////////////////
// 新しいネットワークを作成するコマンド
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
NewNtwk::NewNtwk(NetMgr* mgr) :
  BaseCmd(mgr)
{
  mPoptTrace = new TclPoptBool(this, "trace",
			       "set network trace");
  set_usage_string("?<network-name>?");
}

// @brief デストラクタ
NewNtwk::~NewNtwk()
{
}

// コマンドを実行する．
int
NewNtwk::cmd_proc(TclObjVector& objv)
{
  // trace を付けるかどうか(デフォルトでは付ける)
  bool trace = true;
  // ネットワーク名
  string name;

  if ( mPoptTrace->is_specified() ) {
    trace = mPoptTrace->val();
  }

  size_t objc = objv.size();
  if ( objc > 2 ) {
    // 引数が多すぎる．
    print_usage();
    return TCL_ERROR;
  }
  if ( objc == 2 ) {
    // ネットワーク名が残っていた．
    name = objv[1];
  }
  else {
    // ネットワーク名がない．
    name = mgr()->new_tmpname();
    // 自動的に名前をつける．
    // まさかとは思うけど念のため
    if ( name == string() ) {
      set_result("No more available name for temporary network.");
      return TCL_ERROR;
    }
    // 名無しのネットワークにはデフォルトではトレースを付けない．
    if ( mPoptTrace->count() == 0 ) {
      trace = false;
    }
  }

  // 新たなネットワークを生成する．
  NetHandle* neth = new_nethandle(name);
  if ( !neth ) {
    // 何らかのエラーが起こった．
    return TCL_ERROR;
  }

  // ネットワーク名を返り値に入れておく．
  set_result(name);

  // 正常終了
  return TCL_OK;
}


//////////////////////////////////////////////////////////////////////
// ネットワークを削除するコマンド
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
DelNtwk::DelNtwk(NetMgr* mgr) :
  BaseCmd(mgr)
{
  set_usage_string("<network-name>");
}

// @brief デストラクタ
DelNtwk::~DelNtwk()
{
}

// コマンドを実行する．
int
DelNtwk::cmd_proc(TclObjVector& objv)
{
  // このコマンドはネットワーク名以外の引数をとらない．
  if ( objv.size() != 2 ) {
    print_usage();
    return TCL_ERROR;
  }

  // ネットワークの削除を行う．
  bool result = delete_nethandle(objv[1]);
  if ( !result ) {
    // エラー
    return TCL_ERROR;
  }
  // 正常終了
  return TCL_OK;
}


//////////////////////////////////////////////////////////////////////
// ネットワークの内容をクリアするコマンド
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
ClrNtwk::ClrNtwk(NetMgr* mgr) :
  BaseCmd(mgr)
{
}

// @brief デストラクタ
ClrNtwk::~ClrNtwk()
{
}

// コマンドを実行する．
int
ClrNtwk::cmd_proc(TclObjVector& objv)
{
  // このコマンドは引数を取らない．
  if ( objv.size() != 1 ) {
    print_usage();
    return TCL_ERROR;
  }

  // ネットワークをクリアする．
  cur_nethandle()->bnetwork()->clear();
#warning "TODO: BdNetwork の場合も考慮する"
  
  // このコマンドはエラーとならない．

  // 正常終了
  return TCL_OK;
}


//////////////////////////////////////////////////////////////////////
// コピーを行うコマンド
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
CopyNtwk::CopyNtwk(NetMgr* mgr) :
  BaseCmd(mgr)
{
  set_usage_string("<src-network-name> <dst-network-name>");
}

// @brief デストラクタ
CopyNtwk::~CopyNtwk()
{
}

// コマンドを実行する．
int
CopyNtwk::cmd_proc(TclObjVector& objv)
{
  // このコマンドは厳密に2つの引数をとる．
  if ( objv.size() != 3 ) {
    print_usage();
    return TCL_ERROR;
  }

  // コピー元のネットワーク名を取ってくる．
  NetHandle* s_neth = find_nethandle(objv[1]);
  if ( !s_neth ) {
    // ソースとなるネットワークが存在しなかった．
    return TCL_ERROR;
  }

  // コピー先のネットワーク名をとってくる．
  NetHandle* d_neth = find_or_new_nethandle(objv[2]);
  if ( !d_neth ) {
    // 何らかのエラーが起こった．
    return TCL_ERROR;
  }

  // コピーを行う．
  d_neth->copy(*s_neth);

  // 正常終了
  return TCL_OK;
}


//////////////////////////////////////////////////////////////////////
// カレントネットワークスタックにプッシュするコマンド
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
PushNtwk::PushNtwk(NetMgr* mgr) :
  BaseCmd(mgr)
{
  mPoptNew = new TclPopt(this, "new",
			 "new network before push");
  set_usage_string("<network-name>");
}

// @brief デストラクタ
PushNtwk::~PushNtwk()
{
}

// コマンドを実行する．
int
PushNtwk::cmd_proc(TclObjVector& objv)
{
  bool new_flag = mPoptNew->is_specified();
  size_t objc = objv.size();
  if ( objc != 2 ) {
    print_usage();
    return TCL_ERROR;
  }

  TclObj arg = objv[1];
  if ( new_flag ) {
    TclObj script;
    script.append_element("::magus::new_network");
    script.append_element(arg);
    int stat = eval(script);
    if ( stat == TCL_ERROR ) {
      return TCL_ERROR;
    }
  }

  // カレントネットワークスタックにいれる．
  if ( !push_cur_nethandle(arg) ) {
    return TCL_ERROR;
  }

  // 正常終了
  return TCL_OK;
}


//////////////////////////////////////////////////////////////////////
// カレントネットワークのスタックを元に戻す．
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
PopNtwk::PopNtwk(NetMgr* mgr) :
  BaseCmd(mgr)
{
}

// @brief デストラクタ
PopNtwk::~PopNtwk()
{
}

// コマンドを実行する．
int
PopNtwk::cmd_proc(TclObjVector& objv)
{
  size_t objc = objv.size();
  if ( objc != 1 && objc != 4 ) {
    // 通常は引数はなしだが，変数のトレースで呼び出される時に3つの引数
    // をとる．
    print_usage();
    return TCL_ERROR;
  }

  if ( !pop_cur_nethandle() ) {
    return TCL_ERROR;
  }

  return TCL_OK;
}


//////////////////////////////////////////////////////////////////////
// カレントネットワークを変更するコマンド
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
ChgNtwk::ChgNtwk(NetMgr* mgr) :
  BaseCmd(mgr)
{
  mPoptNew = new TclPopt(this, "new",
			 "new network before change");
  set_usage_string("<network-name>");
}

// @brief デストラクタ
ChgNtwk::~ChgNtwk()
{
}

// コマンドを実行する．
int
ChgNtwk::cmd_proc(TclObjVector& objv)
{
  bool new_flag = mPoptNew->is_specified();
  size_t objc = objv.size();
  if ( objc != 2 ) {
    print_usage();
    return TCL_ERROR;
  }

  string arg = objv[1];
  if ( new_flag ) {
    TclObj script;
    script.append_element("::magus::new_network");
    script.append_element(arg);
    int stat = eval(script);
    if ( stat == TCL_ERROR ) {
      return TCL_ERROR;
    }
  }

  // カレントネットワークにする．
  change_cur_nethandle(arg);

  // 正常終了
  return TCL_OK;
}


//////////////////////////////////////////////////////////////////////
// カレントネットワーク名を返すコマンド
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
CurNtwk::CurNtwk(NetMgr* mgr) :
  BaseCmd(mgr)
{
}

// @brief デストラクタ
CurNtwk::~CurNtwk()
{
}

// コマンドを実行する．
int
CurNtwk::cmd_proc(TclObjVector& objv)
{
  size_t objc = objv.size();
  if( objc != 1 ) {
    print_usage();
    return TCL_ERROR;
  }

  NetHandle* neth = cur_nethandle();
  string name = neth->name();
  set_result(name);
  return TCL_OK;
}


//////////////////////////////////////////////////////////////////////
// ネットワークのリストを作るコマンド
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
ListNtwk::ListNtwk(NetMgr* mgr) :
  BaseCmd(mgr)
{
  mPoptAll = new TclPopt(this, "all",
			 "list all networks");
}

// @brief デストラクタ
ListNtwk::~ListNtwk()
{
}

// コマンドを実行する．
int
ListNtwk::cmd_proc(TclObjVector& objv)
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
  list<string> name_list;
  mgr()->get_network_names(name_list);
  for (list<string>::iterator p = name_list.begin();
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


//////////////////////////////////////////////////////////////////////
// ネットワークを一つづつ取り出して処理するコマンド
// その都度，カレントネットワークを切替える．
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
ForNtwk::ForNtwk(NetMgr* mgr) :
  BaseCmd(mgr)
{
  set_usage_string("<varname> <command>");
}

// @brief デストラクタ
ForNtwk::~ForNtwk()
{
}

// コマンドを実行する．
int
ForNtwk::cmd_proc(TclObjVector& objv)
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
  list<string> name_list;
  mgr()->get_network_names(name_list);
  for (list<string>::iterator p = name_list.begin();
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
