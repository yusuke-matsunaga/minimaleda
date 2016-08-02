
/// @file libym_mvn/MvArithOp.cc
/// @brief MvArithOp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "MvArithOp.h"
#include "ym_mvn/MvMgr.h"


BEGIN_NAMESPACE_YM_MVN

//////////////////////////////////////////////////////////////////////
// クラス MvAdd
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] module 親のモジュール
// @param[in] bit_width1 入力1のビット幅
// @param[in] bit_width2 入力2のビット幅
// @param[in] bit_width3 出力のビット幅
MvAdd::MvAdd(MvModule* module,
	     ymuint bit_width1,
	     ymuint bit_width2,
	     ymuint bit_width3) :
  MvBinaryOp(module, bit_width1, bit_width2, bit_width3)
{
}

// @brief デストラクタ
MvAdd::~MvAdd()
{
}

// @brief ノードの種類を得る．
MvNode::tType
MvAdd::type() const
{
  return kAdd;
}

// @brief add ノードを生成する．
// @param[in] module ノードが属するモジュール
// @param[in] bit_width1 入力1のビット幅
// @param[in] bit_width2 入力2のビット幅
// @param[in] bit_width3 出力のビット幅
// @return 生成したノードを返す．
MvNode*
MvMgr::new_add(MvModule* module,
	       ymuint bit_width1,
	       ymuint bit_width2,
	       ymuint bit_width3)
{
  MvNode* node = new MvAdd(module, bit_width1, bit_width2, bit_width3);
  reg_node(node);

  assert_cond( node->input_num() == 2, __FILE__, __LINE__);
  assert_cond( node->output_num() == 1, __FILE__, __LINE__);

  return node;
}


//////////////////////////////////////////////////////////////////////
// クラス MvSub
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] module 親のモジュール
// @param[in] bit_width1 入力1のビット幅
// @param[in] bit_width2 入力2のビット幅
// @param[in] bit_width3 出力のビット幅
MvSub::MvSub(MvModule* module,
	     ymuint bit_width1,
	     ymuint bit_width2,
	     ymuint bit_width3) :
  MvBinaryOp(module, bit_width1, bit_width2, bit_width3)
{
}

// @brief デストラクタ
MvSub::~MvSub()
{
}

// @brief ノードの種類を得る．
MvNode::tType
MvSub::type() const
{
  return kSub;
}

// @brief sub ノードを生成する．
// @param[in] module ノードが属するモジュール
// @param[in] bit_width1 入力1のビット幅
// @param[in] bit_width2 入力2のビット幅
// @param[in] bit_width3 出力のビット幅
// @return 生成したノードを返す．
MvNode*
MvMgr::new_sub(MvModule* module,
	       ymuint bit_width1,
	       ymuint bit_width2,
	       ymuint bit_width3)
{
  MvNode* node = new MvSub(module, bit_width1, bit_width2, bit_width3);
  reg_node(node);

  assert_cond( node->input_num() == 2, __FILE__, __LINE__);
  assert_cond( node->output_num() == 1, __FILE__, __LINE__);

  return node;
}


//////////////////////////////////////////////////////////////////////
// クラス MvMult
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] module 親のモジュール
// @param[in] bit_width1 入力1のビット幅
// @param[in] bit_width2 入力2のビット幅
// @param[in] bit_width3 出力のビット幅
MvMult::MvMult(MvModule* module,
	       ymuint bit_width1,
	       ymuint bit_width2,
	       ymuint bit_width3) :
  MvBinaryOp(module, bit_width1, bit_width2, bit_width3)
{
}

// @brief デストラクタ
MvMult::~MvMult()
{
}

// @brief ノードの種類を得る．
MvNode::tType
MvMult::type() const
{
  return kMult;
}

// @brief mult ノードを生成する．
// @param[in] module ノードが属するモジュール
// @param[in] bit_width1 入力1のビット幅
// @param[in] bit_width2 入力2のビット幅
// @param[in] bit_width3 出力のビット幅
// @return 生成したノードを返す．
MvNode*
MvMgr::new_mult(MvModule* module,
		ymuint bit_width1,
		ymuint bit_width2,
		ymuint bit_width3)
{
  MvNode* node = new MvMult(module, bit_width1, bit_width2, bit_width3);
  reg_node(node);

  assert_cond( node->input_num() == 2, __FILE__, __LINE__);
  assert_cond( node->output_num() == 1, __FILE__, __LINE__);

  return node;
}


//////////////////////////////////////////////////////////////////////
// クラス MvDiv
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] module 親のモジュール
// @param[in] bit_width1 入力1のビット幅
// @param[in] bit_width2 入力2のビット幅
// @param[in] bit_width3 出力のビット幅
MvDiv::MvDiv(MvModule* module,
	     ymuint bit_width1,
	     ymuint bit_width2,
	     ymuint bit_width3) :
  MvBinaryOp(module, bit_width1, bit_width2, bit_width3)
{
}

