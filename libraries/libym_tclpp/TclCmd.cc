
/// @file libym_tclpp/TclCmd.cc
/// @brief TclCmd の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: TclCmd.cc 1978 2009-02-06 12:29:16Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_tclpp/TclCmd.h"
#include "ym_tclpp/TclPopt.h"
#include "ym_tclpp/TclObj.h"

#include "TclPoptGroup.h"
#include "StrBuf.h"


BEGIN_NAMESPACE_YM_TCLPP

//////////////////////////////////////////////////////////////////////
// クラス TclCmd
//////////////////////////////////////////////////////////////////////

// ClientData から TclCmd へキャストを行う．
// 常に安全な訳ではないので使用は注意する事．
inline
TclCmd*
cmd_obj(ClientData clientData)
{
  return static_cast<TclCmd*>( clientData );
}

// コマンド実行のためのコールバックルーティン
int
cmd_command_callback(ClientData clientData,
		     Tcl_Interp* interp,
		     int objc,
		     Tcl_Obj *const objv[])
{
  TclCmd* cmd = cmd_obj(clientData);
  // 念のため cmd の持っているインタプリタと interp が一致する事を
  // 確かめておく．
  assert_cond( interp == cmd->interp(), __FILE__, __LINE__);

  // TclObj の配列に入れ直す．
  TclObjVector obj_array(objc);
  for (int i = 0; i < objc; i ++) {
    obj_array[i] = objv[i];
  }

  // 本当のコマンド処理関数を呼ぶ．
  int rcode = cmd->_cmd_proc(obj_array);

  return rcode;
}

// オブジェクト削除のためのコールバックルーティン
void
cmd_delete_callback(ClientData clientData)
{
  TclCmd* cmd = cmd_obj(clientData);
  delete cmd;
}

// コンストラクタ
TclCmd::TclCmd() :
  mCmdToken(NULL),
  mAutoHelp(true)
{
}

// デストラクタ
TclCmd::~TclCmd()
{
  for (list<TclPopt*>::iterator p = mPoptList.begin();
       p != mPoptList.end(); ++ p) {
    TclPopt* popt = *p;
    delete popt;
  }
  for (list<TclPoptGroup*>::iterator p = mPoptGroupList.begin();
       p != mPoptGroupList.end(); ++ p) {
    TclPoptGroup* popt_group = *p;
    delete popt_group;
  }
}

// コマンドを登録する．
int
TclCmd::bind(Tcl_Interp* interp,
	     const string& cmd_name)
{
  // インタプリタをセットする．
  set_interp(interp);
  for (list<TclPopt*>::iterator p = mPoptList.begin();
       p != mPoptList.end(); ++ p) {
    TclPopt* popt = *p;
    popt->set_interp(interp);
  }

  // コマンド名をセットする．
  mCmdName = cmd_name;
  // コマンドを登録する．
  StrBuf sptr(mCmdName);
  mCmdToken = Tcl_CreateObjCommand(interp, sptr,
				   cmd_command_callback,
				   reinterpret_cast<ClientData>(this),
				   cmd_delete_callback);
  if ( mCmdToken == NULL ) {
    return TCL_ERROR;
  }
  return TCL_OK;
}

// @brief TclPopt を登録する．
// @param[in] popt 登録するオプション解析用オブジェクト
// @note TclPopt オブジェクトはこのコマンドが削除されるときに一緒に削除される．
void
TclCmd::bind_popt(TclPopt* popt)
{
  mPoptList.push_back(popt);
  string tmp = "-" + popt->opt_str();
  mPoptTable.insert(make_pair(tmp, popt));
}

// @brief help/usage オプションを制御する．
// @param[in] enable true の時，help/usage オプションを有効にする．
void
TclCmd::autohelp(bool enable)
{
  mAutoHelp = enable;
}

