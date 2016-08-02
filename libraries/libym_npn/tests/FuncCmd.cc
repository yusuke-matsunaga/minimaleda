
/// @file libym_npn/tests/FuncCmd.cc
/// @brief FuncCmd の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: FuncCmd.cc 700 2007-05-31 00:41:30Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_utils/random.h"
#include "ym_npn/TvFunc.h"

#include "TvFuncConv.h"
#include "FuncCmd.h"


BEGIN_NAMESPACE_YM_NPN

//////////////////////////////////////////////////////////////////////
/// 全ての関数を列挙するコマンド
//////////////////////////////////////////////////////////////////////

// usage を出力する関数
void
ForallFuncCmd::usage()
{
  const char* usage =
    "USAGE: forall_func #inputs var-name command";
  set_result(usage);
}

// 全ての関数を列挙するコマンド
int
ForallFuncCmd::cmd_proc(TclObjVector& objv)
{
  if ( objv.size() != 4 ) {
    usage();
    return TCL_ERROR;
  }

  // 入力数の取り出し
  unsigned int ni;
  if ( uint_conv(objv[1], ni) != TCL_OK ) {
    return TCL_ERROR;
  }

  // 関数のパタンを格納する変数名の取り出し
  const TclObj& varname = objv[2];
  
  // 実行すべきコマンドの取り出し
  const TclObj& cmd = objv[3];

  size_t ni_exp = 1 << ni;
  vector<int> vec(ni_exp);
  for (size_t i = 0; i < ni_exp; ++ i) {
    vec[i] = 0;
  }

  int result = TCL_OK;
  for ( ; ; ) {
    // 関数のパタンを変数にセットする．
    TclObj func_obj = TvFuncConv::to_tclobj(ni, vec);
    set_var(varname, func_obj, 0);
    
    // コマンドを実行する．
    int code = eval(cmd);

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

    bool carry = false;
    for (size_t i = 0; i < ni_exp; ++ i) {
      if ( vec[i] == 0 ) {
	vec[i] = 1;
	break;
      }
      vec[i] = 0;
      if ( i == ni_exp - 1 ) {
	carry = true;
      }
    }
    if ( carry ) {
      break;
    }
  }
  
  return result;
}


//////////////////////////////////////////////////////////////////////
/// ランダムに関数を生成するコマンド
//////////////////////////////////////////////////////////////////////

// @brief usage を出力する関数
void
RandomFuncCmd::usage()
{
  const char* usage =
    "USAGE: ramdom_func ?-s|--seed random-seed? ?-n|--num #funcs?\n"
    "       #inputs var-name command";
  set_result(usage);
}
  
