
/// @file magus/lutmap/DagDCover.cc
/// @brief DagDCover の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: DagDCover.cc 1293 2008-02-21 02:25:52Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_lutmap/DagDCover.h"
#include "ym_lutmap/WeightDCover.h"


BEGIN_NAMESPACE_YM_LUTMAP

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
  if ( args == "dag" ) {
    return new WeightDCover(0);
  }
  else if ( args == "dag-r" ) {
    return new WeightDCover(2);
  }
  else if ( args == "fo" ) {
    return new WeightDCover(1);
  }
  else if ( args == "fo-r" ) {
    return new WeightDCover(3);
  }
  assert_not_reached(__FILE__, __LINE__);
  return NULL;
}

END_NAMESPACE_YM_LUTMAP
