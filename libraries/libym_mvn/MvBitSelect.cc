
/// @file libym_mvn/MvBitSelect.cc
/// @brief MvBitSelect の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "MvBitSelect.h"
#include "ym_mvn/MvMgr.h"


BEGIN_NAMESPACE_YM_MVN

//////////////////////////////////////////////////////////////////////
// クラス MvBitSelect
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] module 親のモジュール
// @param[in] bit_width1 入力のビット幅
// @param[in] bit_width2 ビット指定入力のビット幅
MvBitSelect::MvBitSelect(MvModule* module,
			 ymuint bit_width1,
			 ymuint bit_width2) :
  MvBinaryOp(module, bit_width1, bit_width2, 1)
{
}

// @brief デストラクタ
MvBitSelect::~MvBitSelect()
{
}

// @brief ノードの種類を得る．
MvNode::tType
MvBitSelect::type() const
{
  return kBitSelect;
}

// @brief 可変 bit-selectノードを生成する．
// @param[in] module ノードが属するモジュール
// @param[in] bit_width1 入力のビット幅
// @param[in] bit_width2 ビット指定入力のビット幅
// @return 生成したノードを返す．
MvNode*
MvMgr::new_bitselect(MvModule* module,
		     ymuint bit_width1,
		     ymuint bit_width2)
{
  MvNode* node = new MvBitSelect(module, bit_width1, bit_width2);
  reg_node(node);

  return node;
}

END_NAMESPACE_YM_MVN
