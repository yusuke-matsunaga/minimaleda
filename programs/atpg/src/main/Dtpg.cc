
/// @file src/main/Dtpg.cc
/// @brief AtpgMgr の実装ファイル (Dtpgを行う部分)
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: Dtpg.cc 2203 2009-04-16 05:04:40Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "Dtpg.h"
#include "dtpg_classic/DtpgCl.h"
#include "dtpg_sat/DtpgSat.h"
#include "fsim/Fsim.h"
#include "fsim3/Fsim3.h"
#include "fsimx/FsimX.h"
#include "ym_utils/StopWatch.h"

#include "SaFault.h"
#include "FaultMgr.h"
#include "TestVector.h"
#include "TvMgr.h"
#include "TvBuff.h"

#include "ym_tgnet/TgNode.h"


BEGIN_NAMESPACE_YM_ATPG

// DTPG の結果を故障シミュレーションで検証する時に 1 にする．
const int verify_dtpg = 1;


// @brief コンストラクタ
Dtpg::Dtpg(AtpgMgr* mgr) :
  AtpgFunctor(mgr),
  mSat(false),
  mScoping(false),
  mDtpgCl(new DtpgCl()),
  mDtpgSat(new DtpgSat()),
  mDynMode(nsDtpgCl::kDynNone),
  mTvBuff(mgr),
  mTmpTv(NULL)
{
  mgr->bind_module(mDtpgCl);
}

// @brief デストラクタ
Dtpg::~Dtpg()
{
}

// @brief 未検出の故障に対してテストパタン生成を行う．
// @param[in] sim_mode シミュレーションモード
void
Dtpg::operator()(tSimMode sim_mode)
{
  FaultMgr& fmgr = _fault_mgr();
  const vector<SaFault*>& flist = fmgr.remain_list();
  
  if ( flist.empty() ) {
    mStats.mDetectNum = 0;
    mStats.mUntestNum = 0;
    mStats.mAbortNum = 0;
    mStats.mPatNum = 0;
    mStats.mTime = USTime(0.0, 0.0, 0.0);
    return;
  }
  
  TvMgr& tvmgr = _tv_mgr();
  vector<TestVector*>& tv_list = _tv_list();
  Fsim& fsim = _fsim();
  Fsim3& fsim3 = _fsim3();
  FsimX& fsimx = _fsimx();
  MStopWatch& timer = _timer();

  mTmpTv = tvmgr.new_vector();
  
  fsim.set_faults(flist);
  fsim3.set_faults(flist);
  fsimx.set_faults(flist);
  
  int old_tm_id = timer.change(TM_DTPG);
  
  StopWatch local_timer;
  local_timer.start();
  
  ymuint d_num0 = fmgr.det_num();
  ymuint r_num0 = fmgr.untest_num();
  ymuint p_num0 = tv_list.size();
  
  mStats.mDtpgNum = 0;
  
  mTvBuff.clear();
 
  if ( mSat ) {
    dtpg2(sim_mode);
  }
  else {
    dtpg1(sim_mode);
  }
    
  mTvBuff.flush();
  
  fmgr.update();

  tvmgr.delete_vector(mTmpTv);
  
  mStats.mDetectNum = fmgr.det_num() - d_num0;
  mStats.mUntestNum = fmgr.untest_num() - r_num0;
  mStats.mAbortNum = fmgr.remain_num();
  mStats.mPatNum = tv_list.size() - p_num0;
  local_timer.stop();
  mStats.mTime = local_timer.time();
  
  timer.change(old_tm_id);
}

// @brief 未検出の故障に対してテストパタン生成を行う．
// @param[in] sim_mode シミュレーションモード
void
Dtpg::dtpg1(tSimMode sim_mode)
{
  FaultMgr& fmgr = _fault_mgr();
  
  const vector<SaFault*>& flist = fmgr.remain_list();

  mDtpgCl->clear_backtrack_count();
  
  if ( mScoping ) {
    for ( ; ; ) {
      const vector<SaFault*>& flist = fmgr.remain_list();
      SaFault* f0 = NULL;
      vector<SaFault*>::const_iterator p;
      for (p = flist.begin(); p != flist.end(); ++ p) {
	SaFault* f = *p;
	if ( f->status() == kFsUndetected ) {
	  f0 = f;
	  break;
	}
      }
      if ( f0 == NULL ) {
	// 検査すべき故障は残っていない．
	break;
      }

      bool stat = mDtpgCl->activate_1fault(f0);
      if ( stat ) {
	mCurFaults.clear();
	mCurFaults.reserve(flist.size());
	for ( ; p != flist.end(); ++ p) {
	  SaFault* f = *p;
	  if ( f->status() == kFsUndetected &&
	       mDtpgCl->check_fault(f) ) {
	    mCurFaults.push_back(f);
	  }
	}
	pat_gen_sub(sim_mode);
      }
      else {
	fmgr.set_status(f0, kFsUntestable);
      }
      mDtpgCl->deactivate();
    }
  }
  else {
    mDtpgCl->activate_all();
    
    mCurFaults.clear();
    mCurFaults.reserve(flist.size());
    for (vector<SaFault*>::const_iterator p = flist.begin();
	 p != flist.end(); ++ p) {
      mCurFaults.push_back(*p);
    }
    pat_gen_sub(sim_mode);
    
    mDtpgCl->deactivate();
  }
  mStats.mBtNum = mDtpgCl->total_backtrack_count();
}

