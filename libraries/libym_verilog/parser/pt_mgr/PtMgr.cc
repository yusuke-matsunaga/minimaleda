
/// @file libym_verilog/parser/PtMgr.cc
/// @brief PtMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: PtMgr.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "PtMgr.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス PtMgr
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] factory Pti オブジェクトのファクトリクラス
PtMgr::PtMgr()
{
}

// @brief デストラクタ
PtMgr::~PtMgr()
{
}

// @brief 今までに生成したインスタンスをすべて破壊する．
void
PtMgr::clear()
{
  mUdpList.clear();
  mModuleList.clear();
  mDefNames.clear();
}  

// @brief 登録されているモジュールのリストを返す．
// @return 登録されているモジュールのリスト
const list<const PtModule*>&
PtMgr::pt_module_list() const
{
  return mModuleList;
}

// @brief 登録されている UDP のリストを返す．
// @return 登録されている UDP のリスト
const list<const PtUdp*>&
PtMgr::pt_udp_list() const
{
  return mUdpList;
}

// @brief インスタンス記述で用いられている名前かどうか調べる．
// @param[in] name 調べる名前
// @return 用いられていたら true を返す．
bool
PtMgr::check_def_name(const char* name) const
{
  if ( mDefNames.count(name) > 0 ) {
    return true;
  }
  return false;
}

// UDP の登録
// @param udp 登録する UDP
void
PtMgr::reg_udp(const PtUdp* udp)
{
  mUdpList.push_back(udp);
}

// モジュールの登録
// @param module 登録するモジュール
void
PtMgr::reg_module(const PtModule* module)
{
  mModuleList.push_back(module);
}

// @brief インスタンス定義名を追加する．
void
PtMgr::reg_defname(const char* name)
{
  mDefNames.insert(name);
}

END_NAMESPACE_YM_VERILOG