// オプションの解析を行う．
int
TclCmd::parse_opt(TclObjVector& objv,
		  bool& help)
{
  help = false;

  // 初期化
  for (list<TclPopt*>::iterator p = mPoptList.begin();
       p != mPoptList.end(); ++ p) {
    TclPopt* popt = *p;
    popt->_init();
  }

  // 読み出し位置
  TclObjVector::iterator rpos = objv.begin();
  // コマンド名をスキップ
  ++ rpos;
  // 書き込み位置
  TclObjVector::iterator wpos = rpos;
  // 終了位置
  TclObjVector::iterator end = objv.end();
  while ( rpos != end ) {
    TclObj obj = *rpos;
    string opt_str = obj;
    if ( opt_str[0] != '-' || opt_str == "--" ) {
      break;
    }
    ++ rpos;

    if ( mAutoHelp ) {
      if ( opt_str == "-help" ) {
	print_help();
	help = true;
	return TCL_OK;
      }
      if ( opt_str == "-usage" ) {
	print_usage();
	help = true;
	return TCL_OK;
      }
    }
    hash_map<string, TclPopt*>::iterator p = mPoptTable.find(opt_str);
    if ( p == mPoptTable.end() ) {
      string emsg = opt_str + ": unknown option";
      set_result(emsg);
      return TCL_ERROR;
    }
    TclPopt* popt = p->second;
    tTclPoptStat stat = popt->_action(opt_str, rpos, end);
    if ( stat == kTclPoptError ) {
      return TCL_ERROR;
    }
    if ( stat == kTclPoptBreak ) {
      break;
    }
  }
  for ( ; rpos != end; ++ rpos, ++ wpos) {
    TclObj obj = *rpos;
    *wpos = obj;
  }
  objv.erase(wpos, end);

  // 排他的オプションのチェック
  for (list<TclPoptGroup*>::iterator p = mPoptGroupList.begin();
       p != mPoptGroupList.end(); ++ p) {
    TclPoptGroup* popt_group = *p;
    if ( popt_group->check() ) {
      const list<TclPopt*>& popt_list = popt_group->popt_list();
      size_t num = popt_list.size();
      string emsg;
      size_t i = 0;
      for (list<TclPopt*>::const_iterator q = popt_list.begin();
	   q != popt_list.end(); ++ q, ++ i) {
	TclPopt* popt = *q;
	if ( i > 0 ) {
	  if ( i < (num - 1) ) {
	    emsg += ", ";
	  }
	  else if ( num > 2 ) {
	    emsg += ", and ";
	  }
	  else {
	    emsg += " and ";
	  }
	}
	emsg += "-" + popt->opt_str();
      }
      emsg += " are exclusive";
      set_result(emsg);
      return TCL_ERROR;
    }
  }

  return TCL_OK;
}

// デフォルトの前処理関数
int
TclCmd::before_cmd_proc(TclObjVector& objv)
{
  return TCL_OK;
}

// デフォルトの終了関数
void
TclCmd::after_cmd_proc()
{
}

// コマンドの開始を記録する．
void
TclCmd::rec_cmd_start(const TclObjVector&)
{
}

// コマンドの終了を記録する．
void
TclCmd::rec_cmd_end(int)
{
}

BEGIN_NONAMESPACE

// @brief print_help の下請け関数
void
print_opt(ostream& s,
	  const string& opt_str,
	  const string& arg_desc,
	  const string& opt_desc)
{
  s << "  -" << opt_str;
  size_t len = opt_str.size();
  if ( arg_desc != string() ) {
    s << "=" << arg_desc;
    len += arg_desc.size() + 1;
  }
  if ( len < 20 ) {
    for (size_t i = len; i < 20; ++ i) {
      s << " ";
    }
  }
  else {
    s << endl << "                       ";
  }
  s << opt_desc << endl;
}

END_NONAMESPACE

// help オプションがあったときに呼ばれる仮想関数
void
TclCmd::print_help()
{
  bool has_options = !mPoptList.empty();
  ostringstream buf;
  buf << "Usage: " << arg0() << " ";
  if ( has_options ) {
    buf << "?OPTIONS*? ";
  }
  buf << usage_string() << endl;
  if ( has_options ) {
    buf << endl
	<< "Options:" << endl;
  }
  for (list<TclPopt*>::iterator p = mPoptList.begin();
       p != mPoptList.end(); ++ p) {
    TclPopt* popt = *p;
    print_opt(buf, popt->opt_str(), popt->arg_desc(), popt->opt_desc());
  }
  if ( mAutoHelp ) {
    buf << endl
	<< "Help options:" << endl;
    print_opt(buf, "help", string(), "Show this help message");
    print_opt(buf, "usage", string(), "Display brief usage message");
  }
  set_result(buf.str());
}

