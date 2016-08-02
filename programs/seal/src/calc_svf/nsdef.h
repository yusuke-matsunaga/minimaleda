#ifndef CALC_SVF_NSDEF_H
#define CALC_SVF_NSDEF_H

/// @file calc_svf/nsdef.h
/// @brief calc_svf 用の名前空間の定義
///
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: nsdef.h 1920 2008-12-20 15:52:42Z matsunaga $
///
/// Copyright (C) 2005-2008 Yusuke Matsunaga
/// All rights reserved.

// 名前空間の定義ファイル
// 必ず先頭でインクルードしなければならない．

// namespace でネストするのがいやなので define マクロでごまかす．

#include "seal_nsdef.h"

#define BEGIN_NAMESPACE_YM_SEAL_SVF \
BEGIN_NAMESPACE_YM_SEAL \
BEGIN_NAMESPACE(nsSvf)

#define END_NAMESPACE_YM_SEAL_SVF \
END_NAMESPACE(nsSvf)	    \
END_NAMESPACE_YM_SEAL

#endif // CALC_SVF_NSDEF_H
