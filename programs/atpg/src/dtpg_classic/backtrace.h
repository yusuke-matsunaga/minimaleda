#ifndef DTPG_CLASSIC_BACKTRACE_H
#define DTPG_CLASSIC_BACKTRACE_H

/// @file src/dtpg_classic/backtrace.h
/// @brief バックトレースを行う関数の定義ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: backtrace.h 2128 2009-03-12 07:25:33Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "dtpgcl_nsdef.h"
#include "Val3.h"


BEGIN_NAMESPACE_YM_ATPG_DTPGCL

class Gate;

extern
bool
C_decision(Gate* f_node,
	   Gate*& o_gate,
	   Val3& o_gval,
	   Val3& o_fval);

extern
bool
O_decision(Gate* f_node,
	   Gate*& o_gate,
	   Val3& o_gval,
	   Val3& o_fval);

END_NAMESPACE_YM_ATPG_DTPGCL

#endif
