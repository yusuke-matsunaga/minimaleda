#ifndef SRC_FSIM_RTPGCMD_H
#define SRC_FSIM_RTPGCMD_H

/// @file src/fsim/RtpgCmd.h
/// @brief RtpgCmd のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: RtpgCmd.h 2128 2009-03-12 07:25:33Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "AtpgCmd.h"
#include "Rtpg.h"


BEGIN_NAMESPACE_YM_ATPG

//////////////////////////////////////////////////////////////////////
// 故障シミュレーションを実行するコマンド
//////////////////////////////////////////////////////////////////////
class RtpgCmd :
  public AtpgCmd
{
public:

  /// @brief コンストラクタ
  RtpgCmd(AtpgMgr* mgr);

  /// @brief デストラクタ
  virtual
  ~RtpgCmd();


protected:
  
  // コマンド処理関数
  virtual
  int
  cmd_proc(TclObjVector& objv);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////
  
  // npat オプションの解析用オブジェクト
  TclPoptInt* mPoptNpat;
  
  // min_fault オプションの解析用オブジェクト
  TclPoptInt* mPoptMin;

  // max_interval オプションの解析用オブジェクト
  TclPoptInt* mPoptMax;

  // seed オプションの解析用オブジェクト
  TclPoptUint* mPoptSeed;

  // file オプションの解析用オブジェクト
  TclPoptStr* mPoptFile;

  // print_stats オプションの解析用オブジェクト
  TclPopt* mPoptPrintStats;
  
  // 故障シミュレーションを行うクラス
  Rtpg mRtpg;
  
};

END_NAMESPACE_YM_ATPG

#endif // SRC_FSIM_RTPGCMD_H
