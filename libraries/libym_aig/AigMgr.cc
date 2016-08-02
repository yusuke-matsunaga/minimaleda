
/// @file libym_aig/AigMgr.cc
/// @brief AigMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
/// 
/// $Id: AigMgr.cc 2274 2009-06-10 07:45:29Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_aig/AigMgr.h"
#include "ym_aig/AigNode.h"
#include "AigMgrImpl.h"


BEGIN_NAMESPACE_YM_AIG

//////////////////////////////////////////////////////////////////////
// クラス AigMgr
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
AigMgr::AigMgr() :
  mImpl(new AigMgrImpl)
{
}
  
// @brief デストラクタ
AigMgr::~AigMgr()
{
  delete mImpl;
}
  
// @brief 入力ノード数を得る．
size_t
AigMgr::input_num() const
{
  return mImpl->input_num();
}

// @brief 入力ノードを取り出す．
// @param[in] pos 入力番号 ( 0 <= pos < input_num() )
AigNode*
AigMgr::input_node(size_t pos) const
{
  return mImpl->input_node(pos);
}

// @brief ノード数を得る．
size_t
AigMgr::node_num() const
{
  return mImpl->node_num();
}

// @brief ノードを取り出す．
// @param[in] pos ノード番号 ( 0 <= pos < input_num() )
// @note ANDノードの他に入力ノードも含まれる．
AigNode*
AigMgr::node(size_t pos) const
{
  return mImpl->node(pos);
}

// @brief 定数0関数をつくる．
AigHandle
AigMgr::make_zero()
{
  return mImpl->make_zero();
}

// @brief 定数1関数をつくる．
AigHandle
AigMgr::make_one()
{
  return mImpl->make_one();
}

// @brief 外部入力を作る．
AigHandle
AigMgr::make_input()
{
  return mImpl->make_input();
}

// @brief AND ノードを作る．
AigHandle
AigMgr::make_and(AigHandle handle1,
		 AigHandle handle2)
{
  return mImpl->make_and(handle1, handle2);
}

// @brief 論理式に対応するノード(木)をつくる．
AigHandle
AigMgr::make_logic(const LogExpr& expr,
		   const vector<AigHandle>& inputs)
{
  if ( expr.is_zero() ) {
    return make_zero();
  }
  if ( expr.is_one() ) {
    return make_one();
  }
  if ( expr.is_posiliteral() ) {
    tVarId id = expr.varid();
    assert_cond(id < inputs.size(), __FILE__, __LINE__);
    return inputs[id];
  }
  if ( expr.is_negaliteral() ) {
    tVarId id = expr.varid();
    assert_cond(id < inputs.size(), __FILE__, __LINE__);
    return ~inputs[id];
  }
  if ( expr.is_and() ) {
    size_t n = expr.child_num();
    AigHandle ans = make_logic(expr.child(0), inputs);
    for (size_t i = 1; i < n; ++ i) {
      AigHandle tmp = make_logic(expr.child(i), inputs);
      ans = make_and(ans, tmp);
    }
    return ans;
  }
  if ( expr.is_or() ) {
    size_t n = expr.child_num();
    AigHandle ans = make_logic(expr.child(0), inputs);
    for (size_t i = 1; i < n; ++ i) {
      AigHandle tmp = make_logic(expr.child(i), inputs);
      ans = make_or(ans, tmp);
    }
    return ans;
  }
  if ( expr.is_xor() ) {
    size_t n = expr.child_num();
    AigHandle ans = make_logic(expr.child(0), inputs);
    for (size_t i = 1; i < n; ++ i) {
      AigHandle tmp = make_logic(expr.child(i), inputs);
      ans = make_xor(ans, tmp);
    }
    return ans;
  }
  assert_not_reached(__FILE__, __LINE__);
  return make_zero();
}
  
// @brief コファクターを計算する．
// @param[in] edge 対象の AIG ハンドル
// @param[in] id コファクターをとる変数番号
// @param[in] pol 極性
AigHandle
AigMgr::make_cofactor(AigHandle edge,
		      tVarId id,
		      tPol pol)
{
  if ( edge.is_const() ) {
    // edge が定数の時は変更なし
    return edge;
  }
  
  AigNode* node = edge.node();
  AigHandle ans;
  if ( node->is_input() ) {
    // 入力ノード時は番号が id どうかで処理が変わる．
    if ( node->input_id() == id ) {
      if ( pol == kPolPosi ) {
	ans = make_one();
      }
      else {
	ans = make_zero();
      }
    }
    else {
      ans = AigHandle(node, false);
    }
  }
  else {
    // AND ノードの場合
    // 2つの子供に再帰的な処理を行って結果の AND を計算する．
    AigHandle new_handle0 = make_cofactor(node->fanin0_handle(), id, pol);
    AigHandle new_handle1 = make_cofactor(node->fanin1_handle(), id, pol);
    AigHandle ans = make_and(new_handle0, new_handle1);
  }
  if ( edge.inv() ) {
    ans = ~ans;
  }
  return ans;
}
  
// @brief SAT 問題を解く．
// @param[in] solver SAT-solver
// @param[in] edge この出力を1にできるか調べる．
// @param[out] model 外部入力の割り当てを入れる配列
// @retval kB3False 充足不能
// @retval kB3True 充足可能
// @retval kB3X 不明
Bool3
AigMgr::sat(SatSolver* solver,
	    AigHandle edge,
	    vector<Bool3>& model)
{
  return kB3X;
}

#if 0
// @brief 構造に対応した CNF を作成する．
// @param[in] solver SAT ソルバ
// @param[out] varidmap AigNode の ID 番号をキーにして SAT 変数番号を入れる配列
void
AigMgr::make_cnf(SatSolver& solver,
		 vector<ymuint>& varidmap)
{
  ymuint n = node_num();
  varidmap.clear();
  varidmap.resize(n);
  for (ymuint i = 0; i < n; ++ i) {
    AigNode* aignode = node(i);
    tVarId id = solver.new_var();
    varidmap[i] = id;
    if ( !aignode->is_input() ) {
      ymuint id0 = varidmap[aignode->fanin0()->node_id()];
      tPol pol0 = aignode->fanin0_inv() ? kPolNega : kPolPosi;
      ymuint id1 = varidmap[aignode->fanin1()->node_id()];
      tPol pol1 = aignode->fanin1_inv() ? kPolNega : kPolPosi;
      Literal lito(id, kPolPosi);
      Literal lit1(id0, pol0);
      Literal lit2(id1, pol1);
      solver.add_clause(~lit1, ~lit2, lito);
      solver.add_clause( lit1, ~lito);
      solver.add_clause( lit2, ~lito);
      AigHandle rep = aignode->rep_handle();
      if ( rep.node() != aignode ) {
	ymuint rep_id = varidmap[rep.node()->node_id()];
	tPol rep_pol = rep.inv() ? kPolNega : kPolPosi;
	Literal rep_lit(rep_id, rep_pol);
	solver.add_clause( lito, ~rep_lit);
	solver.add_clause(~lito,  rep_lit);
      }
    }
  }

}
#endif

END_NAMESPACE_YM_AIG
