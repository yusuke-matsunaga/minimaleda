
/// @file libym_iscas89/Iscas89Handler.cc
/// @brief BlibHandler の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: Iscas89Handler.cc 1978 2009-02-06 12:29:16Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_iscas89/Iscas89Handler.h"
#include "Iscas89ParserImpl.h"


BEGIN_NAMESPACE_YM_ISCAS89

//////////////////////////////////////////////////////////////////////
// Iscas89Handler
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
Iscas89Handler::Iscas89Handler()
{
}

// @brief デストラクタ
Iscas89Handler::~Iscas89Handler()
{
}

// @brief 初期化
bool
Iscas89Handler::init()
{
  return true;
}

// @brief INPUT 文を読み込む．
// @param[in] loc ファイル位置
// @param[in] name_id 入力ピン名の ID 番号
// @retval true 処理が成功した．
// @retval false エラーが起こった．
bool
Iscas89Handler::read_input(const FileRegion& loc,
			   ymuint32 name_id)
{
  return true;
}

// @brief OUTPUT 文を読み込む．
// @param[in] loc ファイル位置
// @param[in] name_id 出力ピン名の ID 番号
// @retval true 処理が成功した．
// @retval false エラーが起こった．
bool
Iscas89Handler::read_output(const FileRegion& loc,
			    ymuint32 name_id)
{
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
Iscas89Handler::read_gate(const FileRegion& loc,
			  tIscas89GateType type,
			  ymuint32 oname_id,
			  const vector<ymuint32>& iname_list)
{
  return true;
}

// @brief 通常終了時の処理
void
Iscas89Handler::normal_exit()
{
}

// @brief エラー終了時の処理
void
Iscas89Handler::error_exit()
{
}
  
// @brief メッセージマネージャの取得
MsgMgr&
Iscas89Handler::msg_mgr()
{
  return mParser->msg_mgr();
}
  
// @brief ID 番号から文字列を得る．
const char*
Iscas89Handler::id2str(ymuint32 id) const
{
  return mParser->id2str(id);
}
  
// @brief ID 番号から位置情報を得る．
FileRegion
Iscas89Handler::id2loc(ymuint32 id) const
{
  return mParser->id2loc(id);
}

END_NAMESPACE_YM_ISCAS89
