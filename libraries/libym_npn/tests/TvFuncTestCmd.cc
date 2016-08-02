
/// @file libym_npn/tests/TvFuncTestCmd.cc
/// @brief TvFuncTestCmd の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: TvFuncTestCmd.cc 700 2007-05-31 00:41:30Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_npn/TvFunc.h"

#include "TvFuncTestCmd.h"
#include "TvFuncConv.h"


BEGIN_NAMESPACE_YM_NPN

// @brief コンストラクタ
TvFuncTestCmd::TvFuncTestCmd() :
  test(cout)
{
}

// @brief デストラクタ
TvFuncTestCmd::~TvFuncTestCmd()
{
}

// @brief usage を出力する関数
void
TvFuncTestCmd::usage()
{
  const char* usage =
    "USAGE: tvfunc_test base0 <ni>\n"
    "                   base1 <func>\n"
    "                   base2 <func>\n"
    "                   count_one <func>\n"
    "                   walsh_0 <func>\n"
    "                   walsh_1 <func> <pos>\n"
    "                   walsh_2 <func> <pos1> <pos2>\n"
    "                   walsh_01 <func>\n"
    "                   walsh_012 <func>\n"
    "                   check_sup <func> <pos>\n"
    "                   check_sym <func> <pos1> <pos2>\n"
    "                   walsh_w0 <func> <ibits>\n"
    "                   walsh_w1 <func> <pos> <ibits>\n";
  set_result(usage);
}

