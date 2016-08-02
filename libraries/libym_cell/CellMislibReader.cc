
/// @file libym_cell/CellMislibReader.cc
/// @brief CellMislibReader の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: CellMislibReader.cc 1978 2009-02-06 12:29:16Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_cell/CellMislibReader.h"
#include "ym_cell/Cell.h"
#include "ym_cell/CellBus.h"
#include "ym_cell/CellBundle.h"
#include "ym_cell/CellTiming.h"
#include "ym_cell/Msg.h"


BEGIN_NAMESPACE_YM_CELL

// @brief コンストラクタ
CellMislibReader::CellMislibReader()
{
}

// @brief デストラクタ
CellMislibReader::~CellMislibReader()
{
}

// @brief mislib 形式のファイルを読み込む
// @param[in] in 入力元のストリーム
// @param[in] filename ファイル名
// @param[out] library 読み込んだ内容を設定するネットワーク
// @retval true 正常に読み込めた
// @retval false 読み込み中にエラーが起こった．
bool
CellMislibReader::read(istream& in,
		       const string& filename,
		       CellLibrary& library)
{
}

// @brief メッセージハンドラを付加する．
void
CellMislibReader::add_msg_handler(MsgHandler* msg_handler)
{
  MsgMgr::the_mgr().reg_handler(msg_handler);
}

END_NAMESPACE_YM_CELL
