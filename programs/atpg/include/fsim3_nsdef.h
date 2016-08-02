#ifndef ATPG_FSIM3_NSDEF_H
#define ATPG_FSIM3_NSDEF_H

/// @file include/fsim3_nsdef.h
/// @brief FsimX 用の名前空間の定義
///
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: fsim3_nsdef.h 2203 2009-04-16 05:04:40Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.

// 名前空間の定義ファイル
// 必ず先頭でインクルードしなければならない．

// namespace でネストするのがいやなので define マクロでごまかす．

#include "atpg_nsdef.h"

#define BEGIN_NAMESPACE_YM_ATPG_FSIM3 \
BEGIN_NAMESPACE_YM_ATPG \
BEGIN_NAMESPACE(nsFsim3)

#define END_NAMESPACE_YM_ATPG_FSIM3 \
END_NAMESPACE(nsFsim3) \
END_NAMESPACE_YM_ATPG


BEGIN_NAMESPACE_YM_ATPG_FSIM3

class Fsim3;

END_NAMESPACE_YM_ATPG_FSIM3

BEGIN_NAMESPACE_YM_ATPG

using nsFsim3::Fsim3;

END_NAMESPACE_YM_ATPG

#endif // ATPG_FSIM3_NSDEF_H