// @brief 未検出の故障に対してテストパタン生成を行う．
// @param[in] sim_mode シミュレーションモード
void
Dtpg::dtpg2(tSimMode sim_mode)
{
  FaultMgr& fmgr = _fault_mgr();
  vector<TestVector*>& tv_list = _tv_list();
  
  const vector<SaFault*>& flist = fmgr.remain_list();
  
  mCurFaults.clear();
  mCurFaults.reserve(flist.size());
  for (vector<SaFault*>::const_iterator p = flist.begin();
       p != flist.end(); ++ p) {
    mCurFaults.push_back(*p);
  }
  
  mTvBuff.clear();
  
  mStats.mBtNum = 0;
  
  switch ( sim_mode ) {
  case kSimNone:
    for (ymuint i = 0; i < mCurFaults.size(); ++ i) {
      SaFault* f = mCurFaults[i];
      TestVector* cur_tv = dtpg_sub(f);
      if ( cur_tv ) {
	tv_list.push_back(cur_tv);
      }
    }
    break;

  case kSimSingle:
    for (ymuint i = 0; i < mCurFaults.size(); ++ i) {
      SaFault* f = mCurFaults[i];
      TestVector* cur_tv = dtpg_sub(f);
      if ( cur_tv ) {
	mTvBuff.put(cur_tv);
	mTvBuff.flush();
	tv_list.push_back(cur_tv);
      }
    }
    break;

  case kSimPpsfp:
    {
      ymuint cf_num = mCurFaults.size();
      while ( cf_num > 0 ) {
	// ランダムに故障を抜き出す．
	ymuint pos = ymuint(mFselGen.int32() % cf_num);
	SaFault* f = mCurFaults[pos];
	-- cf_num;
	if ( pos < cf_num ) {
	  mCurFaults[pos] = mCurFaults[cf_num];
	}
	
	TestVector* cur_tv = dtpg_sub(f);
	if ( cur_tv ) {
	  mTvBuff.put(cur_tv);
	  tv_list.push_back(cur_tv);
	}
      }
    }
    break;
  }
}

void
Dtpg::pat_gen_sub(tSimMode sim_mode)
{
  vector<TestVector*>& tv_list = _tv_list();
  
  mDetFaults.reserve(mCurFaults.size());
  
  if ( sim_mode == kSimNone || sim_mode == kSimSingle ) {
    for (ymuint i = 0; i < mCurFaults.size(); ++ i) {
      SaFault* f = mCurFaults[i];
      TestVector* cur_tv = dtpg_sub(f);
      if ( cur_tv ) {
	if ( sim_mode == kSimSingle ) {
	  mTvBuff.put(cur_tv);
	  mTvBuff.flush();
	}
	tv_list.push_back(cur_tv);
      }
    }
  }
  else {
    ymuint cf_num = mCurFaults.size();
    while ( cf_num > 0 ) {
      // ランダムに故障を抜き出す．
      ymuint pos = ymuint(mFselGen.int32() % cf_num);
      SaFault* f = mCurFaults[pos];
      -- cf_num;
      if ( pos < cf_num ) {
	mCurFaults[pos] = mCurFaults[cf_num];
      }
      TestVector* cur_tv = dtpg_sub(f);
      if ( cur_tv ) {
	mTvBuff.put(cur_tv);
	tv_list.push_back(cur_tv);
      }
    }
  }
}

