#ifndef PACKEDVAL3_H
#define PACKEDVAL3_H

/// @file PackedVal3.h
/// @brief 3値のビットベクタ型の定義ファイル
/// @author Yusuke Matsunaga (松永 裕介)
/// 
/// $Id: PackedVal3.h 2203 2009-04-16 05:04:40Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.

#include "atpg_nsdef.h"
#include "PackedVal.h"


BEGIN_NAMESPACE_YM_ATPG

//////////////////////////////////////////////////////////////////////
/// @class PackedaVal3 PackedVal.h "PackedVal3.h"
/// @brief PackedVal を2つ使って 3値を表すクラス
//////////////////////////////////////////////////////////////////////
class PackedVal3
{
public:

  /// @brief 空のコンストラクタ
  /// @note 内容は不定
  PackedVal3();

  /// @brief コンストラクタ
  /// @param[in] pat0 0 を表すビットベクタ
  /// @param[in] pat1 1 を表すビットベクタ
  PackedVal3(PackedVal pat0,
	     PackedVal pat1);
  
  /// @brief PackedVal からのキャスト演算子
  explicit
  PackedVal3(PackedVal pat);
  
  /// @brief デストラクタ
  ~PackedVal3();


public:

  /// @brief 値をセットする．
  /// @param[in] pat0 0 を表すビットベクタ
  /// @param[in] pat1 1 を表すビットベクタ
  void
  set(PackedVal pat0,
      PackedVal pat1);

  /// @brief 2値の値からセットする．
  void
  set(PackedVal pat);
  
  /// @brief マスク付きでセットする．
  void
  set_with_mask(const PackedVal3& src,
		PackedVal mask);
  
  /// @brief 0 を表すビットベクタを取り出す．
  PackedVal
  _pat0() const;

  /// @brief 1 を表すビットベクタを取り出す．
  PackedVal
  _pat1() const;
  
  /// @brief 0 の部分を取り出す．
  /// @note 要するに 0 -> 1, 1 -> 0, X -> 0 の変換を行う．
  PackedVal
  extract_0() const;

  /// @brief 1 の部分を取り出す．
  /// @note 要するに 0 -> 0, 1 -> 1, X -> 0 の変換を行う．
  PackedVal
  extract_1() const;

  /// @brief X の部分を取り出す．
  /// @note 要するに 0 -> 0, 1 -> 0, X -> 1 の変換を行う．
  PackedVal
  extract_X() const;

  /// @brief 01 の部分を取り出す．
  /// @note 要するに 0 -> 1, 1 -> 1, X -> 0 の変換を行う．
  PackedVal
  extract_01() const;
  
  
public:

  /// @brief 否定代入演算子
  /// @return 自分自身を返す．
  const PackedVal3&
  negate();

  /// @brief opr と値の異なっているビットを求める．
  PackedVal
  diff(const PackedVal3& opr);
  
  /// @brief AND代入演算子
  /// @param[in] opr オペランド
  /// @return 自分自身を返す．
  const PackedVal3&
  operator&=(const PackedVal3& opr);
  
  /// @brief OR代入演算子
  /// @param[in] opr オペランド
  /// @return 自分自身を返す．
  const PackedVal3&
  operator|=(const PackedVal3& opr);
  
  /// @brief XOR代入演算子
  /// @param[in] opr オペランド
  /// @return 自分自身を返す．
  const PackedVal3&
  operator^=(const PackedVal3& opr);

  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 0 を表すビットベクタ
  PackedVal mPat0;

  // 1 を表すビットベクタ
  PackedVal mPat1;
  
};

/// @relates PackedVal3
/// @brief 否定演算子
PackedVal3
operator~(const PackedVal3& opr);

/// @relates PackedVal3
/// @brief AND 演算子
/// @param[in] opr1, opr2 オペランド
PackedVal3
operator&(const PackedVal3& opr1,
	  const PackedVal3& opr2);

