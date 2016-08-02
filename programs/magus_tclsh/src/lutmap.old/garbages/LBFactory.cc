
/// @file magus/lutmap/LBFactory.cc
/// @brief LBFactory の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: LBFactory.cc 1077 2007-12-03 10:18:47Z matsunaga $
///
/// Copyright (C) 2005-2007 Yusuke Matsunaga
/// All rights reserved.


#if HAVE_CONFIG_H
#include <magus_config.h>
#endif

#include "LowerBound.h"
#include "TreeLB.h"
#include "WeightLB.h"
#include "XLB.h"


BEGIN_NAMESPACE_MAGUS_LUTMAP

//////////////////////////////////////////////////////////////////////
// クラス LBFactory
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
LBFactory::LBFactory()
{
}
  
// @brief デストラクタ
LBFactory::~LBFactory()
{
}

/// @brief LowerBound を生成する．
LowerBound*
LBFactory::operator()(const string& args) const
{
  if ( args == "tree" ) {
    // 本当はその後ろの数字を読む．
    return new TreeLB(1);
  }
  if ( args == "weight" ) {
    return new WeightLB;
  }
  if ( args == "xlb" ) {
    // 本当はその後ろの数字を読む．
    return new XLB(1);
  }
  assert_not_reached(__FILE__, __LINE__);
  return NULL;
}


END_NAMESPACE_MAGUS_LUTMAP
