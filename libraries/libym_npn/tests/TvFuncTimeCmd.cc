/// @file libym_npn/tests/TvFuncTimeCmd.cc
/// @brief TvFuncTimeCmd の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: TvFuncTimeCmd.cc 1508 2008-06-30 04:55:42Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_utils/StopWatch.h"
#include "ym_npn/TvFunc.h"

#include "TvFuncTimeCmd.h"
#include "TvFuncConv.h"


BEGIN_NAMESPACE_YM_NPN

extern
void cannonical(const TvFunc& func,
		NpnMap& map);

// @brief コンストラクタ
TvFuncTimeCmd::TvFuncTimeCmd()
{
}

// @brief デストラクタ
TvFuncTimeCmd::~TvFuncTimeCmd()
{
}

// @brief usage を出力する関数
void
TvFuncTimeCmd::usage()
{
  const char* usage =
    "USAGE: tvfunc_time ?-mag n? count_one <func>\n"
    "                   ?-mag n? walsh_0 <func>\n"
    "                   ?-mag n? walsh_1 <func> <pos>\n"
    "                   ?-mag n? walsh_2 <func> <pos1> <pos2>\n"
    "                   ?-mag n? walsh_01 <func>\n"
    "                   ?-mag n? walsh_012 <func>\n"
    "                   ?-mag n? check_sup <func> <pos>\n"
    "                   ?-mag n? check_sym <func> <pos1> <pos2>\n"
    "                   ?-mag n? walsh_w0 <func> <w> <ibits>\n"
    "                   ?-mag n? walsh_w1 <func> <pos> <w> <ibits>\n"
    "                   get_time\n"
    "                   reset";
  set_result(usage);
}

