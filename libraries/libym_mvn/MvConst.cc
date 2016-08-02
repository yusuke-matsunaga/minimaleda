
/// @file libym_mvn/MvConst.cc
/// @brief MvConst の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "MvConst.h"
#include "ym_mvn/MvMgr.h"


BEGIN_NAMESPACE_YM_MVN

// @brief コンストラクタ
// @param[in] module 親のモジュール
// @param[in] bit_width ビット幅
// @param[in] val 値
MvConst::MvConst(MvModule* module,
		 ymuint bit_width,
		 const vector<ymuint32>& val) :
  MvNode(module, 0, 1),
  mVal(val)
{
  set_opin_bit_width(0, bit_width);
}

// @brief デストラクタ
MvConst::~MvConst()
{
}

// @brief ノードの種類を得る．
MvNode::tType
MvConst::type() const
{
  return kConst;
}

// @brief 定数値を得る．
// @param[out] val 値を格納するベクタ
// @note type() が kConst の時のみ意味を持つ．
void
MvConst::const_value(vector<ymuint32>& val) const
{
  val = mVal;
}

// @brief constant ノードを生成する．
// @param[in] module ノードが属するモジュール
// @param[in] bit_width ビット幅
// @param[in] val 値
MvNode*
MvMgr::new_const(MvModule* module,
		 ymuint bit_width,
		 const vector<ymuint32>& val)
{
  MvNode* node = new MvConst(module, bit_width, val);
  reg_node(node);

  assert_cond( node->input_num() == 0, __FILE__, __LINE__);
  assert_cond( node->output_num() == 1, __FILE__, __LINE__);

  return node;
}

END_NAMESPACE_YM_MVN
