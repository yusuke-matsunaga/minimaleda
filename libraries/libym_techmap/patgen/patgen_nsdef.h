#ifndef LIBYM_TECHMAP_PATGEN_PATGEN_NSDEF_H
#define LIBYM_TECHMAP_PATGEN_PATGEN_NSDEF_H

/// @file libym_techmap/patgen/patgen_nsdef.h
/// @brief patgen 用の名前空間の定義
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_techmap/techmap_nsdef.h"

/// @brief patgen 用の名前空間の開始
#define BEGIN_NAMESPACE_YM_TECHMAP_PATGEN \
BEGIN_NAMESPACE_YM_TECHMAP \
BEGIN_NAMESPACE(nsPatgen)

/// @brief patgen 用の名前空間の終了
#define END_NAMESPACE_YM_TECHMAP_PATGEN \
END_NAMESPACE(nsPatgen) \
END_NAMESPACE_YM_TECHMAP

#endif // LIBYM_TECHMAP_PATGEN_PATGEN_NSDEF_H