// @brief コマンドを実行する仮想関数
int
TvFuncTimeCmd::cmd_proc(TclObjVector& objv)
{
  StopWatch sw;
  
  unsigned int mag = 1;

  // オプションの解析
  TclObjVector::iterator end = objv.end();
  TclObjVector::iterator rpos = objv.begin() + 1; // コマンド名をスキップ
  TclObjVector::iterator wpos = rpos;
  for ( ; rpos != end; ++ rpos) {
    TclObj obj = *rpos;
    string arg = obj;
    if ( arg == "-mag" ) {
      ++ rpos;
      if ( rpos == end ) {
	set_result("-mag needs a number");
	return TCL_ERROR;
      }
      
      int code = uint_conv(*rpos, mag);
      if ( code != TCL_OK ) {
	return code;
      }
      if ( mag == 0 ) {
	mag = 1;
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

  if ( objv.size() < 2 ) {
    usage();
    return TCL_ERROR;
  }

  string subcmd = objv[1];

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

    sw.start();
    for (size_t m = 0; m < mag; ++ m) {
      func.count_one();
    }
    sw.stop();

    USTime time = sw.time();
    mAccTime += time;
    mNum += mag;

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

    sw.start();
    for (size_t m = 0; m < mag; ++ m) {
      func.walsh_0();
    }
    sw.stop();

    USTime time = sw.time();
    mAccTime += time;
    mNum += mag;
    
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

    sw.start();
    for (size_t m = 0; m < mag; ++ m) {
      func.walsh_1(pos);
    }
    sw.stop();

    USTime time = sw.time();
    mAccTime += time;
    mNum += mag;

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

    sw.start();
    for (size_t m = 0; m < mag; ++ m) {
      func.walsh_2(pos1, pos2);
    }
    sw.stop();

    USTime time = sw.time();
    mAccTime += time;
    mNum += mag;

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

    int* vec = new int[func.ni()];

    sw.start();
    for (size_t m = 0; m < mag; ++ m) {
      func.walsh_01(vec);
    }
    sw.stop();

    delete [] vec;

    USTime time = sw.time();
    mAccTime += time;
    mNum += mag;

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

    int* vec1 = new int[func.ni()];
    int* vec2 = new int[func.ni() * func.ni()];
    
    sw.start();
    for (size_t m = 0; m < mag; ++ m) {
      func.walsh_012(vec1, vec2);
    }
    sw.stop();

    delete [] vec1;
    delete [] vec2;
    
    USTime time = sw.time();
    mAccTime += time;
    mNum += mag;

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

    sw.start();
    for (size_t m = 0; m < mag; ++ m) {
      func.check_sup(pos);
    }
    sw.stop();

    USTime time = sw.time();
    mAccTime += time;
    mNum += mag;

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

    sw.start();
    for (size_t m = 0; m < mag; ++ m) {
      func.check_sym(pos1, pos2);
    }
    sw.stop();

    USTime time = sw.time();
    mAccTime += time;
    mNum += mag;

    return TCL_OK;
  }

  if ( subcmd == "walsh_w0" ) {
    if ( objv.size() != 5 ) {
      usage();
      return TCL_ERROR;
    }

    TvFunc func;
    int code = get_tvfunc(objv[2], func);
    if ( code != TCL_OK ) {
      return code;
    }

    unsigned long w;
    code = ulong_conv(objv[3], w);
    if ( code != TCL_OK ) {
      return code;
    }

    unsigned long ibits;
    code = ulong_conv(objv[4], ibits);
    if ( code != TCL_OK ) {
      return code;
    }

    sw.start();
    for (size_t m = 0; m < mag; ++ m) {
      func.walsh_w0(w, kPolPosi, ibits);
    }
    sw.stop();

    USTime time = sw.time();
    mAccTime += time;
    mNum += mag;

    return TCL_OK;
  }

  if ( subcmd == "walsh_w1" ) {
    if ( objv.size() != 6 ) {
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

    unsigned long w;
    code = ulong_conv(objv[4], w);
    if ( code != TCL_OK ) {
      return code;
    }

    unsigned long ibits;
    code = ulong_conv(objv[5], ibits);
    if ( code != TCL_OK ) {
      return code;
    }

    sw.start();
    for (size_t m = 0; m < mag; ++ m) {
      func.walsh_w1(pos1, w, kPolPosi, ibits);
    }
    sw.stop();

    USTime time = sw.time();
    mAccTime += time;
    mNum += mag;

    return TCL_OK;
  }

  if ( subcmd == "get_time" ) {
    TclObj tmp[3];
    if ( mNum > 0 ) {
      tmp[0] = mAccTime.usr_time_usec() / mNum;
      tmp[1] = mAccTime.sys_time_usec() / mNum;
      tmp[2] = mAccTime.real_time_usec() / mNum;
    }
    else {
      tmp[0] = 0;
      tmp[1] = 0;
      tmp[2] = 0;
    }
    TclObj result(3, tmp);
    set_result(result);

    return TCL_OK;
  }

  if ( subcmd == "reset" ) {
    if ( objv.size() != 2 ) {
      usage();
      return TCL_ERROR;
    }

    mAccTime.set(0.0, 0.0, 0.0);
    mNum = 0;
    return TCL_OK;
  }
  
  usage();
  return TCL_ERROR;
}

// 関数を取り出す．
int
TvFuncTimeCmd::get_tvfunc(TclObj& obj,
			  TvFunc& func)
{
  int code = TvFuncConv::from_tclobj(obj, interp(), func);
  return code;
}

// 入力番号を取り出す．
int
TvFuncTimeCmd::get_pos(TclObj& obj,
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


//////////////////////////////////////////////////////////////////////
/// NPN 同値類の正規化を行うコマンド
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
TvFuncNpnCmd::TvFuncNpnCmd()
{
}

// @brief デストラクタ
TvFuncNpnCmd::~TvFuncNpnCmd()
{
  mMagOpt = new TclPoptInt(this, "mag",
			   "specify magnification factor");
  set_usage_string("w2_recur <func>\n"
		   "repfunc_list\n"
		   "repfunc_num\n"
		   "get_time\n"
		   "reset");
}
  
// @brief コマンドを実行する仮想関数
int
TvFuncNpnCmd::cmd_proc(TclObjVector& objv)
{
  StopWatch sw;
  
  int mag = mMagOpt->val();

  if ( objv.size() < 2 ) {
    print_usage();
    return TCL_ERROR;
  }

  string subcmd = objv[1];

  if ( subcmd == "w2_recur" ) {
    if ( objv.size() != 3 ) {
      print_usage();
      return TCL_ERROR;
    }

    TvFunc func;
    int code = get_tvfunc(objv[2], func);
    if ( code != TCL_OK ) {
      return code;
    }

    NpnMap map;

    sw.start();
    for (size_t m = 0; m < mag; ++ m) {
      cannonical(func, map);
    }
    sw.stop();

    TvFunc repfunc = func.xform(map);
    if ( mRepFuncSet.count(repfunc) == 0 ) {
      mRepFuncSet.insert(repfunc);
    }

    USTime time = sw.time();
    mAccTime += time;
    mNum += mag;

    return TCL_OK;
  }

  if ( subcmd == "reset" ) {
    if ( objv.size() != 2 ) {
      print_usage();
      return TCL_ERROR;
    }
    mRepFuncSet.clear();
    mAccTime.set(0.0, 0.0, 0.0);
    mNum = 0;

    return TCL_OK;
  }

  if ( subcmd == "repfunc_list" ) {
    if ( objv.size() != 2 ) {
      print_usage();
      return TCL_ERROR;
    }
    TclObj ans;
    for (hash_set<TvFunc>::iterator p = mRepFuncSet.begin();
	 p != mRepFuncSet.end(); ++ p) {
      TclObj tmp = TvFuncConv::to_tclobj(*p);
      int code = ans.append_element(tmp, interp());
      if ( code != TCL_OK ) {
	return code;
      }
    }
    set_result(ans);
    return TCL_OK;
  }

  if ( subcmd == "repfunc_num" ) {
    if ( objv.size() != 2 ) {
      print_usage();
      return TCL_ERROR;
    }
    size_t num = mRepFuncSet.size();
    TclObj ans(num);
    set_result(ans);
    return TCL_OK;
  }

  if ( subcmd == "get_time" ) {
    TclObj tmp[3];
    if ( mNum > 0 ) {
      tmp[0] = mAccTime.usr_time_usec() / mNum;
      tmp[1] = mAccTime.sys_time_usec() / mNum;
      tmp[2] = mAccTime.real_time_usec() / mNum;
    }
    else {
      tmp[0] = 0;
      tmp[1] = 0;
      tmp[2] = 0;
    }
    TclObj result(3, tmp);
    set_result(result);

    return TCL_OK;
  }

  print_usage();
  return TCL_ERROR;
}

// 関数を取り出す．
int
TvFuncNpnCmd::get_tvfunc(TclObj& obj,
			 TvFunc& func)
{
  int code = TvFuncConv::from_tclobj(obj, interp(), func);
  return code;
}

END_NAMESPACE_YM_NPN
