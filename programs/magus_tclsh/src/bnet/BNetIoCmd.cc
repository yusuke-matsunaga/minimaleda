
/// @file magus/logbase/BNetIoCmd.cc
/// @brief BNetIoCmd の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: BNetIoCmd.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "BNetIoCmd.h"

#include "ym_bnet/BNetBlifReader.h"
#include "ym_bnet/BNetBlifWriter.h"
#include "ym_bnet/BNetEquWriter.h"
#include "ym_bnet/BNetVerilogWriter.h"
#include "ym_utils/FileRegion.h"


BEGIN_NAMESPACE_MAGUS

//////////////////////////////////////////////////////////////////////
// TclObj に出力するメッセージハンドラ
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
TclObjMsgHandler::TclObjMsgHandler()
{
}

// @brief デストラクタ
TclObjMsgHandler::~TclObjMsgHandler()
{
}

// @brief メッセージが登録されるたびに呼ばれる仮想関数
// @param[in] src_file この関数を読んでいるソースファイル名
// @param[in] src_line この関数を読んでいるソースの行番号
// @param[in] loc ファイル位置
// @param[in] label メッセージラベル
// @param[in] body メッセージ本文
void
TclObjMsgHandler::put_msg(const char* src_file,
			  int src_line,
			  const FileRegion& loc,
			  tMsgType type,
			  const char* label,
			  const char* body)
{
  ostringstream buf;
  buf << loc << type << " [" << label << "]: " << body << endl;
  mMsg << buf.str();
}

// @brief 内容をクリアする．
void
TclObjMsgHandler::clear()
{
  mMsg.clear();
}

// @brief メッセージオブジェクトを取り出す．
TclObj
TclObjMsgHandler::msg_obj() const
{
  return mMsg;
}


//////////////////////////////////////////////////////////////////////
// blif 形式のファイルを読み込むコマンド
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
ReadBlif::ReadBlif(MagMgr* mgr) :
  BNetCmd(mgr)
{
  mReader.add_msg_handler(&mMsgHandler);
  set_usage_string("<filename>");
}

// @brief デストラクタ
ReadBlif::~ReadBlif()
{
}

// コマンドを実行する．
int
ReadBlif::cmd_proc(TclObjVector& objv)
{
  size_t objc = objv.size();

  // このコマンドはファイル名を引数としてとる．
  if ( objc != 2 ) {
    print_usage();
    return TCL_ERROR;
  }

  BNetwork* network = cur_network();

  // コマンド行を解析してファイル名を取って来て，ファイルを開く
  string file_name = objv[1];
  // ファイル名の展開を行う．
  string ex_file_name;
  bool stat = tilde_subst(file_name, ex_file_name);
  if ( !stat ) {
    // ファイル名文字列の中に誤りがあった．
    return TCL_ERROR;
  }

  // 実際の読み込みを行う．
  try {
#if defined(USE_LIBCELL)
    const CellMgr& library = CurLibrary();
    bool result = network.ReadBlif(fp, stderr, &library);

    // エラーが起きていないか調べる．
    if ( !result ) {
      TclObj emsg;
      emsg << "Error occurred in reading " << objv[1];
      set_result(emsg);
      return TCL_ERROR;
    }
#else
    bool result = mReader.read(ex_file_name, *network);

    // エラーが起きていないか調べる．
    if ( !result ) {
      TclObj emsg = mMsgHandler.msg_obj();
      mMsgHandler.clear();
      emsg << "Error occurred in reading " << objv[1];
      set_result(emsg);
      return TCL_ERROR;
    }
#endif
  }
  catch ( AssertError x ) {
    ostringstream buf;
    buf << x << endl;
    TclObj emsg;
    emsg << "assert error: " << buf.str();
    set_result(emsg);
    return TCL_ERROR;
  }

  // 正常終了
  return TCL_OK;
}


//////////////////////////////////////////////////////////////////////
// iscas89 形式のファイルを読み込むコマンド
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
ReadIscas89::ReadIscas89(MagMgr* mgr) :
  BNetCmd(mgr)
{
  mReader.add_msg_handler(&mMsgHandler);
  set_usage_string("<filename>");
}

// @brief デストラクタ
ReadIscas89::~ReadIscas89()
{
}

// コマンドを実行する．
int
ReadIscas89::cmd_proc(TclObjVector& objv)
{
  size_t objc = objv.size();

  // このコマンドはファイル名を引数としてとる．
  if ( objc != 2 ) {
    print_usage();
    return TCL_ERROR;
  }

  BNetwork* network = cur_network();

  // コマンド行を解析してファイル名を取って来て，ファイルを開く
  string file_name = objv[1];
  // ファイル名の展開を行う．
  string ex_file_name;
  bool stat = tilde_subst(file_name, ex_file_name);
  if ( !stat ) {
    // ファイル名の文字列中に誤りがあった．
    return TCL_ERROR;
  }

  // 実際の読み込みを行う．
  try {
    bool result = mReader.read(ex_file_name, *network);

    // エラーが起きていないか調べる．
    if ( !result ) {
      TclObj emsg = mMsgHandler.msg_obj();
      mMsgHandler.clear();
      emsg << "Error occurred in reading " << objv[1];
      set_result(emsg);
      return TCL_ERROR;
    }
  }
  catch ( AssertError x ) {
    ostringstream buf;
    buf << x << endl;
    TclObj emsg;
    emsg << "assert error: " << buf.str();
    set_result(emsg);
    return TCL_ERROR;
  }

  // 正常終了
  return TCL_OK;
}


