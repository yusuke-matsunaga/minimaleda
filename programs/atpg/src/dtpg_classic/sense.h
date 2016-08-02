#ifndef DTPG_CLASSIC_SENSE_H
#define DTPG_CLASSIC_SENSE_H

/// @file src/dtpg_classic/sense.h
/// @brief unique sensitization を行う関数の定義ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: sense.h 2128 2009-03-12 07:25:33Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "dtpgcl_nsdef.h"


BEGIN_NAMESPACE_YM_ATPG_DTPGCL

class Gate;

extern
bool
sensitize(Gate* f_node);

END_NAMESPACE_YM_ATPG_DTPGCL

#endif
