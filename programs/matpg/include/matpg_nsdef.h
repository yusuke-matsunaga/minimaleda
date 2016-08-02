#ifndef MATPG_MATPG_NSDEF_H
#define MATPG_MATPG_NSDEF_H

/// @file matpg/matpg_nsdef.h
/// @brief MATPG 用の名前空間の定義
///
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: matpg_nsdef.h 2043 2009-02-21 07:08:48Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.

// 名前空間の定義ファイル
// 必ず先頭でインクルードしなければならない．

// namespace でネストするのがいやなので define マクロでごまかす．

#include "ymtools.h"

#define BEGIN_NAMESPACE_YM_MATPG \
BEGIN_NAMESPACE_YM \
BEGIN_NAMESPACE(nsMatpg)

#define END_NAMESPACE_YM_MATPG \
END_NAMESPACE(nsMatpg) \
END_NAMESPACE_YM

#endif // MATPG_MATPG_NSDEF_H
