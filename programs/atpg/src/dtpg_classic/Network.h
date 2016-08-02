#ifndef DTPG_CLASSIC_NETWORK_H
#define DTPG_CLASSIC_NETWORK_H

/// @file src/dtpg_classic/Network.h
/// @brief Network のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: Network.h 2128 2009-03-12 07:25:33Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "dtpgcl_nsdef.h"
#include "Val3.h"
#include "ym_tgnet/tgnet_nsdef.h"
#include "ym_tgnet/TgNode.h"


BEGIN_NAMESPACE_YM_ATPG

class SaFault;

END_NAMESPACE_YM_ATPG


BEGIN_NAMESPACE_YM_ATPG_DTPGCL

class Gate;
class GateSa;

//////////////////////////////////////////////////////////////////////
/// @class Network Network.h "Network.h"
/// @brief テスト対象回路を表すクラス
//////////////////////////////////////////////////////////////////////
class Network
{
public:

  /// @brief コンストラクタ
  Network();

  /// @brief デストラクタ
  ~Network();


public:
  //////////////////////////////////////////////////////////////////////
  // 内容を設定する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容をクリアする．
  void
  clear();

  /// @brief 内容を設定する．
  /// @param[in] src もとのネットワーク
  void
  set(const TgNetwork& src);
  

public:
  //////////////////////////////////////////////////////////////////////
  // 構造に関する情報を取得する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 外部入力数を得る．
  ymuint
  npi() const;

  /// @brief 外部出力数を得る．
  ymuint
  npo() const;

  /// @brief 全ゲート数を得る．
  /// @note 外部入力数も含む．
  ymuint
  ngate() const;

  /// @brief 外部入力を得る．
  /// @param[in] pos 位置番号 ( 0 <= pos < npi() )
  Gate*
  input(ymuint pos) const;

  /// @brief 外部出力を得る．
  /// @param[in] pos 位置番号 ( 0 <= pos < npo() )
  Gate*
  output(ymuint pos) const;

  /// @brief ゲートを得る．
  /// @param[in] pos 位置番号 ( 0 <= pos < ngate() )
  /// @note 0 <= pos < npi() では input(pos) と同じものが返される．
  Gate*
  gate(ymuint pos) const;

  /// @brief node に対応するゲートを返す．
  Gate*
  gate(const TgNode* node) const;

  /// @brief 最大レベルを得る．
  ymuint
  max_level() const;

  
public:
  //////////////////////////////////////////////////////////////////////
  // アクティベイトに関係する関数
  //////////////////////////////////////////////////////////////////////
  
  /// @brief 全体をアクティブにする．
  void
  activate_all();

  /// @brief 指定されたゲートの TFI of TFO のみアクティブにする．
  bool
  activate_tfitfo(Gate* pivot);

  /// @brief 全体をアクティブでなくす．
  void
  deactivate();
  

public:
  //////////////////////////////////////////////////////////////////////
  // 故障ノードの挿入に関する関数
  //////////////////////////////////////////////////////////////////////
  
  /// @brief f に対応する故障ノードを挿入する．
  void
  inject_fault(SaFault* f);

  /// @brief f に対応する故障ノードを取り除く．
  void
  remove_fault(SaFault* f);

  
public:
  //////////////////////////////////////////////////////////////////////
  // その他の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 全ゲートのマークを消す．
  void
  clear_mark();
  

private:
  //////////////////////////////////////////////////////////////////////
  // 下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief テスト容易性指標の計算を行う．
  /// @note 副作用でファンイン/ファンアウトの順序づけも行われる．
  void
  t_analyze();

  /// @brief activate_tfitfo() の下請け関数
  bool
  mark_tfo(Gate* gate);
  
  /// @brief 構造的に等価なゲートを求める．
  void
  set_eq_gate();
  
  /// @brief basis node を求める．
  void
  calc_basis();
  
  /// @brief 故障ノードを挿入する．
  void
  insert(Gate* i_gate,
	 ymuint opos,
	 Gate* o_gate,
	 ymuint ipos,
	 Gate* f_gate);

  /// @brief 故障ノードを挿入する．(外部出力の場合)
  void
  insert(Gate* i_gate,
	 Gate* f_gate);
  
  /// @brief 新しい故障ノードを取り出す．
  /// @param[in] fval 故障値
  GateSa*
  new_fault(Val3 fval);
  
  /// @brief 故障ノードを削除する．
  void
  delete_fault(GateSa* f_gate);
  
  /// @brief ゲートを生成する．
  /// @param[in] node オリジナルのノード
  /// @param[in] input_id 入力番号
  /// @note input_id は入力ノードの時のみ意味を持つ．
  /// @note 自動的に ID 番号が振られる．
  Gate*
  new_Gate(const TgNode* node,
	   ymuint input_id = 0);

  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////
  
  // 全ゲートを納めるリスト
  vector<Gate*> mGateList;

  // 外部出力に対応したゲートを納めるリスト
  vector<Gate*> mOutputList;

  // 外部入力数
  ymuint32 mNpi;

  // アクティブなゲートを納めるリスト
  vector<Gate*> mActGateList;

  // 最大レベル
  ymuint32 mMaxLevel;

  // TgNode::gid() をキーとしてゲートを納める配列
  vector<Gate*> mGateMap;
  
  // 0縮退故障ノードを入れておくリスト
  list<GateSa*> mSa0List;

  // 1縮退故障ノードを入れておくリスト
  list<GateSa*> mSa1List;

};

extern
Network gn;


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 外部入力数を得る．
inline
ymuint
Network::npi() const
{
  return mNpi;
}

// @brief 外部出力数を得る．
inline
ymuint
Network::npo() const
{
  return mOutputList.size();
}

// @brief 全ゲート数を得る．
// @note 外部入力数も含む．
inline
ymuint
Network::ngate() const
{
  return mGateList.size();
}

// @brief 外部入力を得る．
// @param[in] pos 位置番号 ( 0 <= pos < npi() )
inline
Gate*
Network::input(ymuint pos) const
{
  return mGateList[pos];
}

// @brief 外部出力を得る．
// @param[in] pos 位置番号 ( 0 <= pos < npo() )
inline
Gate*
Network::output(ymuint pos) const
{
  return mOutputList[pos];
}

// @brief ゲートを得る．
// @param[in] pos 位置番号 ( 0 <= pos < ngate() )
// @note 0 <= pos < npi() では input(pos) と同じものが返される．
inline
Gate*
Network::gate(ymuint pos) const
{
  return mGateList[pos];
}

// @brief node に対応するゲートを返す．
inline
Gate*
Network::gate(const TgNode* node) const
{
  return mGateMap[node->gid()];
}

// @brief 最大レベルを得る．
inline
ymuint
Network::max_level() const
{
  return mMaxLevel;
}

END_NAMESPACE_YM_ATPG_DTPGCL

#endif