// @brief デストラクタ
MvDiv::~MvDiv()
{
}

// @brief ノードの種類を得る．
MvNode::tType
MvDiv::type() const
{
  return kDiv;
}

// @brief div ノードを生成する．
// @param[in] module ノードが属するモジュール
// @param[in] bit_width1 入力1のビット幅
// @param[in] bit_width2 入力2のビット幅
// @param[in] bit_width3 出力のビット幅
// @return 生成したノードを返す．
MvNode*
MvMgr::new_div(MvModule* module,
	       ymuint bit_width1,
	       ymuint bit_width2,
	       ymuint bit_width3)
{
  MvNode* node = new MvDiv(module, bit_width1, bit_width2, bit_width3);
  reg_node(node);

  assert_cond( node->input_num() == 2, __FILE__, __LINE__);
  assert_cond( node->output_num() == 1, __FILE__, __LINE__);

  return node;
}


//////////////////////////////////////////////////////////////////////
// クラス MvMod
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] module 親のモジュール
// @param[in] bit_width1 入力1のビット幅
// @param[in] bit_width2 入力2のビット幅
// @param[in] bit_width3 出力のビット幅
MvMod::MvMod(MvModule* module,
	     ymuint bit_width1,
	     ymuint bit_width2,
	     ymuint bit_width3) :
  MvBinaryOp(module, bit_width1, bit_width2, bit_width3)
{
}

// @brief デストラクタ
MvMod::~MvMod()
{
}

// @brief ノードの種類を得る．
MvNode::tType
MvMod::type() const
{
  return kMod;
}

// @brief modulo ノードを生成する．
// @param[in] module ノードが属するモジュール
// @param[in] bit_width1 入力1のビット幅
// @param[in] bit_width2 入力2のビット幅
// @param[in] bit_width3 出力のビット幅
// @return 生成したノードを返す．
MvNode*
MvMgr::new_mod(MvModule* module,
	       ymuint bit_width1,
	       ymuint bit_width2,
	       ymuint bit_width3)
{
  MvNode* node = new MvMod(module, bit_width1, bit_width2, bit_width3);
  reg_node(node);

  assert_cond( node->input_num() == 2, __FILE__, __LINE__);
  assert_cond( node->output_num() == 1, __FILE__, __LINE__);

  return node;
}


//////////////////////////////////////////////////////////////////////
// クラス MvPow
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] module 親のモジュール
// @param[in] bit_width1 入力1のビット幅
// @param[in] bit_width2 入力2のビット幅
// @param[in] bit_width3 出力のビット幅
MvPow::MvPow(MvModule* module,
	     ymuint bit_width1,
	     ymuint bit_width2,
	     ymuint bit_width3) :
  MvBinaryOp(module, bit_width1, bit_width2, bit_width3)
{
}

// @brief デストラクタ
MvPow::~MvPow()
{
}

// @brief ノードの種類を得る．
MvNode::tType
MvPow::type() const
{
  return kPow;
}

// @brief power ノードを生成する．
// @param[in] module ノードが属するモジュール
// @param[in] bit_width1 入力1のビット幅
// @param[in] bit_width2 入力2のビット幅
// @param[in] bit_width3 出力のビット幅
// @return 生成したノードを返す．
MvNode*
MvMgr::new_pow(MvModule* module,
	       ymuint bit_width1,
	       ymuint bit_width2,
	       ymuint bit_width3)
{
  MvNode* node = new MvPow(module, bit_width1, bit_width2, bit_width3);
  reg_node(node);

  assert_cond( node->input_num() == 2, __FILE__, __LINE__);
  assert_cond( node->output_num() == 1, __FILE__, __LINE__);

  return node;
}


//////////////////////////////////////////////////////////////////////
// クラス MvSll
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] module 親のモジュール
// @param[in] bit_width1 入力1のビット幅
// @param[in] bit_width2 入力2のビット幅
// @param[in] bit_width3 出力のビット幅
MvSll::MvSll(MvModule* module,
	     ymuint bit_width1,
	     ymuint bit_width2,
	     ymuint bit_width3) :
  MvBinaryOp(module, bit_width1, bit_width2, bit_width3)
{
}

// @brief デストラクタ
MvSll::~MvSll()
{
}

// @brief ノードの種類を得る．
MvNode::tType
MvSll::type() const
{
  return kSll;
}

