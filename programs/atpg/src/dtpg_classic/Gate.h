#ifndef DTPG_CLASSIC_GATE_H
#define DTPG_CLASSIC_GATE_H

/// @file src/dtpg_classic/Gate.h
/// @brief Gate のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: Gate.h 2203 2009-04-16 05:04:40Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "dtpgcl_nsdef.h"
#include "Val3.h"
#include "ym_tgnet/TgGateTemplate.h"
#include "LqElem.h"



BEGIN_NAMESPACE_YM_ATPG_DTPGCL

class Network;
class Gate;

//////////////////////////////////////////////////////////////////////
/// @class Gate Gate.h "Gate.h"
/// @brief パタン生成のためのゲート構造を表すクラス
//////////////////////////////////////////////////////////////////////
class Gate :
  public LqElem
{
  friend class Network;
  friend class SearchMgr;
  
public:

  /// @brief コンストラクタ
  /// @param[in] id ID 番号
  /// @param[in] node もとのノード
  /// @param[in] ni 入力数
  Gate(ymuint id,
       const TgNode* node,
       ymuint ni);

  /// @brief デストラクタ
  virtual
  ~Gate();


public:
  //////////////////////////////////////////////////////////////////////
  // 入出力の接続情報
  //////////////////////////////////////////////////////////////////////

  /// @brief 入力数を得る．
  ymuint
  nfi() const;

  /// @brief ファンアウト数を得る．
  ymuint
  nfo() const;

  /// @brief アクティブなファンアウト数を得る．
  ymuint
  act_nfo() const;

  /// @brief ファンインを取り出す．
  Gate*
  fanin(ymuint pos) const;

  /// @brief ソート順にファンインを取り出す．
  Gate*
  sorted_fanin(ymuint pos) const;

  /// @brief ソート順を取り出す．
  ymuint
  sorted_pos(ymuint pos) const;

  /// @brief ファンアウトのゲートを得る．
  Gate*
  fanout(ymuint pos) const;

  /// @brief ファンアウトの入力位置を得る．
  ymuint
  fanout_ipos(ymuint pos) const;

  /// @brief アクティブなファンアウトのゲートを得る．
  Gate*
  act_fanout(ymuint pos) const;

  /// @brief アクティブなファンアウトの入力位置を得る．
  ymuint
  act_fanout_ipos(ymuint pos) const;


public:
  //////////////////////////////////////////////////////////////////////
  // その他の属性
  //////////////////////////////////////////////////////////////////////
  
  /// @brief ID番号を得る．
  ymuint
  id() const;
  
  /// @brief ゲート名を得る．
  virtual
  const char*
  name() const;

  /// @brief ゲート型を得る．
  virtual
  tTgGateType
  gate_type() const;
  
  /// @brief 名前とゲート型を表す文字列を返す．
  virtual
  string
  description() const;
  
  /// @brief 外部入力の時 true を返す．
  bool
  is_pi() const;

  /// @brief 外部入力の時 入力番号を返す．
  /// @note デフォルトでは 0 を返す．
  virtual
  ymuint
  input_id() const;

  /// @brief 外部出力の時 true を返す．
  bool
  is_po() const;
  
  /// @brief 外部出力でなくす．
  void
  rst_po();

  /// @breif FOS のゲートを得る．
  Gate*
  fos() const;

  /// @brief FOS のゲートの時 true を返す．
  bool
  is_fos() const;
  
  /// @brief レベルを返す．
  ymuint
  level() const;

  /// @brief 到達可能な外部出力のレベルの最小値を返す．
  ymuint
  min_level() const;

  /// @brief 制御値を得る．
  /// @note デフォルトでは X を返す．
  virtual
  Val3
  c_val() const;

  /// @brief 非制御値を得る．
  Val3
  nc_val() const { return ~c_val(); }

  /// @brief 制御出力値を得る．
  /// @note デフォルトでは X を返す．
  virtual
  Val3
  o_val() const;
  
  /// @brief 非制御出力値を得る．
  Val3
  no_val() const { return ~o_val(); }

  
public:
  //////////////////////////////////////////////////////////////////////
  // 探索用の値のアクセス関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 正常値を返す．
  Val3
  gval() const;

  /// @brief 正常値を設定する．
  void
  set_gval(Val3 val);

  /// @brief 正常値を設定し，正当化フラグを立てる．
  void
  set_gjval(Val3 val);

  /// @brief 正常値が正当化されていたら true を返す．
  bool
  chk_gj() const;

  /// @brief 正常値の正当化フラグを立てる．
  void
  set_gj();

  /// @brief 正常値の正当化フラグを下ろす．
  void
  rst_gj();

  /// @brief 故障値を返す．
  Val3
  fval() const;

  /// @brief 故障値を設定する．
  void
  set_fval(Val3 val);

  /// @brief 故障値を設定し，正当化フラグを立てる．
  void
  set_fjval(Val3 val);

  /// @brief 故障値が正当化されていたら true を返す．
  bool
  chk_fj() const;

  /// @brief 故障値の正当化フラグを立てる．
  void
  set_fj();

  /// @brief 故障値の正当化フラグを下ろす．
  void
  rst_fj();

  /// @brief 両方の値を同時にセットする．
  void
  set_hval(Val3 val);

  /// @brief 両方の値を同時にセットし，両方の正当化フラグを立てる．
  void
  set_hjval(Val3 val);
  
  /// @brief 正常値と故障値から mState の値を作る．
  static
  ymuint32
  gf2state(Val3 gval,
	   Val3 fval);

  /// @brief mState の値から正常値を取り出す．
  static
  Val3
  state2gval(ymuint32 state);

  /// @brief mState の値から故障値を取り出す．
  static
  Val3
  state2fval(ymuint32 state);

  
public:
  //////////////////////////////////////////////////////////////////////
  // Propagation Graph に関する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief Propagation Graph に含まれていたら true を返す．(PGフラグ)
  bool
  chk_pg() const;

  /// @brief PG フラグを立てる．
  void
  set_pg();

  /// @brief PG フラグを下ろす
  void
  rst_pg();

  /// @brief 故障値を伝搬していたら true を返す． (DIFフラグ)
  bool
  chk_dif() const;

  /// @brief DIF フラグを立てる．
  void
  set_dif();

  /// @brief DIF フラグを下ろす．
  void
  rst_dif();

  /// @brief PG フラグと DIF フラグを立てる．
  void
  set_pg_dif();

  /// @brief PG フラグと DIF フラグを下ろす．
  void
  rst_pg_dif();
  

public:
  //////////////////////////////////////////////////////////////////////
  // その他の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief マークが付いているか調べる．
  bool
  chk_mark() const;

  /// @brief マークをつける．
  void
  set_mark();
  
  /// @brief マークを消す．
  void
  rst_mark();
  
  /// @brief フラグをすべて下ろす．
  void
  clr_flag();
  
  /// @brief アクティブなゲートか調べる．
  bool
  is_active() const;
  
  /// @brief 外部出力で正当化されていたら true を返す．
  bool
  chk_POJ() const;
  
  /// @brief 基底ノードの時 true を返す． 
  bool
  is_basis() const;

  /// @brief fault-cone に含まれているか調べる．
  bool
  chk_fcone() const;

  /// @brief fcone にする．
  void
  set_fcone();

  /// @brief fcone でなくす．
  void
  rst_fcone();
  
  /// @brief lvlq に入っているか調べる．
  bool
  chk_lvlq();

  /// @brief lvlq マークをつける．
  void
  set_lvlq();

  /// @brief lvlq マークを消す．
  void
  rst_lvlq();
  
  /// @brief primary path かどうか調べる．
  bool
  chk_ppath() const;

  /// @brief ppath フラグを立てる．
  void
  set_ppath();

  /// @brief ppath フラグを下ろす．
  void
  rst_ppath();

  /// @brief block フラグを調べる．
  bool
  chk_block() const;

  /// @brief block フラグを立てる．
  void
  set_block();

  /// @brief block フラグを下ろす．
  void
  rst_block();

  /// @brief 定数フラグを調べる．
  bool
  chk_const() const;

  /// @brief 定数フラグを立てる．
  void
  set_const();

  /// @brief 定数フラグを下ろす．
  void
  rst_const();

  /// @brief CFNA フラグを調べる．
  bool
  chk_cfna(Val3 val) const;

  /// @brief CFNA0 フラグを調べる．
  bool
  chk_cfna0() const;

  /// @brief CFNA0 フラグを立てる．
  void
  set_cfna0();

  /// @brief CFNA0 フラグを下ろす．
  void
  rst_cfna0();

  /// @brief CFNA1 フラグを調べる．
  bool
  chk_cfna1() const;

  /// @brief CFNA1 フラグを立てる．
  void
  set_cfna1();

  /// @brief CFNA1 フラグを下ろす．
  void
  rst_cfna1();

  /// @brief scope フラグを調べる．
  bool
  chk_scope() const;

  /// @brief scope フラグを立てる．
  void
  set_scope();

  /// @brief scope フラグを下ろす．
  void
  rst_scope();

  /// @brief 0 の設定要求を即正当化できるか調べる．
  bool
  chk_term0() const;
  
  /// @brief 1 の設定要求を即正当化できるか調べる．
  bool
  chk_term1() const;
  
  /// @brief val の設定要求を即正当化できるか調べる．
  bool
  chk_term(Val3 val) const;

  
public:
  //////////////////////////////////////////////////////////////////////
  // testability measure の計算関係の関数
  //////////////////////////////////////////////////////////////////////
  
  /// @brief 可制御性を計算する．
  virtual
  void
  calc_C() = 0;

  /// @brief 可観測性を計算する．
  virtual
  ymuint
  calc_O(ymuint pos) = 0;


public:
  //////////////////////////////////////////////////////////////////////
  // パタン生成関係の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 現在の値割り当てでの故障伝搬性を計算する．
  virtual
  bool
  calc_obs(ymuint ipos) const = 0;
  
  /// @brief 正当化のチェックを行う．
  virtual
  void
  chk_just();
  
  /// @brief 正常値/故障値の含意を行う．
  bool
  imp0_h();
  
  /// @brief 正常値/故障値の含意を行う．
  bool
  imp1_h();
  
  /// @brief 正常値/故障値の含意を行う．
  bool
  imp0_hb();
  
  /// @brief 正常値/故障値の含意を行う．
  bool
  imp1_hb();
  
  /// @brief 正常値の含意を行う．
  bool
  imp0_g();
  
  /// @brief 正常値の含意を行う．
  bool
  imp1_g();
  
  /// @brief 正常値の含意を行う．
  bool
  imp0_gb();
  
  /// @brief 正常値の含意を行う．
  bool
  imp1_gb();
  
  /// @brief 故障値の含意を行う．
  bool
  imp0_f();
  
  /// @brief 故障値の含意を行う．
  bool
  imp1_f();
  
  /// @brief 故障値の含意を行う．
  bool
  imp0_fb();
  
  /// @brief 故障値の含意を行う．
  bool
  imp1_fb();
  
  
public:
  
  /// @brief 正常値/故障値の前方含意を行う．
  virtual
  bool
  fwd_imp0_h() = 0;
  
  /// @brief 正常値/故障値の前方含意を行う．
  virtual
  bool
  fwd_imp1_h() = 0;
  
  /// @brief 正常値の前方含意を行う．
  virtual
  bool
  fwd_imp0_g() = 0;
  
  /// @brief 正常値の前方含意を行う．
  virtual
  bool
  fwd_imp1_g() = 0;
  
  /// @brief 故障値の前方含意を行う．
  virtual
  bool
  fwd_imp0_f() = 0;
  
  /// @brief 故障値の前方含意を行う．
  virtual
  bool
  fwd_imp1_f() = 0;
  
  /// @brief 正常値/故障値の後方含意を行う．
  virtual
  bool
  bwd_imp0_h() = 0;
  
  /// @brief 正常値/故障値の後方含意を行う．
  virtual
  bool
  bwd_imp1_h() = 0;
  
  /// @brief 正常値の後方含意を行う．
  virtual
  bool
  bwd_imp0_g() = 0;
  
  /// @brief 正常値の後方含意を行う．
  virtual
  bool
  bwd_imp1_g() = 0;
  
  /// @brief 故障値の後方含意を行う．
  virtual
  bool
  bwd_imp0_f() = 0;
  
  /// @brief 故障値の後方含意を行う．
  virtual
  bool
  bwd_imp1_f() = 0;

  /// @brief 論理的に等価なゲートを得る．
  Gate*
  eq_gate() const;
  
  /// @brief 論理的に等価なゲートに含意を行う．
  bool
  eq_imp0();
  
  /// @brief 論理的に等価なゲートに含意を行う．
  bool
  eq_imp1();
  
  
public:
  
  /// @brief 故障ノードのときに true を返す．
  /// @note デフォルトでは false を返す．
  virtual
  bool
  is_f_site() const;
  

protected:
  //////////////////////////////////////////////////////////////////////
  // GateInput が使う関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 外部入力にする．
  void
  set_pi();
  
  
protected:
  //////////////////////////////////////////////////////////////////////
  // 多入力ゲートの派生クラスが使う関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ソート順を設定する．
  void
  set_sorted_pos(ymuint pos,
		 ymuint s_pos);
  

private:
  //////////////////////////////////////////////////////////////////////
  // 接続関係を設定する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファンインを設定する．
  void
  set_fanin(ymuint pos,
	     Gate* input);

  /// @brief ファンアウトを設定する．
  void
  set_fanout(ymuint pos,
	     Gate* output,
	     ymuint ipos);

  /// @brief ファンアウト用配列の確保を行う．
  void
  init_fogate(ymuint no);

  /// @brief アクティブなファンアウトを設定する．
  void
  set_act_fanout(ymuint pos,
		 Gate* output,
		 ymuint ipos);

  /// @brief アクティブなファンアウト数を設定する．
  void
  set_act_no(ymuint no);
  

private:
  //////////////////////////////////////////////////////////////////////
  // 属性を設定する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 外部出力にする．
  void
  set_po();

  /// @brief FOS のゲートを設定する．
  void
  set_fos(Gate* gate);

  /// @brief アクティブにする．
  void
  set_active();

  /// @brief アクティブでなくす．
  void
  rst_active();

  /// @brief 基底ノードにする．
  void
  set_basis();

  /// @brief 基底ノードでなくす．
  void
  rst_basis(); 


private:
  //////////////////////////////////////////////////////////////////////
  // mFlags 操作用の下請け関数
  //////////////////////////////////////////////////////////////////////

  bool
  chk_generic(ymuint32 bitpat) const;
  
  bool
  chk_OR_generic(ymuint32 bitpat) const;
  
  void
  set_generic(ymuint32 bitpat);
  
  void
  rst_generic(ymuint32 bitpat);
  

private:
  //////////////////////////////////////////////////////////////////////
  // このクラス内で用いる型定義
  //////////////////////////////////////////////////////////////////////

  /// @brief ファンアウトブランチを表す構造体
  struct Fob
  {
    Gate* mGate;
    ymuint32 mIpos;
  };
  
  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////
  
  // ID 番号
  ymuint32 mId;

  // もとのノード
  const TgNode* mTgNode;

  // フラグの集まり
  ymuint32 mFlags;

  // 汎用のマーク用カウンタ
  ymuint32 mMarkCount;
  
  // Fanout Stem のノード
  Gate* mFos;
  
  // ファンイン数
  ymuint32 mNi;

  // ファンインの配列
  Gate** mFanins;

  // 優先順位の配列
  ymuint32* mSortedPos;
  
  // ファンアウト数
  ymuint32 mNo;

  // ファンアウトの配列
  Fob* mFanouts;

  // アクティブなファンアウト数
  ymuint32 mActNo;

  // アクティブなファンアウトの配列
  Fob* mActFanouts;
  
  // レベル
  ymuint32 mLevel;
  
  // 到達可能な外部出力の最小レベル
  ymuint32 mMinLevel;
  
  // for DTPG
  ymuint32 mState;
  
  // for learning
  Gate* mEqGate;
  
  
public:
  
  // for unique sensitization
  ymuint32 side_val;
  
  
private:
  //////////////////////////////////////////////////////////////////////
  // このクラスに固有の定数
  //////////////////////////////////////////////////////////////////////
  
  static
  const ymuint J_SHIFT = 2;
  static
  const ymuint F_SHIFT = 3;
  static
  const ymuint P_SHIFT = 6;
  static
  const ymuint D_SHIFT = 7;
  
  static
  const ymuint32 GVAL3_MASK    = (1 << J_SHIFT) - 1;
  static
  const ymuint32 GJ_MASK       = 1 << J_SHIFT;
  static
  const ymuint32 GSTATE_MASK   = GVAL3_MASK | GJ_MASK;
  
  static
  const ymuint32 FVAL3_MASK    = GVAL3_MASK << F_SHIFT;
  static
  const ymuint32 FJ_MASK       = GJ_MASK << F_SHIFT;
  static
  const ymuint32 FSTATE_MASK   = FVAL3_MASK | FJ_MASK;
  
  static
  const ymuint32 J_MASK        = GJ_MASK | FJ_MASK;
  
  static
  const ymuint32 P_MASK        = 1 << P_SHIFT;
  static
  const ymuint32 D_MASK        = 1 << D_SHIFT;

  static
  const ymuint32 GN_active     = 1 << 0;
  static
  const ymuint32 GN_PI	       = 1 << 1;
  static
  const ymuint32 GN_PO	       = 1 << 2;
  static
  const ymuint32 GN_basis      = 1 << 3;
  static
  const ymuint32 GN_fcone      = 1 << 4;
  static
  const ymuint32 GN_lvlq       = 1 << 5;
  static
  const ymuint32 GN_ppath      = 1 << 6;
  static
  const ymuint32 GN_block      = 1 << 7;
  static
  const ymuint32 GN_const      = 1 << 8;
  static
  const ymuint32 GN_cfna0      = 1 << 9;
  static
  const ymuint32 GN_cfna1      = 1 << 10;
  static
  const ymuint32 GN_scope      = 1 << 11;
  
  static
  const ymuint32 GN_term0      = GN_cfna0 | GN_const;
  static
  const ymuint32 GN_term1      = GN_cfna1 | GN_const;

};
  

