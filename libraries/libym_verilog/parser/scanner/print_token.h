#ifndef LIBYM_VERILOG_SCANNER_PRINT_TOKEN_H
#define LIBYM_VERILOG_SCANNER_PRINT_TOKEN_H

/// @file libym_verilog/scanner/print_token.h
/// @brief print_token() のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: print_token.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_verilog/verilog.h"


BEGIN_NAMESPACE_YM_VERILOG

/// @brief トークンの内容を出力する．
/// @param[in] s 出力先のストリーム
/// @param[in] id トークン番号
/// @param[in] str トークン文字列
void
print_token(ostream& s,
	    int id,
	    const char* str);

END_NAMESPACE_YM_VERILOG

#endif // LIBYM_VERILOG_SCANNER_PRINT_TOKEN_H
