
/// @file libym_lexp/Literal.cc
/// @brief Literal の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: Literal.cc 750 2007-06-30 09:09:58Z matsunaga $
///
/// Copyright (C) 2005-2007 Yusuke Matsunaga
/// All rights reserved.


#include "ym_lexp/Literal.h"


BEGIN_NAMESPACE_YM_LEXP

// 実はデフォルトコンストラクタが未定義リテラルを作っている．
const
Literal kLiteralX;

// ostream に対する書出し
ostream&
operator<<(ostream& s,
	   const Literal& lit)
{
  if ( lit == kLiteralX ) {
    s << "-X-";
  }
  else {
    s << "v_" << lit.varid();
    if ( lit.pol() == kPolNega ) {
      s << "'";
    }
  }
  return s;
}

END_NAMESPACE_YM_LEXP
