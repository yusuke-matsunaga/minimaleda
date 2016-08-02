#ifndef BDDSIM_SIMNODE_H
#define BDDSIM_SIMNODE_H

/// @file bddsim/SimNode.h
/// @brief SimNode のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
/// 
/// $Id: SimNode.h 1920 2008-12-20 15:52:42Z matsunaga $
///
/// Copyright (C) 2005-2007 Yusuke Matsunaga
/// All rights reserved.

#include "nsdef.h"
#include <ym_tgnet/TgGateTemplate.h>
#include <ym_lexp/LogExpr.h>
#include <ym_bdd/Bdd.h>


BEGIN_NAMESPACE_YM_SEAL_BDDSIM

//////////////////////////////////////////////////////////////////////
/// @class SimNode SimNode.h "SimNode.h"
/// @brief 故障シミュレーション用のノード
//////////////////////////////////////////////////////////////////////
class SimNode
{
  friend class BddsimCmd;
  friend class EventQ;
  
private:

  /// @brief 入力ノードのコンストラクタ
  explicit
  SimNode(ymuint32 id);

  /// @brief logic 用のノードのコンストラクタ
  SimNode(ymuint32 id,
	  tTgGateType type,
	  const vector<SimNode*>& inputs);

  /// @brief logic 用のノードのコンストラクタ
  SimNode(ymuint32 id,
	  tTgGateType type,
	  const LogExpr& lexp,
	  const vector<SimNode*>& inputs);
  
  /// @brief デストラクタ
  ~SimNode();


public:
  //////////////////////////////////////////////////////////////////////
  // 構造に関する情報の取得
  //////////////////////////////////////////////////////////////////////

  /// @brief ID番号を返す．
  ymuint32
  id() const;
  
  /// @brief 出力ノードの時 true を返す．
  bool
  is_output() const;
  
  /// @brief logic ノードのゲートタイプを返す．
  tTgGateType
  gate_type() const;

  /// @brief ファンイン数を得る．
  size_t
  nfi() const;

  /// @brief pos 番目のファンインを得る．
  SimNode*
  fanin(size_t pos) const;

  /// @brief ファンアウト数を得る．
  size_t
  nfo() const;

  /// @brief pos 番目のファンアウトを得る．
  SimNode*
  fanout(size_t pos) const;

  /// @brief レベルを得る．
  size_t
  level() const;


public:
  //////////////////////////////////////////////////////////////////////
  // 故障シミュレーションに関する情報の取得/設定
  //////////////////////////////////////////////////////////////////////
  
  /// @brief 正常値のセットを行う．
  /// @param[in] func セットする関数
  /// @note 通常は外部入力に対して行われる．
  /// @note 故障値も同様にセットされる．
  void
  set_gfunc(const Bdd& func);

  /// @brief 正常値を得る．
  Bdd
  get_gfunc() const;

  /// @brief 故障値をセットする．
  /// @param[in] func セットする関数
  void
  set_ffunc(const Bdd& func);

  /// @brief 故障値を得る．
  Bdd
  get_ffunc() const;
  
  /// @brief 正常値の計算を行う．
  /// @note 結果は mGfunc と mFfunc にセットされる．
  void
  calc_gfunc();

  /// @brief 故障値の計算を行う．
  /// @return 故障差関数を返す．
  Bdd
  calc_ffunc();

  /// @brief obs の値を得る．
  Bdd
  obs() const;

  /// @brief obs マークを得る．
  bool
  obs_mark() const;

  /// @brief obs の値をセットする．
  void
  set_obs(const Bdd& obs);

  /// @brief obs の値をクリアする．
  void
  clear_obs();

  /// @brief 入力の可観測性を再帰的に計算する．
  void
  calc_iobs(const Bdd& obs,
	    bool force = false);
   
  /// @brief 入力の擬似最小 obs を計算する．
  void
  calc_pseudo_min_iobs();
  
  /// @brief 入力の最大 obs を計算する．
  void
  calc_max_iobs();

  /// @brief target マークを得る．
  bool
  target() const;

  /// @brief target マークをつける．
  void
  set_target();

  /// @brief target マークを消す．
  void
  clear_target();

  /// @brief キューに積まれていたら true を返す．
  bool
  in_queue() const;

  /// @brief キューフラグをセットする．
  void
  set_queue();

  /// @brief キューフラグをクリアする．
  void
  clear_queue();

  /// @brief 出力マークをつける．
  void
  set_output();

