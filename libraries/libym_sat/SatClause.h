#ifndef LIBYM_SAT_SATCLAUSE_H
#define LIBYM_SAT_SATCLAUSE_H

/// @file libym_sat/SatClause.h
/// @brief SatClause のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: SatClause.h 2203 2009-04-16 05:04:40Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_sat/SatSolver.h"


BEGIN_NAMESPACE_YM_SAT

//////////////////////////////////////////////////////////////////////
/// @class SatClause SatClause.h "SatClause.h"
/// @brief 節を表すクラス
///
/// 中身は本当に Literal の配列．しかも，クラスのインスタンスと別に配列
/// 領域を確保するのはポインタ参照が一回増えて嫌なので，見かけはサイズ1
/// の配列 mLits[1] を定義しておいて，実際には要素数分の領域を確保した
/// メモリブロックを SatClause* として扱う．
/// このために「ポインタ付き new」演算子を用いている．
/// 詳しくは SatSolverImpl::new_clause() を参照
/// SatClause はそれ以外の情報として，制約節か学習節かを区別する1ビット
/// (サイズと合わせて1ワード)のフラグ，activity を表す double 変数を
/// 持つ．
//////////////////////////////////////////////////////////////////////
class SatClause
{
  friend class SatSolverImpl;
private:

  /// @brief コンストラクタ
  /// @param[in] lits リテラルのリスト
  /// @param[in] learnt 学習節の場合 true 
  explicit
  SatClause(const vector<Literal>& lits,
	    bool learnt);

  /// @brief デストラクタ
  ~SatClause();


public:

  /// @brief リテラル数の取得
  ymuint
  size() const;

  /// @brief リテラルのアクセス
  /// @param[in] pos リテラルの位置
  Literal&
  lit(ymuint pos);

  /// @brief リテラルのアクセス
  /// @param[in] pos リテラルの位置
  Literal
  lit(ymuint pos) const;

  /// @brief 0番目のリテラルに対するアクセス
  Literal&
  lit0();

  /// @brief 1番目のリテラルに対するアクセス
  Literal&
  lit1();
  
  /// @brief 学習節の場合 true を返す．
  bool
  is_learnt() const;

  /// @brief 学習節の場合にアクティビティを返す．
  double
  activity() const;
  

private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // サイズと learnt フラグをパックしたもの
  ymuint32 mSizeLearnt;

  // activity
  double mActivity;
  
  // リテラルの配列
  // 実際にはこの後にリテラル数分の領域を確保する．
  Literal mLits[1];
  
};

/// @relates SatClause
/// @brief SatClause の内容を出力する
ostream&
operator<<(ostream& s,
	   const SatClause& c);


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief リテラル数の取得
inline
ymuint
SatClause::size() const
{
  return (mSizeLearnt >> 1);
}

// @brief リテラルのアクセス
inline
Literal&
SatClause::lit(ymuint pos)
{
  return mLits[pos];
}

// @brief リテラルのアクセス
inline
Literal
SatClause::lit(ymuint pos) const
{
  return mLits[pos];
}

// @brief 0番目のリテラルに対するアクセス
inline
Literal&
SatClause::lit0()
{
  return mLits[0];
}

// @brief 1番目のリテラルに対するアクセス
inline
Literal&
SatClause::lit1()
{
  return mLits[1];
}

// @brief 学習節の場合 true を返す．
inline
bool
SatClause::is_learnt() const
{
  return static_cast<bool>(mSizeLearnt & 1UL);
}

// @brief 学習節の場合にアクティビティを返す．
inline
double
SatClause::activity() const
{
  return mActivity;
}

END_NAMESPACE_YM_SAT

#endif // LIBYM_SAT_SATCLAUSE_H
