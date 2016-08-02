#ifndef DTPG_CLASSIC_PGRAPH_H
#define DTPG_CLASSIC_PGRAPH_H

/// @file src/dtpg_classic/PGraph.h
/// @brief PGraph のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: PGraph.h 2128 2009-03-12 07:25:33Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "dtpgcl_nsdef.h"


BEGIN_NAMESPACE_YM_ATPG_DTPGCL

class Gate;

//////////////////////////////////////////////////////////////////////
/// @class PGraph PGraph.h "PGraph.h"
/// @brief propagation graph を表すクラス
//////////////////////////////////////////////////////////////////////
class PGraph
{
public:

  /// @brief コンストラクタ
  PGraph();

  /// @brief デストラクタ
  ~PGraph();


public:
  
  /// @brief PGraph を構築する．
  void
  construct(Gate* f_gate);

  /// @brief PGraph を消去する．
  void
  destruct();

  /// @brief 変更フラグをクリアする．
  void
  clear_changes();

  /// @brief 変更フラグを調べる．
  bool
  check_changes() const;

  /// @brief 故障差のなくなったゲートを追加する．
  void
  add_indif(Gate* gate);

  /// @brief 故障差のなくなったゲートの情報をクリアする．
  void
  clear_indif();

  /// @brief 更新する．
  /// @return 故障伝搬の可能性がなくなったら false を返す．
  bool
  update();

  /// @brief 内容をダンプする(デバッグ用)
  void
  dump(ostream& s) const;
  

private:

  /// @brief destruct() の下請け関数
  static
  void
  dest_sub(Gate* gate);
  
  /// @brief dump() の下請け関数
  static
  void
  dump_sub(Gate* gate,
	   hash_set<ymuint>& mark,
	   ostream& s);
  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 故障箇所のソースとなっているゲート
  Gate* mFaultSrc;
  
  // 変更の有無を記録するフラグ
  bool mChangeFlag;
  
  // 故障差のなくなったゲートを入れておくリスト
  vector<Gate*> mIndifGates;
  
};

extern
PGraph pgraph;


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 変更フラグをクリアする．
inline
void
PGraph::clear_changes()
{
  mChangeFlag = false;
}

// @brief 変更フラグを調べる．
inline
bool
PGraph::check_changes() const
{
  return mChangeFlag;
}

// @brief 故障差のなくなったゲートを追加する．
inline
void
PGraph::add_indif(Gate* gate)
{
  mIndifGates.push_back(gate);
}

// @brief 故障差のなくなったゲートの情報をクリアする．
inline
void
PGraph::clear_indif()
{
  mIndifGates.clear();
}

END_NAMESPACE_YM_ATPG_DTPGCL

#endif
