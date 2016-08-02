#ifndef BDDSIM_NSDEF_H
#define BDDSIM_NSDEF_H

/// @file bddsim/nsdef.h
/// @brief bddsim 用の名前空間の定義
///
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: nsdef.h 1920 2008-12-20 15:52:42Z matsunaga $
///
/// Copyright (C) 2005-2007 Yusuke Matsunaga
/// All rights reserved.

// 名前空間の定義ファイル
// 必ず先頭でインクルードしなければならない．

// namespace でネストするのがいやなので define マクロでごまかす．

#include "seal_nsdef.h"

#define BEGIN_NAMESPACE_YM_SEAL_BDDSIM \
BEGIN_NAMESPACE_YM_SEAL \
BEGIN_NAMESPACE(nsBddsim)

#define END_NAMESPACE_YM_SEAL_BDDSIM \
END_NAMESPACE(nsBddsim) \
END_NAMESPACE_YM_SEAL

#endif // BDDSIM_NSDEF_H