/// @relates PackedVal3
/// @brief OR 演算子
/// @param[in] opr1, opr2 オペランド
PackedVal3
operator|(const PackedVal3& opr1,
	  const PackedVal3& opr2);

/// @relates PackedVal3
/// @brief XOR 演算子
/// @param[in] opr1, opr2 オペランド
PackedVal3
operator^(const PackedVal3& opr1,
	  const PackedVal3& opr2);

/// @relates PackedVal3
/// @brief ITE 演算
/// @param[in] opr1, opr2, opr3 オペランド
/// @note if ( opr1 ) then opr2 else opr3 の計算を行う．
PackedVal3
ite(PackedVal opr1,
    const PackedVal3& opr2,
    const PackedVal3& opr3);


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 空のコンストラクタ
// @note 内容は不定
inline
PackedVal3::PackedVal3()
{
}

// @brief コンストラクタ
// @param[in] pat0 0 を表すビットベクタ
// @param[in] pat1 1 を表すビットベクタ
inline
PackedVal3::PackedVal3(PackedVal pat0,
		       PackedVal pat1) :
  mPat0(pat0),
  mPat1(pat1)
{
}
  
// @brief PackedVal からのキャスト演算子
inline
PackedVal3::PackedVal3(PackedVal pat) :
  mPat0(~pat),
  mPat1(pat)
{
}
  
// @brief デストラクタ
inline
PackedVal3::~PackedVal3()
{
}

// @brief 値をセットする．
// @param[in] pat0 0 を表すビットベクタ
// @param[in] pat1 1 を表すビットベクタ
inline
void
PackedVal3::set(PackedVal pat0,
		PackedVal pat1)
{
  mPat0 = pat0;
  mPat1 = pat1;
}

// @brief 2値の値からセットする．
inline
void
PackedVal3::set(PackedVal pat)
{
  mPat0 = ~pat;
  mPat1 = pat;
}
  
// @brief マスク付きでセットする．
inline
void
PackedVal3::set_with_mask(const PackedVal3& src,
			  PackedVal mask)
{
  mPat0 &= ~mask;
  mPat0 |= src.mPat0 & mask;
  mPat1 &= ~mask;
  mPat1 |= src.mPat1 & mask;
}
  
// @brief 0 を表すビットベクタを取り出す．
inline
PackedVal
PackedVal3::_pat0() const
{
  return mPat0;
}

// @brief 1 を表すビットベクタを取り出す．
inline
PackedVal
PackedVal3::_pat1() const
{
  return mPat1;
}
 
// @brief 0 の部分を取り出す．
// @note 要するに 0 -> 1, 1 -> 0, X -> 0 の変換を行う．
inline
PackedVal
PackedVal3::extract_0() const
{
  return mPat0 & ~mPat1;
}

// @brief 1 の部分を取り出す．
// @note 要するに 0 -> 0, 1 -> 1, X -> 0 の変換を行う．
inline
PackedVal
PackedVal3::extract_1() const
{
  return ~mPat0 & mPat1;
}

// @brief X の部分を取り出す．
// @note 要するに 0 -> 0, 1 -> 0, X -> 1 の変換を行う．
inline
PackedVal
PackedVal3::extract_X() const
{
  return ~mPat0 & ~mPat1;
}

// @brief 01 の部分を取り出す．
// @note 要するに 0 -> 1, 1 -> 1, X -> 0 の変換を行う．
inline
PackedVal
PackedVal3::extract_01() const
{
  return mPat0 | mPat1;
}

// @brief 否定代入演算子
// @return 自分自身を返す．
inline
const PackedVal3&
PackedVal3::negate()
{
  PackedVal tmp = mPat0;
  mPat0 = mPat1;
  mPat1 = tmp;
  return *this;
}

// @brief opr と値の異なっているビットを求める．
inline
PackedVal
PackedVal3::diff(const PackedVal3& opr)
{
  return (mPat0 ^ opr.mPat0) | (mPat1 ^ opr.mPat1);
}
  
