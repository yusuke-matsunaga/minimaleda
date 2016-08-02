#ifndef YM_UTILS_STOPWATCH_H
#define	YM_UTILS_STOPWATCH_H

/// @file ym_utils/StopWatch.h
/// @brief 時間計測用のクラスのヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: StopWatch.h 400 2007-03-06 17:58:07Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ymtools.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class USTime StopWatch.h <ym_utils/StopWatch.h>
/// @ingroup YmUtils
/// @brief ユーザー時間とシステム時間および実時間をもつクラス
/// @sa StopWatch, MStopWatch
//////////////////////////////////////////////////////////////////////
class USTime
{
public:

  /// @brief 空のコンストラクタ
  USTime();

  /// @brief 内容を指定したコンストラクタ
  /// @param[in] u ユーザー時間(usec)
  /// @param[in] s システム時間(usec)
  /// @param[in] r 実経過時間(usec)
  USTime(double u,
	 double s,
	 double r);
  
  // コピーコンストラクタと代入はデフォルトで大丈夫
  
  /// @brief 設定用関数
  /// @param[in] u ユーザー時間(usec)
  /// @param[in] s システム時間(usec)
  /// @param[in] r 実経過時間(usec)
  void
  set(double u,
      double s,
      double r);
  
  /// @brief ユーザー時間の取得
  /// @return ユーザー時間(usec)
  /// @sa sys_time(), real_time()
  double
  usr_time_usec() const;

  /// @brief システム時間の取得
  /// @return システム時間(usec)
  /// @sa usr_time(), real_time()
  double
  sys_time_usec() const;

  /// @brief 実経過時間を取り出す．
  /// @return 実経過時間(usec)
  /// @sa usr_time(), sys_time()
  double
  real_time_usec() const;
  
  /// @brief ユーザー時間の取得
  /// @return ユーザー時間(sec)
  /// @sa sys_time(), real_time()
  double
  usr_time() const;

  /// @brief システム時間の取得
  /// @return システム時間(sec)
  /// @sa usr_time(), real_time()
  double
  sys_time() const;

  /// @brief 実経過時間を取り出す．
  /// @return 実経過時間(sec)
  /// @sa usr_time(), sys_time()
  double
  real_time() const;
  

  /// @brief 加算付き代入文
  /// @param[in] a 第一引数
  /// @return 自分自身に a を足したもの
  /// @sa operator+(const USTime& a, const USTime& b);
  const USTime&
  operator+=(const USTime& a);
  
  /// @brief 減算付き代入文
  /// @param[in] a 第一引数
  /// @return 自分自身から a を引いたもの
  /// @sa operator-(const USTime& a, const USTime& b)
  const USTime&
  operator-=(const USTime& a);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ユーザー時間 in microseconds
  double mUtime;

  // システム時間 in microseconds
  double mStime;

  // 実時間 in microseconds
  double mRtime;

};


/// @name 時間計測用関数
/// @{

/// @relates USTime
/// @ingroup YmUtils
/// @brief 加算
/// @param[in] a 第一引数
/// @param[in] b 第二引数
/// @return a + b
/// @sa USTime::operator+=(const USTime& a)
USTime
operator+(const USTime& a,
	  const USTime& b);
  
/// @relates USTime
/// @ingroup YmUtils
/// @brief 減算
/// @param[in] a 第一引数
/// @param[in] b 第二引数
/// @return a - b
/// @sa USTime::operator-=(const USTime& a)
USTime
operator-(const USTime& a,
	  const USTime& b);

/// @relates USTime
/// @ingroup YmUtils
/// @brief 時間をストリームに出力する．
/// @param[in] s 出力先のストリーム
/// @param[in] time 出力対象の時間
/// @return ストリーム s
ostream&
operator<<(ostream& s,
	   const USTime& time);

/// @}


//////////////////////////////////////////////////////////////////////
/// @class StopWatch StopWatch.h <ym_utils/StopWatch.h>
/// @ingroup YmUtils
/// @brief ストップウォッチのクラス
/// @note
/// - {"停止中", "計時中"}の2つの内部状態を持つ．
/// - 計時中の累積時間を内部に保持している．
/// @sa USTime, MStopWatch
//////////////////////////////////////////////////////////////////////
class StopWatch
{
  friend class MStopWatch;
  
public:

  /// @brief コンストラクタ．
  /// @note
  /// - 停止中
  /// - 累積時間は 0
  /// の状態で初期化される．
  StopWatch();
  
