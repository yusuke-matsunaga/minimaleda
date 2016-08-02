#ifndef SRC_SEAL_MCACMD2_H
#define SRC_SEAL_MCACMD2_H

/// @file src/seal/McaCmd2.h
/// @brief McaCmd2 のヘッダファイル
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
/// @class McaCmd2 McaCmd2.h "McaCmd2.h"
/// @brief ソフトエラーの解析を実行するコマンド
//////////////////////////////////////////////////////////////////////
class McaCmd2 :
  public BNetCmd
{
public:

  /// @brief コンストラクタ
  McaCmd2(MagMgr* mgr);

  /// @brief デストラクタ
  virtual
  ~McaCmd2();


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

  // -kiss オプションの解析用
  TclPoptStr* mPoptKiss;

  // -state オプションの解析用
  TclPoptStr* mPoptState;

  // -comb オプションの解析用
  TclPopt* mPoptComb;

  // -ffpo オプションの解析用
  TclPopt* mPoptFFPO;

};

END_NAMESPACE_MAGUS

#endif // SRC_SEAL_MCACMD_H
