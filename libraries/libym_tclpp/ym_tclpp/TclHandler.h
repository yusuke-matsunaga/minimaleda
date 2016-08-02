#ifndef YM_TCLPP_TCLHANDLER_H
#define YM_TCLPP_TCLHANDLER_H

/// @file ym_tclpp/TclHandler.h
/// @brief TclHandler のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: TclHandler.h 1343 2008-03-25 17:15:35Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_tclpp/tclpp_nsdef.h"


BEGIN_NAMESPACE_YM_TCLPP

//////////////////////////////////////////////////////////////////////
/// @class TclHandler TclHandler.h <ym_tclpp/TclHandler.h>
/// @ingroup Tclpp
/// @brief Tcl のさまざまなハンドラの基底クラス
//////////////////////////////////////////////////////////////////////
class TclHandler
{
public:

  /// @brief コンストラクタ
  TclHandler();
  
  /// @brief デストラクタ
  /// @note 登録されているハンドラがあればそれを削除する．
  virtual
  ~TclHandler();
  
  /// @brief 現在，ハンドラが登録されているかを調べる．
  /// @return 登録されていたら true を返す．
  bool
  is_registered() const;
  
  /// @brief ハンドラを登録する共通関数．
  /// @note 実際の振舞いは仮想関数 create_handler() で定義する．
  /// 多くのハンドラは登録の際に引数を必要とするので，
  /// 各々のクラスに固有のreg()関数を定義する必要がある．
  virtual
  void
  reg_common();
  
  /// @brief ハンドラを削除する．
  /// @note 実際の振舞いは仮想関数 delete_handler() で定義する．
  virtual
  void unreg();


protected:

  /// @brief mRegFlag をセットする．
  virtual
  void
  set_regflag();

  /// @brief mRegFLag をリセットする．
  virtual
  void
  reset_regflag();

  /// @brief ハンドラの登録を行なう仮想関数
  virtual
  void
  create_handler() = 0;

  /// @brief ハンドラの削除を行なう仮想関数
  virtual
  void
  delete_handler() = 0;


private:
  //////////////////////////////////////////////////////////////////////
  // メンバ変数
  //////////////////////////////////////////////////////////////////////

  // ハンドラが登録されているかを示すフラグ
  bool mRegFlag;

};


//////////////////////////////////////////////////////////////////////
/// @class TclFileHandler TclHandler.h <ym_tclpp/Tclpp.h>
/// @ingroup Tclpp
/// @brief Tcl のファイルイベントハンドラの純粋仮想クラス
///
/// 指定したファイルハンドルを監視して指定したイベントが起こった場合
/// に仮想関数 file_proc() を呼び出す．
///
/// ユーザーはこのクラスを継承して file_proc() を実装する必要がある．
//////////////////////////////////////////////////////////////////////
class TclFileHandler :
  public TclHandler
{
public:

  /// @brief コンストラクタ
  /// @note この時点では登録されていない．
  TclFileHandler();
  
  /// @brief デストラクタ
  /// @note ハンドラが登録されていたら削除する．
  virtual
  ~TclFileHandler();
  
  /// @brief ファイルハンドルを得る．
  int
  file() const;
  
  /// @brief マスクを得る．
  int
  mask() const;
  
  /// @brief ハンドラの登録を行う．
  /// @param[in] file ファイルハンドル
  /// @param[in] mask イベントマスク
  virtual
  void
  reg(int file,
      int mask);
  
  /// @brief マスクを指定し直して再登録する．
  /// @param[in] mask イベントマスク
  /// mask = 0 とすればイベントを受けとらないようにすることができる．
  /// unreg() するよりも効率的．
  /// @note そもそもこのオブジェクトが登録されていない時はなにもしない．
  virtual
  void
  change_mask(int mask);
  
  // ハンドラの削除は TclHandler::unreg() で行なう．
  // といっても中で delete_handler() が呼ばれるんだけどね．

protected:

  /// @brief イベント処理を行う本体の仮想関数
  /// @param[in] mask 呼び出される原因となったイベント
  virtual
  void
  file_proc(int mask) = 0;

  /// @brief ハンドラの登録を行なう本当の関数
  virtual
  void
  create_handler();

  /// @brief ハンドラの削除を行なう本当の関数
  virtual
  void
  delete_handler();


private:

  /// @brief Tcl に登録するみかけ上のイベント処理関数
  static
  void
  _file_proc(ClientData clientData,
	     int mask);


private:
  //////////////////////////////////////////////////////////////////////
  // メンバ変数
  //////////////////////////////////////////////////////////////////////

  // ファイルハンドル
  int mFile;

  // マスク
  int mMask;

};