  /// @brief "リセット" する.
  /// @note
  /// - 停止中
  /// - 累積時間は 0
  /// の状態にリセットする．計測途中の場合には今までの累積値は捨てられる．
  void
  reset();
  
  /// @brief 計測開始.
  /// @note すでに計時中だったらなにもしない．
  void
  start();
  
  /// @brief 計測を止める.
  /// @note
  /// - すでに停止状態だったらなにもしない．
  /// - 計測状態だったら直前の start() から今までの経過時間を
  /// 累積時間に足す．
  void
  stop();

  /// @brief 累積時間の取得
  /// @retval ラップタイム 計測状態の場合
  /// @retval 経過累積時間 停止状態
  USTime
  time() const;
  

private:

  // 現在の時間をとってくる関数
  static
  USTime
  cur_time();
  
  // timeval構造体をdoubleに変換する関数
  static
  double
  xchg(struct timeval& tv);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ストップウォッチの状態を表す
  enum {
    // 停止状態
    OFF,
    // 計測状態
    ON
  } mState;

  // 開始時刻
  USTime mStartTime;

  // 経過時間
  USTime mTotalTime;

};


//////////////////////////////////////////////////////////////////////
/// @class MStopWatch StopWatch.h <ym_utils/StopWatch.h>
/// @ingroup YmUtils
/// @brief 複数のストップウォッチを切替えるためのクラス
/// @sa USTime, StopWatch
//////////////////////////////////////////////////////////////////////
class MStopWatch
{
public:

  /// @brief コンストラクタ.
  /// @param[in] n 時計の数
  /// @param[in] id 最初にアクティブな時計番号
  MStopWatch(size_t n,
	     size_t id = 0);
  
  /// @brief アクティブな時計を切替える.
  /// @param[in] new_id アクティブにする時計番号
  /// @return 昔の時計番号を返す
  size_t
  change(size_t new_id);
  
  /// @brief 現在アクティブな時計番号を返す.
  /// @return 現在アクティブな時計番号を返す.
  size_t
  cur_id() const;
  
  /// @brief 経過時間の取得
  /// @param[in] id 対象の時計番号
  /// @return id で指定された時計の経過時間
  USTime
  time(size_t id) const;
  

private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 時計の数
  size_t mNum;

  // 現在アクティブな時計の番号
  size_t mCur;

  // StopWatchの配列
  vector<StopWatch> mWarray;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// 空のコンストラクタ
inline
USTime::USTime()
{
}

// 値を設定するコンストラクタ
inline
USTime::USTime(double u,
	       double s,
	       double r) :
  mUtime(u),
  mStime(s),
  mRtime(r)
{
}

// 設定
inline
void
USTime::set(double u,
	    double s,
	    double r)
{
  mUtime = u;
  mStime = s;
  mRtime = r;
}

// ユーザー時間を取り出す．
inline
double
USTime::usr_time_usec() const
{
  return mUtime;
}

// システム時間を取り出す
inline
double
USTime::sys_time_usec() const
{
  return mStime;
}

// 実時間を取り出す
inline
double
USTime::real_time_usec() const
{
  return mRtime;
}

// ユーザー時間を取り出す．
inline
double
USTime::usr_time() const
{
  return mUtime / (1000 * 1000);
}

// システム時間を取り出す
inline
double
USTime::sys_time() const
{
  return mStime / (1000 * 1000);
}

// 実時間を取り出す
inline
double
USTime::real_time() const
{
  return mRtime / (1000 * 1000);
}

// 加算
inline
USTime
operator+(const USTime& a,
	  const USTime& b)
{
  return USTime(a).operator+=(b);
}

// 加算付き代入
inline
const USTime&
USTime::operator+=(const USTime& a)
{
  mUtime += a.mUtime;
  mStime += a.mStime;
  mRtime += a.mRtime;
  return *this;
}
 
// 減算
inline
USTime
operator-(const USTime& a,
	  const USTime& b)
{
  return USTime(a).operator-=(b);
}

// 減算つき代入
inline
const USTime&
USTime::operator-=(const USTime& a)
{
  mUtime -= a.mUtime;
  mStime -= a.mStime;
  mRtime -= a.mRtime;
  return *this;
}

// 現在アクティブな時計番号を返す．
inline
size_t
MStopWatch::cur_id() const
{
  return mCur;
}

// idで指定された時計の経過時間を返す
inline
USTime
MStopWatch::time(size_t id) const
{
  // 範囲外の場合は補正する．
  if ( id >= mNum ) {
    id = 0;
  }
  return mWarray[id].time();
}

END_NAMESPACE_YM

#endif // YM_UTILS_STOPWATCH_H
