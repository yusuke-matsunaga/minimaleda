#ifndef CALC_CVF_NSDEF_H
#define CALC_CVF_NSDEF_H

/// @file calc_cvf/nsdef.h
/// @brief calc_cvf 用の名前空間の定義
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

#define BEGIN_NAMESPACE_YM_SEAL_CVF \
BEGIN_NAMESPACE_YM_SEAL \
BEGIN_NAMESPACE(nsCvf)

#define END_NAMESPACE_YM_SEAL_CVF \
END_NAMESPACE(nsCvf)	    \
END_NAMESPACE_YM_SEAL

#endif // CALC_CVF_NSDEF_H
