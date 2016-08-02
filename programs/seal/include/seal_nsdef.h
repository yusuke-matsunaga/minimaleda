#ifndef SEAL_SEAL_NSDEF_H
#define SEAL_SEAL_NSDEF_H

/// @file seal/seal_nsdef.h
/// @brief SEAL 用の名前空間の定義
///
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: seal_nsdef.h 1920 2008-12-20 15:52:42Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.

// 名前空間の定義ファイル
// 必ず先頭でインクルードしなければならない．

// namespace でネストするのがいやなので define マクロでごまかす．

#include "ymtools.h"

#define BEGIN_NAMESPACE_YM_SEAL \
BEGIN_NAMESPACE_YM \
BEGIN_NAMESPACE(nsSeal)

#define END_NAMESPACE_YM_SEAL \
END_NAMESPACE(nsSeal) \
END_NAMESPACE_YM

#endif // SEAL_SEAL_NSDEF_H
