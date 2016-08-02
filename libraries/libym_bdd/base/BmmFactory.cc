
/// @file libym_bdd/base/BmmFactory.cc
/// @brief BmmFactory の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: BmmFactory.cc 700 2007-05-31 00:41:30Z matsunaga $
///
/// Copyright (C) 2005-2006 Yusuke Matsunaga
/// All rights reserved.


#if HAVE_CONFIG_H
#include <ymconfig.h>
#endif

#include <ym_bdd/BmmFactory.h>

#include "bmm/BddMgrModern.h"


BEGIN_NAMESPACE_YM_BDD

//////////////////////////////////////////////////////////////////////
// BddMgrModern を生成するクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
BmmFactory::BmmFactory(bool reorder,
		       const string& name) :
  mReorder(reorder),
  mName(name)
{
}

// デストラクタ
BmmFactory::~BmmFactory()
{
}

// BddMgrModern を生成する．
BddMgr*
BmmFactory::operator()() const
{
  return new BddMgrModern(mReorder, mName);
}

END_NAMESPACE_YM_BDD