// @brief コマンドを実行する仮想関数
int
RandomFuncCmd::cmd_proc(TclObjVector& objv)
{
  // 繰り返し数のデフォルト値
  unsigned int n = 10000;

  // オプションの解析
  TclObjVector::iterator end = objv.end();
  TclObjVector::iterator rpos = objv.begin() + 1; // コマンド名をスキップ
  TclObjVector::iterator wpos = rpos;
  for ( ; rpos != end; ++ rpos) {
    TclObj obj = *rpos;
    string arg = obj;
    if ( arg == "-s" || arg == "--seed" ) {
      ++ rpos;
      if ( rpos == end ) {
	set_result("-s|--seed needs a seed number(unsigned long)");
	return TCL_ERROR;
      }
      unsigned long seed;
      int ret = ulong_conv(*rpos, seed);
      if ( ret != TCL_OK ) {
	return ret;
      }
      init_random_seed(seed);
      continue;
    }
    if ( arg == "-n" || arg == "--num" ) {
      ++ rpos;
      if ( rpos == end ) {
	set_result("-n|--num needs a number(unsigned long)");
	return TCL_ERROR;
      }
      int ret = uint_conv(*rpos, n);
      if ( ret != TCL_OK ) {
	return ret;
      }
      continue;
    }
    // オプションでない引数はそのまま残す
    if ( wpos != rpos ) {
      *wpos = obj;
    }
    ++ wpos;
  }
  objv.erase(wpos, end);

  if ( objv.size() != 4 ) {
    usage();
    return TCL_ERROR;
  }

  // 入力数の取り出し
  unsigned int ni;
  if ( uint_conv(objv[1], ni) != TCL_OK ) {
    return TCL_ERROR;
  }

  // 関数のパタンを格納する変数名の取り出し
  const TclObj& varname = objv[2];
  
  // 実行すべきコマンドの取り出し
  const TclObj& cmd = objv[3];

  size_t ni_exp = 1 << ni;
  vector<int> vec(ni_exp);

  int result = TCL_OK;
  for (size_t j = 0; j < n; ++ j) {
    for (size_t i = 0; i < ni_exp; ++ i) {
      if ( random_num() & 1 ) {
	vec[i] = 1;
      }
      else {
	vec[i] = 0;
      }
    }
    // 関数のパタンを変数にセットする．
    TclObj func_obj = TvFuncConv::to_tclobj(ni, vec);
    set_var(varname, func_obj, 0);

    // コマンドを実行する．
    int code = eval(cmd);

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


//////////////////////////////////////////////////////////////////////
/// ビットベクタ形式のファイルから関数を生成するコマンド
//////////////////////////////////////////////////////////////////////

// @brief usage を出力する関数
void
ReadFuncCmd::usage()
{
  const char* usage =
    "USAGE: read_func ?-f|--file file-name? var-name command";
  set_result(usage);
}
  
// @brief コマンドを実行する仮想関数
int
ReadFuncCmd::cmd_proc(TclObjVector& objv)
{
  istream* is = NULL;
  ifstream* ifs = NULL;

  // オプションの解析
  TclObjVector::iterator end = objv.end();
  TclObjVector::iterator rpos = objv.begin() + 1; // コマンド名をスキップ
  TclObjVector::iterator wpos = rpos;
  for ( ; rpos != end; ++ rpos) {
    TclObj obj = *rpos;
    string arg = obj;
    if ( arg == "-f" || arg == "--file" ) {
      if ( is != NULL ) {
	set_result("-f|--file: appear more than once");
	return TCL_ERROR;
      }
      ++ rpos;
      if ( rpos == end ) {
	set_result("-f|--file needs a file name");
	return TCL_ERROR;
      }
      string fname = *rpos;
      ifs = new ifstream;
      ifs->open(fname.c_str());
      if ( !(*ifs) ) {
	delete ifs;
	TclObj emsg;
	emsg << fname << ": Could not open";
	set_result(emsg);
	return TCL_ERROR;
      }
      is = ifs;
      continue;
    }

    // オプションでない引数はそのまま残す
    if ( wpos != rpos ) {
      *wpos = obj;
    }
    ++ wpos;
  }
  objv.erase(wpos, end);

  if ( objv.size() != 3 ) {
    usage();
    return TCL_ERROR;
  }

  // 関数のパタンを格納する変数名の取り出し
  const TclObj& varname = objv[1];
  
  // 実行すべきコマンドの取り出し
  const TclObj& cmd = objv[2];

  if ( is == NULL ) {
    is = &cin;
  }

  size_t maxni = 20;
  size_t ni_exp = 1UL << maxni;
  size_t buff_size = ni_exp + 10;
  char* buff = new char[buff_size + 1];

  int result = TCL_OK;
  while ( is->peek() != EOF ) {
    is->getline(buff, buff_size);

    // 関数のパタンを変数にセットする．
    set_var(varname, buff, 0);
    
    // コマンドを実行する．
    int code = eval(cmd);

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

  delete [] buff;

  delete ifs;

  return result;
}


//////////////////////////////////////////////////////////////////////
/// 関数のパタンを出力するコマンド
//////////////////////////////////////////////////////////////////////

// @brief usage を出力する関数
void
DumpFuncCmd::usage()
{
}
  
// @brief コマンドを実行する仮想関数
int
DumpFuncCmd::cmd_proc(TclObjVector& objv)
{
  int mode = 2;

  // オプションの解析
  TclObjVector::iterator end = objv.end();
  TclObjVector::iterator rpos = objv.begin() + 1; // コマンド名をスキップ
  TclObjVector::iterator wpos = rpos;
  for ( ; rpos != end; ++ rpos) {
    TclObj obj = *rpos;
    string arg = obj;
    if ( arg == "-h" || arg == "--hex" ) {
      mode = 16;
      continue;
    }
    if ( arg == "-b" || arg == "--bin" ) {
      mode = 2;
      continue;
    }

    // オプションでない引数はそのまま残す
    if ( wpos != rpos ) {
      *wpos = obj;
    }
    ++ wpos;
  }
  objv.erase(wpos, end);

  if ( objv.size() != 2 ) {
    usage();
    return TCL_ERROR;
  }

  TvFunc func;
  int code = TvFuncConv::from_tclobj(objv[1], interp(), func);
  if ( code != TCL_OK ) {
    return code;
  }

  ostringstream buf;
  func.dump(buf, mode);
  string tmp = buf.str();

  set_result(tmp);
  
  return TCL_OK;
}

END_NAMESPACE_YM_NPN
