
/// @file libym_tclpp/TclHandler.cc
/// @brief TclHandler とその継承クラスの実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: TclHandler.cc 700 2007-05-31 00:41:30Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_tclpp/TclHandler.h"


BEGIN_NAMESPACE_YM_TCLPP

//////////////////////////////////////////////////////////////////////
// クラス TclHander
//////////////////////////////////////////////////////////////////////

// コンストラクタ
TclHandler::TclHandler()
{
  // まだハンドラは登録されていない．
  mRegFlag = false;
}

// デストラクタ
TclHandler::~TclHandler(void)
{
  // 登録されているハンドラがあってはいけない．
  // ここでは派生クラスの仮想関数を呼ぶ事ができない．
  assert_cond( !is_registered(), __FILE__, __LINE__);
}

// ハンドラを登録する．
void
TclHandler::reg_common()
{
  if ( !is_registered() ) {
    set_regflag();
    // 本当のハンドラ登録処理を行なう仮想関数を呼ぶ．
    create_handler();
  }
}

// ハンドラの登録を削除する．
void
TclHandler::unreg()
{
  if ( is_registered() ) {
    reset_regflag();
    // 本当のハンドラ削除処理を行なう仮想関数を呼ぶ．
    delete_handler();
  }
}

// mRegFlag をセットする．
void
TclHandler::set_regflag()
{
  mRegFlag = true;
}

// mRegFlag をリセットする．
void
TclHandler::reset_regflag()
{
  mRegFlag = false;
}


//////////////////////////////////////////////////////////////////////
// クラス TclFileHander
//////////////////////////////////////////////////////////////////////

// コンストラクタ
TclFileHandler::TclFileHandler()
{
}

// デストラクタ
TclFileHandler::~TclFileHandler()
{
  // もしもハンドラが登録中ならここで削除しておく．
  unreg();
}

// ハンドラの登録を行なう本当の関数
void
TclFileHandler::create_handler()
{
  Tcl_CreateFileHandler(mFile, mMask, _file_proc, (ClientData)this);
}

// ハンドラの削除を行なう本当の関数
void
TclFileHandler::delete_handler()
{
  Tcl_DeleteFileHandler(mFile);
}

// ハンドラの登録を行なう．
// これは他のハンドラと異なり，以前のハンドラを上書きするので，
// mRegFlag をチェックしない．
void
TclFileHandler::reg(int file,
		    int mask)
{
  set_regflag();
  mFile = file;
  mMask = mask;
  // 本当のハンドラ登録処理を行なう仮想関数を呼ぶ．
  create_handler();
}

// マスクを設定し直して登録し直す．
void
TclFileHandler::change_mask(int mask)
{
  if ( is_registered() ) {
    // 一回も reg() が呼ばれていなければ再登録はできない．
    mMask = mask;
    create_handler();
  }
}

// イベント処理ルーティン
// といっても本体は仮想関数 TclFileHandler::file_proc()
void
TclFileHandler::_file_proc(ClientData clientData,
			   int mask)
{
  TclFileHandler* handler = static_cast<TclFileHandler*>( clientData );
  handler->file_proc(mask);
}


//////////////////////////////////////////////////////////////////////
// クラス TclIdleHander
//////////////////////////////////////////////////////////////////////

// コンストラクタ
TclIdleHandler::TclIdleHandler()
{
}

// デストラクタ
TclIdleHandler::~TclIdleHandler()
{
  // 登録されているハンドラがあれば削除する．
  unreg();
}

// ハンドラの登録を行う本当の関数．
void
TclIdleHandler::create_handler()
{
  Tcl_DoWhenIdle(_idle_proc, (ClientData)this);
}

// ハンドラの削除を行う本当の関数
void
TclIdleHandler::delete_handler()
{
  Tcl_CancelIdleCall(_idle_proc, (ClientData)this);
}

// イベント処理ルーティン
void
TclIdleHandler::_idle_proc(ClientData clientData)
{
  TclIdleHandler* handler = static_cast<TclIdleHandler*>( clientData );
  handler->idle_proc();
  // この時点でこのハンドラは削除されたことになっているはず．
  handler->reset_regflag();
}


//////////////////////////////////////////////////////////////////////
// クラス TclTimerHander
//////////////////////////////////////////////////////////////////////

// コンストラクタ
TclTimerHandler::TclTimerHandler()
{
}

// デストラクタ
TclTimerHandler::~TclTimerHandler()
{
  // 登録中のハンドラがあれば削除しておく．
  unreg();
}

// ハンドラの登録を行なう本当の関数
void
TclTimerHandler::create_handler()
{
  mToken = Tcl_CreateTimerHandler(mInterval, _timer_proc,
				  (ClientData) this);
}

// ハンドラの削除を行なう本当の関数
void
TclTimerHandler::delete_handler()
{
  Tcl_DeleteTimerHandler(mToken);
}

// ハンドラの登録を行う．
void
TclTimerHandler::reg(int interval)
{
  mInterval = interval;
  TclHandler::reg_common();
}

// イベント処理ルーティン
// といっても実際の処理は仮想関数 TclTimerHandler::TimerProc() が行う．
void
TclTimerHandler::_timer_proc(ClientData clientData)
{
  TclTimerHandler* handler = static_cast<TclTimerHandler*>( clientData );
  // 他のハンドラと異なり，一回呼ばれると自動的にハンドラが削除される
  // ので，mRegFlag をリセットする必要がある．
  handler->reset_regflag();
  handler->timer_proc();
}

//////////////////////////////////////////////////////////////////////
// クラス TclPeriodicTimerHander
//////////////////////////////////////////////////////////////////////

// コンストラクタ
TclPeriodicTimerHandler::TclPeriodicTimerHandler()
{
}

// デストラクタ
TclPeriodicTimerHandler::~TclPeriodicTimerHandler()
{
}

// 毎回，次のイベントハンドラの登録を行なう．
void
TclPeriodicTimerHandler::timer_proc()
{
  // 本当のイベント処理関数を呼ぶ．
  periodic_timer_proc();
  // 次のイベントハンドラの登録を行なう．
  reg(interval());
}

END_NAMESPACE_YM_TCLPP
