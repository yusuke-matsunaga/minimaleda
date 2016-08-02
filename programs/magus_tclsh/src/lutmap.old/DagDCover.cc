
/// @file magus/lutmap/DagDCover.cc
/// @brief DagDCover の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: DagDCover.cc 1293 2008-02-21 02:25:52Z matsunaga $
///
/// Copyright (C) 2005-2007 Yusuke Matsunaga
/// All rights reserved.


#if HAVE_CONFIG_H
#include "magus_config.h"
#endif


#include "DagDCover.h"
#include "WeightDCover.h"


BEGIN_NAMESPACE_MAGUS_LUTMAP

//////////////////////////////////////////////////////////////////////
// クラス DagDCoverFactory
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
DagDCoverFactory::DagDCoverFactory()
{
}

// @brief デストラクタ
DagDCoverFactory::~DagDCoverFactory()
{
}

// @brief DagDCover (の派生クラス)を生成する．
DagDCover*
DagDCoverFactory::operator()(const string& args)
{
  if ( args == "dagd" ) {
    return new WeightDCover(0);
  }
  else if ( args == "dag" ) {
    return new WeightDCover(0);
  }
  else if ( args == "dagdr" ) {
    return new WeightDCover(2);
  }
  else if ( args == "fo" ) {
    return new WeightDCover(1);
  }
  else if ( args == "fod" ) {
    return new WeightDCover(1);
  }
  else if ( args == "fodr" ) {
    return new WeightDCover(3);
  }
  assert_not_reached(__FILE__, __LINE__);
  return NULL;
}

END_NAMESPACE_MAGUS_LUTMAP