//////////////////////////////////////////////////////////////////////
// ファイルに blif 形式で書き出すコマンド
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
WriteBlif::WriteBlif(MagMgr* mgr) :
  BNetCmd(mgr)
{
  set_usage_string("?<filename>?");
}

// @brief デストラクタ
WriteBlif::~WriteBlif()
{
}

// コマンドを実行する．
int
WriteBlif::cmd_proc(TclObjVector& objv)
{
  size_t objc = objv.size();

  // このコマンドはファイル名のみを引数に取る．
  // 引数がなければ標準出力に出す．
  bool map = false;
  size_t base = 1;
  if ( objc > 1 && string(objv[1]) == "-n" ) {
    map = true;
    base = 2;
  }

  if ( objc - base > 1 ) {
    print_usage();
    return TCL_ERROR;
  }

  // 出力先のファイルを開く
  ostream* osp = &cout;
  ofstream ofs;
  if ( objc - base == 1 ) {
    string file_name = objv[base];
    if ( !open_ofile(ofs, file_name) ) {
      // ファイルが開けなかった．
      return TCL_ERROR;
    }
    osp = &ofs;
  }

  bool result = false;  // 念のため FALSE を入れておく．

  // 実際の書き出しを行う．
  BNetBlifWriter writer;
  writer.dump(*osp, *(cur_network()));
  // この関数はfailしない．
  result = true;

  // 結果がエラーでないか調べる．今はエラーとはならない．
  if ( !result ) {
    TclObj msg;
    msg << "Error occurred in writing " << objv[base];
    set_result(msg);
    return TCL_ERROR;
  }

  // 正常終了
  return TCL_OK;
}


//////////////////////////////////////////////////////////////////////
// ファイルに EQU 形式で書き出すコマンド
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
WriteEqu::WriteEqu(MagMgr* mgr) :
  BNetCmd(mgr)
{
  set_usage_string("?<filename>?");
}

// @brief デストラクタ
WriteEqu::~WriteEqu()
{
}

// コマンドを実行する．
int
WriteEqu::cmd_proc(TclObjVector& objv)
{
  size_t objc = objv.size();

  // 引き数の数をチェックする．
  if( objc > 2 ){
    print_usage();
    return TCL_ERROR;
  }

  // 出力先のファイルを開く
  ostream* osp = &cout;
  ofstream ofs;
  if ( objc == 2 ) {
    string file_name = objv[1];
    if ( !open_ofile(ofs, file_name) ) {
      // ファイルが開けなかった．
      return TCL_ERROR;
    }
    osp = &ofs;
  }

  bool result = false;  // 念のため FALSE を入れておく．

  // 実際の書き出しを行う．
  BNetEquWriter writer;
  writer.dump(*osp, *(cur_network()));
  // この関数はfailしない．
  result = true;

  // 結果がエラーでないか調べる．今はエラーとはならない．
  if ( !result ) {
    TclObj msg;
    msg << "Error occured in writing " << objv[1];
    set_result(msg);
    return TCL_ERROR;
  }

  // 正常終了
  return TCL_OK;
}


//////////////////////////////////////////////////////////////////////
// ファイルに Verilog 形式で書き出すコマンド
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
WriteVerilog::WriteVerilog(MagMgr* mgr) :
  BNetCmd(mgr)
{
  set_usage_string("?<filename>?");
}

// @brief デストラクタ
WriteVerilog::~WriteVerilog()
{
}

// コマンドを実行する．
int
WriteVerilog::cmd_proc(TclObjVector& objv)
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
    string file_name = objv[1];
    if ( !open_ofile(ofs, file_name) ) {
      // ファイルが開けなかった．
      return TCL_ERROR;
    }
    osp = &ofs;
  }

  bool result = false;  // 念のため FALSE を入れておく．

  // 実際の書き出しを行う．
  BNetVerilogWriter writer;
  writer.dump(*osp, *(cur_network()));
  // この関数はfailしない．
  result = true;

  // 結果がエラーでないか調べる．今はエラーとはならない．
  if ( !result ) {
    TclObj emsg;
    emsg << "Error occurred in writing " << objv[1];
    set_result(emsg);
    return TCL_ERROR;
  }

  // 正常終了
  return TCL_OK;
}

END_NAMESPACE_MAGUS
