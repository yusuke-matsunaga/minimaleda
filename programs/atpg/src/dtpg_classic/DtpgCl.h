#ifndef DTPGCL_H
#define DTPGCL_H

/// @file include/DtpgCl.h
/// @brief DtpgCl のヘッダファイル
///
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: DtpgCl.h 2128 2009-03-12 07:25:33Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "dtpgcl_nsdef.h"
#include "ModBase.h"


BEGIN_NAMESPACE_YM_ATPG

class SaFault;
class TvMgr;
class TestVector;

END_NAMESPACE_YM_ATPG

BEGIN_NAMESPACE_YM_ATPG_DTPGCL

//////////////////////////////////////////////////////////////////////
/// @class DtpgCl DtpgCl.h "DtpgCl.h"
/// @brief 探索を行いパタン生成を行うモジュール
/// @sa ModBase
//////////////////////////////////////////////////////////////////////
class DtpgCl :
  public ModBase
{
public:

  /// @brief コンストラクタ
  DtpgCl();

  /// @brief デストラクタ
  virtual
  ~DtpgCl();
  

public:
  //////////////////////////////////////////////////////////////////////
  // パタン生成を行う関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ネットワーク全体をアクティブにする．
  void
  activate_all();
  
  /// @brief f に関わる部分のみをアクティブにする．
  /// @param[in] f 対象の故障
  /// @return アクティブにできる範囲があれば true を返す．
  bool
  activate_1fault(SaFault* f);

  /// @brief ネットワーク全体をアクティブでなくす．
  void
  deactivate();
  
  /// @brief f がアクティブな範囲に入っていれば true を返す．
  bool
  check_fault(SaFault* f);
  
  /// @brief 一つの故障に対してテストパタン生成を行う．
  /// @param[in] f 対象の故障
  /// @param[in] flist その他の未検出故障のリスト
  /// @param[in] dyn_mode dynamic implication のモード
  /// @param[out] tv 生成したパタンを入れるベクタ
  /// @param[out] tv で検出できる故障のリスト
  /// @retval kDetect パタン生成が成功した．
  /// @retval kUntest テスト不能故障だった．
  /// @retval kAbort アボートした．
  tStat
  run(SaFault* f,
      const vector<SaFault*>& flist,
      tDynMode dyn_mode,
      TestVector* tv,
      vector<SaFault*>& det_faults);
  

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

  
private:
  //////////////////////////////////////////////////////////////////////
  // ModBase の継承クラスが用意する仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ネットワークをセットした後に呼ばれる関数
  virtual
  void
  after_set_network(const TgNetwork& network,
		    TvMgr& tvmgr);

  /// @brief 故障リストの内容が変わった後に呼ばれる関数
  virtual
  void
  after_update_faults(const vector<SaFault*>& flist);
  

private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  
};
  
END_NAMESPACE_YM_ATPG_DTPGCL

#endif // DTPGCL_H
