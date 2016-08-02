
/// @file libym_mvn/MvModule.cc
/// @brief MvModule の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_mvn/MvModule.h"
#include "ym_mvn/MvPin.h"


BEGIN_NAMESPACE_YM_MVN

// @brief コンストラクタ
// @param[in] name 名前
// @param[in] np ポート数
// @param[in] ni 入力ノード数
// @param[in] no 出力ノード数
// @param[in] nio 入出力ノード数
MvModule::MvModule(const char* name,
		   ymuint np,
		   ymuint ni,
		   ymuint no,
		   ymuint nio) :
  mName(name),
  mParent(NULL),
  mPortArray(np),
  mInputArray(ni),
  mOutputArray(no),
  mInoutArray(nio)
{
}

// @brief デストラクタ
MvModule::~MvModule()
{
}

END_NAMESPACE_YM_MVN
