
/// @file src/main/MinPat.cc
/// @brief Fsim の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: MinPat.cc 2203 2009-04-16 05:04:40Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "MinPat.h"
#include "fsim/Fsim.h"
#include "base/SaFault.h"
#include "base/FaultMgr.h"
#include "base/TvMgr.h"
#include "ym_utils/StopWatch.h"


BEGIN_NAMESPACE_YM_ATPG

//////////////////////////////////////////////////////////////////////
// クラス MinPat
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
MinPat::MinPat(AtpgMgr* mgr) :
  AtpgFunctor(mgr)
{
}

// @brief デストラクタ
MinPat::~MinPat()
{
}

// @brief テストベクタの最小化を行う．
// @param[in] mode モード
void
MinPat::operator()(ymuint mode)
{
  StopWatch local_timer;
  MStopWatch& timer = _timer();

  int old_timer_id = timer.change(TM_FSIM);
  
  local_timer.start();
  
  FaultMgr& fault_mgr = _fault_mgr();
  TvMgr& tv_mgr = _tv_mgr();
  vector<TestVector*>& tv_list = _tv_list();
  Fsim& fsim = _fsim();
  
  fsim.set_faults(fault_mgr.remain_list());
  
  ymuint fnum = fault_mgr.remain_list().size();
  ymuint undet_i = 0;
  ymuint epat_num = 0;
  ymuint total_det_count = 0;
  
  TestVector* tv_array[kPvBitLen];
  for (ymuint i = 0; i < kPvBitLen; ++ i) {
    tv_array[i] = tv_mgr.new_vector();
  }
  
  vector<TestVector*> cur_array;
  cur_array.reserve(kPvBitLen);
  vector<list<SaFault*> > det_faults(kPvBitLen);
  
  ymuint pat_num = 0;
  for ( ; ; ) {
    if ( pat_num < max_pat ) {
      TestVector* tv = tv_array[cur_array.size()];
      tv->set_from_random(mPatGen);
      cur_array.push_back(tv);
      ++ pat_num;
      if ( cur_array.size() < kPvBitLen ) {
	continue;
      }
    }
    else if ( cur_array.empty() ) {
      break;
    }

    fsim.run(cur_array, det_faults);
    
    ymuint det_count = 0;
    for (ymuint i = 0; i < cur_array.size(); ++ i) {
      ymuint det_count1 = det_faults[i].size();
      if ( det_count1 ) {
	det_count += det_count1;
	TestVector* tv = cur_array[i];
	tv_list.push_back(tv);
	tv_array[i] = tv_mgr.new_vector();
	++ epat_num;
	for (list<SaFault*>::iterator p = det_faults[i].begin();
	     p != det_faults[i].end(); ++ p) {
	  SaFault* f = *p;
	  if ( f->status() == kFsNotYet ) {
	    fault_mgr.set_detected(f, tv);
	  }
	}
      }
    }
    cur_array.clear();
    
    fault_mgr.update();
    total_det_count += det_count;
    
    if ( total_det_count == fnum ) {
      // すべての故障を検出した．
      break;
    }
    if ( det_count < min_f ) {
      // 検出故障数の下限を下回った
      break;
    }
    if ( det_count > 0 ) {
      undet_i = 0;
    }
    else {
      ++ undet_i;
      if ( undet_i > max_i ) {
	// 未検出の回数が max_i を越えた．
	break;
      }
    }
  }

  for (ymuint i = 0; i < kPvBitLen; ++ i) {
    tv_mgr.delete_vector(tv_array[i]);
  }

  local_timer.stop();
  USTime time = local_timer.time();

  timer.change(old_timer_id);

  mStats.mDetectNum = total_det_count;
  mStats.mPatNum = pat_num;
  mStats.mEfPatNum = epat_num;
  mStats.mTime = local_timer.time();
}
  
// @brief 直前の実行結果を得る．
const MinPatStats&
MinPat::stats() const
{
  return mStats;
}


//////////////////////////////////////////////////////////////////////
// クラス MinPatStats
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
MinPatStats::MinPatStats()
{
  mPatNum1 = 0;
  mPatNum2 = 0;
}

// @brief デストラクタ
MinPatStats::~MinPatStats()
{
}

// @brief 元のパタン数を得る．
ymuint
MinPatStats::original_patterns() const
{
  return mPatNum1;
}

// @brief 最小化後のパタン数を得る．
ymuint
MinPatStats::minimized_patterns() const
{
  return mPatNum2;
}

// @brief 計算時間を得る．
USTime
MinPatStats::time() const
{
  return mTime;
}

END_NAMESPACE_YM_ATPG
