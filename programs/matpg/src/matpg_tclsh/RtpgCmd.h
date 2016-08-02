#ifndef SRC_MATPG_RTPGCMD_H
#define SRC_MATPG_RTPGCMD_H

/// @file src/matpg/RtpgCmd.h
/// @brief RtpgCmd のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: RtpgCmd.h 2043 2009-02-21 07:08:48Z matsunaga $
///
/// Copyright (C) 2005-2008 Yusuke Matsunaga
/// All rights reserved.


#include "matpg_nsdef.h"
#include <ym_tclpp/TclCmd.h>


BEGIN_NAMESPACE_YM_MATPG

//////////////////////////////////////////////////////////////////////
// ランダムパタン生成を行うコマンド
//////////////////////////////////////////////////////////////////////
class RtpgCmd :
  public TclCmd
{
public:

  /// @brief コンストラクタ
  RtpgCmd();

  /// @brief デストラクタ
  virtual
  ~RtpgCmd();


protected:

  /// @brief コマンド処理関数
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
  
};

END_NAMESPACE_YM_MATPG

#endif // SRC_MATPG_RTPGCMD_H
