#ifndef LIBYM_NPN_TESTS_TVFUNCTIMECMD_H
#define LIBYM_NPN_TESTS_TVFUNCTIMECMD_H

/// @file libym_npn/tests/TvFuncTimeCmd.h
/// @brief TvFuncTimeCmd のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: TvFuncTimeCmd.h 1508 2008-06-30 04:55:42Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_utils/StopWatch.h"
#include "ym_tclpp/TclCmd.h"
#include "ym_npn/common.h"

#include "TvFuncTest.h"


BEGIN_NAMESPACE_YM_NPN

//////////////////////////////////////////////////////////////////////
/// TvFunc のメンバ関数の時間計測をするコマンド
//////////////////////////////////////////////////////////////////////
class TvFuncTimeCmd :
  public TclCmd
{
public:

  /// @brief コンストラクタ
  TvFuncTimeCmd();

  /// @brief デストラクタ
  ~TvFuncTimeCmd();


protected:

  /// @brief usage を出力する関数
  virtual
  void
  usage();
  
  /// @brief コマンドを実行する仮想関数
  virtual
  int
  cmd_proc(TclObjVector& objv);


private:

  // 関数を取り出す．
  int
  get_tvfunc(TclObj& obj,
	     TvFunc& func);

  // 入力番号を取り出す．
  int
  get_pos(TclObj& obj,
	  size_t ni,
	  unsigned int& pos);


private:

  // 累積計算時間を保持しておく変数
  USTime mAccTime;

  // 実行回数
  size_t mNum;

};


//////////////////////////////////////////////////////////////////////
/// NPN 同値類の正規化を行うコマンド
//////////////////////////////////////////////////////////////////////
class TvFuncNpnCmd :
  public TclCmd
{
public:

  /// @brief コンストラクタ
  TvFuncNpnCmd();

  /// @brief デストラクタ
  ~TvFuncNpnCmd();


protected:
  
  /// @brief コマンドを実行する仮想関数
  virtual
  int
  cmd_proc(TclObjVector& objv);


private:

  // 関数を取り出す．
  int
  get_tvfunc(TclObj& obj,
	     TvFunc& func);


private:
  
  // -mag オプション解析用のオブジェクト
  TclPoptInt* mMagOpt;
  
  // NPN 代表関数を保持しておくハッシュ表
  hash_set<TvFunc> mRepFuncSet;

  // 累積計算時間を保持しておく変数
  USTime mAccTime;

  // 実行回数
  size_t mNum;

};

END_NAMESPACE_YM_NPN

#endif // LIBYM_NPN_TESTS_TVFUNCTIMECMD_H
