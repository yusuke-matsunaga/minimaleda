#ifndef YM_LEXP_LITERAL_H
#define YM_LEXP_LITERAL_H

/// @file ym_lexp/Literal.h
/// @brief Literal のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: Literal.h 2203 2009-04-16 05:04:40Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_lexp/lexp_nsdef.h"


BEGIN_NAMESPACE_YM_LEXP

//////////////////////////////////////////////////////////////////////
/// @ingroup LexpGroup
/// @class Literal Literal.h <ym_lexp/Literal.h>
/// @brief リテラル(変数番号＋極性)を表すクラス
/// @sa tVarId, tPol
//////////////////////////////////////////////////////////////////////
class Literal
{
public:

  /// @brief デフォルトコンストラクタ
  /// 内容は不定なのであまり使わない方が良い．
  Literal();
  
  /// @brief 変数番号と極性を指定したコンストラクタ
  /// @param[in] varid 変数番号
  /// @param[in] pol 極性
  Literal(tVarId varid,
	  tPol pol);

  /// @brief index からの変換関数
  static
  Literal
  index2literal(ymuint index);
  
  // コピーコンストラクタ,代入演算子,デストラクタはデフォルト
  // のものがそのまま使える．
  
  /// @brief 変数番号を得る．
  /// @return 変数番号
  tVarId
  varid() const;
  
  /// @brief 極性を得る．
  /// @return 極性
  tPol
  pol() const;
  
  /// @brief 内容を設定する．
  /// @param[in] varid 変数番号
  /// @param[in] pol 極性
  void
  set(tVarId varid,
      tPol pol);
  
  /// @brief 極性の反転
  /// @return 極性を反転させたリテラルを返す．
  Literal
  operator~() const;

  /// @brief 同じ変数の正極性リテラルを返す．
  Literal
  make_positive() const;

  /// @brief 同じ変数の負極性リテラルを返す．
  Literal
  make_negative() const;
  
  // 等価比較
  friend
  bool
  operator==(Literal lit1,
	     Literal lit2);

  // 大小比較
  friend
  bool
  operator<(Literal lit1,
	    Literal lit2);

  /// @brief ハッシュ用の関数
  ymuint
  hash() const;

  /// @brief 配列のインデックスとして使用可能な数を返す．
  ymuint
  index() const;


private:

  // 内部でのみ用いるコンストラクタ
  explicit
  Literal(ymuint32 body);

  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////
  
  // 変数番号と極性をパックしたもの
  ymuint32 mBody;

};

/// @relates Literal
/// @brief 未定義リテラル
extern
const
Literal kLiteralX;

/// @relates Literal
/// @brief 等価比較
/// @param[in] lit1, lit2 比較するリテラル
/// @return lit1 と lit2 が等しいリテラルの時 true を返す．
bool
operator==(Literal lit1,
	   Literal lit2);

/// @relates Literal
/// @brief 非等価比較
/// @param[in] lit1, lit2 比較するリテラル
/// @return lit1 と lit2 が等しいリテラルでないとき true を返す．
bool
operator!=(Literal lit1,
	   Literal lit2);

/// @relates Literal
/// @brief 小なり比較
/// @param[in] lit1, lit2 比較するリテラル
/// @return lit1 が lit2 より小さいとき true を返す．
bool
operator<(Literal lit1,
	  Literal lit2);

/// @relates Literal
/// @brief 大なり比較
/// @param[in] lit1, lit2 比較するリテラル
/// @return lit1 が lit2 より大きいとき true を返す．
bool
operator>(Literal lit1,
	  Literal lit2);

/// @relates Literal
/// @brief 小なりイコール比較
/// @param[in] lit1, lit2 比較するリテラル
/// @return lit1 が lit2 より小さいか等しいとき true を返す．
bool
operator<=(Literal lit1,
	   Literal lit2);

/// @relates Literal
/// @brief 大なりイコール比較
/// @param[in] lit1, lit2 比較するリテラル
/// @return lit1 が lit2 より大きいか等しいとき true を返す．
bool
operator>=(Literal lit1,
	   Literal lit2);

