
/// @file src/common/AtpgMgr.cc
/// @brief AtpgMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: AtpgMgr.cc 2203 2009-04-16 05:04:40Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "AtpgMgr.h"
#include "ModBase.h"
#include "fsim/Fsim.h"
#include "fsim3/Fsim3.h"
#include "fsimx/FsimX.h"
#include "dtpg_classic/DtpgCl.h"
#include "dtpg_sat/DtpgSat.h"


BEGIN_NAMESPACE_YM_ATPG

//////////////////////////////////////////////////////////////////////
// AtpgMgr
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
AtpgMgr::AtpgMgr() :
  mTimer(TM_SIZE)
{
  mFsim = new Fsim();
  mFsim3 = new Fsim3();
  mFsimX = new FsimX();
  
  bind_module(mFsim);
  bind_module(mFsim3);
  bind_module(mFsimX);
}

// @brief デストラクタ
AtpgMgr::~AtpgMgr()
{
  for (list<ModBase*>::iterator p = mModList.begin();
       p != mModList.end(); ++ p) {
    delete *p;
  }
}

// @brief ネットワークをセットした後に呼ぶ関数
void
AtpgMgr::after_set_network()
{
  TgNetwork& network = _network();
  TvMgr& tv_mgr = _tv_mgr();
  FaultMgr& fmgr = _fault_mgr();
  
  fmgr.clear();
  
  tv_mgr.clear();

  size_t ni = network.input_num2();
  tv_mgr.init(ni);
  
  for (list<ModBase*>::iterator p = mModList.begin();
       p != mModList.end(); ++ p) {
    ModBase* mod = *p;
    mod->after_set_network(network, tv_mgr);
  }
}

// @brief 故障リストの内容が変わった後に呼ぶ関数
void
AtpgMgr::after_update_faults()
{
  const vector<SaFault*>& flist = _fault_mgr().remain_list();
  
  for (list<ModBase*>::iterator p = mModList.begin();
       p != mModList.end(); ++ p) {
    ModBase* mod = *p;
    mod->after_update_faults(flist);
  }
}

// @brief モジュールを登録する．
void
AtpgMgr::bind_module(ModBase* mod)
{
  mModList.push_back(mod);
}

END_NAMESPACE_YM_ATPG