//////////////////////////////////////////////////////////////////////
/// @class TclIdleHandler TclHandler.h <ym_tclpp/Tclpp.h>
/// @ingroup Tclpp
/// @brief Tcl のアイドルコールバックハンドラの純粋仮想クラス
///
/// アイドル状態になると idle_proc() が呼び出される．
///
/// ユーザーはこのクラスを継承して idle_proc() を実装する必要がある．
//////////////////////////////////////////////////////////////////////
class TclIdleHandler :
  public TclHandler
{
public:

  /// @brief コンストラクタ
  /// @note コールバックの登録はしない．
  TclIdleHandler();
  
  /// @brief デストラクタ
  /// アイドルコールバックが登録されていたら削除する．
  virtual
  ~TclIdleHandler();
  
  /// @brief 登録を行なう．
  void
  reg();


protected:

  /// @brief 実際の処理を行う仮想関数
  virtual
  void
  idle_proc() = 0;

  /// @brief ハンドラの登録を行なう本当の関数
  virtual
  void
  create_handler();

  /// @brief ハンドラの削除を行なう本当の関数
  virtual
  void
  delete_handler();


private:
  
  /// @brief Tcl に登録するみかけ上のイベント処理関数
  static
  void
  _idle_proc(ClientData clientData);

};


//////////////////////////////////////////////////////////////////////
/// @class TclTimerHandler TclHandler.h <ym_tclpp/Tclpp.h>
/// @ingroup Tclpp
/// @brief Tcl のタイマハンドラを表す純粋仮想クラス
///
/// 指定されたインターバルが経過すると timer_proc() が呼び出される．
///
/// ユーザーはこのクラスを継承して timer_proc() を実装する必要がある．
//////////////////////////////////////////////////////////////////////
class TclTimerHandler :
  public TclHandler
{
public:

  /// @brief コンストラクタ
  /// @note 登録はしない．
  TclTimerHandler();
  
  /// @brief デストラクタ
  /// @note 未処理のイベントがあれば削除する．
  virtual
  ~TclTimerHandler();
  
  /// @brief インターバルを得る．
  /// @note 一度も reg() を呼んでいなければ値は不定
  int
  interval() const;
  
  /// @brief インターバルを指定して登録する．
  /// @param[in] interval イベントを駆動する間隔 (ミリ秒)
  void
  reg(int interval);


protected:

  /// @brief 本当のイベント処理を行なう関数
  virtual
  void
  timer_proc() = 0;

  /// @brief ハンドラの登録を行なう本当の関数
  virtual
  void
  create_handler();

  /// @brief ハンドラの削除を行なう本当の関数
  virtual
  void
  delete_handler();


private:
  
  /// @brief Tcl に登録するみかけ上のイベント処理関数
  static
  void
  _timer_proc(ClientData clientdata);

private:
  //////////////////////////////////////////////////////////////////////
  // メンバ変数
  //////////////////////////////////////////////////////////////////////

  // 間隔(ミリ秒)
  int mInterval;

  // トークン
  Tcl_TimerToken mToken;

};


//////////////////////////////////////////////////////////////////////
/// @class TclPeriodicTimerHandler TclHandler.h <ym_tclpp/Tclpp.h>
/// @ingroup Tclpp
/// @brief 周期的にイベント処理を行なうハンドラの純粋仮想クラス
///
/// TclTimerHandler の継承クラスなので登録の仕方は同じ
///
/// ただし timer_proc() ではなく periodic_timer_proc() を実装する
/// 必要がある．
/// @sa TclTimerHandler
//////////////////////////////////////////////////////////////////////
class TclPeriodicTimerHandler :
  public TclTimerHandler
{
public:

  /// @brief コンストラクタ
  TclPeriodicTimerHandler();
  
  /// @brief デストラクタ
  virtual
  ~TclPeriodicTimerHandler();


protected:

  /// @brief timer_procから呼ばれる関数
  virtual
  void
  timer_proc();

  /// @brief 本当のイベント処理関数
  virtual
  void
  periodic_timer_proc(void) = 0;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////
  
// 現在，ハンドラが登録されているかを調べる．
inline
bool
TclHandler::is_registered() const
{
  return mRegFlag;
}
  
// ファイルハンドルを得る．
inline
int
TclFileHandler::file() const
{
  return mFile;
}

// マスクを得る．
inline
int
TclFileHandler::mask() const
{
  return mMask;
}
  
// 登録を行なう．
inline
void
TclIdleHandler::reg()
{
  TclHandler::reg_common();
}

// インターバルを得る．
inline
int
TclTimerHandler::interval() const
{
  return mInterval;
}

END_NAMESPACE_YM_TCLPP

#endif // YM_TCLPP_TCLHANDLER_H
