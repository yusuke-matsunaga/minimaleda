
/// @file libym_mvn/MvInout.cc
/// @brief MvInout の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "MvInout.h"


BEGIN_NAMESPACE_YM_MVN

// @brief コンストラクタ
// @param[in] module 親のモジュール
// @param[in] bit_width ビット幅
MvInout::MvInout(MvModule* module,
		 ymuint bit_width) :
  MvUnaryOp(module, bit_width, bit_width)
{
}

// @brief デストラクタ
MvInout::~MvInout()
{
}

// @brief ノードの種類を得る．
MvNode::tType
MvInout::type() const
{
  return kInout;
}

END_NAMESPACE_YM_MVN
