#ifndef MAGUS_LUTMAP_AREAMAPCMD_H
#define MAGUS_LUTMAP_AREAMAPCMD_H

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
/// @class AreaMapCmd LutmapCmd "LutmapCmd.h"
/// @brief area map コマンド
//////////////////////////////////////////////////////////////////////
class AreaMapCmd :
  public LutmapCmd
{
public:

  /// @brief コンストラクタ
  AreaMapCmd(MagMgr* mgr,
	     LutmapData* data);

  /// @brief デストラクタ
  virtual
  ~AreaMapCmd();


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

  // resub オプションの解析用オブジェクト
  TclPopt* mPoptResub;

  // verbose オプションの解析用オブジェクト
  TclPopt* mPoptVerbose;

};

END_NAMESPACE_MAGUS

#endif // MAGUS_LUTMAP_AREAMAPCMD_H
