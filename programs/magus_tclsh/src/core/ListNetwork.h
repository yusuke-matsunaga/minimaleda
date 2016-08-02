#ifndef SRC_CORE_LISTNETWORK_H
#define SRC_CoRE_LISTNETWORK_H

/// @file src/core/ListNetwork.h
/// @brief ListNetwork のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: NtwkBaseCmd.h 2274 2009-06-10 07:45:29Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "MagCmd.h"


BEGIN_NAMESPACE_MAGUS

//////////////////////////////////////////////////////////////////////
/// @class ListNetwork ListNetwork.h "ListNetwork.h"
/// @brief ネットワークのリストを作るコマンド
//////////////////////////////////////////////////////////////////////
class ListNetwork :
  public MagCmd
{
public:

  /// @brief コンストラクタ
  ListNetwork(MagMgr* mgr);

  /// @brief デストラクタ
  virtual
  ~ListNetwork();


protected:

  /// @brief コマンド処理関数
  virtual
  int
  cmd_proc(TclObjVector& objv);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // all オプション解析用のオブジェクト
  TclPopt* mPoptAll;

};

END_NAMESPACE_MAGUS

#endif // SRC_CORE_LISTNETWORK_H
