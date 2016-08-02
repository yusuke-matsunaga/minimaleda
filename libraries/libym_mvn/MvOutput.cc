
/// @file libym_mvn/MvOutput.cc
/// @brief MvOutput の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "MvOutput.h"


BEGIN_NAMESPACE_YM_MVN

// @brief コンストラクタ
// @param[in] module 親のモジュール
// @param[in] bit_width ビット幅
MvOutput::MvOutput(MvModule* module,
		   ymuint bit_width) :
  MvUnaryOp(module, bit_width, bit_width)
{
}

// @brief デストラクタ
MvOutput::~MvOutput()
{
}

// @brief ノードの種類を得る．
MvNode::tType
MvOutput::type() const
{
  return kOutput;
}

END_NAMESPACE_YM_MVN
