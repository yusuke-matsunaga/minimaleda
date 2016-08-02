#ifndef LIBYM_VERILOG_PARSER_PTIFWD_H
#define LIBYM_VERILOG_PARSER_PTIFWD_H

/// @file libym_verilog/parser/PtiFwd.h
/// @brief Pti クラスの名前の宣言のみおこなうヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: PtiFwd.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_verilog/verilog.h"
#include "ym_verilog/pt/PtArray.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス名の前方宣言
//////////////////////////////////////////////////////////////////////

// in PtiFactory.h
class PtiFactory;

// in PtiDecl.h
class PtiPort;
class PtiPortRef;
class PtiIOHead;
class PtiDeclHead;

typedef PtArray<PtiPort> PtiPortArray;
typedef PtArray<PtiPortRef> PtiPortRefArray;

END_NAMESPACE_YM_VERILOG

#endif // LIBYM_VERILOG_PARSER_PTIFWD_H
