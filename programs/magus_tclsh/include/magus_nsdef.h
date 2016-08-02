#ifndef MAGUS_MAGUS_NSDEF_H
#define MAGUS_MAGUS_NSDEF_H

/// @file magus.h
/// @brief magus用名前空間の定義ファイル
///
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: magus.h 1343 2008-03-25 17:15:35Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.

/// @defgroup MagusGroup Magus 関連クラス

#include "ymtools.h"

#define BEGIN_NAMESPACE_MAGUS \
BEGIN_NAMESPACE_YM \
BEGIN_NAMESPACE(nsMagus)

#define END_NAMESPACE_MAGUS \
END_NAMESPACE(nsMagus) \
END_NAMESPACE_YM


BEGIN_NAMESPACE_MAGUS

// 前方参照のためのクラス宣言
class MagMgr;
class NetHandle;

END_NAMESPACE_MAGUS

#endif // MAGUS_MAGUS_NSDEF_H
