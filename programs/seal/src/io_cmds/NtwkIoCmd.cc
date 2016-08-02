
/// @file src/io_cmds/NtwkIoCmd.cc
/// @brief TgNetwork 操作関連のコマンド
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: NtwkIoCmd.cc 1978 2009-02-06 12:29:16Z matsunaga $
///
/// Copyright (C) 2005-2006 Yusuke Matsunaga
/// All rights reserved.


#if HAVE_CONFIG_H
#include "seal_config.h"
#endif

#include "NtwkIoCmd.h"
#include <ym_tgnet/TgNetwork.h>
#include "SealMsgHandler.h"


BEGIN_NAMESPACE_YM_SEAL

//////////////////////////////////////////////////////////////////////
// blif 形式のファイルを読み込むコマンド
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
ReadBlif::ReadBlif(SealMgr* mgr) :
  SealCmd(mgr),
  mMsgHandler(new SealMsgHandler)
{
  set_usage_string("filename");
  mReader.add_msg_handler(mMsgHandler);
}

// @brief デストラクタ
ReadBlif::~ReadBlif()
{
  // mMsgHandler は BlifParser(TgBlifReader) が責任をもって破壊してくれる．
}
  
// コマンド処理関数
int
ReadBlif::cmd_proc(TclObjVector& objv)
{
  size_t objc = objv.size();

  // このコマンドはファイル名を引数としてとる．
  if ( objc != 2 ) {
    print_usage();
    return TCL_ERROR;
  }

  string filename = objv[1];
  // ファイル名の展開を行う．
  string ex_filename;
  bool stat1 = tilde_subst(filename, ex_filename);
  if ( !stat1 ) {
    // ファイル名の文字列に誤りがあった．
    return TCL_ERROR;
  }

  bool stat = mReader(filename, _network());
  if ( !stat ) {
    set_result(mMsgHandler->msg_obj());
    return TCL_ERROR;
  }

  after_set_network();
  return TCL_OK;
}


//////////////////////////////////////////////////////////////////////
// ISCAS89 形式のファイルを読み込むコマンド
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
ReadIscas89::ReadIscas89(SealMgr* mgr) :
  SealCmd(mgr),
  mMsgHandler(new SealMsgHandler)
{
  set_usage_string("filename");
  mReader.add_msg_handler(mMsgHandler);
}

// @brief デストラクタ
ReadIscas89::~ReadIscas89()
{
  // mMsgHandler は Iscas89Parser(TgIscas89Reader) が責任をもって破壊してくれる．
}
  
// コマンド処理関数
int
ReadIscas89::cmd_proc(TclObjVector& objv)
{
  size_t objc = objv.size();

  // このコマンドはファイル名を引数としてとる．
  if ( objc != 2 ) {
    print_usage();
    return TCL_ERROR;
  }

  string filename = objv[1];
  // ファイル名の展開を行う．
  string ex_filename;
  bool stat1 = tilde_subst(filename, ex_filename);
  if ( !stat1 ) {
    // ファイル名の文字列に誤りがあった．
    return TCL_ERROR;
  }

  bool stat = mReader(filename, _network());
  if ( !stat ) {
    set_result(mMsgHandler->msg_obj());
    return TCL_ERROR;
  }
  
  after_set_network();
  return TCL_OK;
}


//////////////////////////////////////////////////////////////////////
// TgNetwork の内容を出力するコマンド
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
WriteNetwork::WriteNetwork(SealMgr* mgr) :
  SealCmd(mgr)
{
  set_usage_string("?filename?");
}
  
// コマンド処理関数
int
WriteNetwork::cmd_proc(TclObjVector& objv)
{
  size_t objc = objv.size();

  // このコマンドはファイル名のみを引数に取る．
  // 引数がなければ標準出力に出す．
  if ( objc > 2 ) {
    print_usage();
    return TCL_ERROR;
  }

  // 出力先のファイルを開く
  ostream* osp = &cout;
  ofstream ofs;
  if ( objc == 2 ) {
    string filename = objv[1];
    if ( !open_ofile(ofs, filename) ) {
      // ファイルが開けなかった．
      return TCL_ERROR;
    }
    osp = &ofs;
  }

  dump(*osp, _network());

  return TCL_OK;
}

// ネットワーク関係のコマンドを作る．
int
make_ntwk_io_commands(Tcl_Interp* interp,
		      SealMgr* mgr,
		      const string& ns)
{
  string tmp;

  tmp = ns + "::" + "read_blif";
  TclCmdBinder1<ReadBlif, SealMgr*>::reg(interp, mgr, tmp);

  tmp = ns + "::" + "read_iscas89";
  TclCmdBinder1<ReadIscas89, SealMgr*>::reg(interp, mgr, tmp);

  tmp = ns + "::" + "write_network";
  TclCmdBinder1<WriteNetwork, SealMgr*>::reg(interp, mgr, tmp);

  
  //////////////////////////////////////////////////////////////////////
  // tclreadline 用の処理
  // すごい手抜き
  //////////////////////////////////////////////////////////////////////
  ostringstream buf;
  buf << "namespace eval tclreadline {" << endl
      << "namespace eval " << ns << " {" << endl
      << "proc complete(read_blif) { t s e l p m } { return \"\" }" << endl
      << "proc complete(read_iscas89) { t s e l p m } { return \"\" }" << endl
      << "proc complete(write_network) { t s e l p m } { return \"\" }" << endl
      << "}" << endl
      << "}" << endl;
  string str = buf.str();
  if ( Tcl_Eval(interp, str.c_str()) == TCL_ERROR ) {
    return TCL_ERROR;
  }
  return TCL_OK;
}

END_NAMESPACE_YM_SEAL