/// @relates Literal
/// @brief Literal の内容を ostream に出力する関数
/// @param[in] s 出力ストリーム
/// @param[in] lit 出力対象のリテラル
/// @return s
ostream&
operator<<(ostream& s,
	   const Literal& lit);


//////////////////////////////////////////////////////////////////////
// 上記のクラスを要素とするコンテナクラス
//////////////////////////////////////////////////////////////////////

/// @brief リテラルのベクタ
typedef vector<Literal> LiteralVector;

/// @brief リテラルのリスト
typedef list<Literal> LiteralList;


//////////////////////////////////////////////////////////////////////
// inline 関数の定義
//////////////////////////////////////////////////////////////////////

// 内容を設定する．
inline
void
Literal::set(tVarId varid,
	     tPol pol)
{
  mBody = (varid << 1) + static_cast<ymuint32>(pol);
}
  
// デフォルトコンストラクタ
inline
Literal::Literal() :
  mBody(0xfffffffe)
{
}

// 変数番号と極性を指定したコンストラクタ
inline
Literal::Literal(tVarId varid,
		 tPol pol)
{
  set(varid, pol);
}

// 内部でのみ用いるコンストラクタ
inline
Literal::Literal(ymuint body) :
  mBody(body)
{
}

// @brief index からの変換関数
inline
Literal
Literal::index2literal(ymuint index)
{
  return Literal(index);
}

// 変数番号を得る．
inline
tVarId
Literal::varid() const
{
  return static_cast<tVarId>(mBody >> 1);
}

// 極性を得る．
inline
tPol
Literal::pol() const
{
  return static_cast<tPol>(mBody & 1);
}

// 極性を反転させたリテラルを返す．
inline
Literal
Literal::operator~() const
{
  return Literal(mBody ^ 1U);
}

// @brief 同じ変数の正極性リテラルを返す．
inline
Literal
Literal::make_positive() const
{
  return Literal(mBody & ~1U);
}

// @brief 同じ変数の負極性リテラルを返す．
inline
Literal
Literal::make_negative() const
{
  return Literal(mBody | 1U);
}

// 等価比較
inline
bool
operator==(Literal lit1,
	   Literal lit2)
{
  return lit1.mBody == lit2.mBody;
}
inline
bool
operator!=(Literal lit1,
	   Literal lit2)
{
  return !operator==(lit1, lit2);
}

// @brief 小なり比較
inline
bool
operator<(Literal lit1,
	  Literal lit2)
{
  return lit1.mBody < lit2.mBody;
}

// @brief 大なり比較
inline
bool
operator>(Literal lit1,
	  Literal lit2)
{
  return operator<(lit2, lit1);
}

// @brief 小なりイコール比較
inline
bool
operator<=(Literal lit1,
	   Literal lit2)
{
  return !operator<(lit2, lit1);
}

// @brief 大なりイコール比較
inline
bool
operator>=(Literal lit1,
	   Literal lit2)
{
  return !operator<(lit1, lit2);
}

// ハッシュ用の関数
inline
ymuint
Literal::hash() const
{
  return mBody;
}

// @brief 配列のインデックスとして使用可能な数を返す．
inline
ymuint
Literal::index() const
{
  return mBody;
}

END_NAMESPACE_YM_LEXP


BEGIN_NAMESPACE_STD

template <>
struct equal_to<nsYm::nsLexp::Literal>
{
  bool
  operator()(nsYm::nsLexp::Literal n1,
	     nsYm::nsLexp::Literal n2) const
  {
    return n1 == n2;
  }
};

END_NAMESPACE_STD


BEGIN_NAMESPACE_HASH

// Literal をキーにしたハッシュ関数クラス
template <>
struct hash<nsYm::nsLexp::Literal>
{
  ymuint
  operator()(nsYm::nsLexp::Literal lit) const
  {
    return lit.hash();
  }
};

END_NAMESPACE_HASH


BEGIN_NAMESPACE_YM

using nsLexp::Literal;
using nsLexp::LiteralVector;
using nsLexp::LiteralList;
using nsLexp::kLiteralX;

END_NAMESPACE_YM

#endif // YM_LEXP_LITERAL_H