/// @brief mMarkCount 用の変数
extern
ymuint gn_common_count;


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 入力数を得る．
inline
ymuint
Gate::nfi() const
{
  return mNi;
}

// @brief ファンアウト数を得る．
inline
ymuint
Gate::nfo() const
{
  return mNo;
}

// @brief アクティブなファンアウト数を得る．
inline
ymuint
Gate::act_nfo() const
{
  return mActNo;
}

// @brief アクティブなファンアウト数を設定する．
inline
void
Gate::set_act_no(ymuint no)
{
  mActNo = no;
}

// @brief ファンインを取り出す．
inline
Gate*
Gate::fanin(ymuint pos) const
{
  return mFanins[pos];
}

// @brief ファンインを設定する．
inline
void
Gate::set_fanin(ymuint pos,
		 Gate* input)
{
  mFanins[pos] = input;
}

// @brief ソート順にファンインを取り出す．
inline
Gate*
Gate::sorted_fanin(ymuint pos) const
{
  return mFanins[sorted_pos(pos)];
}

// @brief ソート順を取り出す．
inline
ymuint
Gate::sorted_pos(ymuint pos) const
{
  return mSortedPos[pos];
}

// @brief ソート順を設定する．
inline
void
Gate::set_sorted_pos(ymuint pos,
		     ymuint s_pos)
{
  mSortedPos[pos] = s_pos;
}

