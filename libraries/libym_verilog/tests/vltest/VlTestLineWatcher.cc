
/// @file libym_verilog/tests/vltest/VlTestLineWatcher.cc
/// @brief VlTestLineWatcher の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: VlTestLineWatcher.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2009 Yusuke Matsunaga
/// All rights reserved.


#if HAVE_CONFIG_H
#include <ymconfig.h>
#endif

#include "VlTestLineWatcher.h"


BEGIN_NAMESPACE_YM_VERILOG

// コンストラクタ
VlTestLineWatcher::VlTestLineWatcher(int line_unit) :
  mLineUnit(line_unit)
{
}

// デストラクタ
VlTestLineWatcher::~VlTestLineWatcher()
{
}

// 改行ごとの処理関数
void
VlTestLineWatcher::event_proc(int line)
{
  if ( (line % mLineUnit) == 0 ) {
    cout << "line: " << line << endl;
  }
}

END_NAMESPACE_YM_VERILOG
