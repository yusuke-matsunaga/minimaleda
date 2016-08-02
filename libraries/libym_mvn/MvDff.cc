
/// @file libym_mvn/MvDff.cc
/// @brief MvDff の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "MvDff.h"
#include "ym_mvn/MvMgr.h"


BEGIN_NAMESPACE_YM_MVN

//////////////////////////////////////////////////////////////////////
// クラス MvDff
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] module 親のモジュール
// @param[in] bit_width ビット幅
MvDff::MvDff(MvModule* module,
	     ymuint bit_width) :
  MvNode(module, 4, 1)
{
  set_ipin_bit_width(0, bit_width);
  set_ipin_bit_width(1, 1);
  set_ipin_bit_width(2, 1);
  set_ipin_bit_width(3, 1);
  set_opin_bit_width(0, bit_width);
}

// @brief デストラクタ
MvDff::~MvDff()
{
}


//////////////////////////////////////////////////////////////////////
// クラス MvDff1
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] module 親のモジュール
// @param[in] bit_width ビット幅
MvDff1::MvDff1(MvModule* module,
	       ymuint bit_width) :
  MvDff(module, bit_width)
{
}

// @brief デストラクタ
MvDff1::~MvDff1()
{
}

// @brief ノードの種類を得る．
MvNode::tType
MvDff1::type() const
{
  return kDff1;
}

// @brief 非同期セット/リセットタイプの FF ノードを生成する．
MvNode*
MvMgr::new_dff1(MvModule* module,
		ymuint bit_width)
{
  MvNode* node = new MvDff1(module, bit_width);
  reg_node(node);

  assert_cond( node->input_num() == 4, __FILE__, __LINE__);
  assert_cond( node->output_num() == 1, __FILE__, __LINE__);

  return node;
}


//////////////////////////////////////////////////////////////////////
// クラス MvDff2
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] module 親のモジュール
// @param[in] bit_width ビット幅
MvDff2::MvDff2(MvModule* module,
	       ymuint bit_width) :
  MvDff(module, bit_width)
{
}

// @brief デストラクタ
MvDff2::~MvDff2()
{
}

// @brief ノードの種類を得る．
MvNode::tType
MvDff2::type() const
{
  return kDff2;
}

// @brief 同期セット/リセットタイプの FF ノードを生成する．
MvNode*
MvMgr::new_dff2(MvModule* module,
		ymuint bit_width)
{
  MvNode* node = new MvDff2(module, bit_width);
  reg_node(node);

  assert_cond( node->input_num() == 4, __FILE__, __LINE__);
  assert_cond( node->output_num() == 1, __FILE__, __LINE__);

  return node;
}

END_NAMESPACE_YM_MVN
