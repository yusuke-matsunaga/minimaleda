#ifndef TECHMAP_DUMPCNCMD_H
#define TECHMAP_DUMPCNCMD_H

/// @file magus/techmap/DumpCnCmd.h
/// @brief DumpCnCmd のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: DumpCnCmd.h 2274 2009-06-10 07:45:29Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "TechmapCmd.h"


BEGIN_NAMESPACE_MAGUS_TECHMAP

//////////////////////////////////////////////////////////////////////
/// @class DumpCnCmd DumpCnCmd "DumpCnCmd.h"
/// @brief CnGraph の内容をダンプするコマンド
//////////////////////////////////////////////////////////////////////
class DumpCnCmd :
  public TechmapCmd
{
public:

  /// @brief コンストラクタ
  DumpCnCmd(MagMgr* mgr,
	    TechmapData* data);

  /// @brief デストラクタ
  virtual
  ~DumpCnCmd();


protected:

  /// @brief コマンドを実行する仮想関数
  virtual
  int
  cmd_proc(TclObjVector& objv);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // verilog オプションの解析用オブジェクト
  TclPopt* mPoptVerilog;

};

END_NAMESPACE_MAGUS_TECHMAP

#endif // TECHMAP_DUMPCNCMD_H
