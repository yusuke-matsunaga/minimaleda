#ifndef LIBYM_LEXP_SOPLIT_H
#define LIBYM_LEXP_SOPLIT_H

/// @file libym_lexp/SopLit.h
/// @brief SopLit のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: SopLit.h 693 2007-05-28 09:13:57Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_lexp/LogExpr.h"


BEGIN_NAMESPACE_YM_LEXP

//////////////////////////////////////////////////////////////////////
/// リテラル数見積り用のクラス
/// 具体的には積項数とリテラル数のペア
//////////////////////////////////////////////////////////////////////
class SopLit
{
public:
  /// コンストラクタ
  SopLit(size_t np = 0,
	 size_t nl = 0);
  
  /// 積項数を得る．
  size_t
  np() const;
  
  /// リテラル数を得る．
  size_t
  nl() const;
  
  /// 和を取って代入する．
  const SopLit&
  operator+=(const SopLit& src);
  
  /// 積を取って代入する．
  const SopLit&
  operator*=(const SopLit& src);

  /// 2つの積項の和
  friend
  SopLit
  operator+(const SopLit& src1,
	    const SopLit& src2);

  /// 2つの積項の積
  friend
  SopLit
  operator*(const SopLit& src1,
	    const SopLit& src2);
  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 積項数
  size_t mNp;

  // リテラル数
  size_t mNl;
};



//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

inline
SopLit::SopLit(size_t np,
	       size_t nl) :
  mNp(np),
  mNl(nl)
{
}

inline
size_t
SopLit::np() const
{
  return mNp;
}

inline
size_t
SopLit::nl() const
{
  return mNl;
}

inline
const SopLit&
SopLit::operator+=(const SopLit& src)
{
  mNp += src.mNp;
  mNl += src.mNl;
  return *this;
}

inline
const SopLit&
SopLit::operator*=(const SopLit& src)
{
  size_t old_np = mNp;
  mNp *= src.mNp;
  mNl = mNl * src.mNp + src.mNl * old_np;
  return *this;
}

inline
SopLit
operator+(const SopLit& src1,
	  const SopLit& src2)
{
  return SopLit(src1.mNp + src2.mNp, src1.mNl + src2.mNl);
}

inline
SopLit
operator*(const SopLit& src1,
	  const SopLit& src2)
{
  return SopLit(src1.mNp * src2.mNp,
		src1.mNl * src2.mNp + src2.mNl * src1.mNp);
}

END_NAMESPACE_YM_LEXP

#endif // LIBYM_LEXP_SOPLIT_H