// @brief f に対するテストパタン生成を行う．
// @return パタンを返す．
TestVector*
Dtpg::dtpg_sub(SaFault* f)
{
  if ( f->status() != kFsUndetected ) {
    // すでに処理済みになっていた．
    return NULL;
  }
  
  FaultMgr& fmgr = _fault_mgr();
    
  ++ mStats.mDtpgNum;
  tStat stat;
  if ( mSat ) {
    MStopWatch& timer = _timer();
    int old_timer_id = timer.change(TM_SAT);
    TgNetwork& network = _network();
    mDetFaults.clear();
    stat = mDtpgSat->run(network, f, mTmpTv);
    mStats.mBtNum += mDtpgSat->stats().mDecisionNum;
    timer.change(old_timer_id);
  }
  else {
    stat = mDtpgCl->run(f, mCurFaults, mDynMode, mTmpTv, mDetFaults);
  }
  if ( stat == kUntest ) {
    // 検出不能故障
    fmgr.set_status(f, kFsUntestable);
    return NULL;;
  }
  if ( stat == kAbort ) {
    // アボート
    fmgr.set_status(f, kFsAbort);
    return NULL;
  }

  TvMgr& tvmgr = _tv_mgr();
  TestVector* cur_tv = tvmgr.new_vector();
  cur_tv->set_from_random(mPatGen);
  cur_tv->copy(*mTmpTv);

  if ( verify_dtpg ) {
    Fsim3& fsimx = _fsim3();
    list<SaFault*> flist;
    fsimx.run(mTmpTv, flist);

    for (list<SaFault*>::iterator p = flist.begin();
	 p != flist.end(); ++ p) {
      SaFault* f = *p;
      fmgr.set_status(f, kFsDetected);
    }
    if ( f->status() != kFsDetected ) {
      cout << "dtpg error : tv is not a test vector" << endl;
      cout << f->str() << endl;
      mTmpTv->dump_bin(cout);
      cout << endl;
      assert_not_reached(__FILE__, __LINE__);
    }
    for (vector<SaFault*>::iterator p = mDetFaults.begin();
	 p != mDetFaults.end(); ++ p) {
      SaFault* f0 = *p;
      if ( f0->status() != kFsDetected ) {
	cout << "drop_fault error : " << f0->str()
	     << " is not detected" << endl;
	assert_not_reached(__FILE__, __LINE__);
      }
    }
  }

  fmgr.set_status(f, kFsDetected);
  //fmgr.set_detected(f, cur_tv);
  for (vector<SaFault*>::iterator p = mDetFaults.begin();
       p != mDetFaults.end(); ++ p) {
    SaFault* f = *p;
    fmgr.set_status(f, kFsDetected);
    //fmgr.set_detected(f, cur_tv);
  }
  return cur_tv;
}
  
// @brief 直前の実行結果を返す．
const DtpgStats&
Dtpg::stats() const
{
  return mStats;
}

// @brief 現在のバックトラック制限数を得る．
ymuint
Dtpg::backtrack_limit() const
{
  return mDtpgCl->backtrack_limit();
}

// @brief バックトラック制限数を設定する．
// @param[in] limit 設定する制限数
void
Dtpg::set_backtrack_limit(ymuint limit)
{
  mDtpgCl->set_backtrack_limit(limit);
}

// @brief デフォルトのバックトラック制限数を設定する．
void
Dtpg::set_default_backtrack_limit()
{
  mDtpgCl->set_default_backtrack_limit();
}

// @brief Unique Sensitization モードを得る．
int
Dtpg::us_mode() const
{
  return mDtpgCl->us_mode();
}

// @brief Unique Sensitization モードを設定する．
// @param[in] us_mode 設定する Unique Sensitization モードの値
void
Dtpg::set_us_mode(ymuint us_mode)
{
  mDtpgCl->set_us_mode(us_mode);
}
    
// @brief dynamic implication のモードを設定する．
void
Dtpg::set_dyn_mode(nsDtpgCl::tDynMode dyn_mode)
{
  mDynMode = dyn_mode;
}
  
// @brief SAT モードの制御を行う．
void
Dtpg::set_sat(bool flag)
{
  mSat = flag;
}
  
// @brief SAT エンジンの指定を行う．
void
Dtpg::set_sat_mode(int mode)
{
  mDtpgSat->set_mode(mode);
}

// @brief スコーピングの制御を行う．
void
Dtpg::set_scoping(bool flag)
{
  mScoping = flag;
}

// @brief バックトラック数のカウンタをクリアする．
void
Dtpg::clear_backtrack_count()
{
  mDtpgCl->clear_backtrack_count();
}
  
// @brief 今までのバックトラック数の総和を返す．
ymuint
Dtpg::total_backtrack_count() const
{
  return mDtpgCl->total_backtrack_count();
}
  
// @brief verbose level を設定する．
void
Dtpg::set_verbose_level(ymuint vlevel)
{
  mDtpgCl->set_verbose_level(vlevel);
}


//////////////////////////////////////////////////////////////////////
// クラス DtpgStats
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
DtpgStats::DtpgStats()
{
  mDetectNum = 0;
  mUntestNum = 0;
  mAbortNum = 0;
  mPatNum = 0;
  mDtpgNum = 0;
  mBtNum = 0;
}

// @brief デストラクタ
DtpgStats::~DtpgStats()
{
}

// @brief 検出した故障数を得る．
ymuint
DtpgStats::detected_faults() const
{
  return mDetectNum;
}

// @brief 検出不能と判断した故障数を得る．
ymuint
DtpgStats::untestable_faults() const
{
  return mUntestNum;
}

// @brief アボートした故障数を得る．
ymuint
DtpgStats::aborted_faults() const
{
  return mAbortNum;
}

// @brief 生成したパタン数を得る．
ymuint
DtpgStats::generated_patterns() const
{
  return mPatNum;
}
  
// @brief 起動回数を得る．
ymuint
DtpgStats::call_count() const
{
  return mDtpgNum;
}
  
// @brief バックトラック数を得る．
ymuint
DtpgStats::backtrack_count() const
{
  return mBtNum;
}

// @brief 計算時間を得る．
USTime
DtpgStats::time() const
{
  return mTime;
}

END_NAMESPACE_YM_ATPG