// @brief コマンドを実行する仮想関数
int
TvFuncTestCmd::cmd_proc(TclObjVector& objv)
{
  if ( objv.size() < 2 ) {
    usage();
    return TCL_ERROR;
  }

  string subcmd = objv[1];
  if ( subcmd == "base0" ) {
    if ( objv.size() != 3 ) {
      usage();
      return TCL_ERROR;
    }

    unsigned int ni;
    int code = uint_conv(objv[2], ni);
    if ( code != TCL_OK ) {
      return code;
    }

    test.check0(ni);
    return TCL_OK;
  }

  if ( subcmd == "base1" ) {
    if ( objv.size() != 3 ) {
      usage();
      return TCL_ERROR;
    }

    TvFunc func;
    int code = get_tvfunc(objv[2], func);
    if ( code != TCL_OK ) {
      return code;
    }

    size_t ni = func.ni();
    size_t ni_pow = 1UL << ni;
    vector<int> vect(ni_pow);
    for (size_t i = 0; i < ni_pow; ++ i) {
      vect[i] = func.value(i);
    }

    test.check_base(ni, vect);
    return TCL_OK;
  }

  if ( subcmd == "base2" ) {
    if ( objv.size() != 3 ) {
      usage();
      return TCL_ERROR;
    }

    TvFunc func;
    int code = get_tvfunc(objv[2], func);
    if ( code != TCL_OK ) {
      return code;
    }

    test.check_base2(func);
    return TCL_OK;
  }

  if ( subcmd == "count_one" ) {
    if ( objv.size() != 3 ) {
      usage();
      return TCL_ERROR;
    }

    TvFunc func;
    int code = get_tvfunc(objv[2], func);
    if ( code != TCL_OK ) {
      return code;
    }

    test.check_count_one(func);
    return TCL_OK;
  }

  if ( subcmd == "walsh_0" ) {
    if ( objv.size() != 3 ) {
      usage();
      return TCL_ERROR;
    }

    TvFunc func;
    int code = get_tvfunc(objv[2], func);
    if ( code != TCL_OK ) {
      return code;
    }

    test.check_walsh_0(func);
    return TCL_OK;
  }

  if ( subcmd == "walsh_1" ) {
    if ( objv.size() != 4 ) {
      usage();
      return TCL_ERROR;
    }

    TvFunc func;
    int code = get_tvfunc(objv[2], func);
    if ( code != TCL_OK ) {
      return code;
    }
    
    unsigned int pos;
    code = get_pos(objv[3], func.ni(), pos);
    if ( code != TCL_OK ) {
      return code;
    }

    test.check_walsh_1(func, pos);
    return TCL_OK;
  }

  if ( subcmd == "walsh_2" ) {
    if ( objv.size() != 5 ) {
      usage();
      return TCL_ERROR;
    }

    TvFunc func;
    int code = get_tvfunc(objv[2], func);
    if ( code != TCL_OK ) {
      return code;
    }

    unsigned int pos1;
    code = get_pos(objv[3], func.ni(), pos1);
    if ( code != TCL_OK ) {
      return code;
    }

    unsigned int pos2;
    code = get_pos(objv[4], func.ni(), pos2);
    if ( code != TCL_OK ) {
      return code;
    }

    test.check_walsh_2(func, pos1, pos2);
    return TCL_OK;
  }

  if ( subcmd == "walsh_01" ) {
    if ( objv.size() != 3 ) {
      usage();
      return TCL_ERROR;
    }

    TvFunc func;
    int code = get_tvfunc(objv[2], func);
    if ( code != TCL_OK ) {
      return code;
    }

    test.check_walsh_01(func);
    return TCL_OK;
  }

  if ( subcmd == "walsh_012" ) {
    if ( objv.size() != 3 ) {
      usage();
      return TCL_ERROR;
    }

    TvFunc func;
    int code = get_tvfunc(objv[2], func);
    if ( code != TCL_OK ) {
      return code;
    }

    test.check_walsh_012(func);
    return TCL_OK;
  }

  if ( subcmd == "check_sup" ) {
    if ( objv.size() != 4 ) {
      usage();
      return TCL_ERROR;
    }

    TvFunc func;
    int code = get_tvfunc(objv[2], func);
    if ( code != TCL_OK ) {
      return code;
    }

    unsigned int pos;
    code = get_pos(objv[3], func.ni(), pos);
    if ( code != TCL_OK ) {
      return code;
    }

    test.check_check_sup(func, pos);
    return TCL_OK;
  }

  if ( subcmd == "check_sym" ) {
    if ( objv.size() != 5 ) {
      usage();
      return TCL_ERROR;
    }

    TvFunc func;
    int code = get_tvfunc(objv[2], func);
    if ( code != TCL_OK ) {
      return code;
    }

    unsigned int pos1;
    code = get_pos(objv[3], func.ni(), pos1);
    if ( code != TCL_OK ) {
      return code;
    }

    unsigned int pos2;
    code = get_pos(objv[4], func.ni(), pos2);
    if ( code != TCL_OK ) {
      return code;
    }

    test.check_check_sym(func, pos1, pos2);
    return TCL_OK;
  }

  if ( subcmd == "walsh_w0" ) {
    if ( objv.size() != 4 ) {
      usage();
      return TCL_ERROR;
    }

    TvFunc func;
    int code = get_tvfunc(objv[2], func);
    if ( code != TCL_OK ) {
      return code;
    }

    unsigned long ibits;
    code = ulong_conv(objv[3], ibits);
    if ( code != TCL_OK ) {
      return code;
    }

    test.check_walsh_w0(func, ibits);
    return TCL_OK;
  }

  if ( subcmd == "walsh_w1" ) {
    if ( objv.size() != 5 ) {
      usage();
      return TCL_ERROR;
    }

    TvFunc func;
    int code = get_tvfunc(objv[2], func);
    if ( code != TCL_OK ) {
      return code;
    }

    unsigned int pos1;
    code = get_pos(objv[3], func.ni(), pos1);
    if ( code != TCL_OK ) {
      return code;
    }

    unsigned long ibits;
    code = ulong_conv(objv[4], ibits);
    if ( code != TCL_OK ) {
      return code;
    }

    test.check_walsh_w1(func, pos1, ibits);
    return TCL_OK;
  }

  usage();
  return TCL_ERROR;
}

// 関数を取り出す．
int
TvFuncTestCmd::get_tvfunc(TclObj& obj,
			  TvFunc& func)
{
  int code = TvFuncConv::from_tclobj(obj, interp(), func);
  return code;
 }

// 入力番号を取り出す．
int
TvFuncTestCmd::get_pos(TclObj& obj,
		       size_t ni,
		       unsigned int& pos)
{
  int code = uint_conv(obj, pos);
  if ( code != TCL_OK ) {
    return code;
  }

  if ( pos >= ni ) {
    ostringstream buf;
    buf << pos << " >= func.ni()";
    set_result(buf.str());
    return TCL_ERROR;
  }
  return TCL_OK;
}

END_NAMESPACE_YM_NPN
