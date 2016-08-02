
/// @file libym_mvn/MvLogOp.cc
/// @brief MvLogOp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "MvLogOp.h"
#include "ym_mvn/MvMgr.h"


BEGIN_NAMESPACE_YM_MVN

//////////////////////////////////////////////////////////////////////
// クラス MvAnd
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] module 親のモジュール
// @param[in] bit_width ビット幅
MvAnd::MvAnd(MvModule* module,
	     ymuint bit_width) :
  MvBinaryOp(module, bit_width, bit_width, bit_width)
{
}

// @brief デストラクタ
MvAnd::~MvAnd()
{
}

// @brief ノードの種類を得る．
MvNode::tType
MvAnd::type() const
{
  return kAnd;
}

// @brief and ノードを生成する．
// @param[in] module ノードが属するモジュール
// @param[in] bit_width ビット幅
// @return 生成したノードを返す．
MvNode*
MvMgr::new_and(MvModule* module,
	       ymuint bit_width)
{
  MvNode* node = new MvAnd(module, bit_width);
  reg_node(node);

  assert_cond( node->input_num() == 2, __FILE__, __LINE__);
  assert_cond( node->output_num() == 1, __FILE__, __LINE__);

  return node;
}


//////////////////////////////////////////////////////////////////////
// クラス MvOr
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] module 親のモジュール
// @param[in] bit_width ビット幅
MvOr::MvOr(MvModule* module,
	     ymuint bit_width) :
  MvBinaryOp(module, bit_width, bit_width, bit_width)
{
}

// @brief デストラクタ
MvOr::~MvOr()
{
}

// @brief ノードの種類を得る．
MvNode::tType
MvOr::type() const
{
  return kOr;
}

// @brief or ノードを生成する．
// @param[in] module ノードが属するモジュール
// @param[in] bit_width ビット幅
// @return 生成したノードを返す．
MvNode*
MvMgr::new_or(MvModule* module,
	      ymuint bit_width)
{
  MvNode* node = new MvOr(module, bit_width);
  reg_node(node);

  assert_cond( node->input_num() == 2, __FILE__, __LINE__);
  assert_cond( node->output_num() == 1, __FILE__, __LINE__);

  return node;
}


//////////////////////////////////////////////////////////////////////
// クラス MvXor
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] module 親のモジュール
// @param[in] bit_width ビット幅
MvXor::MvXor(MvModule* module,
	     ymuint bit_width) :
  MvBinaryOp(module, bit_width, bit_width, bit_width)
{
}

// @brief デストラクタ
MvXor::~MvXor()
{
}

// @brief ノードの種類を得る．
MvNode::tType
MvXor::type() const
{
  return kXor;
}

// @brief xor ノードを生成する．
// @param[in] module ノードが属するモジュール
// @param[in] bit_width ビット幅
// @return 生成したノードを返す．
MvNode*
MvMgr::new_xor(MvModule* module,
	       ymuint bit_width)
{
  MvNode* node = new MvXor(module, bit_width);
  reg_node(node);

  assert_cond( node->input_num() == 2, __FILE__, __LINE__);
  assert_cond( node->output_num() == 1, __FILE__, __LINE__);

  return node;
}

END_NAMESPACE_YM_MVN
