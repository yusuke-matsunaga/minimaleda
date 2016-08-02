#ifndef LIBYM_AIG_CUTSWEEP_H
#define LIBYM_AIG_CUTSWEEP_H

/// @file libym_aig/tests/CutSweep.h
/// @brief CutSweep のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: CutSweep.h 1978 2009-02-06 12:29:16Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_aig/AigNetwork.h"
#include "ym_aig/AigBlifReader.h"
#include "ym_aig/AigBlifWriter.h"
#include "ym_aig/AigCutEnum.h"


BEGIN_NAMESPACE_YM_AIG

class CutSweep :
  public AigCutEnum
{
public:

  /// @brief 処理の最初に呼ばれる関数
  /// @param[in] network 対象のサブジェクトグラフ
  /// @param[in] limit カットサイズ
  /// @param[in] mode カット列挙モード
  /// @note デフォルトの実装ではなにもしない．
  virtual
  void
  all_init(const AigNetwork& network,
	   size_t limit);

  /// @brief cut が一つ見つかったときに呼ばれる関数(non-trivial cut)
  /// @param[in] root 根のノード
  /// @param[in] ni 入力数
  /// @param[in] inputs 入力ノードの配列
  /// @note デフォルトの実装ではなにもしない．
  virtual
  void
  found(AigNode* root,
	size_t ni,
	AigNode* inputs[]);
  
  /// @brief node のカット集合のフットプリントを得る．
  virtual
  vector<AigNode*>&
  cnode_list(AigNode* node);

  
private:
  
  ymulong
  calc_vect(AigNode* node);
  
  void
  clear_vect(AigNode* node);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ローカル関数計算用の作業領域
  vector<ymulong> mTmpVect;
  
  // mTmpVect に有効な値が設定されているとき true
  vector<bool> mValid;

  // cnode_list の配列
  vector<vector<AigNode*> > mCnodeListArray;
  
};

END_NAMESPACE_YM_AIG

#endif // LIBYM_AIG_CUTSWEEP_H
