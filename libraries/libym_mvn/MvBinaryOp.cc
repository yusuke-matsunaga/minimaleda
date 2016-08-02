
/// @file libym_mvn/MvBinaryOp.cc
/// @brief MvBinaryOp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "MvBinaryOp.h"


BEGIN_NAMESPACE_YM_MVN

// @brief コンストラクタ
// @param[in] module 親のモジュール
// @param[in] bit_width1 入力1のビット幅
// @param[in] bit_width2 入力2のビット幅
// @param[in] bit_width3 出力のビット幅
MvBinaryOp::MvBinaryOp(MvModule* module,
		       ymuint bit_width1,
		       ymuint bit_width2,
		       ymuint bit_width3) :
  MvNode(module, 2, 1)
{
  set_ipin_bit_width(0, bit_width1);
  set_ipin_bit_width(1, bit_width2);
  set_opin_bit_width(0, bit_width3);
}

// @brief デストラクタ
MvBinaryOp::~MvBinaryOp()
{
}

END_NAMESPACE_YM_MVN