// @brief ファンアウトのゲートを得る．
inline
Gate*
Gate::fanout(ymuint pos) const
{
  return mFanouts[pos].mGate;
}

// @brief ファンアウトの入力位置を得る．
inline
ymuint
Gate::fanout_ipos(ymuint pos) const
{
  return mFanouts[pos].mIpos;
}

// @brief ファンアウトを設定する．
inline
void
Gate::set_fanout(ymuint pos,
		 Gate* output,
		 ymuint ipos)
{
  Fob& fob = mFanouts[pos];
  fob.mGate = output;
  fob.mIpos = ipos;
}

// @brief アクティブなファンアウトのゲートを得る．
inline
Gate*
Gate::act_fanout(ymuint pos) const
{
  return mActFanouts[pos].mGate;
}

// @brief アクティブなファンアウトの入力位置を得る．
inline
ymuint
Gate::act_fanout_ipos(ymuint pos) const
{
  return mActFanouts[pos].mIpos;
}

// @brief アクティブなファンアウトを設定する．
inline
void
Gate::set_act_fanout(ymuint pos,
		     Gate* output,
		     ymuint ipos)
{
  Fob& fob = mActFanouts[pos];
  fob.mGate = output;
  fob.mIpos = ipos;
}
  
// @brief ID番号を得る．
inline
ymuint
Gate::id() const
{
  return mId;
}

