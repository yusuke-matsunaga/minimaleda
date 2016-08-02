
/// @file src/logbase/BNetHandle.cc
/// @brief BNetHandle の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "BNetHandle.h"
#include "ym_bnet/BNetwork.h"
#include "ym_bdn/BdNetwork.h"


BEGIN_NAMESPACE_MAGUS

//////////////////////////////////////////////////////////////////////
// クラス BNetHandle
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] name 名前
BNetHandle::BNetHandle(const string& name) :
  NetHandle(name)
{
}

// @brief デストラクタ
BNetHandle::~BNetHandle()
{
}

// @brief 型を返す．
NetHandle::tType
BNetHandle::type() const
{
  return kMagBNet;
}

// @brief BNetwork を得る．
const BNetwork*
BNetHandle::bnetwork() const
{
  return &mNetwork;
}

// @brief BNetwork を得る．
BNetwork*
BNetHandle::_bnetwork()
{
  return &mNetwork;
}

// @brief ネットワークをクリアする．
void
BNetHandle::clear()
{
  mNetwork.clear();
}

// @brief ネットワークをコピーする．
// @param[in] src コピー元のネットハンドル
// @param[in] allow_conv true ならタイプが異なる時に変換する．
// @return コピーが成功したら true を返す．
bool
BNetHandle::copy(const NetHandle* src,
		 bool allow_conv)
{
  switch ( src->type() ) {
  case kMagBNet:
    mNetwork = *(src->bnetwork());
    break;

  case kMagBdn:
    return false;

  case kMagMvn:
    return false;

  default:
    assert_not_reached(__FILE__, __LINE__);
    break;
  }
  return true;
}

END_NAMESPACE_MAGUS
