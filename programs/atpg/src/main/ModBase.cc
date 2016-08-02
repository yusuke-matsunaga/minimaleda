
/// @file src/ModBase.cc
/// @brief ModBase の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: ModBase.cc 2203 2009-04-16 05:04:40Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ModBase.h"
#include "AtpgMgr.h"


BEGIN_NAMESPACE_YM_ATPG

//////////////////////////////////////////////////////////////////////
// ModBase
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] mgr AtpgMgr 
ModBase::ModBase(AtpgMgr* mgr) :
  mMgr(mgr)
{
  mgr->bind_module(this);
}

// @brief デストラクタ
ModBase::~ModBase()
{
}

// @brief AtpgMgr を取り出す．
AtpgMgr&
ModBase::atpg_mgr() const
{
  return *mMgr;
}

// @brief TgNetwork を取り出す．
TgNetwork&
ModBase::_network()
{
  return mMgr->_network();
}
  
// @brief FaultMgr を取り出す．
FaultMgr&
ModBase::_fault_mgr()
{
  return mMgr->_fault_mgr();
}

// @brief TvMgr を取り出す．
TvMgr&
ModBase::_tv_mgr()
{
  return mMgr->_tv_mgr();
}
  
// @brief Fsm を取り出す．
Fsim&
ModBase::_fsim()
{
  return mMgr->_fsim();
}

// @brief DtpgCl を取り出す．
DtpgCl&
ModBase::_dtpg_cl()
{
  return mMgr->_dtpg_cl();
}

// @brief DtpgSat を取り出す．
DtpgSat&
ModBase::_dtpg_sat()
{
  return mMgr->_dtpg_sat();
}

// @brief ネットワークをセットした後に呼ばれる関数
// @note デフォルトの実装ではなにもしない．
void
ModBase::after_set_network()
{
}

// @brief 故障リストの内容が変わった後に呼ばれる関数
// @note デフォルトの実装ではなにもしない．
void
ModBase::after_update_faults()
{
}

END_NAMESPACE_YM_ATPG
