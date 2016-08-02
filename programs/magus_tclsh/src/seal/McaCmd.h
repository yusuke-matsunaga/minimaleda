#ifndef SRC_SEAL_MCACMD_H
#define SRC_SEAL_MCACMD_H

/// @file src/seal/McaCmd.h
/// @brief McaCmd のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: McaCmd.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "BNetCmd.h"
#include "ym_tclpp/TclPopt.h"


BEGIN_NAMESPACE_MAGUS

typedef string State;

//////////////////////////////////////////////////////////////////////
/// @class McaCmd McaCmd.h "McaCmd.h"
/// @brief ソフトエラーの解析を実行するコマンド
//////////////////////////////////////////////////////////////////////
class McaCmd :
  public BNetCmd
{
public:

  /// @brief コンストラクタ
  McaCmd(MagMgr* mgr);

  /// @brief デストラクタ
  virtual
  ~McaCmd();


protected:

  /// @brief コマンド処理関数
  virtual
  int
  cmd_proc(TclObjVector& objv);


private:

  /// @brief KISS ファイルを読み込んで状態を記録する．
  void
  read_kiss(ifstream& fi,
	    vector<State>& init_states);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // -use_bdd オプションの解析用
  TclPopt* mPoptBdd;

  // -dump_trans オプションの解析用
  TclPopt* mPoptDump;

  // -restore オプションの解析用
  TclPoptStr* mPoptRestore;

  // -kiss オプションの解析用
  TclPoptStr* mPoptKiss;

};

END_NAMESPACE_MAGUS

#endif // SRC_SEAL_MCACMD_H
