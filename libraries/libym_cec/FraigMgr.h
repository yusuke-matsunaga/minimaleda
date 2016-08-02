#ifndef LIBYM_CEC_FRAIGMGR_H
#define LIBYM_CEC_FRAIGMGR_H

/// @file ym_cec/FraigMgr.h
/// @brief FraigMgr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
/// 
/// $Id: FraigMgr.h 2203 2009-04-16 05:04:40Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_cec/cec_nsdef.h"
#include "FraigHandle.h"
#include "ym_lexp/LogExpr.h"
#include "ym_sat/Bool3.h"


BEGIN_NAMESPACE_YM_CEC

class FraigMgrImpl;

//////////////////////////////////////////////////////////////////////
/// @class FraigMgr FraigMgr.h <ym_aig/FraigMgr.h>
/// @brief Functionary Reduced AND-INVERTOR Graph を管理するクラス
//////////////////////////////////////////////////////////////////////
class FraigMgr
{
 public:

  /// @brief コンストラクタ
  /// @brief sig_size シグネチャのサイズ
  /// @param[in] sat_type SAT-solver の種類を表す文字列
  /// @param[in] sat_option SAT-solver 生成用オプション文字列
  /// @param[in] sat_log ログの出力用ストリーム
  FraigMgr(ymuint sig_size,
	   const string& sat_type = string(),
	   const string& sat_option = string(),
	   ostream* sat_log = NULL);

  /// @brief デストラクタ
  ~FraigMgr();

  
public:
  //////////////////////////////////////////////////////////////////////
  // 情報を取得するメンバ関数
  //////////////////////////////////////////////////////////////////////
  
  /// @brief 入力ノード数を得る．
  ymuint
  input_num() const;

  /// @brief 入力ノードを取り出す．
  /// @param[in] pos 入力番号 ( 0 <= pos < input_num() )
  FraigNode*
  input_node(ymuint pos) const;

  /// @brief ノード数を得る．
  ymuint
  node_num() const;

  /// @brief ノードを取り出す．
  /// @param[in] pos ノード番号 ( 0 <= pos < input_num() )
  /// @note ANDノードの他に入力ノードも含まれる．
  FraigNode*
  node(ymuint pos) const;
  

public:
  //////////////////////////////////////////////////////////////////////
  // 構造(FraigNode)を作成するメンバ関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 定数0関数をつくる．
  FraigHandle
  make_zero();

  /// @brief 定数1関数をつくる．
  FraigHandle
  make_one();

  /// @brief 外部入力を作る．
  FraigHandle
  make_input();
  
  /// @brief 2つのノードの AND を取る．
  /// @param[in] edge1, edge2 入力の AIG ハンドル
  FraigHandle
  make_and(FraigHandle edge1,
	   FraigHandle edge2);

  /// @brief 2つのノードの OR を取る．
  /// @param[in] edge1, edge2 入力の AIG ハンドル
  FraigHandle
  make_or(FraigHandle edge1,
	  FraigHandle edge2);

  /// @brief 2つのノードの XOR を取る．
  /// @param[in] edge1, edge2 入力の AIG ハンドル
  FraigHandle
  make_xor(FraigHandle edge1,
	   FraigHandle edge2);

  /// @brief 論理式に対応するノード(木)をつくる．
  /// @param[in] expr 対象の論理式
  /// @param[in] inputs 入力に対応する AIG ハンドル
  FraigHandle
  make_logic(const LogExpr& expr,
	     const vector<FraigHandle>& inputs);
  
  /// @brief コファクターを計算する．
  /// @param[in] edge 対象の AIG ハンドル
  /// @param[in] id コファクターをとる変数番号
  /// @param[in] pol 極性
  FraigHandle
  make_cofactor(FraigHandle edge,
		tVarId id,
		tPol pol);


public:
  
  /// @brief 2つのハンドルが等価かどうか調べる．
  Bool3
  check_equiv(FraigHandle aig1,
	      FraigHandle aig2);
  
  /// @brief ログレベルを設定する．
  void
  set_loglevel(int level);
  
  /// @brief ログ出力用ストリームを設定する．
  void
  set_logstream(ostream* out);

  /// @brief ランダムシミュレーション制御用のパラメータを設定する．
  /// @param[in] loop_limit 変化のない状態がこの回数連続したら止める．
  void
  set_loop_limit(size_t loop_limit);
  
  /// @brief 内部の統計情報を出力する．
  void
  dump_stats(ostream& s);

  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////
  
  // 実際の処理を行う実装クラス
  FraigMgrImpl* mImpl;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 2つのノードの OR を取る．
// @param[in] edge1, edge2 入力の AIG ハンドル
inline
FraigHandle
FraigMgr::make_or(FraigHandle edge1,
		  FraigHandle edge2)
{
  return ~make_and(~edge1, ~edge2);
}

// @brief 2つのノードの XOR を取る．
// @param[in] edge1, edge2 入力の AIG ハンドル
inline
FraigHandle
FraigMgr::make_xor(FraigHandle edge1,
		   FraigHandle edge2)
{
  FraigHandle tmp1 = make_and(edge1, ~edge2);
  FraigHandle tmp2 = make_and(~edge1, edge2);
  return make_or(tmp1, tmp2);
}

END_NAMESPACE_YM_CEC

#endif // LIBYM_CEC_FRAIGMGR_H
