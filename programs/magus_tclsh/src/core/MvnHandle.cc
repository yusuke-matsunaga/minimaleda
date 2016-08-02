
/// @file src/logbase/MvnHandle.cc
/// @brief MvnHandle の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "MvnHandle.h"
#include "ym_mvn/MvMgr.h"
#include "ym_bdn/BdNetwork.h"


BEGIN_NAMESPACE_MAGUS

//////////////////////////////////////////////////////////////////////
// クラス MvnHandle
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] name 名前
MvnHandle::MvnHandle(const string& name) :
  NetHandle(name)
{
}

// @brief デストラクタ
MvnHandle::~MvnHandle()
{
}

// @brief 型を返す．
NetHandle::tType
MvnHandle::type() const
{
  return kMagMvn;
}

// @brief MvMgr を得る．
const MvMgr*
MvnHandle::mvn() const
{
  return &mNetwork;
}

// @brief MvMgr を得る．
MvMgr*
MvnHandle::_mvn()
{
  return &mNetwork;
}

// @brief ネットワークをクリアする．
void
MvnHandle::clear()
{
  #warning "MvnHandle::clear(): no implemented yet"
}

// @brief ネットワークをコピーする．
// @param[in] src コピー元のネットハンドル
// @param[in] allow_conv true ならタイプが異なる時に変換する．
// @return コピーが成功したら true を返す．
bool
MvnHandle::copy(const NetHandle* src,
		 bool allow_conv)
{
  switch ( src->type() ) {
  case kMagBNet:
    return false;

  case kMagBdn:
    return false;

  case kMagMvn:
#warning "MvnHandle::copy(): not implemented yet"
    return false;

  default:
    assert_not_reached(__FILE__, __LINE__);
    break;
  }
  return true;
}

END_NAMESPACE_MAGUS
