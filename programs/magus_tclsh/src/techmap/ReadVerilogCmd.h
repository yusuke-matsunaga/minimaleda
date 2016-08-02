#ifndef TECHMAP_READVERILOGCMD_H
#define TECHMAP_READVERILOGCMD_H

/// @file magus/techmap/ReadVerilogCmd.h
/// @brief ReadVerilogCmd のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: ReadVerilogCmd.h 2274 2009-06-10 07:45:29Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "TechmapCmd.h"


BEGIN_NAMESPACE_MAGUS_TECHMAP

//////////////////////////////////////////////////////////////////////
/// @class ReadVerilogCmd ReadVerilogCmd "ReadVerilogCmd.h"
/// @brief verilog ファイルを読み込むコマンド
//////////////////////////////////////////////////////////////////////
class ReadVerilogCmd :
  public TechmapCmd
{
public:

  /// @brief コンストラクタ
  ReadVerilogCmd(MagMgr* mgr,
		 TechmapData* data);

  /// @brief デストラクタ
  virtual
  ~ReadVerilogCmd();


protected:

  /// @brief コマンドを実行する仮想関数
  virtual
  int
  cmd_proc(TclObjVector& objv);

};

END_NAMESPACE_MAGUS_TECHMAP

#endif // TECHMAP_READVERILOGCMD_H
