
/// @file libym_tgnet/TgBlifHandler.cc
/// @brief TgBlifHandler の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: TgBlifHandler.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.



#include "TgBlifHandler.h"
#include "ym_tgnet/TgNetwork.h"
#include "ym_tgnet/TgNode.h"


BEGIN_NAMESPACE_YM_TGNET

// @brief コンストラクタ
TgBlifHandler::TgBlifHandler()
{
}

// @brief デストラクタ
TgBlifHandler::~TgBlifHandler()
{
}

// @brief ネットワークをセットする．
void
TgBlifHandler::set_network(TgNetwork& network)
{
  mNetwork = &network;
}

// @brief 初期化
bool
TgBlifHandler::init()
{
  mNetwork->clear();
  mConList.clear();
  return true;
}
    
// @brief .model 文の処理
// @param[in] loc1 .model の位置情報
// @param[in] loc2 文字列の位置情報
// @param[in] name 文字列
// @retval true 処理が成功した．
// @retval false エラーが起こった．
bool
TgBlifHandler::model(const FileRegion& loc1,
		     const FileRegion& loc2,
		     const char* name)
{
  return true;
}

// @brief .input 文の読み込み
bool
TgBlifHandler::inputs_elem(ymuint32 name_id)
{
  const char* name = id2str(name_id);
  TgNode* node = mNetwork->find_node(name, true);
  mNetwork->set_to_input(node);
  return true;
}
  
// @brief .output 文の読み込み
bool
TgBlifHandler::outputs_elem(ymuint32 name_id)
{
  TgNode* onode = mNetwork->new_node();
  mNetwork->set_to_output(onode);
  const char* name = id2str(name_id);
  TgNode* node = mNetwork->find_node(name, true);
  connect(node, onode, 0);
  return true;
}

// @brief .names 文の処理
bool
TgBlifHandler::names(const vector<ymuint32>& name_id_array,
		     ymuint32 nc,
		     const char* cover_pat,
		     char opat)
{
  ymuint32 n = name_id_array.size();
  
  mCurFanins.clear();
  for (ymuint32 i = 0; i < n; ++ i) {
    ymuint32 id = name_id_array[i];
    const char* name = id2str(id);
    TgNode* node = mNetwork->find_node(name, true);
    mCurFanins.push_back(node);
  }
  
  ymuint32 ni = n - 1;
  TgNode* node = mCurFanins[ni];
  mChd2.reserve(ni);

  LogExpr expr;
  mChd1.clear();
  if ( opat == 1 ) {
    for (ymuint32 c = 0; c < nc; ++ c) {
      mChd2.clear();
      for (ymuint32 i = 0; i < ni; ++ i) {
	char v = cover_pat[c * ni + i];
	if ( v == '0' ) {
	  mChd2.push_back(LogExpr::make_negaliteral(i));
	}
	else if ( v == '1' ) {
	  mChd2.push_back(LogExpr::make_posiliteral(i));
	}
      }
      mChd1.push_back(LogExpr::make_and(mChd2));
    }
    if ( mChd1.empty() ) {
      expr = LogExpr::make_one();
    }
    else {
      expr = LogExpr::make_or(mChd1);
    }
  }
  else {
    for (ymuint32 c = 0; c < nc; ++ c) {
      mChd2.clear();
      for (ymuint32 i = 0; i < ni; ++ i) {
	char v = cover_pat[c * ni + i];
	if ( v == '0' ) {
	  mChd2.push_back(LogExpr::make_posiliteral(i));
	}
	else if ( v == '1' ) {
	  mChd2.push_back(LogExpr::make_negaliteral(i));
	}
      }
      mChd1.push_back(LogExpr::make_or(mChd2));
    }
    if ( mChd1.empty() ) {
      expr = LogExpr::make_zero();
    }
    else {
      expr = LogExpr::make_and(mChd1);
    }
  }
  TgGateTemplate id = mNetwork->reg_lexp(expr);

  mNetwork->set_to_logic(node, id);
  if ( ni != node->ni() ) {
    // .names の後の要素数とパタンの列数が異なる．
    ostringstream buf;
    buf << node->name() << ": illegal cover" << endl;
    msg_mgr().put_msg(__FILE__, __LINE__,
		      id2loc(name_id_array[ni]),
		      kMsgError,
		      "TG_BLIF_005",
		      buf.str());
    return false;
  }
  for (ymuint32 i = 0; i < ni; ++ i) {
    TgNode* inode = mCurFanins[i];
    connect(inode, node, i);
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
TgBlifHandler::gate_begin(const FileRegion& loc1,
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
TgBlifHandler::gate_assign(const FileRegion& loc1,
			   const char* f_name,
			   const FileRegion& loc2,
			   const char* a_name)
{
  return true;
}

// @brief .gate 文の終了
bool
TgBlifHandler::gate_end()
{
  return true;
}
  
// @brief .latch 文の読み込み
bool
TgBlifHandler::latch(ymuint32 name1_id,
		     ymuint32 name2_id,
		     const FileRegion& loc4,
		     char rval)
{
  const char* name2 = id2str(name2_id);
  TgNode* ffout = mNetwork->find_node(name2, true);
  TgNode* ffin = mNetwork->new_node();
  mNetwork->set_to_ff(ffin, ffout);
  const char* name1 = id2str(name1_id);
  TgNode* inode = mNetwork->find_node(name1, true);
  connect(inode, ffin, 0);

  // rval は使っていない．
  return true;
}

// @brief .end 文の処理
bool
TgBlifHandler::end(const FileRegion& loc)
{
  for (vector<Connection>::iterator p = mConList.begin();
       p != mConList.end(); ++ p) {
    const Connection& con = *p;
    mNetwork->connect(con.mFrom, con.mTo, con.mPos);
  }
  mConList.clear();
  
  size_t n = mNetwork->node_num();
  for (size_t i = 0; i < n; ++ i) {
    const TgNode* node = mNetwork->node(i);
    if ( node->is_undef() ) {
      // node は定義されていない
      ostringstream buf;
      buf << node->name() << ": undefined";
      msg_mgr().put_msg(__FILE__, __LINE__,
			loc,
			kMsgError,
			"TG_BLIF_03",
			buf.str());
      return false;
    }
  }

  return true;
}
  
// @brief 終了処理
void
TgBlifHandler::normal_exit()
{
  mNetwork->wrap_up();
}

// @brief エラー終了時の処理
void
TgBlifHandler::error_exit()
{
  mNetwork->clear();
}

// 可能なら接続する．
// できれなれば mConList に積む
void
TgBlifHandler::connect(TgNode* from,
		       TgNode* to,
		       size_t pos)
{
  if ( from->is_undef() ) {
    mConList.push_back(Connection(from, to, pos));
  }
  else {
    mNetwork->connect(from, to, pos);
  }
}

END_NAMESPACE_YM_TGNET
