#ifndef MAIN_ATPGMGR_H
#define MAIN_ATPGMGR_H

/// @file src/main/AtpgMgr.h
/// @brief AtpgMgr のヘッダファイル
///
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: AtpgMgr.h 2203 2009-04-16 05:04:40Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "atpg_nsdef.h"
#include "fsim_nsdef.h"
#include "fsim3_nsdef.h"
#include "fsimx_nsdef.h"
#include "ym_tgnet/TgNetwork.h"
#include "FaultMgr.h"
#include "TvMgr.h"
#include "ym_utils/StopWatch.h"


BEGIN_NAMESPACE_YM_ATPG

class ModBase;

//////////////////////////////////////////////////////////////////////
/// @class AtpgMgr AtpgMgr.h "AtpgMgr.h"
/// @brief ATPG に共通のデータを保持しておくためのクラス
//////////////////////////////////////////////////////////////////////
class AtpgMgr
{
  friend class ModBase;
public:

  /// @brief コンストラクタ
  AtpgMgr();

  /// @brief デストラクタ
  ~AtpgMgr();

  
public:
  
  /// @brief TgNetwork を取り出す．
  TgNetwork&
  _network();
  
  /// @brief FaultMgr を取り出す．
  FaultMgr&
  _fault_mgr();

  /// @brief TvMgr を取り出す．
  TvMgr&
  _tv_mgr();
  
  /// @brief テストベクタのリストを取り出す．
  vector<TestVector*>&
  _tv_list();
  
  /// @brief Fsim を取り出す．
  Fsim&
  _fsim();
  
  /// @brief Fsim を取り出す．
  Fsim3&
  _fsim3();
  
  /// @brief Fsim を取り出す．
  FsimX&
  _fsimx();
  
  /// @brief タイマーを取り出す．
  MStopWatch&
  _timer();

  
public:

  /// @brief モジュールを登録する．
  void
  bind_module(ModBase* mod);
  
  /// @brief ネットワークをセットした後に呼ぶ関数
  void
  after_set_network();

  /// @brief 故障リストの内容が変わった後に呼ぶ関数
  void
  after_update_faults();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 対象のネットワーク
  TgNetwork mNetwork;
  
  // 故障リスト
  FaultMgr mFaultMgr;

  // テストベクタを管理するオブジェクト
  TvMgr mTvMgr;
  
  // テストベクタのリスト
  vector<TestVector*> mTvList;

  // タイマー
  MStopWatch mTimer;
  
  // 登録されているモジュール
  list<ModBase*> mModList;
  
  // Fsim モジュール
  Fsim* mFsim;
  
  // Fsim3 モジュール
  Fsim3* mFsim3;
  
  // FsimX モジュール
  FsimX* mFsimX;
  
};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief TgNetwork を取り出す．
inline
TgNetwork&
AtpgMgr::_network()
{
  return mNetwork;
}

// @brief FaultMgr を取り出す．
inline
FaultMgr&
AtpgMgr::_fault_mgr()
{
  return mFaultMgr;
}

// @brief TvMgr を取り出す．
inline
TvMgr&
AtpgMgr::_tv_mgr()
{
  return mTvMgr;
}
  
// @brief テストベクタのリストを取り出す．
inline
vector<TestVector*>&
AtpgMgr::_tv_list()
{
  return mTvList;
}

// @brief Fsim を取り出す．
inline
Fsim&
AtpgMgr::_fsim()
{
  return *mFsim;
}

// @brief Fsim を取り出す．
inline
Fsim3&
AtpgMgr::_fsim3()
{
  return *mFsim3;
}

// @brief FsimX を取り出す．
inline
FsimX&
AtpgMgr::_fsimx()
{
  return *mFsimX;
}
  
// @brief タイマーを取り出す．
inline
MStopWatch&
AtpgMgr::_timer()
{
  return mTimer;
}

END_NAMESPACE_YM_ATPG

#endif // MAIN_ATPGMGR_H
