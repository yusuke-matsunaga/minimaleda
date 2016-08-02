#ifndef MAGUS_LOGBASE_ELIMCMD_H
#define MAGUS_LOGBASE_ELIMCMD_H

/// @file magus/logbase/ElimCmd.h
/// @brief ネットワークの中身を操作するコマンドのヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: ElimCmd.h 2274 2009-06-10 07:45:29Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "BNetCmd.h"


BEGIN_NAMESPACE_MAGUS

//////////////////////////////////////////////////////////////////////
/// @class ElimCmd ElimCmd.h "ElimCmd.h"
/// @brief eliminate を行うコマンド
//////////////////////////////////////////////////////////////////////
class ElimCmd :
  public BNetCmd
{
public:

  /// @brief コンストラクタ
  ElimCmd(MagMgr* mgr);

  /// @brief デストラクタ
  virtual
  ~ElimCmd();


protected:

  // コマンド処理関数
  virtual
  int
  cmd_proc(TclObjVector& objv);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // l オプションの解析用オブジェクト
  TclPoptUint* mPoptL;

  // autolimit オプションの解析用オブジェクト
  TclPopt* mPoptAutoLimit;

  // noautolimit オプションの解析用オブジェクト
  TclPopt* mPoptNoAutoLimit;

};


//////////////////////////////////////////////////////////////////////
/// @class CleanCmd ElimCmd.h "ElimCmd.h"
/// @brief clean up (ファンアウト数が0のノードの削除) を行うコマンド
//////////////////////////////////////////////////////////////////////
class CleanCmd :
  public BNetCmd
{
public:

  /// @brief コンストラクタ
  CleanCmd(MagMgr* mgr);

  /// @brief デストラクタ
  virtual
  ~CleanCmd();


protected:

  // コマンド処理関数
  virtual
  int
  cmd_proc(TclObjVector& objv);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // remove_input オプションの解析用オブジェクト
  TclPopt* mPoptRI;

  // preserve_input オプションの解析用オブジェクト
  TclPopt* mPoptPI;

  // remove_latch オプションの解析用オブジェクト
  TclPopt* mPoptRL;

  // preserve_latch オプションの解析用オブジェクト
  TclPopt* mPoptPL;

};


//////////////////////////////////////////////////////////////////////
/// @class SweepCmd ElimCmd.h "ElimCmd.h"
/// @brief sweep (ファンイン数が1以下のノードの削除) を行うコマンド
//////////////////////////////////////////////////////////////////////
class SweepCmd :
  public BNetCmd
{
public:

  /// @brief コンストラクタ
  SweepCmd(MagMgr* mgr);

  /// @brief デストラクタ
  virtual
  ~SweepCmd();


protected:

  // コマンド処理関数
  virtual
  int
  cmd_proc(TclObjVector& objv);

};


//////////////////////////////////////////////////////////////////////
/// @class Simplify ElimCmd.h "ElimCmd.h"
/// @brief simplify を行うコマンド
//////////////////////////////////////////////////////////////////////
class SimplifyCmd :
  public BNetCmd
{
public:

  /// @brief コンストラクタ
  SimplifyCmd(MagMgr* mgr);

  /// @brief デストラクタ
  virtual
  ~SimplifyCmd();


protected:

  // コマンド処理関数
  virtual
  int
  cmd_proc(TclObjVector& objv);

};


//////////////////////////////////////////////////////////////////////
/// @class Decomp ElimCmd.h "ElimCmd.h"
/// @brief decompose を行うコマンド
//////////////////////////////////////////////////////////////////////
class DecompCmd :
  public BNetCmd
{
public:

  /// @brief コンストラクタ
  DecompCmd(MagMgr* mgr);

  /// @brief デストラクタ
  virtual
  ~DecompCmd();


protected:

  // コマンド処理関数
  virtual
  int
  cmd_proc(TclObjVector& objv);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // xor オプションの解析用オブジェクト
  TclPopt* mPoptXor;

  // max_fanin オプションの解析用オブジェクト
  TclPoptInt* mPoptMaxFanin;

  // balanced オプションの解析用オブジェクト
  TclPopt* mPoptBalanced;

  // random オプションの解析用オブジェクト
  TclPopt* mPoptRandom;

  // random_seed オプションの解析用オブジェクト
  TclPoptInt* mPoptRandomSeed;

};


//////////////////////////////////////////////////////////////////////
/// @class GateDecompCmd ElimCmd.h "ElimCmd.h"
/// @brief gate_decompose を行うコマンド
//////////////////////////////////////////////////////////////////////
class GateDecompCmd :
  public BNetCmd
{
public:

  /// @brief コンストラクタ
  GateDecompCmd(MagMgr* mgr);

  /// @brief デストラクタ
  virtual
  ~GateDecompCmd();


protected:

  // コマンド処理関数
  virtual
  int
  cmd_proc(TclObjVector& objv);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // max_fanin オプションの解析用オブジェクト
  TclPoptInt* mPoptMaxFanin;

  // balanced オプションの解析用オブジェクト
  TclPopt* mPoptBalanced;

  // random オプションの解析用オブジェクト
  TclPopt* mPoptRandom;

  // random_seed オプションの解析用オブジェクト
  TclPoptInt* mPoptRandomSeed;

};


//////////////////////////////////////////////////////////////////////
/// @class SopDecompCmd ElimCmd.h "ElimCmd.h"
/// @brief sop_decompose を行うコマンド
//////////////////////////////////////////////////////////////////////
class SopDecompCmd :
  public BNetCmd
{
public:

  /// @brief コンストラクタ
  SopDecompCmd(MagMgr* mgr);

  /// @brief デストラクタ
  virtual
  ~SopDecompCmd();


protected:

  // コマンド処理関数
  virtual
  int
  cmd_proc(TclObjVector& objv);

};


//////////////////////////////////////////////////////////////////////
/// @class ConvCombi ElimCmd.h "ElimCmd.h"
/// @brief 組み合わせ回路化する行うコマンド
//////////////////////////////////////////////////////////////////////
class ConvCombiCmd :
  public BNetCmd
{
public:

  /// @brief コンストラクタ
  ConvCombiCmd(MagMgr* mgr);

  /// @brief デストラクタ
  virtual
  ~ConvCombiCmd();


protected:

  // コマンド処理関数
  virtual
  int
  cmd_proc(TclObjVector& objv);

};

END_NAMESPACE_MAGUS

#endif // MAGUS_LOGBASE_ELIMCMD_H