// usage オプションがあったときに呼ばれる仮想関数
void
TclCmd::print_usage()
{
  ostringstream buf;
  buf << "Usage: " << arg0();
  for (list<TclPopt*>::iterator p = mPoptList.begin();
       p != mPoptList.end(); ++ p) {
    TclPopt* popt = *p;
    buf << " [-" << popt->opt_str();
    if ( popt->arg_desc() != string() ) {
      buf << " " << popt->arg_desc();
    }
    buf << "]";
  }
  if ( mAutoHelp ) {
    buf << " [-help] [-usage]";
  }
  buf << " " << usage_string();
  set_result(buf.str());
}

// @brief Popt グループを生成する．
// @param[in] popt1, popt2, popt3, popt4, popt5, popt6 グループの要素
// @return 生成した TclPoptGroup を返す．
// @note 後で TclPoptGroup::add() で要素を追加することもできる．
TclPoptGroup*
TclCmd::new_popt_group(TclPopt* popt1,
		       TclPopt* popt2,
		       TclPopt* popt3,
		       TclPopt* popt4,
		       TclPopt* popt5,
		       TclPopt* popt6)
{
  TclPoptGroup* pg = new TclPoptGroup;
  if ( popt1 ) {
    pg->add(popt1);
  }
  if ( popt2 ) {
    pg->add(popt2);
  }
  if ( popt3 ) {
    pg->add(popt3);
  }
  if ( popt4 ) {
    pg->add(popt4);
  }
  if ( popt5 ) {
    pg->add(popt5);
  }
  if ( popt6 ) {
    pg->add(popt6);
  }
  mPoptGroupList.push_back(pg);
  return pg;
}

// @brief Popt グループに要素を追加する．
// @param[in] group new_popt_group() で作成した TclPoptGroup
// @param[in] popt 追加する TclPopt
void
TclCmd::add_popt(TclPoptGroup* group,
		 TclPopt* popt)
{
  group->add(popt);
}

// コマンド処理関数
int
TclCmd::_cmd_proc(TclObjVector& objv)
{
  // mArg0 にセットする．
  mArg0 = objv[0];

  // コマンドの開始を記録する．
  rec_cmd_start(objv);

  // 実行結果をクリアする．
  reset_result();

  // オプションの解析を行う．
  bool help;
  int rcode = parse_opt(objv, help);
  if ( help ) {
    return rcode;
  }
  if ( rcode == TCL_OK ) {
    // プレコールバック関数を呼ぶ
    rcode = before_cmd_proc(objv);
    if ( rcode == TCL_OK ) {
      // 本当のコールバック関数を呼ぶ．
      rcode = cmd_proc(objv);
      // 今の処理結果を保存しておく．
      // 間違えると結構，ヤバイコード
      TclObj result_obj = result();

      // 終了関数を呼ぶ．
      after_cmd_proc();

      // 処理結果をもとに戻す．
      reset_result();
      set_result(result_obj);
    }
    else {
      // 途中でエラーになっても終了関数を呼ぶ．
      after_cmd_proc();
    }
  }

  // mArg0 を無効化しておく．
  mArg0 = "";

  // コマンドの終了を記録する．
  rec_cmd_end(rcode);

  return rcode;
}


//////////////////////////////////////////////////////////////////////
// クラス TclCmdCls
//////////////////////////////////////////////////////////////////////

// オブジェクトを生成するためのコマンド
int
TclCmdCls::cmd_proc(TclObjVector& objv)
{
  if ( objv.size() < 2 ) {
    set_result("illegal # of args.");
    return TCL_ERROR;
  }

  // オブジェクト名は objv[1]
  string obj_name = objv[1];

  TclCmd* obj = create_obj();
  if ( obj == NULL ) {
    // オブジェクトが生成できなかった．
    set_result("Can not create new object.");
    return TCL_ERROR;
  }
  // コマンドの登録を行なう．
  int result = obj->bind(interp(), obj_name);

  return result;
}

END_NAMESPACE_YM_TCLPP
