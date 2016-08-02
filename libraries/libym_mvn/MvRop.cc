
/// @file libym_mvn/MvRop.cc
/// @brief MvRop の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "MvRop.h"
#include "ym_mvn/MvMgr.h"


BEGIN_NAMESPACE_YM_MVN

//////////////////////////////////////////////////////////////////////
// クラス MvRand
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] module 親のモジュール
// @param[in] bit_width ビット幅
MvRand::MvRand(MvModule* module,
	       ymuint bit_width) :
  MvUnaryOp(module, bit_width, 1)
{
}

// @brief デストラクタ
MvRand::~MvRand()
{
}

// @brief ノードの種類を得る．
MvNode::tType
MvRand::type() const
{
  return kRand;
}

// @brief reduction and ノードを生成する．
// @param[in] module ノードが属するモジュール
// @param[in] bit_width ビット幅
// @return 生成したノードを返す．
MvNode*
MvMgr::new_rand(MvModule* module,
		ymuint bit_width)
{
  MvNode* node = new MvRand(module, bit_width);
  reg_node(node);

  assert_cond( node->input_num() == 1, __FILE__, __LINE__);
  assert_cond( node->output_num() == 1, __FILE__, __LINE__);
  
  return node;
}


//////////////////////////////////////////////////////////////////////
// クラス MvRor
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] module 親のモジュール
// @param[in] bit_width ビット幅
MvRor::MvRor(MvModule* module,
	       ymuint bit_width) :
  MvUnaryOp(module, bit_width, 1)
{
}

// @brief デストラクタ
MvRor::~MvRor()
{
}

// @brief ノードの種類を得る．
MvNode::tType
MvRor::type() const
{
  return kRor;
}

// @brief reduction or ノードを生成する．
// @param[in] module ノードが属するモジュール
// @param[in] bit_width ビット幅
// @return 生成したノードを返す．
MvNode*
MvMgr::new_ror(MvModule* module,
	       ymuint bit_width)
{
  MvNode* node = new MvRor(module, bit_width);
  reg_node(node);

  assert_cond( node->input_num() == 1, __FILE__, __LINE__);
  assert_cond( node->output_num() == 1, __FILE__, __LINE__);

  return node;
}


//////////////////////////////////////////////////////////////////////
// クラス MvRxor
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] module 親のモジュール
// @param[in] bit_width ビット幅
MvRxor::MvRxor(MvModule* module,
	       ymuint bit_width) :
  MvUnaryOp(module, bit_width, 1)
{
}

// @brief デストラクタ
MvRxor::~MvRxor()
{
}

// @brief ノードの種類を得る．
MvNode::tType
MvRxor::type() const
{
  return kRxor;
}

// @brief reduction xor ノードを生成する．
// @param[in] module ノードが属するモジュール
// @param[in] bit_width ビット幅
// @return 生成したノードを返す．
MvNode*
MvMgr::new_rxor(MvModule* module,
		ymuint bit_width)
{
  MvNode* node = new MvRxor(module, bit_width);
  reg_node(node);

  assert_cond( node->input_num() == 1, __FILE__, __LINE__);
  assert_cond( node->output_num() == 1, __FILE__, __LINE__);

  return node;
}

END_NAMESPACE_YM_MVN
