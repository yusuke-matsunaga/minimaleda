#ifndef CALC_CVF_SIMNODE_H
#define CALC_CVF_SIMNODE_H

/// @file calc_cvf/SimNode.h
/// @brief SimNode のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
/// 
/// $Id: SimNode.h 1978 2009-02-06 12:29:16Z matsunaga $
///
/// Copyright (C) 2005-2008 Yusuke Matsunaga
/// All rights reserved.


#include "seal_utils.h"
#include <ym_utils/RandGen.h>
#include "EqElem.h"
#include "SimFFR.h"
#include <ym_bdd/Bdd.h>


BEGIN_NAMESPACE_YM_SEAL_CVF

//////////////////////////////////////////////////////////////////////
/// @class SimNode SimNode.h "SimNode.h"
/// @brief 故障シミュレーション用のノード
//////////////////////////////////////////////////////////////////////
class SimNode :
  public EqElem
{
  friend class CalcCvf;
  
protected:

  /// @brief コンストラクタ
  SimNode(ymuint32 id);


  /// @brief デストラクタ
  virtual
  ~SimNode();


public:
  //////////////////////////////////////////////////////////////////////
  // 構造に関する情報の取得
  //////////////////////////////////////////////////////////////////////

  /// @brief ID番号を返す．
  ymuint32
  id() const;

  /// @brief ファンイン数を得る．
  virtual
  size_t
  nfi() const = 0;

  /// @brief pos 番めのファンインを得る．
  virtual
  SimNode*
  fanin(size_t pos) const = 0;

  /// @brief ファンアウト数を得る．
  size_t
  nfo() const;

  /// @brief pos 番目のファンアウトを得る．
  SimNode*
  fanout(size_t pos) const;
  
  /// @brief FFR を得る．
  SimFFR*
  ffr() const;

  /// @brief FFR の根のノードの時 true を返す．
  bool
  is_ffr_root() const;

  /// @brief レベルを得る．
  size_t
  level() const;

  /// @brief 出力ノードの時 true を返す．
  bool
  is_output() const;

  /// @brief 外部ノードの時 true を返す．
  bool
  is_external() const;


public:
  //////////////////////////////////////////////////////////////////////
  // 故障シミュレーションに関する情報の取得/設定
  //////////////////////////////////////////////////////////////////////
  
  /// @brief 正常値のセットを行う．
  /// @param[in] pat 値
  /// @note 通常は外部入力に対して行われる．
  /// @note 故障値も同様にセットされる．
  void
  set_gval(tPackedVal pat);

  /// @brief 正常値を得る．
  tPackedVal
  get_gval() const;

  /// @brief 故障値をセットする．
  void
  set_fval(tPackedVal pat);

  /// @brief 故障値を得る．
  tPackedVal
  get_fval() const;

  /// @brief 故障値をクリアする．
  void
  clear_fval();

  /// @brief 正常値の計算を行いセットする．
  /// @note 結果は mGval と mFval に格納される．
  /// @note mObs はクリアされる．
  void
  calc_gval();

  /// @brief 故障値の計算を行い故障差を返す．
  /// @param[in] mask マスク値
  /// @note 故障値のうちマスクビットが1の部分のみ書き換えられる．
  /// @note 返り値の故障差もマスクビットとの AND を取る．
  tPackedVal
  calc_fval(tPackedVal mask);
  
  /// @brief 出力の obs を設定する．
  void
  set_obs(tPackedVal val);
  
  /// @brief 出力の obs の bitwise-OR を計算する．
  void
  or_obs(tPackedVal val);
  
  /// @brief 出力の obs の bitwise-OR を計算する．
  void
  and_obs(tPackedVal val);
  
  /// @brief 出力における観測性を得る．
  tPackedVal
  get_obs();
  
  /// @brief 出力の作業用 obs を設定する．
  void
  set_obs2(tPackedVal val);
  
  /// @brief 出力の作業用 obs の bitwise-OR を計算する．
  void
  or_obs2(tPackedVal val);
  
  /// @brief 出力における作業用 obs を得る．
  tPackedVal
  get_obs2();
  
  /// @brief 入力の obs を再帰的に計算する．
  void
  calc_iobs(tPackedVal obs,
	     bool force = false);
   
  /// @brief 入力の擬似最小 obs を計算する．
  virtual
  void
  calc_pseudo_min_iobs() = 0;
  
  /// @brief 入力の最大 obs を計算する．
  virtual
  void
  calc_max_iobs(RandGen& randgen) = 0;
  
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

  /// @brief 故障値をクリアする．
  void
  clear_ffunc();
  
  /// @brief 正常値の計算を行う．
  /// @note 結果は mGfunc と mFfunc にセットされる．
  void
  calc_gfunc();

  /// @brief 故障値の計算を行う．
  /// @param[in] mask マスク関数
  /// @return 故障差関数を返す．
  Bdd
  calc_ffunc(const Bdd& mask);

  /// @brief obs の値を得る．
  Bdd
  get_obsfunc() const;

  /// @brief obs の値をセットする．
  void
  set_obsfunc(const Bdd& obs);
  
  /// @brief 出力の obs の bitwise-OR を計算する．
  void
  or_obsfunc(const Bdd& val);

  /// @brief 入力の可観測性を再帰的に計算する．
  void
  calc_iobsfunc(const Bdd& obs,
		bool force = false);
   
  /// @brief 入力の擬似最小 obs を計算する．
  void
  calc_pseudo_min_iobsfunc();
  
  /// @brief 入力の最大 obs を計算する．
  void
  calc_max_iobsfunc();
 
  /// @brief target マークを得る．
  bool
  target() const;

  /// @brief target マークをつける．
  void
  set_target();

  /// @brief target マークを消す．
  void
  clear_target();


public:
  //////////////////////////////////////////////////////////////////////
  // 構造に関する情報の設定用関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 出力マークをつける．
  void
  set_output();

  /// @brief 外部ノードマークをつける．
  void
  set_external();


private:

  /// @brief 正常値の計算を行う．
  /// @return 出力における正常値を返す．
  virtual
  tPackedVal
  _calc_gval() = 0;

  /// @brief 故障値の計算を行う．
  /// @return 出力における故障値を返す．
  virtual
  tPackedVal
  _calc_fval() = 0;
  
  /// @brief calc_iobs の下請け関数
  virtual
  void
  _calc_iobs(tPackedVal obs) = 0;


private:

  /// @brief 正常値の計算を行う．
  /// @return 出力における正常値を返す．
  virtual
  Bdd
  _calc_gfunc() = 0;

  /// @brief 故障値の計算を行う．
  /// @return 出力における故障値を返す．
  virtual
  Bdd
  _calc_ffunc() = 0;
  
  /// @brief calc_iobs の下請け関数
  virtual
  void
  _calc_iobsfunc(const Bdd& obs) = 0;


public:

  /// @brief 内容をダンプする．
  virtual
  void
  dump(ostream& s) const = 0;


protected:
  
  /// @brief レベルを設定する．
  void
  set_level(size_t level);
  
  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ID 番号
  ymuint32 mId;
  
  // ファンアウトリストの要素数
  size_t mNfo;

  // ファンアウトリスト
  SimNode** mFanouts;
  
  // FFR
  SimFFR* mFFR;
  
  // レベル
  size_t mLevel;

  // 正常値
  tPackedVal mGval;

  // 故障値
  tPackedVal mFval;

  // 可観測性マスク
  tPackedVal mObs;

  // 上限値近似用の作業領域
  tPackedVal mTmpObs;
  
  // 正常値の関数を表す BDD
  Bdd mGfunc;

  // 故障値の関数を表す BDD
  Bdd mFfunc;

  // 可観測性マスクの関数を表す BDD
  Bdd mObsFunc;

  // obs と target フラグ
  ymuint32 mFlags;

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

// @brief FFR を得る．
inline
SimFFR*
SimNode::ffr() const
{
  return mFFR;
}

// @brief レベルを得る．
inline
size_t
SimNode::level() const
{
  return mLevel;
}

// @brief 出力ノードの時 true を返す．
inline
bool
SimNode::is_output() const
{
  return static_cast<bool>(mFlags & (1UL << 1));
}

// @brief 出力マークをつける．
inline
void
SimNode::set_output()
{
  mFlags |= (1UL << 1);
}

// @brief 外部ノードの時 true を返す．
inline
bool
SimNode::is_external() const
{
  return static_cast<bool>(mFlags & (1UL << 2));
}

// @brief 外部ノードマークをつける．
inline
void
SimNode::set_external()
{
  mFlags |= (1UL << 2);
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

// @brief 正常値のセットを行う．
// @param[in] val 値
// @note 通常は外部入力に対して行われる．
// @note 故障値も同様にセットされる．
inline
void
SimNode::set_gval(tPackedVal val)
{
  mGval = val;
  mFval = val;
  mObs = kPvAll0;
}

// @brief 正常値を得る．
inline
tPackedVal
SimNode::get_gval() const
{
  return mGval;
}

// @brief 故障値をセットする．
inline
void
SimNode::set_fval(tPackedVal pat)
{
  mFval = pat;
}

// @brief 故障値を得る．
inline
tPackedVal
SimNode::get_fval() const
{
  return mFval;
}

// @brief 故障値をクリアする．
inline
void
SimNode::clear_fval()
{
  mFval = mGval;
}

// @brief 正常値の計算を行いセットする．
inline
void
SimNode::calc_gval()
{
  set_gval(_calc_gval());
}

/// @brief 故障値の計算を行い故障差を返す．
inline
tPackedVal
SimNode::calc_fval(tPackedVal mask)
{
  tPackedVal val = _calc_fval();
  tPackedVal diff = (mGval ^ val) & mask;
  mFval ^= diff;
  return diff;
}

// @brief 出力における観測性を得る．
inline
tPackedVal
SimNode::get_obs()
{
  return mObs;
}

// @brief 出力の obs を設定する．
inline
void
SimNode::set_obs(tPackedVal val)
{
  mObs = val;
}

// @brief 出力の obs の bitwise-OR を計算する．
inline
void
SimNode::or_obs(tPackedVal val)
{
  mObs |= val;
}

// @brief 出力の obs の bitwise-AND を計算する．
inline
void
SimNode::and_obs(tPackedVal val)
{
  mObs &= val;
}

// @brief 出力における作業用 obs を得る．
inline
tPackedVal
SimNode::get_obs2()
{
  return mTmpObs;
}

// @brief 出力の作業用 obs を設定する．
inline
void
SimNode::set_obs2(tPackedVal val)
{
  mTmpObs = val;
}

// @brief 出力の作業用 obs の bitwise-OR を計算する．
inline
void
SimNode::or_obs2(tPackedVal val)
{
  mTmpObs |= val;
}

// @brief 入力の obs を再帰的に計算する．
inline
void
SimNode::calc_iobs(tPackedVal obs,
		   bool force)
{
  if ( force || !(is_output() || nfo() != 1) ) {
    set_obs(obs);
    _calc_iobs(obs);
  }
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

// @brief 故障値をクリアする．
inline
void
SimNode::clear_ffunc()
{
  mFfunc = mGfunc;
}

// @brief 正常値の計算を行いセットする．
inline
void
SimNode::calc_gfunc()
{
  set_gfunc(_calc_gfunc());
}

/// @brief 故障値の計算を行い故障差を返す．
inline
Bdd
SimNode::calc_ffunc(const Bdd& mask)
{
  Bdd f = _calc_ffunc();
  Bdd diff = (mGfunc ^ f) & mask;
  mFfunc ^= diff;
  return diff;
}

// @brief obs の値を得る．
inline
Bdd
SimNode::get_obsfunc() const
{
  return mObsFunc;
}

// @brief 出力の obs を設定する．
inline
void
SimNode::set_obsfunc(const Bdd& val)
{
  mObsFunc = val;
}
  
// @brief 出力の obs の bitwise-OR を計算する．
inline
void
SimNode::or_obsfunc(const Bdd& val)
{
  mObsFunc |= val;
}

// @brief 入力の obs を再帰的に計算する．
inline
void
SimNode::calc_iobsfunc(const Bdd& obs,
		       bool force)
{
  if ( force || !(is_output() || nfo() != 1) ) {
    set_obsfunc(obs);
    _calc_iobsfunc(obs);
  }
}

END_NAMESPACE_YM_SEAL_CVF

#endif // CALC_CVF_SIMNODE_H
