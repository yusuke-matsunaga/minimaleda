#ifndef MAGUS_LOGBASE_NTWKNDCMD_H
#define MAGUS_LOGBASE_NTWKNDCMD_H

/// @file magus/logbase/NtwkNdCmd.h
/// @brief NtwkNdCmd のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: NtwkNdCmd.h 2274 2009-06-10 07:45:29Z matsunaga $
///
/// Copyright (C) 2005-2007 Yusuke Matsunaga
/// All rights reserved.

// ノードに対して処理を行うコマンド

#include "BNetNodeCmd.h"


BEGIN_NAMESPACE_MAGUS


//////////////////////////////////////////////////////////////////////
/// @class NodeInfo NtwkNdCmd.h "NtwkNdCmd.h"
/// @brief ノードの諸元を取り出すコマンド
//////////////////////////////////////////////////////////////////////
class NodeInfo :
  public BNetCmd
{
public:

  /// @brief コンストラクタ
  NodeInfo(MagMgr* mgr);

  /// @brief デストラクタ
  virtual
  ~NodeInfo();


protected:

  /// @brief コマンド処理関数
  virtual
  int
  cmd_proc(TclObjVector& objv);

};


//////////////////////////////////////////////////////////////////////
/// @class ListNode NtwkNdCmd.h "NtwkNdCmd.h"
/// @brief ノード名のリストを作るコマンド
//////////////////////////////////////////////////////////////////////
class ListNode :
  public AllNodeCmd
{
public:

  /// @brief コンストラクタ
  ListNode(MagMgr* mgr);

  /// @brief デストラクタ
  virtual
  ~ListNode();


protected:

  /// @brief mResult をクリアする．
  virtual
  int
  before_node_proc();

  /// @brief node の名前を結果のリストに追加する．
  virtual
  int
  node_proc(BNode* node);

  /// @brief mResult をインタプリタの結果にセットする．
  virtual
  int
  after_node_proc();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 結果のリスト
  TclObj mResult;

};


//////////////////////////////////////////////////////////////////////
/// @class ElimNode NtwkNdCmd.h "NtwkNdCmd.h"
/// @brief ノードの elimination を行う．
//////////////////////////////////////////////////////////////////////
class ElimNode :
  public NodeCmd
{
public:

  /// @brief コンストラクタ
  ElimNode(MagMgr* mgr);

  /// @brief デストラクタ
  virtual
  ~ElimNode();


protected:

  /// @brief name という名のノードを eliminate する．
  virtual
  int
  node_proc(BNode* node);

};


//////////////////////////////////////////////////////////////////////
/// @class DelNode NtwkNdCmd.h "NtwkNdCmd.h"
/// @brief ノードの削除を行う．
//////////////////////////////////////////////////////////////////////
class DelNode :
  public NodeCmd
{
public:

  /// @brief コンストラクタ
  DelNode(MagMgr* mgr);

  /// @brief デストラクタ
  virtual
  ~DelNode();


protected:

  /// @brief name という名前のノードを delete する．
  virtual
  int
  node_proc(BNode* node);

};


//////////////////////////////////////////////////////////////////////
/// @class SimplifyNode NtwkNdCmd.h "NtwkNdCmd.h"
/// @brief ノードの論理式の簡単化を行う．
//////////////////////////////////////////////////////////////////////
class SimplifyNode :
  public AllNodeCmd
{
public:

  /// @brief コンストラクタ
  SimplifyNode(MagMgr* mgr);

  /// @brief デストラクタ
  virtual
  ~SimplifyNode();


protected:

  /// @brief name という名前のノードを simplify する．
  virtual
  int
  node_proc(BNode* node);

};

END_NAMESPACE_MAGUS

#endif // MAGUS_LOGBASE_NTWKNDCMD_H
