#ifndef YM_AIG_AIGMGR_H
#define YM_AIG_AIGMGR_H

/// @file ym_aig/AigMgr.h
/// @brief AigMgrのヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
/// 
/// $Id: AigMgr.h 2274 2009-06-10 07:45:29Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_aig/AigHandle.h"
#include "ym_lexp/LogExpr.h"
#include "ym_sat/SatSolver.h"


BEGIN_NAMESPACE_YM_AIG

class AigMgrImpl;

//////////////////////////////////////////////////////////////////////
/// @class AigMgr AigMgr.h <ym_aig/AigMgr.h>
/// @brief AIGを管理するクラス(の純粋仮想クラス)
//////////////////////////////////////////////////////////////////////
class AigMgr
{
public:
  
  /// @brief コンストラクタ
  AigMgr();
  
  /// @brief デストラクタ
  ~AigMgr();

  
public:
  //////////////////////////////////////////////////////////////////////
  // 情報を取得するメンバ関数
  //////////////////////////////////////////////////////////////////////
  
  /// @brief 入力ノード数を得る．
  size_t
  input_num() const;

  /// @brief 入力ノードを取り出す．
  /// @param[in] pos 入力番号 ( 0 <= pos < input_num() )
  AigNode*
  input_node(size_t pos) const;

  /// @brief ノード数を得る．
  size_t
  node_num() const;

  /// @brief ノードを取り出す．
  /// @param[in] pos ノード番号 ( 0 <= pos < node_num() )
  /// @note ANDノードの他に入力ノードも含まれる．
  AigNode*
  node(size_t pos) const;


public:
  //////////////////////////////////////////////////////////////////////
  // 構造(AigNode)を作成するメンバ関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 定数0関数をつくる．
  AigHandle
  make_zero();

  /// @brief 定数1関数をつくる．
  AigHandle
  make_one();

  /// @brief 外部入力を作る．
  AigHandle
  make_input();
  
  /// @brief 2つのノードの AND を取る．
  /// @param[in] edge1, edge2 入力の AIG ハンドル
  AigHandle
  make_and(AigHandle edge1,
	   AigHandle edge2);

  /// @brief 2つのノードの OR を取る．
  /// @param[in] edge1, edge2 入力の AIG ハンドル
  AigHandle
  make_or(AigHandle edge1,
	  AigHandle edge2);

  /// @brief 2つのノードの XOR を取る．
  /// @param[in] edge1, edge2 入力の AIG ハンドル
  AigHandle
  make_xor(AigHandle edge1,
	   AigHandle edge2);

  /// @brief 論理式に対応するノード(木)をつくる．
  /// @param[in] expr 対象の論理式
  /// @param[in] inputs 入力に対応する AIG ハンドル
  AigHandle
  make_logic(const LogExpr& expr,
	     const vector<AigHandle>& inputs);
  
  /// @brief コファクターを計算する．
  /// @param[in] edge 対象の AIG ハンドル
  /// @param[in] id コファクターをとる変数番号
  /// @param[in] pol 極性
  AigHandle
  make_cofactor(AigHandle edge,
		tVarId id,
		tPol pol);
  

public:
  //////////////////////////////////////////////////////////////////////
  // SAT solver とのインターフェイス
  //////////////////////////////////////////////////////////////////////
  
  /// @brief SAT 問題を解く．
  /// @param[in] solver SAT-solver
  /// @param[in] edge この出力を1にできるか調べる．
  /// @param[out] model 外部入力の割り当てを入れる配列
  /// @retval kB3False 充足不能
  /// @retval kB3True 充足可能
  /// @retval kB3X 不明
  Bool3
  sat(SatSolver* solver,
      AigHandle edge,
      vector<Bool3>& model);

  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 実際の AigMgr の実装
  AigMgrImpl* mImpl;
  
};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 2つのノードの OR を取る．
inline
AigHandle
AigMgr::make_or(AigHandle edge1,
		AigHandle edge2)
{
  return ~make_and(~edge1, ~edge2);
}

// @brief 2つのノードの XOR を取る．
inline
AigHandle
AigMgr::make_xor(AigHandle edge1,
		 AigHandle edge2)
{
  return make_or(make_and(edge1, ~edge2), make_and(~edge1, edge2));
}

END_NAMESPACE_YM_AIG

#endif // YM_AIG_AIGMGR_H
