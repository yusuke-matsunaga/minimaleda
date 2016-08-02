#ifndef BDDSIM_BDDSIMCMD_H
#define BDDSIM_BDDSIMCMD_H

/// @file src/bddsim/BddsimCmd.h
/// @brief BddsimCmd のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: BddsimCmd.h 1920 2008-12-20 15:52:42Z matsunaga $
///
/// Copyright (C) 2005-2007 Yusuke Matsunaga
/// All rights reserved.

// ネットワークを操作するコマンド群

#include "nsdef.h"
#include "SealCmd.h"
#include "EventQ.h"
#include <ym_tgnet/tgnet_nsdef.h>
#include <ym_bdd/Bdd.h>


BEGIN_NAMESPACE_YM_SEAL_BDDSIM

class SimNode;

//////////////////////////////////////////////////////////////////////
// BDD による故障シミュレーションを実行するコマンド
//////////////////////////////////////////////////////////////////////
class BddsimCmd :
  public SealCmd
{
public:

  /// @brief コンストラクタ
  BddsimCmd(SealMgr* mgr);

  /// @brief デストラクタ
  virtual
  ~BddsimCmd();


protected:
  
  // コマンド処理関数
  virtual
  int
  cmd_proc(TclObjVector& objv);


private:

  // 入力用の SimNode を生成する．
  SimNode*
  make_input(const TgNode* node);

  // logic 用の SimNode を生成する．
  SimNode*
  make_logic(const TgNode* node);

  // 全ノードの可観測性を単純に計算する．
  void
  calc_obs_naive();

  // 全ノードの可観測性を FFR を考慮して計算する．
  void
  calc_obs_ffr();

  // FFR 内部の可観測性を計算する．
  void
  calc_obs_ffr_sub(const TgNode* node);

  // 全ノードの可観測性を DSS を考慮して計算する．
  void
  calc_obs_dss();

  // 全ノードの可観測性を高速に近似的に計算する．
  void
  calc_obs_fast();

  // simnode に ffunc を設定して故障シミュレーションを行う．
  Bdd
  calc_ffunc(SimNode* simnode,
	     Bdd ffunc);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // trace オプション解析用オブジェクト
  TclPopt* mPoptTrace;

  // trace_count オプション解析用のオブジェクト
  TclPopt* mPoptTraceCount;

  // method オプション解析用のオブジェクト
  TclPoptStr* mPoptMethod;
  
  // BDD マネージャ
  BddMgrRef mMgr;

  // SimNode を納めた配列
  vector<SimNode*> mNodeArray;

  // 故障シミュレーション用のイベントキュー
  EventQ mEventQ;

  // 故障シミュレーション用の作業領域
  vector<SimNode*> mClearArray;
  
};

END_NAMESPACE_YM_SEAL_BDDSIM

#endif // BDDSIM_BDDSIMCMD_H