// @breif FOS のゲートを得る．
inline
Gate*
Gate::fos() const
{
  return mFos;
}

// @brief FOS のゲートを設定する．
inline
void
Gate::set_fos(Gate* gate)
{
  mFos = gate;
}

// @brief FOS のゲートの時 true を返す．
inline
bool
Gate::is_fos() const
{
  return this == mFos;
}
    
// @brief レベルを返す．
inline
ymuint
Gate::level() const
{
  return mLevel;
}

// @brief 到達可能な外部出力のレベルの最小値を返す．
inline
ymuint
Gate::min_level() const
{
  return mMinLevel;
}

// @brief 正常値と故障値から mState の値を作る．
inline
ymuint32
Gate::gf2state(Val3 gval,
	       Val3 fval)
{
  return gval | (fval << F_SHIFT);
}

// @brief mState の値から正常値を取り出す．
inline
Val3
Gate::state2gval(ymuint32 state)
{
  return static_cast<Val3>(state & GVAL3_MASK);
}

// @brief mState の値から故障値を取り出す．
inline
Val3
Gate::state2fval(ymuint32 state)
{
  return static_cast<Val3>((state & FVAL3_MASK) >> F_SHIFT);
}

// @brief 正常値を返す．
inline
Val3
Gate::gval() const
{
  return state2gval(mState);
}

