#ifndef SRC_CALC_SVF_SVFCMD_H
#define SRC_CALC_SVF_SVFCMD_H

/// @file src/calc_svf/SvfCmd.h
/// @brief SvfCmd のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: SvfCmd.h 1920 2008-12-20 15:52:42Z matsunaga $
///
/// Copyright (C) 2005-2008 Yusuke Matsunaga
/// All rights reserved.


#include "SealCmd.h"
#include "CalcSvf.h"


BEGIN_NAMESPACE_YM_SEAL_SVF

//////////////////////////////////////////////////////////////////////
// 可観測性を計算するコマンド
//////////////////////////////////////////////////////////////////////
class SvfCmd :
  public SealCmd
{
public:

  /// @brief コンストラクタ
  SvfCmd(SealMgr* mgr);

  /// @brief デストラクタ
  virtual
  ~SvfCmd();


protected:
  
  // コマンド処理関数
  virtual
  int
  cmd_proc(TclObjVector& objv);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////
  
  // loop オプションの解析用オブジェクト
  TclPoptUint* mPoptLoop;
  
  // time_frame オプションの解析用オブジェクト
  TclPoptUint* mPoptTf;
  
  // trace オプションの解析用オブジェクト
  TclPopt* mPoptTrace;

  // trace_count オプションの解析用オブジェクト
  TclPopt* mPoptTraceCount;
  
  // init オプションの解析用オブジェクト
  TclPopt* mPoptInit;
  
  // dss オプションの解析用オブジェクト
  TclPopt* mPoptDss;
  
  // exact オプションの解析用オブジェクト
  TclPopt* mPoptExact;
  
  // exact2 オプションの解析用オブジェクト
  TclPopt* mPoptExact2;
  
  // pseudo-min オプションの解析用オブジェクト
  TclPopt* mPoptMin;

  // max オプションの解析用オブジェクト
  TclPopt* mPoptMax;
  
  // diff オプションの解析用オブジェクト
  TclPopt* mPoptDiff;
  
  // gate オプションの解析用オブジェクト
  TclPopt* mPoptGate;
  
  CalcSvf mCalc;
  
};

END_NAMESPACE_YM_SEAL_SVF

BEGIN_NAMESPACE_YM_SEAL

/// @brief 故障シミュレーション関係のコマンドを作る
/// @param[in] interp TCL インタープリタ
/// @param[in] mgr Seal の共通データ
/// @param[in] ns 名前空間
int
make_svf_commands(Tcl_Interp* interp,
		  SealMgr* mgr,
		  const string& ns);

END_NAMESPACE_YM_SEAL

#endif // SRC_SVFCMD_H
