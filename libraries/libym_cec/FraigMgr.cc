
/// @file libym_cec/FraigMgr.cc
/// @brief FraigMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
/// 
/// $Id: FraigMgr.cc 2203 2009-04-16 05:04:40Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "FraigMgr.h"
#include "FraigMgrImpl.h"
#include "FraigNode.h"

#define DEBUG 0


BEGIN_NAMESPACE_YM_CEC

//////////////////////////////////////////////////////////////////////
// FraigMgr
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @brief sig_size シグネチャのサイズ
// @param[in] sat_type SAT-solver の種類を表す文字列
// @param[in] sat_option SAT-solver 生成用オプション文字列
// @param[in] sat_log ログの出力用ストリーム
FraigMgr::FraigMgr(ymuint sig_size,
		   const string& sat_type,
		   const string& sat_option,
		   ostream* sat_log) :
  mImpl(new FraigMgrImpl(sig_size, sat_type, sat_option, sat_log))
{
}

// @brief デストラクタ
FraigMgr::~FraigMgr()
{
  delete mImpl;
}
  
// @brief 入力ノード数を得る．
ymuint
FraigMgr::input_num() const
{
  return mImpl->input_num();
}

// @brief 入力ノードを取り出す．
// @param[in] pos 入力番号 ( 0 <= pos < input_num() )
FraigNode*
FraigMgr::input_node(ymuint pos) const
{
  return mImpl->input_node(pos);
}

// @brief ノード数を得る．
ymuint
FraigMgr::node_num() const
{
  return mImpl->node_num();
}

// @brief ノードを取り出す．
// @param[in] pos ノード番号 ( 0 <= pos < input_num() )
// @note ANDノードの他に入力ノードも含まれる．
FraigNode*
FraigMgr::node(ymuint pos) const
{
  return mImpl->node(pos);
}

// @brief 定数0関数をつくる．
FraigHandle
FraigMgr::make_zero()
{
  return mImpl->make_zero();
}

// @brief 定数1関数をつくる．
FraigHandle
FraigMgr::make_one()
{
  return mImpl->make_one();
}

// @brief 外部入力を作る．
FraigHandle
FraigMgr::make_input()
{
#if DEBUG
  cout << "make_input ...";
#endif
  FraigHandle ans = mImpl->make_input();
#if DEBUG
  cout << " -> " << ans << endl;
#endif
  return ans;
}

// @brief 2つのノードの AND を取る．
// @param[in] edge1, edge2 入力の AIG ハンドル
FraigHandle
FraigMgr::make_and(FraigHandle edge1,
		   FraigHandle edge2)
{
#if DEBUG
  cout << "make_and(" << edge1 << ", " << edge2 << ") ..." << endl;
#endif
  FraigHandle ans = mImpl->make_and(edge1, edge2);
#if DEBUG
  cout << "  -> " << ans << endl;
#endif
  return ans;
}

// @brief 論理式に対応するノード(木)をつくる．
// @param[in] expr 対象の論理式
// @param[in] inputs 入力に対応する AIG ハンドル
FraigHandle
FraigMgr::make_logic(const LogExpr& expr,
		     const vector<FraigHandle>& inputs)
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
    FraigHandle ans = make_logic(expr.child(0), inputs);
    for (size_t i = 1; i < n; ++ i) {
      FraigHandle tmp = make_logic(expr.child(i), inputs);
      ans = make_and(ans, tmp);
    }
    return ans;
  }
  if ( expr.is_or() ) {
    size_t n = expr.child_num();
    FraigHandle ans = make_logic(expr.child(0), inputs);
    for (size_t i = 1; i < n; ++ i) {
      FraigHandle tmp = make_logic(expr.child(i), inputs);
      ans = make_or(ans, tmp);
    }
    return ans;
  }
  if ( expr.is_xor() ) {
    size_t n = expr.child_num();
    FraigHandle ans = make_logic(expr.child(0), inputs);
    for (size_t i = 1; i < n; ++ i) {
      FraigHandle tmp = make_logic(expr.child(i), inputs);
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
FraigHandle
FraigMgr::make_cofactor(FraigHandle edge,
			tVarId id,
			tPol pol)
{
  if ( edge.is_const() ) {
    // edge が定数の時は変更なし
    return edge;
  }
  
  FraigNode* node = edge.node();
  FraigHandle ans;
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
      ans = FraigHandle(node, false);
    }
  }
  else {
    // AND ノードの場合
    // 2つの子供に再帰的な処理を行って結果の AND を計算する．
    FraigHandle new_handle0 = make_cofactor(node->fanin0_handle(), id, pol);
    FraigHandle new_handle1 = make_cofactor(node->fanin1_handle(), id, pol);
    FraigHandle ans = make_and(new_handle0, new_handle1);
  }
  if ( edge.inv() ) {
    ans = ~ans;
  }
  return ans;
}
  
// @brief 2つのハンドルが等価かどうか調べる．
Bool3
FraigMgr::check_equiv(FraigHandle aig1,
		      FraigHandle aig2)
{
  Bool3 stat = mImpl->check_equiv(aig1, aig2);
  return stat;
}

// @brief ログレベルを設定する．
void
FraigMgr::set_loglevel(int level)
{
  mImpl->set_loglevel(level);
}

// @brief ログ出力用ストリームを設定する．
void
FraigMgr::set_logstream(ostream* out)
{
  mImpl->set_logstream(out);
}

// @brief ランダムシミュレーション制御用のパラメータを設定する．
void
FraigMgr::set_loop_limit(size_t val)
{
  mImpl->set_loop_limit(val);
}
  
// @brief 内部の統計情報を出力する．
void
FraigMgr::dump_stats(ostream& s)
{
  mImpl->dump_stats(s);
}

END_NAMESPACE_YM_CEC