// @brief 正常値を設定する．
inline
void
Gate::set_gval(Val3 val)
{
  mState &= ~GSTATE_MASK;
  mState |= val;
}

// @brief 正常値を設定し，正当化フラグを立てる．
inline
void
Gate::set_gjval(Val3 val)
{
  mState &= ~GVAL3_MASK;
  mState |= val | GJ_MASK;
}

// @brief 正常値が正当化されていたら true を返す．
inline
bool
Gate::chk_gj() const
{
  return (mState & GJ_MASK) != 0;
}

// @brief 正常値の正当化フラグを立てる．
inline
void
Gate::set_gj()
{
  mState |= GJ_MASK;
}

// @brief 正常値の正当化フラグを下ろす．
inline
void
Gate::rst_gj()
{
  mState &= ~GJ_MASK;
}

// @brief 故障値を返す．
inline
Val3
Gate::fval() const
{
  return state2fval(mState);
}

// @brief 故障値を設定する．
inline
void
Gate::set_fval(Val3 val)
{
  mState &= ~FSTATE_MASK;
  mState |= (val << F_SHIFT);
}

// @brief 故障値を設定し，正当化フラグを立てる．
inline
void
Gate::set_fjval(Val3 val)
{
  mState &= ~FVAL3_MASK;
  mState |= (val << F_SHIFT) | FJ_MASK;
}

