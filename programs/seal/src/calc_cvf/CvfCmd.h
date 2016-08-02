#ifndef SRC_CALC_CVF_CVFCMD_H
#define SRC_CALC_CVF_CVFCMD_H

/// @file src/calc_cvf/CvfCmd.h
/// @brief CvfCmd のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: CvfCmd.h 1978 2009-02-06 12:29:16Z matsunaga $
///
/// Copyright (C) 2005-2008 Yusuke Matsunaga
/// All rights reserved.


#include "SealCmd.h"
#include "CalcCvf.h"


BEGIN_NAMESPACE_YM_SEAL_CVF

//////////////////////////////////////////////////////////////////////
// 可観測性を計算するコマンド
//////////////////////////////////////////////////////////////////////
class CvfCmd :
  public SealCmd
{
public:

  /// @brief コンストラクタ
  CvfCmd(SealMgr* mgr);

  /// @brief デストラクタ
  virtual
  ~CvfCmd();


protected:
  
  // コマンド処理関数
  virtual
  int
  cmd_proc(TclObjVector& objv);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////
  
  // loop オプションの介せ器用オブジェクト
  TclPoptUint* mPoptLoop;
  
  // trace オプションの解析用オブジェクト
  TclPopt* mPoptTrace;

  // trace_count オプションの解析用オブジェクト
  TclPopt* mPoptTraceCount;
  
  // init オプションの解析用オブジェクト
  TclPopt* mPoptInit;
  
  // dss オプションの解析用オブジェクト
  TclPopt* mPoptDss;
  
  // init-old-dss オプションの解析用オブジェクト
  TclPopt* mPoptOldDss;
  
  // exact オプションの解析用オブジェクト
  TclPopt* mPoptExact;
  
  // pseudo-min オプションの解析用オブジェクト
  TclPopt* mPoptMin;

  // max オプションの解析用オブジェクト
  TclPopt* mPoptMax;

  // max_sample オプションの解析用オブジェクト
  TclPoptInt* mPoptMaxSample;
  
  // diff オプションの解析用オブジェクト
  TclPopt* mPoptDiff;
  
  // gate オプションの解析用オブジェクト
  TclPopt* mPoptGate;
  
  CalcCvf mCalc;
  
};

END_NAMESPACE_YM_SEAL_CVF

BEGIN_NAMESPACE_YM_SEAL

/// @brief 故障シミュレーション関係のコマンドを作る
/// @param[in] interp TCL インタープリタ
/// @param[in] mgr Seal の共通データ
/// @param[in] ns 名前空間
int
make_cvf_commands(Tcl_Interp* interp,
		  SealMgr* mgr,
		  const string& ns);

END_NAMESPACE_YM_SEAL

#endif // SRC_CVFCMD_H
