#ifndef SRC_CORE_CHGNETWORK_H
#define SRC_CORE_CHGNETWORK_H

/// @file src/core/ChgNetwork.h
/// @brief ChgNetwork のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: NtwkBaseCmd.h 2274 2009-06-10 07:45:29Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "MagCmd.h"


BEGIN_NAMESPACE_MAGUS

//////////////////////////////////////////////////////////////////////
/// @class ChgNetwork ChgNetwork.h "ChgNetwork.h"
/// @brief カレントネットワークを変更するコマンド
//////////////////////////////////////////////////////////////////////
class ChgNetwork :
  public MagCmd
{
public:

  /// @brief コンストラクタ
  ChgNetwork(MagMgr* mgr);

  /// @brief デストラクタ
  virtual
  ~ChgNetwork();


protected:

  /// @brief コマンド処理関数
  virtual
  int
  cmd_proc(TclObjVector& objv);


protected:
  //////////////////////////////////////////////////////////////////////
  // 継承クラスが用いる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief -new_bnet, -new_bdn オプションの処理を行う．
  /// @param[in] name ネットワーク名
  int
  new_opt(string name);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // new_bnet オプション解析用のオブジェクト
  TclPopt* mPoptNewBNet;

  // new_bdn オプション解析用のオブジェクト
  TclPopt* mPoptNewBdn;

  // new_mvn オプション解析用のオブジェクト
  TclPopt* mPoptNewMvn;

};

END_NAMESPACE_MAGUS

#endif // SRC_CORE_CHGNETWORK_H
