#ifndef CALC_CVF_CALCCVF_H
#define CALC_CVF_CALCCVF_H

/// @file calc_cvf/CalcCvf.h
/// @brief CalcCvf のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
/// 
/// $Id: CalcCvf.h 1978 2009-02-06 12:29:16Z matsunaga $
///
/// Copyright (C) 2005-2008 Yusuke Matsunaga
/// All rights reserved.

#include "seal_nsdef.h"
#include "seal_utils.h"
#include <ym_tgnet/TgGateTemplate.h>
#include <ym_tgnet/TgNode.h>
#include "EventQ.h"
#include "TestVector.h"
#include <ym_lexp/LogExpr.h>
#include <ym_utils/Alloc.h>
#include "SimFFR.h"


BEGIN_NAMESPACE_YM_SEAL_CVF

class SimNode;

//////////////////////////////////////////////////////////////////////
/// @class CalcCvf CalcCvf.h "CalcCvf.h"
/// @brief SER 解析器
//////////////////////////////////////////////////////////////////////
class CalcCvf
{
public:

  /// @brief コンストラクタ
  CalcCvf();
  
  /// @brief デストラクタ
  ~CalcCvf();


public:

  /// @brief シミュレーション対象のネットワークをセットする．
  void
  set_network(const TgNetwork& network,
	      bool dss,
	      bool new_algorithm = true);

  /// @brief 全てのノードの出力に対する観測性の計算を行う．
  /// @param[in] tv_array テストベクタの配列
  void
  calc_exact(const vector<TestVector*>& tv_array);

  /// @brief 全てのノードの出力に対する観測性の最小値もどきの計算を行う．
  /// @param[in] tv_array テストベクタの配列
  void
  calc_pseudo_min(const vector<TestVector*>& tv_array);

  /// @brief 全てのノードの出力に対する観測性の最大値の計算を行う．
  /// @param[in] tv_array テストベクタの配列
  /// @param[in] ns サンプリング数
  void
  calc_max(const vector<TestVector*>& tv_array,
	   size_t ns);
  
  /// @brief node の出力における可観測性パタンを返す．
  tPackedVal
  get_obs(const TgNode* node);
  

private:
  
  /// @brief 正常値のシミュレーションを行う．
  /// @param[in] tv_array テストベクタの配列
  void
  calc_gval(const vector<TestVector*>& tv_array);

  /// @brief SimNode のネットワークをダンプする．
  void
  dump(ostream& s) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データをセットするために使用される関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 現在保持している SimNode のネットワークを破棄する．
  void
  clear();
  
  /// @brief DSS を求める．
  void
  find_dss(bool new_algorithm);
  
  /// @brief node に対応する SimNode を得る．
  SimNode*
  find_simnode(const TgNode* node) const;

  /// @brief logic ノードを作る．
  SimNode*
  make_logic(const LogExpr& lexp,
	     const vector<SimNode*>& inputs);

  /// @brief logic ノードを作る．
  SimNode*
  make_node(tTgGateType type,
	    const vector<SimNode*>& inputs);

  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////
  
  // SimNode の確保用アロケータ
  SimpleAlloc mNodeAlloc;
  
  // 対象のネットワーク
  const TgNetwork* mNetwork;
  
  // TgNode の gid をキーにして SimNode を入れる配列
  vector<SimNode*> mSimMap;
  
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

  // fval を元にもどすためにノードを入れておく配列
  vector<SimNode*> mClearArray;
  
};

// @brief node に対応する SimNode* を得る．
inline
SimNode*
CalcCvf::find_simnode(const TgNode* node) const
{
  return mSimMap[node->gid()];
}

END_NAMESPACE_YM_SEAL_CVF

#endif // CALC_CVF_CALCCVF_H
