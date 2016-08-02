#ifndef TECHMAP_DUMPSBJCMD_H
#define TECHMAP_DUMPSBJCMD_H

/// @file magus/techmap/DumpSbjCmd.h
/// @brief TechmapCmd のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: DumpSbjCmd.h 2274 2009-06-10 07:45:29Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "TechmapCmd.h"


BEGIN_NAMESPACE_MAGUS_TECHMAP

//////////////////////////////////////////////////////////////////////
/// @class DumpSbjCmd DumpSbjCmd "DumpSbjCmd.h"
/// @brief SbjGraph の内容をダンプするコマンド
//////////////////////////////////////////////////////////////////////
class DumpSbjCmd :
  public TechmapCmd
{
public:

  /// @brief コンストラクタ
  DumpSbjCmd(MagMgr* mgr,
	     TechmapData* data);

  /// @brief デストラクタ
  virtual
  ~DumpSbjCmd();


protected:

  /// @brief コマンドを実行する仮想関数
  virtual
  int
  cmd_proc(TclObjVector& objv);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // blif オプションの解析用オブジェクト
  TclPopt* mPoptBlif;

  // verilog オプションの解析用オブジェクト
  TclPopt* mPoptVerilog;

};

END_NAMESPACE_MAGUS_TECHMAP

#endif // TECHMAP_DUMPSBJCMD_H