  /// @brief レベルを設定する．
  void
  set_level(size_t level);

  /// @brief ファンアウトリストを作成する．
  void
  set_fanout_list(const vector<SimNode*>& fo_list);

  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ID 番号
  ymuint32 mId;

  // ゲートタイプ
  tTgGateType mGateType;

  // 論理式
  LogExpr mLexp;
  
  // ファンイン数
  size_t mNfi;

  // ファンインの配列
  SimNode** mFanins;
  
  // ファンアウトリストの要素数
  size_t mNfo;
  
  // ファンアウトリスト
  SimNode** mFanouts;
  
  // FFR の根のノード
  SimNode* mFfrRoot;
  
  // レベル
  size_t mLevel;
  
  // 正常値
  Bdd mGfunc;

  // 故障値
  Bdd mFfunc;

  // 可観測性マスク
  Bdd mObs;
  
  // 計算用の作業領域
  static
  vector<Bdd> mTmp1;
  static
  vector<Bdd> mTmp2;
  
  // obs と target フラグ
  ymuint32 mFlags;

  // イベントキューの次の要素
  SimNode* mLink;

};


//////////////////////////////////////////////////////////////////////
// SimNode のインライン関数
//////////////////////////////////////////////////////////////////////

// @brief ID番号を返す．
inline
ymuint32
SimNode::id() const
{
  return mId;
}

// @brief 出力ノードの時 true を返す．
inline
bool
SimNode::is_output() const
{
  return static_cast<bool>(mFlags & (1UL << 1));
}

// @brief logic ノードのゲートタイプを返す．
inline
tTgGateType
SimNode::gate_type() const
{
  return mGateType;
}

// @brief ファンイン数を得る．
inline
size_t
SimNode::nfi() const
{
  return mNfi;
}

// @brief pos 番目のファンインを得る．
inline
SimNode*
SimNode::fanin(size_t pos) const
{
  return mFanins[pos];
}

// @brief ファンアウト数を得る．
inline
size_t
SimNode::nfo() const
{
  return mNfo;
}

// @brief pos 番目のファンアウトを得る．
inline
SimNode*
SimNode::fanout(size_t pos) const
{
  return mFanouts[pos];
}

// @brief レベルを得る．
inline
size_t
SimNode::level() const
{
  return mLevel;
}

// @brief 正常値のセットを行う．
// @param[in] func 値
// @note 通常は外部入力に対して行われる．
// @note 故障値も同様にセットされる．
inline
void
SimNode::set_gfunc(const Bdd& func)
{
  mGfunc = func;
  mFfunc = func;
}

// @brief 正常値を得る．
inline
Bdd
SimNode::get_gfunc() const
{
  return mGfunc;
}

// @brief 故障値をセットする．
inline
void
SimNode::set_ffunc(const Bdd& pat)
{
  mFfunc = pat;
}

// @brief 故障値を得る．
inline
Bdd
SimNode::get_ffunc() const
{
  return mFfunc;
}

// @brief obs の値を得る．
inline
Bdd
SimNode::obs() const
{
  return mObs;
}

// @brief キューに積まれていたら true を返す．
inline
bool
SimNode::in_queue() const
{
  return static_cast<bool>((mFlags >> 2) & 1UL);
}

// @brief キューフラグをセットする．
inline
void
SimNode::set_queue()
{
  mFlags |= (1UL << 2);
}

// @brief キューフラグをクリアする．
inline
void
SimNode::clear_queue()
{
  mFlags &= ~(1UL << 2);
}

// @brief obs マークを得る．
inline
bool
SimNode::obs_mark() const
{
  return static_cast<bool>((mFlags >> 3) & 1UL);
}

// @brief obs の値をセットする．
inline
void
SimNode::set_obs(const Bdd& val)
{
  mObs = val;
  mFlags |= (1UL << 3);
}

// @brief obs の値をクリアする．
inline
void
SimNode::clear_obs()
{
  mFlags &= ~(1UL << 3);
}

// @brief target マークを得る．
inline
bool
SimNode::target() const
{
  return static_cast<bool>((mFlags >> 4) & 1UL);
}

// @brief target マークをつける．
inline
void
SimNode::set_target()
{
  mFlags |= (1UL << 4);
}

// @brief target マークを消す．
inline
void
SimNode::clear_target()
{
  mFlags &= ~(1UL << 4);
}

END_NAMESPACE_YM_SEAL_BDDSIM

#endif // BDDSIM_SIMNODE
