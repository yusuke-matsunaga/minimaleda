#ifndef ATPG_FSIMX_NSDEF_H
#define ATPG_FSIMX_NSDEF_H

/// @file include/fsimx_nsdef.h
/// @brief FsimX 用の名前空間の定義
///
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: fsimx_nsdef.h 2203 2009-04-16 05:04:40Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.

// 名前空間の定義ファイル
// 必ず先頭でインクルードしなければならない．

// namespace でネストするのがいやなので define マクロでごまかす．

#include "atpg_nsdef.h"

#define BEGIN_NAMESPACE_YM_ATPG_FSIMX \
BEGIN_NAMESPACE_YM_ATPG \
BEGIN_NAMESPACE(nsFsimX)

#define END_NAMESPACE_YM_ATPG_FSIMX \
END_NAMESPACE(nsFsimX) \
END_NAMESPACE_YM_ATPG


BEGIN_NAMESPACE_YM_ATPG_FSIMX

class FsimX;

END_NAMESPACE_YM_ATPG_FSIMX

BEGIN_NAMESPACE_YM_ATPG

using nsFsimX::FsimX;

END_NAMESPACE_YM_ATPG

#endif // ATPG_FSIMX_NSDEF_H
