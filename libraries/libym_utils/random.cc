
/// @file libym_utils/random.cc
/// @brief 乱数発生関数の実装ファイル
/// @author Yusuke Matsunaga
///
/// $Id: random.cc 700 2007-05-31 00:41:30Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_utils/random.h"


BEGIN_NAMESPACE_YM

// 乱数のシードを初期化する．
void
init_random_seed(unsigned long seed)
{
#if HAVE_RANDOM
  srandom(seed);
#elif HAVE_LRAND48
  srand48((long) seed);
#else
# error "no srandom() nor srand48()"
#endif
}

// 乱数を生成する関数
long
random_num()
{
#if HAVE_RANDOM
  return random();
#elif HAVE_RANDOM
  return lrand48();
#else
# error "no random() nor lrand48()"
  return 0; // dummy
#endif
}

END_NAMESPACE_YM
