#ifndef LIBYM_SAT_SATREASON_H
#define LIBYM_SAT_SATREASON_H

/// @file libym_sat/SatReason.h
/// @brief SatReason のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: SatReason.h 2203 2009-04-16 05:04:40Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_sat/sat_nsdef.h"
#include "ym_lexp/Literal.h"
#include "ym_sat/Bool3.h"


BEGIN_NAMESPACE_YM_SAT

class SatClause;

//////////////////////////////////////////////////////////////////////
/// @class SatReason SatReason.h "SatReason.h"
/// @brief 含意の原因を表すクラス
///
/// (a1 + a2 + a3 + ... an + b) という節があるとする．
/// 今，a1 = a2 = a3 = ... an = 0 になるとこの節を充足させるためには
/// b = 1 が必要となる．そこで，この節(および a1 ... an の割り当て)が
/// b = 1 という割り当ての原因となる．
/// SatSolverImpl::implication() の中でこのような含意が起きたときには
/// 結果のリテラル(上の例では b)を最初のリテラルに移動しているので，
/// 通常であれば SatClause そのもので原因を表現しても構わない．
/// ただし，もともとの節が (a + b) の形なら節の代わりに ~a というリテラ
/// ルを用いて原因を表すこともできる．そこで MiniSat では GClause
/// という節とリテラルの両方を一般化したクラスを用いている．
/// ここではそれに倣い，SatClause へのポインタと Literal を排他的に
/// 表現するクラスを作った．
//////////////////////////////////////////////////////////////////////
class SatReason
{
public:

  /// @brief コンストラクタ
  /// @param[in] clause 節へのポインタ
  explicit
  SatReason(SatClause* clause = NULL);

  /// @brief コンストラクタ
  /// @param[in] lit リテラル
  explicit
  SatReason(Literal lit);


public:

  /// @brief 内容が節の時 true を返す．
  bool
  is_clause() const;

  /// @brief 節を取り出す．
  SatClause&
  clause() const;

  /// @brief 内容がリテラルの時 true を返す．
  bool
  is_literal() const;

  /// @brief リテラルを取り出す．
  Literal
  literal() const;


  //////////////////////////////////////////////////////////////////////
  // friend 関数の定義
  //////////////////////////////////////////////////////////////////////

  friend
  bool
  operator==(const SatReason& left,
	     const SatReason& right);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // SatClause か Literal を保持する
  ympuint mBody;
  
};


/// @relates SatReason
/// @brief 等価比較演算子
bool
operator==(const SatReason& left,
	   const SatReason& right);

/// @relates SatReason
/// @brief 非等価比較演算子
bool
operator!=(const SatReason& left,
	   const SatReason& right);
  
/// @relates SatReason
/// @brief SatReason の内容を出力する
ostream&
operator<<(ostream& s,
	   const SatReason& c);

/// @relates SatReason
/// @brief 空の SatReason を表す定数
const SatReason kNullSatReason = SatReason();


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] clause 節へのポインタ
inline
SatReason::SatReason(SatClause* clause)
{
  mBody = reinterpret_cast<ympuint>(clause);
}

// @brief コンストラクタ
// @param[in] lit リテラル
inline
SatReason::SatReason(Literal lit)
{
  mBody = (lit.index() << 1) | 1UL;
}

// @brief 内容が節の時 true を返す．
inline
bool
SatReason::is_clause() const
{
  return !is_literal();
}

// @brief 節を取り出す．
inline
SatClause&
SatReason::clause() const
{
  return *reinterpret_cast<SatClause*>(mBody);
}

// @brief 内容がリテラルの時 true を返す．
inline
bool
SatReason::is_literal() const
{
  return static_cast<bool>(mBody & 1UL);
}

// @brief リテラルを取り出す．
inline
Literal
SatReason::literal() const
{
  return Literal::index2literal(mBody >> 1);
}

// @brief 等価比較演算子
inline
bool
operator==(const SatReason& left,
	   const SatReason& right)
{
  return left.mBody == right.mBody;
}

// @brief 非等価比較演算子
inline
bool
operator!=(const SatReason& left,
	   const SatReason& right)
{
  return !operator==(left, right);
}

END_NAMESPACE_YM_SAT

#endif // LIBYM_SAT_SATREASON_H
