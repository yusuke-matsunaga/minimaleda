
/// @file seal/src/misc/TimeCmd.cc
/// @brief TimeCmd の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: TimeCmd.cc 1920 2008-12-20 15:52:42Z matsunaga $
///
/// Copyright (C) 2005-2006 Yusuke Matsunaga
/// All rights reserved.

// 時間計測を行うパッケージ
// 実はこのパッケージは Magus には依存していないので汎用になる．


#if HAVE_CONFIG_H
#include <seal_config.h>
#endif

#include "TimeCmd.h"


BEGIN_NAMESPACE_YM_SEAL

//////////////////////////////////////////////////////////////////////
// ストップウォッチ操作を行うコマンド用のクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
StpwatchObj::StpwatchObj()
{
  const char* usage =
    "reset|start|stop|get|get_rtime|get_utime|get|stime\n"
    "       reset     # reset the stopwatch\n"
    "       start     # start the stopwatch\n"
    "       stop      # stop the stopwatch\n"
    "       get       # get real/user/system time\n"
    "       get_rtime # get real time\n"
    "       get_utime # get user time\n"
    "       get_stime # get system time";
  set_usage_string(usage);
}

// デストラクタ
StpwatchObj::~StpwatchObj()
{
}

// ストップウォッチを操作するコマンド
int
StpwatchObj::cmd_proc(TclObjVector& objv)
{
  if ( objv.size() != 2 ) {
    print_usage();
    return TCL_ERROR;
  }

  string cmd = objv[1];
  if ( cmd == "reset" ) {
    mStopWatch.reset();
  }
  else if ( cmd == "start" ) {
    mStopWatch.start();
  }
  else if ( cmd == "stop" ) {
    mStopWatch.stop();
  }
  else if ( cmd == "get" ) {
    USTime time = mStopWatch.time();
    TclObj tmp[3];
    tmp[0] = time.real_time();
    tmp[1] = time.usr_time();
    tmp[2] = time.sys_time();
    TclObj result(3, tmp);
    set_result(result);
  }
  else if ( cmd == "get_rtime" ) {
    USTime time = mStopWatch.time();
    set_result(time.real_time());
  }
  else if ( cmd == "get_utime" ) {
    USTime time = mStopWatch.time();
    set_result(time.usr_time());
  }
  else if ( cmd == "get_stime" ) {
    USTime time = mStopWatch.time();
    set_result(time.sys_time());
  }
  else {
    TclObj emsg;
    emsg << cmd << " : Bad stopwatch command";
    set_result(emsg);
    return TCL_ERROR;
  }
  return TCL_OK;
}


//////////////////////////////////////////////////////////////////////
// タイマーオブジェクトを生成するコマンドクラス
//////////////////////////////////////////////////////////////////////

// StpwatchObjのオブジェクトを生成する仮想関数
TclCmd*
StpwatchCls::create_obj() const
{
  return new StpwatchObj();
}


//////////////////////////////////////////////////////////////////////
// 時間計測コマンド
//////////////////////////////////////////////////////////////////////

// コンストラクタ
TimeCmd::TimeCmd()
{
  mStopWatch.reset();
  mStopWatch.start();
  mTotalTime.set(0.0, 0.0, 0.0);
}

// デストラクタ
TimeCmd::~TimeCmd()
{
}

// コマンド処理関数
// 引数がなければ，前回このコマンドが呼ばれてからの経過時間を計測する．
// 引数があれば残りの引数を実行し，その実行時間を計測する．
int
TimeCmd::cmd_proc(TclObjVector& objv)
{
  mStopWatch.stop();
  USTime time = mStopWatch.time();
  mTotalTime += time;

  int code = TCL_OK;
  TclObj res;
  if ( objv.size() > 1 ) {
    mStopWatch.reset();
    mStopWatch.start();

    // 残りの引数を評価する．
    TclObjVector::const_iterator p1 = objv.begin();
    p1 ++;
    TclObjVector::const_iterator p2 = objv.end();
    TclObjVector tmp_array(p1, p2);
    TclObj new_args(tmp_array);
    code = eval(new_args);

    mStopWatch.stop();
    time = mStopWatch.time();
    mTotalTime += time;
    res = result();
    res << "\n";
  }

  char buff[128];
  sprintf(buff, "Lap:%7.2fu%7.2fs%7.2fr, Total:%7.2fu%7.2fs%7.2fr",
	  time.usr_time(),
	  time.sys_time(),
	  time.real_time(),
	  mTotalTime.usr_time(),
	  mTotalTime.sys_time(),
	  mTotalTime.real_time());

  mStopWatch.reset();
  mStopWatch.start();

  res << buff;
  set_result(res);

  return code;
}

END_NAMESPACE_YM_SEAL
