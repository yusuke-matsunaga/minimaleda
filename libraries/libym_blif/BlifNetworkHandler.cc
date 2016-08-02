
/// @file libym_blif/BlifNetworkHandler.cc
/// @brief BlifNetworkHandler の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: BlifNetworkReader.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "BlifNetworkHandler.h"
#include "ym_blif/BlifNetwork.h"


BEGIN_NAMESPACE_YM_BLIF

// @brief コンストラクタ
BlifNetworkHandler::BlifNetworkHandler() :
  mNetwork(NULL)
{
}

// @brief デストラクタ
BlifNetworkHandler::~BlifNetworkHandler()
{
}

// @brief 読み込む対象のネットワークを設定する．
void
BlifNetworkHandler::set_network(BlifNetwork* network)
{
  mNetwork = network;
}

// @brief 初期化
bool
BlifNetworkHandler::init()
{
  mNetwork->clear();
  return true;
}
  
// @brief .model 文の読み込み
// @param[in] loc 位置情報
// @param[in] name model名
bool
BlifNetworkHandler::model(const FileRegion& loc1,
			  const FileRegion& loc2,
			  const char* name)
{
  mNetwork->mName = name;
  return true;
}
  
// @brief .input 文の読み込み
// @param[in] loc 位置情報
// @param[in] name 入力ノード名
bool
BlifNetworkHandler::inputs_elem(ymuint32 name_id)
{
  BlifNode* node = mNetwork->get_node(name_id);
  if ( !mNetwork->set_input_type(node) ) {
    return false;
  }
  return true;
}
  
// @brief .output 文の読み込み
// @param[in] loc 位置情報
// @param[in] name 出力ノード名
bool
BlifNetworkHandler::outputs_elem(ymuint32 name_id)
{
  BlifNode* node = mNetwork->get_node(name_id);
  mNetwork->mPOArray.push_back(node);
  return true;
}

// @brief .names 文の処理
bool
BlifNetworkHandler::names(const vector<ymuint32>& name_id_array,
			  ymuint32 nc,
			  const char* cover_pat,
			  char opat)
{
  ymuint32 n = name_id_array.size();
  ymuint32 ni = n - 1;
  ymuint32 name_id = name_id_array[ni];
  BlifNode* node = mNetwork->get_node(name_id);
  if ( !mNetwork->set_logic_type(node, ni, nc, cover_pat, opat) ) {
    return false;
  }
  for (size_t i = 0; i < ni; ++ i) {
    ymuint32 id = name_id_array[i];
    BlifNode* inode = mNetwork->get_node(id);
    node->mFanins[i] = inode;
  }
  
  return true;
}

// @brief .gate 文の開始
// @param[in] loc1 .gate の位置情報
// @param[in] loc2 セル名の位置情報
// @param[in] name セル名
// @retval true 処理が成功した．
// @retval false エラーが起こった．
bool
BlifNetworkHandler::gate_begin(const FileRegion& loc1,
			       const FileRegion& loc2,
			       const char* name)
{
  return true;
}

// @brief .gate 文中のピン割り当ての処理
// @param[in] loc1 ピン名の位置情報
// @param[in] f_name ピン名
// @param[in] loc2 ノード名の位置情報
// @param[in] a_name ノード名
// @retval true 処理が成功した．
// @retval false エラーが起こった．
bool
BlifNetworkHandler::gate_assign(const FileRegion& loc1,
				const char* f_name,
				const FileRegion& loc2,
				const char* a_name)
{
  return true;
}

// @brief .gate 文の終了
bool
BlifNetworkHandler::gate_end()
{
  return true;
}
  
// @brief .latch 文の読み込み
bool
BlifNetworkHandler::latch(ymuint32 name1_id,
			  ymuint32 name2_id,
			  const FileRegion& loc4,
			  char rval)
{
  BlifNode* node2 = mNetwork->get_node(name2_id);
  if ( !mNetwork->set_latch_type(node2, rval) ) {
    return false;
  }
  BlifNode* node1 = mNetwork->get_node(name1_id);
  node2->mFanins[0] = node1;
  
  return true;
}
  
// @brief 終了処理
// @param[in] loc 位置情報
bool
BlifNetworkHandler::end(const FileRegion& loc)
{
  // 各ノードの名前の領域を確保する．
  ymuint32 n = mNetwork->max_node_id();
  for (ymuint32 id = 0; id < n; ++ id) {
    BlifNode* node = mNetwork->mNodeArray[id];
    if ( node == NULL ) continue;
    mNetwork->set_node_name(node, id2str(id));
  }
  return true;
}

// @brief 通常終了時の処理
void
BlifNetworkHandler::normal_exit()
{
}

// @brief エラー終了時の処理
void
BlifNetworkHandler::error_exit()
{
  mNetwork->clear();
}

END_NAMESPACE_YM_BLIF
