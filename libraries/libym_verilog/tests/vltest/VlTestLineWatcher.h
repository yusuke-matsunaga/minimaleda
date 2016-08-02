#ifndef VLTESTLINEWATCHER_H
#define VLTESTLINEWATCHER_H

/// @file libym_verilog/tests/vltest/VlTestLineWatcher.h
/// @brief VlTestLineWatcher のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: VlTestLineWatcher.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2009 Yusuke Matsunaga
/// All rights reserved.


#include <ym_verilog/VlLineWatcher.h>


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class VlTestLineWatcher VlTestLineWatcher.h
/// @brief vltest 用の行番号ウォッチャー
//////////////////////////////////////////////////////////////////////
class VlTestLineWatcher :
  public VlLineWatcher
{
public:
  
  // コンストラクタ
  VlTestLineWatcher(int line_unit);

  // デストラクタ
  virtual
  ~VlTestLineWatcher();

  // 改行ごとの処理関数
  virtual
  void
  event_proc(int line);

  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 行単位
  int mLineUnit;

};

END_NAMESPACE_YM_VERILOG

#endif // VLTESTLINEWATCHER
