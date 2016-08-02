#ifndef FSIM3_SIMNODE_H
#define FSIM3_SIMNODE_H

/// @file fsim/SimNode.h
/// @brief SimNode のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
/// 
/// $Id: SimNode.h 2203 2009-04-16 05:04:40Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "fsim3_nsdef.h"
#include "EqElem.h"
#include "Val3.h"
#include "ym_tgnet/TgGateTemplate.h"
#include "ym_lexp/LogExpr.h"


BEGIN_NAMESPACE_YM_ATPG_FSIM3

class SimFFR;

//////////////////////////////////////////////////////////////////////
/// @class SimNode SimNode.h "SimNode.h"
/// @brief 故障シミュレーション用のノード
//////////////////////////////////////////////////////////////////////
class SimNode :
  public EqElem
{
protected:

  /// @brief コンストラクタ
  SimNode(ymuint32 id);


public:

  /// @brief デストラクタ
  virtual
  ~SimNode();


public:

  /// @brief ノードを生成するクラスメソッド
  static
  SimNode*
  new_node(ymuint32 id,
	   tTgGateType type,
	   const LogExpr& lexp,
	   const vector<SimNode*>& inputs);


public:
  //////////////////////////////////////////////////////////////////////
  // 構造に関する情報の取得
  //////////////////////////////////////////////////////////////////////

  /// @brief ID番号を返す．
  ymuint32
  id() const;

  /// @brief ファンイン数を得る．
  virtual
  ymuint
  nfi() const = 0;

  /// @brief pos 番めのファンインを得る．
  virtual
  SimNode*
  fanin(ymuint pos) const = 0;

  /// @brief ファンアウト数を得る．
  ymuint
  nfo() const;

  /// @brief pos 番目のファンアウトを得る．
  SimNode*
  fanout(ymuint pos) const;

  /// @brief 最初のファンアウト先の入力位置を得る．
  ymuint
  fanout_ipos() const;
  
  /// @brief FFR を得る．
  SimFFR*
  ffr() const;

  /// @brief FFR の根のノードの時 true を返す．
  bool
  is_ffr_root() const;

  /// @brief レベルを得る．
  ymuint
  level() const;

  /// @brief 出力ノードの時 true を返す．
  bool
  is_output() const;


public:
  //////////////////////////////////////////////////////////////////////
  // 故障シミュレーションに関する情報の取得/設定
  //////////////////////////////////////////////////////////////////////

  /// @brief 正常値のセットを行う．(3値版)
  /// @param[in] val 値
  /// @note 通常は外部入力に対して行われる．
  /// @note 故障値も同様にセットされる．
  void
  set_gval(Val3 val);

  /// @breif 正常値を得る．(3値版)
  Val3
  gval() const;

  /// @brief 故障値をセットする．(3値版)
  /// @param[in] val 値
  void
  set_fval(Val3 val);

  /// @brief 故障値を得る．(3値版)
  Val3
  fval() const;

  /// @brief 故障値をクリアする．
  void
  clear_fval();

  /// @brief 正常値の計算を行う．(3値版)
  /// @return 結果が X でなければ true を返す．
  /// @note 結果は mGval にセットされる．
  bool
  calc_gval3();

  /// @brief 故障値の計算を行う．(3値版)
  /// @return 故障差を返す．
  /// @note 結果は mFval にセットされる．
  bool
  calc_fval3();
  
  /// @brief ローカルな obs の計算を行う．
  bool
  calc_lobs();
  
  /// @brief lobs が計算済みかチェックする．
  bool
  check_lobs() const;

  /// @brief lobs が計算済みの印をつける．
  void
  set_lobs();

  /// @brief lobs の計算済みの印を消す．
  void
  clear_lobs();

  
public:
  //////////////////////////////////////////////////////////////////////
  // 構造に関する情報の設定用関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 出力マークをつける．
  void
  set_output();

  /// @brief ファンアウトリストを作成する．
  void
  set_fanout_list(const vector<SimNode*>& fo_list,
		  ymuint ipos);
  
  /// @brief FFR を設定する．
  void
  set_ffr(SimFFR* ffr);

  
public:
  //////////////////////////////////////////////////////////////////////
  // 派生クラスで実装する仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 正常値の計算を行う．(3値版)
  virtual
  Val3
  _calc_gval3() = 0;

  /// @brief 故障値の計算を行う．(3値版)
  virtual
  Val3
  _calc_fval3() = 0;

  /// @brief ゲートの入力から出力までの可観測性を計算する．(3値版)
  virtual
  bool
  calc_gobs3(ymuint ipos) = 0;

  /// @brief 内容をダンプする．
  virtual
  void
  dump(ostream& s) const = 0;


protected:
  
  /// @brief レベルを設定する．
  void
  set_level(ymuint level);
  
  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ID 番号
  ymuint32 mId;
  
  // ファンアウトリストの要素数
  ymuint32 mNfo;
  
  // ファンアウトリスト
  SimNode** mFanouts;

  // 最初のファンアウトの入力位置(FFR内のノードのみ意味を持つ)
  ymuint32 mFanoutIpos;
  
  // FFR
  SimFFR* mFFR;
  
  // レベル
  ymuint32 mLevel;

  // 正常値
  Val3 mGval;

  // 故障値
  Val3 mFval;
  
  // FFR 内のローカルな obs
  bool mLobs;

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
ymuint
SimNode::nfo() const
{
  return mNfo;
}

// @brief pos 番目のファンアウトを得る．
inline
SimNode*
SimNode::fanout(ymuint pos) const
{
  return mFanouts[pos];
}

// @brief 最初のファンアウト先の入力位置を得る．
inline
ymuint
SimNode::fanout_ipos() const
{
  return mFanoutIpos >> 2;
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
ymuint
SimNode::level() const
{
  return mLevel;
}

// @brief 出力ノードの時 true を返す．
inline
bool
SimNode::is_output() const
{
  return (mFanoutIpos & 1U) == 1U;
}

// @brief 出力マークをつける．
inline
void
SimNode::set_output()
{
  mFanoutIpos |= 1U;
}
  
// @brief lobs が計算済みかチェックする．
inline
bool
SimNode::check_lobs() const
{
  return (mFanoutIpos >> 1) & 1U == 1U;
}

// @brief lobs が計算済みの印をつける．
inline
void
SimNode::set_lobs()
{
  mFanoutIpos |= (1U << 1);
}

// @brief lobs の計算済みの印を消す．
inline
void
SimNode::clear_lobs()
{
  mFanoutIpos &= ~(1U << 1);
}

// @brief 正常値のセットを行う．(3値版)
// @param[in] val0, val1 値
// @note 通常は外部入力に対して行われる．
// @note 故障値も同様にセットされる．
inline
void
SimNode::set_gval(Val3 val)
{
  mGval = val;
  mFval = val;
}

// @brief 正常値を得る．(3値版)
inline
Val3
SimNode::gval() const
{
  return mGval;
}

// @brief 故障値をセットする．(3値版)
inline
void
SimNode::set_fval(Val3 val)
{
  mFval = val;
}

// @brief 故障値を得る．(3値版)
inline
Val3
SimNode::fval() const
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

// @brief 正常値の計算を行う．(3値版)
// @return 結果が X でなければ true を返す．
// @note 結果は mGval にセットされる．
inline
bool
SimNode::calc_gval3()
{
  mGval = _calc_gval3();
  mFval = mGval;
  return mGval != kValX;
}

// @brief 故障値の計算を行う．(3値版)
// @return 故障差を返す．
// @note 結果は mFval にセットされる．
inline
bool
SimNode::calc_fval3()
{
  mFval = _calc_fval3();
  return mFval != mGval;
}
 
// @brief FFR を設定する．
inline
void
SimNode::set_ffr(SimFFR* ffr)
{
  mFFR = ffr;
}

END_NAMESPACE_YM_ATPG_FSIM3

#endif // FSIM3_SIMNODE_H
