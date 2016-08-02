#ifndef MAGUS_LUTMAP_LUTMAPCMD2_H
#define MAGUS_LUTMAP_LUTMAPCMD2_H

/// @file magus/lutmap/LutmapCmd.h
/// @brief LutmapCmd のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: LutmapCmd.h 2274 2009-06-10 07:45:29Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "MagCmd.h"
#include "ym_sbj/SbjGraph.h"
#include "ym_lutmap/LnGraph.h"


BEGIN_NAMESPACE_MAGUS

//////////////////////////////////////////////////////////////////////
/// @class LutmapData LutmapCmd.h "LutmapCmd.h"
/// @brief LutmapCmd に共通のデータ
//////////////////////////////////////////////////////////////////////
struct LutmapData
{

  // サブジェクトグラフ
  SbjGraph mSbjGraph;

  // LUTネットワーク
  LnGraph mLutNetwork;

};


//////////////////////////////////////////////////////////////////////
/// @class LutmapCmd LutmapCmd.h "LutmapCmd.h"
/// @brief LUTマッピングを行うコマンドの基底クラス
//////////////////////////////////////////////////////////////////////
class LutmapCmd :
  public MagCmd
{
public:

  /// @brief コンストラクタ
  LutmapCmd(MagMgr* mgr,
	    LutmapData* data);

  /// @brief デストラクタ
  virtual
  ~LutmapCmd();


protected:

  /// @brief SbjGraph を得る．
  SbjGraph&
  sbjgraph();

  /// @brief LutNetwork を得る．
  LnGraph&
  lutnetwork();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  LutmapData* mData;

};


//////////////////////////////////////////////////////////////////////
/// @class Conv2SbjCmd LutmapCmd "LutmapCmd.h"
/// @brief BNetwork から SbjGraph に変換するコマンド
//////////////////////////////////////////////////////////////////////
class Conv2SbjCmd :
  public LutmapCmd
{
public:

  /// @brief コンストラクタ
  Conv2SbjCmd(MagMgr* mgr,
	      LutmapData* data);

  /// @brief デストラクタ
  virtual
  ~Conv2SbjCmd();


protected:

  /// @brief コマンドを実行する仮想関数
  virtual
  int
  cmd_proc(TclObjVector& objv);

};


//////////////////////////////////////////////////////////////////////
/// @class ReadVerilogCmd LutmapCmd "LutmapCmd.h"
/// @brief verilog ファイルを読み込むコマンド
//////////////////////////////////////////////////////////////////////
class ReadVerilogCmd :
  public LutmapCmd
{
public:

  /// @brief コンストラクタ
  ReadVerilogCmd(MagMgr* mgr,
		 LutmapData* data);

  /// @brief デストラクタ
  virtual
  ~ReadVerilogCmd();


protected:

  /// @brief コマンドを実行する仮想関数
  virtual
  int
  cmd_proc(TclObjVector& objv);

};


//////////////////////////////////////////////////////////////////////
/// @class DumpSbjCmd LutmapCmd "LutmapCmd.h"
/// @brief SbjGraph の内容をダンプするコマンド
//////////////////////////////////////////////////////////////////////
class DumpSbjCmd :
  public LutmapCmd
{
public:

  /// @brief コンストラクタ
  DumpSbjCmd(MagMgr* mgr,
	     LutmapData* data);

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


//////////////////////////////////////////////////////////////////////
/// @class DumpLutCmd LutmapCmd "LutmapCmd.h"
/// @brief LutNetwork の内容をダンプするコマンド
//////////////////////////////////////////////////////////////////////
class DumpLutCmd :
  public LutmapCmd
{
public:

  /// @brief コンストラクタ
  DumpLutCmd(MagMgr* mgr,
	     LutmapData* data);

  /// @brief デストラクタ
  virtual
  ~DumpLutCmd();


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


//////////////////////////////////////////////////////////////////////
/// @class NodeSubstCmd LutmapCmd "LutmapCmd.h"
/// @brief LutNetwork のノードの置き換えを行うコマンド
//////////////////////////////////////////////////////////////////////
class NodeSubstCmd :
  public LutmapCmd
{
public:

  /// @brief コンストラクタ
  NodeSubstCmd(MagMgr* mgr,
	       LutmapData* data);

  /// @brief デストラクタ
  virtual
  ~NodeSubstCmd();


protected:

  /// @brief コマンドを実行する仮想関数
  virtual
  int
  cmd_proc(TclObjVector& objv);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // npat オプションの解析用オブジェクト
  TclPoptInt* mPoptNpat;

};


//////////////////////////////////////////////////////////////////////
/// @class NodeMergeCmd LutmapCmd "LutmapCmd.h"
/// @brief LutNetwork のノードの置き換えを行うコマンド
//////////////////////////////////////////////////////////////////////
class NodeMergeCmd :
  public LutmapCmd
{
public:

  /// @brief コンストラクタ
  NodeMergeCmd(MagMgr* mgr,
	       LutmapData* data);

  /// @brief デストラクタ
  virtual
  ~NodeMergeCmd();


protected:

  /// @brief コマンドを実行する仮想関数
  virtual
  int
  cmd_proc(TclObjVector& objv);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // npat オプションの解析用オブジェクト
  TclPoptInt* mPoptNpat;

};


//////////////////////////////////////////////////////////////////////
/// @class Conv2BNetCmd LutmapCmd "LutmapCmd.h"
/// @brief LutNetwork を BNetwork に変換するコマンド
//////////////////////////////////////////////////////////////////////
class Conv2BNetCmd :
  public LutmapCmd
{
public:

  /// @brief コンストラクタ
  Conv2BNetCmd(MagMgr* mgr,
	      LutmapData* data);

  /// @brief デストラクタ
  virtual
  ~Conv2BNetCmd();


protected:

  /// @brief コマンドを実行する仮想関数
  virtual
  int
  cmd_proc(TclObjVector& objv);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // dst_network オプションの解析用オブジェクト
  TclPoptStr* mPoptDstNetwork;

};


//////////////////////////////////////////////////////////////////////
/// @class WriteVqmCmd LutmapCmd "LutmapCmd.h"
/// @brief LutNetwork を VQN に変換するコマンド
//////////////////////////////////////////////////////////////////////
class WriteVqmCmd :
  public LutmapCmd
{
public:

  /// @brief コンストラクタ
  WriteVqmCmd(MagMgr* mgr,
	      LutmapData* data);

  /// @brief デストラクタ
  virtual
  ~WriteVqmCmd();


protected:

  /// @brief コマンドを実行する仮想関数
  virtual
  int
  cmd_proc(TclObjVector& objv);

};


//////////////////////////////////////////////////////////////////////
/// @class GenIlpCmd LutmapCmd "LutmapCmd.h"
/// @brief ILP に変換するコマンド
//////////////////////////////////////////////////////////////////////
class GenIlpCmd :
  public LutmapCmd
{
public:

  /// @brief コンストラクタ
  GenIlpCmd(MagMgr* mgr,
	    LutmapData* data);

  /// @brief デストラクタ
  virtual
  ~GenIlpCmd();


protected:

  /// @brief コマンドを実行する仮想関数
  virtual
  int
  cmd_proc(TclObjVector& objv);

};

END_NAMESPACE_MAGUS

#endif // MAGUS_LUTMAP_LUTMAPCMD_H
