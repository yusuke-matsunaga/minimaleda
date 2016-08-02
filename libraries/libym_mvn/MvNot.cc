
/// @file libym_mvn/MvNot.cc
/// @brief MvNot の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "MvNot.h"
#include "ym_mvn/MvMgr.h"


BEGIN_NAMESPACE_YM_MVN

// @brief コンストラクタ
// @param[in] module 親のモジュール
// @param[in] bit_width ビット幅
MvNot::MvNot(MvModule* module,
	     ymuint bit_width) :
  MvUnaryOp(module, bit_width, bit_width)
{
}

// @brief デストラクタ
MvNot::~MvNot()
{
}

// @brief ノードの種類を得る．
MvNode::tType
MvNot::type() const
{
  return kNot;
}

// @brief not ノードを生成する．
// @param[in] module ノードが属するモジュール
// @param[in] bit_width ビット幅
// @return 生成したノードを返す．
MvNode*
MvMgr::new_not(MvModule* module,
	       ymuint bit_width)
{
  MvNode* node = new MvNot(module, bit_width);
  reg_node(node);

  assert_cond( node->input_num() == 1, __FILE__, __LINE__);
  assert_cond( node->output_num() == 1, __FILE__, __LINE__);

  return node;
}

END_NAMESPACE_YM_MVN
