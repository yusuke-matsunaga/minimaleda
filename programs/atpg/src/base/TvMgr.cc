
/// @file src/base/TvMgr.cc
/// @brief TvMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
/// 
/// $Id: TvMgr.cc 2203 2009-04-16 05:04:40Z matsunaga $
///
/// Copyright (C) 2005-2009 Yusuke Matsunaga
/// All rights reserved.

#if HAVE_CONFIG_H
#include "atpg_config.h"
#endif


#include "TvMgr.h"


BEGIN_NAMESPACE_YM_ATPG

// @brief コンストラクタ
TvMgr::TvMgr() :
  mAlloc(4096),
  mNi(0)
{
}

// @brief デストラクタ
TvMgr::~TvMgr()
{
  clear();
}

/// @brief 内容を空にする．
void
TvMgr::clear()
{
  if ( mNi > 0 ) {
    mAlloc.destroy();
    mNi = 0;
    mAvail.clear();
  }
}

// @brief 内容を初期化する．
// @param[in] ni 入力数
void
TvMgr::init(size_t ni)
{
  clear();
  
  mNi = ni;
  if ( mNi == 0 ) {
    // 0 だとヤバい
    mNi = 1;
  }
}

// @brief 新しいパタンを生成する．
// @return 生成されたパタンを返す．
// @note パタンは0で初期化される．
TestVector*
TvMgr::new_vector()
{
  if ( !mAvail.empty() ) {
    TestVector* tv = mAvail.back();
    mAvail.pop_back();
    return tv;
  }
  
  size_t nb = TestVector::block_num(mNi);
  size_t size = sizeof(TestVector) + kPvBitLen * (nb - 1);
  void* p = mAlloc.get_memory(size);
  TestVector* tv = new (p) TestVector(mNi);

  // X に初期化しておく．
  for (size_t i = 0; i < nb; ++ i) {
    tv->mPat[i] = kPvAll0;
  }

  return tv;
}
  
// @brief パタンを削除する．
void
TvMgr::delete_vector(TestVector* tv)
{
  mAvail.push_back(tv);
}

END_NAMESPACE_YM_ATPG
