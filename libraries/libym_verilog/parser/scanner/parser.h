#ifndef LIBYM_VERILOG_SCANNER_PARSER_H
#define LIBYM_VERILOG_SCANNER_PARSER_H

/// @file libym_verilog/scanner/parser.h
/// @brief YACC の生成したヘッダファイルをインクルードする為のファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: parser.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_verilog/pt/PtP.h"
#include "PtiFwd.h"
#include "PtrList.h"


BEGIN_NAMESPACE_YM_VERILOG

class PuHierName;

#include "verilog_grammer.h"

END_NAMESPACE_YM_VERILOG

#endif // LIBYM_VERILOG_SCANNER_PARSER_H