// @brief 故障値が正当化されていたら true を返す．
inline
bool
Gate::chk_fj() const
{
  return (mState & FJ_MASK) != 0;
}

// @brief 故障値の正当化フラグを立てる．
inline
void
Gate::set_fj()
{
  mState |= FJ_MASK;
}

// @brief 故障値の正当化フラグを下ろす．
inline
void
Gate::rst_fj()
{
  mState &= ~FJ_MASK;
}

// @brief 両方の値を同時にセットする．
inline
void
Gate::set_hval(Val3 val)
{
  mState &= ~(GSTATE_MASK | FSTATE_MASK);
  mState |= gf2state(val, val);
}

// @brief 両方の値を同時にセットし，両方の正当化フラグを立てる．
inline
void
Gate::set_hjval(Val3 val)
{
  mState &= ~(GVAL3_MASK | FVAL3_MASK);
  mState |= gf2state(val, val) | GJ_MASK | FJ_MASK;
}

// @brief Propagation Graph に含まれていたら true を返す．(PGフラグ)
inline
bool
Gate::chk_pg() const
{
  return (mState & P_MASK) != 0;
}

// @brief PG フラグを立てる．
inline
void
Gate::set_pg()
{
  mState |= P_MASK;
}

// @brief PG フラグを下ろす
inline
void
Gate::rst_pg()
{
  mState &= ~P_MASK;
}

