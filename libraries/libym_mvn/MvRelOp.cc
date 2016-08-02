
/// @file libym_mvn/MvRelOp.cc
/// @brief MvRelOp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "MvRelOp.h"
#include "ym_mvn/MvMgr.h"


BEGIN_NAMESPACE_YM_MVN

//////////////////////////////////////////////////////////////////////
// クラス MvEq
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] module 親のモジュール
// @param[in] bit_width ビット幅
MvEq::MvEq(MvModule* module,
	   ymuint bit_width) :
  MvBinaryOp(module, bit_width, bit_width, 1)
{
}

// @brief デストラクタ
MvEq::~MvEq()
{
}

// @brief ノードの種類を得る．
MvNode::tType
MvEq::type() const
{
  return kEq;
}

// @brief equal ノードを生成する．
// @param[in] module ノードが属するモジュール
// @param[in] bit_width ビット幅
// @return 生成したノードを返す．
MvNode*
MvMgr::new_equal(MvModule* module,
		 ymuint bit_width)
{
  MvNode* node = new MvEq(module, bit_width);
  reg_node(node);

  assert_cond( node->input_num() == 2, __FILE__, __LINE__);
  assert_cond( node->output_num() == 1, __FILE__, __LINE__);

  return node;
}


//////////////////////////////////////////////////////////////////////
// クラス MvLt
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] module 親のモジュール
// @param[in] bit_width ビット幅
MvLt::MvLt(MvModule* module,
	   ymuint bit_width) :
  MvBinaryOp(module, bit_width, bit_width, 1)
{
}

// @brief デストラクタ
MvLt::~MvLt()
{
}

// @brief ノードの種類を得る．
MvNode::tType
MvLt::type() const
{
  return kLt;
}

// @brief less than ノードを生成する．
// @param[in] module ノードが属するモジュール
// @param[in] bit_width ビット幅
// @return 生成したノードを返す．
MvNode*
MvMgr::new_lt(MvModule* module,
	      ymuint bit_width)
{
  MvNode* node = new MvLt(module, bit_width);
  reg_node(node);

  assert_cond( node->input_num() == 2, __FILE__, __LINE__);
  assert_cond( node->output_num() == 1, __FILE__, __LINE__);

  return node;
}

END_NAMESPACE_YM_MVN
