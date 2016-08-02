#ifndef MAGUS_LOGBASE_NTWKBASECMD_H
#define MAGUS_LOGBASE_NTWKBASECMD_H

/// @file magus/logbase/NtwkBaseCmd.h
/// @brief ネットワーク操作関連の主要なコマンドのヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: NtwkBaseCmd.h 2274 2009-06-10 07:45:29Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.

// ネットワークを操作するサブパッケージ

#include "BaseCmd.h"


BEGIN_NAMESPACE_MAGUS

//////////////////////////////////////////////////////////////////////
/// @class NewNtwk NtwkBaseCmd.h "NtwkBaseCmd.h"
/// @brief 新しいネットワークを作成するコマンド
//////////////////////////////////////////////////////////////////////
class NewNtwk :
  public BaseCmd
{
public:

  /// @brief コンストラクタ
  NewNtwk(NetMgr* mgr);

  /// @brief デストラクタ
  virtual
  ~NewNtwk();


protected:
  
  /// @brief コマンド処理関数
  virtual
  int
  cmd_proc(TclObjVector& objv);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // trace オプション解析用オブジェクト
  TclPoptBool* mPoptTrace;

};


//////////////////////////////////////////////////////////////////////
/// @class DelNtwk NtwkBaseCmd.h "NtwkBaseCmd.h"
/// @brief ネットワークを削除するコマンド
//////////////////////////////////////////////////////////////////////
class DelNtwk :
  public BaseCmd
{
public:

  /// @brief コンストラクタ
  DelNtwk(NetMgr* mgr);

  /// @brief デストラクタ
  virtual
  ~DelNtwk();


protected:
  
  /// @brief コマンド処理関数
  virtual
  int
  cmd_proc(TclObjVector& objv);

};


//////////////////////////////////////////////////////////////////////
/// @class ClrNtwk NtwkBaseCmd.h "NtwkBaseCmd.h"
/// @brief ネットワークの内容をクリアするコマンド
//////////////////////////////////////////////////////////////////////
class ClrNtwk :
  public BaseCmd
{
public:

  /// @brief コンストラクタ
  ClrNtwk(NetMgr* mgr);

  /// @brief デストラクタ
  virtual
  ~ClrNtwk();


protected:
  
  /// @brief コマンド処理関数
  virtual
  int
  cmd_proc(TclObjVector& objv);

};


//////////////////////////////////////////////////////////////////////
/// @class CopyNtwk NtwkBaseCmd.h "NtwkBaseCmd.h"
/// @brief コピーを行うコマンド
//////////////////////////////////////////////////////////////////////
class CopyNtwk :
  public BaseCmd
{
public:

  /// @brief コンストラクタ
  CopyNtwk(NetMgr* mgr);

  /// @brief デストラクタ
  virtual
  ~CopyNtwk();


protected:
  
  /// @brief コマンド処理関数
  virtual
  int
  cmd_proc(TclObjVector& objv);

};


//////////////////////////////////////////////////////////////////////
/// @class PushNtwk NtwkBaseCmd.h "NtwkBaseCmd.h"
/// @brief カレントネットワークスタックにプッシュするコマンド
//////////////////////////////////////////////////////////////////////
class PushNtwk :
  public BaseCmd
{
public:

  /// @brief コンストラクタ
  PushNtwk(NetMgr* mgr);

  /// @brief デストラクタ
  virtual
  ~PushNtwk();


protected:
  
  /// @brief コマンド処理関数
  virtual
  int
  cmd_proc(TclObjVector& objv);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // new オプション解析用のオブジェクト
  TclPopt* mPoptNew;
  
};


//////////////////////////////////////////////////////////////////////
/// @class PopNtwk NtwkBaseCmd.h "NtwkBaseCmd.h"
/// @brief カレントネットワークのスタックを元に戻す．
//////////////////////////////////////////////////////////////////////
class PopNtwk :
  public BaseCmd
{
public:

  /// @brief コンストラクタ
  PopNtwk(NetMgr* mgr);

  /// @brief デストラクタ
  virtual
  ~PopNtwk();


protected:
  
  /// @brief コマンド処理関数
  virtual
  int
  cmd_proc(TclObjVector& objv);

};


//////////////////////////////////////////////////////////////////////
/// @class ChgNtwk NtwkBaseCmd.h "NtwkBaseCmd.h"
/// @brief カレントネットワークを変更するコマンド
//////////////////////////////////////////////////////////////////////
class ChgNtwk :
  public BaseCmd
{
public:

  /// @brief コンストラクタ
  ChgNtwk(NetMgr* mgr);

  /// @brief デストラクタ
  virtual
  ~ChgNtwk();


protected:
  
  /// @brief コマンド処理関数
  virtual
  int
  cmd_proc(TclObjVector& objv);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // new オプション解析用のオブジェクト
  TclPopt* mPoptNew;

};


//////////////////////////////////////////////////////////////////////
/// @class CurNtwk NtwkBaseCmd.h "NtwkBaseCmd.h"
/// @brief カレントネットワーク名を返すコマンド
//////////////////////////////////////////////////////////////////////
class CurNtwk :
  public BaseCmd
{
public:

  /// @brief コンストラクタ
  CurNtwk(NetMgr* mgr);

  /// @brief デストラクタ
  virtual
  ~CurNtwk();


protected:
  
  /// @brief コマンド処理関数
  virtual
  int
  cmd_proc(TclObjVector& objv);

};


//////////////////////////////////////////////////////////////////////
/// @class ListNtwk NtwkBaseCmd.h "NtwkBaseCmd.h"
/// @brief ネットワークのリストを作るコマンド
//////////////////////////////////////////////////////////////////////
class ListNtwk :
  public BaseCmd
{
public:

  /// @brief コンストラクタ
  ListNtwk(NetMgr* mgr);

  /// @brief デストラクタ
  virtual
  ~ListNtwk();


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


//////////////////////////////////////////////////////////////////////
/// @class ForNtwk NtwkBaseCmd.h "NtwkBaseCmd.h"
/// @brief ネットワークを一つづつ取り出して処理するコマンド
/// その都度，カレントネットワークを切替える．
//////////////////////////////////////////////////////////////////////
class ForNtwk :
  public BaseCmd
{
public:

  /// @brief コンストラクタ
  ForNtwk(NetMgr* mgr);

  /// @brief デストラクタ
  virtual
  ~ForNtwk();


protected:
  
  /// @brief コマンド処理関数
  virtual
  int
  cmd_proc(TclObjVector& objv);

};

END_NAMESPACE_MAGUS

#endif // MAGUS_LOGBASE_NTWKBASECMD_H
