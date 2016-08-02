
/// @file libym_cell/ci/CiBundle.cc
/// @brief CiBundle の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "CiBundle.h"


BEGIN_NAMESPACE_YM_CELL

//////////////////////////////////////////////////////////////////////
// クラス CiBundle
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
CiBundle::CiBundle() :
  mPinNum(0),
  mPinList(NULL)
{
}

// @brief デストラクタ
CiBundle::~CiBundle()
{
  // メモリ管理は他のクラスがやる．
}

// @brief 名前の取得
string
CiBundle::name() const
{
  return mName;
}

// @brief ピン数の取得
ymuint
CiBundle::pin_num() const
{
  return mPinNum;
}

// @brief ピンの取得
// @param[in] pos 位置番号 ( 0 <= pos < pin_num() )
const CellPin*
CiBundle::pin(ymuint pos) const
{
  return mPinList[pos];
}

END_NAMESPACE_YM_CELL
