#ifndef LIBYM_SAT_SABASE_H
#define LIBYM_SAT_SABASE_H

/// @file libym_sat/SaBase.h
/// @brief SaBase のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: SaBase.h 2203 2009-04-16 05:04:40Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "SatAnalyzer.h"


BEGIN_NAMESPACE_YM_SAT

//////////////////////////////////////////////////////////////////////
/// @class SaBase SaBase.h "SaBase.h"
/// @brief SatAnalyzer のベース実装クラス
//////////////////////////////////////////////////////////////////////
class SaBase :
  public SatAnalyzer
{
public:

  /// @brief コンストラクタ
  SaBase();
  
  /// @brief デストラクタ
  virtual
  ~SaBase();


public:

  /// @brief 新しい変数が追加されたときに呼ばれる仮想関数
  virtual
  void
  alloc_var(ymuint size);


protected:

  // learnt を極小セパレータにする．
  void
  simplify(vector<Literal>& learnt);
  
  // decision level の高いリテラルを 2番め (learnt[1] の位置) に持ってくる
  // 2番めのリテラルのレベルを返す．
  int
  reorder(vector<Literal>& learnt);
  
  // mClearQueue につまれた変数のマークを消す．
  void
  clear_marks();

  // 変数のマークにアクセスする．
  bool
  get_mark(tVarId var);

  // var->mMark を設定してキューに積む
  void
  set_mark_and_putq(tVarId var);

  // 変数のマークをセットする．
  void
  set_mark(tVarId var,
	   bool mark);


private:
  
  // simplify のサブルーティン
  bool
  check_recur(tVarId var,
	      ymuint64 lmask);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // var->mMark を消すためのキュー
  vector<tVarId> mClearQueue;
  
  // var->mMark を消すためのキュー(simplify_recur用)
  vector<tVarId> mClearQueue2;
  
  // 変数のマーク
  vector<bool> mMark;
  
};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// 変数のマークにアクセスする．
inline
bool
SaBase::get_mark(tVarId var)
{
  return mMark[var];
}

// 変数のマークをセットする．
inline
void
SaBase::set_mark(tVarId var,
		 bool mark)
{
  mMark[var] = mark;
}

END_NAMESPACE_YM_SAT

#endif // LIBYM_SAT_SABASE_H
