
/// @file magus/lutmap/DagCoverFactory.cc
/// @brief DagCoverFactory の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: DagACover.cc 1077 2007-12-03 10:18:47Z matsunaga $
///
/// Copyright (C) 2005-2007 Yusuke Matsunaga
/// All rights reserved.


#if HAVE_CONFIG_H
#include "magus_config.h"
#endif

#include "DagACover.h"
#include "TreeCover.h"
#include "DFCover.h"
#include "WeightCover.h"


BEGIN_NAMESPACE_MAGUS_LUTMAP

//////////////////////////////////////////////////////////////////////
// クラス DagACoverFactory
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
DagACoverFactory::DagACoverFactory()
{
}

// @brief デストラクタ
DagACoverFactory::~DagACoverFactory()
{
}

// @brief DagACover (の派生クラス)を生成する．
DagACover*
DagACoverFactory::operator()(const string& args)
{
  if ( args == "tree" ) {
    return new TreeCover;
  }
  if ( args == "df" ) {
    return new DFCover;
  }
  if ( args == "dag" ) {
    return new WeightCover(0);
  }
  if ( args == "dagr" ) {
    return new WeightCover(2);
  }
  if ( args == "fo" ) {
    return new WeightCover(1);
  }
  if ( args == "for" ) {
    return new WeightCover(3);
  }
  assert_not_reached(__FILE__, __LINE__);
  return NULL;
}

END_NAMESPACE_MAGUS_LUTMAP
