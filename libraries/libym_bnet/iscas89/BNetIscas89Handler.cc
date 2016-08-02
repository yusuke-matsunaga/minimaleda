
/// @file libym_bnetiscas89reader/BNetIscas89Handler.cc
/// @brief BNetIscas89Handler の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: BNetIscas89Reader.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "BNetIscas89Handler.h"
#include "ym_bnet/BNetwork.h"
#include "ym_bnet/BNetManip.h"


BEGIN_NAMESPACE_YM_BNET

// @brief コンストラクタ
BNetIscas89Handler::BNetIscas89Handler() :
  mNetwork(NULL),
  mManip(NULL)
{
}

// @brief デストラクタ
BNetIscas89Handler::~BNetIscas89Handler()
{
  delete mManip;
}

// @brief 読み込む対象のネットワークを設定する．
void
BNetIscas89Handler::set_network(BNetwork* network)
{
  mNetwork = network;
  mManip = new BNetManip(network);
}

// @brief 初期化
bool
BNetIscas89Handler::init()
{
  mNetwork->clear();
  return true;
}

// @brief INPUT 文を読み込む．
// @param[in] loc ファイル位置
// @param[in] name_id 入力ピン名の ID 番号
// @retval true 処理が成功した．
// @retval false エラーが起こった．
bool
BNetIscas89Handler::read_input(const FileRegion& loc,
			       ymuint32 name_id)
{
  const char* name = id2str(name_id);
  BNode* node = mManip->new_input(name);
  resize(name_id);
  NodeInfo& node_info = mNodeArray[name_id];
  node_info.mNode = node;
  return true;
}

// @brief OUTPUT 文を読み込む．
// @param[in] loc ファイル位置
// @param[in] name_id 出力ピン名の ID 番号
// @retval true 処理が成功した．
// @retval false エラーが起こった．
bool
BNetIscas89Handler::read_output(const FileRegion& loc,
				ymuint32 name_id)
{
  const char* name = id2str(name_id);
  mManip->new_output(name);
  return true;
}

// @brief ゲート文を読み込む．
// @param[in] loc ファイル位置
// @param[in] type ゲートの型
// @param[in] oname_id 出力名の ID 番号
// @param[in] iname_list 入力名のリスト
// @retval true 処理が成功した．
// @retval false エラーが起こった．
bool
BNetIscas89Handler::read_gate(const FileRegion& loc,
			      tIscas89GateType type,
			      ymuint32 oname_id,
			      const vector<ymuint32>& iname_list)
{
  mCurFanins.clear();
  for (vector<ymuint32>::const_iterator p = iname_list.begin();
       p != iname_list.end(); ++ p) {
    BNode* inode = get_node(*p);
    mCurFanins.push_back(inode);
  }

  bool status = true;
  BNode* onode = get_node(oname_id);
  switch ( type ) {
  case kGt89BUFF:
    assert_cond(mCurFanins.size() == 1, __FILE__, __LINE__);
    status = mManip->change_to_buffer(onode, mCurFanins[0]);
    break;

  case kGt89NOT:
    assert_cond(mCurFanins.size() == 1, __FILE__, __LINE__);
    status = mManip->change_to_inverter(onode, mCurFanins[0]);
    break;

  case kGt89AND:
    assert_cond(mCurFanins.size() > 1, __FILE__, __LINE__);
    status = mManip->change_to_and(onode, mCurFanins);
    break;

  case kGt89NAND:
    assert_cond(mCurFanins.size() > 1, __FILE__, __LINE__);
    status = mManip->change_to_nand(onode, mCurFanins);
    break;

  case kGt89OR:
    assert_cond(mCurFanins.size() > 1, __FILE__, __LINE__);
    status = mManip->change_to_or(onode, mCurFanins);
    break;

  case kGt89NOR:
    assert_cond(mCurFanins.size() > 1, __FILE__, __LINE__);
    status = mManip->change_to_nor(onode, mCurFanins);
    break;

  case kGt89XOR:
    assert_cond(mCurFanins.size() > 1, __FILE__, __LINE__);
    status = mManip->change_to_xor(onode, mCurFanins);
    break;

  case kGt89XNOR:
    assert_cond(mCurFanins.size() > 1, __FILE__, __LINE__);
    status = mManip->change_to_xnor(onode, mCurFanins);
    break;

  case kGt89DFF:
    assert_cond(mCurFanins.size() == 1, __FILE__, __LINE__);
    status = mManip->change_latch(onode, mCurFanins[0], 2);
  }
  return status;
}

// @brief 通常終了時の処理
void
BNetIscas89Handler::normal_exit()
{
  for (BNodeList::const_iterator p = mNetwork->outputs_begin();
       p != mNetwork->outputs_end(); ++ p) {
    BNode* onode = *p;
    BNode* node = mNetwork->find_node(onode->name());
    assert_cond(node, __FILE__, __LINE__);
    mManip->change_output(onode, node);
  }
  delete mManip;
  mManip = NULL;
}

// @brief エラー終了時の処理
void
BNetIscas89Handler::error_exit()
{
  mNetwork->clear();
  delete mManip;
  mManip = NULL;
}

// @brief ノードの取得 & 生成
BNode*
BNetIscas89Handler::get_node(ymuint32 id)
{
  resize(id);
  NodeInfo& node_info = mNodeArray[id];
  if ( node_info.mNode ) {
    return node_info.mNode;
  }
  BNode* node = mManip->new_logic(id2str(id));
  node_info.mNode = node;
  return node;
}

void
BNetIscas89Handler::resize(ymuint32 id)
{
  size_t n = mNodeArray.size();
  while ( n <= id ) {
    mNodeArray.push_back(NodeInfo(n));
    ++ n;
  }
}

END_NAMESPACE_YM_BNET
