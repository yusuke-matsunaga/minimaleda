
/// @file libym_bdd/base/BmcFactory.cc
/// @brief BmcFactory の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: BmcFactory.cc 700 2007-05-31 00:41:30Z matsunaga $
///
/// Copyright (C) 2005-2006 Yusuke Matsunaga
/// All rights reserved.


#if HAVE_CONFIG_H
#include <ymconfig.h>
#endif

#include <ym_bdd/BmcFactory.h>

#include "bmc/BddMgrClassic.h"


BEGIN_NAMESPACE_YM_BDD

//////////////////////////////////////////////////////////////////////
// BddMgrClassic を生成するクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
BmcFactory::BmcFactory(const string& name) :
  mName(name)
{
}

// デストラクタ
BmcFactory::~BmcFactory()
{
}

// BddMgrClassic を生成する．
BddMgr*
BmcFactory::operator()() const
{
  return new BddMgrClassic(mName);
}

END_NAMESPACE_YM_BDD
