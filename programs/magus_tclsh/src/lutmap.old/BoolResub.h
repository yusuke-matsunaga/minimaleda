#ifndef MAGUS_LUTMAP_BOOLRESUB_H
#define MAGUS_LUTMAP_BOOLRESUB_H

/// @file magus/lutmap/BoolResub.h
/// @brief BoolResub のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: BoolResub.h 2274 2009-06-10 07:45:29Z matsunaga $
///
/// Copyright (C) 2005-2006 Yusuke Matsunaga
/// All rights reserved.

#include <ym_utils/RandGen.h>
#include <ym_utils/Alloc.h>
#include <ym_lexp/LogExpr.h>
#include <ym_sat/sat_nsdef.h>

#include "lutmap.h"
#include "PatVect.h"


BEGIN_NAMESPACE_MAGUS_LUTMAP

class LnGraph;
class LnNode;

//////////////////////////////////////////////////////////////////////
/// @class BoolResub BoolResub.h "BoolResub.h"
/// @brief Boolean resubstitution を行うクラス
//////////////////////////////////////////////////////////////////////
class BoolResub
{
public:

  /// @brief コンストラクタ
  BoolResub();

  /// @brief デストラクタ
  virtual
  ~BoolResub();


public:
  
  /// @brief LUT の置き換えを行って LUT 数の削減を行う．
  /// @param[in] lutnetwork LUT ネットワーク
  /// @param[in] npat シミュレーションで用いるパタンサイズ
  void
  node_subst(LnGraph& lutnetwork,
	     ymuint npat);
  
  /// @brief LUT のマージを行って LUT 数の削減を行う．
  /// @param[in] lutnetwork LUT ネットワーク
  /// @param[in] max_ni LUT の最大入力数
  /// @param[in] npat シミュレーションで用いるパタンサイズ
  void
  node_merge(LnGraph& lutnetwork,
	     ymuint max_ni,
	     ymuint npat);

  
private:
  //////////////////////////////////////////////////////////////////////
  // 内部でのみ用いられる関数
  //////////////////////////////////////////////////////////////////////
  
  /// @brief データ構造を初期化する．
  void
  init(LnGraph& lutnetwork,
       ymuint npat);
       
  /// @brief global function の計算を行う．
  void
  calc_gf();

  /// @brief ODC の計算を行う．
  void
  calc_odc();

  /// @brief node の変化に合わせて global function と ODC の再計算を行う．
  void
  update_patvect(LnNode* node);
  
  /// @brief ノードの置き換えをチェックする．
  bool
  check_node_subst(LnNode* node1,
		   LnNode* node2);
  
  /// @brief ノードのマージをチェックする．
  bool
  check_node_merge(LnNode* node1,
		   LnNode* node2);
  
  void
  set_tfimark(LnNode* node);

  void
  clear_tfimark(LnNode* node);

  void
  set_tfomark(LnNode* node);

  void
  clear_tfomark(LnNode* node);
  
  void
  delete_tfi(LnNode* node);

  /// @brief ノードの入出力の関係を表す CNF クローズを生成する．
  /// @param[in] solver SAT ソルバ
  /// @param[in] lexp 論理式
  /// @param[in] output 出力リテラル
  /// @param[in] ipos_begin 入力リテラルの開始位置
  /// @param[in] ipos_end 入力リテラルの終了位置
  void
  make_cnf(SatSolver* solver,
	   const LogExpr& lexp,
	   Literal output,
	   const vector<Literal>& inputs);

  /// @brief 一定数以下のサポートを求める．
  /// @param[in] support もとのサポート
  /// @param[in] vect 仕様となる不完全指定論理関数
  /// @param[in] new_support 結果のサポートを納めるベクタ
  /// @param[in] new_vect 結果の真理値表
  /// @note サイズが mMaxNi 以下のサポートを探す．
  bool
  find_support(const vector<LnNode*>& support,
	       const vector<int>& vect,
	       vector<LnNode*>& new_support,
	       vector<int>& new_vect);
  
  /// @brief 真理値表から論理式を得る．
  static
  LogExpr
  tv2expr(ymuint ni,
	  const vector<int>& tv);
  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 乱数発生器
  RandGen mRandGen;
  
  // パタン数
  ymuint mNpat;
  
  // 対象の LUT network
  LnGraph* mLutNetwork;
  
  // mLutNetwork のノードをトポロジカル順に並べた配列
  vector<LnNode*> mNodeList;
  
  // LUT の最大入力数
  ymuint mMaxNi;

  // 汎用マーク
  vector<ymuint> mMark;
  
  // 削除マーク
  vector<bool> mDmark;

  // TFI マーク
  vector<bool> mTfimark;

  // TFO マーク
  vector<bool> mTfomark;

  // キュー1
  vector<LnNode*> mQueue1;

  // キュー2
  vector<LnNode*> mQueue2;

  // make_cnf 用のスタック
  vector<Literal> mLitStack;
  
  // make_cnf 用の作業領域
  vector<Literal> mTmpLit;
  
};

END_NAMESPACE_MAGUS_LUTMAP

#endif // MAGUS_LUTMAP_BOOLRESUB_H
