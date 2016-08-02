#ifndef MAIN_DTPG_H
#define MAIN_DTPG_H

/// @file src/main/Dtpg.h
/// @brief Dtpg のヘッダファイル
///
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: Dtpg.h 2203 2009-04-16 05:04:40Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "AtpgFunctor.h"
#include "dtpgcl_nsdef.h"
#include "dtpgsat_nsdef.h"
#include "TvBuff.h"


BEGIN_NAMESPACE_YM_ATPG

//////////////////////////////////////////////////////////////////////
/// @class DtpgStats Dtpg.h "Dtpg.h"
/// @brief DTPG の statistics data を表すクラス
//////////////////////////////////////////////////////////////////////
class DtpgStats
{
  friend class Dtpg;

public:

  /// @brief コンストラクタ
  DtpgStats();

  /// @brief デストラクタ
  ~DtpgStats();


public:
  //////////////////////////////////////////////////////////////////////
  // 情報を取得する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 検出した故障数を得る．
  ymuint
  detected_faults() const;

  /// @brief 検出不能と判断した故障数を得る．
  ymuint
  untestable_faults() const;

  /// @brief アボートした故障数を得る．
  ymuint
  aborted_faults() const;

  /// @brief 生成したパタン数を得る．
  ymuint
  generated_patterns() const;
  
  /// @brief 起動回数を得る．
  ymuint
  call_count() const;
  
  /// @brief バックトラック数を得る．
  ymuint
  backtrack_count() const;

  /// @brief 計算時間を得る．
  USTime
  time() const;
  
  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 検出した故障数
  ymuint mDetectNum;

  // 検出不能な故障数
  ymuint mUntestNum;

  // アボート故障数
  ymuint mAbortNum;
  
  // パタン数
  ymuint mPatNum;

  // 起動回数
  ymuint mDtpgNum;
  
  // バックトラック数
  ymuint mBtNum;
  
  // 計算時間
  USTime mTime;
  
};


//////////////////////////////////////////////////////////////////////
/// @class Dtpg Dtpg.h "Dtpg.h"
/// @brief DTPG を行うクラス
//////////////////////////////////////////////////////////////////////
class Dtpg :
  public AtpgFunctor
{
public:

  /// @brief コンストラクタ
  Dtpg(AtpgMgr* mgr);

  /// @brief デストラクタ
  virtual
  ~Dtpg();
  
  
public:

  /// @brief 未検出の故障に対してテストパタン生成を行う．
  /// @param[in] sim_mode シミュレーションモード
  void
  operator()(tSimMode sim_mode);
  
  /// @brief 直前の実行結果を返す．
  const DtpgStats&
  stats() const;

  
public:
  //////////////////////////////////////////////////////////////////////
  // バックトラック制限数に関するアクセス関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 現在のバックトラック制限数を得る．
  ymuint
  backtrack_limit() const;

  /// @brief バックトラック制限数を設定する．
  /// @param[in] limit 設定する制限数
  void
  set_backtrack_limit(ymuint limit);

  /// @brief デフォルトのバックトラック制限数を設定する．
  void
  set_default_backtrack_limit();
  

public:
  //////////////////////////////////////////////////////////////////////
  // Unique Sensitization モードに関するアクセス関数
  //////////////////////////////////////////////////////////////////////

  /// @brief Unique Sensitization モードを得る．
  int
  us_mode() const;

  /// @brief Unique Sensitization モードを設定する．
  /// @param[in] us_mode 設定する Unique Sensitization モードの値
  void
  set_us_mode(ymuint us_mode);
  

public:
  //////////////////////////////////////////////////////////////////////
  // バックトラック数に関するアクセス関数
  //////////////////////////////////////////////////////////////////////
  
  /// @brief バックトラック数のカウンタをクリアする．
  void
  clear_backtrack_count();
  
  /// @brief 今までのバックトラック数の総和を返す．
  ymuint
  total_backtrack_count() const;
  
  /// @brief verbose level を設定する．
  void
  set_verbose_level(ymuint vlevel);
  

public:
  //////////////////////////////////////////////////////////////////////
  // dynamic implication のモードに関するアクセス関数
  //////////////////////////////////////////////////////////////////////
  
  /// @brief dynamic implication のモードを設定する．
  void
  set_dyn_mode(nsDtpgCl::tDynMode dyn_mode);
  

public:
  //////////////////////////////////////////////////////////////////////
  // SAT に関する制御
  //////////////////////////////////////////////////////////////////////
  
  /// @brief SAT モードの制御を行う．
  void
  set_sat(bool flag);
  
  /// @brief SAT エンジンの指定を行う．
  void
  set_sat_mode(int mode);

  
public:
  //////////////////////////////////////////////////////////////////////
  // scoping に関する制御
  //////////////////////////////////////////////////////////////////////
  
  /// @brief スコーピングの制御を行う．
  void
  set_scoping(bool flag);

  
private:
  //////////////////////////////////////////////////////////////////////
  // 下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 未検出の故障に対してテストパタン生成を行う．
  /// @param[in] sim_mode シミュレーションモード
  void
  dtpg1(tSimMode sim_mode);

  /// @brief 未検出の故障に対してテストパタン生成を行う．
  /// @param[in] sim_mode シミュレーションモード
  void
  dtpg2(tSimMode sim_mode);
  
  void
  pat_gen_sub(tSimMode sim_mode);
  
  /// @brief f に対するテストパタン生成を行う．
  /// @return パタンを返す．
  TestVector*
  dtpg_sub(SaFault* f);
  
  /// @brief f に対するテストパタン生成を行う．
  /// @return パタンを返す．
  TestVector*
  dtpg2_sub(SaFault* f);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // SAT モード
  bool mSat;

  // スコーピングフラグ
  bool mScoping;
  
  // 構造に基づいてテストパタン生成を行うクラス
  DtpgCl* mDtpgCl;

  // SAT を用いてテストパタン生成を行うクラス
  DtpgSat* mDtpgSat;
  
  // dynamic implication モード
  nsDtpgCl::tDynMode mDynMode;
  
  // テストベクタのバッファ
  TvBuff mTvBuff;
  
  // 対象の故障を保持するリスト
  vector<SaFault*> mCurFaults;

  // テストベクタを格納する作業領域
  TestVector* mTmpTv;

  // 検出された故障を貯めておくバッファ
  vector<SaFault*> mDetFaults;

  // パタン生成用の乱数生成器
  RandGen mPatGen;

  // 故障選択用の乱数生成器
  RandGen mFselGen;
  
  // 実行結果
  DtpgStats mStats;
  
};

END_NAMESPACE_YM_ATPG

#endif // MAIN_DTPG_H
