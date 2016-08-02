
/// @file src/SealMgr.cc
/// @brief SealMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: SealMgr.cc 1920 2008-12-20 15:52:42Z matsunaga $
///
/// Copyright (C) 2005-2008 Yusuke Matsunaga
/// All rights reserved.


#if HAVE_CONFIG_H
#include "seal_config.h"
#endif

#include "SealMgr.h"
#include <ym_tgnet/TgNetwork.h>


BEGIN_NAMESPACE_YM_SEAL

//////////////////////////////////////////////////////////////////////
// SealMgr
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SealMgr::SealMgr() :
  mNetwork(new TgNetwork)
{
}

// @brief デストラクタ
SealMgr::~SealMgr()
{
  delete mNetwork;
}

// @brief TgNetwork を取り出す．
TgNetwork&
SealMgr::_network()
{
  return *mNetwork;
}

END_NAMESPACE_YM_SEAL