// @brief shift left logical ノードを生成する．
// @param[in] module ノードが属するモジュール
// @param[in] bit_width1 入力データのビット幅
// @param[in] bit_width2 シフト量のビット幅
// @param[in] bit_width3 出力のビット幅
// @return 生成したノードを返す．
MvNode*
MvMgr::new_sll(MvModule* module,
	       ymuint bit_width1,
	       ymuint bit_width2,
	       ymuint bit_width3)
{
  MvNode* node = new MvSll(module, bit_width1, bit_width2, bit_width3);
  reg_node(node);

  assert_cond( node->input_num() == 2, __FILE__, __LINE__);
  assert_cond( node->output_num() == 1, __FILE__, __LINE__);

  return node;
}


//////////////////////////////////////////////////////////////////////
// クラス MvSrl
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] module 親のモジュール
// @param[in] bit_width1 入力1のビット幅
// @param[in] bit_width2 入力2のビット幅
// @param[in] bit_width3 出力のビット幅
MvSrl::MvSrl(MvModule* module,
	     ymuint bit_width1,
	     ymuint bit_width2,
	     ymuint bit_width3) :
  MvBinaryOp(module, bit_width1, bit_width2, bit_width3)
{
}

// @brief デストラクタ
MvSrl::~MvSrl()
{
}

// @brief ノードの種類を得る．
MvNode::tType
MvSrl::type() const
{
  return kSrl;
}

// @brief shift right logical ノードを生成する．
// @param[in] module ノードが属するモジュール
// @param[in] bit_width1 入力データのビット幅
// @param[in] bit_width2 シフト量のビット幅
// @param[in] bit_width3 出力のビット幅
// @return 生成したノードを返す．
MvNode*
MvMgr::new_srl(MvModule* module,
	       ymuint bit_width1,
	       ymuint bit_width2,
	       ymuint bit_width3)
{
  MvNode* node = new MvSrl(module, bit_width1, bit_width2, bit_width3);
  reg_node(node);

  assert_cond( node->input_num() == 2, __FILE__, __LINE__);
  assert_cond( node->output_num() == 1, __FILE__, __LINE__);

  return node;
}


//////////////////////////////////////////////////////////////////////
// クラス MvSla
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] module 親のモジュール
// @param[in] bit_width1 入力1のビット幅
// @param[in] bit_width2 入力2のビット幅
// @param[in] bit_width3 出力のビット幅
MvSla::MvSla(MvModule* module,
	     ymuint bit_width1,
	     ymuint bit_width2,
	     ymuint bit_width3) :
  MvBinaryOp(module, bit_width1, bit_width2, bit_width3)
{
}

// @brief デストラクタ
MvSla::~MvSla()
{
}

// @brief ノードの種類を得る．
MvNode::tType
MvSla::type() const
{
  return kSla;
}

// @brief shift left arithmetic ノードを生成する．
// @param[in] module ノードが属するモジュール
// @param[in] bit_width1 入力データのビット幅
// @param[in] bit_width2 シフト量のビット幅
// @param[in] bit_width3 出力のビット幅
// @return 生成したノードを返す．
MvNode*
MvMgr::new_sla(MvModule* module,
	       ymuint bit_width1,
	       ymuint bit_width2,
	       ymuint bit_width3)
{
  MvNode* node = new MvSla(module, bit_width1, bit_width2, bit_width3);
  reg_node(node);

  assert_cond( node->input_num() == 2, __FILE__, __LINE__);
  assert_cond( node->output_num() == 1, __FILE__, __LINE__);

  return node;
}


//////////////////////////////////////////////////////////////////////
// クラス MvSra
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] module 親のモジュール
// @param[in] bit_width1 入力1のビット幅
// @param[in] bit_width2 入力2のビット幅
// @param[in] bit_width3 出力のビット幅
MvSra::MvSra(MvModule* module,
	     ymuint bit_width1,
	     ymuint bit_width2,
	     ymuint bit_width3) :
  MvBinaryOp(module, bit_width1, bit_width2, bit_width3)
{
}

// @brief デストラクタ
MvSra::~MvSra()
{
}

// @brief ノードの種類を得る．
MvNode::tType
MvSra::type() const
{
  return kSra;
}

// @brief shift right arithmetic ノードを生成する．
// @param[in] module ノードが属するモジュール
// @param[in] bit_width1 入力データのビット幅
// @param[in] bit_width2 シフト量のビット幅
// @param[in] bit_width3 出力のビット幅
// @return 生成したノードを返す．
MvNode*
MvMgr::new_sra(MvModule* module,
	       ymuint bit_width1,
	       ymuint bit_width2,
	       ymuint bit_width3)
{
  MvNode* node = new MvSra(module, bit_width1, bit_width2, bit_width3);
  reg_node(node);

  assert_cond( node->input_num() == 2, __FILE__, __LINE__);
  assert_cond( node->output_num() == 1, __FILE__, __LINE__);

  return node;
}

END_NAMESPACE_YM_MVN
