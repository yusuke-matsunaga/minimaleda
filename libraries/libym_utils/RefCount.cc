
/// @file libym_utils/RefCount.cc
/// @brief RefCount の実装ファイル
/// @author Yusuke Matsunaga
///
/// $Id: RefCount.cc 700 2007-05-31 00:41:30Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_utils/RefCount.h"


BEGIN_NAMESPACE_YM

// デストラクタ
RefCount::~RefCount()
{
}

// 参照回数が 0 になったときのデフォルトの動作
void
RefCount::suicide()
{
  delete this;
}

END_NAMESPACE_YM
