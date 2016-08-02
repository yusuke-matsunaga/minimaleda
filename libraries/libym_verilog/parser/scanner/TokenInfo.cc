
/// @file libym_verilog/scanner/TokenInfo.cc
/// @brief TokenInfo の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: TokenInfo.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "TokenInfo.h"
#include "print_token.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// トークンの情報を表す構造体
//////////////////////////////////////////////////////////////////////

// @relates TokenInfo
// @brief TokenInfo の内容を出力するための関数
// @param[in] s 出力ストリーム
// @param[in] token トークン情報
// @return s を返す．
ostream&
operator<<(ostream& s,
	   const TokenInfo& token)
{
  s << token.loc() << ": ";
  int id = token.id();
  const char* str = token.str();
  print_token(s, id, str);
  return s;
}

END_NAMESPACE_YM_VERILOG