// @brief 故障値を伝搬していたら true を返す． (DIFフラグ)
inline
bool
Gate::chk_dif() const
{
  return (mState & D_MASK) != 0;
}

// @brief DIF フラグを立てる．
inline
void
Gate::set_dif()
{
  mState |= D_MASK;
}

// @brief DIF フラグを下ろす．
inline
void
Gate::rst_dif()
{
  mState &= ~D_MASK;
}

// @brief PG フラグと DIF フラグを立てる．
inline
void
Gate::set_pg_dif()
{
  mState |= (P_MASK | D_MASK);
}

// @brief PG フラグと DIF フラグを下ろす．
inline
void
Gate::rst_pg_dif()
{
  mState &= ~ (P_MASK | D_MASK);
}

// @brief マークが付いているか調べる．
inline
bool
Gate::chk_mark() const
{
  return mMarkCount == gn_common_count;
}

// @brief マークをつける．
inline
void
Gate::set_mark()
{
  mMarkCount = gn_common_count;
}
  
// @brief マークを消す．
inline
void
Gate::rst_mark()
{
  mMarkCount = 0;
}
  
// @brief フラグをすべて下ろす．
inline
void
Gate::clr_flag()
{
  mFlags = 0;
}
  
// @brief アクティブなゲートか調べる．
inline
bool
Gate::is_active() const
{
  return chk_generic(GN_active);
}

// @brief アクティブにする．
inline
void
Gate::set_active()
{
  set_generic(GN_active);
}

// @brief アクティブでなくす．
inline
void
Gate::rst_active()
{
  rst_generic(GN_active);
}

// @brief 外部入力の時 true を返す．
inline
bool
Gate::is_pi() const
{
  return chk_generic(GN_PI);
}
  
// @brief 外部入力にする．
inline
void
Gate::set_pi()
{
  set_generic(GN_PI);
}

// @brief 外部出力の時 true を返す．
inline
bool
Gate::is_po() const
{
  return chk_generic(GN_PO);
}
  
// @brief 外部出力にする．
inline
void
Gate::set_po()
{
  set_generic(GN_PO);
}
  
// @brief 外部出力でなくす．
inline
void
Gate::rst_po()
{
  rst_generic(GN_PO);
}
  
// @brief 外部出力で正当化されていたら true を返す．
inline
bool
Gate::chk_POJ() const
{
  return is_po() && ((mState & J_MASK) == J_MASK);
}
  
// @brief 基底ノードの時 true を返す．
inline
bool
Gate::is_basis() const
{
  return chk_generic(GN_basis);
}

// @brief 基底ノードにする．
inline
void
Gate::set_basis()
{
  set_generic(GN_basis);
}

// @brief 基底ノードでなくす．
inline
void
Gate::rst_basis()
{
  rst_generic(GN_basis);
}

// @brief fault-cone に含まれているか調べる．
inline
bool
Gate::chk_fcone() const
{
  return chk_generic(GN_fcone);
}

// @brief fcone にする．
inline
void
Gate::set_fcone()
{
  set_generic(GN_fcone);
}

// @brief fcone でなくす．
inline
void
Gate::rst_fcone()
{
  rst_generic(GN_fcone);
}
  
// @brief lvlq に入っているか調べる．
inline
bool
Gate::chk_lvlq()
{
  return chk_generic(GN_lvlq);
}

// @brief lvlq マークをつける．
inline
void
Gate::set_lvlq()
{
  set_generic(GN_lvlq);
}

// @brief lvlq マークを消す．
inline
void
Gate::rst_lvlq()
{
  rst_generic(GN_lvlq);
}

