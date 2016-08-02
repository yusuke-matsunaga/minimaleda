
/// @file src/main/Rtpg.cc
/// @brief Fsim の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: Rtpg.cc 2203 2009-04-16 05:04:40Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "Rtpg.h"
#include "fsim/Fsim.h"
#include "SaFault.h"
#include "FaultMgr.h"
#include "TvMgr.h"
#include "ym_utils/StopWatch.h"


BEGIN_NAMESPACE_YM_ATPG

//////////////////////////////////////////////////////////////////////
// クラス Rtpg
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
Rtpg::Rtpg(AtpgMgr* mgr) :
  AtpgFunctor(mgr)
{
}

// @brief デストラクタ
Rtpg::~Rtpg()
{
}

// @brief ランダムパタンを用いた故障シミュレーションを行う．
// @param[in] min_f 1回のシミュレーションで検出する故障数の下限
// @param[in] max_i 故障検出できないシミュレーション回数の上限
// @param[in] max_pat 最大のパタン数
// @param[in] randgen 乱数発生器
void
Rtpg::operator()(ymuint min_f,
		 ymuint max_i,
		 ymuint max_pat)
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
	  if ( f->status() == kFsUndetected ) {
	    fault_mgr.set_status(f, kFsDetected);
	    //fault_mgr.set_detected(f, tv);
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
const RtpgStats&
Rtpg::stats() const
{
  return mStats;
}
  
// @brief パタン生成用の乱数発生器を取り出す．
RandGen&
Rtpg::randgen()
{
  return mPatGen;
}


//////////////////////////////////////////////////////////////////////
// クラス RtpgStats
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
RtpgStats::RtpgStats()
{
  mDetectNum = 0;
  mPatNum = 0;
  mEfPatNum = 0;
}

// @brief デストラクタ
RtpgStats::~RtpgStats()
{
}

// @brief 検出した故障数を得る．
ymuint
RtpgStats::detected_faults() const
{
  return mDetectNum;
}

// @brief シミュレーションしたパタン数を得る．
ymuint
RtpgStats::simulated_patterns() const
{
  return mPatNum;
}

// @brief 有効なパタン数を得る．
ymuint
RtpgStats::effective_patterns() const
{
  return mEfPatNum;
}

// @brief 計算時間を得る．
USTime
RtpgStats::time() const
{
  return mTime;
}

END_NAMESPACE_YM_ATPG
