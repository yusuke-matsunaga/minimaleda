#ifndef LIBYM_TCLPP_EXAMPLES_OPTTESTCMD_H
#define LIBYM_TCLPP_EXAMPLES_OPTTESTCMD_H

/// @file OptTestCmd.h
/// @brief OptTestCmd のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: OptTestCmd.h 1417 2008-05-06 18:12:40Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.

// コマンドの製作例


#include "ym_tclpp/TclCmd.h"


BEGIN_NAMESPACE_YM_TCLPP

//////////////////////////////////////////////////////////////////////
// オプション解析のテスト用コマンド
//////////////////////////////////////////////////////////////////////
class OptTestCmd :
  public TclCmd
{
public:

  /// @brief コンストラクタ
  OptTestCmd();

  /// @brief デストラクタ
  virtual
  ~OptTestCmd();

  
protected:
  
  // コマンド処理関数
  virtual
  int
  cmd_proc(TclObjVector& objv);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  TclPopt* mOptSwitch1;
  
  TclPoptStr* mOptStr1;

  TclPoptBool* mOptBool1;
  
  TclPoptInt* mOptInt1;

  TclPoptDouble* mOptDouble1;

  TclPopt* mOptA;

  TclPopt* mOptB;

  TclPopt* mOptC;
  
};

END_NAMESPACE_YM_TCLPP

#endif // LIBYM_TCLPP_EXAMPLES_OPTTESTCMD_H