// @brief AND代入演算子
// @param[in] opr オペランド
// @return 自分自身を返す．
inline
const PackedVal3&
PackedVal3::operator&=(const PackedVal3& opr)
{
  mPat0 |= opr.mPat0;
  mPat1 &= opr.mPat1;
  return *this;
}
  
// @brief OR代入演算子
// @param[in] opr オペランド
// @return 自分自身を返す．
inline
const PackedVal3&
PackedVal3::operator|=(const PackedVal3& opr)
{
  mPat0 &= opr.mPat0;
  mPat1 |= opr.mPat1;
  return *this;
}
  
// @brief XOR代入演算子
// @param[in] opr オペランド
// @return 自分自身を返す．
inline
const PackedVal3&
PackedVal3::operator^=(const PackedVal3& opr)
{
  // this & ~opr の計算
  PackedVal tmp0_0 = mPat0 | opr.mPat1;
  PackedVal tmp0_1 = mPat1 & opr.mPat0;
  // ~this & opr の計算
  PackedVal tmp1_0 = mPat1 | opr.mPat0;
  PackedVal tmp1_1 = mPat0 & opr.mPat1;
  mPat0 = tmp0_0 & tmp1_0;
  mPat1 = tmp0_1 | tmp1_1;
  return *this;
} 

// @relates PackedVal3
// @brief 否定演算子
inline
PackedVal3
operator~(const PackedVal3& opr)
{
  return PackedVal3(opr._pat1(), opr._pat0());
}

// @relates PackedVal3
// @brief AND 演算子
// @param[in] opr1, opr2 オペランド
inline
PackedVal3
operator&(const PackedVal3& opr1,
	  const PackedVal3& opr2)
{
  PackedVal pat0 = opr1._pat0() | opr2._pat0();
  PackedVal _pat1 = opr1._pat1() & opr2._pat1();
  return PackedVal3(pat0, _pat1);
}

// @relates PackedVal3
// @brief OR 演算子
// @param[in] opr1, opr2 オペランド
inline
PackedVal3
operator|(const PackedVal3& opr1,
	  const PackedVal3& opr2)
{
  PackedVal pat0 = opr1._pat0() & opr2._pat0();
  PackedVal _pat1 = opr1._pat1() | opr2._pat1();
  return PackedVal3(pat0, _pat1);
}

// @relates PackedVal3
// @brief XOR 演算子
// @param[in] opr1, opr2 オペランド
inline
PackedVal3
operator^(const PackedVal3& opr1,
	  const PackedVal3& opr2)
{
  // opr1 & ~opr2 の計算
  PackedVal tmp0_0 = opr1._pat0() | opr2._pat1();
  PackedVal tmp0_1 = opr1._pat1() & opr2._pat0();
  // ~opr1 & opr2 の計算
  PackedVal tmp1_0 = opr1._pat1() | opr2._pat0();
  PackedVal tmp1_1 = opr1._pat0() & opr2._pat1();
  PackedVal pat0 = tmp0_0 & tmp1_0;
  PackedVal _pat1 = tmp0_1 | tmp1_1;
  return PackedVal3(pat0, _pat1);
}

// @relates PackedVal3
// @brief ITE 演算
// @param[in] opr1, opr2, opr3 オペランド
// @note if ( opr1 ) then opr2 else opr3 の計算を行う．
inline
PackedVal3
ite(PackedVal opr1,
    const PackedVal3& opr2,
    const PackedVal3& opr3)
{
  PackedVal val0 = (opr1 & opr2._pat0()) | (~opr1 & opr3._pat0());
  PackedVal val1 = (opr1 & opr2._pat1()) | (~opr1 & opr3._pat1());
  return PackedVal3(val0, val1);
}

END_NAMESPACE_YM_ATPG

#endif // PACKEDVAL3_H
