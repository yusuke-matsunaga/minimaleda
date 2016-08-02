#ifndef MAGUS_EQUIV_EQUIVCMD_H
#define MAGUS_EQUIV_EQUIVCMD_H

/// @file magus/equiv/EquivCmd.h
/// @brief EquivCmd のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: EquivCmd.h 2274 2009-06-10 07:45:29Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.

#include "MagCmd.h"
#include "NetHandle.h"


BEGIN_NAMESPACE_MAGUS

//////////////////////////////////////////////////////////////////////
/// @class EquivCmdBase EquivCmd.h "EquivCmd.h"
/// @brief 検証を行うコマンドの基底クラス
//////////////////////////////////////////////////////////////////////
class EquivCmdBase :
  public MagCmd
{
public:

  /// @brief コンストラクタ
  EquivCmdBase(MagMgr* mgr);

  /// @brief デストラクタ
  virtual
  ~EquivCmdBase();


protected:

  /// @brief 共通のオプション解析と前処理を行う．
  /// @return エラーが起きたら TCL_ERROR を返す．
  int
  prologue(TclObjVector& objv);

  /// @brief ログレベルを返す．
  int
  log_level() const;

  /// @brief 時間制限を返す．
  int
  time_limit() const;

  /// @brief 使用する SAT-Solver の種類を表す文字列を返す．
  string
  sat_type() const;

  /// @brief 使用する SAT-Solver に渡すオプション文字列を返す．
  string
  sat_option() const;

  /// @brief SAT-Solver のログ出力用ストリームを返す．
  ostream*
  sat_out() const;

  /// @brief 検証対象のネットワーク1を返す．
  const BNetwork*
  network1() const;

  /// @brief 検証対象のネットワーク2を返す．
  const BNetwork*
  network2() const;

  /// @brief 入力の対応関係を返す．
  const vector<pair<ymuint32, ymuint32> >&
  input_match() const;

  /// @brief 出力の対応関係を返す．
  const vector<pair<ymuint32, ymuint32> >&
  output_match() const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる下請け関数
  //////////////////////////////////////////////////////////////////////


  /// @brief 時間を指定した文字列から時間を取り出す．
  /// @return エラーが起きたら TCL_ERROR を返す．
  int
  get_time(const string& str,
	   int& time);

  /// @brief 順番で対応をとり，ID番号のペアのリストを作る．
  void
  assoc_by_order(const BNetwork& network1,
		 const BNetwork& network2,
		 vector<pair<ymuint32, ymuint32> >& iassoc,
		 vector<pair<ymuint32, ymuint32> >& oassoc);

  /// @brief 名前で対応をとり, ID番号のペアのリストを作る．
  /// @return エラーが起きたら TCL_ERROR を返す．
  bool
  assoc_by_name(const BNetwork& network1,
		const BNetwork& network2,
		vector<pair<ymuint32, ymuint32> >& iassoc,
		vector<pair<ymuint32, ymuint32> >& oassoc);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // loglevel オプション解析用のオブジェクト
  TclPoptInt* mPoptLoglevel;

  // assoc-by-order オプション解析用のオブジェクト
  TclPopt* mPoptOrder;

  // assoc-by-name オプション解析用のオブジェクト
  TclPopt* mPoptName;

  // time_limit オプション解析用のオブジェクト
  TclPoptObj* mPoptTimeLimit;

  // sat オプション解析用のオブジェクト
  TclPoptStr* mPoptSat;

  // sat-option オプション解析用のオブジェクト
  TclPoptStr* mPoptSatOption;

  // satlog オプション解析用のオブジェクト
  TclPoptStr* mPoptSatlog;

  // ログレベル
  int mLogLevel;

  // 時間制限
  int mTimeLimit;

  // SAT-Solver のタイプ
  string mSatType;

  // SAT-Solver のオプション
  string mSatOption;

  // SAT-Solver のログ出力用ストリーム
  ostream* mSatOut;

  // SAT-Solver のログをファイルに出力する時のストリーム
  ofstream mSatLogFile;

  // 検証対象のネットワーク1
  NetHandle* mNetwork1;

  // 検証対象のネットワーク2
  NetHandle* mNetwork2;

  // 入力の対応関係
  vector<pair<ymuint32, ymuint32> > mInputMatch;

  // 出力の対応関係
  vector<pair<ymuint32, ymuint32> > mOutputMatch;

};


//////////////////////////////////////////////////////////////////////
/// @class EquivCmd EquivCmd.h "EquivCmd.h"
/// @brief 検証を行うコマンド
//////////////////////////////////////////////////////////////////////
class EquivCmd :
  public EquivCmdBase
{
public:

  /// @brief コンストラクタ
  EquivCmd(MagMgr* mgr);

  /// @brief デストラクタ
  virtual
  ~EquivCmd();


protected:

  /// @brief コマンド処理関数
  /// @param[in] objv 引数の配列
  int
  cmd_proc(TclObjVector& objv);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // loglevel オプション解析用のオブジェクト
  TclPoptInt* mPoptLoglevel;

  // sigsize オプション解析用のオブジェクト
  TclPoptInt* mPoptSigSize;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief ログレベルを返す．
inline
int
EquivCmdBase::log_level() const
{
  return mLogLevel;
}

// @brief 時間制限を返す．
inline
int
EquivCmdBase::time_limit() const
{
  return mTimeLimit;
}

// @brief 使用する SAT-Solver の種類を表す文字列を返す．
inline
string
EquivCmdBase::sat_type() const
{
  return mSatType;
}

// @brief 使用する SAT-Solver に渡すオプション文字列を返す．
string
EquivCmdBase::sat_option() const
{
  return mSatOption;
}

// @brief SAT-Solver のログ出力用ストリームを返す．
inline
ostream*
EquivCmdBase::sat_out() const
{
  return mSatOut;
}

// @brief 検証対象のネットワーク1を返す．
inline
const BNetwork*
EquivCmdBase::network1() const
{
  return mNetwork1->bnetwork();
}

// @brief 検証対象のネットワーク2を返す．
inline
const BNetwork*
EquivCmdBase::network2() const
{
  return mNetwork2->bnetwork();
}

// @brief 入力の対応関係を返す．
inline
const vector<pair<ymuint32, ymuint32> >&
EquivCmdBase::input_match() const
{
  return mInputMatch;
}

// @brief 出力の対応関係を返す．
inline
const vector<pair<ymuint32, ymuint32> >&
EquivCmdBase::output_match() const
{
  return mOutputMatch;
}

END_NAMESPACE_MAGUS

#endif // MAGUS_EQUIV_EQUIVCMD_H
