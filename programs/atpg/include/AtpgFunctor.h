#ifndef MAIN_ATPGFUNCTOR_H
#define MAIN_ATPGFUNCTOR_H

/// @file src/main/AtpgFunctor.h
/// @brief AtpgFunctor のヘッダファイル
///
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: AtpgFunctor.h 2203 2009-04-16 05:04:40Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "AtpgMgr.h"


BEGIN_NAMESPACE_YM_ATPG

//////////////////////////////////////////////////////////////////////
/// @class AtpgFunctor AtpgFunctor.h "AtpgFunctor.h"
/// @brief ATPG の処理を行うファンクターの基底クラス
//////////////////////////////////////////////////////////////////////
class AtpgFunctor
{
public:

  /// @brief コンストラクタ
  /// @param[in] mgr AtpgMgr
  AtpgFunctor(AtpgMgr* mgr);

  /// @brief デストラクタ
  virtual
  ~AtpgFunctor() { }

  
protected:
  //////////////////////////////////////////////////////////////////////
  // 継承クラスのためのアクセス関数
  //////////////////////////////////////////////////////////////////////
  
  /// @brief TgNetwork を取り出す．
  TgNetwork&
  _network();
  
  /// @brief FaultMgr を取り出す．
  FaultMgr&
  _fault_mgr();

  /// @brief TvMgr を取り出す．
  TvMgr&
  _tv_mgr();
  
  /// @brief テストベクタのリストを取り出す．
  vector<TestVector*>&
  _tv_list();
  
  /// @brief Fsim を取り出す．
  Fsim&
  _fsim();
  
  /// @brief Fsim を取り出す．
  Fsim3&
  _fsim3();
  
  /// @brief FsimX を取り出す．
  FsimX&
  _fsimx();
  
  /// @brief タイマーを取り出す．
  MStopWatch&
  _timer();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////
  
  // 親のマネージャ
  AtpgMgr* mMgr;
  
};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] mgr AtpgMgr
inline
AtpgFunctor::AtpgFunctor(AtpgMgr* mgr) :
  mMgr(mgr)
{
}

// @brief TgNetwork を取り出す．
inline
TgNetwork&
AtpgFunctor::_network()
{
  return mMgr->_network();
}

// @brief FaultMgr を取り出す．
inline
FaultMgr&
AtpgFunctor::_fault_mgr()
{
  return mMgr->_fault_mgr();
}

// @brief TvMgr を取り出す．
inline
TvMgr&
AtpgFunctor::_tv_mgr()
{
  return mMgr->_tv_mgr();
}
  
// @brief テストベクタのリストを取り出す．
inline
vector<TestVector*>&
AtpgFunctor::_tv_list()
{
  return mMgr->_tv_list();
}

// @brief Fsim を取り出す．
inline
Fsim&
AtpgFunctor::_fsim()
{
  return mMgr->_fsim();
}

// @brief Fsim を取り出す．
inline
Fsim3&
AtpgFunctor::_fsim3()
{
  return mMgr->_fsim3();
}

// @brief FsimX を取り出す．
inline
FsimX&
AtpgFunctor::_fsimx()
{
  return mMgr->_fsimx();
}
  
// @brief タイマーを取り出す．
inline
MStopWatch&
AtpgFunctor::_timer()
{
  return mMgr->_timer();
}

END_NAMESPACE_YM_ATPG

#endif // MAIN_ATPGFUNCTOR_H
