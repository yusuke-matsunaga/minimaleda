
/// @file magus/logbase/NetHandle.cc
/// @brief NetHandle の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: NetHandle.cc 2274 2009-06-10 07:45:29Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "NetHandle.h"


BEGIN_NAMESPACE_MAGUS

//////////////////////////////////////////////////////////////////////
// クラス NetHandle
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] name 名前
NetHandle::NetHandle(const string& name) :
  mName(name),
  mCount(0)
{
}

// @brief デストラクタ
NetHandle::~NetHandle()
{
}

// @brief BNetwork を得る．
const BNetwork*
NetHandle::bnetwork() const
{
  return NULL;
}

// @brief BNetwork を得る．
BNetwork*
NetHandle::_bnetwork()
{
  return NULL;
}

// @brief BdNetwork を得る．
const BdNetwork*
NetHandle::bdn() const
{
  return NULL;
}

// @brief BdNetwork を得る．
BdNetwork*
NetHandle::_bdn()
{
  return NULL;
}

// @brief MvNetwork を得る．
const MvMgr*
NetHandle::mvn() const
{
  return NULL;
}

// @brief MvNetwork を得る．
MvMgr*
NetHandle::_mvn()
{
  return NULL;
}

END_NAMESPACE_MAGUS
