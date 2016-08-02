#ifndef YM_UTILS_RANDOM_H
#define	YM_UTILS_RANDOM_H

/// @file ym_utils/random.h
/// @brief 乱数発生関数のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: random.h 400 2007-03-06 17:58:07Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ymtools.h"


BEGIN_NAMESPACE_YM

/// @name 乱数発生関数
/// @{

/// @ingroup YmUtils
/// @brief 乱数のシードを初期化する．
///
/// @param[in] seed 乱数の種
void
init_random_seed(unsigned long seed);

/// @ingroup YmUtils
/// @brief 乱数を返す．
///
/// @return 乱数
long
random_num();

/// @}

END_NAMESPACE_YM

#endif // YM_UTILS_RANDOM_H
