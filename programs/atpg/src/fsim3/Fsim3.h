#ifndef FSIM3_FSIM3_H
#define FSIM3_FSIM3_H

/// @file src/fsim/Fsim.h
/// @brief Fsim のヘッダファイル
///
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: Fsim3.h 2203 2009-04-16 05:04:40Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "fsim3_nsdef.h"
#include "ModBase.h"
#include "ym_tgnet/tgnet_nsdef.h"
#include "ym_tgnet/TgNode.h"
#include "ym_lexp/LogExpr.h"
#include "EventQ.h"
#include "FsimFault.h"


BEGIN_NAMESPACE_YM_ATPG

class SaFault;
class TestVector;

END_NAMESPACE_YM_ATPG


BEGIN_NAMESPACE_YM_ATPG_FSIM3

class SimFFR;
class SimNode;

//////////////////////////////////////////////////////////////////////
/// @class Fsim3 Fsim3.h "Fsim3.h"
/// @brief 故障シミュレーションを行うモジュール
/// @sa ModBase
//////////////////////////////////////////////////////////////////////
class Fsim3 :
  public ModBase
{
public:

  /// @brief コンストラクタ
  Fsim3();

  /// @brief デストラクタ
  virtual
  ~Fsim3();


public:
  
  /// @brief 対象の故障をセットする
  /// @param[in] flist 対象の故障リスト
  void
  set_faults(const vector<SaFault*>& flist);
  
  /// @brief 故障シミュレーションを行う．
  /// @param[in] tv テストベクタ
  /// @param[out] det_faults 検出された故障を格納するリスト
  void
  run(TestVector* tv,
      list<SaFault*>& det_faults);

  /// @brief 故障シミュレーションを行う．
  /// @param[in] tv_array テストベクタの配列
  /// @param[out] det_faults 検出された故障を格納するリストの配列
  void
  run(const vector<TestVector*>& tv_array,
      vector<list<SaFault*> >& det_faults);
  
  /// @brief 一つのパタンで一つの故障に対するシミュレーションを行う．
  /// @param[in] tv テストベクタ
  /// @param[in] f 対象の故障
  bool
  run(TestVector* tv,
      SaFault* f);


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
  
  /// @brief FFR 内の故障シミュレーションを行う．
  bool
  ffr_simulate(SimFFR* ffr,
	       vector<FsimFault*>& det_flist);
  
  /// @brief 正常値の計算を行う．
  /// @note 値の変わったノードは mGvalClearArray に積まれる．
  void
  calc_gval();

  /// @brief 正常値をクリアする．
  /// @note mGvalClearArray を使う．
  void
  clear_gval();
  
  /// @brief 故障値の計算を行う．
  /// @note この関数を抜けた時点で故障値はクリアされている．
  bool
  calc_fval(SimNode* root);


private:
  //////////////////////////////////////////////////////////////////////
  // TgNode と SimNode の対応関係を表すためのデータ構造
  //////////////////////////////////////////////////////////////////////

  struct EdgeMap {
    SimNode* mNode;
    ymuint32 mPos;
  };


private:

  /// @brief 現在保持している SimNode のネットワークを破棄する．
  /// 内部で clear_faults() を呼ぶ．
  void
  clear();
  
  /// @brief FsimFault を破棄する．
  void
  clear_faults();
  
  /// @brief node に対応する SimNode を得る．
  SimNode*
  find_simnode(const TgNode* node) const;

  /// @brief node の pos 番めの入力に対応する枝を得る．
  void
  find_simedge(const TgNode* node,
	       ymuint pos,
	       SimNode*& simnode,
	       ymuint& ipos) const;

  /// @brief logic ノードを作る．
  SimNode*
  make_logic(const LogExpr& lexp,
	     const vector<SimNode*>& inputs,
	     const vector<EdgeMap*>& emap);

  /// @brief logic ノードを作る．
  SimNode*
  make_node(tTgGateType type,
	    const vector<SimNode*>& inputs);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 対象のネットワーク
  const TgNetwork* mNetwork;
  
  // TgNode の gid をキーにして SimNode を入れる配列
  vector<SimNode*> mSimMap;
  
  // TgNode の gid と入力番号から枝の情報を取り出す配列
  vector<vector<EdgeMap> > mEdgeMap;
  
  // 全ての SimNode を納めた配列
  vector<SimNode*> mNodeArray;

  // 外部入力に対応する SimNode を納めた配列
  vector<SimNode*> mInputArray;

  // 外部出力に対応する SimNode を納めた配列
  vector<SimNode*> mOutputArray;

  // 入力からのトポロジカル順に並べた logic ノードの配列
  vector<SimNode*> mLogicArray;

  // FFR を納めた配列
  vector<SimFFR> mFFRArray;

  // イベントキュー
  EventQ mEventQ;

  // 正常値を消去する必要のあるノードを入れておく配列
  vector<SimNode*> mGvalClearArray;

  // 故障値を消去する必要のあるノードを入れておく配列
  vector<SimNode*> mFvalClearArray;
  
  // 故障シミュレーション用の故障の配列
  vector<FsimFault> mFsimFaults;
  
};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief node に対応する SimNode* を得る．
inline
SimNode*
Fsim3::find_simnode(const TgNode* node) const
{
  return mSimMap[node->gid()];
}

// @brief node の pos 番めの入力に対応する枝を得る．
inline
void
Fsim3::find_simedge(const TgNode* node,
		    ymuint pos,
		    SimNode*& simnode,
		    ymuint& ipos) const
{
  const EdgeMap& edge_map = mEdgeMap[node->gid()][pos];
  simnode = edge_map.mNode;
  ipos = edge_map.mPos;
}

END_NAMESPACE_YM_ATPG_FSIM3

#endif // FSIM3_FSIM3_H
