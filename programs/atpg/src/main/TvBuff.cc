
/// @file src/main/TvBuff.cc
/// @brief TvBuff の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: TvBuff.cc 2203 2009-04-16 05:04:40Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "TvBuff.h"
#include "AtpgMgr.h"
#include "PackedVal.h"
#include "SaFault.h"
#include "FaultMgr.h"
#include "fsim/Fsim.h"


BEGIN_NAMESPACE_YM_ATPG

// @brief コンストラクタ
TvBuff::TvBuff(AtpgMgr* mgr) :
  mMgr(mgr)
{
  mBuff.reserve(kPvBitLen);
}
  
// @brief デストラクタ
TvBuff::~TvBuff()
{
}
    
// @brief バッファをクリアする．
void
TvBuff::clear()
{
  mBuff.clear();
}
  
// @brief パタンをバッファに入れる．
// @param[in] tv テストベクタ
// @note バッファが一杯になったら故障シミュレーションを行う．
void
TvBuff::put(TestVector* tv)
{
  mBuff.push_back(tv);
  if ( mBuff.size() == kPvBitLen ) {
    do_simulation();
  }
}

// @brief バッファに残っているパタンで故障シミュレーションを行う．
void
TvBuff::flush()
{
  if ( !mBuff.empty() ) {
    do_simulation();
  }
}

// @brief 故障シミュレーションを行う．
void
TvBuff::do_simulation()
{
  FaultMgr& fmgr = mMgr->_fault_mgr();
  Fsim& fsim = mMgr->_fsim();
  MStopWatch& timer = mMgr->_timer();
  int old_timer_id = timer.change(TM_FSIM);
  ymuint n = mBuff.size();
  if ( n == 1 ) {
    list<SaFault*> f_list;
    fsim.run(mBuff[0], f_list);
    for (list<SaFault*>::iterator p = f_list.begin();
	 p != f_list.end(); ++ p) {
      SaFault* f = *p;
      if ( f->status() == kFsUndetected ) {
	fmgr.set_status(f, kFsDetected);
      }
    }
  }
  else {
    vector<list<SaFault*> > f_lists(n);
    fsim.run(mBuff, f_lists);
    for (ymuint i = 0; i < n; ++ i) {
      TestVector* tv = mBuff[i];
      for (list<SaFault*>::iterator p = f_lists[i].begin();
	   p != f_lists[i].end(); ++ p) {
	SaFault* f = *p;
	if ( f->status() == kFsUndetected ) {
	  fmgr.set_status(f, kFsDetected);
	  //fmgr.set_detected(f, tv);
	}
      }
    }
  }
  mBuff.clear();
  timer.change(old_timer_id);
}

END_NAMESPACE_YM_ATPG
