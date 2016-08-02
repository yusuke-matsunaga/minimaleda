#ifndef CALC_SVF_CALCSVF_H
#define CALC_SVF_CALCSVF_H

/// @file calc_svf/CalcSvf.h
/// @brief CalcSvf のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
/// 
/// $Id: CalcSvf.h 1920 2008-12-20 15:52:42Z matsunaga $
///
/// Copyright (C) 2005-2008 Yusuke Matsunaga
/// All rights reserved.

#include "nsdef.h"
#include "seal_utils.h"
#include <ym_tgnet/TgGateTemplate.h>
#include <ym_tgnet/TgNode.h>
#include "EventQ.h"
#include "TestVector.h"
#include <ym_lexp/LogExpr.h>
#include <ym_utils/Alloc.h>
#include "SimFFR.h"


BEGIN_NAMESPACE_YM_SEAL_SVF

class SimNode;

//////////////////////////////////////////////////////////////////////
/// @class CalcSvf CalcSvf.h "CalcSvf.h"
/// @brief SER 解析器
//////////////////////////////////////////////////////////////////////
class CalcSvf
{
public:

  /// @brief コンストラクタ
  CalcSvf();
  
  /// @brief デストラクタ
  ~CalcSvf();


public:

  /// @brief シミュレーション対象のネットワークをセットする．
  void
  set_network(const TgNetwork& network,
	      size_t time_frame,
	      bool dss);

  /// @brief 全てのノードの出力に対する観測性の計算を行う．
  /// @param[in] tv_array テストベクタの配列
  void
  calc_exact(const vector<TestVector*>& tv_array);

  /// @brief 全てのノードの出力に対する観測性の計算を行う．
  /// @param[in] tv_array テストベクタの配列
  void
  calc_exact2(const vector<TestVector*>& tv_array);

  /// @brief 全てのノードの出力に対する観測性の最小値もどきの計算を行う．
  /// @param[in] tv_array テストベクタの配列
  void
  calc_pseudo_min(const vector<TestVector*>& tv_array);

  /// @brief 全てのノードの出力に対する観測性の最大値の計算を行う．
  /// @param[in] tv_array テストベクタの配列
  void
  calc_max(const vector<TestVector*>& tv_array);
  
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

  /// @brief node に対応する SimNode を得る．
  SimNode*
  find_simnode(const TgNode* node,
	       size_t tf) const;

  /// @brief logic ノードを作る．
  SimNode*
  make_logic(const LogExpr& lexp,
	     const vector<SimNode*>& inputs);

  /// @brief logic ノードを作る．
  SimNode*
  make_node(tTgGateType type,
	    const vector<SimNode*>& inputs);
  
  /// @brief DSS を求める．
  void
  find_dss();

  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////
  
  // SimNode の確保用アロケータ
  SimpleAlloc mNodeAlloc;
  
  // 対象のネットワーク
  const TgNetwork* mNetwork;
  
  // mSimMap のオフセット
  size_t mSimMapOffset;
  
  // TgNode の gid をキーにして SimNode を入れる配列
  vector<SimNode*> mSimMap;
  
  // 全ての SimNode を納めた配列
  vector<SimNode*> mNodeArray;

  // 外部入力に対応する SimNode を納めた配列
  vector<SimNode*> mInputArray;

  // 外部出力に対応する SimNode を納めた配列
  vector<SimNode*> mOutputArray;

  // 1フレーム目の出力に対応する SimNode を納めた配列
  vector<SimNode*> mOutput1Array;

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
CalcSvf::find_simnode(const TgNode* node,
		      size_t tf) const
{
  return mSimMap[node->gid() + mSimMapOffset * tf];
}

END_NAMESPACE_YM_SEAL_SVF

#endif // CALC_SVF_CALCSVF_H
