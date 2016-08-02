
/// @file libym_aig/LrMgr_init_table.cc
/// @brief LrMgr::init_table() の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
/// 
/// $Id: LrMgr_init_table.cc 1594 2008-07-18 10:26:12Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "LrMgr.h"


BEGIN_NAMESPACE_YM_AIG

// @brief 置き換えテーブルを初期化する．
void
LrMgr::init_table()
{
#include "lrtable.dat"
}

END_NAMESPACE_YM_AIG
