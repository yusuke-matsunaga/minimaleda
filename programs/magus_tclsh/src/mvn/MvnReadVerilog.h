#ifndef SRC_LOGBASE_MVNREADVERILOG_H
#define SRC_LOGBASE_MVNREADVERILOG_H

/// @file src/logbase/MvnReadVerilog.h
/// @brief MvnReadVerilog のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: Lutmap.h 2274 2009-06-10 07:45:29Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "MvnCmd.h"


BEGIN_NAMESPACE_MAGUS

//////////////////////////////////////////////////////////////////////
/// @class MvnReadVerilog MvnReadVerilog "MvnReadVerilog"
/// @brief verilog ファイルを読み込むコマンド
//////////////////////////////////////////////////////////////////////
class MvnReadVerilog :
  public MvnCmd
{
public:

  /// @brief コンストラクタ
  MvnReadVerilog(MagMgr* mgr);

  /// @brief デストラクタ
  virtual
  ~MvnReadVerilog();


protected:

  /// @brief コマンドを実行する仮想関数
  virtual
  int
  cmd_proc(TclObjVector& objv);

};

END_NAMESPACE_MAGUS

#endif // SRC_LOGBASE_MVNREADVERILOG_H
