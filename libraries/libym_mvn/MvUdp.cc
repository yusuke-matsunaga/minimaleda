
/// @file libym_mvn/MvUdp.cc
/// @brief MvUdp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "MvUdp.h"
#include "ym_mvn/MvMgr.h"


BEGIN_NAMESPACE_YM_MVN

//////////////////////////////////////////////////////////////////////
// クラス MvCombUdp
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] module 親のモジュール
// @param[in] ni 入力数
MvCombUdp::MvCombUdp(MvModule* module,
		     ymuint ni) :
  MvNode(module, ni, 1)
{
  for (ymuint i = 0; i < ni; ++ i) {
    set_ipin_bit_width(i, 1);
  }
  set_opin_bit_width(0, 1);
}

// @brief デストラクタ
MvCombUdp::~MvCombUdp()
{
}

// @brief ノードの種類を得る．
MvNode::tType
MvCombUdp::type() const
{
  return kCombUdp;
}

// @brief combinational UDP ノードを生成する．
// @param[in] module ノードが属するモジュール
// @param[in] ni 入力数
// @note ビット幅はすべて1ビット
MvNode*
MvMgr::new_combudp(MvModule* module,
		   ymuint ni)
{
  MvNode* node = new MvCombUdp(module, ni);
  reg_node(node);

  return node;
}


//////////////////////////////////////////////////////////////////////
// クラス MvSeqUdp
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] module 親のモジュール
// @param[in] ni 入力数
MvSeqUdp::MvSeqUdp(MvModule* module,
		   ymuint ni) :
  MvNode(module, ni, 1)
{
  for (ymuint i = 0; i < ni; ++ i) {
    set_ipin_bit_width(i, 1);
  }
  set_opin_bit_width(0, 1);
}

// @brief デストラクタ
MvSeqUdp::~MvSeqUdp()
{
}

// @brief ノードの種類を得る．
MvNode::tType
MvSeqUdp::type() const
{
  return kSeqUdp;
}

// @brief sequential UDP ノードを生成する．
// @param[in] module ノードが属するモジュール
// @param[in] ni 入力数
// @note ビット幅はすべて1ビット
MvNode*
MvMgr::new_sequdp(MvModule* module,
		  ymuint ni)
{
  MvNode* node = new MvSeqUdp(module, ni);
  reg_node(node);

  return node;
}

END_NAMESPACE_YM_MVN
