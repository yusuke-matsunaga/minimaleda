
/// @file libym_mislib/MislibHandler.cc
/// @brief MislibHandler の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: MislibHandler.cc 1978 2009-02-06 12:29:16Z matsunaga $
///
/// Copyright (C) 2005-2006 Yusuke Matsunaga
/// All rights reserved.


#if HAVE_CONFIG_H
#include <ymconfig.h>
#endif

#include <ym_mislib/MislibHandler.h>
#include "MislibParserImpl.h"


BEGIN_NAMESPACE_YM_MISLIB

//////////////////////////////////////////////////////////////////////
// MislibPin
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
MislibPin::MislibPin()
{
}

// @brief デストラクタ
MislibPin::~MislibPin()
{
}


//////////////////////////////////////////////////////////////////////
// MislibHandler
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
MislibHandler::MislibHandler()
{
}

// @brief デストラクタ
MislibHandler::~MislibHandler()
{
}

// @brief 初期化
void
MislibHandler::init()
{
}

// @brief 通常終了時の処理
void
MislibHandler::normal_exit()
{
}

// @brief エラー終了時の処理
void
MislibHandler::error_exit()
{
}

// @brief GATE 文の処理
// @param[in] loc GATE 文の位置情報
// @param[in] name 名前
// @param[in] name_loc 名前の位置情報
// @param[in] area 面積
// @param[in] area_loc 面積の位置情報
// @param[in] oname 出力ピン名
// @param[in] oname_loc 出力ピン名の位置情報
// @param[in] expr 論理式
// @param[in] expr_loc 論理式の位置情報
// @param[in] ipin_list 入力ピンの情報の配列
void
MislibHandler::gate(const FileRegion& loc,
		    ShString name,
		    const FileRegion& name_loc,
		    double area,
		    const FileRegion& area_loc,
		    ShString oname,
		    const FileRegion& oname_loc,
		    const LogExpr& expr,
		    const FileRegion& expr_loc,
		    const vector<MislibPin>& ipin_list)
{
}
  
// @brief メッセージマネージャの取得
MsgMgr&
MislibHandler::msg_mgr()
{
  return mParser->msg_mgr();
}

END_NAMESPACE_YM_MISLIB
