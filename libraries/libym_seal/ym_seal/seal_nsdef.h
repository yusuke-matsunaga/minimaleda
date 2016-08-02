#ifndef YM_SEAL_SEAL_NSDEF_H
#define YM_SEAL_SEAL_NSDEF_H

/// @file ym_seal/seal_nsdef.h
/// @brief seal パッケージに共通な定義ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: seal.h 1343 2008-03-25 17:15:35Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ymtools.h"

/// @brief seal 用の名前空間の開始
#define BEGIN_NAMESPACE_YM_SEAL \
BEGIN_NAMESPACE_YM \
BEGIN_NAMESPACE(nsSeal)

/// @brief seal 用の名前空間の終了
#define END_NAMESPACE_YM_SEAL \
END_NAMESPACE(nsSeal) \
END_NAMESPACE_YM

#endif // YM_SEAL_SEAL_NSDEF_H
