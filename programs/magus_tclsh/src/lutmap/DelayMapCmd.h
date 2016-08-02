#ifndef MAGUS_LUTMAP_DELAYMAPCMD_H
#define MAGUS_LUTMAP_DELAYMAPCMD_H

/// @file magus/lutmap/LutmapCmd.h
/// @brief LutmapCmd のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: LutmapCmd.h 2274 2009-06-10 07:45:29Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "LutmapCmd.h"


BEGIN_NAMESPACE_MAGUS

//////////////////////////////////////////////////////////////////////
/// @class DelayMapCmd LutmapCmd "LutmapCmd.h"
/// @brief delay map コマンド
//////////////////////////////////////////////////////////////////////
class DelayMapCmd :
  public LutmapCmd
{
public:

  /// @brief コンストラクタ
  DelayMapCmd(MagMgr* mgr,
	      LutmapData* data);

  /// @brief デストラクタ
  virtual
  ~DelayMapCmd();


protected:

  /// @brief コマンドを実行する仮想関数
  virtual
  int
  cmd_proc(TclObjVector& objv);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // method オプションの解析用オブジェクト
  TclPoptStr* mPoptMethod;

  // slack オプションの解析用オブジェクト
  TclPoptInt* mPoptSlack;

  // resub オプションの解析用オブジェクト
  TclPopt* mPoptResub;

  // verbose オプションの解析用オブジェクト
  TclPopt* mPoptVerbose;

};

END_NAMESPACE_MAGUS

#endif // MAGUS_LUTMAP_DELAYMAPCMD_H