// @brief primary path かどうか調べる．
inline
bool
Gate::chk_ppath() const
{
  return chk_generic(GN_ppath);
}

// @brief ppath フラグを立てる．
inline
void
Gate::set_ppath()
{
  set_generic(GN_ppath);
}

// @brief ppath フラグを下ろす．
inline
void
Gate::rst_ppath()
{
  rst_generic(GN_ppath);
}

// @brief block フラグを調べる．
inline
bool
Gate::chk_block() const
{
  return chk_generic(GN_block);
}

// @brief block フラグを立てる．
inline
void
Gate::set_block()
{
  set_generic(GN_block);
}

// @brief block フラグを下ろす．
inline
void
Gate::rst_block()
{
  rst_generic(GN_block);
}

// @brief 定数フラグを調べる．
inline
bool
Gate::chk_const() const
{
  return chk_generic(GN_const);
}

// @brief 定数フラグを立てる．
inline
void
Gate::set_const()
{
  set_generic(GN_const);
}

// @brief 定数フラグを下ろす．
inline
void
Gate::rst_const()
{
  rst_generic(GN_const);
}

// @brief CFNA0 フラグを調べる．
inline
bool
Gate::chk_cfna(Val3 val) const
{
  if ( val == kVal0 ) {
    return chk_generic(GN_cfna0);
  }
  else {
    return chk_generic(GN_cfna1);
  }
}

// @brief CFNA0 フラグを調べる．
inline
bool
Gate::chk_cfna0() const
{
  return chk_generic(GN_cfna0);
}

// @brief CFNA0 フラグを立てる．
inline
void
Gate::set_cfna0()
{
  set_generic(GN_cfna0);
}

// @brief CFNA0 フラグを下ろす．
inline
void
Gate::rst_cfna0()
{
  rst_generic(GN_cfna0);
}

// @brief CFNA1 フラグを調べる．
inline
bool
Gate::chk_cfna1() const
{
  return chk_generic(GN_cfna1);
}

// @brief CFNA1 フラグを立てる．
inline
void
Gate::set_cfna1()
{
  set_generic(GN_cfna1);
}

// @brief CFNA1 フラグを下ろす．
inline
void
Gate::rst_cfna1()
{
  rst_generic(GN_cfna1);
}

// @brief scope フラグを調べる．
inline
bool
Gate::chk_scope() const
{
  return chk_generic(GN_scope);
}

// @brief scope フラグを立てる．
inline
void
Gate::set_scope()
{
  set_generic(GN_scope);
}

// @brief scope フラグを下ろす．
inline
void
Gate::rst_scope()
{
  rst_generic(GN_scope);
}

// @brief 0 の設定要求を即正当化できるか調べる．
inline
bool
Gate::chk_term0() const
{
  return chk_OR_generic(GN_term0) && !chk_generic(GN_fcone);
}

// @brief 1 の設定要求を即正当化できるか調べる．
inline
bool
Gate::chk_term1() const
{
  return chk_OR_generic(GN_term1) && !chk_generic(GN_fcone);
}
  
// @brief val の設定要求を即正当化できるか調べる．
inline
bool
Gate::chk_term(Val3 val) const
{
  if ( !chk_generic(GN_fcone) ) {
    if ( val == kVal0 ) {
      return chk_OR_generic(GN_term0);
    }
    else {
      return chk_OR_generic(GN_term1);
    }
  }
  return false;
}

inline
bool
Gate::chk_generic(ymuint32 bitpat) const
{
  return (mFlags & bitpat) == bitpat;
}

inline
bool
Gate::chk_OR_generic(ymuint32 bitpat) const
{
  return (mFlags & bitpat) != 0U;
}

inline
void
Gate::set_generic(ymuint32 bitpat)
{
  mFlags |= bitpat;
}

inline
void
Gate::rst_generic(ymuint32 bitpat)
{
  mFlags &= ~bitpat;
}

// @brief 論理的に等価なゲートを得る．
inline
Gate*
Gate::eq_gate() const
{
  return mEqGate;
}

END_NAMESPACE_YM_ATPG_DTPGCL

#endif
