#ifndef MAIN_TVBUFF_H
#define MAIN_TVBUFF_H

/// @file main/TvBuff.h
/// @brief TvBuff のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
/// 
/// $Id: TvBuff.h 2128 2009-03-12 07:25:33Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "atpg_nsdef.h"
#include "fsim_nsdef.h"


BEGIN_NAMESPACE_YM_ATPG

class AtpgMgr;
class TestVector;

//////////////////////////////////////////////////////////////////////
/// @class TvBuff TvBuff.h "TvBuff.h"
/// @brief TestVector のバッファ
//////////////////////////////////////////////////////////////////////
class TvBuff
{
public:

  /// @brief コンストラクタ
  TvBuff(AtpgMgr* mgr);
  
  /// @brief デストラクタ
  ~TvBuff();


public:
  //////////////////////////////////////////////////////////////////////
  // 公開関数
  //////////////////////////////////////////////////////////////////////

  /// @brief バッファをクリアする．
  void
  clear();
  
  /// @brief パタンをバッファに入れる．
  /// @param[in] tv テストベクタ
  /// @note バッファが一杯になったら故障シミュレーションを行う．
  void
  put(TestVector* tv);

  /// @brief バッファに残っているパタンで故障シミュレーションを行う．
  void
  flush();
  

private:
  //////////////////////////////////////////////////////////////////////
  // 下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 故障シミュレーションを行う．
  void
  do_simulation();

  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 親のマネージャ
  AtpgMgr* mMgr;
  
  // ベクタを貯めておくバッファ
  vector<TestVector*> mBuff;
  
};

END_NAMESPACE_YM_ATPG

#endif // MAIN_TVBUFF_H
