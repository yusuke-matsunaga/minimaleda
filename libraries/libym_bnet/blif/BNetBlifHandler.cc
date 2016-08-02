
/// @file libym_bnetblifreader/BNetBlifHandler.cc
/// @brief BNetBlifHandler の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: BNetBlifReader.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "BNetBlifHandler.h"
#include "ym_bnet/BNetwork.h"
#include "ym_bnet/BNetManip.h"


BEGIN_NAMESPACE_YM_BNET

// @brief コンストラクタ
BNetBlifHandler::BNetBlifHandler() :
  mNetwork(NULL),
  mManip(NULL)
{
}

// @brief デストラクタ
BNetBlifHandler::~BNetBlifHandler()
{
  delete mManip;
}

// @brief 読み込む対象のネットワークを設定する．
void
BNetBlifHandler::set_network(BNetwork* network)
{
  mNetwork = network;
}

// @brief 初期化
bool
BNetBlifHandler::init()
{
  assert_cond( mManip == NULL, __FILE__, __LINE__);

  mManip = new BNetManip(mNetwork);
  mNetwork->clear();

  mNodeArray.clear();

  return true;
}

// @brief .model 文中の文字列の処理
// @retval true 処理が成功した．
// @retval false エラーが起こった．
bool
BNetBlifHandler::model(const FileRegion& loc1,
		       const FileRegion& loc2,
		       const char* name)
{
  mNetwork->set_model_name(name);
  return true;
}

// @brief .inputs 文中の文字列の処理
// @param[in] name_id 文字列の情報
// @retval true 処理が成功した．
// @retval false エラーが起こった．
bool
BNetBlifHandler::inputs_elem(ymuint32 name_id)
{
  const char* name = id2str(name_id);
  BNode* node = mManip->new_input(name);
  resize(name_id);
  mNodeArray[name_id] = node;
  return true;
}

// @brief .outputs 文中の文字列の処理
// @param[in] name_id 文字列の情報
// @retval true 処理が成功した．
// @retval false エラーが起こった．
bool
BNetBlifHandler::outputs_elem(ymuint32 name_id)
{
  const char* name = id2str(name_id);
  mManip->new_output(name);
  return true;
}

// @brief .names 文の処理
// @param[in] name_id_array 各識別子のID番号の配列
// @param[in] nc キューブ数
// @param[in] cover_pat 入力カバーを表す文字列
// @param[in] opat 出力の極性
// @retval true 処理が成功した．
// @retval false エラーが起こった．
// @note cover_pat は ni 個ごとに1行のパタンを表す．
// 各要素のとりうる値は '0', '1', '-' を表す．
// @note opat は '0' か '1' のどちらか
bool
BNetBlifHandler::names(const vector<ymuint32>& name_id_array,
		       ymuint32 nc,
		       const char* cover_pat,
		       char opat)
{
  size_t ni = name_id_array.size();
  -- ni;
  BNode* node = get_node(name_id_array[ni]);

  LogExpr expr;
  if ( opat == '1' ) {
    if ( ni == 0 ) {
      expr = LogExpr::make_one();
    }
    else {
      vector<LogExpr> or_expr;
      for (ymuint32 c = 0; c < nc; ++ c) {
	vector<LogExpr> and_expr;
	for (ymuint32 i = 0; i < ni; ++ i) {
	  char v = cover_pat[c * ni + i];
	  if ( v == '0' ) {
	    and_expr.push_back(LogExpr::make_negaliteral(i));
	  }
	  else if ( v == '1' ) {
	    and_expr.push_back(LogExpr::make_posiliteral(i));
	  }
	}
	or_expr.push_back(LogExpr::make_and(and_expr));
      }
      expr = LogExpr::make_or(or_expr);
    }
  }
  else {
    if ( ni == 0 ) {
      expr = LogExpr::make_zero();
    }
    else {
      vector<LogExpr> or_expr;
      for (ymuint32 c = 0; c < nc; ++ c) {
	vector<LogExpr> and_expr;
	for (ymuint32 i = 0; i < ni; ++ i) {
	  char v = cover_pat[c * ni + i];
	  if ( v == '0' ) {
	    and_expr.push_back(LogExpr::make_posiliteral(i));
	  }
	  else if ( v == '1' ) {
	    and_expr.push_back(LogExpr::make_negaliteral(i));
	  }
	}
	or_expr.push_back(LogExpr::make_or(and_expr));
      }
      expr = LogExpr::make_and(or_expr);
    }
  }
  BNodeVector fanins;
  for (ymuint i = 0; i < ni; ++ i) {
    BNode* inode = get_node(name_id_array[i]);
    fanins.push_back(inode);
  }
  bool stat = mManip->change_logic(node, expr, fanins);
  return stat;
}

// @brief .gate 文の開始
// @param[in] loc1 .gate の位置情報
// @param[in] loc2 セル名の位置情報
// @param[in] name セル名
// @retval true 処理が成功した．
// @retval false エラーが起こった．
bool
BNetBlifHandler::gate_begin(const FileRegion& loc1,
			    const FileRegion& loc2,
			    const char* name)
{
  // 無視
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
BNetBlifHandler::gate_assign(const FileRegion& loc1,
			     const char* f_name,
			     const FileRegion& loc2,
			     const char* a_name)
{
  // 無視
  return true;
}

// @brief .gate 文の終了
bool
BNetBlifHandler::gate_end()
{
  // 無視
  return true;
}

// @brief .latch 文中の本体の処理
// @param[in] name1_id 最初の文字列
// @param[in] name2_id 次の文字列
// @param[in] loc4 リセット値の位置情報
// @param[in] rval リセット時の値 ('0'/'1') 未定義なら ' '
// @retval true 処理が成功した．
// @retval false エラーが起こった．
bool
BNetBlifHandler::latch(ymuint32 name1_id,
		       ymuint32 name2_id,
		       const FileRegion& loc4,
		       char rval)
{
  BNode* inode = get_node(name1_id);
  BNode* node = get_node(name2_id);
  int rv = 0;
  switch ( rval ) {
  case '0': rv = 0; break;
  case '1': rv = 1; break;
  default:  rv = 2; break;
  }
  return mManip->change_latch(node, inode, rv);
}

// @brief .end 文の処理
// @param[in] loc 位置情報
// @retval true 処理が成功した．
// @retval false エラーが起こった．
bool
BNetBlifHandler::end(const FileRegion& loc)
{
  for (BNodeList::const_iterator p = mNetwork->outputs_begin();
       p != mNetwork->outputs_end(); ++ p) {
    BNode* onode = *p;
    BNode* node = mNetwork->find_node(onode->name());
    assert_cond(node, __FILE__, __LINE__);
    mManip->change_output(onode, node);
  }
  return true;
}

// @brief 通常終了時の処理
void
BNetBlifHandler::normal_exit()
{
  delete mManip;
  mManip = NULL;
}

// @brief エラー終了時の処理
void
BNetBlifHandler::error_exit()
{
  mNetwork->clear();
  delete mManip;
  mManip = NULL;
}

// @brief ノードの取得 & 生成
BNode*
BNetBlifHandler::get_node(ymuint32 id)
{
  resize(id);
  BNode* node = mNodeArray[id];
  if ( node == NULL ) {
    node = mManip->new_logic(id2str(id));
    mNodeArray[id] = node;
  }
  return node;
}

void
BNetBlifHandler::resize(ymuint32 id)
{
  size_t n = mNodeArray.size();
  while ( n <= id ) {
    mNodeArray.push_back(NULL);
    ++ n;
  }
}

END_NAMESPACE_YM_BNET
