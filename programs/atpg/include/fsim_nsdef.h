#ifndef ATPG_FSIM_NSDEF_H
#define ATPG_FSIM_NSDEF_H

/// @file include/fsim_nsdef.h
/// @brief Fsim 用の名前空間の定義
///
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: fsim_nsdef.h 2128 2009-03-12 07:25:33Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.

// 名前空間の定義ファイル
// 必ず先頭でインクルードしなければならない．

// namespace でネストするのがいやなので define マクロでごまかす．

#include "atpg_nsdef.h"

#define BEGIN_NAMESPACE_YM_ATPG_FSIM \
BEGIN_NAMESPACE_YM_ATPG \
BEGIN_NAMESPACE(nsFsim)

#define END_NAMESPACE_YM_ATPG_FSIM \
END_NAMESPACE(nsFsim) \
END_NAMESPACE_YM_ATPG


BEGIN_NAMESPACE_YM_ATPG_FSIM

class Fsim;

END_NAMESPACE_YM_ATPG_FSIM

BEGIN_NAMESPACE_YM_ATPG

using nsFsim::Fsim;

END_NAMESPACE_YM_ATPG

#endif // ATPG_FSIM_NSDEF_H
