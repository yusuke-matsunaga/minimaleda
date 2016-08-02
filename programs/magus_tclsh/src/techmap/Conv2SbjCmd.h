#ifndef TECHMAP_CONV2SBJCMD_H
#define TECHMAP_CONV2SBJCMD_H

/// @file magus/techmap/Conv2SbjCmd.h
/// @brief Conv2SbjCmd のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: Conv2SbjCmd.h 2274 2009-06-10 07:45:29Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "TechmapCmd.h"


BEGIN_NAMESPACE_MAGUS_TECHMAP

//////////////////////////////////////////////////////////////////////
/// @class Conv2SbjCmd Conv2SbjCmd "Conv2SbjCmd.h"
/// @brief BNetwork から SbjGraph に変換するコマンド
//////////////////////////////////////////////////////////////////////
class Conv2SbjCmd :
  public TechmapCmd
{
public:

  /// @brief コンストラクタ
  Conv2SbjCmd(MagMgr* mgr,
	      TechmapData* data);

  /// @brief デストラクタ
  virtual
  ~Conv2SbjCmd();


protected:

  /// @brief コマンドを実行する仮想関数
  virtual
  int
  cmd_proc(TclObjVector& objv);

};

END_NAMESPACE_MAGUS_TECHMAP

#endif // MAGUS_TECHMAP_CONV2SBJCMD_H
