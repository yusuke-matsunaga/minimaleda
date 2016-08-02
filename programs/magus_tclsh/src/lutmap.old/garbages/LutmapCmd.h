#ifndef MAGUS_LUTMAP_LUTMAPCMD_H
#define MAGUS_LUTMAP_LUTMAPCMD_H

/// @file magus/lutmap/LutmapCmd.h
/// @brief LutmapCmd のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: LutmapCmd.h 1920 2008-12-20 15:52:42Z matsunaga $
///
/// Copyright (C) 2005-2006 Yusuke Matsunaga
/// All rights reserved.


#include <MagCmd.h>
#include "lutmap.h"


BEGIN_NAMESPACE_MAGUS_LUTMAP

//////////////////////////////////////////////////////////////////////
/// マッピングを行うコマンド
//////////////////////////////////////////////////////////////////////
class LutmapCmd :
  public MagCmd
{
public:

  /// @brief コンストラクタ
  LutmapCmd(MagMgr* mgr);
  

protected:
  
  /// @brief コマンドを実行する仮想関数
  virtual
  int
  cmd_proc(TclObjVector& objv);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // dst_network オプション解析用のオブジェクト
  TclPoptStr* mPoptDstNetwork;

  // method オプション解析用のオブジェクト
  TclPoptStr* mPoptMethod;

  // verbose オプション解析用のオブジェクト
  TclPopt* mPoptVerbose;

  // slack オプション解析用のオブジェクト
  TclPoptInt* mPoptSlack;

  // climit オプション解析用のオブジェクト
  TclPoptInt* mPoptClimit;
  
};

END_NAMESPACE_MAGUS_LUTMAP

#endif // MAGUS_LUTMAP_LUTMAPCMD_H
